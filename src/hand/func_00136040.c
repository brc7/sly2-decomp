struct VTE { short d; short pad; void (*pfn)(); };
struct ASEG;
struct S { int pad[3]; struct VTE *vt; char pad1[0x190 - 0x10]; float v190[3]; char pad2[0x1030 - 0x19c]; void *pasega1030; int f1034;
    char pad3[0x14e4 - 0x1038]; struct ASEG *f14e4; struct ASEG *f14e8; char pad4[0x15d4 - 0x14ec]; struct ASEG *f15d4;
    char pad5[0x15f0 - 0x15d8]; struct ASEG *f15f0; char pad6[0x16b0 - 0x15f4]; int f16b0; };
void ReleaseSma(void *);
void ApplyAsegCur__FP4ASEGP3ALOffiPP5ASEGA(struct ASEG *, struct S *, float, float, int, void **);
/** @sly2-annot addr=0x136040
 * Purpose: Set +0x16B0 sub-state and play the matching aseg
 * Info: proposed name SetBentleySubState16B0 (T2); TU bentley
 * Confidence: low (draft) [T2 0.35] - evidence: static analysis; sources: r2b_rest
 * See: docs/reference/functions/player.md#bentley-0x134648-0x1378d8
 * @end-sly2-annot */
void func_00136040(struct S *p, int n) {
    struct ASEG *paseg;
    int grfapl;
    if (n == p->f16b0) return;
    p->f16b0 = n;
    paseg = 0;
    grfapl = 1;
    switch (n) {
    case 0:
        paseg = p->f14e4;
        {
            float *pv = p->v190;
            pv[2] = 0.0f;
            pv[1] = 0.0f;
            pv[0] = 0.0f;
        }
        break;
    case 1:
        paseg = p->f15f0;
        grfapl = 9;
        break;
    case 2:
        paseg = p->f14e8;
        if (!paseg) paseg = p->f15d4;
        grfapl = 9;
        break;
    }
    ReleaseSma(&p->f1034);
    ApplyAsegCur__FP4ASEGP3ALOffiPP5ASEGA(paseg, p, 0.0f, 1.0f, grfapl, &p->pasega1030);
    ((void (*)(void *))p->vt[90].pfn)((char *)p + p->vt[90].d);
}
