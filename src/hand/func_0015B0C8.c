typedef int qword __attribute__((mode(TI)));
typedef union { qword q; float f[4]; } U;
struct P { char pad[0xC]; float gC; char pad1[0x38 - 0x10]; float g38; float g3C; char pad2[0x48 - 0x40]; float g48; float g4C; float g50; float g54; };
struct CLOCK { float t; };
extern struct CLOCK D_002DDED0;
void ChooseEmitVelocity__FP6EMITVXffP2LMP6VECTORiT4(void *pemitvx, float a, float b, struct P *plm, qword *pv, int n, qword *pvOut);
float GRandInRange__Fff(float a, float b);

void func_0015B0C8(struct P *p, void *pemitvx, int unused, int n, qword *ppos, qword *pv, qword *pvel, float *ptBirth, float *ptDeath)
{
    float t, g;
    ChooseEmitVelocity__FP6EMITVXffP2LMP6VECTORiT4(pemitvx, p->g38, p->gC, p, pv, n, pvel);
    t = D_002DDED0.t;
    t = t - GRandInRange__Fff(p->g50, p->g54);
    *ptBirth = t;
    *ptDeath = t + p->g3C;
    g = GRandInRange__Fff(p->g48, p->g4C);
    if (g != 0.0f) {
        U u;
        qword r;
        __asm__("qmtc2.ni %1, %0" : "=j"(u.q) : "r"(g));
        __asm__("vadda.xyzw $ACC, %1, $vf0\n\tvmaddx.xyzw %0, %2, %3x" : "=j"(r) : "j"(*ppos), "j"(*pvel), "j"(u.q));
        *ppos = r;
    }
}
