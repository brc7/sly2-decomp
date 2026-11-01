typedef int qword __attribute__((mode(TI)));
static __inline__ float GetY(qword q) { float y; __asm__("prot3w %0, %1" : "=r"(y) : "r"(q)); return y; }
struct T { char pad[0x14]; int x14; };
struct P { char pad[0x10DC]; void *x10DC; char pad1[0x17FC - 0x10E0]; int x17FC; int x1800; int x1804; int x1808; char pad2[0x190C - 0x180C]; struct T *x190C; };
void func_001B6078(struct P *p, int n);
qword func_001A62E8(void *p);
qword ConvertAloVec(void *a, void *b, qword q);
void func_001B86B0(struct P *p, int k);
static __inline__ qword Sub(qword a, qword b) { qword d; __asm__("vsub.xyzw %0, %1, %2" : "=j"(d) : "j"(a), "j"(b)); return d; }
void func_001B85D0(struct P *p)
{
    int k;
    if (!p->x1804 || !p->x1808) return;
    func_001B6078(p, 0xB);
    if (p->x190C) {
        int v = p->x190C->x14;
        if (v == p->x17FC || v == p->x1804) k = 3;
        else k = 2;
    } else if (p->x10DC) {
        if (GetY(ConvertAloVec(0, p, Sub(func_001A62E8(p->x10DC), func_001A62E8(p)))) > 0.0f) k = 2;
        else k = 3;
    } else
        k = 3;
    func_001B86B0(p, k);
}
