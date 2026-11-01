typedef struct { short d; short pad; void *pfn; } VTE;
typedef struct { char pad0[0xC]; VTE *vt; } SUB;
typedef struct { int key; int pad[7]; } E;
typedef struct { int a, b, c, xC; int pad[4]; } REC;
typedef struct OBJ {
    char pad0[0x20];
    float x20;
    char pad24[0x28 - 0x24];
    float x28;
    char pad2c[0x30 - 0x2C];
    int x30;
    void *x34;
    SUB *x38;
    int x3C;
    int x40;
    int x44;
    int x48;
    int x4C;
} OBJ;
extern E D_002C2DE8[], D_002C3028[], D_002C3148[];
extern REC D_002BCD00[];
int FIsBasicDerivedFrom__FP5BASIC3CID(void *, int);
int func_001DAEA8(int);
void func_00213B00(SUB *, int);
void func_00213928(SUB *, int);
void func_00212E40(SUB *, int, float);
#define VCALL1(o, n, a) ((void (*)(void *, int))(o)->vt[n].pfn)((char *)(o) + (o)->vt[n].d, a)
/** @sly2-annot addr=0x1D9B60
 * Purpose: Meter state switch: 0/5 hide/appear anims, 1 steady (show hp gauge; show power gauge only if the
 *   character owns a power-up with a cost), 2 damage, 6/7 alternate anims
 * Info: proposed name SetHudMeterState (T1); TU screen
 * Confidence: mid (draft) [T1 0.55] - evidence: static analysis; sources: r9_bhv_screen
 * See: docs/reference/functions/hud-ui.md#screen-0x1d93d8-0x1de2a0
 * @end-sly2-annot */
void func_001D9B60(OBJ *p, int s)
{
    int old = p->x30;
    if (old == s)
        return;
    if (!p->x38)
        return;
    if (old == 2 || old == 6)
        VCALL1(p->x38, 49, p->x3C);
    p->x30 = s;
    switch (s) {
    case 0:
    case 5:
        VCALL1(p->x38, 49, p->x44);
        break;
    case 1: {
        int n = 0;
        E *tbl = 0;
        int found;
        int i;
        VCALL1(p->x38, 49, p->x3C);
        func_00213B00(p->x38, 0x34F);
        func_00212E40(p->x38, 0x34F, 1.0f - p->x20);
        if (FIsBasicDerivedFrom__FP5BASIC3CID(p->x34, 0x35)) {
            n = 0x12;
            tbl = D_002C2DE8;
        } else if (FIsBasicDerivedFrom__FP5BASIC3CID(p->x34, 0x37)) {
            n = 9;
            tbl = D_002C3028;
        } else if (FIsBasicDerivedFrom__FP5BASIC3CID(p->x34, 0x36)) {
            n = 8;
            tbl = D_002C3148;
        }
        found = 0;
        for (i = 0; i < n; i++) {
            int id = tbl[i].key;
            if (func_001DAEA8(id)) {
                REC r = D_002BCD00[id];
                if (r.xC > 0) {
                    found = 1;
                    break;
                }
            }
        }
        if (found) {
            func_00213B00(p->x38, 0x350);
            func_00212E40(p->x38, 0x350, 1.0f - p->x28);
        } else {
            func_00213928(p->x38, 0x350);
        }
        break;
    }
    case 2:
        VCALL1(p->x38, 49, p->x40);
        break;
    case 6:
        VCALL1(p->x38, 49, p->x48);
        break;
    case 7:
        VCALL1(p->x38, 49, p->x4C);
        break;
    }
}
