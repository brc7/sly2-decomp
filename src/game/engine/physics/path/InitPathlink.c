struct S { char pad[0x120]; int a; char pad2[0x2c]; int b; };
void InitLo(struct S *p);
/** @sly2-annot addr=0x1C2048
 * Purpose: CID 124 vt7: InitLo
 * Info: proposed name InitPathlink (T1); class PATHLINK slot7; TU path
 * Notes: names_v2 v1 proposed UpdateJtPosWorldPrev__FP2JT (0.040)
 * Confidence: mid (draft) [T1 0.55] - evidence: static naming pass; sources: r5b_rest
 * See: docs/reference/functions/physics-collision.md#path-0x1bf110-0x1c2920
 * @end-sly2-annot */
void InitPathlink(struct S *p) { InitLo(p); p->a = -1; p->b = -1; }
