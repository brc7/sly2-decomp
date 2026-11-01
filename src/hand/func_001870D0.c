extern int D_002DFBC0, D_002DFBC4, D_002E0CBC;
/** @sly2-annot addr=0x1870D0
 * Purpose: g_grfjoyt (D_002DFBC0) = 5/0/5/4/7 from grfusr bits 0x10/0xC/1/2; if not bit 2 reset
 *   D_002E0CBC=0xFFFF
 * Info: proposed name UpdateGrfjoytFromGrfusr (T1); TU joy
 * Confidence: high [T1 0.85] - evidence: static analysis; sources: r4b_rest
 * See: docs/engine/main-loop-startup.md#8-key-functions,
 *   docs/reference/functions/main-loop-startup.md#joy-0x187030-0x188488
 * @end-sly2-annot */
void func_001870D0(void)
{
    int v = D_002DFBC4;
    if (v & 0x10) D_002DFBC0 = 5;
    else if (v & 0xC) D_002DFBC0 = 0;
    else if (v & 1) D_002DFBC0 = 5;
    else if (v & 2) D_002DFBC0 = 4;
    else D_002DFBC0 = 7;
    if (!(D_002DFBC0 & 2)) D_002E0CBC = 0xFFFF;
}
