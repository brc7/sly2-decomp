struct VTE { short d; short pad; void (*pfn)(); };
struct S { int pad[3]; struct VTE *vt; };
/** @sly2-annot addr=0x133340
 * Purpose: vt3 HandleSpliceEvent(5) on the object
 * Info: proposed name FireLoSpliceEvent5 (T2); TU basic
 * Confidence: low (draft) [T2 0.40] - evidence: static analysis; sources: r2b_rest
 * See: docs/reference/functions/object-model.md#basic-0x132b00-0x1336f0
 * @end-sly2-annot */
void func_00133340(struct S *p) { struct VTE *vt = p->vt; ((void (*)(void *, int, int, int, int, int))vt[3].pfn)((char *)p + vt[3].d, 5, 0, 0, 0, 0); }
