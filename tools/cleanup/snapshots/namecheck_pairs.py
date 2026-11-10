"""Pass 3: paired-state semantic checks. Compares named fields across snapshots captured in known situations (what
the player did between two states: collected a coin, jumped, got hurt, alerted a guard...). Which snapshot is which
situation comes from a spec JSON (--spec; keys as in DEFAULT_SPEC below, values are snapshot names as listed by
common.snapshots()). Missing snapshots are skipped. Output: WORK/namecheck/pairs.json, merged into namecheck.json
as "paired_states" (run after the check pass). Run through namecheck.py."""
import sys, os, re, json, collections
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
import numpy as np
import snapcfg
A = snapcfg.parse(__doc__, lambda ap: ap.add_argument('--spec', help='paired-state spec JSON (default: DEFAULT_SPEC)'))
from namecheck_check import flat, is_unk, chain, L, CACHE, HERE

# The capture set this was written for (one snapshot per situation, named S01..). Replace with --spec for yours.
DEFAULT_SPEC = {
    'coins': {'S02': 6, 'S03': 6, 'S04': 9, 'S05': 9, 'S10': 22, 'S11': 23},      # snapshot -> coin count on screen
    'clue_bottles': {'S02': 0, 'S03': 0, 'S04': 0, 'S05': 1},                    # snapshot -> bottles collected
    'jump': {'S12': '+', 'S13': '0/-', 'S14': '+', 'S15': '+', 'S16': '+?', 'S19': '-', 'S20': '0', 'S21': '0'},
    'jump_states': ['S12', 'S13', 'S15', 'S19', 'S20', 'S21'],
    'speed': {'JT': ['S20', 'S21', 'S69'], 'MURRAY': ['S33', 'S32', 'S34']},      # class -> [run, walk, idle]
    'jump_murray': {'S44': '+', 'S45': '-', 'S46': '?'},
    'hurt_states': ['S25', 'S26', 'S27', 'S28', 'S29', 'S30', 'S64'],
    'hurt_pairs': [['S26', 'S27'], ['S27', 'S28']],                               # [before, after] hurt / death
    'guard_pairs': [['S06', 'S07'], ['S08', 'S09']],                              # [calm, alerted] guards
    'murray_states': ['S32', 'S33', 'S34', 'S37', 'S44', 'S45'],
    'timestamps': {'S12': 'JT', 'S13': 'JT', 'S20': 'JT', 'S21': 'JT', 'S27': 'JT', 'S28': 'JT',
                   'S32': 'MURRAY', 'S33': 'MURRAY', 'S34': 'MURRAY'},
}
SPEC = dict(DEFAULT_SPEC, **(json.load(open(A.spec)) if A.spec else {}))

def load(sn):
    p = os.path.join(CACHE, sn + '.npz')
    return np.load(p) if os.path.exists(p) else None
Z = {os.path.basename(f)[:-4]: None for f in os.listdir(CACHE) if f.endswith('.npz')}
Z = {k: load(k) for k in Z}
def f32(w): return float(np.array([w], np.uint32).view(np.float32)[0])
def s32(w): return int(np.array([w], np.uint32).view(np.int32)[0])
def clock(sn): return f32(Z[sn]['M_g_clock'][0, 0])

def fields_at(t, off):
    return [p for (o, sz, nm, ty, isp, decl, ro, p) in flat(t) if o <= off < o + max(sz, 1)]
def scalar_fields(t):
    out = []
    for (o, sz, nm, ty, isp, decl, ro, p) in flat(t):
        if isp or o % 4 or sz != 4 or ty not in ('int', 'uint', 'float') and not re.match(r'^[A-Z]+$', ty): continue
        if ty in L: continue
        out.append((o, p, ty, decl))
    return out
def val(M, o, ty): return f32(M[o // 4]) if ty == 'float' else s32(M[o // 4])

R = {}
# ---- T1 coins / clue bottles (globals)
exp = SPEC['coins']
r = dict(expected=exp, field_at_0xA0=fields_at('SAVEDATA', 0xA0),
         observed={s: s32(Z[s]['M_g_gs'][0, 0xA0 // 4]) for s in exp if s in Z})
# which named g_gs / g_sw ints track the coin count exactly
for g, t in (('g_gs', 'SAVEDATA'), ('g_sw', 'SW'), ('g_save', 'SAVE')):
    hits = [p for (o, p, ty, d) in scalar_fields(t) if ty != 'float' and exp and all(s in Z and s32(Z[s]['M_' + g][0, o // 4]) == v for s, v in exp.items())]
    r['exact_matches_' + g] = hits
R['coins'] = r
exp = SPEC['clue_bottles']
R['clue_bottles'] = dict(expected_collected=exp, fields_0x4764=fields_at('SW', 0x4764), fields_0x4768=fields_at('SW', 0x4768),
                         observed={s: (s32(Z[s]['M_g_sw'][0, 0x4764 // 4]), s32(Z[s]['M_g_sw'][0, 0x4768 // 4])) for s in exp if s in Z},
                         afClue_field=fields_at('SAVEDATA', 0x278),
                         afClue={s: hex(int(Z[s]['M_g_gs'][0, 0x278 // 4])) for s in exp if s in Z})

# ---- player helpers
def player(sn, lab):
    z = Z.get(sn)
    if z is None or 'M_' + lab not in z.files: return None
    return z['M_' + lab][0], (z['P%X_%s' % (0xE8, lab)][0] if 'PE8_' + lab in z.files else None)
def vel(sn, lab):
    p = player(sn, lab)
    if p is None or p[1] is None: return None
    return [round(f32(w), 3) for w in p[1][0x60 // 4: 0x60 // 4 + 3]]
def named_vals(sn, lab, pat):
    p = player(sn, lab)
    if p is None: return {}
    return {pth: round(val(p[0], o, ty), 3) for (o, pth, ty, d) in scalar_fields(lab) if re.search(pat, pth.split('.')[-1]) and not is_unk(pth.split('.')[-1])}

STATEPAT = r'([sS]tate$|[kK]ind$|[mM]ode$)'
# ---- T3 jump (vertical velocity sign), T4 run vs walk
jump = SPEC['jump']
R['jump'] = dict(expected_vz=jump, sophys_velocity={s: vel(s, 'JT') for s in jump},
                 jt_states={s: named_vals(s, 'JT', STATEPAT) for s in SPEC['jump_states']})
for lab, (fast, slow, idle) in SPEC['speed'].items():
    vv = {s: vel(s, lab) for s in (fast, slow, idle)}
    sp = {s: (round(float(np.hypot(v[0], v[1])), 3) if v else None) for s, v in vv.items()}
    R['speed_' + lab] = dict(run=fast, walk=slow, idle=idle, velocity=vv, horiz_speed=sp,
                             named_speed={s: named_vals(s, lab, r'([sS]peed|[vV]el)') for s in (fast, slow, idle)},
                             states={s: named_vals(s, lab, STATEPAT) for s in (fast, slow, idle)})
R['jump_MURRAY'] = dict(expected=SPEC['jump_murray'], velocity={s: vel(s, 'MURRAY') for s in SPEC['jump_murray']})
# ---- T5 hurt / death
hp = r'([hH]p|[hH]ealth|[hH]it|[lL]ife|[dD]ead|[dD]ie|[dD]eath|[hH]urt|[dD]amage|[zZ]ap)'
R['hurt_death'] = {s: dict(named=named_vals(s, 'JT', hp), states={k: v for k, v in named_vals(s, 'JT', STATEPAT).items()
                                                                   if 'hurt' in k or k in ('state', 'bodyState', 'jtmode.state')})
                   for s in SPEC['hurt_states']}
# which JT named scalar fields change across each hurt pair
def changes(a, b, lab='JT'):
    pa, pb = player(a, lab), player(b, lab)
    out = []
    if pa is None or pb is None: return out
    for (o, pth, ty, d) in scalar_fields(lab):
        if is_unk(pth.split('.')[-1]): continue
        x, y = val(pa[0], o, ty), val(pb[0], o, ty)
        if x != y and not re.search(r'([tT]ime|^t[A-Z]|Matq|mat|pos|Pos)', pth): out.append((pth, round(x, 3), round(y, 3)))
    return out
R['hurt_changes'] = {'%s->%s' % (a, b): [c for c in changes(a, b) if re.search(r'(state|State|kind|Kind|mode|Mode|hp|Hit|hurt)', c[0])][:30]
                     for a, b in SPEC['hurt_pairs']}
# ---- T6 guards: same-address guard objects, which named STEPGUARD-chain fields change across each guard pair
GL = [k for k in L if 'STEPGUARD' in chain(k)]
def guard_changes(a, b):
    out = collections.Counter(); ex = {}
    za, zb = Z.get(a), Z.get(b)
    if za is None or zb is None: return []
    for lab in GL:
        if 'M_' + lab not in za.files or 'M_' + lab not in zb.files: continue
        A, B = za['A_' + lab], zb['A_' + lab]
        for i, ad in enumerate(A):
            j = np.nonzero(B == ad)[0]
            if not len(j): continue
            ma, mb = za['M_' + lab][i], zb['M_' + lab][j[0]]
            for (o, pth, ty, d) in scalar_fields(lab):
                if d not in ('STEPGUARD', 'GUARD', lab) or is_unk(pth.split('.')[-1]): continue
                x, y = val(ma, o, ty), val(mb, o, ty)
                if x != y:
                    out[pth] += 1; ex.setdefault(pth, (lab, round(x, 3), round(y, 3)))
    return [(k, n) + ex[k] for k, n in out.most_common() if not re.search(r'([tT]ime$|^t[A-Z]|Matq|^mat|[pP]os|Heading$|[pP]hase$)', k)][:25]
R['guard_alert'] = {'%s->%s' % (a, b): guard_changes(a, b) for a, b in SPEC['guard_pairs']}
R['guard_alert']['field_at_0x2C0'] = fields_at('STEPGUARD', 0x2C0)
# ---- T7 Murray states
R['murray_states'] = {s: named_vals(s, 'MURRAY', STATEPAT) for s in SPEC['murray_states']}
# ---- T8 timestamp sanity on player: named *Time fields <= clock, and the one for the current state recent
ts = {}
for s, lab in SPEC['timestamps'].items():
    if s not in Z: continue
    nv = named_vals(s, lab, r'(StartTime|EnterTime|enterTime|startTime)$')
    c = clock(s)
    ts[s] = dict(clock=round(c, 3), age={k: round(c - v, 3) for k, v in nv.items() if v})
R['timestamps'] = ts
json.dump(R, open(os.path.join(HERE, 'pairs.json'), 'w'), indent=1, default=str)
for k, v in R.items(): print(k, json.dumps(v, default=str)[:900])
# merge into namecheck.json (rerun after check.py)
NC = json.load(open(os.path.join(HERE, 'namecheck.json'))); NC['paired_states'] = R
json.dump(NC, open(os.path.join(HERE, 'namecheck.json'), 'w'), indent=1, default=str)
