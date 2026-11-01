struct S { char pad[0x150]; int k; };
void *func_001F4FC8(struct S *p, void *q);
int func_00132B10(void *q, int a);
void *func_00134310(void *v, int a);
/** @sly2-annot addr=0x2242E0
 * Purpose: base 0x1F4FC8; in states 0/4 and other is STEPPLAYER (flag 0x34) add flag via 0x134310
 * Info: class CID100 slot61; TU tu_223F70
 * Notes: descriptive only: Cid100 filter flags vs player | no usable identifier; purpose only
 * Confidence: low (draft) [T2 0.30] - evidence: static analysis; sources: r8_21BB88
 * @end-sly2-annot */
void *func_002242E0(struct S *p, void *q)
{
    void *v = func_001F4FC8(p, q);
    int k = p->k;
    if (k == 0 || k == 4) { if (func_00132B10(q, 0x34)) v = func_00134310(v, 1); }
    return v;
}
