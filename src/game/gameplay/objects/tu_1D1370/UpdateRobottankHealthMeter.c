struct S { char p0[0x55C]; int n55C, n560, f564; void *p568; };
void SetWidgetAnimPhase(void *p, int n, float g);
/** @sly2-annot addr=0x1D4C88
 * Purpose: Set meter value 0x34F = health/max (inverted if x564)
 * Info: proposed name UpdateRobottankHealthMeter (T1); TU tu_1D1370
 * Notes: names_v2 v1 proposed PostRemitEmit__FP5REMITP5EMITB (0.001) | class identity from whole_game WE
 *   names/object strings: UpdateRcCarHealthMeter -> UpdateRobottankHealthMeter
 * Confidence: mid (draft) [T1 0.50] - evidence: static naming pass; sources: r5b_rest+whole_game(class
 *   identity)
 * See: docs/reference/functions/gameplay-objects.md#tu_1d1370-0x1d1370-0x1d5150
 * @end-sly2-annot */
void UpdateRobottankHealthMeter(struct S *p) {
    if (p->p568) {
        float g = (float)p->n560 / (float)p->n55C;
        if (p->f564) g = 1.0f - g;
        SetWidgetAnimPhase(p->p568, 0x34F, g);
    }
}
