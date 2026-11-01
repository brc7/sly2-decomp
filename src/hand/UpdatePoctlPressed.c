struct S { char pad[0x1C]; float f1C; char p1[0x2C - 0x20]; float f2C; int grfCur; char p2[0x78 - 0x34]; int grfPrev; char p3[0x88 - 0x7C]; float a[16]; float aC8[2]; float aD0[2]; };
extern float D_002DDED0;
/** @sly2-annot addr=0x1C45A8
 * Purpose: Record g_clock.t into atPressed[i] for newly pressed buttons, and per-stick direction timestamps
 *   (atLeftStick/atRightStick)
 * Info: proposed name UpdatePoctlPressed (T1); class POCTL; TU po
 * Notes: names_v2 B proposed PostRocLoad__FP3ROC (0.947)
 * Confidence: mid (draft) [T1 0.60] - evidence: static naming pass; sources: r5_1AE130
 * See: docs/reference/functions/player.md#po-0x1c3d70-0x1c4e50
 * @end-sly2-annot */
void UpdatePoctlPressed(struct S *p)
{
    float t = D_002DDED0;
    int grf = p->grfCur & ~p->grfPrev;
    float *pg;
    p->grfPrev = p->grfCur;
    if (grf) {
        pg = p->a;
        do { if (grf & 1) *pg = t; grf >>= 1; pg++; } while (grf);
    }
    p->aC8[p->f1C > 0.0f] = t;
    p->aD0[p->f2C > 0.0f] = t;
}
