typedef int qword __attribute__((mode(TI)));
class VUF {
public:
    VUF();
    qword q;
};
struct NODE { NODE *pnodeLeft; NODE *pnodeRight; qword *pplane; };
extern NODE D_002BBB10;
extern NODE D_002BBB20;
static inline NODE *PnodeCheck(NODE *p, NODE *pnodeMax)
{
    if (!(p < pnodeMax))
        p = 0;
    return p;
}
/** @sly2-annot addr=0x118A88
 * Purpose: Recursive prune of a BSP against a sphere (pos, radius): emits a reduced tree
 * Info: proposed name PruneBsp (T1); TU bsp
 * Confidence: high [T1 0.88] - evidence: static analysis; sources: r1_116918+verdicts_A1
 * See: docs/engine/core-library.md#7-key-functions,
 *   docs/reference/functions/core-library.md#bsp-0x118668-0x1195e0,
 *   docs/reference/functions/physics-collision.md#vol-0x227bb8-0x227fe8
 * @end-sly2-annot */
extern "C" int func_00118A88(NODE *pnode, NODE *pnodeMax, qword pos, float eps, int *pcnode, NODE **ppnode)
{
    float g;
    while (pnode) {
        qword plane = *pnode->pplane;
        VUF d;
        VUF one;
        qword dq, oq;
        __asm__("vmul.xyz %0, %2, %3\n\tvaddw.x %1, $vf0, $vf0w\n\tvadday.x $ACC, %0, %0y\n\tvmaddaz.x $ACC, %1, %0z\n\tvmsubw.x %0, %1, %0w"
                : "=j"(dq), "=j"(oq) : "j"(plane), "j"(pos));
        int n;
        __asm__("qmfc2.ni %0, %2\n\tmtc1 %0, %1" : "=&r"(n), "=f"(g) : "j"(dq));
        one.q = oq;
        d.q = dq;
        if (g > eps) {
            pnode = PnodeCheck(pnode->pnodeRight, pnodeMax);
        } else if (g < -eps) {
            pnode = PnodeCheck(pnode->pnodeLeft, pnodeMax);
        } else {
            int nl, nr;
            NODE *pl = PnodeCheck(pnode->pnodeLeft, pnodeMax);
            if (pl) {
                nl = func_00118A88(pl, pnodeMax, pos, eps, pcnode, ppnode);
                pl = *ppnode - 1;
            } else {
                nl = 0;
            }
            NODE *pr = PnodeCheck(pnode->pnodeRight, pnodeMax);
            if (pr) {
                nr = func_00118A88(pr, pnodeMax, pos, eps, pcnode, ppnode);
                pr = *ppnode - 1;
            } else {
                nr = 1;
            }
            if (nl == nr && nr != 2)
                return nr;
            if (*pcnode <= 0)
                return 1;
            NODE *pn = (*ppnode)++;
            (*pcnode)--;
            pn->pplane = pnode->pplane;
            switch (nl) {
            case 0: pn->pnodeLeft = 0; break;
            case 1: pn->pnodeLeft = &D_002BBB10; break;
            case 2: pn->pnodeLeft = pl; break;
            }
            switch (nr) {
            case 0: pn->pnodeRight = &D_002BBB20; break;
            case 1: pn->pnodeRight = 0; break;
            case 2: pn->pnodeRight = pr; break;
            }
            return 2;
        }
    }
    return g > 0.0f;
}
