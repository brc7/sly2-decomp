typedef int qword __attribute__((mode(TI)));
struct CLOCK { float t; float dt; };
extern struct CLOCK D_002DDED0;
extern float D_002BE618[2];
struct VTE { short d; short pad; void (*pfn)(void *); };
struct B6 { qword q[6]; };
struct F { int a; int pad[3]; union { struct { int n10; float f14; } s; struct B6 b; } u; };
struct O {
    char pad[0xC]; struct VTE *vt; char pad2[0x20]; int n30; int pad3; int n38; int n3C;
    union { qword v40; float f40; } u40; struct B6 b50;
};
struct A { char pad[0x4B0]; qword v4B0; char pad2[0xF68 - 0x4C0]; int nF68; int pad3; qword vF70; char pad4[0x108C - 0xF80]; int n108C; };
struct S { char pad[0x10]; struct A *p10; int pad2; char a18[0x18]; struct F f30; float fA0; float fA4; };
int func_00177DC0(struct F *a, struct F *b);
void func_001EF0D8(void *p);
struct O *func_00205438(void *p, int n);
void func_00171258(struct O *o, int a, int b, int c);
void func_001725E0(struct O *o, int n);
float GRandInRange__Fff(float a, float b);
void func_00208540(void *p, int n);
#define VCALL(o) { struct VTE *e = &(o)->vt[8]; e->pfn((char *)(o) + e->d); }
void func_00176750(struct S *p, struct F *m) {
    int fl;
    if (func_00177DC0(m, &p->f30)) return;
    func_001EF0D8(p->a18);
    p->f30 = *m;
    p->fA0 = D_002DDED0.t;
    fl = 0;
    switch (m->a) {
    case 5:
        { struct O *o; fl = 1; o = func_00205438(p->p10, 0xA9); func_00171258(o, m->u.s.n10, 1, 0); VCALL(o) }
        break;
    case 1:
        { struct O *o; fl = 1; o = func_00205438(p->p10, 0xAA); VCALL(o) }
        break;
    case 3:
        { struct O *o = func_00205438(p->p10, 0x9D); o->n3C = 0xB; o->n38 = m->u.s.n10; VCALL(o) }
        break;
    case 4:
        { struct O *o = func_00205438(p->p10, 0xA5); o->n30 = m->u.s.n10; VCALL(o) }
        break;
    case 2:
        { struct O *o; fl = 1; o = func_00205438(p->p10, 0xAB); func_001725E0(o, m->u.s.n10); VCALL(o) }
        break;
    case 9:
        { struct O *o = func_00205438(p->p10, 0xA1); o->b50 = m->u.b; VCALL(o) }
        break;
    case 10:
        { struct O *o = func_00205438(p->p10, 0xA2); o->b50 = m->u.b; VCALL(o) }
        break;
    case 11:
        { struct O *o = func_00205438(p->p10, 0xA3); o->b50 = m->u.b; fl = 1; VCALL(o) }
        break;
    case 8:
        { struct O *o = func_00205438(p->p10, 0xAF); VCALL(o) }
        break;
    case 6:
        { struct O *o = func_00205438(p->p10, 0xB0); o->n3C = *(int *)((char *)p + 0x40); VCALL(o) }
        break;
    case 7:
        { struct O *o = func_00205438(p->p10, 0xB1); *(int *)((char *)o + 0x5C) = m->u.s.n10; VCALL(o) }
        break;
    case 12:
        { struct O *o = func_00205438(p->p10, 0xB2); VCALL(o) }
        break;
    case 13:
        { struct O *o = func_00205438(p->p10, 0xB4); o->n38 = m->u.s.n10; VCALL(o) }
        break;
    case 14:
        { struct O *o = func_00205438(p->p10, 0xB6); o->n38 = m->u.s.n10; VCALL(o) }
        break;
    case 15:
        { struct O *o = func_00205438(p->p10, 0xAE); o->n38 = p->p10->n108C; VCALL(o) }
        break;
    case 16:
        { struct O *o; fl = 1; o = func_00205438(p->p10, 0xB7); o->n30 = p->p10->nF68; o->u40.v40 = p->p10->vF70; VCALL(o) }
        break;
    case 17:
        { struct O *o = func_00205438(p->p10, 0x9E); o->n3C = 0x1A; o->n38 = m->u.s.n10; VCALL(o) }
        { float t = D_002DDED0.t; p->fA4 = t + GRandInRange__Fff(D_002BE618[0], D_002BE618[1]); }
        break;
    case 18:
        { struct O *o = func_00205438(p->p10, 0xBD); o->n38 = m->u.s.n10; o->u40.f40 = m->u.s.f14; VCALL(o) }
        break;
    case 19:
        { struct O *o = func_00205438(p->p10, 0xBF); o->u40.v40 = p->p10->v4B0; VCALL(o) }
        break;
    }
    if (fl) func_00208540(p->p10, 0);
}
