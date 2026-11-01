struct S { char p0[0x40]; int x40; char p1[0x148-0x44]; unsigned long fl; char p2[0x1C0-0x150]; float g1C0; };
void func_001DE2A0(struct S *p);
/** @sly2-annot addr=0x1DE400
 * Purpose: LASEN::Init (vt7): InitSensor; speed(+0x1C0)=1.0; clear grf bit 35; set LO flag 8
 * Info: proposed name InitLasen (T2); class LASEN slot7; TU sensor
 * Notes: superseded r6_1D5150 proposal: InitLasen? (0.25) | names_v2 C proposed SetLightKind__FP5LIGHT6LIGHTK
 *   (0.638)
 * Confidence: low (draft) [T2 0.45] - evidence: static naming pass; sources: r6b_rest
 * See: docs/reference/functions/gameplay-objects.md#sensor-0x1de2a0-0x1dfad8
 * @end-sly2-annot */
void func_001DE400(struct S *p) {
    int v;
    func_001DE2A0(p);
    p->g1C0 = 1.0f;
    p->fl &= ~0x800000000UL;
    v = p->x40;
    if ((v ^ 1) & 1) *(int *)v |= 8;
    else p->x40 = v | 8;
}
