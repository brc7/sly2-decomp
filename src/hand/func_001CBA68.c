struct S { char pad[0xE4]; int ne4; };
float func_001E6620(struct S *p);
float func_001E6648(struct S *p);
/** @sly2-annot addr=0x1CBA68
 * Purpose: GRABSPOT iface: if enabled return path [low,high] distance
 * Info: proposed name RAIL_FGetGrabRange (T2); class RAIL; TU rail
 * Notes: names_v2 B proposed CloneHpnt__FP4HPNTT0 (0.923)
 * Confidence: low (draft) [T2 0.35] - evidence: static naming pass; sources: r5_1AE130
 * See: docs/reference/functions/player.md#rail-0x1cb908-0x1cbeb0
 * @end-sly2-annot */
int func_001CBA68(struct S *p, float *pg)
{
    /*B*/if (p->ne4) { pg[0] = func_001E6620(p); pg[1] = func_001E6648(p); return 1; } return 0;/*E*/
}
