typedef int qword __attribute__((mode(TI)));
struct CTX { int a[4]; };
struct AREQ { int cb; int cbAlign; };
struct LSG { qword v0; qword v10; qword v20; qword v30; float at[2]; int x48; int x4C; int x50; int pad[3]; };
struct CTX *STACKSCOPE_ctor(struct CTX *);
void STACKSCOPE_ctor_1AE4A0(struct CTX *, int);
void *func_001AE318(struct AREQ *);
void func_001AE5A0(void *, void *, int);

/** @sly2-annot addr=0x14C9F8
 * Purpose: Merge two sorted LSG interval lists into an output list with a set operation selected by k (0 =
 *   union ...)
 * Info: proposed name ClsgMergeAlsg (T1); TU clip
 * Confidence: high [T1 0.85] - evidence: static analysis; sources: r2b_rest+verdicts_A1+verify_side
 * See: docs/engine/camera.md#5-key-functions, docs/engine/physics-collision.md#4-geometry-and-queries,
 *   docs/engine/physics-collision.md#7-key-functions
 * @end-sly2-annot */
int ClsgMergeAlsg(int c1, struct LSG *alsg1, int c2, struct LSG *alsg2_, int cMax, struct LSG *alsgOut, int k)
{
    /* hard-register local: fixes the s2/s3 assignment of alsg2 vs c2 */
    register struct LSG *alsg2 __asm__("$18") = alsg2_;
    struct CTX ctx;
    struct AREQ req;
    struct LSG *apl[2];
    struct LSG *alsgT;
    struct LSG *plsg;
    struct LSG *plsgEnd1;
    struct LSG *plsgEnd2;
    int i1, i2, fPrev, n, c;
    STACKSCOPE_ctor(&ctx);
    {
        struct AREQ *preq = &req;
        preq->cb = cMax * sizeof(struct LSG);
        preq->cbAlign = 0x10;
        alsgT = func_001AE318(preq);
    }
    i1 = 0;
    i2 = 0;
    apl[0] = alsg1;
    fPrev = 0;
    apl[1] = alsg2;
    plsgEnd1 = alsg1 + c1;
    plsgEnd2 = alsg2 + c2;
    plsg = alsgT;
    switch (k) {
    case 0:
        n = 1;
        break;
    case 1:
        n = 2;
        break;
    }
    while (apl[0] < plsgEnd1 || apl[1] < plsgEnd2) {
        float g1, g2;
        int iOld;
        struct LSG **ppl;
        struct LSG *pl;
        int fIn;
        g1 = apl[0] < plsgEnd1 ? apl[0]->at[i1] : 3.40282346638528859812e+38f;
        g2 = apl[1] < plsgEnd2 ? apl[1]->at[i2] : 3.40282346638528859812e+38f;
        if (g1 <= g2) {
            iOld = i1;
            i1 = 1 - iOld;
            ppl = &apl[0];
        } else {
            iOld = i2;
            i2 = 1 - iOld;
            ppl = &apl[1];
        }
        fIn = n <= i1 + i2;
        pl = *ppl;
        if (fIn != fPrev) {
            if (fIn) {
                plsg->at[0] = pl->at[0];
                plsg->v0 = pl->v0;
                plsg->v20 = pl->v20;
                plsg->x4C = pl->x4C;
            } else {
                plsg->at[1] = pl->at[1];
                plsg->v10 = pl->v10;
                plsg->v30 = pl->v30;
                plsg->x50 = pl->x50;
                plsg++;
                if (cMax < plsg - alsgT) {
                    plsg--;
                    break;
                }
            }
            fPrev = fIn;
        }
        if (iOld == 1)
            *ppl = pl + 1;
    }
    c = plsg - alsgT;
    func_001AE5A0(alsgOut, alsgT, c * sizeof(struct LSG));
    STACKSCOPE_ctor_1AE4A0(&ctx, 2);
    return c;
}
