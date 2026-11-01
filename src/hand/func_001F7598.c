typedef int qword __attribute__((mode(TI)));
struct O;
struct VT {
    short d; short pad;
    void *pfn;
};
struct O { char pad[0xC]; struct VT *pvt; struct O *pNext; };
struct K {
    struct O *p0; int n4; void *p8; int nC;
    char pad[0x44 - 0x10];
    int n44; struct O *p48; float f4C; char dle[0x10];
} __attribute__((aligned(16)));
struct N { int n0; int n4; int pad8; int nC; char pad[0x48 - 0x10]; struct O *p48; char pad2[0x50 - 0x4C]; struct N *pNext; };
struct M { struct O *po; int n; struct K *pk; };
struct M2 { struct O **ppo; int *pn; };
struct M3 { struct K *pk; };
extern struct N *D_003E15C0;
extern char D_003E15A8[];
extern float D_002DDED0;
struct O *func_001A5BF0(void *);
int func_001F78D8(struct O *, int);
struct K *PvAllocSlotheapClear(void *);
void ClearDle__FP3DLE(void *);
void AppendDlEntry__FP2DLPv(void *, void *);

#define VCALL(po, slot, T) ((T)(po)->pvt[slot].pfn)((void *)((char *)(po) + (po)->pvt[slot].d)

struct O *func_001F7598(struct K *pk)
{
    int unused[4];
    struct M m;
    struct M2 m2;
    struct O *po;
    for (po = func_001A5BF0(pk->p8); po; po = po->pNext) {
        struct N *pn;
        for (pn = D_003E15C0; pn; pn = pn->pNext) {
            if (pn->n0 == (int)pk->p0 && pn->n4 == pk->n4 && pn->nC == pk->nC && pn->p48 == po) return 0;
        }
        pk->n44 = func_001F78D8(po, pk->nC);
        if (VCALL(po, 75, int (*)(void *, struct K *)), pk)) {
            struct K *pkNew;
            pk->p48 = po;
            pkNew = PvAllocSlotheapClear(D_003E15A8);
            *pkNew = *pk;
            pkNew->f4C = D_002DDED0;
            ClearDle__FP3DLE(pkNew->dle);
            AppendDlEntry__FP2DLPv(&D_003E15C0, pkNew);
            if (pk->n44 & 0xF8) {
                {
                    m.po = po; m.n = (int)pk->p0; m.pk = pk;
                    VCALL(po, 4, void (*)(void *, int, struct M *)), 0xB, &m);
                    VCALL(pk->p0, 4, void (*)(void *, int, struct M *)), 0xB, &m);
                }
                {
                    m2.ppo = &po; m2.pn = &pk->nC;
                    VCALL(pk->p0, 3, void (*)(void *, int, int, int, void *, int)), 0xA, 1, 2, &m2, 0);
                }
                {
                    m2.ppo = (struct O **)pk;
                    VCALL(po, 3, void (*)(void *, int, int, int, void *, int)), 0xB, 1, 2, &m2, 0);
                }
            }
            return po;
        }
    }
    return 0;
}
