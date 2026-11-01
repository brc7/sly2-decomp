typedef int qword __attribute__((mode(TI)));
struct VTE { short d; short pad; void (*pfn)(void *); };
struct O { char pad[0xC]; struct VTE *vt; char pad1[0x38 - 0x10]; int x38; char pad2[0xC0 - 0x3C]; int xC0; int xC4; float gC8; };
struct X30 { char pad[0x150]; int x150; };
struct P {
    char pad[0x10]; void *p10; char pad1[4]; char m18[0x30 - 0x18]; struct X30 *p30; char pad2[0x60 - 0x34];
    int state; float tState; char pad3[0x80 - 0x68]; qword v80; int x90;
};
struct CLOCK { float t; };
extern struct CLOCK D_002DDED0;
void func_001EF0D8(void *p);
struct O *func_00205438(void *p, int oid);
int func_00132B10(void *p, int n);
float GRandInRange__Fff(float a, float b);
void func_001457A8(struct P *p);
void func_0019F540(void *p, qword v);
int func_00132EA0(void *p);
static __inline__ void Activate(struct P *p, int oid)
{
    struct O *po = func_00205438(p->p10, oid);
    po->x38 = p->x90;
    po->vt[8].pfn((char *)po + po->vt[8].d);
}
void func_00145B18(struct P *p, int state)
{
    if (state == p->state) return;
    func_001EF0D8(p->m18);
    p->state = state;
    p->tState = D_002DDED0.t;
    switch (state) {
    case 0: {
        struct O *po = func_00205438(p->p10, 0xA8);
        if (func_00132B10(p->p30, 0x32)) {
            po->xC0 = p->p30->x150;
            po->xC4 = 8;
            po->gC8 = GRandInRange__Fff(0.1f, 0.3f);
        }
        func_001457A8(p);
        po->vt[8].pfn((char *)po + po->vt[8].d);
        break;
    }
    case 1:
        func_0019F540(p->p10, p->v80);
        break;
    case 2:
        switch (func_00132EA0(p->p10)) {
        case 0x36: Activate(p, 0xC3); break;
        case 0x37: Activate(p, 0xC4); break;
        case 0x35: Activate(p, 0xC5); break;
        }
        break;
    }
}
