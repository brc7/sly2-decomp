struct G { char pad[0x463c]; int c; };
extern struct G D_003DD490;
struct S { char pad[0x180]; int id; };
void func_00183F50(struct S *);
/** @sly2-annot addr=0x13CEF8
 * Purpose: cid 0x25 vt15: base 0x183F50, unique id from a g_sw counter
 * Info: proposed name PostBootyContainerLoad (T2); class CID37 slot15; TU booty
 * Confidence: low (draft) [T2 0.40] - evidence: static analysis; sources: r2b_rest
 * See: docs/reference/functions/game-progression.md#booty-0x13bdf0-0x13d558
 * @end-sly2-annot */
void func_0013CEF8(struct S *p) { func_00183F50(p); p->id = D_003DD490.c++; }
