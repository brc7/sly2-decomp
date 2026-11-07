"""Check documentation links.

Default mode: every relative link and #anchor in the repo's markdown (docs/, the root *.md files and src/**/README.md)
must resolve. Exit 1 if any is broken. tools/verify.sh runs this.

--src: instead, check the docs/... references written in source comments (src/, include/, tools/): the file must exist,
a `#anchor` must exist in it, and a quoted section name (`docs/guides/tu-merging.md "Phase 5: data order"`) or
`section N` must match a heading of that file. Prints one line per dangling reference and a summary; exit 1 if any.
"""
import os, re, sys

REPO = os.path.abspath(os.path.join(os.path.dirname(__file__), '..'))
LINK = re.compile(r'(?<![!\\])\[(?:[^\]\\]|\\.)*\]\(([^)\s]+)\)')


def slug(h):
    h = re.sub(r'[`*]', '', h.strip().lower())
    h = re.sub(r'[^\w\- ]', '', h)
    return h.replace(' ', '-')


_headings = {}


def headings(p):
    """(set of GitHub-style anchors, list of heading texts) of a markdown file."""
    if p not in _headings:
        anchors, texts, seen, incode = set(), [], {}, False
        for line in open(p, encoding='utf-8'):
            if line.startswith('```'):
                incode = not incode
            if incode:
                continue
            m = re.match(r'#+\s+(.*)', line)
            if m:
                texts.append(m.group(1).strip())
                b = slug(m.group(1)); n = seen.get(b, 0); seen[b] = n + 1
                anchors.add(b if n == 0 else f'{b}-{n}')
            for a in re.findall(r'<a (?:name|id)="([^"]+)"', line):
                anchors.add(a)
        _headings[p] = (anchors, texts)
    return _headings[p]


def markdown_files():
    for fn in sorted(os.listdir(REPO)):
        if fn.endswith('.md'):
            yield os.path.join(REPO, fn)
    for top in ('docs', 'src'):
        for root, dirs, files in os.walk(os.path.join(REPO, top)):
            dirs.sort()
            for fn in sorted(files):
                if fn.endswith('.md') and (top == 'docs' or fn == 'README.md'):
                    yield os.path.join(root, fn)


def check_markdown():
    bad = total = 0
    for p in markdown_files():
        incode = False
        for ln, line in enumerate(open(p, encoding='utf-8'), 1):
            if line.startswith('```'):
                incode = not incode
            if incode:
                continue
            for t in LINK.findall(line):
                if re.match(r'[a-z]+:', t):
                    continue
                total += 1
                path, _, frag = t.partition('#')
                tgt = os.path.normpath(os.path.join(os.path.dirname(p), path)) if path else p
                where = f'{os.path.relpath(p, REPO)}:{ln}: {t}'
                if not os.path.exists(tgt):
                    bad += 1; print(f'BROKEN {where}'); continue
                if frag and tgt.endswith('.md') and frag not in headings(tgt)[0]:
                    bad += 1; print(f'BAD-ANCHOR {where}')
    print(f'{total} links checked, {bad} broken')
    return bad


SRCREF = re.compile(r'(docs/[A-Za-z0-9_./-]*[A-Za-z0-9_-])(#[A-Za-z0-9_.-]+)?'
                    r'(?:[ ,:(]{0,3}(?:"([^"]+)"|(?:section|§) ?([0-9]+)))?')


def check_sources():
    bad = total = 0
    for top in ('src', 'include', 'tools'):
        for root, dirs, files in os.walk(os.path.join(REPO, top)):
            dirs.sort()
            for fn in sorted(files):
                p = os.path.join(root, fn)
                if fn.endswith('.md') or p == os.path.abspath(__file__):
                    continue
                try:
                    lines = open(p, encoding='utf-8').readlines()
                except (UnicodeDecodeError, OSError):
                    continue
                for ln, line in enumerate(lines, 1):
                    for m in SRCREF.finditer(line):
                        path, frag, quoted, sec = m.groups()
                        total += 1
                        tgt = os.path.join(REPO, path)
                        why = None
                        if not os.path.exists(tgt):
                            why = 'missing file'
                        elif tgt.endswith('.md') and os.path.isfile(tgt):
                            anchors, texts = headings(tgt)
                            if frag and frag[1:] not in anchors:
                                why = 'missing anchor'
                            elif quoted and not any(quoted in t for t in texts):
                                why = 'missing section'
                            elif sec and not any(re.match(rf'{sec}[. ]', t) for t in texts):
                                why = 'missing section'
                        if why:
                            bad += 1
                            print(f'{why}: {os.path.relpath(p, REPO)}:{ln}: {m.group(0).strip()}')
    print(f'{total} source-comment doc references checked, {bad} dangling')
    return bad


if __name__ == '__main__':
    sys.exit(1 if (check_sources() if '--src' in sys.argv[1:] else check_markdown()) else 0)
