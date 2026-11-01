void *PloFindSwObject__FP2SWi3OIDP2LO(void *pv, int k, int id, void *ctx);
int FIsBasicDerivedFrom__FP5BASIC3CID(void *p, int cid);
void ShowSoGlobsById(void *p, void *q);
void HideSoGlobsById(void *p, void *q);
/** @sly2-annot addr=0x210408
 * Purpose: ASEG event 13/14: on SO target call 0x1F6978 or 0x1F69F8 (enable/disable pair)
 * Info: proposed name HandleSwAsegEvent13 (T2); TU sw
 * Notes: names_v2 v1 proposed IposFindButtonClosest__FP6BUTTONPf (0.068)
 * Confidence: low (draft) [T2 0.25] - evidence: static naming pass; sources: r7_1F8DD8
 * See: docs/reference/functions/world-sw.md#sw-0x20d9d0-0x210eb0
 * @end-sly2-annot */
void func_00210408(void *pv, int f, int id, void *q, void *pdef, void *ctx)
{
    void *p;
    if (id == -1)
        p = pdef;
    else
        p = PloFindSwObject__FP2SWi3OIDP2LO(pv, 4, id, ctx);
    if (p && FIsBasicDerivedFrom__FP5BASIC3CID(p, 0x30)) {
        if (f)
            ShowSoGlobsById(p, q);
        else
            HideSoGlobsById(p, q);
    }
}
