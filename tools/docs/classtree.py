"""Generate reference/class-tree.md from types/cid_table.csv + v2 class names (master_names_v2 getters) + identifications + runtime counts.
Run: python3 -I classtree.py"""
import os as _os
_REPO = _os.path.abspath(_os.path.join(_os.path.dirname(__file__), '..', '..'))
_RESEARCH = _os.environ.get('SLY2_RESEARCH', _os.path.join(_REPO, '..', 'research'))

import csv, os, sys
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
import load
R = _RESEARCH
OUT = os.path.join(_REPO, 'docs', 'reference', 'class-tree.md')
# cid -> (name, tag, source)
IDS = {
 0x04: ('LIGHT (relight inputs: 3 qw from +0x120)', 'C:static', 'MICROCODE 3.1'),
 0x05: ('vis zone (all *_vz WEs)', 'C:parse', 'whole_game'),
 0x07: ('PROXY child; clue-bottle proxy (REVIEW, medium)', 'H', 'r5 prize.md; REVIEW'),
 0x08: ('mesh surface', 'H', 'r4b meshsurf.md'), 0x09: ('mesh surface (jump target list)', 'H', 'r4b meshsurf.md'),
 0x0B: ('PIPE (lamp posts; JTPIPE)', 'C:runtime', 'runtime2'), 0x0C: ('wall grabspot (wall press)', 'C:runtime', 'runtime2'),
 0x0D: ('RAIL (wires)', 'C:runtime', 'runtime2'), 0x0E: ('convex polygon SHAPE / pathzone obstacle', 'H', 'r5 SUMMARY'),
 0x10: ('DETECTOR (guard vision)', 'C:runtime', 'runtime 1-2'), 0x11: ('PATHZONE (*_pz)', 'C:parse', 'whole_game'),
 0x13: ('HND (hook; REVIEW proposes HND)', 'H', 'r4b hnd.md; REVIEW'), 0x14: ('hand-attach XFM (adjust_hand_jt / _bentley, 2 per level)', 'C:parse', 'whole_game'),
 0x15: ('camera-related XFM (task visibility lists)', 'H', 'r8 target.md'),
 0x17: ('VOL child', 'H', 'r8 vol.md'), 0x18: ('VOLBTN (REVIEW, high)', 'H', 'r2b button.md; REVIEW'), 0x19: ('VOLBTN variant', 'H', 'r2b button.md'),
 0x1A: ('EXIT (level exit volume)', 'C:verified', 'verify r7r8; REVIEW'), 0x1B: ('table grabspot (VOL + GRABSPOT; hide under)', 'C:runtime', 'runtime2'),
 0x22: ('pole (GRABSPOT)', 'H', 'r4b grabspot.md'), 0x23: ('GRABSPOT XFM; mission trigger spot (ApplyTaskStates)', 'C:runtime', 'runtime4/6'),
 0x24: ('carry grabspot (child of CID35)', 'C:runtime', 'runtime4'), 0x25: ('booty container', 'H', 'r2b booty.md'),
 0x27: ('GRABSPOT XFM, list D_003E13D8', 'H', 'r7'), 0x28: ('JMT (jump target; none in the Paris hub)', 'H', 'r4b air.md; runtime2'),
 0x29: ('in-level teleport destination (WARP_* strings)', 'H', 'REVIEW (TRIGGER 0x228170)'),
 0x2A: ('WARP (destinations; entrance_* aliases)', 'C:verified', 'verify r7r8; REVIEW'), 0x2B: ('ambient-sound XFM (OnAdd/OnRemove start/stop)', 'H', 'r1; arbitration'),
 0x2C: ('camera marker (CPCAMERA target)', 'C:runtime', 'runtime5b'), 0x2D: ('render-to-texture owner (item preview?)', 'C:static', 'RENDERING P8/P9'),
 0x2E: ('recon photo target', 'H', 'r7c'), 0x2F: ('stimulus listener', 'H', 'r3b'),
 0x39: ('GOMER (instances are swarmers)', 'C:parse', 'whole_game; REVIEW'), 0x3A: ('GUARD (plain guard)', 'C:parse', 'whole_game; runtime'),
 0x3B: ('DIMITRI', 'C:parse', 'whole_game'), 0x3C: ('NEELA', 'C:parse', 'whole_game'), 0x3D: ('RAJAN', 'C:parse', 'whole_game'),
 0x3E: ('JEANBISON', 'C:parse', 'whole_game'), 0x3F: ('CARMELITA', 'C:parse', 'whole_game'), 0x40: ('CONTESSA', 'C:parse', 'whole_game'),
 0x41: ('ARPEGGIO', 'C:parse', 'whole_game'), 0x42: ('BEAR', 'C:parse', 'whole_game'), 0x43: ('BEARCUB (single instance)', 'H', 'whole_game'),
 0x44: ('DANCER (rhythm-game performer)', 'H', 'whole_game; r2'), 0x45: ('TURRET (grapple turret)', 'C:parse', 'whole_game'),
 0x46: ('SAFEHOUSE (character-select PO)', 'C:runtime', 'runtime3/5b'),
 0x47: ('hacking minigame base (damageable PO)', 'H', 'REVIEW; r4b'), 0x48: ('HACKSHIP', 'C:parse', 'whole_game'), 0x49: ('HACKENEMY', 'C:parse', 'whole_game'),
 0x4A: ('HACKBULLET', 'C:parse', 'whole_game'), 0x4B: ('HACKLOCK', 'C:parse', 'whole_game'), 0x4C: ('HACKWALL', 'C:parse', 'whole_game'),
 0x4D: ('ROBOTTANK (single instance)', 'H', 'whole_game'), 0x4E: ('VAULT (clue-bottle safe)', 'C:parse', 'whole_game'),
 0x51: ('ACCMISS = Bentley crossbow dart', 'C:runtime', 'runtime5a'),
 0x53: ('BUTTON (instances: bouncers, buoys)', 'C:static', 'REVIEW'), 0x54: ('SENSOR', 'H', 'r6b'), 0x55: ('LASEN laser beams (zap 0x1000)', 'C:runtime', 'runtime7a; REVIEW'), 0x4F: ('BOMB (also guard rock ammo)', 'C:runtime', 'runtime7a'),
 0x56: ('BARRIER', 'C:static', 'cid_table'), 0x58: ('BARREL / carryable (REVIEW, medium)', 'H', 'r4b; REVIEW'),
 0x59: ('WATER (zap kind 0x2000)', 'C:runtime', 'runtime3'), 0x5A: ('DPRIZE (abstract)', 'H', 'r5'), 0x5B: ('COIN pool', 'C:runtime', 'runtime'),
 0x5C: ('KEY', 'C:runtime', 'runtime2; whole_game'), 0x5D: ('MEDICINE (health refill)', 'C:runtime', 'runtime2; whole_game'), 0x5E: ('LOOT (pickpocket item)', 'C:runtime', 'runtime'),
 0x5F: ('CLUE (clue bottle)', 'C:runtime', 'runtime'), 0x60: ('mini pigeon/rat critter', 'H', 'whole_game'), 0x61: ('SKY (sky dome; render kind 1)', 'C:parse', 'whole_game; RENDERING'),
 0x62: ('breakable props (chairs, barrels, crates, containers)', 'C:runtime', 'runtime2/4/5a; whole_game'), 0x63: ('pickpocket dispenser / guard pouch', 'C:runtime', 'runtime'),
 0x64: ('tossables (alarm clock tossable_distract, music box)', 'C:runtime', 'runtime7a; whole_game'), 0x65: ('Bentley trigger bomb / mines (gadget object)', 'C:runtime', 'runtime5a'),
 0x66: ('guard trap (drops a prize)', 'H', 'whole_game; r8'), 0x67: ('BOOTY (replaces cid_table ALARM)', 'C:parse', 'whole_game; arbitration'),
 0x68: ('buffer effects / frame-capture transition', 'H', 'whole_game; r2b'), 0x69: ('motion trail', 'H', 'r8'), 0x6A: ('mesh deformed along a path', 'H', 'r6b'),
 0x6B: ('WAYPT (job marker mission_start_*)', 'C:runtime', 'runtime6; whole_game'), 0x6C: ('robot-tank ammo pickup', 'H', 'r5b'),
 0x70: ('aim manager / launcher (darts, guard pebbles, sword missiles)', 'C:runtime', 'runtime5a/7a; whole_game'), 0x71: ('guard script', 'C:runtime', 'runtime'), 0x72: ('RELAY (cutscene / sequence script)', 'C:runtime', 'runtime6'),
 0x74: ('screen tint / fill (render kinds 14 and 1)', 'C:static', 'RENDERING'), 0x75: ('stimulus template (radius, height, ramp)', 'C:runtime', 'runtime7a'), 0x77: ('off-screen guard spawner', 'H', 'r3b'),
 0x78: ('guard attack definition', 'C:runtime', 'runtime'), 0x79: ('rhythm-game manager', 'H', 'r2'), 0x7A: ('? (LoadFromBrx with FILEREC list)', 'C:static', 'level_parser'),
 0x7B: ('256-point route', 'H', 'r2b'), 0x7C: ('PATHLINK (BHVPATHLINK walks it; REVIEW)', 'C:runtime', 'runtime2; REVIEW'), 0x7D: ('guard manager (oid 0x656)', 'C:runtime', 'runtime'),
 0x7E: ('WIDGET', 'C:static', 'cid_table'), 0x7F: ('WIDGET: modal hands-off box', 'H', 'r7c'), 0x80: ('WIDGET: job completion banner (pauses clock slot 7)', 'C:runtime', 'runtime6'),
 0x81: ('WIDGET: objective counter (computed per frame)', 'C:runtime', 'runtime6'), 0x83: ('WIDGET: episode menu', 'C:runtime', 'runtime5b'),
 0x84: ('JOB (save m<N>_<name>)', 'C:runtime', 'runtime6'), 0x85: ('TASK (save t<N>_<name>)', 'C:runtime', 'runtime6'), 0x87: ('CM', 'C:runtime', 'runtime'),
 0x89: ('POCTL', 'C:runtime', 'runtime3'), 0x91: ('LEAP', 'H', 'r7c'), 0x92: ('DIRANIM', 'H', 'r7c'), 0x93: ('stimulus (noise) instance', 'C:runtime', 'runtime2/7a'),
 0x95: ('Splice XF handle', 'H', 'r6'), 0x96: ('Splice SOPART handle', 'H', 'r6'), 0x97: ('SND (sound instance)', 'H', 'r7'), 0x98: ('sound manager (find-sound-object~)', 'H', 'splice'),
 0x99: ('JSG (relay interpreter)', 'C:runtime', 'runtime6'), 0x9A: ('ticker / hint box (find-ticker-object~)', 'H', 'splice; top50'),
 0x9B: ('BHV (behaviour base)', 'C:static', 'cid_table'), 0x9C: ('guard root: awareness mode +0x30', 'C:runtime', 'runtime2/4'),
 0x9D: ('animation/locomotion leaf (flag 9)', 'C:runtime', 'runtime2'), 0x9E: ('idle leaf (flag 2)', 'C:runtime', 'runtime2'), 0x9F: ('attack-animation leaf (flag 3)', 'C:runtime', 'runtime2'),
 0xA0: ('hit reaction', 'H', 'r9'), 0xA3: ('KO / dead (flag 7)', 'C:runtime', 'runtime4'), 0xA4: ('attack execution (flag 0x18)', 'C:runtime', 'runtime2'),
 0xA5: ('alert/combat mode (flag 0x1D)', 'C:runtime', 'runtime2'), 0xA6: ('path base', 'H', 'r9'), 0xA7: ('path/goal follower', 'C:runtime', 'runtime2'),
 0xA8: ('player walk-to-goal (writes POCTL stick)', 'C:runtime', 'runtime6'), 0xA9: ('guard-script runner (flag 0x21)', 'C:runtime', 'runtime2'),
 0xAA: ('calm/patrol mode (flag 0x1E)', 'C:runtime', 'runtime2'), 0xAB: ('suspicious/search mode (flag 0x1F)', 'C:runtime', 'runtime2'), 0xAC: ('leap (flag 0xF)', 'C:runtime', 'runtime2'),
 0xAD: ('SHOOT: ranged attack (BHVSHOOT)', 'C:runtime', 'runtime7a S67'), 0xAE: ('NEELA watch/alarm', 'H', 'r5b'), 0xB1: ('juggled / carried (flag 0x17)', 'C:runtime', 'runtime4'), 0xB4: ('DIMITRI flee', 'H', 'r3b'), 0xB5: ('DIMITRI flee', 'H', 'r3b'),
 0xB6: ('keep pace on route', 'H', 'r2b'), 0xB8: ('rail traversal', 'H', 'r9'), 0xB9: ('pipe traversal', 'H', 'r9'), 0xBA: ('go-to / look-at tracker (flag 0x1C)', 'C:runtime', 'runtime2'),
 0xBE: ('pathlink traversal (flag 0x2C)', 'C:runtime', 'runtime2'), 0xC0: ('patrol-route walker (flag 0x2E)', 'C:runtime', 'runtime2'),
 0xC1: ('player scripted go-to', 'H', 'r2b'), 0xC2: ('player follow leader (StartStepplayerFollow)', 'C:runtime', 'runtime6'), 0xC6: ('Murray behaviour', 'H', 'r2b'),
}
rows = list(csv.DictReader(open(f'{R}/types/cid_table.csv')))
cnt = {}
for r in csv.DictReader(open(f'{R}/runtime/class_counts.csv')):
    if r['cid']: cnt[int(r['cid'], 16)] = r['S04']
byc = {int(r['cid'], 16): r for r in rows}
V2 = load.class_names()
def depth(c):
    d = 0
    while byc[c]['parent_cid']:
        c = int(byc[c]['parent_cid'], 0); d += 1
    return d
L = ['# Class tree (all 199 CIDs)', '',
 'Generated by `tools/classtree.py` from the class-descriptor table D_002BAAE8 (`types/cid_table.csv`), the v2 class names (the 199 `Get<Cls>ClassDesc`',
 'getters in `master_names_v2`, which follow CONVENTIONS 7.4 and 7.11), the identifications gathered since (runtime rounds 1-6, the whole-game object data,',
 'the v2.1 class review and its PENDING_FIXES) and the runtime object census (S04, Paris hub). The tree structure, sizes and vtable addresses are exact',
 '`[C:runtime]` (every live instance resolves through vtable slot 6; minimum address gaps equal `cb`). **Slots** = number of 8-byte vtable slots (slot 0 null).',
 'CIDs are pre-order numbers of a different tree than Sly 1: no Sly 1 CID value can be reused past 2.', '',
 '**Name (v2)** is the class word used in function names (`CIDnn`/`BHVnnn` = placeholder, decimal). **cid_table** is the older label (kept for',
 'cross-reference; `types/cid_table.csv` itself has not been updated). **Identification** says what the class is, with the tag of the strongest evidence:',
 '`C:runtime` = seen in RAM, `C:parse` = whole-game object data (sole class of named world entries, object strings), `C:verified`/`C:static` = code.',
 'Class words such as GUARD or HACKSHIP are team coinages backed by that data; Sucker Punch\'s real spellings are unknown.', '',
 'Key facts: there is no ALO class (Sly 1 ALO split into LO + XFM); SW and CM derive from BASIC; STEPPLAYER, BHV, POCTL and the GRABSPOT',
 'mix-ins are new; MURRAY moved under STEPPLAYER; every named character has its own STEPGUARD subclass `[C:static]`.', '',
 '| CID | Name (v2) | cid_table | Identification | Tag / source | Parent | Size | vtable | Slots | Live (S04) |', '|---|---|---|---|---|---|---|---|---|---|']
for r in rows:
    c = int(r['cid'], 16)
    nm = r['name_sly2']
    ind = '&nbsp;&nbsp;' * depth(c)
    idn, tag, src = IDS.get(c, ('', '', ''))
    conf = r['confidence']
    tagcell = f'[{tag}] {src}' if tag else (f'cid_table {conf}' if nm not in ('?', '') else '')
    vt = r['vtable'] if not r['vtable'].startswith('multiple') else 'multiple'
    v2 = V2.get(c, nm)
    pv2 = V2.get(int(byc[c]['parent_cid'], 0), r['parent_name']) if byc[c]['parent_cid'] else ''
    L.append(f"| 0x{c:02X} ({c}) | {ind}{v2} | {nm if nm != v2 else ''} | {idn} | {tagcell} | {pv2} | {r['size']} | {vt} | {r['vtable_len']} | {cnt.get(c, '')} |")
L += ['', 'Notes:', '- SND (0x97) has no vtable of its own in cid_table (3 candidate tables: base SND 0x2D9D00, BANKSND 0x2D9C70, STREAMSND 0x2D9BE0 per r7 sound.md).',
 '- MISSILE family vtables: cid_table says 78 slots, r5 counts 79 (slot 78 = Fire). CONFLICTS #4, open.',
 '- BHV slot meanings differ from LO\'s (7 Init, 8 Start, 9 Close, 10 Update, 11 UpdateMotion, 12 GrfGroundProbe, 13 FHandleMessage, 14 UpdateInput, 15 SetState): see [guards-ai](../engine/guards-ai.md#3-the-behaviour-stack-and-the-guard-modes).',
 '- Pending class names from the v2.1 review (not yet in master_names): VOLBTN 0x18, LASEN 0x55, PATHLINK 0x7C, HND 0x13 (and medium: clue-bottle proxy 0x07, hacking base 0x47, BARREL 0x58). cid 0x29 = in-level teleport destination; 0x2A WARP = warp destinations.',
 '- Placeholder names (`CIDnn`, `BHVnnn`, `WIDGETnnn`) use the decimal CID, as cid_table does. Earlier write-ups mixed hex and decimal (CONFLICTS #11).', '']
open(OUT, 'w').write('\n'.join(L))
print('ok', len(rows))
