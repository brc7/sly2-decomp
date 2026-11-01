struct S { int a[4]; float g10, g14, g18, g1c, g20, g24, g28, g2c; int n30; };
void ClearPoctlJoyPressed(struct S *p, int grf);
/** @sly2-annot addr=0x1C42E8
 * Purpose: Zero stick values (dx/dy/rad/s left+right), grfbtn=0, ClearJoyPressed(0xFFFF)
 * Info: proposed name ResetPoctl (T1); class POCTL; TU po
 * Notes: names_v2 B proposed PostRohLoad__FP3ROH (0.793)
 * Confidence: high [T1 0.85] - evidence: static naming pass; sources: r5_1AE130+verdicts_tu_r3r5
 * See: docs/engine/player.md#11-key-functions, docs/reference/functions/player.md#po-0x1c3d70-0x1c4e50
 * @end-sly2-annot */
void ResetPoctl(struct S *p)
{
    p->n30 = 0;
    p->g20 = p->g24 = p->g28 = p->g2c = p->g10 = p->g14 = p->g18 = p->g1c = 0.0f;
    ClearPoctlJoyPressed(p, 0xFFFF);
}
