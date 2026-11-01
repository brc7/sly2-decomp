struct S { char pad[0x88]; float a[32]; };
extern float D_002DDED0;
/** @sly2-annot addr=0x1C4660
 * Purpose: Mask of buttons in grf pressed within the last dt seconds (t - atPressed[i] <= dt)
 * Info: proposed name POCTL_GrfPressedWithin (T1); class POCTL; TU po
 * Notes: names_v2 B proposed RocsNextRoc__FP3ROC (0.923)
 * Confidence: high [T1 0.85] - evidence: static naming pass; sources: r5_1AE130+verdicts_tu_r3r5
 * See: docs/engine/player.md#11-key-functions, docs/reference/functions/player.md#po-0x1c3d70-0x1c4e50
 * @end-sly2-annot */
int GrfPressedWithin__5POCTLUif(struct S *p, unsigned int grf, float dt)
{
    int grfRet = 0;
    int i = 0;
    float *pg;
    float t = D_002DDED0;
    if (grf) {
        pg = p->a;
        do {
            if ((grf & 1) && t - *pg <= dt) grfRet |= 1 << i;
            grf >>= 1; pg++; i++;
        } while (grf);
    }
    return grfRet;
}
