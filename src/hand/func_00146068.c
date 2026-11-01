typedef int qword __attribute__((mode(TI)));
typedef union { qword q; float f[4]; } U;
struct Q { char pad[0x18]; float x18; float x1C; char pad2[0x30 - 0x20]; int x30; };
struct P { char pad[0x10]; void *p10; char pad2[0x30 - 0x14]; int x30; float x34; void *p38; };
struct CLOCK { float t; };
extern struct CLOCK D_002DDED0;
void func_001EF0C0(struct P *p, struct Q *q);
qword func_001A62E8(void *p);
float func_002458F0(float y, float x);
void UpdatePoctlPressed(struct Q *q);
static __inline__ float GetX(qword q) { float x; __asm__("mtc1 %1,%0" : "=f"(x) : "r"(q)); return x; }
static __inline__ float GetY(qword q) { float y; __asm__("prot3w %0,%1" : "=r"(y) : "r"(q)); return y; }
void func_00146068(struct P *p, struct Q *q)
{
    func_001EF0C0(p, q);
    if (p->x30 < 3) if (p->x30 >= 0) {
        qword a, b, d;
        float g;
        a = func_001A62E8(p->p38);
        b = func_001A62E8(p->p10);
        __asm__("vsub.xyzw %0, %1, %2" : "=j"(d) : "j"(a), "j"(b));
        g = 1.0f;
        if (p->x30 == 0) g = 0.1f;
        q->x1C = g;
        q->x18 = func_002458F0(GetY(d), GetX(d));
        if (p->x30 == 2 && D_002DDED0.t - p->x34 < 0.1f) {
            q->x30 |= 0x80;
            UpdatePoctlPressed(q);
        }
    }
}
