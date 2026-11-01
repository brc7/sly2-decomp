typedef int qword __attribute__((mode(TI)));
struct X { char pad[0x1A8]; float g1A8; char pad2[0x1840 - 0x1AC]; int x1840; };
struct P { char pad[0x10]; struct X *px; char pad2[0x30 - 0x14]; float g30; };
struct CLOCK { float t; float dt; };
extern struct CLOCK D_002DDED0;
extern void *D_002E1E40;
extern float D_002BD374, D_002BD378;
void func_001EEF98(struct P *p);
qword func_001A62E8(void *p);
void func_002046B0(struct X *px, qword v);
float func_002458F0(float y, float x);
float RadNormalize__Ff(float g);
float func_00204740(struct X *px);
float func_0011CFE0(float g, float dg);
void func_00204660(struct X *px, float g);
static __inline__ float GetX(qword q) { float x; __asm__("mtc1 %1,%0" : "=f"(x) : "r"(q)); return x; }
static __inline__ float GetY(qword q) { float y; __asm__("prot3w %0,%1" : "=r"(y) : "r"(q)); return y; }
void func_00146F18(struct P *p)
{
    func_001EEF98(p);
    switch (p->px->x1840) {
    case 0:
        if (D_002E1E40) {
            qword a, b, d;
            a = func_001A62E8(D_002E1E40);
            b = func_001A62E8(p->px);
            __asm__("vsub.xyzw %0, %1, %2" : "=j"(d) : "j"(a), "j"(b));
            func_002046B0(p->px, d);
        }
        p->g30 = p->px->g1A8;
        break;
    case 1:
        if (D_002E1E40) {
            qword a, b, d;
            float g, gCur;
            a = func_001A62E8(D_002E1E40);
            b = func_001A62E8(p->px);
            __asm__("vsub.xyzw %0, %1, %2" : "=j"(d) : "j"(a), "j"(b));
            g = func_002458F0(GetY(d), GetX(d));
            if (__builtin_fabsf(RadNormalize__Ff(g - p->g30)) < D_002BD374) {
                gCur = func_00204740(p->px);
                { float d = RadNormalize__Ff(g - gCur); float dt = D_002DDED0.dt; float k = D_002BD378; func_00204660(p->px, gCur + func_0011CFE0(d, k * dt)); }
            }
        }
        break;
    }
}
