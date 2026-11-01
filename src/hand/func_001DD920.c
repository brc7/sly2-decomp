typedef struct { char pad[0x10B4]; int fl; } PAD;
typedef struct {
    char pad[0x10];
    PAD *pad10;
    char pad14[0x24 - 0x14];
    void *x24;
} OBJ;
typedef struct { int ids; void (*pfn)(void); int k; int pad[5]; } ITEM;
typedef struct { char pad[0xFD50]; void *xFD50; void *xFD54; } BIG;
extern BIG D_002E65E0;
extern char D_002E6550[];
extern struct G D_003DD490;
extern char D_003D9ED0[];
extern ITEM D_003DA578[];
extern int D_002C35B8[];
extern char D_002C35F0[];
void func_001DD8E0(void);
void func_001DA808(OBJ *);
int func_00187D80(PAD *, int);
int func_00187DD0(PAD *, int);
void func_00187DE0(PAD *, int);
void func_001B04E0(int);
void func_001ACC38(void *, int);
void func_00165E38(void *);
void func_001DDB08(OBJ *);
int func_001A2C98(void);
void *func_0015E938(struct G *, int, int, int);
void func_001A2C48(int *, int **);
void *func_002333B4(void *, int, int);
void func_001AF4D8(void *, int, ITEM *, void *, void *);
/** @sly2-annot addr=0x1DD920
 * Purpose: Title screen per-frame: START continues the memcard save (SetMcUiPhase, PlayGameFromGs) or starts
 *   a new game; SELECT opens options; X opens the language list when the region has several languages
 * Info: proposed name UpdateTitleScreen (T1); TU screen
 * Confidence: mid (draft) [T1 0.60] - evidence: static analysis; sources: r9_bhv_screen
 * See: docs/reference/functions/hud-ui.md#screen-0x1d93d8-0x1de2a0
 * @end-sly2-annot */
void func_001DD920(OBJ *p)
{
    void *pv;
    int c;
    int *ak;
    int i;
    func_001DA808(p);
    if (p->x24 == 0)
        return;
    if (p->pad10 == 0)
        return;
    if (func_00187D80(p->pad10, 0x800)) {
        if ((D_002E65E0.xFD50 = D_002E65E0.xFD54) != 0) {
            func_001ACC38(D_002E6550, 1);
            func_00165E38(D_002E65E0.xFD50);
        } else {
            func_001DDB08(p);
        }
        return;
    }
    if (func_00187DD0(p->pad10, 0x100) && (p->pad10->fl & 0x108) == 0x100) {
        func_00187DE0(p->pad10, 0x100);
        func_001B04E0(0);
        return;
    }
    if (func_001A2C98() < 2)
        return;
    if (!func_00187D80(p->pad10, 0x10))
        return;
    pv = func_0015E938(&D_003DD490, 5, 0x5D6, 0);
    if (pv == 0)
        return;
    func_001A2C48(&c, &ak);
    for (i = 0; i < c; i++) {
        int k = ak[i];
        ITEM *pi = &D_003DA578[i];
        func_002333B4(pi, 0, sizeof(ITEM));
        pi->k = k;
        pi->ids = D_002C35B8[k];
        pi->pfn = func_001DD8E0;
    }
    func_001AF4D8(D_003D9ED0, c, D_003DA578, pv, D_002C35F0);
}
