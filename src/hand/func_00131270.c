struct S { int pad[5]; int a14; int a18; };
/** @sly2-annot addr=0x131270
 * Purpose: Returns p->+0x18 if set else p->+0x14
 * Info: proposed name PloAsegaEventTarget (T2); TU asega
 * Confidence: low (draft) [T2 0.30] - evidence: static analysis; sources: r1_116918
 * See: docs/reference/functions/animation.md#asega-0x1308a8-0x131f70
 * @end-sly2-annot */
int func_00131270(struct S *p) { if (p->a18) return p->a18; return p->a14; }
