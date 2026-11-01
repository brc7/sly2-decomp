#include "gifs.h"
/** @sly2-annot addr=0x138F08
 * Purpose: Build static blip GIF packet for blend mode n
 * Info: proposed name BuildBlipAqwGifs (T1); TU blip
 * Confidence: high [T1 0.90] - evidence: static analysis; sources: r2b_rest+verdicts_A1
 * See: docs/engine/animation.md#6-source-files, docs/engine/particles-effects.md#5-source-files,
 *   docs/engine/particles-effects.md#6-key-functions
 * @end-sly2-annot */
extern "C" void func_00138F08(int n, QW *aqw)
{
    GIFS gifs;
    gifs.AllocStatic(4, aqw);
    gifs.AddPrimPack(0, 1, 0xe);
    gifs.PackAD(0x47, 0x7180b);
    gifs.PackAD(0x42, ((long)n << 2) | 0x40);
    gifs.AddPrimBulk(0x54, 0, 10, 0x424242412eUL, 0);
    gifs.EndDmaCnt();
}
