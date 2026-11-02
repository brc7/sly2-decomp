struct VTE { short d; short pad; void (*pfn)(void *); };
struct E { int n0; int n4; };
struct P { int pad[3]; VTE *vt; char pad2[0x28 - 0x10]; E *peFirst; E *peMax; E *peCur; };
class LOCK {
public:
    P *m_p;
    LOCK(P *p) : m_p(p) { p->vt[1].pfn((char *)p + p->vt[1].d); }
    ~LOCK() { m_p->vt[2].pfn((char *)m_p + m_p->vt[2].d); }
};
extern "C" {
void NextJsgCommand__FP3JSG(P *p);
int FJsgRemoved(P *p);
void ReleaseJsgJtIfComplete(P *p);
int FIsJsgCommandComplete__FP3JSGP8RELAYCMD(P *p, E *pe);
void RemoveJsg(P *p);
void func_001E1C40(P *p);
}
/** @sly2-annot addr=0x1E1C40
 * Purpose: Run a script: start at prelaycmdFirst if not started, then keep stepping (NextJsgCommand) while
 *   the current command is fAsync or complete (FIsJsgCommandComplete); release JT if done; remove the JSG
 *   past the block end
 * Info: proposed name UpdateJsg (T1); TU relay
 * Confidence: high [T1 0.80] - evidence: static analysis; sources: r6b_rest
 * See: docs/engine/cutscenes.md#5-key-functions,
 *   docs/reference/functions/cutscenes.md#relay-0x1dfad8-0x1e5d80
 * @end-sly2-annot */
void func_001E1C40(P *p)
{
    LOCK lock(p);
    if (!p->peCur) {
        p->peCur = p->peFirst;
        NextJsgCommand__FP3JSG(p);
        if (FJsgRemoved(p)) return;
    }
    while (p->peCur <= p->peMax && (p->peCur->n4 || FIsJsgCommandComplete__FP3JSGP8RELAYCMD(p, p->peCur))) {
        ReleaseJsgJtIfComplete(p);
        NextJsgCommand__FP3JSG(p);
        if (FJsgRemoved(p)) return;
    }
    ReleaseJsgJtIfComplete(p);
    if (p->peCur > p->peMax) RemoveJsg(p);
}
