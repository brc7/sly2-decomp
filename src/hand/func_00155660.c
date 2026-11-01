typedef int qword __attribute__((mode(TI)));
struct Y { char pad[0x30]; int x30; };
struct X { char pad[0x10]; void *p10; int x14; struct Y *p18; char pad1[0x30 - 0x1C]; int state; int x34; void *p38; };
void UpdateBhv(struct X *p);
int FBhvBusyAbove(struct X *p);
qword func_001A62E8(void *p);
void func_00155788(struct X *p, int k);
static __inline__ float GetZ(qword q) { float z; __asm__("pextuw %0, $0, %1" : "=r"(z) : "r"(q)); return z; }
void func_00155660(struct X *p)
{
    UpdateBhv(p);
    while (!FBhvBusyAbove(p)) {
        int s = p->state;
        switch (s) {
        case 0: {
            struct Y *py = p->p18;
            qword a = func_001A62E8(p->p10);
            qword b = func_001A62E8(p->p38);
            qword d, l;
            float dist; int f;
            __asm__("vsub.xyzw %0, %1, %2" : "=j"(d) : "j"(a), "j"(b));
            __asm__("vmul.xy %0, %1, %1\n\tvaddy.x %0, %0, %0y" : "=&j"(l) : "j"(d));
            __asm__("vsqrt Q, %1x\n\tvwaitq\n\tvaddq.x %0, $vf0, Q" : "=j"(l) : "j"(l));
            __asm__("qmfc2.ni %0, %1" : "=r"(dist) : "j"(l));
            f = 0;
            if (dist < 850.0f && __builtin_fabsf(GetZ(d)) < 400.0f && py->x30) f = 1;
            if (f) func_00155788(p, 1);
            break;
        }
        }
        if (p->state == s) return;
    }
}
