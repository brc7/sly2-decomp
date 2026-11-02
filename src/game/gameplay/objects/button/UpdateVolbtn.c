/* UpdateVolbtn (Claude agent, lane D27). The compare loop: the first element is tested before a real for(;;) that is entered by a goto at its increment, so loop.c sees a multiple-entry loop and leaves it alone (no strength reduction), like the target. The redundant fChanged = 1 before `goto changed` is removed later by flow, and jump2 then turns that test into the target bne. */
typedef int qword __attribute__((mode(TI)));
struct CTX { int a[4]; };
struct G { char pad[0x3EB8]; int c; void *apo[1]; };
extern struct G D_003DD490;
struct CLOCK { float t; };
extern struct CLOCK D_002DDED0;
struct VTE { short d; short pad; void *pfn; };
struct P {
    int pad[3]; struct VTE *vt; char pad1[0x1C0 - 0x10]; int x1C0; float x1C4; float x1C8; char pad2[0x1D8 - 0x1CC]; void *x1D8;
    char pad3[0x288 - 0x1DC]; int c288; void *a28C[8]; int c2AC; int a2B0[4]; int c2C0; void **p2C4; int x2C8;
};
void UpdateLo(struct P *);
void SetVolbtnButtons(struct P *, int);
struct CTX *STACKSCOPE_ctor(struct CTX *);
void STACKSCOPE_ctor_1AE4A0(struct CTX *, int);
void CopyAb__FPvT0Ui(void *, void *, int);
int CploFindSwObjectsByClass(struct G *, int, int, int, int, void **);
int func_001A61A0(void *);
qword func_001A62E8(void *);
int FCheckVolPoint(struct P *, qword);
void qsort(void *, int, int, int (*)(const void *, const void *));
int func_00149420(const void *, const void *);

/** @sly2-annot addr=0x149440
 * Purpose: vt16: volume button - objects inside the volume trigger/untrigger
 * Info: proposed name UpdateVolbtn (T1); class VOLBTN slot16; TU button
 * Confidence: mid (draft) [T1 0.70] - evidence: static analysis; sources: r2b_rest
 * See: docs/reference/functions/gameplay-objects.md#button-0x148620-0x149a50
 * @end-sly2-annot */
void UpdateVolbtn(struct P *p)
{
    int k;
    UpdateLo(p);
    k = p->x1C0;
    if (k == 2) {
        if (0.0f <= p->x1C8 && p->x1C8 < D_002DDED0.t - p->x1C4)
            SetVolbtnButtons(p, 1);
    } else if (k < 3) if (k >= 0) {
        void *apo[64];
        void *apoSel[64];
        struct CTX ctx;
        int i = 0;
        int fFound = 0;
        int c;
        int j;
        int cSel;
        STACKSCOPE_ctor(&ctx);
        CopyAb__FPvT0Ui(apo, p->a28C, p->c288 * 4);
        c = p->c288;
        for (; i < p->c2AC; i++)
            c += CploFindSwObjectsByClass(&D_003DD490, 0x505, p->a2B0[i], 0, 0x40 - c, &apo[c]);
        if (p->c288 == 0 && p->c2AC == 0) {
            CopyAb__FPvT0Ui(&apo[c], D_003DD490.apo, D_003DD490.c * 4);
            c += D_003DD490.c;
        }
        p->x1D8 = 0;
        cSel = 0;
        j = 0;
        if (j < c) do {
            void *po = apo[j];
            if (po && func_001A61A0(po) && FCheckVolPoint(p, func_001A62E8(po))) {
                fFound = 1;
                if (!p->x1D8)
                    p->x1D8 = po;
                if (!p->x2C8)
                    break;
                apoSel[cSel++] = po;
            }
        } while (++j < c);
        if (p->x2C8) {
            int fChanged = 0;
            qsort(apo, c, 4, func_00149420);
            if (p->c2C0 != cSel) {
                fChanged = 1;
            } else {
                /* first element checked before the loop; the loop is entered at its increment */
                int i2 = 0;
                if (i2 < cSel) {
                    if (apoSel[i2] == p->p2C4[i2])
                        goto next;
                    fChanged = 1;
                    goto changed;
                    for (;;) {
                    next:
                        i2++;
                        if (i2 >= cSel)
                            break;
                        if (apoSel[i2] != p->p2C4[i2]) {
                        changed:
                            fChanged = 1;
                            break;
                        }
                    }
                }
            }
            if (fChanged) {
                struct VTE *e;
                p->c2C0 = cSel;
                CopyAb__FPvT0Ui(p->p2C4, apoSel, cSel * 4);
                e = &p->vt[3];
                ((void (*)(void *, int, int, int, int, int))e->pfn)((char *)p + e->d, 9, 0, 0, 0, 0);
            }
        }
        SetVolbtnButtons(p, fFound);
        STACKSCOPE_ctor_1AE4A0(&ctx, 2);
    }
}
