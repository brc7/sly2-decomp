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
int func_00140AF8(struct P *p, int n);
void func_00135248(struct P *p, int n);
int GrfConsumePressed__5POCTLUif(struct PO *po, int grf, float g);
int GrfPressedWithin__5POCTLUif(struct PO *po, int grf, float g);
void ClearPoctlJoyPressed(struct PO *po, int grf);
void func_00197500(void *pv);
void func_001911A8(void *pv);
void func_001A12F0(void *pv);
void func_0019CC50(void *pv, int a, int b, int c);
void func_001938C0(void *pv, int a, int b);
int func_00136A28(struct P *p, int n);
void func_0019E1E0(void *pv);
void func_00136118(struct P *p);
void func_001367D8(struct P *p);
void func_0021F418(void *pv);
int func_00131968(void *pv);
int FSmaRetracted(void *pv);
void func_00136338(struct P *p);

#define VCALL99(p, a) ((int (*)(void *, int))(p)->vt[99].pfn)((char *)(p) + (p)->vt[99].d, a)

void func_00134850(struct P *p)
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
            if (!(p->p150->x30 & 8) && func_00140AF8(p, 0)) {
                func_00135248(p, 5);
            } else if (GrfConsumePressed__5POCTLUif(p->p150, 0x40, 0.1f)) {
                func_00195A00(p->a2C0, 1, 0, 0);
            } else if (GrfPressedWithin__5POCTLUif(p->p150, 0x400, 0.15f) && VCALL99(p, 3)) {
                ClearPoctlJoyPressed(p->p150, 0x400);
                func_0019CC50(p->aF00, 0, 0, 0);
            } else if (GrfPressedWithin__5POCTLUif(p->p150, 0x10, 0.15f) && VCALL99(p, p->x1870)) {
                ClearPoctlJoyPressed(p->p150, 0x10);
                p->x16B4 = func_00136A28(p, p->x1870);
                func_00135248(p, 6);
            } else if (GrfConsumePressed__5POCTLUif(p->p150, 0x20, 0.2f)) {
                func_001938C0(&p->x4C0, 0xF8FE, 0);
            } else {
                float g = p->p150->g1C;
                if ((p->state == 0 && 0.1f < g) || (p->state != 0 && 0.0f < g)) {
                    if (p->p150->x30 & 8)
                        func_00135248(p, 1);
                    else
                        func_00135248(p, 7);
                } else {
                    func_00135248(p, 0);
                }
            }
            break;
        case 2:
            func_00197500(p->a2C0);
            if (GrfPressedWithin__5POCTLUif(p->p150, 0x10, 0.15f) && VCALL99(p, p->x1870)) {
                ClearPoctlJoyPressed(p->p150, 0x10);
                p->x16B4 = func_00136A28(p, p->x1870);
                func_00135248(p, 6);
            }
            break;
        case 3:
            func_001911A8(&p->x4C0);
            break;
        case 4:
            func_001A12F0(p->aDF0);
            break;
        case 5:
            if ((p->p150->x30 & 8) || !func_00140AF8(p, 1)) {
                func_00135248(p, 0);
            } else if (GrfConsumePressed__5POCTLUif(p->p150, 0x40, 0.1f)) {
                func_00195A00(p->a2C0, 1, 0, 0);
            } else if (GrfPressedWithin__5POCTLUif(p->p150, 0x400, 0.15f) && VCALL99(p, 3)) {
                ClearPoctlJoyPressed(p->p150, 0x400);
                func_0019CC50(p->aF00, 0, 0, 0);
            } else if (GrfConsumePressed__5POCTLUif(p->p150, 0x20, 0.2f)) {
                func_001938C0(&p->x4C0, 0xF8FE, 0);
            } else if (GrfPressedWithin__5POCTLUif(p->p150, 0x10, 0.15f) && VCALL99(p, p->x1870)) {
                ClearPoctlJoyPressed(p->p150, 0x10);
                p->x16B4 = func_00136A28(p, p->x1870);
                func_00135248(p, 6);
            }
            break;
        case 13:
            func_0019E1E0(p->aF50);
            break;
        case 6:
            func_00136118(p);
            break;
        case 8:
            func_001367D8(p);
            break;
        case 12:
            func_0021F418(p->a11B0);
            break;
        case 10:
            if (p->p1030 && func_00131968(p->p1030))
                func_00135248(p, 0);
            else if (p->p1034 && FSmaRetracted(p->p1034))
                func_00135248(p, 0);
            break;
        case 14:
            if (D_002BC930 < D_002DDED0.t - p->t13D4)
                func_00135248(p, 0);
            break;
        }
        func_00136338(p);
    } while (p->state != s);
}
