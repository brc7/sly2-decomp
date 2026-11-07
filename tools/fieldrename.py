#!/usr/bin/env python3
"""Rename struct/class fields with the compiler as the oracle, and prove the objects did not change.

  fieldrename.py --csv PLAN.csv --header clock.h [--struct CLOCK] [--fields t,dt] [--min-confidence high]
  fieldrename.py --csv PLAN.csv --header so.h --dry-run        # print the planned edit sites, change nothing

PLAN.csv columns: header,struct,offset,old_name,new_name,confidence,basis (reports/names_r1/SHARED_FIELDS_REVIEWED.csv).
One run = one batch; all of it is applied or none of it (the tree is restored on any failure, and on Ctrl-C).

How it works
  1. Shadow guard (refuses the row): the new name is a keyword or a #define, already a token inside the body of S, an
     ancestor or a descendant (members, parameters, locals of inline methods; a TU-private ancestor view counts only
     when a same-file view of S or a descendant derives from it, a base-less descendant view not at all), or a free
     (unqualified) token in an out-of-line X::method body of S or a descendant; or two rows of the batch clash.
  2. Declarations: the field token is renamed in every definition of S that declares the old name: the header's and
     every TU-private view with the same unqualified name (namespaced views included); also `OFFSETOF(S, old)` and
     `assert_S_old` in the header. Every edit is a token swap inside one line (line numbers feed the path-hash quirk).
  3. Uses: every TU that includes an edited file is compiled with matchtool's compile path. Each diagnostic
     "`struct T' has no member named `old'" (T = S, a descendant, or a view of either), "`old' undeclared" (implicit
     this) or "does not have any field named `old'" (mem-initializer) rewrites the matching tokens on that line only.
     g++ 2.95 prints one message per access, so the count must equal the candidate tokens on the line: fewer messages
     means another struct's field of the same name shares the line (each subset is tried with a recompile); more
     means a macro body (a function-like #define in the same file has its `.old`/`->old` tokens rewritten; any other
     macro is refused: fix it by hand) or, for a single token in a header, one template body instantiated several
     times. Repeat until every TU compiles (cap --rounds).
  4. Verify: every TU in scope was compiled before the edits; after, each function's words and relocations, each data
     symbol's bytes, and the object's symbol set must be identical. Optional --check: matchtool status per file too.
Blind spots (warned, never edited): member tokens in uninstantiated template bodies and in #define bodies of the
touched headers, and TUs this machine cannot build (refused unless --allow-unverifiable). Comments are not touched.
Exit: 0 applied (or dry run ok), 1 refused/reverted.
"""
import argparse, contextlib, csv, io, itertools, json, os, re, subprocess, sys
from concurrent.futures import ProcessPoolExecutor

ROOT = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
sys.path.insert(0, os.path.join(ROOT, 'tools'))
from rename import blank_non_code          # comment/string-aware lexer (keeps offsets)
import matchtool as mt

SRC_EXTS = ('.c', '.cpp', '.h', '.hpp', '.inc')
KEYWORDS = set('''asm auto bool break case catch char class const continue default delete do double else enum explicit
extern false float for friend goto if inline int long mutable namespace new operator private protected public register
return short signed sizeof static struct switch template this throw true try typedef typename union unsigned using
virtual void volatile while'''.split())
CONF = {'low': 0, 'med': 1, 'high': 2}
DIAG = re.compile(r'^(.+?):(\d+): (.*)$')

def tok(name): return re.compile(r'(?<![A-Za-z0-9_$])%s(?![A-Za-z0-9_$])' % re.escape(name))

class Fail(Exception): pass

# ---------------------------------------------------------------------------------------------------------- repo model
class Repo:
    def __init__(self):
        self.text = {}
        for top in ('src', 'include'):
            for dp, _, fs in os.walk(os.path.join(ROOT, top)):
                for f in fs:
                    if f.endswith(SRC_EXTS):
                        p = os.path.relpath(os.path.join(dp, f), ROOT)
                        self.text[p] = open(os.path.join(ROOT, p), errors='surrogateescape').read()
        self.code = {}
        self.orig = {}
        self._index()

    def code_of(self, p):
        if p not in self.code: self.code[p] = blank_non_code(self.text[p])
        return self.code[p]

    def write(self, p, new):
        if p not in self.orig: self.orig[p] = self.text[p]
        assert new.count('\n') == self.text[p].count('\n'), p   # token swaps only: line numbers must not move
        self.text[p] = new; self.code.pop(p, None)
        if hasattr(self, 'defs'): self.parse_defs(p)
        with open(os.path.join(ROOT, p), 'w', errors='surrogateescape', newline='') as f: f.write(new)

    def restore(self):
        for p, t in self.orig.items():
            with open(os.path.join(ROOT, p), 'w', errors='surrogateescape', newline='') as f: f.write(t)
            self.text[p] = t; self.code.pop(p, None); self.parse_defs(p)
        self.orig = {}

    def _index(self):
        """struct definitions (name -> [(path, open, close, bases)]) and the include graph."""
        self.defs = {}; self.incl = {}
        by_tail = {}
        for p in self.text: by_tail.setdefault(os.path.basename(p), []).append(p)
        for p, t in self.text.items():
            self.parse_defs(p)
            incs = set()
            for inc in re.findall(r'^\s*#\s*include\s*[<"]([^>"]+)[>"]', t, re.M):
                for cand in (os.path.normpath(os.path.join(os.path.dirname(p), inc)), os.path.join('include', inc)):
                    if cand in self.text: incs.add(cand); break
                else:
                    tails = [q for q in by_tail.get(os.path.basename(inc), []) if q.endswith(inc)]
                    if len(tails) == 1: incs.add(tails[0])
            self.incl[p] = incs

    def parse_defs(self, p):
        """(Re)index the struct definitions of one file (offsets move after an edit)."""
        for n in self.defs: self.defs[n] = [d for d in self.defs[n] if d[0] != p]
        c = self.code_of(p)
        for m in re.finditer(r'\b(?:struct|class|union)\s+(\w+)\s*(:[^{};()]*)?\{', c):
            bases = [b.split('::')[-1] for b in re.findall(r'([\w:]+)\s*(?:,|$)', (m.group(2) or ':')[1:].strip())]
            self.defs.setdefault(m.group(1), []).append((p, m.end() - 1, match_brace(c, m.end() - 1), bases))

    def tus_including(self, files):
        """.c/.cpp files that are in `files` or include one of them (transitively)."""
        files = set(files); rev = {}
        for p, incs in self.incl.items():
            for i in incs: rev.setdefault(i, set()).add(p)
        seen = set(files); todo = list(files)
        while todo:
            for q in rev.get(todo.pop(), ()):
                if q not in seen: seen.add(q); todo.append(q)
        return sorted(p for p in seen if p.endswith(('.c', '.cpp')))

    def closure(self, tu):
        seen = {tu}; todo = [tu]
        while todo:
            for q in self.incl.get(todo.pop(), ()):
                if q not in seen: seen.add(q); todo.append(q)
        return seen

    def hierarchy(self, name):
        """(ancestors, descendants) by unqualified name over every definition in the tree."""
        up = {}; down = {}
        for n, ds in self.defs.items():
            for d in ds:
                for b in d[3]: up.setdefault(n, set()).add(b); down.setdefault(b, set()).add(n)
        def walk(g):
            seen = set(); todo = [name]
            while todo:
                for q in g.get(todo.pop(), ()):
                    if q not in seen and q != name: seen.add(q); todo.append(q)
            return seen
        return walk(up), walk(down)

    def local_bases(self, p, names):
        """Names reached from the definitions of `names` in file p through bases that p itself defines."""
        here = {n: [b for d in ds if d[0] == p for b in d[3]] for n, ds in self.defs.items() if any(d[0] == p for d in ds)}
        seen = set(); todo = [n for n in names if n in here]
        while todo:
            for b in here.get(todo.pop(), ()):
                if b in here and b not in seen: seen.add(b); todo.append(b)
        return seen

def match_brace(c, i):
    d = 0
    for j in range(i, len(c)):
        if c[j] == '{': d += 1
        elif c[j] == '}':
            d -= 1
            if not d: return j
    return len(c)

def member_spans(c, a, b):
    """Spans of struct body c[a+1:b] that hold member declarations of this struct: method bodies and named nested
    types are cut out, anonymous struct/union bodies are kept (their members belong to the struct)."""
    spans = []; i = a + 1; start = i
    while i < b:
        if c[i] == '{':
            j = match_brace(c, i); head = c[start:i].rstrip()
            anon = re.search(r'\b(struct|union)\s*$', head)
            spans.append((start, i))
            if anon: spans += member_spans(c, i, j)
            i = start = j + 1; continue
        i += 1
    spans.append((start, b))
    return spans

def field_decls(c, a, b, name):
    """Offsets of `name` declared as a data member in the struct body c[a:b]."""
    out = []
    for s, e in member_spans(c, a, b):
        seg = c[s:e]
        for m in tok(name).finditer(seg):
            pre = seg[:m.start()].rstrip(); post = seg[m.end():].lstrip()
            if not post or post[0] not in ';,[:=)' or post.startswith('::') or pre.endswith(('.', '->', '::')): continue
            depth = pre.count('(') - pre.count(')')
            if depth and not re.search(r'\(\s*\*\s*$', pre): continue      # method parameter, not (*pfn) field
            out.append(s + m.start())
    return out

def line_span(t, line):
    a = 0
    for _ in range(line - 1):
        a = t.index('\n', a) + 1
    b = t.find('\n', a)
    return a, (len(t) if b < 0 else b)

def swap(t, offs, old, new):
    for o in sorted(offs, reverse=True):
        assert t[o:o + len(old)] == old
        t = t[:o] + new + t[o + len(old):]
        # keep a same-line trailing comment in its column: absorb the length change in the gap before it
        e = t.find('\n', o); e = len(t) if e < 0 else e
        m = re.search(r'(\S)(  +)(/\*|//)', t[o + len(new):e])
        if m and '/*' not in t[o + len(new):o + len(new) + m.start(2)] and '"' not in t[o + len(new):e]:
            a = o + len(new) + m.start(2); gap = len(m.group(2)) - (len(new) - len(old))
            t = t[:a] + ' ' * max(gap, 1) + t[a + len(m.group(2)):]
    return t

# ------------------------------------------------------------------------------------------------------------ compile
def _compile(tu):
    buf = io.StringIO()
    try:
        with contextlib.redirect_stdout(buf): obj = mt.compile_src(tu)
        return tu, obj, ''
    except SystemExit:
        return tu, None, buf.getvalue()
    except Exception as e:   # noqa: BLE001 - report, never crash the batch midway
        return tu, None, 'fieldrename: %r' % e

def stable(name):
    # g++ names static constructors/destructors after the temp file (_GLOBAL_$I$<path><random>): not a real change
    return re.sub(r'^(_GLOBAL_\$[A-Z]\$).*', r'\1*', name)

def signature(obj):
    d, secs, syms, relocs = mt.read_obj(obj)
    nm = sorted((stable(s['name']), s['shndx'] == 0, s['type']) for s in syms if s['name'] and s['type'] not in (3, 4))
    data = {stable(k): (v[0], v[1], sorted(v[2].items()), v[3]) for k, v in mt.obj_data(obj).items()}
    funcs = {stable(k): (w, sorted(r.items())) for k, (w, r) in mt.obj_functions(obj).items()}
    os.unlink(obj)
    return dict(funcs=funcs, data=data, nm=nm)

def compile_all(tus, jobs):
    with ProcessPoolExecutor(jobs) as ex:
        return {tu: (obj, log) for tu, obj, log in ex.map(_compile, sorted(tus))}

# ------------------------------------------------------------------------------------------------------------- driver
class Batch:
    def __init__(self, repo, rows, args):
        self.repo, self.rows, self.args = repo, rows, args
        self.sites = []          # (path, line, old, new, kind)
        self._closure = {}
        self.warnings = []

    def log(self, msg): print(msg, flush=True)

    # 1. shadow guard ---------------------------------------------------------------------------------------------
    def guard(self):
        r = self.repo; keep = []
        macros = set(re.findall(r'^\s*#\s*define\s+(\w+)', '\n'.join(r.text.values()), re.M))
        for row in self.rows:
            S, old, new = row['struct'], row['old_name'], row['new_name']
            anc, desc = r.hierarchy(S); hier = {S} | anc | desc
            why = None
            if not re.match(r'^[A-Za-z_]\w*$', new) or new in KEYWORDS: why = 'not a usable identifier'
            elif new in macros: why = 'a #define of that name exists'
            elif S not in r.defs: why = 'no definition of %s found' % S
            else:
                hdr = [d for d in r.defs[S] if os.path.basename(d[0]) == row['header']]
                n = sum(len(field_decls(r.code_of(p), a, b, old)) for p, a, b, _ in hdr)
                if n != 1: why = '%d declarations of %s::%s in %s (need exactly 1)' % (n, S, old, row['header'])
            if not why:
                for n in hier:
                    for p, a, b, _ in r.defs.get(n, ()):
                        if n == S and not field_decls(r.code_of(p), a, b, old): continue   # unrelated view, not edited
                        if not p.startswith('include/'):
                            # A TU-private view of a descendant without bases is a separate type; a TU-private view of
                            # an ancestor is S's ancestor only when a view of S or a descendant in the same file reaches
                            # it through bases defined in that file.
                            if n in desc and not _: continue
                            if n in anc and n not in r.local_bases(p, {S} | desc): continue
                        if tok(new).search(r.code_of(p)[a:b]): why = '`%s` already appears in the body of %s (%s)' % (new, n, p)
                        if why: break
                    if why: break
            if not why:
                # Only S's and its descendants' methods see S's members through implicit this; `x.new`, `p->new`
                # and `X::new` in their bodies are qualified accesses, not free uses.
                meth = re.compile(r'\b(%s)\s*::\s*~?\w+\s*\(' % '|'.join(map(re.escape, sorted({S} | desc))))
                for p in r.text:
                    c = r.code_of(p)
                    if new not in c: continue
                    for m in meth.finditer(c):
                        k = c.find('{', m.end()); semi = c.find(';', m.end())
                        if k < 0 or (0 <= semi < k): continue
                        body = c[k:match_brace(c, k)]
                        if any(not body[:t.start()].rstrip().endswith(('.', '->', '::')) for t in tok(new).finditer(body)):
                            why = '`%s` is used inside %s::... (%s:%d): implicit-this shadowing' % (new, m.group(1), p, c[:k].count('\n') + 1)
                            break
                    if why: break
            if not why:
                for o in keep:
                    same = o['struct'] in hier or S in r.hierarchy(o['struct'])[0] | r.hierarchy(o['struct'])[1]
                    if same and (o['new_name'] in (new, old) or o['old_name'] == new):
                        why = 'clashes with %s.%s -> %s in this batch' % (o['struct'], o['old_name'], o['new_name'])
            if why: self.log('REFUSED %s.%s -> %s: %s' % (S, old, new, why)); row['status'] = 'refused: ' + why
            else: row['hier'] = {S} | desc; keep.append(row)
        self.rows = keep

    # 2. declarations ---------------------------------------------------------------------------------------------
    def declare(self):
        r = self.repo; edited = set()
        # function-like macros called as M(S, field): offsetof helpers (SLY2_OFFSETOF, TU-local OFF(t, m), ...)
        fmacros = set(re.findall(r'^[ \t]*#[ \t]*define[ \t]+(\w+)\(\s*\w+\s*,\s*\w+\s*\)', '\n'.join(r.text.values()), re.M)) | {'offsetof'}
        for row in self.rows:
            S, old, new = row['struct'], row['old_name'], row['new_name']
            for p in sorted({d[0] for d in r.defs[S]}):
                c = r.code_of(p)
                offs = [o for q, a, b, _ in r.defs[S] if q == p for o in field_decls(c, a, b, old)]
                for o in offs: self.site(p, r.text[p][:o].count('\n') + 1, old, new, 'decl')
                if offs: r.write(p, swap(r.text[p], offs, old, new)); edited.add(p)
            for p in list(r.text):
                c = r.code_of(p)
                pat = re.compile(r'\b(%s)\s*\(\s*(?:struct\s+)?(%s)\s*,\s*(%s)\s*\)' % ('|'.join(map(re.escape, sorted(fmacros))),
                                 '|'.join(map(re.escape, sorted(row['hier']))), re.escape(old)))
                offs = [m.start(3) for m in pat.finditer(c)]
                at = [m.start() for m in tok('assert_%s_%s' % (S, old)).finditer(c)]
                if not offs and not at: continue
                t = swap(r.text[p], offs, old, new)
                t = swap(t, [o + len('assert_%s_' % S) for o in at], old, new)
                for o in offs: self.site(p, r.text[p][:o].count('\n') + 1, old, new, 'offsetof')
                for o in at: self.site(p, r.text[p][:o].count('\n') + 1, old, new, 'assert')
                r.write(p, t); edited.add(p)
        return edited

    def site(self, p, line, old, new, kind):
        self.sites.append((p, line, old, new, kind))

    # 3. uses -----------------------------------------------------------------------------------------------------
    def phys(self, tu, fname, line, token):
        """Map a diagnostic (file, line) to a physical (path, line), undoing `#line N "path"` pins."""
        f = re.sub(r'^[A-Za-z]:', '', fname.replace('\\', '/'))
        if f.startswith(ROOT + '/'): f = f[len(ROOT) + 1:]
        f = os.path.normpath(f)
        if tu not in self._closure: self._closure[tu] = sorted(self.repo.closure(tu))
        cands = []
        for p in self._closure[tu]:
            t = self.repo.text[p]
            # regions: (first physical line, reported line of that first line, reported name)
            regions = [(1, 1, p)] + [(t[:m.start()].count('\n') + 2, int(m.group(1)), os.path.normpath(m.group(2)))
                                     for m in re.finditer(r'^[ \t]*#[ \t]*line[ \t]+(\d+)[ \t]+"([^"]+)"', t, re.M)]
            for k, (first, rep, name) in enumerate(regions):
                end = regions[k + 1][0] - 1 if k + 1 < len(regions) else t.count('\n') + 1
                q = first + line - rep
                if name == f and first <= q <= end: cands.append((p, q))
        good = [c for c in cands if tok(token).search(self.line_code(*c))]
        return good[0] if len(good) == 1 else (cands[0] if len(cands) == 1 else None)

    def line_code(self, p, line):
        if p not in self.repo.text: return ''
        a, b = line_span(self.repo.text[p], line)
        return self.repo.code_of(p)[a:b]

    def parse(self, tu, log):
        """[(path, line, kind, type, name)]; kind in member|bare|init|other."""
        out = []
        olds = {r['old_name'] for r in self.rows}; news = {r['new_name'] for r in self.rows}
        for ln in log.splitlines():
            m = DIAG.match(ln)
            if not m or m.group(3).startswith(('warning:', '(Each undeclared', 'for each function it appears')): continue
            f, line, msg = m.group(1), int(m.group(2)), m.group(3)
            if msg.startswith(('In function', 'In method', 'At top level')): continue
            mm = re.match(r"`(?:struct|class|union) (.+)' has no member named `(\w+)'", msg) or \
                 re.match(r"(?:structure|union)() has no member named `(\w+)'", msg)
            mu = re.match(r"`(\w+)' undeclared", msg)
            mi = re.match(r"class `(.+)' does not have any field named `(\w+)'", msg)
            if mm and mm.group(2) in olds: kind, ty, name = 'member', mm.group(1), mm.group(2)
            elif mu and mu.group(1) in olds: kind, ty, name = 'bare', None, mu.group(1)
            elif mi and mi.group(2) in olds: kind, ty, name = 'init', mi.group(1), mi.group(2)
            else:
                hit = [n for n in news if tok(n).search(msg)]
                out.append((f, line, 'new' if hit else 'other', None, msg)); continue
            pl = self.phys(tu, f, line, name)
            if not pl: raise Fail('cannot locate %s:%d in the sources (%s)' % (f, line, msg))
            out.append((pl[0], pl[1], kind, ty, name))
        return out

    def row_for(self, name, ty):
        rows = [r for r in self.rows if r['old_name'] == name]
        if ty:
            t = re.sub(r'<.*>', '', ty).split('::')[-1].strip()
            rows = [r for r in rows if t in r['hier']]
        news = {r['new_name'] for r in rows}
        if len(news) != 1:
            raise Fail('%s access to `%s` on type %s: %s' % ('no row for' if not news else 'ambiguous', name, ty, sorted(news)))
        return rows[0]

    def candidates(self, p, line, name, kind):
        a, _ = line_span(self.repo.text[p], line); c = self.line_code(p, line)
        out = []
        for m in tok(name).finditer(c):
            pre = c[:m.start()].rstrip()
            member = pre.endswith(('.', '->', '::'))
            if kind == 'member' and member: out.append(a + m.start())
            elif kind == 'bare' and not member: out.append(a + m.start())
            elif kind == 'init' and not member and re.match(r'\s*\(', c[m.end():]): out.append(a + m.start())
        return out

    def uses(self, scope, edited):
        r = self.repo; objs = {}; pending = set(scope)
        scope = scope  # list, shrunk in place by dry runs
        for rnd in range(1, self.args.rounds + 1):
            res = compile_all(pending, self.args.jobs)
            fails = {tu: log for tu, (obj, log) in res.items() if not obj}
            for tu, (obj, log) in res.items():
                if obj:
                    if tu in objs: os.unlink(objs[tu])
                    objs[tu] = obj
            self.log('round %d: %d compiled, %d with errors' % (rnd, len(res), len(fails)))
            fails = {tu: l for tu, l in fails.items() if tu in scope}
            if not fails: return objs
            sites = {}   # (path, line, name, kind) -> [count, types]
            for tu, log in fails.items():
                ds = self.parse(tu, log); per = {}
                bad = [d for d in ds if d[2] in ('new', 'other')]
                if any(d[2] == 'new' for d in bad):
                    raise Fail('%s: a rewritten token belonged to another struct: %s (typically a template body instantiated '
                               'for several types: rename the same field of every instantiating type in one batch, or edit by hand)'
                               % (tu, [d[:2] + d[4:] for d in bad if d[2] == 'new'][:3]))
                if bad and len(bad) == len(ds) and rnd == 1 and self.args.dry_run:
                    self.warnings.append('%s does not build here (dropped from the dry run)' % tu); scope.remove(tu); continue
                if bad and len(bad) == len(ds):
                    raise Fail('%s: errors not caused by the renames:\n%s' % (tu, log[-1500:]))
                for p, line, kind, ty, name in ds:
                    if kind in ('new', 'other'): continue
                    e = per.setdefault((p, line, name, kind), [0, set()]); e[0] += 1; e[1].add(ty)
                for k, (n, tys) in per.items():
                    e = sites.setdefault(k, [0, set(), tu]); e[0] = max(e[0], n); e[1] |= tys
            touched = set()
            for (p, line, name, kind), (n, tys, tu) in sorted(sites.items()):
                news = {self.row_for(name, ty)['new_name'] for ty in tys}
                if len(news) != 1: raise Fail('%s:%d: `%s` reached through types %s that map to %s' % (p, line, name, sorted(map(str, tys)), sorted(news)))
                new = news.pop()
                cand = self.candidates(p, line, name, kind)
                mc = self.macro_cands(p, line, name) if kind == 'member' else None
                if kind == 'member' and n > len(cand) == 1 and p.endswith('.h') and mc is None:
                    # one token in a header reported once per instantiation of the template around it
                    self.log('  %s:%d: %d reports of one `%s` token (template instantiated several times)' % (p, line, n, name)); n = 1
                if kind == 'member' and (n > len(cand) or not cand) and mc is not None:
                    # a function-like macro defined in the same file is called on the line: rewrite the member
                    # accesses in its body (the line's own tokens, if any, come back in the next round)
                    if not mc: raise Fail('%s:%d: `%s` through a macro on the line but no member token in its body' % (p, line, name))
                    self.log('  %s:%d: `%s` reached through a macro body: %d tokens rewritten there' % (p, line, name, len(mc)))
                    r.write(p, swap(r.text[p], mc, name, new)); touched.add(p)
                    for _ in mc: self.site(p, line, name, new, 'macro')
                    continue
                if kind == 'member' and n > len(cand) or not cand:
                    raise Fail('%s:%d: %d `%s` accesses reported but %d on the line: a macro body or a token pasted '
                               'by the preprocessor; edit it by hand' % (p, line, n, name, len(cand)))
                if kind == 'member' and n < len(cand):
                    cand = self.solve(tu, p, line, name, new, cand, n)
                r.write(p, swap(r.text[p], cand, name, new)); touched.add(p)
                for _ in cand: self.site(p, line, name, new, kind)
            pending = (set(fails) | set(r.tus_including(touched))) & set(scope)
        raise Fail('no fixed point after %d rounds' % self.args.rounds)

    def macro_cands(self, p, line, name):
        """Offsets of `.name`/`->name` tokens in the bodies of function-like macros that are #defined in p and called
        on the line; None when no such macro is called there."""
        c = self.repo.code_of(p); found = False; out = []
        at = line_span(self.repo.text[p], line)[0]
        for ident in set(re.findall(r'[A-Za-z_]\w*', self.line_code(p, line))):
            ms = list(re.finditer(r'^[ \t]*#[ \t]*define[ \t]+%s\(' % re.escape(ident), c, re.M))
            if not ms: continue
            m = ([d for d in ms if d.start() < at] or ms)[-1]   # the #define in effect on the line
            found = True; e = m.start()
            while True:   # the #define runs to the first line not ended by a backslash
                e = c.find('\n', e)
                if e < 0: e = len(c); break
                if self.repo.text[p][:e].rstrip(' \t\r').endswith('\\'): e += 1; continue
                break
            for t in tok(name).finditer(c, m.start(), e):
                if c[:t.start()].rstrip().endswith(('.', '->')): out.append(t.start())
        return sorted(set(out)) if found else None

    def solve(self, tu, p, line, name, new, cand, n):
        """Several `name` accesses on one line, only n of them ours: try each n-subset with a recompile of tu."""
        r = self.repo; base = r.text[p]; ok = []
        for sub in itertools.combinations(cand, n):
            r.write(p, swap(base, sub, name, new))
            obj, log = _compile(tu)[1:]
            if obj: os.unlink(obj)
            ds = self.parse(tu, log) if not obj else []
            if not any(d[0] == p and d[1] == line and (d[2] == 'new' or d[4] == name) for d in ds): ok.append(sub)
        r.write(p, base)
        if len(ok) != 1: raise Fail('%s:%d: cannot tell which `%s` accesses are ours (%d candidate sets fit)' % (p, line, name, len(ok)))
        self.log('  %s:%d: %d of %d `%s` accesses are ours (resolved by recompiling)' % (p, line, n, len(cand), name))
        return list(ok[0])

    # 4. blind spots --------------------------------------------------------------------------------------------------
    def blind_spots(self, scope):
        files = set().union(*(self.repo.closure(tu) for tu in scope)) if scope else set()
        for p in sorted(files):
            c = self.repo.code_of(p); tmpl = 'template' in c
            for row in self.rows:
                for m in re.finditer(r'(?:\.|->)\s*%s(?![A-Za-z0-9_$])' % re.escape(row['old_name']), c):
                    a = c.rfind('\n', 0, m.start()) + 1; ln = c[:m.start()].count('\n') + 1
                    linetxt = c[a:c.find('\n', m.start())]
                    if re.match(r'\s*#\s*define', linetxt) or (tmpl and p.endswith('.h')):
                        self.warnings.append('%s:%d: `%s` in a %s: not checked by the compiler, review by hand'
                                             % (p, ln, row['old_name'], 'macro' if '#' in linetxt else 'template header'))

def load_rows(args):
    rows = []
    for r in csv.DictReader(open(args.csv)):
        if args.header and r['header'] != args.header: continue
        if args.struct and r['struct'] not in args.struct.split(','): continue
        if args.fields and r['old_name'] not in args.fields.split(','): continue
        if CONF.get(r['confidence'], 0) < CONF[args.min_confidence]: continue
        if r['old_name'] == r['new_name']: continue
        rows.append(r)
    return rows

def main(argv=None):
    ap = argparse.ArgumentParser(description=__doc__.split('\n')[0], formatter_class=argparse.RawDescriptionHelpFormatter, epilog=__doc__)
    ap.add_argument('--csv', required=True); ap.add_argument('--header'); ap.add_argument('--struct')
    ap.add_argument('--fields', help='comma-separated old names'); ap.add_argument('--min-confidence', default='low', choices=CONF)
    ap.add_argument('--dry-run', action='store_true'); ap.add_argument('--check', action='store_true', help='also compare matchtool results')
    ap.add_argument('--allow-unverifiable', action='store_true'); ap.add_argument('--allow-dirty', action='store_true')
    ap.add_argument('--jobs', type=int, default=int(os.environ.get('JOBS', 2))); ap.add_argument('--rounds', type=int, default=12)
    ap.add_argument('--log', help='JSON log of every edit site'); ap.add_argument('--commit-msg', help='write a commit message here')
    args = ap.parse_args(argv)
    os.chdir(ROOT)
    rows = load_rows(args)
    if not rows: print('no rows selected'); return 1
    if not args.allow_dirty and subprocess.run(['git', 'status', '--porcelain', '--', 'src', 'include'], capture_output=True, text=True).stdout.strip():
        print('src/ or include/ has uncommitted changes (the batch must be revertible); use --allow-dirty to override'); return 1
    repo = Repo(); b = Batch(repo, rows, args); ok = False
    try:
        b.guard()
        if not b.rows: print('nothing left to apply'); return 1
        # scope: every TU that sees a definition we will edit (header or view); computed before editing
        decl_files = {p for row in b.rows for n in row['hier'] for p, a, e, _ in repo.defs.get(n, ())}
        hdr_files = {p for row in b.rows for p, a, e, _ in repo.defs[row['struct']]}
        scope = repo.tus_including(hdr_files | {p for p in decl_files if p.endswith(('.h', '.hpp', '.inc'))} | decl_files)
        b.log('%d rows, %d TUs in scope' % (len(b.rows), len(scope)))
        before = {}
        if not args.dry_run:
            b.log('compiling %d TUs before editing (jobs=%d)...' % (len(scope), args.jobs))
            res = compile_all(scope, args.jobs)
            bad = sorted(tu for tu, (obj, _) in res.items() if not obj)
            before = {tu: signature(obj) for tu, (obj, _) in res.items() if obj}
            if bad:
                uses = [tu for tu in bad if any(row['old_name'] in repo.code_of(tu) for row in b.rows)]
                msg = '%d TUs in scope do not build here: %s' % (len(bad), ', '.join(bad[:8]))
                if not args.allow_unverifiable: raise Fail(msg + ' (use --allow-unverifiable to skip them)')
                b.warnings.append(msg + '; mentioning an old name (fix by hand): %s' % (', '.join(uses) or 'none'))
                scope = [tu for tu in scope if tu not in bad]
        edited = b.declare()
        objs = b.uses(scope, edited)
        b.blind_spots(scope)
        for p, line, old, new, kind in b.sites: print('  %-7s %s:%d  %s -> %s' % (kind, p, line, old, new))
        if args.dry_run:
            for o in objs.values(): os.unlink(o)
            ok = True; return 0
        b.log('verifying %d objects...' % len(objs))
        diff = []
        for tu in scope:
            s = signature(objs[tu])
            for k in ('funcs', 'data', 'nm'):
                if s[k] != before[tu][k]:
                    names = sorted(n for n in set(s[k]) | set(before[tu][k]) if s[k].get(n) != before[tu][k].get(n)) if k != 'nm' \
                        else sorted(set(s[k]) ^ set(before[tu][k]))
                    diff.append('%s: %s differ: %s' % (tu, k, names[:5]))
        if diff: raise Fail('objects changed:\n  ' + '\n  '.join(diff))
        if args.check:
            touched = sorted({p for p, *_ in b.sites if p.endswith(('.c', '.cpp'))})
            for tu in touched:
                out = subprocess.run([sys.executable, 'tools/matchtool.py', 'check', tu], capture_output=True, text=True).stdout
                n = len(re.findall(r': MATCH', out)); bad = [l for l in out.splitlines() if re.match(r'^\S+ @0x', l) and ': MATCH' not in l]
                b.log('  matchtool %s: %d MATCH, %d other' % (tu, n, len(bad)))
        ok = True
    except Fail as e:
        print('FAILED: %s' % e)
        return 1
    finally:
        if not ok or args.dry_run: repo.restore(); print('tree restored')
        for w in b.warnings: print('WARNING: ' + w)
        if args.log:
            json.dump(dict(rows=[{k: v for k, v in r.items() if k != 'hier'} for r in rows], applied=ok and not args.dry_run,
                           sites=b.sites, warnings=b.warnings), open(args.log, 'w'), indent=1, default=list)
    files = sorted({p for p, *_ in b.sites})
    print('APPLIED %d renames, %d edit sites in %d files; %d objects byte-identical' % (len(b.rows), len(b.sites), len(files), len(scope)))
    if args.commit_msg:
        hdr = args.header or ','.join(sorted({r['header'] for r in b.rows}))
        lines = ['%s: rename %d fields (tools/fieldrename.py)' % (hdr, len(b.rows)), '',
                 'Compiler-guided field renames; %d edit sites in %d files; all %d TUs that see the struct rebuilt and'
                 % (len(b.sites), len(files), len(scope)), 'compared per function/data symbol and symbol set: byte-identical.', '']
        lines += ['  %s.%s -> %s' % (r['struct'], r['old_name'], r['new_name']) for r in b.rows]
        refused = [r for r in rows if 'status' in r]
        if refused: lines += ['', 'Not applied:'] + ['  %s.%s -> %s: %s' % (r['struct'], r['old_name'], r['new_name'], r['status']) for r in refused]
        open(args.commit_msg, 'w').write('\n'.join(lines) + '\n')
    return 0

if __name__ == '__main__':
    sys.exit(main())
