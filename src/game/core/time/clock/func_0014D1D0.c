struct X { char pad[0x18]; float af[4]; };
void func_0014D1F8(struct X *p);
/** @sly2-annot addr=0x14D1D0
 * Purpose: slot[islot] = r; RecalcClockRate()
 * Info: proposed name SetClockRate (T1); TU clock
 * Confidence: mid (draft) [T1 0.70] - evidence: semantic review; sources: semantic_tu_docs/clock
 * See: docs/engine/main-loop-startup.md#2-clock-clockcpp-0x14cfe0-0x14d3d8,
 *   docs/engine/main-loop-startup.md#8-key-functions, docs/guides/live-pine.md#4-clock-experiment
 * @end-sly2-annot */
void func_0014D1D0(struct X *p, int i, float f)
{
    p->af[i] = f;
    func_0014D1F8(p);
}
