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
void SelectLanguageItem(void);
void func_001DA808(OBJ *);
int GrfHandleJoyBtn(PAD *, int);
int GrfJoyPressed(PAD *, int);
void SetJoyBtnHandled(PAD *, int);
void func_001B04E0(int);
void func_001ACC38(void *, int);
void func_00165E38(void *);
void StartNewGame(OBJ *);
int CLanguagesRegion(void);
void *PloFindSwObject__FP2SWi3OIDP2LO(struct G *, int, int, int);
void GetRegionLanguages(int *, int **);
void *func_002333B4(void *, int, int);
void func_001AF4D8(void *, int, ITEM *, void *, void *);
/** @sly2-annot addr=0x1DD920
 * Purpose: Title screen per-frame: START continues the memcard save (SetMcUiPhase, PlayGameFromGs) or starts
 *   a new game; SELECT opens options; X opens the language list when the region has several languages
 * Info: proposed name UpdateTitleScreen (T1); TU screen
 * Confidence: mid (draft) [T1 0.60] - evidence: static analysis; sources: r9_bhv_screen
 * See: docs/reference/functions/hud-ui.md#screen-0x1d93d8-0x1de2a0
 * @end-sly2-annot */
void UpdateTitleScreen(OBJ *p)
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
    if (GrfHandleJoyBtn(p->pad10, 0x800)) {
        if ((D_002E65E0.xFD50 = D_002E65E0.xFD54) != 0) {
            func_001ACC38(D_002E6550, 1);
            func_00165E38(D_002E65E0.xFD50);
        } else {
            StartNewGame(p);
        }
        return;
    }
    if (GrfJoyPressed(p->pad10, 0x100) && (p->pad10->fl & 0x108) == 0x100) {
        SetJoyBtnHandled(p->pad10, 0x100);
        func_001B04E0(0);
        return;
    }
    if (CLanguagesRegion() < 2)
        return;
    if (!GrfHandleJoyBtn(p->pad10, 0x10))
        return;
    pv = PloFindSwObject__FP2SWi3OIDP2LO(&D_003DD490, 5, 0x5D6, 0);
    if (pv == 0)
        return;
    GetRegionLanguages(&c, &ak);
    for (i = 0; i < c; i++) {
        int k = ak[i];
        ITEM *pi = &D_003DA578[i];
        func_002333B4(pi, 0, sizeof(ITEM));
        pi->k = k;
        pi->ids = D_002C35B8[k];
        pi->pfn = SelectLanguageItem;
    }
    func_001AF4D8(D_003D9ED0, c, D_003DA578, pv, D_002C35F0);
}
