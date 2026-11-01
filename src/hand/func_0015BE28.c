typedef int qword __attribute__((mode(TI)));
typedef struct M4 { qword aq[4]; } M4;
typedef struct B7 { qword aq[7]; } B7;
struct X { int a; int b; int c; int pad; M4 m; B7 b7; M4 m2; };
struct Y { int a; int b; int c; };
void func_00159118(struct X *px);
void EmitParticles__FiP5EMITBP5EMITG(void *p, struct X *px, struct Y *py);
void func_002333B4(void *pv, int b, int cb);
void func_0015BE28(qword q0, qword q1, void *p, M4 *pm, B7 *pb7, int n1, int n2, int n3)
{
    struct X x;
    struct Y y;
    func_00159118(&x);
    func_002333B4(&y, 0, sizeof(y));
    x.m.aq[2] = q0;
    x.m.aq[3] = q1;
    x.b7 = *pb7;
    x.c = n1;
    y.b = n2;
    y.c = n3;
    x.b = 1;
    x.m2 = *pm;
    EmitParticles__FiP5EMITBP5EMITG(p, &x, &y);
}
