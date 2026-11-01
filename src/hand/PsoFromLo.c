struct S { char pad[0x10]; struct S *x10; };
int FIsBasicDerivedFrom__FP5BASIC3CID(struct S *p, int n);
/** @sly2-annot addr=0x1A5BF0
 * Purpose: Return plo if it is an SO (FIsBasicDerivedFrom CID 0x30) else plo->psoParent
 * Info: proposed name PsoFromLo (T1); TU lo
 * Notes: names_v2 v1 proposed SetLightSaturate_Old__FP5LIGHTP6VECTOR (0.000)
 * Confidence: mid (draft) [T1 0.55] - evidence: static naming pass; sources: r4_17F090
 * See: docs/reference/functions/object-model.md#lo-0x1a5270-0x1a7518
 * @end-sly2-annot */
struct S *PsoFromLo(struct S *p) { if (FIsBasicDerivedFrom__FP5BASIC3CID(p, 0x30)) return p; return p->x10; }
