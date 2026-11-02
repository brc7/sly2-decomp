struct X;
class CSave {
public:
    X *m_p0;
    X *m_p4;
    int m_n8;
    int m_nC;
    inline CSave(X *px, int nF4, int nF8);
    inline ~CSave();
};
struct X {
    char pad[0x68]; X *f68;
    char pad1[0x7C - 0x6C]; X *f7c;
    char pad2[0x94 - 0x80]; int c94;
    char pad3[0xA0 - 0x98]; int cA0; X **apxA4;
    char pad4[0xF0 - 0xA8]; int fF0; int fF4; int fF8; int fFC; int f100; int f104;
};
inline CSave::CSave(X *px, int nF4, int nF8)
{
    m_p0 = 0;
    m_p4 = px;
    if (px) {
        m_n8 = px->fF4;
        px->fF4 = nF4;
        m_nC = m_p4->fF8;
        m_p4->fF8 = nF8;
    }
}
inline CSave::~CSave()
{
    if (m_p0) {
        m_p0->fF0 = m_n8;
        m_p0->fF4 = m_nC;
    }
    if (m_p4) {
        m_p4->fF4 = m_n8;
        m_p4->fF8 = m_nC;
    }
}
static inline int Max(int a, int b) { return a < b ? b : a; }
/** @sly2-annot addr=0x164100
 * Purpose: Topological visit: records depth, counts visits, emits the task once all prerequisites are visited
 *   and recurses into successors
 * Info: proposed name VisitTaskTopo (T1); TU game
 * Confidence: mid (draft) [T1 0.50] - evidence: static analysis; sources: r3b_rest
 * See: docs/reference/functions/game-progression.md#game-0x1613a8-0x166270
 * @end-sly2-annot */
extern "C" void VisitTaskTopo(X *p, int *pc, X **apx, int nDepth, int n)
{
    p->f100 = Max(nDepth, p->f100);
    p->f104 = Max(n, p->f104);
    p->fFC++;
    if (p->fFC >= p->c94) {
        apx[(*pc)++] = p;
        nDepth++;
        if (p->f7c && p->f7c->f68 == p)
            n++;
        for (int i = 0; i < p->cA0; i++) {
            X *px = p->apxA4[i];
            CSave save(px, p->fF4, p->fF8);
            VisitTaskTopo(px, pc, apx, nDepth, n);
        }
    }
}
