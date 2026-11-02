#include "gifs.h"
struct sceDmaChan;
extern int D_002DEB24;
extern int D_003E12C8;
extern int D_002DEDB0;
extern int D_002DEDB8;
extern char D_002DEBE0[];
extern char D_002DECD0[];
extern sceDmaChan *D_002DE598;
void SendDmaSyncGsFinish(sceDmaChan *pch, QW *pqw);
static inline ulong Tex0FromDisp(ulong disp)
{
    return ((disp & 0x1FF) << 5) | (((disp >> 16) & 0x3F) << 14) | (((disp >> 24) & 0x3F) << 20);
}
/** @sly2-annot addr=0x160F98
 * Purpose: Draws the other (previous) framebuffer back over the current one as 32-px textured strips with
 *   alpha 0x64/0x80 (motion-blur/frame blend), unless disabled by D_002DEB24/D_003E12C8
 * Info: proposed name BlendPrevFrame (T1); TU frm
 * Notes: verifier: purpose partly wrong: Sly 1 has BlendPrevFrame in frm; same callers (FrameRenderLoop,
 *   FlushFrames). Draws the other framebuffer as 32-px sprite strips. Detail wrong: ALPHA_1=0x300000
 * Confidence: high [T1 0.85] - evidence: Sly 1 / Lilly reference; sources: r3_1539A8+verdicts_tu_r3r5
 * See: docs/engine/rendering.md#1-frame-structure, docs/engine/rendering.md#13-key-functions,
 *   docs/reference/functions/rendering.md#frm-0x1607a0-0x1613a8
 * @end-sly2-annot */
extern "C" void BlendPrevFrame__Fv()
{
    if (D_002DEB24 || D_003E12C8)
        return;
    GIFS gifs;
    QW aqw[0x64];
    gifs.AllocStatic(0x64, aqw);
    gifs.AddDmaCnt();
    gifs.AddPrimPack(0, 1, 0xE);
    gifs.PackAD(0x3F, 0);
    ulong *pdisp = (ulong *)((D_002DEDB0 & 1) ? D_002DEBE0 : D_002DECD0);
    gifs.PackAD(6, Tex0FromDisp(*pdisp) | 0x2A8000000L);
    gifs.PackAD(0x14, 0x61);
    gifs.PackAD(8, 5);
    gifs.PackAD(0x47, 0x33001);
    gifs.PackAD(0x42, 0x3000000064L);
    gifs.PackAD(1, 0x3F80000080808080L);
    gifs.PackAD(0x40, 0xDE000101FF0000L);
    gifs.AddPrimPack(0x156, 2, 0x43);
    int v = (D_002DEDB8 & 1) ? 0 : 0x10;
    for (int x = 0; x < 0x200; x += 0x20) {
        gifs.PackUV((x << 4) | 8, v);
        gifs.PackXYZF((x + 0x700) << 4, 0x7900, 0, 0);
        gifs.PackUV(((x + 0x20) << 4) | 8, v + 0xE00);
        gifs.PackXYZF((x + 0x720) << 4, 0x8700, 0, 0);
    }
    gifs.AddPrimPack(0, 1, 0xE);
    gifs.PackAD(0x40, 0xDF000001FF0000L);
    gifs.PackAD(0x61, 0);
    gifs.AddPrimEnd();
    gifs.AddDmaEnd();
    gifs.EndDmaCnt();
    SendDmaSyncGsFinish(D_002DE598, aqw);
}
