"""Shared loader for the docs generator (v2): TU spans, function names, class names, source paths.

Inputs (all read-only):
  master_names_v2/master_names.csv   one row per function address (final_name, tier, confidence, purpose, tu, class, sources, notes)
  master_names_v2/tu_map.csv         212 TU spans (static-init boundaries, _GLOBAL_$D off-by-one already corrected)
  master_names_v2/existing_name_changes.csv   renames of names already in config/symbol_addrs.txt
  master_names_v2/PENDING_FIXES.txt  -> PENDING below (hand-transcribed; apply-before-rename fixes from master_names_v2_1/REVIEW.txt)
  top50_round/names.csv              top-50 naming round (merged per PENDING_FIXES: conf >= 0.6 -> T1, else T2)
  sly2-decomp config/symbol_addrs.txt, config/matched.csv
"""
import os as _os
_REPO = _os.path.abspath(_os.path.join(_os.path.dirname(__file__), '..', '..'))
_RESEARCH = _os.environ.get('SLY2_RESEARCH', _os.path.join(_REPO, '..', 'research'))

import csv, re, os
R = _RESEARCH
DEC = _REPO
MN = f'{R}/master_names_v2'

def spans():
    out = []
    for r in csv.DictReader(open(f'{MN}/tu_map.csv')):
        out.append(dict(lo=int(r['start'], 16), hi=int(r['end'], 16), n=int(r['nfuncs'] or 0), label=r['label'],
                        lconf=r['label_confidence'], early=r['tu_table_label'], tunote=r['note']))
    return out

# PENDING_FIXES.txt (master_names_v2), transcribed. addr -> (name or None to keep, tier or None, purpose suffix)
PENDING = {
    0x2281E8: ('InitExit', None, 'EXIT slot 7 (sets +0x150 = 0x189 oidWarp, +0x154 = -1); was InitWarp'),
    0x2280D0: ('PexitDefault', None, 'returns the EXIT (0x1A) whose +0x130 (fDefault) is set; was PwarpDefault'),
    0x228358: ('SetExitExits', None, 'Sly 1 name; holds the hands-off state, g_sw+0x3C84 = this; was SetWarpState'),
    0x2284E0: ('AddExitRsmg', None, 'optid 527 on EXIT; fields +0x164/+0x168 inside the 0x190-byte EXIT; was AddWarpRsmgSm'),
    0x10B850: ('RefThunk_AddExitRsmg', None, 'thunk of AddExitRsmg'),
    0x228750: (None, None, 'WARP method (uses WARP RSMG +0xE0/+0xE4, list +0x124, pairs +0x14C, then resets all EXITs); Sly 1 candidate name TriggerWarp'),
    0x228170: ('TriggerCid41', 'T2', 'TRIGGER (optid 235) of CID41: MatchStepXf(current player, this) then Splice event 5 = in-level teleport destination'),
    0x227FE8: (None, None, 'purpose fix: the class lookup falls back to cid 0x2A (WARP) alias list +0x16C, not 0x1A'),
    # runtime6 FINDINGS section 8 (name corrections, runtime + code)
    0x20F0C0: ('IncrementSwNoPause', None, 'runtime6: SW+0x3D7C is the no-pause counter (SetJsgHandsOff, JT mode kind 1); Lilly jtmode.c name'),
    0x20F0F0: ('DecrementSwNoPause', None, 'runtime6: inverse of IncrementSwNoPause'),
    0x142540: ('StartStepplayerFollow', None, 'runtime6: Splice optid 446 on STEPPLAYER: clear stack, push BHV194 (follow leader +0x30, params +0x40/+0x44), Start'),
    0x10A940: ('RefThunk_StartStepplayerFollow', None, 'runtime6: optid 446 thunk on STEPPLAYER'),
}

def top50():
    out = {}
    for r in csv.DictReader(open(f'{R}/top50_round/names.csv')):
        try:
            a = int(r['addr'], 16); c = float(r['confidence'])
        except Exception:
            continue
        out[a] = dict(name=r['proposed_name'], c=c, purpose=r['purpose'], tier='T1' if c >= 0.6 else 'T2')
    return out

def evidence_tags(sources, notes):
    s = sources or ''
    tags = []
    if 'runtime-corrected' in s or 'runtime-corrected' in (notes or ''):
        tags.append('runtime-corrected')
    if re.search(r'runtime(?!-corrected)', s):
        tags.append('runtime')
    if 'parse-verified' in s:
        tags.append('parse')
    elif 'parse-code' in s:
        tags.append('parse-code')
    if 'verdicts' in s:
        tags.append('verified')
    if 'arbitration' in s:
        tags.append('arbitration')
    if 'table-verified' in s:
        tags.append('table')
    if 'whole_game' in s:
        tags.append('whole-game')
    if 'format-doc' in s or 'meshes(' in s:
        tags.append('format-doc')
    return tags

def name_rows():
    """addr -> dict(name, purpose, c, tier, tu, cls, tags, cur, note)"""
    rows = {}
    for r in csv.DictReader(open(f'{MN}/master_names.csv')):
        a = int(r['addr'], 16)
        try: c = float(r['confidence'])
        except Exception: c = None
        rows[a] = dict(a=a, name=r['final_name'], purpose=r['purpose'], c=c, tier=r['tier'], tu=r['tu'], cls=r['class'],
                       tags=evidence_tags(r['sources'], r['notes']), cur=r['current_name'], note='')
    for a, t in top50().items():
        row = rows.setdefault(a, dict(a=a, name='', purpose='', c=None, tier='NONE', tu='', cls='', tags=[], cur='', note=''))
        if t['name'] and t['name'] != row['name']:
            row['note'] = f"top50 round; v2 name {row['name']}" if row['name'] else 'top50 round'
            row['name'] = t['name']
        row['c'] = t['c']; row['tier'] = t['tier']; row['purpose'] = t['purpose'] or row['purpose']
        row['tags'] = row['tags'] + ['top50']
    for a, (nm, tier, extra) in PENDING.items():
        row = rows.get(a)
        if not row: continue
        if nm and nm != row['name']:
            row['note'] = f"pending fix; v2 name {row['name']}"
            row['name'] = nm
        if tier: row['tier'] = tier
        row['purpose'] = (row['purpose'] + ' | ' if row['purpose'] else '') + extra
        row['tags'] = row['tags'] + ['pending-fix']
    return rows

def existing_changes():
    out = {}
    for r in csv.DictReader(open(f'{MN}/existing_name_changes.csv')):
        out[int(r['addr'], 16)] = r
    return out

def class_names():
    """cid -> v2 class word, from the 199 Get<Cls>ClassDesc getters (vtable slot 6, table-verified)."""
    out = {}
    for r in csv.DictReader(open(f'{MN}/master_names.csv')):
        m = re.match(r'Get(\w+)ClassDesc$', r['final_name'])
        p = re.search(r'\(cid (\d+)\)', r['purpose'])
        if m and p:
            out[int(p[1])] = m[1].upper()
    return out

def symbols():
    out = {}
    for line in open(f'{DEC}/config/symbol_addrs.txt'):
        m = re.match(r'^(\S+) = 0x([0-9A-Fa-f]+);', line)
        if m: out[int(m[2], 16)] = m[1]
    return out

def sources():
    by_addr = {}
    for r in csv.DictReader(open(f'{DEC}/config/matched.csv')):
        by_addr[int(r['addr'], 16)] = r['source']
    return by_addr

TRIVIAL = re.compile(r'^(__static_initialization|StaticInit_|_GLOBAL_|junk|\(orphan|\(padding|\(tail of|\(empty|not a function|\(identity)', re.I)
