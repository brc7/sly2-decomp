typedef int qword __attribute__((mode(TI)));
typedef struct EDGE { unsigned short ipos[2]; unsigned short pad; unsigned short ivtx; struct EDGE *next; } EDGE;
typedef struct LSG { char pad[0x54]; EDGE *pedge; int pad2[2]; } LSG;
typedef struct VTX { char pad[0x18]; int grf; int pad2; } __attribute__((aligned(32))) VTX;
typedef struct SURF { char pad[0x10]; EDGE *pedgeFirst; int pad2; int grf; } SURF;
typedef struct ENT { int x0; int x4; SURF *psurf; } ENT;
typedef struct GEOM { char pad[0x94]; qword *apos; char pad2[0x9C - 0x98]; VTX *avtx; } GEOM;
typedef struct SO { char pad[0x74]; void *pbsp; } SO;
int func_00118668(void *bsp, int max, qword a, qword b, int clsgMax, LSG *alsg);
// Clip edges of selected surfaces and attach their originating edge.
/** @sly2-annot addr=0x1A81F8
 * Purpose: Clip the edges of selected (pruned) surfaces of one shape against the other's BSP into LSGs,
 *   attaching the source edge
 * Info: proposed name ClsgClipSoBspEdges (T1); TU mark
 * Confidence: mid (draft) [T1 0.60] - evidence: static analysis; sources: r4b_rest
 * See: docs/reference/functions/physics-collision.md#mark-0x1a7c18-0x1aa978
 * @end-sly2-annot */
int func_001A81F8(void *unused, SO *pso, GEOM *pgeom, int cent, ENT *aent, int clsgMax, LSG *alsg)
{
    int ilsg = 0;
    int clsg = 0;
    qword *apos = pgeom->apos;
    VTX *avtx = pgeom->avtx;
    int i;
    for (i = cent - 1; i >= 0; i--)
        aent[i].psurf->grf |= 1;
    for (i = cent - 1; i >= 0; i--) {
        register SURF *psurf __asm__("$5") = aent[i].psurf;
        EDGE *pedge;
        if (!(psurf->grf & 1))
            continue;
        if (psurf->grf & 2)
            continue;
        psurf->grf |= 2;
        for (pedge = psurf->pedgeFirst; pedge; pedge = pedge->next) {
            {
                VTX *pvtx = avtx + pedge->ivtx;
                if ((pvtx->grf ^ 1) & 1)
                    continue;
            }
            clsg += func_00118668(pso->pbsp, 0x7FFFFFFF, apos[pedge->ipos[0]], apos[pedge->ipos[1]], clsgMax - clsg, &alsg[clsg]);
            for (; ilsg < clsg; ilsg++)
                alsg[ilsg].pedge = pedge;
        }
    }
    for (i = cent; --i >= 0;)
        {register SURF *psurf __asm__("$5")=aent[i].psurf;psurf->grf &= ~3;}
    return clsg;
}
