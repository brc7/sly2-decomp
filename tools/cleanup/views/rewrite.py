#!/usr/bin/env python3
"""Replace TU-private views by their real header classes, keeping only the rewrites whose object is byte-identical.

Usage:
  rewrite.py --base-root BASE [--views DIR/views.json] [--log LOG.jsonl] [--min-confidence high|medium|low]
             [--only NS::VIEW,...] [--dry-run] FILE...

For each FILE (repo-relative), the views inventory.py found there with a target class, every used member mapped
(status exact or vptr) and at least --min-confidence (default high: offset fit alone often lands on an unrelated class with the same
field offsets, which would compile to the same bytes under wrong names) are rewritten per namespace block:

  1. the definition is removed (its newlines are kept, so every later line keeps its number: asm ordering in g++
     2.95 hashes file name and line), and so are forward declarations `struct VIEW;` in the block;
  2. in the namespace block every `VIEW` type name becomes the class, and every `->m` / `.m` access of a used member
     becomes the class's member path (inventory's mapping: aliases in anonymous unions chosen by type);
  3. the file is compiled with the matching flags and compared with the baseline worktree's object (every section's
     bytes, relocations by symbol, symbols): identical -> kept, else reverted with the difference recorded.

Order: first all views of a block together; if that fails, one view at a time, leaves first (a view whose used
pointer members reach other views needs those rewritten first), repeated while anything succeeds. A view is skipped
(reason logged) when it declares variables with its definition, has member functions or virtuals, re-declares a
class whose real definition the TU also sees under that name in the same block, or when two views in the block
would rename the same member name differently. The log (JSON lines) has one record per attempt; report.py
summarises it. Never edits headers.
"""
import argparse, json, os, re, sys, time
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
import common, parse

CONF = {'high': 3, 'medium': 2, 'low': 1, '': 0}


def eligible(r, minconf):
    if not r.get('target'):
        return 'no target'
    if CONF.get(r.get('confidence', ''), 0) < CONF[minconf]:
        return 'confidence %s' % (r.get('confidence') or 'none')
    if not r.get('mappable'):
        bad = [m for m in r['members'] if not m['pad'] and m['uses'] and m['map'][0] not in ('exact', 'vptr')]
        return 'unmapped: ' + ', '.join('%s %s' % (m['name'], m['map'][0]) for m in bad[:4])
    if r.get('vars'):
        return 'definition declares variables'
    if set(r.get('flags', [])) & {'method', 'virtual'}:
        return 'member functions'
    if any(m['uses'] and m['map'][0] == 'vptr' for m in r['members']):
        return 'uses the EE vptr word'
    if r['name'] == r['target']:
        return ''
    return ''


QWORD_UNIONS = {'QWROW'}


def apply(text, group, allrecs=()):
    """Rewrite text for the views in group (records from views.json, same namespace block). Returns new text.
    allrecs: every struct of the file (for the access resolver: member types of local records)."""
    t = common.strip_comments(text)
    edits = []   # (start, end, replacement) on text
    a, b = group[0]['ns_range']
    names = {r['name']: r['target'] for r in group}
    local = {r['name']: r for r in allrecs if r.get('name') and tuple(r['ns_range']) == (a, b)}
    for r in allrecs:   # file-scope records are visible too
        if r.get('name') and r['name'] not in local and r['ns_range'][0] == 0:
            local[r['name']] = r
    qunions = set(QWORD_UNIONS) | {n for n, r in local.items() if r['kind'] == 'union' and
                                   any(m['name'] == 'q' for m in r['members'])}
    # member renames per view; a name renamed differently by two views needs the access resolver
    per = {}
    for r in group:
        per[r['name']] = {}
        for m in r['members']:
            if not m['pad'] and m['uses'] and m['map'][0] == 'exact':
                per[r['name']][m['name']] = (m['map'][1], m['type'] in qunions and m.get('htype') == 'qword')
    allnames = {}
    for v, d in per.items():
        for mn, x in d.items():
            allnames.setdefault(mn, set()).add(x)
    defs = sorted((r['start'], r['end']) for r in group)
    # headers of the targets the TU does not include yet: added after its last top-level #include; one newline of
    # the first removed multi-line definition pays for the added line (later line numbers unchanged)
    incs = re.findall(r'^\s*#\s*include\s*"([^"]+)"', text, re.M)
    need = sorted({r.get('target_header') for r in group if r.get('target_header') and r['target_header'] not in incs})
    paid = not need
    for s_, e in defs:
        nl = text[s_:e].count('\n')
        if not paid and nl >= len(need):
            nl -= len(need); paid = True
        edits.append((s_, e, '\n' * nl))
    if need:
        last = [m for m in re.finditer(r'^[ \t]*#[ \t]*include[ \t]*["<][^">]+[">][^\n]*\n', text, re.M)
                if m.end() <= defs[0][0]]
        if not last:
            raise ValueError('needs #include %s: no #include line before the views' % ', '.join(need))
        ins = last[-1].end()
        if not paid:
            # take the first empty lines after the include block instead (only the lines in between move)
            blanks = [m.start() for m in re.finditer(r'(?m)^[ \t]*\n', text[ins:defs[0][0]])][:len(need)]
            if len(blanks) < len(need):
                raise ValueError('needs #include %s and no line to give for it' % ', '.join(need))
            for k in blanks:
                j = text.index('\n', ins + k)
                edits.append((ins + k, j + 1, ''))
        edits.append((ins, ins, ''.join('#include "%s"\n' % h for h in need)))
    inside = lambda i: any(s_ <= i < e for s_, e in defs)
    for nm in names:
        for m in re.finditer(r'\b(?:struct|class|union)\s+%s\s*;' % re.escape(nm), t[a:b]):
            i = a + m.start()
            if not inside(i):
                edits.append((i, a + m.end(), ''))
    # the view's own layout asserts (typedef char X[cond ? 1 : -1] naming the view): the header asserts its class
    if names:
        arx = re.compile(r'typedef\s+char\s+\w+\s*\[[^;]*?\b(%s)\b[^;]*?\]\s*;[ \t]*' % '|'.join(map(re.escape, names)))
        for m in arx.finditer(t, a, b):
            if not inside(m.start()):
                edits.append((m.start(), m.end(), ''))
    fwd = [(s_, e) for s_, e, _ in edits]
    infwd = lambda i: any(s_ <= i < e for s_, e in fwd)
    if names:
        rx = re.compile(r'(?<![\w.>:])(%s)\b' % '|'.join(map(re.escape, names)))
        for m in rx.finditer(t, a, b):
            if not infwd(m.start()):
                edits.append((m.start(), m.end(), names[m.group(1)]))
    if allnames:
        rx = re.compile(r'(->|\.)(\s*)(%s)\b(?!\s*\()' % '|'.join(map(re.escape, allnames)))
        for m in rx.finditer(t, a, b):
            if infwd(m.start()):
                continue
            mn = m.group(3)
            vt = resolve(t, m.start(), local, a)
            if vt in per:
                x = per[vt].get(mn)
                if x is None:
                    continue
            elif vt is not None:
                continue            # a known type that is not being rewritten
            elif len(allnames[mn]) == 1:
                x = next(iter(allnames[mn]))
            else:
                raise ValueError('member %s maps to %s; access at line %d not resolved'
                                 % (mn, ' / '.join(sorted(p for p, _ in allnames[mn])), t.count('\n', 0, m.start()) + 1))
            path, strip_q = x
            j = m.end(3)
            mq = re.match(r'\s*\.\s*q\b', t[j:j + 8]) if strip_q else None
            edits.append((m.start(3), j + (mq.end() if mq else 0), path))
    edits.sort(key=lambda x: x[0])
    out, pos = [], 0
    for s_, e, rep in edits:
        if s_ < pos:
            continue
        out.append(text[pos:s_]); out.append(rep); pos = e
    out.append(text[pos:])
    new = ''.join(out)
    # tidy the lines a removed definition left blank or with trailing blanks
    return '\n'.join(l.rstrip() if l.strip() == '' or l != l.rstrip() and _touched(l) else l for l in new.split('\n'))


def resolve(t, i, local, lo):
    """Static type name (a local record's name) of the expression ending just before t[i] ('->' or '.'), or None."""
    j = i
    while j > lo and t[j - 1] in ' \t\n':
        j -= 1
    if j > lo and t[j - 1] == ']':
        d, k = 0, j - 1
        while k > lo:
            if t[k] == ']':
                d += 1
            elif t[k] == '[':
                d -= 1
                if d == 0:
                    break
            k -= 1
        return resolve(t, k, local, lo)
    m = re.search(r'(\w+)$', t[max(lo, j - 80):j])
    if not m:
        return None
    ident = m.group(1)
    k = j - len(ident)
    while k > lo and t[k - 1] in ' \t\n':
        k -= 1
    if t[k - 2:k] == '->' or (t[k - 1:k] == '.' and not t[k - 2:k - 1].isdigit()):
        owner = resolve(t, k - (2 if t[k - 2:k] == '->' else 1), local, lo)
        if owner in local:
            for mm in local[owner]['members']:
                if mm['name'] == ident:
                    return mm['type'] if mm['type'] in local else ('<%s>' % mm['type'])
        return None
    # a variable: its nearest declaration before the access, inside the block
    best = None
    for d in re.finditer(r'\b(\w+)\s*(?:\*+|&)?\s*(?:const\s+)?\b%s\s*(?=[;,=)\[])' % re.escape(ident), t[lo:i]):
        if d.group(1) in ('return', 'else', 'case', 'goto', 'sizeof'):
            continue
        best = d.group(1)
    if best is None:
        return None
    return best if best in local else '<%s>' % best


def _collapse(orig, new):
    """Remove the empty lines of `new` that sit in runs of 2+ empty lines which `orig` did not have."""
    import difflib
    a, b = orig.split('\n'), new.split('\n')
    drop = set()
    for tag, i1, i2, j1, j2 in difflib.SequenceMatcher(None, a, b, autojunk=False).get_opcodes():
        if tag in ('replace', 'insert'):
            for j in range(j1, j2):
                if b[j].strip() == '':
                    drop.add(j)
    out = []
    for j, l in enumerate(b):
        if j in drop and (not out or out[-1].strip() == '' or (j + 1 < len(b) and b[j + 1].strip() == '')):
            continue
        out.append(l)
    return '\n'.join(out)


def _touched(l):
    return l.endswith(' ' * 2) or l.endswith('\t')


def clashes(text, group, allrecs):
    """Reasons the group cannot be applied as a whole (local class of the target's name visible in the block)."""
    a, b = group[0]['ns_range']
    out = []
    local_names = {r['name'] for r in allrecs if tuple(r['ns_range']) == (a, b)} - {r['name'] for r in group}
    for r in group:
        if r['target'] in local_names:
            out.append('%s: the block defines its own %s' % (r['name'], r['target']))
    return out


def deps(r, group_names):
    return {m['type'] for m in r['members'] if m['ptr'] and m['uses'] and m['type'] in group_names and m['type'] != r['name']}


def try_text(rel, root, base, new):
    p = os.path.join(root, rel)
    old = open(p).read()
    open(p, 'w').write(new)
    why = common.check_against_base(rel, base, root)
    if why:
        open(p, 'w').write(old)
    return why


def main():
    ap = argparse.ArgumentParser(description=__doc__.split('\n')[0])
    ap.add_argument('--root', default=common.ROOT)
    ap.add_argument('--base-root', required=True)
    ap.add_argument('--views')
    ap.add_argument('--log')
    ap.add_argument('--cache')
    ap.add_argument('--min-confidence', default='high', choices=['high', 'medium', 'low'])
    ap.add_argument('--only', default='')
    ap.add_argument('--dry-run', action='store_true')
    ap.add_argument('--no-tidy', action='store_true')
    ap.add_argument('files', nargs='+')
    A = ap.parse_args()
    root = os.path.abspath(A.root); common.set_root(root)
    vdir = os.path.join(root, 'build', 'views')
    views = json.load(open(A.views or os.path.join(vdir, 'views.json')))
    base = common.BaseObjects(A.base_root, A.cache or os.path.join(vdir, 'base_objs'))
    logf = open(A.log or os.path.join(vdir, 'rewrite_log.jsonl'), 'a')
    only = set(x for x in A.only.split(',') if x)

    def log(**k):
        k['time'] = time.strftime('%H:%M:%S'); logf.write(json.dumps(k) + '\n'); logf.flush()
        print('  %-8s %s %s' % (k['status'], k.get('views'), k.get('why', '')[:150]))

    for rel in A.files:
        allrecs = [r for r in views if r['file'] == rel]
        recs = [r for r in allrecs if r.get('candidate')]
        print(rel, len(recs), 'views')
        p = os.path.join(root, rel)
        orig = open(p).read()
        if common.check_against_base(rel, base, root):
            print('  file differs from the baseline before any rewrite; skipped'); continue
        blocks = {}
        for r in recs:
            if only and common and parse.qual(r) not in only and r['name'] not in only:
                continue
            why = eligible(r, A.min_confidence)
            if why:
                log(file=rel, views=[parse.qual(r)], target=r.get('target'), status='skipped', why=why)
                continue
            blocks.setdefault(tuple(r['ns_range']), []).append(r)
        # apply blocks from the end of the file backwards so earlier offsets stay valid
        for rng in sorted(blocks, reverse=True):
            group = blocks[rng]
            cl = clashes(open(p).read(), group, allrecs)
            bad = {c.split(':')[0] for c in cl}
            for c in cl:
                log(file=rel, views=[c.split(':')[0]], status='skipped', why=c)
            group = [r for r in group if r['name'] not in bad]
            if not group:
                continue
            cur = open(p).read()
            if _attempt(rel, root, base, cur, group, log, A.dry_run, whole=True, allrecs=allrecs):
                continue
            # one at a time, leaves first; offsets of later records shift, so recompute from a fresh inventory slice
            remaining = list(group)
            done = []
            progress = True
            while remaining and progress:
                progress = False
                names = {r['name'] for r in remaining}
                order = sorted(remaining, key=lambda r: len(deps(r, names)))
                for r in order:
                    sub = [r] + [x for x in done]
                    # re-apply from the text without this block's rewrites (offsets are the inventory's)
                    if _attempt(rel, root, base, cur, sub, log, A.dry_run, whole=False, allrecs=allrecs):
                        done.append(r); remaining.remove(r); progress = True
                        break
            for r in remaining:
                log(file=rel, views=[parse.qual(r)], target=r['target'], status='failed', why=r.get('_why', ''))
        new = open(p).read()
        if new != orig and not A.dry_run and not A.no_tidy:
            # tidy: the removed definitions left runs of empty lines; drop the runs the rewrite created if the object
            # stays identical (lines move, and g++ 2.95's asm ordering can depend on them)
            tidy = _collapse(orig, new)
            if tidy != new:
                why = try_text(rel, root, base, tidy)
                log(file=rel, views=['(tidy)'], status='kept' if not why else 'failed',
                    why=why and 'line numbers matter here; empty lines kept: ' + why)
                new = open(p).read()
        print('  %s' % ('changed' if new != orig else 'unchanged'))


def _attempt(rel, root, base, cur, group, log, dry, whole, allrecs=()):
    try:
        new = apply(cur, group, allrecs)
    except ValueError as e:
        for r in group:
            r['_why'] = str(e)
        if whole:
            log(file=rel, views=[parse.qual(r) for r in group], status='group-fail', why=str(e))
        return False
    if dry:
        log(file=rel, views=[parse.qual(r) for r in group], status='dry-run', why='')
        return True
    why = try_text(rel, root, base, new)
    names = [parse.qual(r) for r in group]
    if not why:
        log(file=rel, views=names, targets=[r['target'] for r in group], status='kept', why='')
        return True
    group[0]['_why'] = why
    if whole and len(group) > 1:
        log(file=rel, views=names, status='group-fail', why=why)
    elif whole:
        log(file=rel, views=names, target=group[0]['target'], status='failed', why=why)
        group[0]['_why'] = '(logged)'
    return False


if __name__ == '__main__':
    main()
