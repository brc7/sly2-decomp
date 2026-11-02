#include "vifs.h"
#include "gifs.h"
extern char D_0025C1C0[];
extern char D_0025E040[];
extern char D_00001AA0[];
extern int D_003E12C8;
extern QW D_002DF800[];
extern int D_002DEB70[2];
extern "C" void EnsureVu1Code__FP4VIFSPvT1(VIFS *pvifs, void *pvStart, void *pvEnd);
extern "C" void AddFramePatch(int n, GIFS *pgifs, short **pps);
extern "C" void func_0016D6C8(GIFS *pgifs, uint grf, short **pps);
/** @sly2-annot addr=0x16DA48
 * Purpose: Pass 6/7 begin (kinds 7/8): upload projected-volume microprogram D_0025C1C0..D_0025E040,
 *   build/kick (MSCAL 0x1AA0) the masked full-screen stencil-init strips
 * Info: proposed name BeginRpProjVolume (T1); TU glob
 * Notes: no usable identifier; purpose only
 * Confidence: mid (draft) [T1 0.60] - evidence: static analysis; sources: r3_1539A8+rendering
 * See: docs/engine/rendering.md#4-the-26-kinds-and-12-passes-cstatic-tables-dumped-from-the-elf-roles-cstatic-unless-marked,
 *   docs/engine/rendering.md#7-shadow-volumes-kinds-78-p6p7-prog4-z-fail-counting-in-the-green-channel-cstatic,
 *   docs/engine/rendering.md#13-key-functions
 * @end-sly2-annot */
extern "C" void BeginRpProjVolume(VIFS *pvifs, int n1, int n2)
{
    EnsureVu1Code__FP4VIFSPvT1(pvifs, D_0025C1C0, D_0025E040);
    pvifs->AddDmaCnt();
    if (n2 != 7 && D_003E12C8) {
        pvifs->AddVifFlush();
        pvifs->AddDmaCall(D_002DF800);
        pvifs->AddVifBaseOffset(0x32, 0x1E7);
    }
    uint *pun;
    pvifs->UnpackHelper(UPK_V2_16, 5, 0x1B, 0, &pun);
    short *ps = (short *)pun;
    GIFS gifs;
    QW aqw[0x100];
    gifs.AllocStatic(0x100, aqw);
    if (n2 != 7)
        func_0016D6C8(&gifs, 0x299, &ps);
    gifs.AddPrimPack(0, 1, 0xE);
    AddFramePatch(0, &gifs, &ps);
    if (n1 == 7 || n2 != 7)
        gifs.PackAD(0x4C, 0xFFFF00FF00000000L);
    else
        gifs.PackAD(0x4C, 0xFFFF80FF00000000L);
    gifs.PackAD(0x46, 0);
    gifs.PackAD(0x47, 0x31001);
    if (n1 == 7)
        gifs.PackAD(1, 0x100007E00L);
    else
        gifs.PackAD(1, 0x100003E00L);
    gifs.PackAD(0, 6);
    gifs.AddPrimList(2, 0x55);
    for (int x = 0; x < 0x200; x += 0x20) {
        gifs.ListXYZ((0x700 + x) * 16, 0x7900, 0);
        gifs.ListXYZ((0x720 + x) * 16, 0x8700, 0);
    }
    if (n1 == 8 && n2 == 7) {
        gifs.AddPrimPack(0, 1, 0xE);
        AddFramePatch(0, &gifs, &ps);
        gifs.PackAD(0x4C, 0xFFFF00FF00000000L);
    }
    gifs.EndPrim(1);
    AddFramePatch(-1, &gifs, &ps);
    pvifs->AddVifUnpack(UPK_V4_32, (uint)(gifs.m_pb - gifs.m_ab) >> 4, gifs.m_ab, 0x23);
    pvifs->AddVifFlusha();
    pvifs->AddVifMscal(D_00001AA0);
    pvifs->AddVifFlusha();
    pvifs->EndDmaCnt();
    D_002DEB70[n1 == 8 ? 1 : 0] = 1;
}
