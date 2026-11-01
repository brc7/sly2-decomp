typedef int qword __attribute__((mode(TI)));
typedef struct { qword a[3]; } MATRIX3;
struct VTE { short d; short pad; void *pfn; };
struct X { char pad[0x9A]; short n9A; char pad1[0xA0 - 0x9C]; void *pA0; char pad2[0xC0 - 0xA4]; qword vC0; };
struct XV { int x, y; float z; int w; };
struct T { char pad[0x60]; void *p60; char pad1[0x10BC - 0x64]; void *p10BC; };
struct S2 { char pad[0x70]; qword v70; };
struct O {
    char pad[0xC]; struct VTE *pvt; char pad1[0x58 - 0x10]; struct X *p58; struct X *p5C;
    char pad2[0x17C - 0x60]; int n17C; float g180; char pad3[0x18C - 0x184]; int n18C; struct T *p190;
};
struct CLOCK { float t; };
extern "C" {
extern struct CLOCK D_002DDED0;
extern qword D_002A2DD0;
extern qword D_002BE8F0;
extern char D_003DD490[];
void SetLoParent_1A58D0(struct O *p, int a, int b);
void func_002303A0(struct O *p, struct X *px, int n);
void SetSoCnstrForce(struct O *p, int n);
void SetSoCnstrTorque(struct O *p, int n);
void func_001F68A8(struct O *p, int n);
void UpdateSwBusyList(void *p);
void *func_001A6318(struct O *p);
void TiltMatUpright(void *pv, int n, MATRIX3 *pm);
MATRIX3 ConvertXfMat(void *pv, void *pvA0, MATRIX3 *pm);
qword func_0011BFF0(MATRIX3 *pm);
qword func_0022FC48(void *pv, void *pvA0, qword v);
void func_00183B80(struct O *p, int n);
}
static inline float GetZ(qword q) { float z; __asm__("pextuw %0, $0, %1" : "=r"(z) : "r"(q)); return z; }
static inline qword VAdd(qword a, qword b) { qword r; __asm__("vadd.xyzw %0, %1, %2" : "=j"(r) : "j"(a), "j"(b)); return r; }

void func_00183B80(struct O *p, int n)
{

    if (p->n17C == n)
        return;
    if (p->n17C == 2) {
        if (p->n18C != 0) {
            SetLoParent_1A58D0(p, 0, 0xF);
            func_002303A0(p, p->p58, 1);
            p->p58->n9A = 0;
            func_002303A0(p, p->p5C, 1);
            p->p5C->n9A = 0;
            { struct VTE *e = &p->pvt[35]; ((void (*)(void *, qword))e->pfn)((char *)p + e->d, D_002A2DD0); }
            SetSoCnstrForce(p, 0);
            SetSoCnstrTorque(p, 0);
            func_001F68A8(p, 0);
        }
        { struct VTE *e = &p->pvt[3]; ((void (*)(void *, int, int, int, int, int))e->pfn)((char *)p + e->d, 6, 0, 0, 0, 0); }
        UpdateSwBusyList(D_003DD490);
        p->p190 = 0;
    }
    if (n == 2) {
        if (p->n18C != 0) {
            struct S2 *ps;
            MATRIX3 m;
            qword q;
            SetSoCnstrForce(p, 1);
            SetSoCnstrTorque(p, 1);
            ps = (struct S2 *)p->p190->p10BC;
            if (!ps)
                ps = (struct S2 *)p->p190->p60;
            { struct VTE *e = &p->pvt[10]; ((void (*)(void *, struct T *, void *, int))e->pfn)((char *)p + e->d, p->p190, ps, 3); }
            TiltMatUpright(func_001A6318(p), 0, &m);
            m = ConvertXfMat(0, p->p5C->pA0, &m);

            q = func_0011BFF0(&m);
            func_002303A0(p, p->p5C, 7);
            {

                struct XV *pv = (struct XV *)&p->p5C->vC0;
                pv->x = 0;
                pv->z = GetZ(q);
                pv->y = 0;
            }
            p->p5C->n9A = n;

            {
            qword q2 = func_0022FC48(0, p->p58->pA0, VAdd(ps->v70, D_002BE8F0));
            func_002303A0(p, p->p58, 7);
            p->p58->vC0 = q2;
            }

            p->p58->n9A = n;
            func_001F68A8(p, 1);
        }
        { struct VTE *e = &p->pvt[3]; ((void (*)(void *, int, int, int, int, int))e->pfn)((char *)p + e->d, 5, 0, 0, 0, 0); }
    }
    p->n17C = n;
    p->g180 = D_002DDED0.t;
}
