typedef int qword __attribute__((mode(TI)));
typedef struct EDGE { unsigned short ipos[2]; int pad[3]; } EDGE;
typedef struct LSG { char pad[0x54]; EDGE *pedge; int pad2[2]; } LSG;
typedef struct GEOM { char pad[0x94]; qword *apos; char pad2[0xA0 - 0x98]; int cedge; EDGE *aedge; } GEOM;
int func_00118668(void *bsp, int max, qword a, qword b, int clsgMax, LSG *alsg);
int ClsgClipSoAllEdges(void *pso, void *psoOther, GEOM *pgeom, void *pbsp, int clsgMax, LSG *alsg)
    __asm__("ClsgClipSoAllEdges__FP2SOT0P3BSPiP3LSG");
/** @sly2-annot addr=0x1A80D8
 * Purpose: not written up yet; only the symbol name (pre-existing / Sly 1-derived) describes it.
 * Confidence: mid (draft) - evidence: pre-existing symbol name only; behaviour not reviewed
 * See: docs/reference/functions/physics-collision.md#mark-0x1a7c18-0x1aa978
 * @end-sly2-annot */
int ClsgClipSoAllEdges(void *pso, void *psoOther, GEOM *pgeom, void *pbsp, int clsgMax, LSG *alsg)
{
    int ilsg = 0;
    int clsg = 0;
    int iedge = pgeom->cedge;
    EDGE *aedge = pgeom->aedge;
    qword *apos = pgeom->apos;
    for (iedge--; iedge >= 0; iedge--) {
        EDGE *pedge = &aedge[iedge];
        clsg += func_00118668(pbsp, 0x7FFFFFFF, apos[pedge->ipos[0]], apos[pedge->ipos[1]], clsgMax - clsg, &alsg[clsg]);
        for (; ilsg < clsg; ilsg++)
            alsg[ilsg].pedge = pedge;
    }
    return clsg;
}