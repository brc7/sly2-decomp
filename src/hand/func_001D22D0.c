typedef int qword __attribute__((mode(TI)));
typedef struct { char pad[0x158]; int x158; } SO;
typedef struct { float f; int a, b; } E12;
typedef struct {
    char pad0[0x57C];
    E12 a57C[5];
    char pad5b8[0x5B8 - 0x57C - 60];
    SO *x5B8;
} OBJ;
extern char D_003DD490[];
extern float D_002A7810;
extern float D_002C20C0[3];
qword func_001A62E8(void *);
int CploFindSwObjectsByClass(void *, int, int, int, int, SO **);
SO *func_001D22D0(OBJ *p)
{
    SO *best = 0;
    SO *aso[16];
    qword pos = func_001A62E8(p);
    int n = CploFindSwObjectsByClass(D_003DD490, 0x105, 0x6C, 0, 0x10, aso);
    float fMin = D_002A7810;
    int i;
    for (i = 0; i < n; i++) {
        SO *so = aso[i];
        qword d;
        float f;
        qword q = func_001A62E8(so);
        __asm__("vsub.xyzw %0, %1, %2" : "=j"(d) : "j"(q), "j"(pos));
        __asm__("vmul.xy %0, %0, %0\n\tvaddy.x %0, %0, %0y\n\tvsqrt $Q, %0x\n\tvwaitq\n\tvaddq.x %0, $vf0, $Q" : "+j"(d));
        __asm__("qmfc2.ni %0, %1" : "=r"(f) : "j"(d));
        if (so == p->x5B8)
            f = D_002C20C0[0] + f * (D_002C20C0[1] + f * D_002C20C0[2]);
        f += p->a57C[so->x158].f;
        if (f < fMin) {
            fMin = f;
            best = so;
        }
    }
    return best;
}
