struct VTE { short d; short pad; void (*pfn)(); };
struct O { int pad[3]; struct VTE *vt; };
/** @sly2-annot addr=0x1306A8
 * Purpose: If *ppasega: vtable slot-2 Decref, *ppasega = NULL
 * Info: proposed name ReleaseAsegaRef (T2); TU aseg
 * Notes: names_v2 v1 proposed FWipingAseg__FP4ASEG (0.919)
 * Confidence: low (draft) [T2 0.45] - evidence: static naming pass; sources: r1_116918
 * See: docs/reference/functions/animation.md#aseg-0x12f248-0x1308a8
 * @end-sly2-annot */
void func_001306A8(struct O **pp) {
    struct O *p = *pp;
    if (p) { struct VTE *vt = p->vt; ((void (*)(void *))vt[2].pfn)((char *)p + vt[2].d); *pp = 0; }
}
