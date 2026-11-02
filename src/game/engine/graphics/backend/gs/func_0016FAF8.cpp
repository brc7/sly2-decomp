#include "gifs.h"
/** @sly2-annot addr=0x16FAF8
 * Purpose: Builds a one-register GIF A+D packet setting GS FBA_1 (reg 0x4A) to d into a static QW buffer
 * Info: proposed name BuildFbaGifs (T1); TU gs
 * Confidence: mid (draft) [T1 0.60] - evidence: static analysis; sources: r3_1539A8
 * See: docs/reference/functions/rendering.md#gs-0x16f2d0-0x1709a8
 * @end-sly2-annot */
extern "C" void func_0016FAF8(int cqw, QW *aqw, long d)
{
    GIFS gifs;
    gifs.AllocStatic(cqw, aqw);
    gifs.AddPrimPack(0, 1, 0xE);
    gifs.PackAD(0x4A, d);
    gifs.EndPrim(1);
}
