struct VTE { short d; short pad; int (*pfn)(void *, ...); };
struct E { int id; int state; int x; };
struct X150 { char pad[0x30]; int grf; };
struct T { int x0; int x4; int x8; int xC; char pad[0x10]; };
struct P {
    char pad[0xC]; struct VTE *vt; char pad1[0x150 - 0x10]; struct X150 *p150; char pad2[0x1180 - 0x154];
    struct E ae[3]; char pad3[0x1304 - 0x11A4]; float t1304;
};
struct CLOCK { float t; };
extern struct CLOCK D_002DDED0;
extern int D_002C3258[];
extern struct T D_002BCD00[];
int GrfConsumePressed__5POCTLUif(struct X150 *p, int mask, float g);
void *func_001FBB90(int n);
void func_001FB2F8(void *p, int n);
int MpStepplayerCur(struct P *p);
void SetGadgetSlotState(struct P *p, struct E *pe, int n);
void UpdateStepplayerGadgets(struct P *p)
{
    int i;
    int *pmask;
    if (!p->p150) return;
    i = 0;
    pmask = D_002C3258;
    do {
        int mask = *pmask;
        struct E *pe = &p->ae[i];
        int id = pe->id;
        switch (pe->state) {
        case 0:
            if (GrfConsumePressed__5POCTLUif(p->p150, mask, 0.15f)) {
                if (!p->vt[99].pfn((char *)p + p->vt[99].d, id)) {
                    func_001FB2F8(func_001FBB90(0x2E), 0);
                } else {
                    int n = MpStepplayerCur(p);
                    if (n < D_002BCD00[id].xC) {
                        func_001FB2F8(func_001FBB90(0x2E), 0);
                        p->t1304 = D_002DDED0.t;
                    } else {
                        SetGadgetSlotState(p, pe, D_002BCD00[id].x0);
                    }
                }
            }
            break;
        case 1:
            if (!(p->p150->grf & mask) || !((int (*)(void *, int))p->vt[99].pfn)((char *)p + p->vt[99].d, id))
                SetGadgetSlotState(p, pe, 0);
            else
                ((void (*)(void *, struct E *))p->vt[101].pfn)((char *)p + p->vt[101].d, pe);
            break;
        }
        pmask++;
    } while (++i < 3);
}
