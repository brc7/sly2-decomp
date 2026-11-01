typedef int qword __attribute__((mode(TI)));
typedef struct T {
    int pad[4];
    int x10, x14, x18, x1C;
    int pad20[2];
    int x28, x2C, x30, x34, x38, x3C, x40, x44;
} T;
typedef struct S { int x0; int x4; int pad[3]; T *p14; } S;
extern char D_002DE578[];
extern char D_003DD490[];
extern float GGetDifficulty(void *p);
extern int NRandInRange__Fii(int a, int b);
extern int FIsBasicDerivedFrom__FP5BASIC3CID(void *p, int k);
extern void func_001C6FD8(void *p, int k, qword q, void *pv, int n);
static inline int NLerp(float g, int a, int b)
{
    if (a == b)
        return b;
    if (a < b)
        return a + (int)(g * ((float)b + 0.999f - (float)a));
    return b + (int)((1.0f - g) * ((float)a + 0.999f - (float)b));
}
void func_00154E10(S *p, qword q, int n, int dn, void *pv)
{
    if (p->x0 <= 0) {
        int a = NLerp(GGetDifficulty(D_002DE578), p->p14->x28, p->p14->x30);
        int b = NLerp(GGetDifficulty(D_002DE578), p->p14->x2C, p->p14->x34);
        p->x0 = NRandInRange__Fii(a, b);
    }
    if (p->x4 <= 0) {
        int a = NLerp(GGetDifficulty(D_002DE578), p->p14->x38, p->p14->x40);
        int b = NLerp(GGetDifficulty(D_002DE578), p->p14->x3C, p->p14->x44);
        p->x4 = NRandInRange__Fii(a, b);
    }
    {
        int d = FIsBasicDerivedFrom__FP5BASIC3CID(pv, 0x38) ? -2 : -1;
        p->x0 += d;
        p->x4 += dn;
    }
    if (n < 0) {
        if (p->x0 <= 0)
            n = NRandInRange__Fii(p->p14->x10, p->p14->x14);
        else
            n = NRandInRange__Fii(p->p14->x18, p->p14->x1C);
    }
    if (n > 0)
        func_001C6FD8(D_003DD490, 0x5B, q, pv, n);
    if (p->x4 <= 0)
        func_001C6FD8(D_003DD490, 0x5D, q, pv, 1);
}
