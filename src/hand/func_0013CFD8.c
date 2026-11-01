struct K { int a; int b; };
extern struct K D_002A4BB0;
extern char D_003DD490[];
void func_00183FB8(void *);
void EnsureSwCallback(void *, void *, struct K, int);
/** @sly2-annot addr=0x13CFD8
 * Purpose: cid 0x25 vt23: base 0x183FB8 then EnsureSwCallback(D_002A4BB0)
 * Info: proposed name OnCid37Add (T2); class CID37 slot23; TU booty
 * Notes: slot 23 OnAdd resolved by arbitration (OnRemove/OnAdd = object leaves/enters the active world, incl.
 *   proximity activation) (was BootyContainer_VUnk22)
 * Confidence: low (draft) [T2 0.85] - evidence: arbitration; sources: arbitration_slots22_23
 * See: docs/engine/game-progression.md#8-key-functions,
 *   docs/reference/functions/game-progression.md#booty-0x13bdf0-0x13d558
 * @end-sly2-annot */
void func_0013CFD8(void *p) { func_00183FB8(p); EnsureSwCallback(D_003DD490, p, D_002A4BB0, 0); }
