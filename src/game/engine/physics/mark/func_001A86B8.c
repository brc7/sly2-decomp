typedef struct EDGE { int x0; unsigned short ipos[2]; } EDGE;
typedef struct LSG { char pad[0x54]; EDGE *pedge; int pad2; struct LSG *next; } LSG;
typedef struct VTX { char pad[0x18]; int grf; int pad2[1]; } __attribute__((aligned(32))) VTX;
typedef struct NODE { struct NODE *parent; LSG *plsg; int ipos; } NODE;
/** @sly2-annot addr=0x1A86B8
 * Purpose: Breadth search (64 nodes) for a chain of LSGs connecting two positions along shared edges
 * Info: proposed name CplsgFindEdgePath (T1); TU mark
 * Confidence: mid (draft) [T1 0.55] - evidence: static naming pass; sources:
 *   r4b_rest+verdicts_A2+names_v2-signature(verified)
 * See: docs/engine/physics-collision.md#7-key-functions,
 *   docs/reference/functions/physics-collision.md#mark-0x1a7c18-0x1aa978
 * @end-sly2-annot */
int func_001A86B8(VTX *avtx, int iposStart, int iposEnd, LSG *plsgFirst, LSG **aplsg)
{
    NODE anode[64];
    int inode;
    int cnode;
    int c;
    inode = 0;
    anode[0].parent = 0;
    anode[0].ipos = iposStart;
    anode[0].plsg = 0;
    cnode = 1;
    for (; inode < cnode; inode++) {
        NODE *pnode = &anode[inode];
        LSG *plsg;
        for (plsg = plsgFirst; plsg; plsg = plsg->next) {
            EDGE *pedge = plsg->pedge;
            int ipos;
            if (pedge->ipos[0] == pnode->ipos)
                ipos = pedge->ipos[1];
            else if (pedge->ipos[1] == pnode->ipos)
                ipos = pedge->ipos[0];
            else
                continue;
            if (ipos == iposEnd) {
                c = 0;
                aplsg[c++] = plsg;
                while (pnode->plsg) {
                    aplsg[c++] = pnode->plsg;
                    pnode = pnode->parent;
                }
                goto done;
            }
            {
            VTX *pvtx = avtx + ipos;
            if (pvtx->grf & 4)
                continue;
            pvtx->grf |= 4;
            }
            {
                NODE *pn = &anode[cnode++];
                pn->ipos = ipos;
                pn->parent = pnode;
                pn->plsg = plsg;
            }
        }
    }
    c = 0;
done:
    for (inode = 0; inode < cnode; inode++)
        avtx[anode[inode].ipos].grf &= ~4;
    return c;
}
