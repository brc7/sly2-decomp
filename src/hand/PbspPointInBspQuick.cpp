// PbspPointInBspQuick (Claude agent, lane A8, 2026-10-23): BSP-style plane walk. From v6: one asm doing qmfc2.ni + mtc1 (scratch "=&r"), and the left child clipped into a block-scoped `q = 0; if (p < pnodeMax) q = p;` (gives the target v1->v0 copy). oq pinned to vf2.
typedef int qword __attribute__((mode(TI)));
union VUF { qword q; float f[4]; };
struct NODE { NODE *pnodeLeft; NODE *pnodeRight; qword *pplane; };
extern "C" void *func_002564B8(void *p);
/** @sly2-annot addr=0x118FB0
 * Purpose: Point location in a BSP without on-plane handling (dist<1e-4 -> left/negative child), returns the
 *   leaf node
 * Info: proposed name PbspPointInBspQuick (T1); TU bsp
 * Notes: names_v2 C proposed CloneBspc__FP4GEOMP4BSPCT0T1 (0.638)
 * Confidence: mid (draft) [T1 0.65] - evidence: static naming pass; sources: r1_116918
 * See: docs/engine/camera.md#5-key-functions, docs/reference/functions/camera.md#tn-0x220d40-0x223f70,
 *   docs/reference/functions/core-library.md#bsp-0x118668-0x1195e0
 * @end-sly2-annot */
extern "C" NODE *PbspPointInBspQuick(NODE *pnode, NODE *pnodeMax, qword pos)
{
    NODE *p;
    VUF d;
    VUF one;
    if (!pnode)
        return 0;
    VUF *pone = &one;
loop:
    {
        qword plane;
        __asm__("lqc2 %0, %1" : "=j"(plane) : "m"(*pnode->pplane));
        qword dq;
        register qword oq __asm__("vf2"); /* pin: the target allocates the one-vector to vf2 (the dot result then gets vf1) */
        float g;
        func_002564B8(&d);
        func_002564B8(pone);
        __asm__("vmul.xyz %0, %0, %3\n\tvaddw.x %1, $vf0, $vf0w\n\tvadday.x $ACC, %0, %0y\n\tvmaddaz.x $ACC, %1, %0z\n\tvmsubw.x %0, %1, %0w"
                : "=j"(dq), "=j"(oq) : "0"(plane), "j"(pos));
        { int t; __asm__("qmfc2.ni %0, %2\n\tmtc1 %0, %1" : "=&r"(t), "=f"(g) : "j"(dq)); }
        one.q = oq;
        d.q = dq;
        if (g < 1.0e-4f) {
            p = pnode->pnodeLeft;
            {NODE *q = 0; if (p < pnodeMax) q = p;
            if (!q)
                return pnode;
            pnode = q;}
            goto loop;
        } else {
            p = pnode->pnodeRight;
            pnode = 0;
            if (p < pnodeMax)
                pnode = p;
        }
    }
    if (pnode) goto loop;
    return 0;
}
