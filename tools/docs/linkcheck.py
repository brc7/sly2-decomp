"""Check every relative markdown link and #anchor under docs/. ../../src/... links resolve against the repo
(docs/ is meant to sit in sly2-decomp), so they are checked against /home/claude/ref/sly2-decomp."""
import os as _os
_REPO = _os.path.abspath(_os.path.join(_os.path.dirname(__file__), '..', '..'))
_RESEARCH = _os.environ.get('SLY2_RESEARCH', _os.path.join(_REPO, '..', 'research'))

import os, re, sys
D = os.path.join(_REPO, 'docs')
REPO = _REPO
LINK = re.compile(r'(?<![!\\])\[(?:[^\]\\]|\\.)*\]\(([^)\s]+)\)')
def slug(h):
    h = re.sub(r'[`*]', '', h.strip().lower())
    h = re.sub(r'[^\w\- ]', '', h)
    return h.replace(' ', '-')
anchors = {}
def get_anchors(p):
    if p not in anchors:
        s, seen = set(), {}
        incode = False
        for line in open(p, encoding='utf-8'):
            if line.startswith('```'): incode = not incode
            if incode: continue
            m = re.match(r'#+\s+(.*)', line)
            if m:
                b = slug(m.group(1)); n = seen.get(b, 0); seen[b] = n + 1
                s.add(b if n == 0 else f'{b}-{n}')
            for a in re.findall(r'<a (?:name|id)="([^"]+)"', line): s.add(a)
        anchors[p] = s
    return anchors[p]
bad = total = 0
for root, _, files in os.walk(D):
    for fn in files:
        if not fn.endswith('.md'): continue
        p = os.path.join(root, fn)
        incode = False
        for ln, line in enumerate(open(p, encoding='utf-8'), 1):
            if line.startswith('```'): incode = not incode
            if incode: continue
            for t in LINK.findall(line):
                if re.match(r'[a-z]+:', t): continue
                total += 1
                path, _, frag = t.partition('#')
                tgt = os.path.normpath(os.path.join(root, path)) if path else p
                if path.startswith('../../src') or '/src/' in path and not os.path.exists(tgt):
                    rel = os.path.relpath(tgt, os.path.normpath(os.path.join(D, '..')))
                    tgt = os.path.join(REPO, rel)
                if not os.path.exists(tgt):
                    bad += 1; print(f'BROKEN {os.path.relpath(p, D)}:{ln}: {t}'); continue
                if frag and tgt.endswith('.md') and frag not in get_anchors(tgt):
                    bad += 1; print(f'BAD-ANCHOR {os.path.relpath(p, D)}:{ln}: {t}')
print(f'{total} links checked, {bad} broken')
sys.exit(1 if bad else 0)
