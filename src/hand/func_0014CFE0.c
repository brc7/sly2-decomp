struct CLOCK {
    float t;
    float dt;
    float rDt;
    float rDtPrev;
    char pad[8];
    float af[9];
    char pad2[0x48 - 0x3C];
    unsigned long tick;
    unsigned long tickLast;
    int hi;
};
extern struct CLOCK D_002DDED0;
unsigned long func_0014D240(struct CLOCK *p);
void func_0014D068(struct CLOCK *p);
/** @sly2-annot addr=0x14CFE0
 * Purpose: Seeds the tick history from CP0 Count, g_clock.tick = TickNow(), sets all 9 rate slots and
 *   rDt/rDtReal to 1, calls ResetClock
 * Info: proposed name StartupClock (T1); TU clock
 * Notes: names_v2 v1 proposed ClsgClipCylinderToObject__FP2SOP6VECTORT1fiP3LSG (0.638)
 * Confidence: high [T1 0.85] - evidence: semantic review; static naming pass; sources: semantic_tu_docs/clock
 * See: docs/engine/main-loop-startup.md#7-source-files, docs/engine/main-loop-startup.md#8-key-functions,
 *   docs/engine/main-loop-startup.md#open-questions
 * @end-sly2-annot */
void func_0014CFE0(void)
{
    unsigned long c[2];
    unsigned int i;
    float *pf;
    __asm__ volatile ("mfc0 %0, $9" : "=r"(c[0]));
    D_002DDED0.tickLast = c[0];
    D_002DDED0.tick = func_0014D240(&D_002DDED0);
    for (i = 0; i < 9; i++) D_002DDED0.af[i] = 1.0f;
    D_002DDED0.rDt = 1.0f;
    D_002DDED0.rDtPrev = 1.0f;
    func_0014D068(&D_002DDED0);
}
