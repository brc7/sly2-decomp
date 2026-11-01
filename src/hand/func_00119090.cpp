// (Claude agent, lane A9, 2026-10-23) BSP point search. On the on-plane path ONE local p holds the checked right child, the
// recursive result and then the checked left child, so it stays in v0 (no if-conversion straight into a0).
typedef int qword __attribute__((mode(TI)));
class VUF {
public:
    VUF();
    qword q;
};
struct NODE { NODE *pnodeLeft; NODE *pnodeRight; qword *pplane; };
static inline NODE *PnodeCheck(NODE *p, NODE *pnodeMax)
{
    if (!(p < pnodeMax))
        p = 0;
    return p;
}
/** @sly2-annot addr=0x119090
 * Purpose: Point location with on-plane case: if |dist|<=1e-4 try both children recursively
 * Info: proposed name PbspPointInBsp (T1); TU bsp
 * Confidence: mid (draft) [T1 0.60] - evidence: static analysis; sources: r1_116918
 * See: docs/reference/functions/core-library.md#bsp-0x118668-0x1195e0
 * @end-sly2-annot */
extern "C" NODE *func_00119090(NODE *pnode, NODE *pnodeMax, qword pos)
{
    while (pnode) {
        qword plane = *pnode->pplane;
        VUF d;
        VUF one;
        qword dq, oq;
        float g;
        __asm__("vmul.xyz %0, %2, %3\n\tvaddw.x %1, $vf0, $vf0w\n\tvadday.x $ACC, %0, %0y\n\tvmaddaz.x $ACC, %1, %0z\n\tvmsubw.x %0, %1, %0w"
                : "=j"(dq), "=j"(oq) : "j"(plane), "j"(pos));
        int n;
        __asm__("qmfc2.ni %0, %2\n\tmtc1 %0, %1" : "=&r"(n), "=f"(g) : "j"(dq));
        one.q = oq;
        d.q = dq;
        if (g < -1.0e-4f) {
            NODE *p = PnodeCheck(pnode->pnodeLeft, pnodeMax);
            if (!p)
                return pnode;
            pnode = p;
        } else if (g > 1.0e-4f) {
            NODE *p = PnodeCheck(pnode->pnodeRight, pnodeMax);
            if (!p)
                return 0;
            pnode = p;
        } else {
            NODE *p = PnodeCheck(pnode->pnodeRight, pnodeMax);
            if (p) {
                p = func_00119090(p, pnodeMax, pos);
                if (p)
                    return p;
            }
            p = PnodeCheck(pnode->pnodeLeft, pnodeMax);
            if (!p)
                return pnode;
            pnode = p;
        }
    }
    return 0;
}
