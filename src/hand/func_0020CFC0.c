struct S { int a; void *o; char pad[8]; long fl; };
int FIsBasicDerivedFrom__FP5BASIC3CID(void *o, int n);
int func_0020FA50(void *pv, struct S *p);
/** @sly2-annot addr=0x20CFC0
 * Purpose: probe filter: ignore cid 0x59 (water?) when flagged
 * Info: proposed name FFilterStuckProbe (T2); TU tu_20CE28
 * Notes: names_v2 C proposed PumpHost__18CBinaryInputStream (0.638)
 * Confidence: low (draft) [T2 0.30] - evidence: static naming pass; sources: r7c_hud
 * See: docs/reference/functions/guards-ai.md#tu_20ce28-0x20ce28-0x20d5d0
 * @end-sly2-annot */
int func_0020CFC0(void *pv, struct S *p) { if ((p->fl & 0x1000000000000L) && FIsBasicDerivedFrom__FP5BASIC3CID(p->o, 0x59)) return 1; return func_0020FA50(pv, p); }
