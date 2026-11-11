#!/usr/bin/env python3
"""Summarise a views inventory and the rewrite log.

Usage:
  report.py [--views DIR/views.json] [--log DIR/rewrite_log.jsonl] [--md OUT.md]

Prints (and with --md writes) totals: view candidates (frame / load path), targets by confidence, mappable; the
rewrite outcomes per view (the last attempt that mentions it wins: kept / failed / skipped) grouped by reason class;
and header findings:
  - members in a class pad that views use (offset, the views' names, files): declaration candidates for the header;
  - header members still named unknown* / unk* that views name (rename suggestions; the views' names are guesses);
  - rewrites that failed on the namespace's signed `qword` typedef (sly2/basic.h's qword is unsigned).
"""
import argparse, collections, json, os, re, sys
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
import common, parse


def reason_class(st, why):
    w = why or ''
    if st == 'kept':
        return 'kept'
    if st == 'skipped':
        if w.startswith('unmapped'):
            kinds = sorted(set(re.findall(r'\b(beyond|in pad|straddles|stride mismatch|size mismatch|type mismatch|no layout)\b', w)))
            return 'unmappable: ' + (', '.join(kinds) or 'other')
        if 'the block defines its own' in w:
            return 'the block re-declares the target class locally'
        if w.startswith('confidence'):
            return 'target not confident (%s)' % w.split()[-1]
        return w.split(':')[0]
    if "M_" in w and "qword" in w and ('VuSub' in w or 'VuDiv' in w or 'no matching function' in w):
        return 'failed: signed qword typedef vs header qword'
    if 'cannot convert' in w:
        return 'failed: pointer to a view not rewritten (type chain)'
    if 'has no member' in w:
        return 'failed: member not renamed / missing in header'
    if 'needs #include' in w:
        return 'failed: header include'
    if w.startswith('compile failed'):
        return 'failed: other compile error'
    if w.startswith('sections') or w.startswith('symbols'):
        return 'failed: object differs (codegen)'
    if 'not resolved' in w:
        return 'failed: ambiguous member access'
    return 'failed: other'


def main():
    ap = argparse.ArgumentParser(description=__doc__.split('\n')[0])
    ap.add_argument('--views')
    ap.add_argument('--log')
    ap.add_argument('--md')
    A = ap.parse_args()
    d = os.path.join(common.ROOT, 'build', 'views')
    allrecs = json.load(open(A.views or os.path.join(d, 'views.json')))
    views = [r for r in allrecs if r.get('candidate')]
    out = []
    p = out.append
    conf = collections.Counter(r.get('confidence') or 'none' for r in views)
    p('# Views report\n')
    p('view candidates: %d in %d files (frame path %d, load path %d); targets: %s; mappable (target + every used '
      'member mapped): %d' % (len(views), len({r['file'] for r in views}), sum(r['path'] == 'frame' for r in views),
                              sum(r['path'] == 'load' for r in views), dict(conf), sum(bool(r.get('mappable')) for r in views)))
    st = collections.Counter()
    for r in views:
        for m in r['members']:
            if not m['pad'] and m['uses']:
                st[m['map'][0]] += 1
    p('used members by mapping status: %s' % dict(st))
    logp = A.log or os.path.join(d, 'rewrite_log.jsonl')
    final = {}
    if os.path.exists(logp):
        for l in open(logp):
            k = json.loads(l)
            if k['status'] in ('group-fail', 'dry-run') or k['views'] == ['(tidy)']:
                continue
            for v in k['views']:
                key = (k['file'], v)
                if final.get(key, ('',))[0] == 'kept':
                    continue        # a later pass cannot un-keep it
                final[key] = (k['status'], k.get('why', ''))
        cls = collections.Counter(reason_class(s, w) for s, w in final.values())
        p('\n## Rewrite outcomes (last attempt per view; %d views attempted or skipped)\n' % len(final))
        for c, n in cls.most_common():
            p('- %s: %d' % (c, n))
        kept = sorted(k for k, v in final.items() if v[0] == 'kept')
        byfile = collections.Counter(f for f, _ in kept)
        p('\nkept by file: ' + ', '.join('%s %d' % (os.path.basename(f), n) for f, n in byfile.most_common()))
    # header findings
    pads = collections.defaultdict(list)
    unk = collections.defaultdict(set)
    for r in views:
        for m in r['members']:
            if m['pad'] or not m['uses']:
                continue
            s_, path = m['map']
            if s_ == 'in pad' and r.get('target'):
                pads[(r['target'], m.get('off'))].append('%s %s::%s' % (os.path.basename(r['file']), r['name'], m['name']))
            if s_ == 'exact' and re.search(r'(^|\.)(unknown|unk_?)[0-9A-Fa-f]', path) and not re.match(r'^(x|p|g|n|v|q)?[0-9A-Fa-f]+$|^unk|^unknown', m['name']):
                unk[(r['target'], path)].add(m['name'])
    p('\n## Header findings\n')
    p('### Members used inside a class pad (%d)\n' % len(pads))
    for (t, o), us in sorted(pads.items(), key=lambda x: (x[0][0], x[0][1] or 0))[:200]:
        p('- %s +0x%X: %s' % (t, o or 0, '; '.join(sorted(set(us))[:4])))
    p('\n### Draft-named header members the views name (%d)\n' % len(unk))
    for (t, path), ns in sorted(unk.items())[:300]:
        p('- %s::%s: %s' % (t, path, ', '.join(sorted(ns))))
    txt = '\n'.join(out) + '\n'
    print(txt)
    if A.md:
        open(A.md, 'w').write(txt)


if __name__ == '__main__':
    main()
