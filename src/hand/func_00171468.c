struct CLOCK { float t; float dt; };
extern struct CLOCK D_002DDED0;
struct E { int k; int pad; float g8; };
struct O { char pad[0xF80]; int nF80; char pad2[0xFD0 - 0xF84]; char aFD0[4]; };
struct S { char pad[0x10]; struct O *p10; int pad2; void *p18; char pad3[0x1C]; struct E *pe; float g3C; int n40; int pad4; float g48; };
int FBhvpatrolpathDone(void *p);
int FBhvrailArrived(void *p);
int FBhvpipeArrived(void *p);
int FBhvgotoArrived(void *p);
int FBhvanimDone(void *p);
void SetStepguardMoverRadTarget(void *p, float g);
void PopBhv(void *p);
int func_00171468(struct S *p) {
    if (!p->n40) return 0;
    switch (p->pe->k) {
    case 6:
        switch (p->p10->nF80) {
        case -1:
            if (FBhvpatrolpathDone(p->p18)) p->n40 = 0;
            break;
        case 0:
            if (FBhvrailArrived(p->p18)) p->n40 = 0;
            break;
        case 1:
            if (FBhvpipeArrived(p->p18)) p->n40 = 0;
            break;
        }
        break;
    case 7:
        if (FBhvgotoArrived(p->p18)) {
            p->n40 = 0;
            SetStepguardMoverRadTarget(p->p10->aFD0, p->g48);
        }
        break;
    case 5:
        if (p->pe->g8 < D_002DDED0.t - p->g3C) p->n40 = 0;
        break;
    case 13: case 14:
        if (FBhvanimDone(p->p18)) p->n40 = 0;
        break;
    }
    if (!p->n40) PopBhv(&p->p18);
    return p->n40;
}
