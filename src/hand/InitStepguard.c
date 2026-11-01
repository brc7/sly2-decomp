// (Claude agent, nonvu16, 2026-10-17) plain stores, no register pins; store order tuned (see notes/200000.md).
struct S;
struct BLOCK {char unknown[0xC];struct S *owner;char tail[0x14];};
struct F64 { char pad[0x40]; int u40; };
struct S {
    char pad0[0x148]; unsigned long u148_lo : 16; unsigned long u148_f : 4; unsigned long u148_hi : 44;
    char pad1[0x260 - 0x150]; float f260; char pad2[0x2CC - 0x264]; float f2CC; float f2D0;
    char pad3[0x314 - 0x2D4]; float f314; float f318; float f31C; float f320; float f324; float f328;
    char pad4[0x350 - 0x32C]; int n350; int n354; int n358; int pad35C; float f360; int n364; char pad5[0x370 - 0x368];
    float f370; float f374; float f378; int n37C; float f380; float f384; float f388;
    char pad6[0x3D8 - 0x38C]; int n3D8; char pad7[0x3F0 - 0x3DC]; int n3F0; char pad8[0x3FC - 0x3F4]; int n3FC;
    char pad9[0x4A0 - 0x400]; int n4A0; char padA[0xE64 - 0x4A4]; int nE64; int nE68; float fE6C; float fE70;
    char padB[0xE78 - 0xE74]; int nE78; float fE7C; float fE80; int nE84; char padC[0xF40 - 0xE88]; int nF40; int padF44;
    float fF48; float fF4C; float fF50; float fF54; float fF58; float fF5C; char padD[0xF64 - 0xF60]; struct F64 *pF64;
    char padE[0xF80 - 0xF68]; int nF80; char padF[0xFBC - 0xF84]; int nFBC; char padG[0xFD0 - 0xFC0]; struct BLOCK block;
    float fFF4; char padH[0x1000 - 0xFF8]; int n1000;
};
extern float D_002A8600;
extern float D_002A8604;
extern int D_002C3CF0;
void InitStep(struct S *p);
void EnsureStepguardManager(void);
struct F64 *PloNew__F3CIDP2SO3OIDi(int a, void *b, int c, int d);
void SetLoBusyk(struct F64 *p, int n);
void func_002081C8(struct S *p);
void func_00209768(struct BLOCK *p, struct S *s);

void InitStepguard(struct S *p)
{
    int v;
    float a, b, limit, medium, high, extent;
    InitStep(p);
    EnsureStepguardManager();
    p->pF64 = PloNew__F3CIDP2SO3OIDi(0x10, 0, 0x288, -1);
    SetLoBusyk(p->pF64, 2);
    v = p->pF64->u40;
    if ((v ^ 1) & 1)
        *(int *)v &= -3;
    else
        p->pF64->u40 = v & -3;
    a = 0.785398245f; b = 1.30899704f; limit = D_002A8600; medium = 300.0f; high = 1200.0f; extent = 800.0f;
    p->fF50 = -a;
    p->fF54 = a;
    p->fF58 = -b;
    p->fF5C = b;
    p->fF48 = medium;
    p->fF4C = high;
    p->f314 = extent;
    p->f31C = medium;
    p->f2CC = D_002A8600;
    p->f318 = high;
    p->f320 = 450.0f;
    p->f324 = medium;
    p->f328 = 600.0f;
    p->f2D0 = limit;
    p->f370 = -100.0f;
    p->f374 = 350.0f;
    p->f378 = 1000.0f;
    p->f380 = -800.0f;
    p->f384 = extent;
    p->f388 = 3000.0f;
    p->fE7C = 16.0f;
    p->fE80 = 20.0f;
    p->nE84 = 50;
    p->fE6C = 5.5f;
    p->fE70 = 6.5f;
    p->n354 = 0x29FF7E;
    p->n364 = 1;
    p->n37C = 1;
    p->nE64 = 1;
    p->nF40 = 1;
    p->nE68 = -1;
    p->nE78 = 0;
    p->n350 = 0x20F800;
    p->n358 = 0xF800;
    p->f360 = -10.0f;
    p->n4A0 = 1;
    p->f260 = 1.5f;
    p->u148_f = 2;
    p->n3F0 = -200;
    p->n3D8 = -1;
    p->nF80 = -1;
    p->n3FC = -1;
    p->nFBC = -1;
    p->fFF4 = D_002A8604;
    p->n1000 = D_002C3CF0;
    func_002081C8(p);
    func_00209768(&p->block, p);
}
