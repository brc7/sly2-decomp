struct S { char pad[0xE4]; int ne4; };
float func_001E6620(struct S *p);
float func_001E6648(struct S *p);
/** @sly2-annot addr=0x1C3BB0
 * Purpose: GRABSPOT iface: [low high] if enabled
 * Info: proposed name PIPE_FGetGrabRange (T2); class PIPE; TU pipe
 * Confidence: low (draft) [T2 0.35] - evidence: static analysis; sources: r5_1AE130
 * See: docs/reference/functions/player.md#pipe-0x1c3390-0x1c3d70
 * @end-sly2-annot */
int func_001C3BB0(struct S *p, float *pg)
{
    /*B*/if (p->ne4) { pg[0] = func_001E6620(p); pg[1] = func_001E6648(p); return 1; } return 0;/*E*/
}
