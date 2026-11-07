#!/usr/bin/env python3
"""Readability metric for the naming / doc-comment effort (docs/guides/naming-workflow.md section 1).

  readability.py [PATH ...] [--csv FILE] [--top N]

PATH: files or directories (default: src/game include/sly2). Scans .c .cpp .h .hpp .inc files. Prints one summary row
per PATH plus a TOTAL row; --csv writes one row per file; --top N lists the N files with the most open items.
No dependencies beyond the standard library; it never builds or modifies anything.

Columns (lower is better except doc, hdr, inferred):
  files / hdr      files / files whose first non-blank line is a comment (the file header comment)
  funcs            function definitions with a body: top level, inside extern "C" / namespace blocks, and inline
                   members inside struct/class bodies
  doc              definitions with a prose comment block directly above (only blank-free lines between; a #line
                   directive or an SLY2_* annotation line may sit in between)
  annot            definitions whose only comment above is a generated /** @sly2-annot ... @end-sly2-annot */ draft
  undoc            definitions with no comment directly above
  func_            definitions still named func_XXXXXXXX
  func_refs        func_XXXXXXXX tokens in code (comments and strings excluded; declarations + calls + definitions)
  hung             definitions whose name has a Hungarian return-type prefix (FCheckLm, PloFind, GGetX, NWorldCur, ...)
  short            single-letter parameters and locals other than i/j
  m2c              m2c-style names in code: temp_*, var_*, sp<hex> (sp10), arg<n> (arg0), saved_reg_*
  offs             offset-coded names in code: x<HEX> (x4C8), unk*/unk_<hex>, _pad_*, pad<hex>
  inferred         "(inferred)" markers (anywhere, comments included)

Heuristics and their limits (regex/brace based, not a C++ parser):
  * Comments, string/char literals and preprocessor lines are blanked first (line structure kept). Code produced by
    macros (SLY2_INSTANTIATE_SLOT_POOLS(), #define bodies) is invisible; a line holding only an all-caps macro call
    (or an *_END macro) is read as `;`. #if 0 blocks are scanned like live code.
  * A definition is a `{` whose statement head (text since the previous `;`, `{` or `}` at that depth) contains a
    `name(` call shape and is not struct/class/union/enum/namespace/extern "C"/`= {` initializer. Function-pointer
    returning declarators and K&R definitions are not recognised; a lambda would count as nothing (none exist).
  * Name: the identifier before the head's first top-level `(` (skipping __attribute__/__asm__); C++ qualifiers are
    kept for display, a gcc 2.95 mangled suffix (`__FP2LOi`) is ignored for the Hungarian test.
  * Hungarian: base name starts with a single-letter type tag F/G/N/C/I/U followed by an English word (`[A-Z][a-z]`),
    with P/Pp + a lowercase tag that is a known type name of the scanned code or a common tag (v, ch, b, n, f, fn,
    ...), or with a fixed tag list (Cb, Cch, Dx, Dy, Dz, Dt, Grf, Id, Oid, Cid, Ref, Rad, Sz, Lm, Pfn, Ix, ...).
    Names whose first `_`-separated word is a struct/class name are skipped (CRef, CMpeg_QueueMovie).
    False positives: real acronyms (`IOther`-style); false negatives: suffix tags (`StepLo`), unlisted prefixes
    (`GXOfVf`, `S32Read`).
  * Single-letter names: parameters of recognised definitions, plus `Type name` / `Type *name` declarations inside
    function bodies (keywords such as return/case excluded). `a * b)` multiplications can be miscounted as a
    declaration; struct fields are not counted (offs covers the offset-coded ones).
  * m2c / offs patterns count every occurrence (a field declared once and used ten times counts eleven), so they track
    progress rather than distinct names. x<HEX> needs an uppercase hex digit or a digit after the x (x0, x4C8, xC),
    so short real names such as `x` / `xy` are not counted; a local named x1 is.
"""
import argparse, csv, os, re, sys

EXTS = ('.c', '.cpp', '.h', '.hpp', '.inc')
DEFAULT_PATHS = ('src/game', 'include/sly2')
COLS = ('files', 'hdr', 'funcs', 'doc', 'annot', 'undoc', 'func_', 'func_refs', 'hung', 'short', 'm2c', 'offs',
        'inferred')

KEYWORDS = set('''if for while switch return case goto delete else sizeof new throw do typedef using operator
    struct class union enum namespace extern static const volatile unsigned signed register inline virtual
    template typename public private protected friend default break continue asm __asm__ __attribute__'''.split())
PLACEHOLDER = re.compile(r'\bfunc_[0-9A-Fa-f]{8}\b')
M2C = re.compile(r'\b(?:temp_\w+|var_\w+|saved_reg_\w+|sp[0-9A-F][0-9A-Fa-f]*|arg[0-9]+)\b')
OFFS = re.compile(r'\b(?:x[0-9A-F][0-9A-F]*|unk_?[0-9A-Fa-f]*\w*|_?pad_\w*|pad[0-9A-F][0-9A-Fa-f]*)\b')
MACRO_LINE = re.compile(r'^(\s*)(?:[A-Z][A-Z0-9_]+\(.*\)|[A-Z][A-Z0-9_]*_END)[ \t]*$', re.M)
INFERRED = re.compile(r'\(inferred\)', re.I)
HUNG_SINGLE = re.compile(r'^[FGNCIU][A-Z][a-z]')
HUNG_P = re.compile(r'^Pp?([a-z]+)[A-Z0-9]')
HUNG_TAGS = re.compile(r'^(?:Cb|Cch|Cpch|Dx|Dy|Dz|Dt|Dg|Grf|Id|Oid|Cid|Ref|Rad|Sz|Lm|Pfn|Fn|Ix|Ipos|Ul|Clr|Rgba|Ch'
                       r'|Mat|Vec|Lp|Psz|Msk|Tm|Qw)[A-Z]')
COMMON_P_TAGS = {'v', 'ch', 'b', 'n', 'f', 'fn', 'w', 's', 'u', 'g', 'l', 'sz', 'mat', 'vec', 'pv', 'qw', 'dw',
                 'ui', 'us', 'grf', 'ref', 'cid', 'oid'}
DECL = re.compile(r'(?:^|[;{(,])\s*(?:(?:const|unsigned|signed|struct|register|static|volatile)\s+)*'
                  r'([A-Za-z_]\w*(?:::\w+)*)\s*(?:\*+|&)?\s*\b([A-Za-z])\b\s*(?=[;=,\[)])', re.M)


def strip_code(text):
    """Blank comments, string/char literals and preprocessor lines, keeping every newline (and so line numbers)."""
    out, i, n = [], 0, len(text)
    line_start = True
    while i < n:
        c = text[i]
        if line_start and c in ' \t':
            out.append(c); i += 1; continue
        if line_start and c == '#':   # preprocessor line, with continuations
            while i < n and text[i] != '\n':
                if text[i] == '\\' and i + 1 < n and text[i + 1] == '\n':
                    out.append('\n'); i += 2; continue
                i += 1
            continue
        line_start = False
        if c == '\n':
            out.append(c); i += 1; line_start = True
        elif text.startswith('//', i):
            while i < n and text[i] != '\n':
                i += 1
        elif text.startswith('/*', i):
            j = text.find('*/', i + 2)
            j = n if j < 0 else j + 2
            out.append(' ' + '\n' * text.count('\n', i, j)); i = j
        elif c in '"\'':
            j = i + 1
            while j < n and text[j] != c and text[j] != '\n':
                j += 2 if text[j] == '\\' else 1
            out.append(c + c + '\n' * text.count('\n', i, j)); i = j + 1
        else:
            out.append(c); i += 1
    # (blank_macro_lines) A bare macro invocation line (SLY2_SPLICEWRAP_BEGIN(x), SLY2_INSTANTIATE_SLOT_POOLS(), SLY2_SPLICEWRAP_END) ends
    # a statement: its expansion is invisible here, so treat it as ';' to keep the next definition's head clean.
    return ''.join(out)


def blank_macro_lines(code):
    return MACRO_LINE.sub(lambda m: m.group(1) + ';', code)


def head_name(head):
    """(name, params) of a definition head, or None if the head is not function-shaped."""
    depth, k = 0, 0
    while k < len(head):
        ch = head[k]
        if ch == '(':
            if depth == 0:
                m = re.search(r'(~?[A-Za-z_][\w]*(?:\s*::\s*~?[A-Za-z_]\w*)*|operator\s*\S+?)\s*$', head[:k])
                if m and m.group(1) not in ('__attribute__', '__asm__', 'asm'):
                    name = re.sub(r'\s+', '', m.group(1))
                    if name.split('::')[-1] in KEYWORDS:
                        return None
                    end, d = k, 0
                    for end in range(k, len(head)):
                        d += {'(': 1, ')': -1}.get(head[end], 0)
                        if d == 0:
                            break
                    return name, head[k + 1:end]
            depth += 1
        elif ch == ')':
            depth -= 1
        k += 1
    return None


def scan_defs(code):
    """Yield (head_start_offset, name, params, body_start, body_end) for function definitions."""
    stack = []        # block kinds: 'ns', 'type', 'func', 'other'
    head_start = 0
    i, n = 0, len(code)
    defs = []
    while i < n:
        ch = code[i]
        if ch == '{':
            head = code[head_start:i]
            parent = stack[-1][0] if stack else 'ns'
            kind = 'other'
            hs = head.strip()
            if parent in ('ns', 'type'):
                if re.search(r'\bextern\s*""\s*$', hs) or re.match(r'namespace\b', hs):
                    kind = 'ns'
                elif re.search(r'(?:^|[\s;])(?:struct|class|union|enum)\b[^()]*$', hs) or re.search(r'=\s*$', hs) \
                        or re.search(r'\b(?:struct|class|union)\s+\w+\s*:\s*[\w\s,:]*$', hs):
                    kind = 'type' if not re.search(r'=\s*$', hs) else 'other'
                else:
                    hn = head_name(hs) if '(' in hs and not re.search(r'=\s*$', hs) else None
                    if hn:
                        kind = 'func'
                        lead = len(head) - len(head.lstrip())
                        stack.append((kind, head_start + lead, hn))
                        head_start = i + 1; i += 1
                        continue
            stack.append((kind, None, None))
            head_start = i + 1
        elif ch == '}':
            if stack:
                kind, hstart, hn = stack.pop()
                if kind == 'func':
                    defs.append((hstart, hn[0], hn[1], i))
            head_start = i + 1
        elif ch == ';':
            head_start = i + 1
        i += 1
    return defs


def preceding_comment(lines, idx):
    """Classify the comment block directly above line index idx: 'doc', 'annot' or None."""
    j = idx - 1
    while j >= 0 and (lines[j].lstrip().startswith('#line') or re.match(r'\s*//\s*SLY2_', lines[j])):
        j -= 1
    block = []
    if j >= 0 and lines[j].rstrip().endswith('*/'):
        while j >= 0:
            block.append(lines[j])
            if '/*' in lines[j]:
                break
            j -= 1
        if j < 0 or not lines[j].lstrip().startswith('/*'):   # a trailing comment after code is not a doc block
            return None
        j -= 1
    while j >= 0 and lines[j].lstrip().startswith('//'):
        block.append(lines[j]); j -= 1
    if not block:
        return None
    text = '\n'.join(reversed(block))
    prose = re.sub(r'/\*\*\s*@sly2-annot.*?@end-sly2-annot\s*\*/', '', text, flags=re.S)
    prose = re.sub(r'[/*\s]', '', prose)
    return 'doc' if prose else 'annot'


def is_hungarian(name, type_tags, class_names=()):
    base = name.split('::')[-1]
    base = re.sub(r'__(?:F|[0-9C]).*$', '', base)
    if base.split('_')[0] in class_names:   # constructors and Class_Method names (CRef, CMpeg_QueueMovie)
        return False
    if HUNG_SINGLE.match(base) or HUNG_TAGS.match(base):
        return True
    m = HUNG_P.match(base)
    return bool(m and (m.group(1) in COMMON_P_TAGS or m.group(1).upper() in type_tags))


def param_names(params):
    names = []
    for p in params.split(','):
        p = re.sub(r'=.*$', '', p).strip()
        m = re.search(r'([A-Za-z_]\w*)\s*(?:\[[^\]]*\])?$', p)
        if m and re.search(r'[\w*&]\s*[*&\s]\s*' + re.escape(m.group(1)) + r'\s*(?:\[[^\]]*\])?$', p) \
                and m.group(1) not in KEYWORDS:
            names.append(m.group(1))
    return names


def measure(path, type_tags, class_names):
    text = open(path, encoding='utf-8', errors='replace').read()
    code = strip_code(text)
    lines = text.split('\n')
    r = dict.fromkeys(COLS, 0)
    r['files'] = 1
    first = next((l.strip() for l in lines if l.strip()), '')
    r['hdr'] = int(first.startswith('//') or first.startswith('/*'))
    r['func_refs'] = len(PLACEHOLDER.findall(code))
    code = blank_macro_lines(code)
    r['m2c'] = len(M2C.findall(code))
    r['offs'] = len(OFFS.findall(code))
    r['inferred'] = len(INFERRED.findall(text))
    for hstart, name, params, bend in scan_defs(code):
        r['funcs'] += 1
        line_idx = code.count('\n', 0, hstart)
        kind = preceding_comment(lines, line_idx)
        r[{'doc': 'doc', 'annot': 'annot', None: 'undoc'}[kind]] += 1
        if PLACEHOLDER.fullmatch(name.split('::')[-1]):
            r['func_'] += 1
        elif is_hungarian(name, type_tags, class_names):
            r['hung'] += 1
        shorts = [p for p in param_names(params) if len(p) == 1 and p not in 'ij']
        body = code[code.find('{', hstart) + 1:bend]
        for m in DECL.finditer(body):
            if m.group(1) not in KEYWORDS and m.group(2) not in 'ij':
                shorts.append(m.group(2))
        r['short'] += len(shorts)
    return r


def collect(paths):
    out = []
    for p in paths:
        if os.path.isfile(p):
            out.append((p, p))
            continue
        for root, dirs, files in os.walk(p):
            dirs.sort()
            for f in sorted(files):
                if f.endswith(EXTS):
                    out.append((p, os.path.join(root, f)))
    return out


def main():
    ap = argparse.ArgumentParser(description=__doc__.split('\n')[0])
    ap.add_argument('paths', nargs='*')
    ap.add_argument('--csv', help='write per-file rows to this CSV file')
    ap.add_argument('--top', type=int, default=0, help='list the N files with the most open items')
    args = ap.parse_args()
    repo = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
    paths = args.paths or [os.path.join(repo, d) for d in DEFAULT_PATHS]
    files = collect(paths)
    # Known type names (all-caps struct/class names) for the P<tag> Hungarian test.
    class_names = set()
    for _, f in files:
        class_names.update(re.findall(r'\b(?:struct|class|union)\s+([A-Za-z_]\w*)',
                                      open(f, encoding='utf-8', errors='replace').read()))
    type_tags = set(c for c in class_names if re.fullmatch(r'[A-Z][A-Z0-9_]+', c))
    rows, groups = [], {}
    for group, f in files:
        r = measure(f, type_tags, class_names)
        rows.append((os.path.relpath(f, repo), r))
        g = groups.setdefault(os.path.relpath(group, repo), dict.fromkeys(COLS, 0))
        for c in COLS:
            g[c] += r[c]
    total = dict.fromkeys(COLS, 0)
    for g in groups.values():
        for c in COLS:
            total[c] += g[c]
    width = max([len(k) for k in groups] + [5])
    print('%-*s ' % (width, 'scope') + ' '.join('%9s' % c for c in COLS))
    for k, g in list(groups.items()) + [('TOTAL', total)]:
        print('%-*s ' % (width, k) + ' '.join('%9d' % g[c] for c in COLS))
    if total['funcs']:
        print('documented %.1f%% of %d functions (%d annot-only drafts); placeholder names %.1f%%, Hungarian %.1f%%; '
              'header comments %d/%d files' % (100.0 * total['doc'] / total['funcs'], total['funcs'], total['annot'],
                                               100.0 * total['func_'] / total['funcs'],
                                               100.0 * total['hung'] / total['funcs'], total['hdr'], total['files']))
    if args.top:
        score = lambda r: r['undoc'] + r['annot'] + r['func_'] + r['hung'] + (1 - r['hdr'])
        print('\nmost open items (undoc + annot + func_ + hung + missing header):')
        for f, r in sorted(rows, key=lambda x: -score(x[1]))[:args.top]:
            print('  %5d  %s' % (score(r), f))
    if args.csv:
        with open(args.csv, 'w', newline='') as fh:
            w = csv.writer(fh)
            w.writerow(('file',) + COLS)
            for f, r in rows:
                w.writerow((f,) + tuple(r[c] for c in COLS))


if __name__ == '__main__':
    sys.exit(main())
