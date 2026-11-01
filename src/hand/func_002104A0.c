void *func_0015E938(void *pv, int k, int id, void *ctx);
int func_00132B10(void *p, int cid);
void func_001F6AC8(void *p, void *q);
void func_001F6B48(void *p, void *q);
/** @sly2-annot addr=0x2104A0
 * Purpose: ASEG event 15/16: on SO call 0x1F6AC8 or 0x1F6B48
 * Info: proposed name HandleSwAsegEvent15 (T2); TU sw
 * Confidence: low (draft) [T2 0.25] - evidence: static analysis; sources: r7_1F8DD8
 * See: docs/reference/functions/world-sw.md#sw-0x20d9d0-0x210eb0
 * @end-sly2-annot */
void func_002104A0(void *pv, int f, int id, void *q, void *pdef, void *ctx)
{
    void *p;
    if (id == -1)
        p = pdef;
    else
        p = func_0015E938(pv, 4, id, ctx);
    if (p && func_00132B10(p, 0x30)) {
        if (f)
            func_001F6AC8(p, q);
        else
            func_001F6B48(p, q);
    }
}
