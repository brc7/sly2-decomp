typedef int qword __attribute__((mode(TI)));
struct VE { short d; short pad; void *pf; };
struct LO { char pad[0xC]; struct VE *vt; };
struct X { char pad[0xC]; struct VE *vt; char pad2[0x188 - 0x10]; void *p188; };
struct S { struct LO *po; int k; float t; struct X *x; int n; };
extern struct { float t; float dt; } D_002DDED0;
extern float D_002C3E58;
qword func_001A62E8(void *p);
void func_0020D3C0(struct S *p, int k);

/** @sly2-annot addr=0x20D1F8
 * Purpose: inside hazard object (vt73) while falling for > D_002C3E58 s -> zap with hazard params
 * Info: proposed name UpdateHazardWatch (T2); TU tu_20CE28
 * Confidence: low (draft) [T2 0.35] - evidence: static analysis; sources: r7c_hud
 * See: docs/reference/functions/guards-ai.md#tu_20ce28-0x20ce28-0x20d5d0
 * @end-sly2-annot */
void func_0020D1F8(struct S *p)
{
    int k;
    if (!p->n)
        return;
    do {
        k = p->k;
        switch (k) {
        case 0: {
            struct X *x = p->x;
            int f;
            struct VE *pe;
            void *pthis;
            if (!x)
                break;
            f = 0;
            pe = &x->vt[73];
            pthis = (char *)x + pe->d;
            if (((int (*)(void *, qword))pe->pf)(pthis, func_001A62E8(p->po))) {
                qword v = ((qword (*)(void *))p->po->vt[33].pf)((char *)p->po + p->po->vt[33].d);
                float z;
                __asm__("pextuw %0, $zero, %1" : "=r"(z) : "r"(v));
                if (z < 0.0f)
                    f = 1;
            }
            if (f)
                func_0020D3C0(p, 1);
            break;
        }
        case 1:
            if (!p->x)
                func_0020D3C0(p, 0);
            else if (D_002DDED0.t - p->t > D_002C3E58 &&
                     !((int (*)(void *, void *))p->po->vt[76].pf)((char *)p->po + p->po->vt[76].d, p->x->p188))
                func_0020D3C0(p, 2);
            break;
        case 2:
            if (!p->x || ((int (*)(void *, void *))p->po->vt[76].pf)((char *)p->po + p->po->vt[76].d, p->x->p188))
                func_0020D3C0(p, 0);
            break;
        }
    } while (p->k != k);
}
