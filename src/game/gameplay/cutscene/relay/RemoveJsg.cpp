struct VTE { short d; short pad; void (*pfn)(void *); };
struct Q { char pad[0x5C]; int dl; };
struct P { int pad[3]; VTE *vt; int pad10; Q *pq; char pad2[0x9C - 0x18]; int x9C; char pad3[0xEC - 0xA0]; void *pvEC; };
class LOCK {
public:
    P *m_p;
    LOCK(P *p) : m_p(p) { p->vt[1].pfn((char *)p + p->vt[1].d); }
    ~LOCK() { m_p->vt[2].pfn((char *)m_p + m_p->vt[2].d); }
};
struct CLOCK { float t; };
extern "C" {
extern CLOCK D_002DDED0;
int FJsgRemoved(P *p);
void RemoveDlEntry__FP2DLPv(int *pdl, P *p);
void ClearJsgAseg(P *p);
void ClearJsgRelayWait(P *p);
void ClearJsgSma(P *p);
void func_001E5468(P *p);
void func_001E54B0(P *p);
void ClearJsgStepHolds(P *p);
void func_001E5570(P *p);
void func_0014D1D0(CLOCK *pclock, int n, float g);
void func_001E4EB8(P *p, int a, int b);
void SetJsgTunnel(P *p, void *po);
void func_001E5000(P *p, int a);
void func_001E50E0(P *p, int a, int b);
void StopSnd(void *pv, float g);
void func_00258040(void **ppv, int n);
void func_001E5A98(P *p, int n);
void RemoveJsg(P *p);
}
/** @sly2-annot addr=0x1E5628
 * Purpose: Tear down a script: unlink from relay dlJsg and release every resource (aseg, relay wait, SMA,
 *   widget, JT mode, step holds, move BHVs, clock rate reset, JT, tunnel, focus, hands-off, sound, camera
 *   blend), then drop the relay's reference
 * Info: proposed name RemoveJsg (T1); TU relay
 * Confidence: mid (draft) [T1 0.60] - evidence: static analysis; sources: r6b_rest
 * See: docs/engine/cutscenes.md#5-key-functions,
 *   docs/reference/functions/cutscenes.md#relay-0x1dfad8-0x1e5d80
 * @end-sly2-annot */
void RemoveJsg(P *p)
{
    if (FJsgRemoved(p)) return;
    LOCK lock(p);
    void **ppv = &p->pvEC;
    RemoveDlEntry__FP2DLPv(&p->pq->dl, p);
    ClearJsgAseg(p);
    ClearJsgRelayWait(p);
    ClearJsgSma(p);
    func_001E5468(p);
    func_001E54B0(p);
    ClearJsgStepHolds(p);
    func_001E5570(p);
    func_0014D1D0(&D_002DDED0, 6, 1.0f);
    func_001E4EB8(p, 0, 0);
    SetJsgTunnel(p, 0);
    func_001E5000(p, 0);
    func_001E50E0(p, 0, 0);
    if (*ppv) {
        StopSnd(*ppv, 0.0f);
        func_00258040(ppv, 0);
    }
    if (p->x9C) func_001E5A98(p, 1);
    p->vt[2].pfn((char *)p + p->vt[2].d);
}
