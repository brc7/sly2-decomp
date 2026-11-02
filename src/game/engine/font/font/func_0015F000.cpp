#include "gifs.h"
/** @sly2-annot addr=0x15F000
 * Purpose: If requested, restores a full-screen SCISSOR_1 (0..511 x 0..223) after text drawing
 * Info: proposed name CleanupDraw (T1); TU font
 * Notes: names_v2 C proposed DyWrapPchz__5CFontPcf (0.167)
 * Confidence: mid (draft) [T1 0.50] - evidence: static naming pass; sources: r3_1539A8
 * See: docs/reference/functions/hud-ui.md#font-0x15ef00-0x1603b8
 * @end-sly2-annot */
extern "C" void func_0015F000(void *p, int f, GIFS *pgifs)
{
    if (f) {
        pgifs->AddPrimPack(0, 1, 0xe);
        pgifs->PackAD(0x40, 0xdf000001ff0000L);
    }
}
