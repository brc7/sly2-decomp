struct S { char pad[0x38]; float g38; };
extern float D_002C0D00;
void func_001A5C48(struct S *p);
float func_001E7D90(struct S *p);
/** @sly2-annot addr=0x1C3458
 * Purpose: PIPE vt15: base PostLoad + default length
 * Info: proposed name PostPipeLoad (T1); class PIPE slot15; TU pipe
 * Notes: names_v2 v1 proposed CollectJtPrize__FP2JT3PCKP3ALO (0.008)
 * Confidence: high [T1 0.80] - evidence: static naming pass; sources: r5_1AE130
 * See: docs/reference/functions/player.md#pipe-0x1c3390-0x1c3d70
 * @end-sly2-annot */
void func_001C3458(struct S *p)
{
    func_001A5C48(p);
    if (p->g38 == 1e10f) p->g38 = func_001E7D90(p) + D_002C0D00;
}
