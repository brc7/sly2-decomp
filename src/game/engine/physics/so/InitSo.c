typedef unsigned long ulong;
struct F { ulong lo : 35; ulong b35 : 1; ulong hi : 28; };
struct P { char pad[0x10]; struct P *parent; char pad1[0x12C - 0x14]; float g12C; char pad2[0x13C - 0x130]; float g13C; int n140; int pad3; union { struct { signed char a, b; } c; struct F f; } u; };
extern float D_002C3B90;
void InitLo(struct P *);
void SetLoBusyk(struct P *, int);
void InitDl__FP2DLi(void *, int);
/** @sly2-annot addr=0x1F2108
 * Purpose: SO vt7 Init: InitLo, SetLoBusyk(0), clear DL +0x64, dhp bytes +0x148/+0x149 = -10/-20,
 *   gAlpha(+0x12C)=1, +0x13C=D_002C3B90, inherit +0x148 bit35 from parent, +0x140=-1
 * Info: proposed name InitSo (T1); class SO slot7; TU so
 * Confidence: high [T1 0.85] - evidence: static analysis; sources: r6_1D5150+verdicts_tu_r4r6
 * See: docs/engine/guards-ai.md#8-source-files, docs/engine/physics-collision.md#6-source-files,
 *   docs/engine/physics-collision.md#7-key-functions
 * @end-sly2-annot */
void InitSo(struct P *p)
{
    InitLo(p);
    SetLoBusyk(p, 0);
    InitDl__FP2DLi((char *)p + 0x64, 0x24);
    p->u.c.a = -10;
    p->u.c.b = -20;
    p->g12C = 1.0f;
    p->g13C = D_002C3B90;
    p->u.f.b35 = p->parent ? p->parent->u.f.b35 : 1;
    p->n140 = -1;
}
