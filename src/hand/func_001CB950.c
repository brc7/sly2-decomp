struct S { char pad[0x38]; float g38; };
extern float D_002C1EC0;
void func_001A5C48(struct S *p);
float func_001E7D90(struct S *p);
/** @sly2-annot addr=0x1CB950
 * Purpose: RAIL vt15 PostLoad: base then if length (+0x38)==1e10 compute path length + margin
 * Info: proposed name PostRailLoad (T1); class RAIL slot15; TU rail
 * Confidence: high [T1 0.80] - evidence: static analysis; sources: r5_1AE130
 * See: docs/reference/functions/player.md#rail-0x1cb908-0x1cbeb0
 * @end-sly2-annot */
void func_001CB950(struct S *p)
{
    func_001A5C48(p);
    if (p->g38 == 1e10f) p->g38 = func_001E7D90(p) + D_002C1EC0;
}
