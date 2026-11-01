struct S { char pad[0x148]; unsigned long fl; };
void func_001F2108(struct S *p);
/** @sly2-annot addr=0x1B4438
 * Purpose: MS::Init (vt7): InitSo then clear flag bit 35 of +0x148
 * Info: proposed name InitMs (T1); class MS slot7; TU ms
 * Confidence: high [T1 0.80] - evidence: static analysis; sources: r5_1AE130
 * See: docs/engine/cutscenes.md#4-source-files, docs/engine/rendering.md#12-source-files,
 *   docs/engine/rendering.md#13-key-functions
 * @end-sly2-annot */
void func_001B4438(struct S *p)
{
    func_001F2108(p);
    p->fl &= ~(1UL << 35);
}
