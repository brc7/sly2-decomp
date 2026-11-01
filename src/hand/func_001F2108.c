typedef unsigned long ulong;
struct F { ulong lo : 35; ulong b35 : 1; ulong hi : 28; };
struct P { char pad[0x10]; struct P *parent; char pad1[0x12C - 0x14]; float g12C; char pad2[0x13C - 0x130]; float g13C; int n140; int pad3; union { struct { signed char a, b; } c; struct F f; } u; };
extern float D_002C3B90;
void InitLo(struct P *);
void SetLoBusyk(struct P *, int);
void InitDl__FP2DLi(void *, int);
void func_001F2108(struct P *p)
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
