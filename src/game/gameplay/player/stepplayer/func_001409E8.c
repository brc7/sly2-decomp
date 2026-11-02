typedef int qword __attribute__((mode(TI)));
struct VTE { short d; short pad; int (*pfn)(void *, void *); };
struct SO { char pad[0xC]; struct VTE *vt; char pad2[0xFE0 - 0x10]; struct SO *psoNext; };
struct X { char pad[0x104]; struct X *pxNext; };
struct P { char pad[0x10C4]; void *p10C4; };
extern struct SO *D_003E1444;
extern struct X *D_003E1468;
int FStepplayerInJtmode(struct P *p);
qword func_001A62E8(struct P *p);
void *func_00156698(void *a, qword pos, struct P *p);
int FStepguardHasSgf(struct SO *pso, int n);
int func_00156FE8(struct X *px, void *q);
void func_00156780(void *q);
/** @sly2-annot addr=0x1409E8
 * Purpose: Returns 2 if in jtmode<3 or no query from 0x156698; else 1 if any guard (D_003E1444) answers
 *   vt89(query) or any D_003E1468 entry 0x156FE8 matches, 0 otherwise; guard states 0x1D/0xB force 2
 * Info: proposed name NStepplayerHideTest (T2); TU stepplayer
 * Confidence: low (draft) [T2 0.30] - evidence: static analysis; sources: r2b_rest
 * See: docs/reference/functions/player.md#stepplayer-0x13ff70-0x144638
 * @end-sly2-annot */
int func_001409E8(struct P *p)
{
    int unused[4];
    void *q;
    int r;
    struct SO *pso;
    struct X *px;
    if (FStepplayerInJtmode(p)) return 2;
    q = 0;
    if (p->p10C4) q = func_00156698(p->p10C4, func_001A62E8(p), p);
    if (!q) return 2;
    r = 0;
    pso = D_003E1444;
    for (;;) {
        if (!pso) break;
        if (FStepguardHasSgf(pso, 0x1D) || FStepguardHasSgf(pso, 0xB)) { r = 2; break; }
        if (pso->vt[89].pfn((char *)pso + pso->vt[89].d, q)) { r = 1; break; }
        pso = pso->psoNext;
    }
    if (r == 0) {
        for (px = D_003E1468; px; px = px->pxNext)
            if (func_00156FE8(px, q)) { r = 1; break; }
    }
    func_00156780(q);
    return r;
}
