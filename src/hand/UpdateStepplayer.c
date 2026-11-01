struct VTE { short d; short pad; void (*pfn)(void *, void *); };
struct O { int pad[3]; struct VTE *vt; };
struct Q { char pad[0x80]; int x80; };
struct G { char pad[0x10B4]; int x10B4; };
struct P {
    char pad[0x150]; struct Q *x150;
    char pad1[0x26C - 0x154]; int x26C; int c270; struct O *a274[1];
    char pad2[0xDF4 - 0x278]; int xDF4;
    char pad3[0xEFC - 0xDF8]; int xEFC;
    char pad4[0xFD0 - 0xF00]; int xFD0;
    char pad5[0x1084 - 0xFD4]; void *x1084;
};
extern struct G D_002DFC00;
extern char D_002DE2E0[];
extern char D_003DA0F0[];
void UpdateStep(struct P *p);
void UpdateStepplayerGadgets(struct P *p);
int GrfConsumePressed__5POCTLUif(struct Q *q, float g, int n);
int GrfPressedWithin__5POCTLUif(struct Q *q, float g, int n);
void func_00140338(struct P *p);
void UpdateJtgoal(int *pl);
int FJtgoalHold(int *pl);
void func_001E46A0(void *p, struct Q *q);
void UpdateStepplayerInteractCandidates(struct P *p);
void func_001A2460(struct P *p);
void func_00140750(struct P *p);
void func_00142AF8(struct P *p);
int FAsegaRetracted(int x);
void func_001306A8(int *px);
int func_0014FD50(void *p);
void SetJoyBtnHandled(struct G *pg, int n);
void func_001DB490(void *p);
void UpdateBhvstk(int *p);
void UpdateStepplayer(struct P *p)
{
    int *pl;
    UpdateStep(p);
    UpdateStepplayerGadgets(p);
    if (GrfConsumePressed__5POCTLUif(p->x150, 0.0f, 0x200))
        func_00140338(p);
    pl = &p->xFD0;
    if (p->xDF4 == -1) {
        UpdateJtgoal(pl);
        if (!FJtgoalHold(pl)) {
            if (p->c270 > 0) {
                struct O *po = p->a274[p->c270 - 1];
                struct VTE *e = &po->vt[14];
                e->pfn((char *)po + e->d, p->x150);
            } else if (p->x1084) {
                func_001E46A0(p->x1084, p->x150);
            }
        }
    }
    UpdateStepplayerInteractCandidates(p);
    func_001A2460(p);
    func_00140750(p);
    func_00142AF8(p);
    if (p->xEFC && FAsegaRetracted(p->xEFC))
        func_001306A8(&p->xEFC);
    if (!func_0014FD50(D_002DE2E0) && p->x150->x80 == 1 && GrfPressedWithin__5POCTLUif(p->x150, 0.0f, 0x100)) {
        struct G *pg = &D_002DFC00;
        if ((pg->x10B4 & 0x108) == 0x100) {
            SetJoyBtnHandled(pg, 0x100);
            func_001DB490(D_003DA0F0);
        }
    }
    if (!FJtgoalHold(pl))
        UpdateBhvstk(&p->x26C);
}
