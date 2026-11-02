#include "common.h"
struct G { ulong a; ulong b; ulong c; };
extern char D_002DEA00[];
extern struct G D_002DECD0;
extern char D_002DFB00[];
extern char D_002DFB20[];
void AddPrimPack__4GIFSiiUl(void *g, int a, int b, ulong c);
void PackAD__4GIFSll(void *g, int reg, ulong v);
void EndDmaCnt__4DMAS(void *g);
void AddDmaRefs__4DMASiP2QW(void *g, int n, void *pv);
void AddDmaCnt__4DMAS(void *g);
void PackRGBA__4GIFSUi(void *g, int rgba);
void PackUV__4GIFSii(void *g, int a, int b);
void PackXYZ__4GIFSiii(void *g, int a, int b, int c);
/** @sly2-annot addr=0x1475C0
 * Purpose: GIF/DMA packet copying the frame buffer into a texture (D_002DECD0)
 * Info: proposed name CaptureFramebufferToTexture (T2); TU tu_1471C0
 * Confidence: low (draft) [T2 0.40] - evidence: static analysis; sources: r2b_rest
 * See: docs/reference/functions/cutscenes.md#tu_1471c0-0x1471c0-0x147e20
 * @end-sly2-annot */
void func_001475C0(void)
{
    void *g = D_002DEA00;
    struct G *pg = &D_002DECD0;
    AddPrimPack__4GIFSiiUl(g, 0, 1, 0xE);
    PackAD__4GIFSll(g, 0x4D, (pg->c & 0x1FF) | (pg->a & 0x3F0000));
    PackAD__4GIFSll(g, 0x48, 0x31001);
    EndDmaCnt__4DMAS(g);
    AddDmaRefs__4DMASiP2QW(g, 2, D_002DFB00);
    AddDmaCnt__4DMAS(g);
    AddPrimPack__4GIFSiiUl(g, 0x316, 7, 0x53531EE);
    PackAD__4GIFSll(g, 0x15, 0x61);
    PackAD__4GIFSll(g, 0x3F, 0);
    PackRGBA__4GIFSUi(g, 0x80808080);
    PackUV__4GIFSii(g, 8, 8);
    PackXYZ__4GIFSiii(g, 0x7000, 0x7900, 0);
    PackUV__4GIFSii(g, 0x2008, 0xE08);
    PackXYZ__4GIFSiii(g, 0x8000, 0x8700, 0);
    EndDmaCnt__4DMAS(g);
    AddDmaRefs__4DMASiP2QW(g, 2, D_002DFB20);
    AddDmaCnt__4DMAS(g);
    AddPrimPack__4GIFSiiUl(g, 0x316, 7, 0x53531EE);
    PackAD__4GIFSll(g, 0x15, 0x61);
    PackAD__4GIFSll(g, 0x3F, 0);
    PackRGBA__4GIFSUi(g, 0x80808080);
    PackUV__4GIFSii(g, 8, 8);
    PackXYZ__4GIFSiii(g, 0x8000, 0x7900, 0);
    PackUV__4GIFSii(g, 0x2008, 0xE08);
    PackXYZ__4GIFSiii(g, 0x9000, 0x8700, 0);
}
