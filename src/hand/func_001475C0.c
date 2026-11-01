#include "common.h"
struct G { ulong a; ulong b; ulong c; };
extern char D_002DEA00[];
extern struct G D_002DECD0;
extern char D_002DFB00[];
extern char D_002DFB20[];
void func_00166BD8(void *g, int a, int b, ulong c);
void func_00166E90(void *g, int reg, ulong v);
void func_001576F0(void *g);
void func_00157500(void *g, int n, void *pv);
void func_001574B8(void *g);
void func_00166C70(void *g, int rgba);
void func_00166CE0(void *g, int a, int b);
void func_00166DA0(void *g, int a, int b, int c);
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
    func_00166BD8(g, 0, 1, 0xE);
    func_00166E90(g, 0x4D, (pg->c & 0x1FF) | (pg->a & 0x3F0000));
    func_00166E90(g, 0x48, 0x31001);
    func_001576F0(g);
    func_00157500(g, 2, D_002DFB00);
    func_001574B8(g);
    func_00166BD8(g, 0x316, 7, 0x53531EE);
    func_00166E90(g, 0x15, 0x61);
    func_00166E90(g, 0x3F, 0);
    func_00166C70(g, 0x80808080);
    func_00166CE0(g, 8, 8);
    func_00166DA0(g, 0x7000, 0x7900, 0);
    func_00166CE0(g, 0x2008, 0xE08);
    func_00166DA0(g, 0x8000, 0x8700, 0);
    func_001576F0(g);
    func_00157500(g, 2, D_002DFB20);
    func_001574B8(g);
    func_00166BD8(g, 0x316, 7, 0x53531EE);
    func_00166E90(g, 0x15, 0x61);
    func_00166E90(g, 0x3F, 0);
    func_00166C70(g, 0x80808080);
    func_00166CE0(g, 8, 8);
    func_00166DA0(g, 0x8000, 0x7900, 0);
    func_00166CE0(g, 0x2008, 0xE08);
    func_00166DA0(g, 0x9000, 0x8700, 0);
}
