struct VTE { short d; short pad; void (*pfn)(); };
struct S { int pad[3]; struct VTE *vt; };
/** @sly2-annot addr=0x133380
 * Purpose: vt3 HandleSpliceEvent(6) on the object
 * Info: proposed name FireLoSpliceEvent6 (T2); TU basic
 * Notes: names_v2 C proposed DeactivateCptn__FP4CPTNPv (0.638)
 * Confidence: low (draft) [T2 0.40] - evidence: static naming pass; sources: r2b_rest
 * See: docs/reference/functions/object-model.md#basic-0x132b00-0x1336f0
 * @end-sly2-annot */
void func_00133380(struct S *p) { struct VTE *vt = p->vt; ((void (*)(void *, int, int, int, int, int))vt[3].pfn)((char *)p + vt[3].d, 6, 0, 0, 0, 0); }
