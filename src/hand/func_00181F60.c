typedef int qword __attribute__((mode(TI)));
struct Q { char pad[0x10]; char x10[0x2C]; float g3c; float g40; float g44; char p1[8]; float g50; float g54; float g58; float g5c; struct Q *next; };
struct E { int a; int b; int c; int d; struct Q *pqFirst; void *pqLast; int ib; int x1c; float g; int oid; float g28; float g2c; };
struct S { char pad[0x1C0]; int x1c0; char p0[4]; int c; struct E ae[4]; };
struct CLOCK { float t; float dt; };
struct V { float a[4]; };
extern struct CLOCK D_002DDED0;
extern void *D_002DFB80;
extern char D_002BE888[];
extern float D_002BE898, D_002BE89C, D_002BE8A0, D_002BE8A4;
void func_001F36E0(struct S *p);
float func_001E6648(int oid);
qword func_001A62E8(void *p);
void func_001E6F60(int oid, qword q, int a, int b, struct V *pv, int c, float *pg);
void func_001E7380(int oid, qword q, int a, int b, struct V *pv, int c, float *pg, float g);
float GRandInRange__Fff(float lo, float hi);
void func_00181CB0(struct S *p, struct E *pe);
float func_0011D1C0(void *pv, float *pg, float a, float b, float c, float dt);
void func_001E6F10(int oid, struct Q *q, void *pv, float g);
void func_00181E10(struct S *p, struct Q *q);
static __inline__ void Integrate(float *px, float *pv, float a, float dt)
{
    float vOld = *pv;
    float vAvg;
    *pv = vOld + dt * a;
    vAvg = (vOld + *pv) * 0.5f;
    *px += dt * vAvg;
}
void func_00181F60(struct S *p)
{
    int i;
    struct V v;
    func_001F36E0(p);
    if (!p->x1c0) return;
    for (i = 0; i < p->c; i++) {
        struct E *pe = &p->ae[i];
        int oid = pe->oid;
        struct Q *q;
        func_001E6648(oid);
        if (!(D_002DDED0.t <= pe->g2c)) {
            float t;
            func_001E6F60(oid, func_001A62E8(D_002DFB80), 0, 0, &v, 0, &pe->g28);
            t = D_002DDED0.t;
            pe->g2c = t + GRandInRange__Fff(0.5f, 0.75f);
        } else {
            func_001E7380(oid, func_001A62E8(D_002DFB80), 0, 0, &v, 0, &pe->g28, pe->g28);
        }
        func_00181CB0(p, pe);
        for (q = pe->pqFirst; q; q = q->next) {
            float a, b;
            q->g3c = func_0011D1C0(D_002BE888, &q->g40, q->g3c, q->g40, q->g44, D_002DDED0.dt);
            func_001E6F10(oid, q, q->x10, q->g3c);
            a = D_002BE898 * q->g50 + D_002BE89C * q->g58;
            b = D_002BE8A0 * q->g54 + D_002BE8A4 * q->g5c;
            Integrate(&q->g50, &q->g58, a, D_002DDED0.dt);
            Integrate(&q->g50, &q->g5c, b, D_002DDED0.dt);
            func_00181E10(p, q);
        }
    }
}
