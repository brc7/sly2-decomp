#include "gifs.h"
extern "C" void AddFramePatch(int n, GIFS *pgifs, short **pps);
/** @sly2-annot addr=0x16D6C8
 * Purpose: Builds one projected-volume stencil/apply GIF prim (FRAME_2 mask, TEX0_2, TEX1, ALPHA...) selected
 *   by grf bits
 * Info: proposed name BuildProjVolumeGifs (T2); TU glob
 * Notes: no usable identifier; purpose only
 * Confidence: low (draft) [T2 0.40] - evidence: static analysis; sources: r3_1539A8+rendering
 * See: docs/reference/functions/rendering.md#glob-0x167310-0x16f148
 * @end-sly2-annot */
extern "C" void func_0016D6C8(GIFS *pgifs, uint grf, short **pps)
{
    pgifs->AddPrimPack(0x200, 1, 0xE);
    AddFramePatch((grf & 4) ? 4 : 5, pgifs, pps);
    if (grf & 0x200) {
        pgifs->PackAD(0x4D, 0x3FFF02000000L);
    } else if (grf & 0x100) {
        pgifs->PackAD(0x4D, 0xFFFFC00002000000L);
    } else {
        uint msk = (grf & 0x800000) ? 0x7FFFFFFF : 0xFFFFFFFF;
        if (grf & 0x400000) msk &= 0xFF7FFFFF;
        if (grf & 0x200000) msk &= 0xFFBFFFFF;
        if (grf & 0x100000) msk &= 0xFFDFFFFF;
        if (grf & 0x80000) msk &= 0xFFEFFFFF;
        if (grf & 0x40000) msk &= 0xFFF0FFFF;
        if (grf & 0x20000) msk &= 0xFFFF7FFF;
        if (grf & 0x10000) msk &= 0xFFFFBFFF;
        pgifs->PackAD(0x4D, ((ulong)msk << 32) | 0x2000000);
    }
    AddFramePatch((grf & 1) ? 6 : 7, pgifs, pps);
    pgifs->PackAD(7, 0xEA8200000L);
    pgifs->PackAD(0x15, 1);
    pgifs->PackAD(0x3F, 0);
    pgifs->PackAD(0x45, 0);
    pgifs->PackAD(0x19, 0);
    pgifs->PackAD(0x41, 0x1C0000002000000L);
    pgifs->PackAD(0x48, 0x31001);
    pgifs->PackAD(0x43, 0x2A);
    pgifs->PackAD(0x3B, 0x8000000000L);
    pgifs->PackAD(1, 0x180808080L);
    pgifs->PackAD(0, 0x356);
    pgifs->AddPrimList(4, 0x5353);
    int u = (grf & 0x10) ? 0 : 8;
    for (int x = (grf & 0x40) ? 0 : 8; x < 0x200; x += 0x10, u += 0x10) {
        pgifs->ListUV((u << 4) + 8, 8);
        pgifs->ListXYZ(x << 4, 0, 0);
        pgifs->ListUV(((u + 8) << 4) + 8, 0x1C08);
        pgifs->ListXYZ((x + 8) << 4, 0x1C00, 0);
    }
    pgifs->AddPrimPack(0x200, 1, 0xE);
    pgifs->PackAD(0x41, 0xE0000002000000L);
}
