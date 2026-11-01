typedef int qword __attribute__((mode(TI)));
struct M { qword a, b, c; };
struct S { char pad[0x70]; qword q70; char p1[0x90 - 0x80]; qword q90; char p2[0xF4 - 0xA0]; int f; };
struct G { char pad[0x60]; qword q60; };
extern G D_002DDF40;
extern qword D_002A2DC0;
extern "C" {
M BuildOrthonormalMatrixZ(qword a, qword b);
int func_001CF338(S *p, qword q, M m);
void func_001CF618(S *p);
}
void func_001CF618(S *p)
{
    qword v;
    M m;
    if (p->f) {
        union { qword q; float f; } s; qword k;
        __asm__("qmtc2.ni %1, %0" : "=j"(k) : "r"(-1.0f));
        s.q = k;
        __asm__("vmulx.xyzw %0, %1, %2x" : "=j"(v) : "j"(D_002DDF40.q60), "j"(k));
    } else {
        v = D_002A2DC0;
    }
    m = BuildOrthonormalMatrixZ(p->q90, v);
    func_001CF338(p, p->q70, m);
}
