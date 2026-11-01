void *func_0015E938(void *pv, int k, int id, void *ctx);
int func_00132B10(void *p, int cid);
void func_001F6978(void *p, void *q);
void func_001F69F8(void *p, void *q);
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
        p = func_0015E938(pv, 4, id, ctx);
    if (p && func_00132B10(p, 0x30)) {
        if (f)
            func_001F6978(p, q);
        else
            func_001F69F8(p, q);
    }
}
