typedef int qword __attribute__((mode(TI)));
struct M4 { qword a, b, c, d; };
struct V { float x, y, z, w; } __attribute__((aligned(16)));
struct VTE { short d; short pad; void (*pf)(); };
struct OBJ { VTE *vt; };
struct H { int x0; OBJ *pobj; };
struct Q { char pad[0x14]; void *x14; char p1[0x1C - 0x18]; float x1C; char p2[0x40 - 0x20]; float x40; };
struct R { char pad[0x30]; Q *x30; };
struct LO { char pad[0x12C]; float x12C; char p1[0x150 - 0x130]; int x150; char p2[0x168 - 0x154]; R *x168; };
extern "C" {
H *func_0012FF10(void *p, LO *plo, int a, int b);
qword func_001A62E8(LO *plo);
void *func_001A6318(LO *plo);
void func_001F3C70(LO *plo);
void func_001C5FA0(LO *p);
}
M4 LoadMatrixFromPosRotScale(qword q, void *pv, V *pvec);
void func_001A5EE8(LO *plo, M4 m, float a, float b);
void func_001C5FA0(LO *p)
{
    Q *q;
    H *ph;
    if (p->x150 == 2) return;
    if (p->x168 && (q = p->x168->x30) != 0) {
        ph = func_0012FF10(q->x14, p, 0xC, 1);
        if (ph) {
            V v;
            OBJ *pobj = ph->pobj;
            ((void (*)(void *, V *, int, float, float))pobj->vt[3].pf)((char *)pobj + pobj->vt[3].d, &v, 0, q->x1C, q->x40);
            M4 m;
            m = LoadMatrixFromPosRotScale(func_001A62E8(p), func_001A6318(p), &v);
            func_001A5EE8(p, m, p->x12C, p->x12C);
        } else {
            func_001F3C70(p);
        }
    } else {
        func_001F3C70(p);
    }
}
