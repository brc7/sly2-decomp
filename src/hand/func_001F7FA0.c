struct P { char pad[0xA4]; int c; void **ap; };
int func_00132B10(void *, int);
void func_001EE678(void *, float, float);
/** @sly2-annot addr=0x1F7FA0
 * Purpose: SetScrollerMasterSpeeds(f,g) on every SCROLLER (CID 0x8F) in SO SAA list (+0xA4)
 * Info: proposed name SetSoScrollerSpeeds (T2); TU so
 * Notes: names_v2 v1 proposed ApplySoProxy__FP2SOP5PROXY (0.069)
 * Confidence: low (draft) [T2 0.45] - evidence: static naming pass; sources: r6_1D5150
 * See: docs/reference/functions/physics-collision.md#so-0x1f2108-0x1f8dd8
 * @end-sly2-annot */
void func_001F7FA0(struct P *p, float f, float g)
{
    int i;
    for (i = 0; i < p->c; i++) {
        void *q = p->ap[i];
        if (func_00132B10(q, 0x8F)) func_001EE678(q, f, g);
    }
}
