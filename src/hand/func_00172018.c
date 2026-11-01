typedef int qword __attribute__((mode(TI)));
struct CLOCK { float t; float dt; };
extern struct CLOCK D_002DDED0;
struct X { qword a; qword v; };
struct A { char pad[0xFD0]; char aFD0[4]; };
struct S { char pad[0x10]; struct A *p10; int pad2; void *p18; char pad3[0x14]; int n30; float f34; float f38; float f3C; char pad4[8]; int n48; };
void UpdateBhv(struct S *p);
int FBhvBusyAbove(struct S *p);
int func_00174FB8(void *p);
int func_00209098(void *p);
int func_00172658(struct S *p);
qword func_00172760(struct S *p);
qword func_001A62E8(void *p);
void func_002562A8(struct X *x);
void func_0017DD38(void *p, struct X *x);
int func_0017DCB8(void *p);
int func_0017DCF8(void *p);
void func_00172320(struct S *p, int n);
void func_00172560(struct S *p);
void func_00172F10(struct S *p);
float func_002458F0(float y, float x);
void func_00209818(void *p, float f);
static __inline__ float GetX(qword q) { float x; __asm__("mtc1 %1,%0" : "=f"(x) : "r"(q)); return x; }
static __inline__ float GetY(qword q) { float y; __asm__("prot3w %0,%1" : "=r"(y) : "r"(q)); return y; }
void func_00172018(struct S *p) {
    int s;
    UpdateBhv(p);
    while (!FBhvBusyAbove(p)) {
        s = p->n30;
        switch (s) {
        case 0:
            if (func_00174FB8(p->p18)) func_00172320(p, 1);
            break;
        case 1:
            if (func_00209098(p->p10)) {
                func_00172320(p, 5);
            } else if (p->n48) {
                if (func_00172658(p)) {
                    func_00172320(p, 2);
                } else {
                    qword v = func_00172760(p);
                    struct X x;
                    func_002562A8(&x);
                    x.v = v;
                    func_0017DD38(p->p18, &x);
                }
                p->n48 = 0;
            } else if (func_00172658(p) || func_0017DCB8(p->p18) || func_0017DCF8(p->p18)) {
                func_00172320(p, 2);
            }
            break;
        case 2:
            if (func_00209098(p->p10)) {
                func_00172320(p, 5);
            } else if (p->n48) {
                if (func_00172658(p)) {
                    func_00172560(p);
                } else {
                    func_00172320(p, 1);
                }
                p->n48 = 0;
            } else if (p->f38 < D_002DDED0.t || func_00174FB8(p->p18)) {
                func_00172320(p, 4);
            } else if (D_002DDED0.t > p->f3C) {
                func_00172320(p, 3);
            } else {
                qword a = func_00172760(p);
                qword b = func_001A62E8(p->p10);
                qword d;
                __asm__("vsub.xyzw %0, %1, %2" : "=j"(d) : "j"(a), "j"(b));
                func_00209818(p->p10->aFD0, func_002458F0(GetY(d), GetX(d)));
            }
            break;
        case 3:
            if (func_00209098(p->p10)) {
                func_00172320(p, 5);
            } else if (p->n48) {
                if (func_00172658(p)) {
                    func_00172320(p, 2);
                } else {
                    func_00172320(p, 1);
                }
                p->n48 = 0;
            } else if (p->f38 < D_002DDED0.t) {
                func_00172320(p, 4);
            } else {
                func_00172F10(p);
            }
            break;
        case 5:
            if (!func_00209098(p->p10)) func_00172320(p, 1);
            break;
        case 4:
            if (p->n48) {
                if (func_00172658(p)) {
                    func_00172320(p, 2);
                } else {
                    func_00172320(p, 1);
                }
                p->n48 = 0;
            } else if (func_00174FB8(p->p18)) {
                func_00172320(p, -1);
            }
            break;
        }
        if (p->n30 != s) continue;
        break;
    }
}
