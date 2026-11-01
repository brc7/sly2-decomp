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
struct O *PsoFromLo(void *);
int GrfzpdFromSoGrfzpk(struct O *, int);
struct K *PvAllocSlotheapClear(void *);
void ClearDle__FP3DLE(void *);
void AppendDlEntry__FP2DLPv(void *, void *);

#define VCALL(po, slot, T) ((T)(po)->pvt[slot].pfn)((void *)((char *)(po) + (po)->pvt[slot].d)

/** @sly2-annot addr=0x1F7598
 * Purpose: Deliver a zap record to the first SO in the target chain that accepts it: dedupe against the
 *   recent-zap list D_003E15C0, compute GrfzpdFromSoGrfzpk, ask target vt75; on accept log a copy with time
 *   and, if damaging (grfzpd&0xF8), send msg 0xB to both and splice events 0xA/0xB; return the SO or NULL
 * Info: proposed name PsoInflictZap (T2); TU so
 * Notes: v3: superseded InflictZap | top50_round (0.55): Body. Sly 1 zap.cpp InflictZpdZap/FInflictSoZap
 *   vocabulary (ZPR record, FInflictSoZap = SO vt). Callers DetonateBomb, InflictBhvguardZap, ZapLasenBeams,
 *   InflictSoContactZaps, SoTouchZapCallback. Returns the zapped SO ->
 * Confidence: low (draft) [T2 0.55] - evidence: Sly 1 / Lilly reference; semantic review; sources:
 *   r6_1D5150+top50_round
 * See: docs/engine/overview.md#6-subsystem-map, docs/engine/physics-collision.md#3-zaps-damage-delivery,
 *   docs/engine/physics-collision.md#7-key-functions
 * @end-sly2-annot */
struct O *func_001F7598(struct K *pk)
{
    int unused[4];
    struct M m;
    struct M2 m2;
    struct O *po;
    for (po = PsoFromLo(pk->p8); po; po = po->pNext) {
        struct N *pn;
        for (pn = D_003E15C0; pn; pn = pn->pNext) {
            if (pn->n0 == (int)pk->p0 && pn->n4 == pk->n4 && pn->nC == pk->nC && pn->p48 == po) return 0;
        }
        pk->n44 = GrfzpdFromSoGrfzpk(po, pk->nC);
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
