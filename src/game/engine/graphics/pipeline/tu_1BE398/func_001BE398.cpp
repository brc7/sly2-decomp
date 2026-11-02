#include "gifs.h"
struct TEX0_FIELDS {
    ulong tbp0 : 14;
    ulong tbw : 6;
    ulong psm : 6;
    ulong tw : 4;
    ulong th : 4;
    ulong tcc : 1;
    ulong rest : 29;
};
union TEX0 { TEX0_FIELDS fields; long word; };
struct T {
    int dx, dy;
    TEX0 tex0;
    int c10;
    QW *a14;
    int c18;
    QW *a1C;
};
struct X { int x0, x4, x8; };
extern "C" {
int IgsAllocGsb__FP3GSBi(X *px, int cb);
uint func_001703C0(uint n);
}

static inline int PackNext(GIFS &g, int x, int y)
{
    g.PackXYZ(x << 4, y << 4, 0);
    return x;
}
#define FIELD_OFFSET(t,m) ((unsigned int)&(((t*)0x10000)->m)-0x10000)
typedef char T_dx_Offset[FIELD_OFFSET(T,dx)==0?1:-1];
typedef char T_dy_Offset[FIELD_OFFSET(T,dy)==4?1:-1];
typedef char T_tex0_Offset[FIELD_OFFSET(T,tex0)==8?1:-1];
typedef char T_c10_Offset[FIELD_OFFSET(T,c10)==16?1:-1];
typedef char T_a14_Offset[FIELD_OFFSET(T,a14)==20?1:-1];
typedef char T_c18_Offset[FIELD_OFFSET(T,c18)==24?1:-1];
typedef char T_a1C_Offset[FIELD_OFFSET(T,a1C)==28?1:-1];
typedef char X_x8_Offset[FIELD_OFFSET(X,x8)==8?1:-1];
typedef char TEX0_fields_Offset[FIELD_OFFSET(TEX0,fields)==0?1:-1];
typedef char TEX0_word_Offset[FIELD_OFFSET(TEX0,word)==0?1:-1];
typedef char T_Size[sizeof(T)==32?1:-1];
typedef char X_Size[sizeof(X)==12?1:-1];
typedef char TEX0_FIELDS_Size[sizeof(TEX0_FIELDS)==8?1:-1];
typedef char TEX0_Size[sizeof(TEX0)==8?1:-1];
typedef char long_Size[sizeof(long)==8?1:-1];
typedef char ulong_Size[sizeof(ulong)==8?1:-1];
typedef char QW_Size[sizeof(QW)==16?1:-1];
typedef char DMAS_Size[sizeof(DMAS)==28?1:-1];
typedef char GIFS_Size[sizeof(GIFS)==40?1:-1];
typedef char int_Size[sizeof(int)==4?1:-1];
typedef char QW_Alignment[__alignof__(QW)==16?1:-1];
/** @sly2-annot addr=0x1BE398
 * Purpose: Allocate GS buffers and build TEX0/CLUT setup packets for the two offscreen item-render slots
 *   (D_003E1A78, D_003E1AA0)
 * Info: proposed name InitItemRenderTargets (T2); TU tu_1BE398
 * Confidence: low (draft) [T2 0.40] - evidence: static analysis; sources: r5b_rest
 * See: docs/engine/physics-collision.md#6-source-files, docs/engine/rendering.md#12-source-files,
 *   docs/reference/functions/rendering.md#tu_1be398-0x1be398-0x1bf110
 * @end-sly2-annot */
extern "C" void func_001BE398(T *pt, X *px)
{
    register int cbBlock __asm__("$17") = 0x40;
    register int negative __asm__("$19") = -1;
    register int divisor __asm__("$2") = 0x40;
    pt->dx = 0x80;
    pt->dy = 0x80;
    int tbw = pt->dx / divisor;
    int tbp = IgsAllocGsb__FP3GSBi(px, 0x100);
    pt->tex0.fields.tbp0 = tbp;
    pt->tex0.fields.tbw = tbw;
    pt->tex0.fields.psm = 0;
    tbp >>= 5;
    pt->tex0.fields.tw = func_001703C0(pt->dx);
    pt->tex0.fields.th = func_001703C0(pt->dy);
    pt->tex0.fields.tcc = 1;
    int zbp = (px->x8 - 0x100) >> 5;
    GIFS gifs;
    int xOff = 0x800 - pt->dx / 2;
    int yOff = 0x800 - pt->dy / 2;
    QW aqw[0x40];
    gifs.AllocStatic(0x40, aqw);
    gifs.AddPrimPack(0, 1, 0xE);
    int w = pt->dx;
    register int m __asm__("$2");
    __asm__("daddu %0,%1,$zero" : "=r"(m) : "r"(cbBlock));
    cbBlock = cbBlock < w;
    if (cbBlock) m = w;
    negative = negative < m;
    int pages = m + 63;
    if (negative) pages = m;
    gifs.PackAD(0x4C, tbp | ((ulong)(pages >> 6) << 16));
    gifs.PackAD(0x4E, zbp | 0x30000000);
    gifs.PackAD(0x18, (xOff << 4) | ((ulong)(yOff << 4) << 32));
    gifs.PackAD(0x40, ((ulong)(pt->dx - 1) << 16) | ((ulong)(pt->dy - 1) << 48));
    gifs.PackAD(0x47, 0x30000);
    gifs.PackAD(1, 0);
    gifs.AddPrimPack(6, 1, 5);
    int x = xOff;
    while (x < xOff + pt->dx) {
        gifs.PackXYZ(x << 4, yOff << 4, 0);
        x = PackNext(gifs, x + 0x20, yOff + pt->dy);
    }
    gifs.AddPrimEnd();
    gifs.DetachCopySw(&pt->c10, 0, &pt->a14);
    gifs.AllocStatic(0x40, aqw);
    gifs.AddPrimPack(0, 1, 0xE);
    gifs.PackAD(6, pt->tex0.word);
    gifs.PackAD(0x14, 0x60);
    gifs.PackAD(8, 5);
    gifs.AddPrimEnd();
    gifs.DetachCopySw(&pt->c18, 0, &pt->a1C);
}
