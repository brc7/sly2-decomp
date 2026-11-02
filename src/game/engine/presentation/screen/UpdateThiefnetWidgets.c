typedef struct { float a[8]; } M2;
typedef struct { int x58; } SUBX;
typedef struct { char pad[0x58]; int x58; } SUB;
typedef struct { int id; int x4; } E8;
typedef struct { int a, b; int pad[6]; } REC;
typedef struct OBJ {
    char pad0[0x24];
    SUB *x24;
    char pad28[0x30 - 0x28];
    int x30;
    char pad34[0x40 - 0x34];
    int x40;
    char pad44[0x48 - 0x44];
    int x48;
    E8 ae[(0x24C - 0x4C) / 8];
    int x24C;
    int x250;
    int x254;
    int x258;
    int x25C;
    int x260;
    int x264;
} OBJ;
extern int D_003DA570;
extern float D_002C35B0;
extern int D_002C3588[];
extern REC D_002BCD00[];
void *func_002333B4(void *, int, unsigned int);
int func_001DBBA0(OBJ *);
void SetWidgetAnimXf(SUB *, int, M2 *);
void SwitchWidgetAnim(SUB *, int, int *);
void func_00213BD8(SUB *, int, int);
void func_00213A08(SUB *, int, int);
void PlayWidgetAnim(SUB *, int);
void StopWidgetAnim(SUB *, int);
/** @sly2-annot addr=0x1DBEF0
 * Purpose: Drive ThiefNet widget anims: scroll arrows (0x4DA/0x4A1, 0x4DD/0x4A4), row icons by item kind
 *   (0x456-0x458), cursor 0x437, popup 0x43B/0x439
 * Info: proposed name UpdateThiefnetWidgets (T1); TU screen
 * Confidence: mid (draft) [T1 0.50] - evidence: static analysis; sources: r9_bhv_screen
 * See: docs/reference/functions/hud-ui.md#screen-0x1d93d8-0x1de2a0
 * @end-sly2-annot */
void UpdateThiefnetWidgets(OBJ *p)
{
    M2 m;
    int end;
    int i;
    if (p->x24->x58 != 2)
        return;
    func_002333B4(&m, 0, sizeof(m));
    m.a[3] = 1.0f;
    m.a[0] = 1.0f;
    end = func_001DBBA0(p);
    if (p->x250 > 0) {
        int id = p->x40 == 0 ? 0x4DA : 0x4A1;
        if (D_003DA570) {
            SetWidgetAnimXf(p->x24, id, &m);
            m.a[5] += D_002C35B0;
        }
        SwitchWidgetAnim(p->x24, id, &p->x260);
    } else {
        SwitchWidgetAnim(p->x24, -1, &p->x260);
    }
    for (i = p->x250; i < end; i++) {
        E8 *e = &p->ae[i];
        int k = D_002C3588[i - p->x250];
        switch (p->x40) {
        case 0:
            if (e->id != -1) {
                switch (D_002BCD00[e->id].b) {
                case 7:
                    e->x4 = 0x456;
                    break;
                case 9:
                    e->x4 = 0x458;
                    break;
                case 8:
                    e->x4 = 0x457;
                    break;
                }
                func_00213BD8(p->x24, 0x455, k);
                func_00213BD8(p->x24, e->x4, k);
            } else {
                func_00213A08(p->x24, 0x455, k);
                func_00213A08(p->x24, e->x4, k);
                e->x4 = -1;
            }
            break;
        case 1:
            if (e->id != -1)
                func_00213BD8(p->x24, 0x455, k);
            else
                func_00213A08(p->x24, 0x455, k);
            break;
        }
    }
    if (p->x24C >= 0) {
        PlayWidgetAnim(p->x24, 0x437);
        func_002333B4(&m, 0, sizeof(m));
        m.a[3] = 1.0f;
        m.a[0] = 1.0f;
        m.a[5] = D_002C35B0 * (float)(p->x24C - p->x250);
        SetWidgetAnimXf(p->x24, 0x437, &m);
    } else {
        StopWidgetAnim(p->x24, 0x437);
    }
    if (end < p->x48) {
        int id = p->x40 == 0 ? 0x4DD : 0x4A4;
        if (D_003DA570) {
            SetWidgetAnimXf(p->x24, id, &m);
            m.a[5] += D_002C35B0;
        }
        SwitchWidgetAnim(p->x24, id, &p->x264);
    } else {
        SwitchWidgetAnim(p->x24, -1, &p->x264);
    }
    SwitchWidgetAnim(p->x24, (p->x30 != -1) ? 0x43B : 0x439, &p->x25C);
}
