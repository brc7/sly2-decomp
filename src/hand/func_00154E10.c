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
extern float func_00154C90(void *p);
extern int func_0011D3F8(int a, int b);
extern int func_00132B10(void *p, int k);
extern void func_001C6FD8(void *p, int k, qword q, void *pv, int n);
static inline int NLerp(float g, int a, int b)
{
    if (a == b)
        return b;
    if (a < b)
        return a + (int)(g * ((float)b + 0.999f - (float)a));
    return b + (int)((1.0f - g) * ((float)a + 0.999f - (float)b));
}
/** @sly2-annot addr=0x154E10
 * Purpose: Difficulty-dependent gameplay helper (uses GGetDifficulty, random 0x11D3F8)
 * Info: TU difficulty
 * Notes: no usable identifier; purpose only
 * Confidence: low (draft) [T2 0.30] - evidence: static analysis; sources: r3_1539A8
 * See: docs/reference/functions/gameplay-objects.md#tu_1bab50-0x1bab50-0x1bc050
 * @end-sly2-annot */
void func_00154E10(S *p, qword q, int n, int dn, void *pv)
{
    if (p->x0 <= 0) {
        int a = NLerp(func_00154C90(D_002DE578), p->p14->x28, p->p14->x30);
        int b = NLerp(func_00154C90(D_002DE578), p->p14->x2C, p->p14->x34);
        p->x0 = func_0011D3F8(a, b);
    }
    if (p->x4 <= 0) {
        int a = NLerp(func_00154C90(D_002DE578), p->p14->x38, p->p14->x40);
        int b = NLerp(func_00154C90(D_002DE578), p->p14->x3C, p->p14->x44);
        p->x4 = func_0011D3F8(a, b);
    }
    {
        int d = func_00132B10(pv, 0x38) ? -2 : -1;
        p->x0 += d;
        p->x4 += dn;
    }
    if (n < 0) {
        if (p->x0 <= 0)
            n = func_0011D3F8(p->p14->x10, p->p14->x14);
        else
            n = func_0011D3F8(p->p14->x18, p->p14->x1C);
    }
    if (n > 0)
        func_001C6FD8(D_003DD490, 0x5B, q, pv, n);
    if (p->x4 <= 0)
        func_001C6FD8(D_003DD490, 0x5D, q, pv, 1);
}
