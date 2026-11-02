#!/usr/bin/env python3
"""Layering check for the Sly 2 source tree.

usage: check_layering.py LAYOUT_MAP DEPS_CSV LAYERS_CSV [--allow ALLOW_CSV] [--json OUT.json] [--quiet]

  LAYOUT_MAP  CSV with `home_tu` (or `tu`) and `subsystem` (or `subsystem_path`) columns: layout_map.csv or unit_layout.csv
  DEPS_CSV    CSV src_tu,dst_tu,kind,weight[,n,examples]   (deps.csv from build_graph/layout)
  LAYERS_CSV  CSV prefix,layer[,description]: a subsystem's layer = layer of its longest matching path prefix
  ALLOW_CSV   CSV src_subsystem,dst_subsystem[,reason]: known, documented upward/cyclic couplings (not failures)

Reports: subsystem dependency matrix (counted weight), cycles (SCCs of the subsystem graph), upward edges
(dst layer > src layer) with the TU pairs responsible. Exit status 1 if an upward edge or a cycle is not allowlisted.
Edge kinds and weights: WEIGHTS below; kinds outside COUNTED are reported but never fail the check."""
import collections, csv, json, signal, sys
signal.signal(signal.SIGPIPE, signal.SIG_DFL)

WEIGHTS = {'call': 1.0, 'tail': 1.0, 'fnref': 1.0, 'vcall': 1.0, 'fptr_table': 1.0, 'data': 1.0,
           'vtable': 0.25, 'data_common': 0.25, 'boilerplate': 0.0, 'ctor_table': 0.0, 'vtable_getter': 0.0}
COUNTED = {'call', 'tail', 'fnref', 'vcall', 'fptr_table', 'data', 'vtable'}


def load_layers(path):
    rows = []
    for r in csv.DictReader(open(path)):
        rows.append((r['prefix'].rstrip('/'), int(r['layer'])))
    rows.sort(key=lambda x: -len(x[0]))
    def layer(sub):
        for p, l in rows:
            if sub == p or sub.startswith(p + '/'):
                return l
        raise SystemExit('no layer for subsystem %s' % sub)
    return layer


def tarjan(nodes, adj):
    idx, low, st, on, out, n = {}, {}, [], set(), [], [0]
    def sc(v):
        idx[v] = low[v] = n[0]; n[0] += 1; st.append(v); on.add(v)
        for w in adj.get(v, ()):
            if w not in idx:
                sc(w); low[v] = min(low[v], low[w])
            elif w in on:
                low[v] = min(low[v], idx[w])
        if low[v] == idx[v]:
            comp = []
            while True:
                w = st.pop(); on.discard(w); comp.append(w)
                if w == v:
                    break
            if len(comp) > 1:
                out.append(sorted(comp))
    sys.setrecursionlimit(10000)
    for v in nodes:
        if v not in idx:
            sc(v)
    return out


def analyse(tu_sub, deps, layer, allow=frozenset()):
    """tu_sub: {tu: subsystem}; deps: iterable of (src_tu, dst_tu, kind, weight, examples). Returns a result dict."""
    mat = collections.defaultdict(float)
    pair_tus = collections.defaultdict(collections.Counter)
    pair_ex = collections.defaultdict(list)
    side = collections.defaultdict(float)  # uncounted kinds
    for s, d, k, w, ex in deps:
        a, b = tu_sub.get(s), tu_sub.get(d)
        if a is None or b is None or a == b:
            continue
        ww = w * WEIGHTS.get(k, 1.0) / (1.0 if k in ('vcall',) else 1.0)
        if k not in COUNTED:
            side[(a, b)] += ww
            continue
        if ww <= 0:
            continue
        mat[(a, b)] += ww
        pair_tus[(a, b)]['%s->%s' % (s, d)] += ww
        if ex:
            pair_ex[(a, b)].append((ww, '%s->%s [%s] %s' % (s, d, k, ex)))
    subs = sorted(set(tu_sub.values()))
    adj = collections.defaultdict(set)
    for (a, b), w in mat.items():
        adj[a].add(b)
    sccs = tarjan(subs, adj)
    up = []
    for (a, b), w in sorted(mat.items(), key=lambda x: -x[1]):
        if layer(b) > layer(a):
            up.append({'src': a, 'dst': b, 'weight': round(w, 2), 'allowed': (a, b) in allow,
                       'tu_pairs': pair_tus[(a, b)].most_common(6),
                       'examples': [x for _, x in sorted(pair_ex[(a, b)], reverse=True)[:4]]})
    # same-layer cycles: SCC edges between peers
    in_cycle = []
    for comp in sccs:
        cs = set(comp)
        es = sorted(((round(w, 2), a, b) for (a, b), w in mat.items() if a in cs and b in cs), reverse=True)
        in_cycle.append({'members': comp, 'edges': es})
    n_tu_up = sum(len(pair_tus[(u['src'], u['dst'])]) for u in up)
    # peer cycles: SCCs after removing upward edges (edges inside one layer that loop)
    adj2 = collections.defaultdict(set)
    for (a, b), w in mat.items():
        if layer(b) <= layer(a):
            adj2[a].add(b)
    peer = []
    for comp in tarjan(subs, adj2):
        cs = set(comp)
        es = sorted(((round(w, 2), a, b) for (a, b), w in mat.items() if a in cs and b in cs), reverse=True)
        # best linear order of the cycle members (exact DP over subsets, n <= 14): minimise back-edge weight
        n = len(comp); ix = {c: i for i, c in enumerate(comp)}
        W = [[mat.get((a, b), 0.0) for b in comp] for a in comp]
        INF = float('inf'); dp = [INF] * (1 << n); par = [-1] * (1 << n); dp[0] = 0.0
        for m in range(1 << n):
            if dp[m] == INF:
                continue
            for j in range(n):
                if not m >> j & 1:
                    # placing j above the set m: edges from m-members up to j are upward (back) edges
                    c = dp[m] + sum(W[k][j] for k in range(n) if m >> k & 1)
                    if c < dp[m | 1 << j]:
                        dp[m | 1 << j] = c; par[m | 1 << j] = j
        order, m = [], (1 << n) - 1
        while m:
            j = par[m]; order.append(comp[j]); m &= ~(1 << j)
        order.reverse()   # lowest first
        pos = {c: i for i, c in enumerate(order)}
        back = sorted(((round(mat[(a, b)], 2), a, b) for (a, b) in mat if a in cs and b in cs and pos[b] > pos[a]), reverse=True)
        peer.append({'members': comp, 'order_low_to_high': order, 'back_weight': round(dp[(1 << n) - 1], 2), 'back_edges': back,
                     'edges': es})
    unallowed = [u for u in up if not u['allowed']]
    cyc_bad = []
    for c in in_cycle:
        # a cycle is excused if at least one of its edges is allowlisted (the documented back edge)
        if not any((a, b) in allow for _, a, b in c['edges']):
            cyc_bad.append(c)
    return {'subsystems': subs, 'matrix': {'%s|%s' % k: round(v, 2) for k, v in mat.items()},
            'side': {'%s|%s' % k: round(v, 2) for k, v in side.items()},
            'sccs': in_cycle, 'upward': up,
            'n_upward_pairs': len(up), 'upward_weight': round(sum(u['weight'] for u in up), 2),
            'n_upward_unallowed': len(unallowed), 'n_cycles_unallowed': len(cyc_bad),
            'tu_upward_edges': n_tu_up, 'peer_cycles': peer,
            'peer_back_weight': round(sum(p['back_weight'] for p in peer), 2)}


def main(argv):
    if len(argv) < 4:
        print(__doc__); return 2
    lm, dp, ly = argv[1:4]
    allow = set()
    if '--allow' in argv:
        for r in csv.DictReader(open(argv[argv.index('--allow') + 1])):
            allow.add((r['src_subsystem'], r['dst_subsystem']))
    tu_sub = {}
    for r in csv.DictReader(open(lm)):
        tu_sub[r.get('home_tu') or r['tu']] = r.get('subsystem') or r.get('subsystem_path')
    deps = [(r['src_tu'], r['dst_tu'], r['kind'], float(r['weight']), r.get('examples', '')) for r in csv.DictReader(open(dp))]
    layer = load_layers(ly)
    res = analyse(tu_sub, deps, layer, frozenset(allow))
    if '--json' in argv:
        json.dump(res, open(argv[argv.index('--json') + 1], 'w'), indent=1)
    q = '--quiet' in argv
    print('subsystems: %d  upward: %d subsystem pairs / %d TU pairs, weight %.1f (%d pairs not allowlisted)  '
          'cycles (full graph): %d  peer cycles: %d (back-edge weight %.1f)' % (
        len(res['subsystems']), res['n_upward_pairs'], res['tu_upward_edges'], res['upward_weight'], res['n_upward_unallowed'],
        len(res['sccs']), len(res['peer_cycles']), res['peer_back_weight']))
    if not q:
        for u in res['upward']:
            print('  UP %s%s (L%d) -> %s (L%d)  w=%.1f  %s' % ('' if not u['allowed'] else '[allowed] ', u['src'], layer(u['src']),
                  u['dst'], layer(u['dst']), u['weight'], ', '.join('%s:%.1f' % t for t in u['tu_pairs'][:4])))
        for c in res['peer_cycles']:
            print('  PEER CYCLE (best order low->high: %s) back-edge weight %.1f: %s' % (' < '.join(c['order_low_to_high']),
                  c['back_weight'], ', '.join('%s->%s %.1f' % (a, b, w) for w, a, b in c['back_edges'][:6])))
    bad_peer = [c for c in res['peer_cycles'] if not any((a, b) in allow for _, a, b in c['back_edges'])]
    return 1 if (res['n_upward_unallowed'] or bad_peer) else 0


if __name__ == '__main__':
    sys.exit(main(sys.argv))
