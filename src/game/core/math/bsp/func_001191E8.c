typedef struct MSG { int cb; int nKind; } MSG;
typedef struct ITEM { char ab[0x20]; } ITEM;
typedef struct OWNER { char pad[0xC]; ITEM *aitem; } OWNER;
typedef struct NODE { struct NODE *pnodeLeft; struct NODE *pnodeRight; void *pv; } NODE;
typedef struct TREE { int cnode; int n4; int n8; NODE *anode; } TREE;

void *func_001AE1D0(MSG *pmsg);
extern char D_002BBAF0[];

#define REBASE(p, baseSrc, baseDst) ((void *)((char *)(baseDst) + ((char *)(p) - (char *)(baseSrc))))

/** @sly2-annot addr=0x1191E8
 * Purpose: Copies a BSPC from one GEOM to another, rebasing internal node/plane pointers
 * Info: proposed name CloneBspc (T1); TU bsp
 * Confidence: mid (draft) [T1 0.75] - evidence: static analysis; sources: r1_116918
 * See: docs/reference/functions/core-library.md#bsp-0x118668-0x1195e0
 * @end-sly2-annot */
void func_001191E8(TREE *ptreeSrc, OWNER *pownerSrc, TREE *ptreeDst, OWNER *pownerDst)
{
    int i;
    MSG msg;

    ptreeDst->cnode = ptreeSrc->cnode;
    ptreeDst->n4 = ptreeSrc->n4;
    msg.cb = ptreeSrc->cnode * sizeof(NODE);
    msg.nKind = 0x10;
    ptreeDst->anode = (NODE *)func_001AE1D0(&msg);

    for (i = 0; i < ptreeDst->cnode; i++) {
        NODE *pnodeSrc = &ptreeSrc->anode[i];
        NODE *pnodeDst = &ptreeDst->anode[i];
        if (pnodeSrc->pv == D_002BBAF0) {
            pnodeDst->pv = pnodeSrc->pv;
        } else {
            pnodeDst->pv = REBASE(pnodeSrc->pv, pownerSrc->aitem, pownerDst->aitem);
        }
        pnodeDst->pnodeLeft = pnodeSrc->pnodeLeft ? (NODE *)REBASE(pnodeSrc->pnodeLeft, ptreeSrc->anode, ptreeDst->anode) : 0;
        pnodeDst->pnodeRight = pnodeSrc->pnodeRight ? (NODE *)REBASE(pnodeSrc->pnodeRight, ptreeSrc->anode, ptreeDst->anode) : 0;
    }
}
