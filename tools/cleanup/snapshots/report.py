#!/usr/bin/env python3
"""Step 4: REPORT.md from classes.json, layouts.json, view_resolution.json."""
import os, sys, json, collections
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
import snapcfg
if __name__ == '__main__': snapcfg.parse(__doc__)
import common as C

H = C.WORK
CJ = json.load(open(os.path.join(H, 'classes.json')))
LJ = json.load(open(os.path.join(H, 'layouts.json')))
VJ = json.load(open(os.path.join(H, 'view_resolution.json')))
cl, lay, sm = CJ['classes'], LJ['classes'], VJ['summary']
out = []
w = out.append

snaps = CJ['snapshots']
w('# Snapshot-driven class identification for the type-c views\n')
w('Scripts: tools/cleanup/snapshots/ snapshot_objects.py (step 1), infer_layout.py (step 2), resolve_views.py '
  '(step 3, with cparse.py), report.py (this file); shared loaders in common.py. Outputs: classes.json, layouts.json, '
  'view_resolution.json. Game-derived data (ram/, objects/) stays in the work directory ' + C.WORK + '.\n')
w('## Inputs\n')
w(f'- {len(snaps)} distinct RAM images (by md5) in {C.RAMDIR}.')
lv = collections.Counter(m['level'] for m in snaps.values())
w(f'- {len(lv)} distinct levels by g_sw.currentLevel.\n')

w('## Step 1: object discovery\n')
s1 = CJ['summary']
w('- Class descriptor table D_002BAAE8 (CLASSDESC[199]: parent, cid, last cid, instance size, pfnNew) gives the '
  f'class tree and sizes; vtables are the {s1["n_vtables"]} primary `_vt$` tables (plus unnamed tables whose slot 6 is a descriptor getter). '
  'The vptr is at +0xC under BASIC; for the others the position is derived from the header root (AC/ACG +0, ACT +0x18, '
  'RIP family +0xEC, streams +0x30).')
w('- Discovery = whole-RAM scan for vtable words (object = word address - vptr offset), cross-checked against '
  'g_sw.objectsByClass[199] (SL lists walked from g_sw): in every snapshot each listed object carries a vtable of its '
  'list\'s class subtree (checked per snapshot by snapshot_objects.py). The scan also finds objects SW never lists '
  '(ASEGA/SMA records in slot heaps, AC/ACT animation curves, RIP particles, UI/screen objects) and some stale copies '
  'in freed memory; step 2 uses the listed instances where a class has any.')
w(f'- **{s1["classes_seen"]} of {s1["classes_with_vtable"]} vtable classes seen alive**; '
  f'{len(s1["registry_classes_unseen"])} of 199 registry CIDs have no instance under their own vtable (abstract bases '
  'such as BASIC/STEP/STEPGUARD/BHV, and CIDs absent from these levels: see the end of this report).\n')
w('| class | cid | instances (all snaps) | listed | snapshots | size |')
w('|---|---:|---:|---:|---:|---:|')
for c in sorted(cl.values(), key=lambda c: -c['instances_total'])[:25]:
    w(f"| {c['label']} | {c['cid'] if c['cid'] is not None else '-'} | {c['instances_total']} | {c['listed_total']} | "
      f"{c['snapshots_seen']} | {hex(c['size'] or c['header_size'] or 0)} |")
w('\nFull per-class, per-snapshot counts: classes.json.\n')

w('## Step 2: field typing\n')
ag = LJ['header_ptr_agreement']
pp = ag.get('ptr/inferred_ptr', 0); pn = ag.get('ptr/inferred_nonptr', 0)
np_ = ag.get('nonptr/inferred_ptr', 0); nn = ag.get('nonptr/inferred_nonptr', 0)
w(f'- Every 4-byte slot of {len(lay)} classes classified over up to 300 instances per snapshot: zero, vptr, object '
  'pointer (target class from its vptr), interior pointer (class+offset), code pointer, pointer to a global symbol, '
  'other heap pointer, small int, -1, float (1e-5..1e7), other bits.')
w(f'- Check against named header fields with >= 5 non-zero values: pointer fields seen as pointers '
  f'{pp}/{pp + pn} ({100 * pp / max(pp + pn, 1):.0f}%); non-pointer fields seen as pointers {np_}/{np_ + nn} '
  f'({100 * np_ / max(np_ + nn, 1):.1f}%). Most of the misses are -1 sentinels in pointer fields (GOMER knockdown '
  'reactions), STREAMSND/BANKSND (scan-only instances, likely stale) and BUTTON\'s rarely-set push list.')
props = [(l, p) for l, L in lay.items() if L['in_headers'] for p in L['proposed_fields']]
w(f'- **Proposed header fields**: {len(props)} slots in {len(set(l for l, _ in props))} header classes where the header '
  'has a pad/unknown (or nothing) and the snapshots show one consistent non-zero kind (>= 3 non-zero values, >= 80% '
  'agreement). Classes without a header add '
  f'{sum(len(L["proposed_fields"]) for L in lay.values() if not L["in_headers"])} more. All in layouts.json '
  '(`proposed_fields`; each slot\'s raw counts, targets, symbols, float/int samples in `slots`).\n')
# best proposals: object pointers with strong evidence first
rank = {'obj': 0, 'ptr': 1, 'global': 2, 'code': 2, 'objint': 3, 'heap': 4, 'float': 5, 'negint': 6, 'int': 7, 'other': 8}
def pk(t):
    for k, v in (('*', 'obj'), ('interior', 'objint'), ('ptr to', 'global'), ('pfn', 'code'), ('heap', 'heap'),
                 ('float', 'float'), ('-1', 'negint'), ('int', 'int')):
        if k in t: return rank[v]
    return 9
props.sort(key=lambda x: (pk(x[1]['inferred']), -x[1]['evidence']))
w('Strongest proposals (object/global/code pointers first, by evidence; 50 of them):\n')
w('| class | offset | header now | inferred | conf | evidence |')
w('|---|---:|---|---|---:|---:|')
for l, p in props[:50]:
    w(f"| {l} | 0x{p['off']:X} | {p['header'] or '(not covered)'} | {p['inferred']} | {p['conf']} | {p['evidence']} |")
# header conflicts: known non-pointer fields that hold pointers
conf = []
for l, L in lay.items():
    for e in L['slots']:
        h = e.get('header')
        if h and h['known'] and not h['isptr'] and e['nonzero'] >= 20 and e['kind'] in ('obj', 'ptr', 'objint', 'global') and e['conf'] >= 0.9:
            conf.append((l, e))
w(f'\nHeader fields typed non-pointer that hold pointers in every snapshot ({len(conf)}, >= 20 values, >= 90%):\n')
w('| class | offset | header field (type) | seen | targets |')
w('|---|---:|---|---|---|')
for l, e in sorted(conf, key=lambda x: -x[1]['nonzero'])[:20]:
    h = e['header']
    w(f"| {l} | 0x{e['off']:X} | {h['name']} ({h['type']}) | {e['kind']} x{e['nonzero']} | "
      f"{', '.join(list(e.get('targets', {}))[:3]) or ', '.join(map(str, list(e.get('syms', {}))[:2]))} |")

w('\nThe most frequent conflict is LO+0x2C: the header has `char cpaloFindSwObjects; uchar visibilityMask` there, '
  'but in most LO descendants the word holds a pointer (often an interior pointer to another object, or a global): '
  'worth checking against the code that writes it before trusting either.\n')
w('Runtime: steps 1-3 together take under a minute for 89 images (numpy); no model tokens.\n')
w('\n## Step 3: view resolution\n')
w('Methods (all mechanical; LLM time only for the residue):')
w('- **a** vtjoin (`this` of a virtual / one caller level).')
w('- **b** typed field or global: the view pointer is `(V *)EXPR`, `V *x = EXPR` or a member `V *m` of another view; '
  'EXPR is walked from a typed base (header type, g_sw and the config globals, a local view resolved earlier) to the '
  'final pointer slot, whose pointee class is read from the snapshots (step-2 targets aggregated over the holder\'s '
  'descendants; LCA of targets >= 3%). `b-static` = the snapshot slot holds non-objects or the holder has no instances, so '
  'the field\'s declared header type is used. Extern views of globals read the object at / pointed to by the address '
  'in every snapshot.')
w('- **c** layout compatibility: pointer and float members (int members are not informative: views declare unknown '
  'words as int) must fit the slot statistics of a class (pointer share >= 80%, typed pointers must hit the declared '
  'class subtree, float share >= 60%); resolved to the LCA of the top-scoring classes when that LCA is itself big '
  'enough; classes with < 3 instances excluded.')
w('- **d** (added; not snapshot based) prototype unification: V is parameter k of a global function (asm label, '
  'mangled name or config symbol) whose other declarations in any file, or whose g++ mangled name, give a header class '
  'or an already-resolved view at k. Only used when all such types are related.')
w('Iterated 4 rounds: views resolved by a/b/d (and high-confidence c) become typed bases/holders for the next round. '
  'Final pick: the most derived class when all methods agree (related in the class tree), else priority a > b > d > '
  'b-static > c and confidence `conflict`.\n')
cres = VJ['views']
w(f"**Coverage: {sm['resolved_any']} of {sm['views_c']} type-c views get a class** "
  f"({100 * sm['resolved_any'] / sm['views_c']:.0f}%); high {sm['conf'].get('high', 0)}, medium {sm['conf'].get('medium', 0)}, "
  f"low {sm['conf'].get('low', 0)}, conflict {sm['conf'].get('conflict', 0)}.\n")
w('| final method | views |')
w('|---|---:|')
for k, v in sorted(sm['resolved_by_final_method'].items(), key=lambda x: -x[1]): w(f'| {k} | {v} |')
mh = sm['method_hits']
w(f"\nMethod hits (a view can have several): a {mh['a']}, b {mh['b']}, c {mh['c']} (+{mh['c_ambiguous']} ambiguous), "
  f"d {mh['d']}; {mh['embedded']} views resolve to an embedded record or non-object pointee (address recorded, no class).\n")
w('| milestone | views | resolved | high+medium |')
w('|---|---:|---:|---:|')
for ms, d in sm['by_milestone'].items(): w(f"| M{ms} | {d['total']} | {d['resolved']} | {d['high_med']} |")
# field-reached
fr = [r for r in cres if 'b' in r['methods'] or 'b_embedded' in r['methods']]
w(f"\nField/global-reached views (a source expression or holder member was found and walked): {len(fr)}; "
  f"{sum(1 for r in fr if r['methods'].get('b', {}).get('cls'))} got a class through b.\n")
acc = sm['accuracy']
w('### Accuracy check (census tier A views of classes seen alive)\n')
w(f"{acc['n']} tier-A definitions whose census type is a class with instances, run through b/c/d without their type:\n")
w('| method | exact | related (same chain) | different | unresolved |')
w('|---|---:|---:|---:|---:|')
for m in ('b', 'c', 'd', 'final'):
    a = acc[m]; w(f"| {m} | {a.get('exact', 0)} | {a.get('related', 0)} | {a.get('wrong', 0)} | {a.get('unresolved', 0)} |")
w('\nThe tier-A labels are themselves heuristic (name or layout match plus a usage hint), so "different" is an upper '
  'bound on errors. A hand check of 11 disagreements: 8 favour the pipeline from the code itself (hud.cpp BIS is a '
  'CBinaryInputStream: the mangled method name says so; stepplayer.cpp\'s local `SO` is passed where step.cpp declares '
  'ASEGA; proxy.cpp N is PXR in two declarations; hurt.cpp C/P go to SetJtHurtState(JTHURT *); guardbhv.cpp SCALE '
  'comes from STEPGUARD+0xFC4, an ACT1 in 97% of 710 values; tn.cpp COEF, mark.cpp NODE (BSP), screen.cpp SO (JOB) '
  'follow typed prototypes); 3 are unclear; none clearly favours the census label. Layout matching (c) is the '
  'cleanest when it fires (14 exact, 1 related, 2 different) but fires rarely. **Use d and b-static answers as '
  '"is-a" bounds (often a base class), not exact classes.**')
ca = sm['candidate_agreement']
w(f"\nAgreement with ptr64's census `candidate_type` guesses on the resolved views: {ca}. The candidate types come "
  'from layout coincidence and mostly disagree; this pipeline\'s answers should replace them.\n')

w('### Sanity\n')
n = bad = 0
for r in cres:
    f = r['final']
    if f and r['size']:
        sz = lay.get(f['cls'], {}).get('size') or C.LAYOUT.get(f['cls'], {}).get('size')
        if sz: n += 1; bad += r['size'] > sz
w(f'- {bad} of {n} resolved views are larger than the resolved class\'s sizeof: typically a base-class bound (b-static, '
  'd) for a view of a derived class; these need the derived class (check view_resolution.json, `final.method`).\n')

w('## Unresolved views and what would help\n')
w('Remaining views by reason (one reason per view, first match):\n')
w('| reason | views |')
w('|---|---:|')
for k, v in sm['unresolved_reasons'].items(): w(f'| {k} | {v} |')
w('\n- Param-only views need call-graph propagation beyond vtjoin\'s one level (callers\' argument expressions), or '
  'a runtime trace (PINE breakpoint on the function: read a0..a3 and the pointee\'s vptr). That is the cheapest way to '
  'cover the largest bucket.')
w('- Views that are members of unresolved views resolve as soon as their holder does (the loop already chains).')
import resolve_views as RV
seen = [k for k, v in cl.items() if v['instances_total'] > 0]
unseen = [c['name'] for c in C.CLASSES.values() if not any(c['name'] in RV.ch(x) for x in seen)]
cand_unseen = collections.Counter()
for r in cres:
    if not r['final'] and r.get('candidate_type') and r['candidate_type'] in unseen: cand_unseen[r['candidate_type']] += 1
w(f"- Registry classes with no instance anywhere in their subtree ({len(unseen)} of 199; the other "
  f"{len(CJ['summary']['registry_classes_unseen']) - len(unseen)} unseen-by-own-vtable CIDs are abstract bases whose "
  f"descendants are seen): {', '.join(unseen)}.")
w(f"- Unresolved views whose census candidate is one of them: {dict(cand_unseen.most_common(15)) or 'none'}.")
w('- Snapshots that add the most cover the unseen classes above: states inside the situations that use them '
  '(behaviour objects only exist while pushed), not more images of levels already seen.\n')
w('## Files\n')
w('- snapshot_objects.py, infer_layout.py, resolve_views.py (+ cparse.py, common.py), report.py')
w('- classes.json (classes, counts per snapshot, snapshot metadata), layouts.json (per class slots + proposed fields), '
  'view_resolution.json (per view: methods with evidence, final class, confidence, census check)')
w('- ram/, objects/: game data, work directory only (never into a repo).')
open(os.path.join(H, 'REPORT.md'), 'w').write('\n'.join(out) + '\n')
print('ok')
