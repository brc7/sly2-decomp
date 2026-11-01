typedef int qword __attribute__((mode(TI)));
struct VTE { short d; short pad; void (*pfn)(); };
struct O { char pad[0xC]; struct VTE *vt; char pad2[0x40 - 0x10]; int n40; char pad3[0x64 - 0x44]; float g64; char pad4[0xCC - 0x68]; int nCC; float gD0; };
struct P { char pad[0x10]; void *o10; char pad2[0x30 - 0x14]; int n30; float g34; int n38; float g3C; qword q40; };
void StartBhv(void *p);
struct O *func_00205438(void *p, int n);
void func_00144F48(struct P *p, qword q);
void func_00144E68(struct P *p)
{
    struct O *o;
    struct VTE *e;
    StartBhv(p);
    o = func_00205438(p->o10, 0xA8);
    o->n40 = p->n30;
    o->g64 = p->g34;
    o->nCC = p->n38;
    o->gD0 = p->g3C;
    func_00144F48(p, p->q40);
    e = &o->vt[8];
    ((void (*)(void *))e->pfn)((char *)o + e->d);
}
