enum OID { OID_Nil };
struct ALO { char pad[0x74]; float t74; };
struct BL { int x0; ALO *palo; };
struct MRSG { float t; float dt; };
extern "C" float func_0012F990(ALO *palo, OID oid);
/** @sly2-annot addr=0x137FB8
 * Purpose: not written up yet; only the symbol name (pre-existing / Sly 1-derived) describes it.
 * Confidence: mid (draft) - evidence: pre-existing symbol name only; behaviour not reviewed
 * See: docs/reference/functions/animation.md#blend-0x137a30-0x138f08
 * @end-sly2-annot */
void FillBlAmrsg(int cbBl, int cbl, void *pbl, int ibAmrsg, int coid, OID *aoid)
{
    int ibl;
    for (ibl = 0; ibl < cbl; ibl++) {
        BL *pblT = (BL *)((char *)pbl + ibl * cbBl);
        ALO *palo = pblT->palo;
        if (palo) {
            MRSG *amrsg = (MRSG *)((char *)pblT + ibAmrsg);
            MRSG *pmrsgPrev;
            MRSG *pmrsg;
            int i;
            pmrsg = amrsg;
            for (i = 0; i < coid; i++, pmrsg++)
                pmrsg->t = func_0012F990(palo, aoid[i]);
            pmrsg = amrsg;
            pmrsgPrev = &pmrsg[coid - 1];
            for (i = 0; i < coid; i++, pmrsg++) {
                float dt = pmrsg->t - pmrsgPrev->t;
                pmrsgPrev->dt = dt;
                if (dt < 0.0f)
                    pmrsgPrev->dt = dt + palo->t74;
                pmrsgPrev = pmrsg;
            }
        }
    }
}
