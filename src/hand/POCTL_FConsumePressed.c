int GrfConsumePressed__5POCTLUif(void *p, float g);
/** @sly2-annot addr=0x1C4700
 * Purpose: GrfConsumePressed(grf 0.0) != 0
 * Info: proposed name POCTL_FConsumePressed (T1); class POCTL; TU po
 * Notes: names_v2 C proposed InitRost__FP4ROST (0.638)
 * Confidence: mid (draft) [T1 0.50] - evidence: static naming pass; sources: r5_1AE130
 * See: docs/reference/functions/gameplay-objects.md#rhythm-0x1520c8-0x1539a8,
 *   docs/reference/functions/gameplay-objects.md#turret-0x2259d8-0x226550,
 *   docs/reference/functions/player.md#po-0x1c3d70-0x1c4e50
 * @end-sly2-annot */
int POCTL_FConsumePressed(void *p) { return GrfConsumePressed__5POCTLUif(p, 0.0f) != 0; }
