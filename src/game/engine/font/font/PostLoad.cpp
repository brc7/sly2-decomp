#include "gifs.h"
struct HDR { char pad[4]; uchar b4, b5, b6, b7; float g8; ushort hC; };
struct BMP { char pad[0xE]; ushort hE; char pad2[4]; int n14; int n18; char pad3[4]; ulong fl20; };
struct CLUT { char pad[0xA]; ushort hA; int nC; int n10; int pad14; ulong fl18; };
struct FD { HDR *phdr; int n4; BMP *pbmp; CLUT *pclut; };
struct FONT {
    int n0, n4, n8, nC; float g10, g14; char pad[0x50 - 0x18]; ulong fl50; int n58; QW *pqw5C;
    QW aqw60[6]; int nC0; int nC4; int nC8; QW *pqwCC; QW aqwD0[6]; int n130; int n134; int n138; int n13C;
};
extern "C" {
void func_0015F888(FONT *pfont);
int IgsAllocGsb__FP3GSBi(void *p, int n);
void BuildBmpTex0__FP3BMPi(BMP *pbmp, int n);
void func_001ECBF0(BMP *pbmp, int n, GIFS *pgifs);
void func_001EC9D0(CLUT *pclut, int n);
void BuildClutGifs__FP4CLUTiP4GIFS(CLUT *pclut, int n, GIFS *pgifs);
/** @sly2-annot addr=0x15F710
 * Purpose: CFontBrx::PostLoad: copies header metrics, registers the font, allocates GS memory (IgsAllocGsb)
 *   and builds TEX0/CLUT upload GIFs for its bitmap
 * Info: proposed name PostLoad (T1); TU font
 * Notes: names_v2 v1 proposed DrawPart__8CFontBrxffffffffUiP4GIFS (0.932)
 * Confidence: mid (draft) [T1 0.60] - evidence: static naming pass; sources: r3_1539A8
 * See: docs/formats/textures.md#7-open, docs/reference/functions/hud-ui.md#font-0x15ef00-0x1603b8
 * @end-sly2-annot */
void PostLoad(FONT *pfont, FD *pfd, void *p)
{
    HDR *phdr = pfd->phdr;
    pfont->n0 = phdr->b7;
    func_0015F888(pfont);
    pfont->n4 = phdr->b4;
    pfont->n8 = phdr->b5;
    pfont->nC = phdr->b6;
    pfont->g10 = phdr->g8;
    pfont->g14 = phdr->g8;
    pfont->n138 = phdr->hC;
    pfont->n13C = pfd->n4;
    GIFS gifs;
    BMP *pbmp = pfd->pbmp;
    CLUT *pclut = pfd->pclut;
    int n = IgsAllocGsb__FP3GSBi(p, pbmp->hE);
    BuildBmpTex0__FP3BMPi(pbmp, n);
    pfont->fl50 = pbmp->fl20;
    gifs.AllocStatic(6, pfont->aqw60);
    func_001ECBF0(pbmp, n, &gifs);
    gifs.Detach(&pfont->n58, &pfont->pqw5C);
    pfont->nC0 = pbmp->n18;
    pfont->nC4 = pbmp->n14;
    if (pclut) {
        n = IgsAllocGsb__FP3GSBi(p, pclut->hA);
        func_001EC9D0(pclut, n);
        pfont->fl50 |= pclut->fl18;
        gifs.AllocStatic(6, pfont->aqwD0);
        BuildClutGifs__FP4CLUTiP4GIFS(pclut, n, &gifs);
        gifs.Detach(&pfont->nC8, &pfont->pqwCC);
        pfont->n130 = pclut->n10;
        pfont->n134 = pclut->nC;
    }
}
}
