typedef int qword __attribute__((mode(TI)));
struct VTE { short d; short pad; void (*pfn)(void *); };
struct LM { char pad[0x60]; float gMin; float gMax; };
struct ASEG { char pad[0xC]; struct VTE *vt; char pad1[0x30 - 0x10]; int x30; char pad2[4]; int x38; int x3C; char pad3[0x40 - 0x40]; int x40;
    char pad4[0xC0 - 0x44]; int xC0; char pad5[4]; int xC8; };
struct ALO { char pad[0xE8]; int xE8; char pad1[0xF4 - 0xEC]; qword *pq; };
struct SO { char pad[0x294]; struct ALO *palo; char pad1[0xF04 - 0x298]; float tF04; char pad2[0xFD0 - 0xF08]; char xFD0[4]; };
struct X {
    char pad[0x10]; struct SO *pso; int x14; int x18; char pad1[0x30 - 0x1C]; int state; float t34;
    char pad2[0x3C - 0x38]; float g3C; void *x40; float t44; float t48; int x4C; int x50; struct LM *plm;
};
struct CLOCK { float t; };
extern struct CLOCK D_002DDED0;
extern qword D_002A2DD0;
extern float D_002A5138;
void func_0020AC28(struct SO *pso, int a, int b, qword q);
float GRandInRange__Fff(float a, float b);
void func_001EF0D8(int *p);
void *func_00155A68(struct X *p);
struct ASEG *func_00205438(struct SO *pso, int id);
qword func_001A62E8(void *p);
float func_002458F0(float y, float x);
void func_00209818(void *p, float g);
void func_001E6180(struct ALO *p);
void func_00178D90(struct ASEG *a, struct ALO *b, int c);

static inline qword Sub(qword a, qword b) { qword d; __asm__("vsub.xyzw %0, %1, %2" : "=j"(d) : "j"(a), "j"(b)); return d; }
static __inline__ float GetX(qword q)
{
    float x;
    __asm__("mtc1 %1, %0" : "=f"(x) : "r"(q));
    return x;
}
static __inline__ float GetY(qword q)
{
    float y;
    __asm__("prot3w %0, %1" : "=r"(y) : "r"(q));
    return y;
}

void func_00155AE0(struct X *p, int k)
{
    if (k == p->state) return;
    switch (p->state) {
    case 1:
        func_0020AC28(p->pso, 0, 0, D_002A2DD0);
        break;
    case 3:
        {
            float t = D_002DDED0.t;
            p->pso->tF04 = t + GRandInRange__Fff(p->plm->gMin, p->plm->gMax);
        }
        p->plm = 0;
        break;
    }
    func_001EF0D8(&p->x18);
    p->state = k;
    p->t34 = D_002DDED0.t;
    switch (k) {
    case 0: {
        struct ASEG *pa;
        qword d;
        p->x40 = func_00155A68(p);
        pa = func_00205438(p->pso, 0x9D);
        pa->x3C = 0xB;
        d = Sub(func_001A62E8(p->x40), func_001A62E8(p->pso));
        {
            float g = func_002458F0(GetY(d), GetX(d)) + 3.1415927f;
            func_00209818(p->pso->xFD0, g);
        }
        pa->vt[8].pfn((char *)pa + pa->vt[8].d);
        p->g3C = D_002A5138;
        break;
    }
    case 1: {
        struct ASEG *pa;
        pa = func_00205438(p->pso, 0x9E);
        pa->x38 = p->x4C;
        pa->vt[8].pfn((char *)pa + pa->vt[8].d);
        break;
    }
    case 2: {
        struct ASEG *pa;
        struct ALO *palo;
        pa = func_00205438(p->pso, 0xA7);
        palo = p->pso->palo;
        palo->xE8 = k;
        {
            qword *pq = palo->pq;
            pq[0] = func_001A62E8(p->pso);
        }
        {
            qword *pq = palo->pq;
            pq[1] = func_001A62E8(p->x40);
        }
        func_001E6180(palo);
        func_00178D90(pa, palo, 0);
        pa->xC0 = 8;
        pa->xC8 = p->x4C;
        pa->x40 = 1;
        pa->vt[8].pfn((char *)pa + pa->vt[8].d);
        break;
    }
    case 3: {
        struct ASEG *pa;
        pa = func_00205438(p->pso, 0xA4);
        pa->x3C = (int)p->plm;
        pa->x30 = p->x4C;
        pa->vt[8].pfn((char *)pa + pa->vt[8].d);
        p->pso->tF04 = D_002A5138;
        p->x50++;
        break;
    }
    }
}
