struct CLK { char pad[0x18]; float af[9]; };
extern struct CLK D_002DDED0;
void RecalcClockRate(void *p);
/** @sly2-annot addr=0x14D068
 * Purpose: t = 0; resets slots 0,1,2,4-8 to 1.0 (**not slot 3**), recomputes the rate
 * Info: proposed name ResetClock (T1); TU clock
 * Confidence: mid (draft) [T1 0.65] - evidence: semantic review; sources: semantic_tu_docs/clock
 * See: docs/engine/main-loop-startup.md#8-key-functions,
 *   docs/reference/functions/main-loop-startup.md#clock-0x14cfe0-0x14d3d8
 * @end-sly2-annot */
void ResetClock(int *p)
{
    *p = 0;
    D_002DDED0.af[0] = 1.0f;
    D_002DDED0.af[1] = 1.0f;
    D_002DDED0.af[2] = 1.0f;
    D_002DDED0.af[5] = 1.0f;
    D_002DDED0.af[6] = 1.0f;
    D_002DDED0.af[8] = 1.0f;
    D_002DDED0.af[7] = 1.0f;
    D_002DDED0.af[4] = 1.0f;
    RecalcClockRate(p);
}
