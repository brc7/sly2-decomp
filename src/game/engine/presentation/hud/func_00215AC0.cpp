struct RGBA { unsigned int u; RGBA() {} RGBA(unsigned int x) { u = x; } };
struct Q { int a[4]; } __attribute__((aligned(16)));
struct F { char pad[0x54]; char *p54; };
struct G { char pad[0x3BE8]; F *pf; };
extern "C" G D_003DD490;
extern "C" Q D_002A2E10;
extern "C" Q D_002A2E20;
struct S { int x0; int x4; char *x8; char *xC; char *x10; int x14; RGBA x18; char pad1c[0x24]; Q q40; Q q50;
  char pad60[4]; float f64; float f68; int x6C; int x70; int x74; int x78; float f7C; float f80; float f84; float f88;
  int x8C; int x90; int x94; };
extern "C" void func_00215CA8(S *p, int i);
/** @sly2-annot addr=0x215AC0
 * Purpose: rich-text ctor from string + font + scale (offsets 1792/1936)
 * Info: proposed name CRichText_ctor (T1); class CRichText; TU hud
 * Confidence: mid (draft) [T1 0.50] - evidence: static analysis; sources: r7c_hud
 * See: docs/reference/functions/hud-ui.md#hud-0x210eb0-0x21bb88
 * @end-sly2-annot */
extern "C" S *func_00215AC0(S *p, int a, float f)
{
    p->x0 = a;
    p->f7C = 1.0f;
    p->f80 = 1.0f;
    p->f64 = 1792.0f;
    p->f68 = 1936.0f;
    p->f88 = f;
    p->x6C = 0;
    p->x70 = 0;
    p->x74 = 0;
    p->x78 = 0;
    p->f84 = f * 51.2f;
    p->x90 = 1;
    p->x8C = 1;
    p->x94 = 6;
    p->x8 = p->xC = D_003DD490.pf->p54;
    p->x10 = D_003DD490.pf->p54 + 0x40;
    p->x18 = RGBA(0x80808080);
    p->q40 = D_002A2E10;
    p->q50 = D_002A2E20;
    func_00215CA8(p, 0);
    return p;
}
