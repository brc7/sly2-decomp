typedef struct { short d; short pad; void *pfn; } VTE;
typedef struct SUB {
    char pad0[0xC];
    VTE *vt;
    char pad10[0x58 - 0x10];
    int x58;
    char pad5c[0x70 - 0x5C];
    int x70;
    int x74;
    int x78;
} SUB;
typedef struct { int x0, x4, x8; } E12;
typedef struct OBJ {
    char pad0[0xC];
    VTE *vt;
    char pad10[0x24 - 0x10];
    SUB *x24;
    char pad28[0x40 - 0x28];
    int x40;
    float x44;
    int x48;
    E12 a4C[1];
    char pad58[0x10C - 0x58];
    int x10C;
    int x110;
    int x114;
    int x118;
    int x11C;
    int x120;
    int x124;
} OBJ;
extern struct CLOCK { float a[16]; } D_002DDED0;
extern char D_003DD490[];
extern int D_003E0F9C;
void IncrementSwHandsOff(void *, int);
void DecrementSwHandsOff(void *, int);
void SetWidgetState(SUB *, int);
void SetWidgetRequest(SUB *, int);
void RefreshJobListEntries(OBJ *);
#define VCALL(o, n) ((void (*)(void *))(o)->vt[n].pfn)((char *)(o) + (o)->vt[n].d)
/** @sly2-annot addr=0x1DD300
 * Purpose: Job list open/page: -1 close, 0 jobs (10 rows), 1 tasks of the selected job (4 rows); hands-off
 *   while open
 * Info: proposed name SetJobListPage (T1); TU screen
 * Confidence: mid (draft) [T1 0.60] - evidence: static analysis; sources: r9_bhv_screen
 * See: docs/reference/functions/hud-ui.md#menu-0x1ae928-0x1b0b78,
 *   docs/reference/functions/hud-ui.md#screen-0x1d93d8-0x1de2a0
 * @end-sly2-annot */
void SetJobListPage(OBJ *p, int s)
{
    int old;
    if (!p->x24)
        s = -1;
    if (s == p->x40)
        return;
    if (p->x40 == -1) {
        IncrementSwHandsOff(D_003DD490, 0);
        VCALL(p, 10);
    }
    old = p->x40;
    p->x40 = s;
    p->x44 = D_002DDED0.a[15];
    switch (s) {
    case -1:
        DecrementSwHandsOff(D_003DD490, 0);
        VCALL(p, 11);
        if (p->x24->x58 > 0)
            SetWidgetRequest(p->x24, 1);
        p->x11C = s;
        p->x120 = s;
        p->x124 = s;
        break;
    case 0:
        p->x10C = 0xA;
        p->x24->x70 = 0x4E3;
        p->x24->x78 = 0x4E2;
        if (old == 1)
            p->x24->x74 = 0x4E8;
        else
            p->x24->x74 = 0x4E1;
        if (p->x24->x58) {
            SetWidgetState(p->x24, 1);
            SetWidgetRequest(p->x24, 2);
            VCALL(p->x24, 16);
        }
        break;
    case 1: {
        int i = p->x118;
        if (i >= 0 && i < p->x48)
            p->x110 = p->a4C[i].x0;
        else
            p->x110 = D_003E0F9C;
        p->x114 = (old == 0);
        p->x10C = 4;
        p->x24->x70 = 0x4E6;
        p->x24->x78 = 0x4E5;
        if (old == 0)
            p->x24->x74 = 0x4E7;
        else
            p->x24->x74 = 0x4E4;
        if (p->x24->x58) {
            SetWidgetState(p->x24, 1);
            SetWidgetRequest(p->x24, 2);
            VCALL(p->x24, 16);
        }
        break;
    }
    }
    RefreshJobListEntries(p);
    ((void (*)(void *, int))p->vt[14].pfn)((char *)p + p->vt[14].d, -1);
}
