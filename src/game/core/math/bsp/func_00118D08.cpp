typedef int qword __attribute__((mode(TI)));
class VUF {
public:
    VUF();
    qword q;
};
struct SURF { qword plane; int pad[2]; int grf; };
struct NODE { NODE *pnodeLeft; NODE *pnodeRight; SURF *psurf; };
static inline NODE *PnodeCheck(NODE *p, NODE *pnodeMax)
{
    if (!(p < pnodeMax))
        p = 0;
    return p;
}
static inline float GMax(float a, float b) { return a < b ? b : a; }
static inline float GMin(float a, float b) { return b < a ? b : a; }
/** @sly2-annot addr=0x118D08
 * Purpose: Walks a BSP with a polygon (cpos points): computes min/max plane distance over the points,
 *   recurses into straddled children and collects touched surfaces into (pcsurf, pppsurf)
 * Info: proposed name FClassifyPolyBsp (T2); TU bsp
 * Notes: names_v2 C proposed PbspPointInBspQuick__FP6VECTORP3BSP (0.438)
 * Confidence: low (draft) [T2 0.35] - evidence: static naming pass; sources: r1_116918
 * See: docs/reference/functions/core-library.md#bsp-0x118668-0x1195e0
 * @end-sly2-annot */
extern "C" int func_00118D08(NODE *pnode, NODE *pnodeMax, int cpos, qword *apos, int *pcsurf, SURF ***pppsurf)
{
    if (cpos == 0)
        return 1;
    while (pnode) {
        float gMax, gMin;
        {
            qword plane = pnode->psurf->plane;
            qword pos = apos[0];
            VUF d;
            VUF one;
            qword dq, oq;
            __asm__("vmul.xyz %0, %2, %3\n\tvaddw.x %1, $vf0, $vf0w\n\tvadday.x $ACC, %0, %0y\n\tvmaddaz.x $ACC, %1, %0z\n\tvmsubw.x %0, %1, %0w"
                    : "=j"(dq), "=j"(oq) : "j"(plane), "j"(pos));
            register int n __asm__("$3");
            __asm__("qmfc2.ni %0, %2\n\tmtc1 %0, %1" : "=&r"(n), "=f"(gMax) : "j"(dq));
            one.q = oq;
            d.q = dq;
            gMin = gMax;
        }
        for (int i = 1; i < cpos; i++) {
            qword plane = pnode->psurf->plane;
            qword pos = apos[i];
            VUF d;
            VUF one;
            qword dq, oq;
            __asm__("vmul.xyz %0, %2, %3\n\tvaddw.x %1, $vf0, $vf0w\n\tvadday.x $ACC, %0, %0y\n\tvmaddaz.x $ACC, %1, %0z\n\tvmsubw.x %0, %1, %0w"
                    : "=j"(dq), "=j"(oq) : "j"(plane), "j"(pos));
            int n;
            float g;
            __asm__("qmfc2.ni %0, %2\n\tmtc1 %0, %1" : "=&r"(n), "=f"(g) : "j"(dq));
            gMin = GMin(gMin, g);
            gMax = GMax(gMax, g);
            one.q = oq;
            d.q = dq;
        }
        if (gMin > 1.0e-4f) {
            pnode = PnodeCheck(pnode->pnodeRight, pnodeMax);
            if (!pnode)
                return 1;
        } else if (gMax < -1.0e-4f) {
            pnode = PnodeCheck(pnode->pnodeLeft, pnodeMax);
            if (!pnode)
                return 0;
        } else {
            int nl, nr;
            NODE *pl = PnodeCheck(pnode->pnodeLeft, pnodeMax);
            if (pl)
                nl = func_00118D08(pl, pnodeMax, cpos, apos, pcsurf, pppsurf);
            else
                nl = 0;
            NODE *pr = PnodeCheck(pnode->pnodeRight, pnodeMax);
            if (pr)
                nr = func_00118D08(pr, pnodeMax, cpos, apos, pcsurf, pppsurf);
            else
                nr = 1;
            if (nl == nr && nl != 2)
                return nl;
            if (*pcsurf <= 0)
                return 1;
            SURF *psurf = pnode->psurf;
            if (!(psurf->grf & 0x4000)) {
                SURF **pp = (*pppsurf)++;
                *pp = psurf;
                psurf->grf |= 0x4000;
            }
            return 2;
        }
    }
    return 1;
}
