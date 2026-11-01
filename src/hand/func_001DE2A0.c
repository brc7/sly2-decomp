struct S { char pad[0x148]; unsigned long fl; };
void func_001F2108(struct S *p);
/** @sly2-annot addr=0x1DE2A0
 * Purpose: SENSOR::Init (vt7): InitSo; set SO grf bit 34
 * Info: proposed name InitSensor (T1); class SENSOR slot7; TU sensor
 * Notes: superseded r6_1D5150 proposal: InitSensor? (0.3)
 * Confidence: mid (draft) [T1 0.55] - evidence: static analysis; sources: r6b_rest
 * See: docs/engine/gameplay-objects.md#gameplay-object-classes, docs/engine/gameplay-objects.md#source-files,
 *   docs/engine/hud-ui.md#4-source-files
 * @end-sly2-annot */
void func_001DE2A0(struct S *p) {
    func_001F2108(p);
    p->fl |= 0x400000000UL;
}
