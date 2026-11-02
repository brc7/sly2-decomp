struct VTE { short d; short pad; void *pfn; };
struct PO { char pad[0x1C]; float g1C; char pad1[0x30 - 0x20]; int x30; };
struct P {
    char pad[0xC]; struct VTE *vt; char pad0[0x150 - 0x10]; struct PO *p150;
    char pad1[0x20C - 0x154]; float g20C; char pad2[0x2C0 - 0x210]; char a2C0[0x4C0 - 0x2C0];
    int x4C0; char pad3[0x4C8 - 0x4C4]; int x4C8; char pad4[0xDF0 - 0x4CC]; char aDF0[0xF00 - 0xDF0];
    char aF00[0xF50 - 0xF00]; char aF50[0x1030 - 0xF50]; void *p1030; void *p1034; char pad5[0x11B0 - 0x1038];
    char a11B0[0x13D0 - 0x11B0]; int state; float t13D4; char pad6[0x16B4 - 0x13D8]; int x16B4;
    char pad7[0x1870 - 0x16B8]; int x1870;
};
struct CLOCK { float t; };
extern struct CLOCK D_002DDED0;
extern float D_002BC930;
void func_00195A00(void *pv, int a, int b, int c);
int FStepplayerNearInteractTarget(struct P *p, int n);
void SetBentleyState(struct P *p, int n);
int GrfConsumePressed__5POCTLUif(struct PO *po, int grf, float g);
int GrfPressedWithin__5POCTLUif(struct PO *po, int grf, float g);
void ClearPoctlJoyPressed(struct PO *po, int grf);
void UpdateJtAirInput(void *pv);
void UpdateJtGrabInput(void *pv);
void func_001A12F0(void *pv);
void func_0019CC50(void *pv, int a, int b, int c);
void FJtGrabNearestCandidate(void *pv, int a, int b);
int PgadgetBentleyFromId(struct P *p, int n);
void UpdateJtActivePipe(void *pv);
void func_00136118(struct P *p);
void func_001367D8(struct P *p);
void UpdateJtthrow(void *pv);
int FAsegaRetracted(void *pv);
int FSmaRetracted(void *pv);
void UpdateBentleyAim(struct P *p);

#define VCALL99(p, a) ((int (*)(void *, int))(p)->vt[99].pfn)((char *)(p) + (p)->vt[99].d, a)

/** @sly2-annot addr=0x134850
 * Purpose: Per-frame transitions of the Bentley state (+0x13D0) driven by component states, then sub-machines
 *   0x136118/0x136338/0x1367D8
 * Info: proposed name UpdateBentleyStateMachine (T1); TU bentley
 * Notes: runtime5a #8/#31: input switch (0x40 jump, 0x400 look, 0x10 gadget, 0x20 grab) consistent with
 *   S49-S53
 * Confidence: mid (draft) [T1 0.55] - evidence: runtime (save-state RAM); sources: r2b_rest+runtime
 * See: docs/engine/player.md#7-bentley-bentleycpp-0x134648,
 *   docs/reference/functions/player.md#bentley-0x134648-0x1378d8
 * @end-sly2-annot */
void UpdateBentleyStateMachine(struct P *p)
{
    int s;
    do {
        s = p->state;
        switch (s) {
        case 2:
        case 4:
        case 6:
        case 8:
        case 9:
        case 10:
        case 11:
        case 13:
        case 14:
            break;
        case 3:
            if (p->x4C8 == 1 || p->x4C8 == 12)
                break;
            if (0.1f < D_002DDED0.t - p->g20C)
                func_00195A00(p->a2C0, 0, 0, 0);
            break;
        case 5:
        case 7:
        case 12:
        default:
            if (0.1f < D_002DDED0.t - p->g20C)
                func_00195A00(p->a2C0, 0, 0, 0);
            break;
        }
        switch (p->state) {
        case 0:
        case 1:
        case 7:
            if (!(p->p150->x30 & 8) && FStepplayerNearInteractTarget(p, 0)) {
                SetBentleyState(p, 5);
            } else if (GrfConsumePressed__5POCTLUif(p->p150, 0x40, 0.1f)) {
                func_00195A00(p->a2C0, 1, 0, 0);
            } else if (GrfPressedWithin__5POCTLUif(p->p150, 0x400, 0.15f) && VCALL99(p, 3)) {
                ClearPoctlJoyPressed(p->p150, 0x400);
                func_0019CC50(p->aF00, 0, 0, 0);
            } else if (GrfPressedWithin__5POCTLUif(p->p150, 0x10, 0.15f) && VCALL99(p, p->x1870)) {
                ClearPoctlJoyPressed(p->p150, 0x10);
                p->x16B4 = PgadgetBentleyFromId(p, p->x1870);
                SetBentleyState(p, 6);
            } else if (GrfConsumePressed__5POCTLUif(p->p150, 0x20, 0.2f)) {
                FJtGrabNearestCandidate(&p->x4C0, 0xF8FE, 0);
            } else {
                float g = p->p150->g1C;
                if ((p->state == 0 && 0.1f < g) || (p->state != 0 && 0.0f < g)) {
                    if (p->p150->x30 & 8)
                        SetBentleyState(p, 1);
                    else
                        SetBentleyState(p, 7);
                } else {
                    SetBentleyState(p, 0);
                }
            }
            break;
        case 2:
            UpdateJtAirInput(p->a2C0);
            if (GrfPressedWithin__5POCTLUif(p->p150, 0x10, 0.15f) && VCALL99(p, p->x1870)) {
                ClearPoctlJoyPressed(p->p150, 0x10);
                p->x16B4 = PgadgetBentleyFromId(p, p->x1870);
                SetBentleyState(p, 6);
            }
            break;
        case 3:
            UpdateJtGrabInput(&p->x4C0);
            break;
        case 4:
            func_001A12F0(p->aDF0);
            break;
        case 5:
            if ((p->p150->x30 & 8) || !FStepplayerNearInteractTarget(p, 1)) {
                SetBentleyState(p, 0);
            } else if (GrfConsumePressed__5POCTLUif(p->p150, 0x40, 0.1f)) {
                func_00195A00(p->a2C0, 1, 0, 0);
            } else if (GrfPressedWithin__5POCTLUif(p->p150, 0x400, 0.15f) && VCALL99(p, 3)) {
                ClearPoctlJoyPressed(p->p150, 0x400);
                func_0019CC50(p->aF00, 0, 0, 0);
            } else if (GrfConsumePressed__5POCTLUif(p->p150, 0x20, 0.2f)) {
                FJtGrabNearestCandidate(&p->x4C0, 0xF8FE, 0);
            } else if (GrfPressedWithin__5POCTLUif(p->p150, 0x10, 0.15f) && VCALL99(p, p->x1870)) {
                ClearPoctlJoyPressed(p->p150, 0x10);
                p->x16B4 = PgadgetBentleyFromId(p, p->x1870);
                SetBentleyState(p, 6);
            }
            break;
        case 13:
            UpdateJtActivePipe(p->aF50);
            break;
        case 6:
            func_00136118(p);
            break;
        case 8:
            func_001367D8(p);
            break;
        case 12:
            UpdateJtthrow(p->a11B0);
            break;
        case 10:
            if (p->p1030 && FAsegaRetracted(p->p1030))
                SetBentleyState(p, 0);
            else if (p->p1034 && FSmaRetracted(p->p1034))
                SetBentleyState(p, 0);
            break;
        case 14:
            if (D_002BC930 < D_002DDED0.t - p->t13D4)
                SetBentleyState(p, 0);
            break;
        }
        UpdateBentleyAim(p);
    } while (p->state != s);
}
