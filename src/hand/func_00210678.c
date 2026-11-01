void *func_0015E938(void *pv, int k, int id);
void *func_001EC918(int id);
int func_00132B10(void *p, int cid);
void func_001F80D0(void *p, float f);
void func_001EE3B0(void *p, float f);
/** @sly2-annot addr=0x210678
 * Purpose: ASEG event 20: SO -> 0x1F80D0(f); EYES (0x8E) found by 0x1EC918 -> 0x1EE3B0(f)
 * Info: proposed name HandleSwAsegEvent20 (T2); TU sw
 * Notes: names_v2 v1 proposed PresetButtonAccel__FP6BUTTONf (0.010)
 * Confidence: low (draft) [T2 0.25] - evidence: static naming pass; sources: r7_1F8DD8
 * See: docs/reference/functions/world-sw.md#sw-0x20d9d0-0x210eb0
 * @end-sly2-annot */
void func_00210678(void *pv, int id1, int id2, float f)
{
    void *p1 = 0;
    void *p2 = 0;
    if (id1 != -1) {
        void *q = func_0015E938(pv, 4, id1);
        if (q && func_00132B10(q, 0x30))
            p1 = q;
    }
    if (id2 != -1) {
        p2 = func_001EC918(id2);
        if (p2 && !func_00132B10(p2, 0x8E))
            p2 = 0;
    }
    if (p1)
        func_001F80D0(p1, f);
    if (p2)
        func_001EE3B0(p2, f);
}
