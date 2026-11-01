"""Generate reference/tu-map.md, reference/functions/*.md and expand {{...}} placeholders in page templates (src/*.md -> docs/engine/*.md).
v2: names come from master_names_v2 (T1/T2 tiers, evidence tags) plus the pending fixes and the top-50 round (see load.py).
Run: python3 -I gen.py   (inputs are read-only; output goes to ../docs)"""
import sys, os, re, csv, collections
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
import load
HERE = os.path.dirname(os.path.abspath(__file__))
D = os.path.join(HERE, '..', '..')   # repo root
DOCS = os.path.join(D, 'docs')

PAGES = {
 'object-model': 'Object model (BASIC/LO/XFM, proxies, lookup)',
 'world-sw': 'World (SW), level load and transitions',
 'main-loop-startup': 'Main loop, startup, clock and input',
 'splice-scripting': 'Splice scripting runtime',
 'rendering': 'Rendering',
 'camera': 'Camera',
 'player': 'Player characters',
 'guards-ai': 'Guards and AI',
 'game-progression': 'Game progression',
 'save-memcard': 'Save data and memory card',
 'sound': 'Sound',
 'hud-ui': 'HUD and UI',
 'particles-effects': 'Particles and effects',
 'physics-collision': 'Physics, collision and navigation',
 'animation': 'Animation',
 'cutscenes': 'Cutscenes, scripted sequences and movies',
 'core-library': 'Core library',
 'gameplay-objects': 'Gameplay object classes',
}
# reference-only function lists (no engine page)
REFPAGES = {
 'crt0-thunks': 'crt0, Splice binding thunks, class factories and descriptor getters (0x100000-0x1147D8)',
 'sdk-libraries': 'Sony SDK libraries and the 989snd tail (0x233000-0x254F00)',
}

SPAN = {
 0x100000: ('crt0 + Splice thunks + class factories', 'object-model', 'no static-init boundary inside; thunks in src/splicewrap'),
 0x1147D8: ('?', 'core-library', ''), 0x114AE0: ('?', 'core-library', ''),
 0x116918: ('heap', 'core-library', ''), 0x116CA0: ('dynarray?', 'core-library', 'name unknown, sorts before bez'),
 0x117030: ('bez', 'core-library', ''), 0x1177A8: ('bis', 'core-library', ''), 0x118668: ('bsp', 'core-library', ''),
 0x1195E0: ('crout', 'core-library', ''), 0x119BC8: ('tu_119BC8', 'core-library', 'small string class'),
 0x11A050: ('dl', 'core-library', ''), 0x11A550: ('dzg (part 1)', 'core-library', ''),
 0x11AFE0: ('tu_11AFE0', 'core-library', 'case-insensitive string hash'), 0x11B300: ('mat', 'core-library', ''),
 0x11CCA8: ('slotheap', 'core-library', ''), 0x11CF60: ('util', 'core-library', ''), 0x11E198: ('vec', 'core-library', ''),
 0x11F440: ('bif', 'splice-scripting', ''), 0x1258F0: ('bif2? (nil stubs)', 'splice-scripting', '29 nil-returning builtins'),
 0x126438: ('eval', 'splice-scripting', ''), 0x127B18: ('frame', 'splice-scripting', ''), 0x128138: ('gc', 'splice-scripting', ''),
 0x128B60: ('method? (empty)', 'splice-scripting', ''), 0x128CB8: ('pair', 'splice-scripting', ''), 0x128F70: ('proc', 'splice-scripting', ''),
 0x1291A8: ('ref', 'splice-scripting', ''), 0x12A3D8: ('serialize', 'splice-scripting', ''), 0x12A980: ('sidebag', 'splice-scripting', ''),
 0x12AE18: ('spliceutils', 'splice-scripting', ''), 0x12B0E8: ('varb', 'splice-scripting', ''),
 0x12B3B0: ('act?', 'animation', 'curves + actuators'), 0x12E600: ('tu_12E600 (cid 0x2C camera XFM)', 'camera', ''),
 0x12EB48: ('amb? (cid 0x2B)', 'animation', ''), 0x12F248: ('aseg', 'animation', ''), 0x1308A8: ('asega', 'animation', ''),
 0x1320C8: ('barrier', 'gameplay-objects', ''), 0x132570: ('bas', 'core-library', ''), 0x132B00: ('basic', 'object-model', ''),
 0x1336F0: ('bbmark', 'physics-collision', ''), 0x134648: ('bentley', 'player', ''), 0x137A30: ('blend', 'animation', ''),
 0x138F08: ('blip', 'particles-effects', ''), 0x13A9E8: ('bomb', 'gameplay-objects', ''), 0x13BDF0: ('booty?', 'game-progression', 'cid_table says ALARM: wrong'),
 0x13D558: ('bq', 'core-library', ''), 0x13DA18: ('brx_sw? (LoadSwFromBrx)', 'world-sw', 'file name unknown, sorts bq<x<brx'),
 0x13E368: ('brx', 'world-sw', ''), 0x13FF70: ('stepplayer TU (b[r-u]*)', 'player', 'Lilly: stepplayer_raw.c'),
 0x144638: ('jtgoal TU', 'player', ''), 0x144B28: ('player BHV TU', 'player', 'BHV168, BHV193-198'),
 0x1471C0: ('cid 0x68 TU', 'cutscenes', 'frame-capture transition'), 0x147E20: ('busy?', 'object-model', ''),
 0x148620: ('button', 'gameplay-objects', ''), 0x149A50: ('callback?', 'world-sw', ''), 0x14A008: ('route/chase TU', 'guards-ai', ''),
 0x14B660: ('tu_14B660 (file helpers)', 'core-library', ''), 0x14B7F8: ('cd', 'core-library', ''), 0x14BEA8: ('cheat', 'main-loop-startup', 'was labelled splotheap'),
 0x14C6A0: ('clip', 'physics-collision', ''), 0x14CFE0: ('clock', 'main-loop-startup', 'was labelled clip'),
 0x14D3D8: ('cm', 'camera', 'TU really ends 0x14FAC8 (0x14FAA8 = cm _GLOBAL_$D)'), 0x14FAA8: ('tu_14FAC8 (cid 0x40 guard)', 'guards-ai', 'starts at 0x14FAC8'),
 0x14FCA8: ('cplcy?', 'camera', ''), 0x1520C8: ('rhythm?', 'gameplay-objects', 'button-timing minigame'),
 0x1539A8: ('tu_1539A8', 'gameplay-objects', 'static-init only + 4-byte stub'), 0x153B08: ('tu_153B08 (cid 0x7A)', 'gameplay-objects', ''),
 0x153D58: ('detect?', 'guards-ai', 'detector cid 0x10'), 0x154A60: ('difficulty', 'game-progression', ''),
 0x1553F0: ('tu_1553F0 (cid 0x3B fleeing guard)', 'guards-ai', ''), 0x1564E8: ('noise? (stimuli)', 'guards-ai', ''),
 0x157200: ('dmas', 'rendering', ''), 0x157A50: ('draw', 'rendering', 'was labelled render'), 0x1585D8: ('dzg (part 2)', 'core-library', ''),
 0x159118: ('emitter', 'particles-effects', ''), 0x15CD68: ('tu_15CD68 (WIDGET131 episode menu)', 'hud-ui', ''),
 0x15DC80: ('tu_15DC80 (cid 0x74 timed LO)', 'gameplay-objects', ''), 0x15E380: ('find', 'object-model', 'was labelled font'),
 0x15EF00: ('font', 'hud-ui', ''), 0x1603B8: ('tu_1603B8 (XFM constraints)', 'object-model', 'sorts font<x<frm'),
 0x1607A0: ('frm', 'rendering', ''), 0x1613A8: ('game', 'game-progression', 'was labelled turret'), 0x166270: ('geom', 'physics-collision', ''),
 0x166920: ('gifs', 'rendering', ''), 0x167310: ('glob', 'rendering', 'was labelled ref'), 0x16F148: ('gomer', 'guards-ai', ''),
 0x16F2D0: ('gs', 'rendering', ''), 0x1709A8: ('tu_1709A8 (guard script cid 0x71)', 'guards-ai', ''),
 0x171F58: ('tu_171F58 (BHV171 search)', 'guards-ai', ''), 0x173158: ('tu_173158 (cid 0x77 spawner)', 'guards-ai', ''),
 0x174898: ('BHV classes TU', 'guards-ai', 'was labelled stepguard'), 0x17F090: ('tu_17F090 (cid 0x47 damageable PO)', 'gameplay-objects', 'was labelled stepguard'),
 0x182C70: ('grabspot TU', 'player', ''), 0x1848A8: ('tu_1848A8 (stream BIS)', 'core-library', ''), 0x184F50: ('hnd', 'player', ''),
 0x185130: ('tu_185130 (mesh surface cid 8/9)', 'player', ''), 0x186610: ('ik', 'animation', ''), 0x187030: ('joy', 'main-loop-startup', ''),
 0x188488: ('jt', 'player', 'was labelled joy'), 0x18E540: ('cane TU', 'player', ''), 0x18F908: ('hang TU', 'player', ''),
 0x191090: ('grab TU', 'player', ''), 0x195950: ('air/jump TU', 'player', ''), 0x19CBF8: ('mode TU', 'player', ''),
 0x19DE10: ('jtpipe TU (pipe context)', 'player', ''), 0x19F0A8: ('tu_19F0A8 (global player/guard broadcasts)', 'player', ''),
 0x19F3E8: ('places TU', 'player', ''), 0x1A0300: ('dirblend TU', 'animation', ''), 0x1A0B18: ('hurt TU', 'player', ''),
 0x1A2BD0: ('language', 'hud-ui', ''), 0x1A3020: ('letterbox', 'hud-ui', ''), 0x1A3260: ('light', 'rendering', ''),
 0x1A4E08: ('bootseq? (step queue)', 'world-sw', ''), 0x1A5270: ('lo', 'object-model', 'was labelled light'), 0x1A7518: ('main', 'main-loop-startup', ''),
 0x1A7C18: ('mark', 'physics-collision', ''), 0x1AA978: ('memcard', 'save-memcard', ''), 0x1AE130: ('memory', 'core-library', '0x1AE130 is memcard _GLOBAL_$D'),
 0x1AE928: ('menu?', 'hud-ui', 'was labelled step'), 0x1B0B78: ('tu_1B0B78 (cid 0x60)', 'gameplay-objects', ''), 0x1B1DB8: ('missile', 'gameplay-objects', ''),
 0x1B2820: ('mpeg', 'cutscenes', ''), 0x1B4438: ('ms', 'rendering', ''), 0x1B4728: ('murray', 'player', ''),
 0x1B9F80: ('tu_1B9F80 (cid 0x3C guard + BHV174)', 'guards-ai', ''), 0x1BAB50: ('tu_1BAB50 (cid 0x62 staged reactions)', 'gameplay-objects', ''),
 0x1BC050: ('tu_1BC050 (cid 0x0E + convex volumes)', 'physics-collision', 'was labelled jt'), 0x1BE398: ('tu_1BE398 (GS texture setup + cid 0x2D)', 'rendering', ''),
 0x1BF110: ('path', 'physics-collision', ''), 0x1C2A78: ('phasemem', 'world-sw', ''), 0x1C2D20: ('tu_1C2D20 (cid 0x63 launcher)', 'gameplay-objects', ''),
 0x1C3390: ('pipe (PIPE class)', 'player', ''), 0x1C3D70: ('po', 'player', ''), 0x1C4E50: ('prize', 'game-progression', 'was labelled alarm'),
 0x1C8CC0: ('prog', 'hud-ui', ''), 0x1C9940: ('proxy', 'object-model', ''), 0x1CB908: ('rail', 'player', ''),
 0x1CBEB0: ('tu_1CBEB0 (cid 0x3D guard)', 'guards-ai', ''), 0x1CC158: ('tu_1CC158 (jthands/reach)', 'player', ''),
 0x1CD5F0: ('render', 'rendering', ''), 0x1CDCD0: ('rip', 'particles-effects', ''), 0x1D1370: ('rog? (cid 0x4D RC car)', 'gameplay-objects', ''),
 0x1D5150: ('rumble', 'main-loop-startup', ''), 0x1D57A8: ('tu_1D57A8 (cull + sphere tree)', 'rendering', ''),
 0x1D6848: ('tu_1D6848 (launch kinematics)', 'physics-collision', ''), 0x1D7238: ('tu_1D7238 (character selector?)', 'player', ''),
 0x1D7B88: ('save', 'save-memcard', ''), 0x1D93D8: ('screen', 'hud-ui', 'was labelled gc'), 0x1DE280: ('sensor', 'gameplay-objects', 'starts 0x1DE2A0'),
 0x1DFAD8: ('relay TU (RELAY + JSG)', 'cutscenes', 'Lilly: jsg.c; was labelled stepguard'), 0x1E5D80: ('shape', 'physics-collision', ''),
 0x1EA568: ('shd', 'rendering', 'was labelled gs'), 0x1ED7A8: ('shdanim', 'rendering', ''), 0x1EE988: ('sjis', 'hud-ui', ''),
 0x1EEC60: ('bhvstk TU (BHV base)', 'guards-ai', ''), 0x1EF850: ('sky', 'rendering', ''), 0x1EFA90: ('sm', 'animation', ''),
 0x1F1768: ('pickpocket TU', 'guards-ai', ''), 0x1F2108: ('so', 'physics-collision', ''), 0x1F8DD8: ('solve', 'physics-collision', 'was labelled so'),
 0x1FAF70: ('sound', 'sound', ''), 0x1FF398: ('tu_1FF3B8 (cid 0x27)', 'gameplay-objects', ''), 0x1FF7E8: ('splicemap? (Splice load)', 'splice-scripting', ''),
 0x1FFCF8: ('spliceobj', 'splice-scripting', ''), 0x201620: ('sprbuf', 'core-library', ''), 0x201D78: ('tu_201D78 (recon photo prompt)', 'player', ''),
 0x203948: ('step', 'guards-ai', ''), 0x205E70: ('stepguard', 'guards-ai', ''), 0x20B840: ('tu_20B840 (GAIT/LEAP/DIRANIM)', 'animation', ''),
 0x20CE28: ('tu_20CE28 (guard watchers)', 'guards-ai', ''), 0x20D5D0: ('stream', 'core-library', ''),
 0x20D9D0: ('sw', 'world-sw', 'was labelled button'), 0x210E90: ('hud TU (WIDGET, sprites, rich text)', 'hud-ui', 'starts 0x210EB0; was labelled screen'),
 0x21BB88: ('trail? (cid 0x69)', 'particles-effects', ''), 0x21C820: ('tail? (rope chains)', 'animation', 'Lilly: chain.c'),
 0x21D278: ('target', 'gameplay-objects', ''), 0x21D638: ('text', 'core-library', ''), 0x21EFD0: ('thread', 'core-library', ''),
 0x21F2E0: ('throw? (JTTHROW)', 'player', ''), 0x220D40: ('tn', 'camera', ''), 0x223F70: ('tu_223F70 (cid 0x64-0x66)', 'gameplay-objects', ''),
 0x2259D8: ('turret', 'gameplay-objects', ''), 0x226550: ('update', 'world-sw', ''), 0x226C10: ('vifs', 'rendering', ''),
 0x227350: ('vis? (cid 5)', 'rendering', ''), 0x2275B8: ('voice? (subtitle helpers)', 'sound', ''), 0x227BB8: ('vol', 'physics-collision', ''),
 0x227FE8: ('warp? (EXIT/WARP)', 'world-sw', 'was labelled stepguard'), 0x228B48: ('water', 'physics-collision', ''),
 0x229EF0: ('waypoint', 'game-progression', ''), 0x22B040: ('wipe', 'world-sw', ''), 0x22B720: ('wr', 'animation', ''),
 0x22C818: ('xf', 'animation', 'Lilly: xf.c'), 0x231100: ('xform', 'object-model', ''), 0x2319E8: ('989snd (Sony lib)', 'sound', ''),
}

# master_names_v2/tu_map.csv corrected five span starts (the _GLOBAL_$D stub belongs to the previous TU) and split the SDK.
SPAN_ALIAS = {0x14FAC8: 0x14FAA8, 0x1AE150: 0x1AE130, 0x1DE2A0: 0x1DE280, 0x1FF3B8: 0x1FF398, 0x210EB0: 0x210E90}
SPAN_EXTRA = {0x233000: ('sdk_libs', 'sdk-libraries', 'Sony libraries'), 0x254F00: ('game_linkonce', 'core-library', 'inline/template helpers (smart pointers, VECTOR ops)')}
PAGE_OVERRIDE = {0x100000: 'crt0-thunks', 0x227FE8: 'world-sw'}

def page_link(pg, depth):
    up = '../' * depth
    if pg in REFPAGES:
        return f"{up}reference/functions/{pg}.md"
    return f"{up}engine/{pg}.md"

def build():
    sp = load.spans(); rows = load.name_rows(); sym = load.symbols(); src = load.sources(); chg = load.existing_changes()
    for s in sp:
        key = SPAN_ALIAS.get(s['lo'], s['lo'])
        lab, page, note = SPAN.get(key, SPAN_EXTRA.get(s['lo'], (None, None, '')))
        if page is None:
            page = 'core-library' if s['label'].startswith('empty') else 'gameplay-objects'
        page = PAGE_OVERRIDE.get(s['lo'], page)
        s['page'] = page
        notes = [x for x in (s['tunote'], note) if x]
        s['note'] = '; '.join(dict.fromkeys(notes))
    def span_of(a):
        for s in sp:
            if s['lo'] <= a < s['hi']: return s
        return None
    allrows = []
    seen = set()
    for a, r in rows.items():
        s = span_of(a)
        if not s: continue
        r = dict(r); r['span'] = s; r['source'] = src.get(a, '')
        if a in chg and chg[a]['proposed'] != r['name']:
            r['note'] = (r['note'] + '; ' if r['note'] else '') + f"existing-name change: {chg[a]['proposed']}"
        elif r['cur'] and r['cur'] != r['name'] and not r['cur'].startswith('func_'):
            r['note'] = (r['note'] + '; ' if r['note'] else '') + f"replaces existing {r['cur']}"
        allrows.append(r); seen.add(a)
    for a, nm in sym.items():
        if a in seen: continue
        s = span_of(a)
        if not s: continue
        if nm.startswith(('func_', 'junk', 'D_')): continue
        note = ''
        if a in chg: note = f"existing-name change: {chg[a]['proposed']}"
        allrows.append(dict(a=a, name=nm, purpose='', c=None, tier='EXISTING', tu=s['label'], cls='', tags=[], cur=nm,
                            note=note, span=s, source=src.get(a, '')))
    allrows.sort(key=lambda x: x['a'])
    return sp, allrows

def esc(t):
    t = (t or '').replace('|', '\\|').replace('\n', ' ')
    return re.sub(r'\]\(', '\\](', t)   # keep text like vt[2](pv) from rendering as a link

def link(row, depth):
    if row['source']:
        return f"[0x{row['a']:06X}]({'../' * depth}{row['source']})"
    return f"0x{row['a']:06X}"

def conf(row):
    return '' if row['c'] is None else f"{row['c']:.2f}"

def tiercell(row):
    t = row['tier']
    return {'EXISTING': 'existing'}.get(t, t)

def evcell(row):
    ev = ', '.join(row['tags'])
    if row['note']:
        ev = (ev + '; ' if ev else '') + esc(row['note'][:90])
    return ev

def trivial(row):
    return bool(load.TRIVIAL.match(row['name'] or '')) or not row['name'] or row['name'].startswith('func_')

def write_tu_map(sp, rows):
    cnt = collections.Counter(r['span']['lo'] for r in rows if not trivial(r))
    L = ['# TU map: address range -> source file', '',
         'Generated by `tools/gen.py` from `master_names_v2/tu_map.csv` (212 spans between `_GLOBAL_$I` static-init stubs, with the',
         '`_GLOBAL_$D` off-by-one already corrected) and the v2 master name list. Read this before trusting any early label: about a third',
         'of the early (Sly 1 majority-vote) labels were wrong.', '',
         'Conventions:',
         '- **Label**: the Sly 2 source-file name as the naming passes inferred it from alphabetical link order plus content. **Conf** is',
         '  the label confidence from tu_map.csv (high / medium / low). `tu_XXXXXX` = file name unknown; `empty_XXXXXX` = static-init only.',
         '  Labels are HYPOTHESIS unless high; link order is a strong constraint because Sucker Punch linked game files alphabetically.',
         '- **Early label** = the first-pass TU-table vote (often wrong; kept for cross-reference).',
         '- **Named** = number of non-trivial functions with a name (T1/T2/T3 proposal or existing symbol).',
         '- **Page** = the engine page (or reference-only function list) that covers the TU.', '',
         '| Span | Fns | Label | Conf | Early label | Named | Page | Notes |', '|---|---|---|---|---|---|---|---|']
    for s in sp:
        pg = s['page']
        L.append(f"| 0x{s['lo']:06X}-0x{s['hi']:06X} | {s['n']} | {esc(s['label'])} | {s['lconf']} | {esc(s['early'])} | {cnt.get(s['lo'],0)} | [{pg}]({page_link(pg, 1)}) | {esc(s['note'])} |")
    L += ['']
    open(f'{DOCS}/reference/tu-map.md', 'w').write('\n'.join(L))

HEADER = ['All code matches the original byte for byte; **names and purposes are proposals** unless the evidence column says otherwise.', '',
          'Columns: **Tier** from `master_names_v2` (CONVENTIONS 6-7): `T1` commit-ready (two independent signals, a verifier, a runtime or',
          'parse check, or a structural fact), `T2` draft (role usually right, ~95% of purpose lines; the exact name ~60% at low confidence),',
          '`T3` disputed, `existing` = already in `config/symbol_addrs.txt`. **Conf** = confidence 0-1 (calibrated: >= 0.8 was 97-100% correct',
          'in four verifier rounds). **Evidence**: `runtime` = confirmed in PCSX2 save-state RAM (rounds 1-6), `runtime-corrected` = runtime',
          'changed the purpose, `parse` = level parser followed this loader over 100% of the bytes of all 43 world files, `verified` = an',
          'independent verifier re-derived it, `arbitration` = decided by the arbitration pass, `table` = cross-checked against the class',
          'descriptor / option tables, `whole-game` = class identity from the 43-level object data, `top50` / `pending-fix` = from the',
          'top-50 round or master_names_v2 PENDING_FIXES (not yet in master_names.csv). Static-init stubs and junk fragments are omitted.', '']

def write_function_lists(sp, rows):
    by_page = collections.defaultdict(list)
    for r in rows:
        by_page[r['span']['page']].append(r)
    allpages = dict(PAGES); allpages.update(REFPAGES)
    for pg, title in allpages.items():
        rs = by_page.get(pg, [])
        intro = (f'Generated by `tools/gen.py`. Every named function in the TUs that belong to [{pg}](../../engine/{pg}.md), grouped by TU.'
                 if pg in PAGES else f'Generated by `tools/gen.py`. Reference-only list (no engine page): every named function in this range, grouped by TU.')
        tc = collections.Counter(r['tier'] for r in rs if not trivial(r))
        L = [f'# Function list: {title}', '', intro] + HEADER + [
             'Totals on this page: ' + ', '.join(f'{k} {v}' for k, v in sorted(tc.items())) + '.', '']
        cur = None
        for r in rs:
            if trivial(r): continue
            if r['span'] is not cur:
                cur = r['span']
                L += ['', f"## {esc(cur['label'])} (0x{cur['lo']:06X}-0x{cur['hi']:06X})", '', '| Addr | Name | Purpose | Conf | Tier | Evidence |', '|---|---|---|---|---|---|']
            L.append(f"| {link(r,3)} | `{esc(r['name'][:70])}` | {esc(r['purpose'][:220])} | {conf(r)} | {tiercell(r)} | {evcell(r)} |")
        open(f'{DOCS}/reference/functions/{pg}.md', 'w').write('\n'.join(L) + '\n')
    return by_page

STRONG = ('runtime', 'runtime-corrected', 'verified', 'parse', 'arbitration')

def keytable(by_page, pg, extra=(), maxrows=60, minc=0.75):
    rs = [r for r in by_page.get(pg, []) if not trivial(r)]
    extra = set(extra)
    pick = [r for r in rs if r['a'] in extra or any(t in STRONG for t in r['tags'])
            or (r['tier'] == 'T1' and r['c'] is not None and r['c'] >= minc)]
    if len(pick) > maxrows:
        pick = sorted(pick, key=lambda r: (r['a'] not in extra, not any(t in STRONG for t in r['tags']), -(r['c'] or 0)))[:maxrows]
        pick.sort(key=lambda r: r['a'])
    L = ['| Addr | Name | Purpose | Conf | Tier | Evidence |', '|---|---|---|---|---|---|']
    for r in pick:
        L.append(f"| {link(r,2)} | `{esc(r['name'][:60])}` | {esc(r['purpose'][:150])} | {conf(r)} | {tiercell(r)} | {evcell(r)} |")
    L.append('')
    L.append(f"Selected {len(pick)} of {len(rs)} named functions (T1 with confidence >= {minc}, or runtime/verified/parse/arbitration evidence, "
             f"or cited in the text). Full list: [reference/functions/{pg}.md](../reference/functions/{pg}.md).")
    return '\n'.join(L)

def tus_table(sp, pg):
    L = ['| Span | File (label) | Conf | Notes |', '|---|---|---|---|']
    for s in sp:
        if s['page'] == pg and not s['label'].startswith('empty'):
            L.append(f"| 0x{s['lo']:06X}-0x{s['hi']:06X} | {esc(s['label'])} | {s['lconf']} | {esc(s['note'])} |")
    return '\n'.join(L)

def expand():
    sp, rows = build()
    write_tu_map(sp, rows)
    by_page = write_function_lists(sp, rows)
    tdir = os.path.join(HERE, 'templates')
    for fn in sorted(os.listdir(tdir)):
        if not fn.endswith('.md'): continue
        txt = open(os.path.join(tdir, fn)).read()
        pg = fn[:-3]
        def rep(m):
            kind, arg = m[1], (m[2] or '')
            if kind == 'KEYFUNCS':
                extra = [int(x, 16) for x in re.findall(r'0x[0-9A-Fa-f]+', arg)]
                mr = re.search(r'max=(\d+)', arg); mc = re.search(r'minc=([0-9.]+)', arg)
                return keytable(by_page, pg, extra, int(mr[1]) if mr else 60, float(mc[1]) if mc else 0.75)
            if kind == 'TUS':
                return tus_table(sp, pg)
            return m[0]
        txt = re.sub(r'\{\{(KEYFUNCS|TUS)(?::([^}]*))?\}\}', rep, txt)
        out = os.path.join(DOCS, 'engine', fn)
        open(out, 'w').write(txt)
    stats = collections.Counter(r['tier'] for r in rows if not trivial(r))
    print('rows', len(rows), dict(stats))

if __name__ == '__main__':
    os.makedirs(os.path.join(HERE, 'templates'), exist_ok=True)
    expand()
