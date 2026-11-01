typedef int qword __attribute__((mode(TI)));
extern qword D_002A2DD0;
extern char D_003DD490[];
struct CLOCK { float t; };
extern struct CLOCK D_002DDED0;
struct V3 { int x; int y; int z; };
struct VTE { short d; short pad; void *pfn; };
struct LO { int pad[3]; struct VTE *vt; };
struct R { char pad[0x60]; float x60; float x64; };
struct SP { char pad[0xE8]; int xE8; char pad2[0xF4 - 0xEC]; qword *xF4; };
struct X {
    int pad[3]; struct VTE *vt; char pad1[0x190 - 0x10]; struct V3 v190; char pad2[0x294 - 0x19C];
    struct SP *x294; char pad3[0x38C - 0x298]; int x38C; char pad4[0xF04 - 0x390]; float xF04;
};
struct P { char pad[0x10]; struct X *p10; char pad1[4]; char x18[0x18]; int x30; float x34; char pad2[0x44 - 0x38]; struct R *p44; };
struct O9E { int pad[3]; struct VTE *vt; char pad1[0x38 - 0x10]; void *x38; int x3C; };
struct OA7 { int pad[3]; struct VTE *vt; char pad1[0x40 - 0x10]; int x40; char pad2[0x64 - 0x44]; float x64; char pad3[0xC0 - 0x68]; int xC0; char pad4[4]; void *xC8; };
struct OAC { int pad[3]; struct VTE *vt; char padx[0x50 - 0x10]; qword x50; char pad1[0x68 - 0x60]; int x68; };
struct OA4 { int pad[3]; struct VTE *vt; char pad1[0x30 - 0x10]; int x30; char pad2[8]; struct R *x3C; };
void func_0020AC28(struct X *, int, int, qword);
void func_001F5720(struct X *, int);
void func_001F57C0(struct X *, int);
float GRandInRange__Fff(float, float);
void func_001EF0D8(void *);
void *func_00205438(struct X *, int);
void *func_001C4840(void *);
qword func_001A62E8(void *);
void *func_0014AE18(struct P *);
void func_001E6180(struct SP *);
void func_00178D90(struct OA7 *, struct SP *, int);

#define VCALL0(po, n) { struct VTE *e = &(po)->vt[n]; ((void (*)(void *))e->pfn)((char *)(po) + e->d); }

void func_0014AE38(struct P *p, int state)
{
    if (state == p->x30)
        return;
    switch (p->x30) {
    case 0:
        func_0020AC28(p->p10, 0, 0, D_002A2DD0);
        break;
    case -1:
    case 1:
        func_001F5720(p->p10, 1);
        func_001F57C0(p->p10, 1);
        break;
    case 3: {
        float t = D_002DDED0.t;
        p->p10->xF04 = t + GRandInRange__Fff(p->p44->x60, p->p44->x64);
        p->p44 = 0;
        break;
    }
    }
    func_001EF0D8(p->x18);
    p->x30 = state;
    p->x34 = D_002DDED0.t;
    switch (state) {
    case 0: {
        struct O9E *po = func_00205438(p->p10, 0x9E);
        po->x38 = func_001C4840(D_003DD490);
        VCALL0(po, 8);
        {
            struct X *px = p->p10;
            struct VTE *e = &px->vt[35];
            ((void (*)(void *, qword))e->pfn)((char *)px + e->d, D_002A2DD0);
        }
        {
            struct X *px = p->p10;
            struct VTE *e = &px->vt[36];
            ((void (*)(void *, qword))e->pfn)((char *)px + e->d, D_002A2DD0);
        }
        {
            struct V3 *pv = &p->p10->v190;
            pv->x = pv->y = pv->z = 0;
        }
        break;
    }
    case 1: {
        struct SP *psp = p->p10->x294;
        struct OA7 *po;
        psp->xE8 = 2;
        {
            qword *pq = psp->xF4;
            *pq = func_001A62E8(p->p10);
            pq = psp->xF4;
            pq[1] = func_001A62E8(func_0014AE18(p));
        }
        func_001E6180(psp);
        po = func_00205438(p->p10, 0xA7);
        func_00178D90(po, psp, 0);
        po->xC0 = 8;
        po->x40 = 1;
        po->x64 = 25.0f;
        po->xC8 = func_001C4840(D_003DD490);
        VCALL0(po, 8);
        p->p10->xF04 = D_002DDED0.t;
        func_001F5720(p->p10, 0);
        func_001F57C0(p->p10, 0);
        break;
    }
    case 2: {
        struct OAC *po = func_00205438(p->p10, 0xAC);
        po->x50 = func_001A62E8(func_0014AE18(p));
        po->x68 = 1;
        VCALL0(po, 8);
        break;
    }
    case 3: {
        struct OA4 *po = func_00205438(p->p10, 0xA4);
        po->x30 = p->p10->x38C;
        po->x3C = p->p44;
        VCALL0(po, 8);
        p->p10->xF04 = 3.40282346638528859812e+38f;
        break;
    }
    case 4: {
        struct O9E *po = func_00205438(p->p10, 0x9E);
        po->x38 = func_001C4840(D_003DD490);
        po->x3C = 1;
        VCALL0(po, 8);
        break;
    }
    case -1:
        func_001F5720(p->p10, 0);
        func_001F57C0(p->p10, 0);
        break;
    }
}
