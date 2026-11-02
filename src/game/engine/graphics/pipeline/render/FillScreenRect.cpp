struct RGBA { unsigned char r, g, b, a; };
#include "gifs.h"
extern "C" void FillScreenRect(struct RGBA*,GIFS*,float,float,float,float);
/** @sly2-annot addr=0x1CD5F0
 * Purpose: GIF sprite rectangle in screen space with RGBA (alpha-blended if a<0xFF)
 * Info: proposed name FillScreenRect (T1); TU render
 * Confidence: high [T1 0.80] - evidence: static analysis; sources: r5_1AE130+verdicts_A2
 * See: docs/engine/player.md#10-source-files, docs/engine/rendering.md#12-source-files,
 *   docs/engine/rendering.md#13-key-functions
 * @end-sly2-annot */
void FillScreenRect(struct RGBA *pc, GIFS *pgs, float x0, float y0, float x1, float y1)
{
    pgs->AddPrimPack( pc->a <= 0xFE ? 0x46 : 6, 5, 0x441EE);
    pc->a = (pc->a + 1) >> 1;
    pgs->PackAD( 0x47, 0x31001);
    pgs->PackAD( 0x42, 0x44);
    pgs->PackRGBA( *(int *)pc);
    pgs->PackXYZF( (int)((x0 * 0.8f + 1792.0f) * 16.0f), (int)((y0 * 0.45454547f + 1936.0f) * 16.0f), 0, 0);
    pgs->PackXYZF( (int)((x1 * 0.8f + 1792.0f) * 16.0f), (int)((y1 * 0.45454547f + 1936.0f) * 16.0f), 0, 0);
}
