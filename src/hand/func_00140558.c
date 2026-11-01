struct VTE { short d; short pad; void (*pfn)(void *, int, void *, int, void *); };
struct SO { char pad[0xFE0]; struct SO *psoNext; };
struct P { char pad[0xC]; struct VTE *vt; char pad2[0x10E0 - 0x10]; int x10E0; };
extern struct SO *D_003E1444;
extern float D_002A4D18;
int func_00208E70(struct SO *pso, struct P *p, float *pg);
/** @sly2-annot addr=0x140558
 * Purpose: Unless +0x10E0 set, scans guard list D_003E1444 with GuardEvalInteractMode 0x208E70 and picks
 *   nearest guard and best-mode guard, then calls vt95 SetPlayerInteractMode(n0,pso0,n1,pso1)
 * Info: proposed name UpdateStepplayerInteractCandidates (T1); TU stepplayer
 * Confidence: mid (draft) [T1 0.60] - evidence: static analysis; sources: r2b_rest
 * See: docs/engine/player.md#4-stepplayer-tu-0x13ff70-0x144638-file-name-unknown-lilly-stepplayer_rawc,
 *   docs/reference/functions/player.md#stepplayer-0x13ff70-0x144638
 * @end-sly2-annot */
void func_00140558(struct P *p)
{
    struct SO *pso, *pso0, *pso1;
    int n0, n1;
    float g0, g1;
    if (p->x10E0) return;
    g1 = D_002A4D18;
    pso1 = 0;
    pso0 = 0;
    n1 = -1;
    n0 = -1;
    g0 = g1;
    for (pso = D_003E1444; pso; pso = pso->psoNext) {
        float ag[2];
        int n = func_00208E70(pso, p, &ag[1]);
        float g = ag[1];
        if (g < g0) {
            g0 = g;
            n0 = n;
            pso0 = pso;
        }
        if (n1 < n || (n == n1 && g < g1)) {
            g1 = g;
            n1 = n;
            pso1 = pso;
        }
    }
    p->vt[95].pfn((char *)p + p->vt[95].d, n0, pso0, n1, pso1);
}
