// screen.cpp -- the UI screens (SCREEN widgets: HUD meter, pause menu, notices, gadget menu, thiefnet, job list,
// title...): the shown-screen list, post-load and draw of the global screens, screen modes, the gadget menu and its
// items, the job list texts and the title texts. TU 0x1D93D8..0x1DE2A0 (98 functions with the linkonce copy at
// 0x258348, address order).
//
// Types: TU-private views for the m2c bodies (SCREEN: vtable +0xC, text ids +0x18/+0x20, widget oid +0x20 / object
// +0x24, mode +0x28 and when it was set +0x2C; the gadget menu and its 0x20-byte items). The hand-written
// per-function files keep their views in namespaces named after each function (as jt.cpp does), their C++-named
// definitions as C-linkage definitions of their literal symbols there.
// The nine global screen objects are defined here (the shown-screen list, then eight screens with inline
// constructors and the linkonce destructor func_00258348, in-charge flag 2): g++ generates the 150-instruction
// construct-and-destruct StaticInit_screen and the _GLOBAL_$D$ stub func_001DE280 (cm.cpp's g_cm pattern).
// SLY2_ALIAS: __static_initialization_and_destruction_0=StaticInit_screen
// SLY2_ALIAS: _GLOBAL_$I$*=func_001DE260
// SLY2_ALIAS: _GLOBAL_$D$*=func_001DE280

#include "sly2/basic.h"
#include "sly2/sw.h"
#include "sly2/slotheap.h"
#include "sly2/stackscope.h"

/* The global screens, as views of their bytes (their classes' constructors and virtuals have C names). */
extern "C" {
void *func_001D93D8(void *pv);      /* the shown-screen list: InitDl(pdl, 0x14) */
void *GADGETMENU_ctor(void *pv);
void *func_001DB7A8(void *pv);      /* THIEFNET constructor */
void *func_001DCC58(void *pv);      /* JOBLIST constructor */
void *func_001DD648(void *pv);      /* screen 0x52D constructor */
void *func_001DD7D8(void *pv);      /* title screen constructor */
void func_00258348(void *pv, int grfDelete);
extern char D_002D8668[], D_002D37D0[], D_002CA490[];   /* vtables of the HUD meter, pause menu and notice */
extern char D_002D8568[];   /* GADGETMENU vtable */
}
struct SCRLIST
{
    char ab[0x10];
    SCRLIST() { func_001D93D8(this); }
};
/* A screen whose base fields are set inline (pframe, psidebag, vtable). */
template <int CB> struct SCRINL
{
    int x0, x4, x8;
    void *pvt;
    char ab[CB - 0x10];
    SCRINL(void *pvtInit) { x0 = 0; x4 = 0; pvt = pvtInit; }
    ~SCRINL() { func_00258348(this, 2); }
};
/* A screen constructed by a C-named constructor. */
template <int CB> struct SCRCTOR
{
    char ab[CB];
    SCRCTOR(void *(*pfnCtor)(void *)) { pfnCtor(this); }
    ~SCRCTOR() { func_00258348(this, 2); }
};
SCRLIST D_003D9E70;
SCRINL<0x50> D_003D9E80(D_002D8668);    /* HUD meter */
SCRINL<0x1F0> D_003D9ED0(D_002D37D0);   /* pause menu */
SCRINL<0x30> D_003DA0C0(D_002CA490);    /* notice */
SCRCTOR<0x60> D_003DA0F0(GADGETMENU_ctor);
SCRCTOR<0x270> D_003DA150(func_001DB7A8);
SCRCTOR<0x110> D_003DA3C0(func_001DCC58);
SCRCTOR<0x40> D_003DA4F0(func_001DD648);
SCRCTOR<0x40> D_003DA530(func_001DD7D8);

// 0x1DE008 StaticInit_screen (the nine screens, then the six slot pools; their destruction) and its stubs.
SLY2_INSTANTIATE_SLOT_POOLS()

/* A screen as the m2c bodies read it. */
struct SCRCHILD { char pad0[0xB0]; int fB0; };
struct SCREEN
{
    char pad0[0xC];
    void *pvt;              /* +0x0C */
    char pad10[0x18 - 0x10];
    int idText;             /* +0x18 */
    char pad1C[4];
    int idTextQueued;       /* +0x20; for widget screens the widget oid */
    SCRCHILD *pchild;       /* +0x24 */
    int nMode;              /* +0x28 */
    float tMode;            /* +0x2C */
    char pad30[0x38 - 0x30];
    int n38;                /* +0x38 */
};
/* The notice screen keeps an object at +0x28 instead of a mode. */
#define PLONOTICE(pscreen) (*(LO **)&(pscreen)->nMode)
/* A gadget menu item (0x20 bytes): gadget id and flags. */
struct GMITEM
{
    int idGadget;
    char pad4[0x1C - 4];
    uint f0 : 1;
    uint fAvailable : 1;    /* +0x1C bit 1 */
    uint fAcquired : 1;     /* +0x1C bit 2 */
    uint fRest : 29;
};
struct GADGETMENU : public SCREEN
{
    char pad3C[0x40 - 0x3C];
    int nState;             /* +0x40 */
    float tState;           /* +0x44 */
    char pad48[4];
    int citem;              /* +0x4C */
    GMITEM *aitem;          /* +0x50 */
};
/* The job list's selected job (+0x6C / +0x70 text ids). */
struct JOBSEL { char pad0[0x6C]; int ids6C; int ids70; };

extern SW D_003DD490;
extern "C" {
extern CLOCK D_002DDED0;
extern DL D_003DA0E0;
extern char D_002E1E60[], D_002E6550[], D_002E65A0[];   /* letterbox, memcard dialog and status */
extern char D_002DEA00[], D_002DEA28[];   /* the two HUD packet streams (DMAS) */
extern LO *D_002E1E40, *D_002DD5BC, *D_002F7900;    /* the three heroes */
extern JOBSEL *D_003DA4D0;
extern const char D_002A7AA8[], D_002A7AB8[];   /* a glyph suffix; "%d%%" */
extern char D_003E0EA8[], D_003D4A60[];
void PostHudMeterLoad(void *pscreen);
void func_001AE928(void *pmenu);
void func_001A3020(void *pletterbox);
void func_001DA770(SCREEN *pscreen);
void PostNoticeLoad(SCREEN *pscreen);
void PostJobListLoad(SCREEN *pscreen);
void func_001ACAE8(void *pdlg);
void func_001DD690(SCREEN *pscreen);
void func_001D9878(void *pscreen);
void func_001D95B8(DL *pdl);
void SetHudMeterState(SCREEN *pscreen, int nState);
void SetNoticeState(SCREEN *pscreen, int nState);
void func_001DA7D0(SCREEN *pscreen);
void func_001DA950(SCREEN *pscreen, int n0, int n1);
SCREEN *func_001DA728(SCREEN *pscreen);
void ClearGadgetMenu(GADGETMENU *pgm);
int func_001DAE58(int idGadget);
int FPowerupAcquired(int id);
LO *PpoHero(SW *psw);
void SetGadgetMenuState(GADGETMENU *pgm, int nState);
void ClearThiefnet(SCREEN *pscreen);
void ClearJobList(SCREEN *pscreen);
int IdsFromWorld(int nWorld);
int NWorldCur();
int NCompletionPercentGs(void *pgs, void *psave);
int sprintf(char *pch, const char *pchzFormat, ...);
void strcpy1(char *pchDst, const char *pchSrc);
}
/* strcpy as a call: the m2c bodies' sources are not constants, and g++'s builtin strcpy schedules differently
   (memcard.cpp func_001ADA20). */
char *StrcpyCall(char *pchDst, const char *pchSrc) __asm__("strcpy");
void InitDl(DL *pdl, int ibDle);
int FFindDlEntry(DL *pdl, void *pv);
void RemoveDlEntry(DL *pdl, void *pv);
void PrependDlEntry(DL *pdl, void *pv);
LO *PloFindSwObject(SW *psw, int grffso, OID oid, LO *plo);
char *PchzFindSwLangstr(SW *psw, int id);
void Flush__4DMAS(void *pdmas) __asm__("Flush__4DMAS");

/** @sly2-annot addr=0x1D93D8
 * Purpose: InitDl(arg, ibDle 0x14) and return it (list constructor)
 * Info: proposed name InitScreenDl (T2); TU screen
 * Confidence: low (draft) [T2 0.30] - evidence: static analysis; sources: r6b_rest
 * See: docs/engine/hud-ui.md#4-source-files, docs/engine/save-memcard.md#4-source-files,
 *   docs/reference/functions/hud-ui.md#screen-0x1d93d8-0x1de2a0
 * @end-sly2-annot */
/* the shown-screen list's constructor (a DL of screens, link at +0x14) */
extern "C" void *func_001D93D8(void *pv)
{
    InitDl((DL *)pv, 0x14);
    return pv;
}

namespace M_func_001D9408 { extern "C" {
typedef struct DL { void *pvFirst; void *pvLast; int ibDle; } DL;
typedef struct DLI { DL *pdl; void **ppv; int ibDle; struct DLI *pdliNext; } DLI;
extern DLI *D_002DD2FC;
extern int D_002DD318;
typedef struct VT { struct { short d; short pad; void (*pfn)(void *); } a[16]; } VT;
typedef struct { char pad[0xC]; VT *vt; } LO;
extern char D_003D9E80[], D_003D9ED0[], D_002E1E60[], D_003DA0F0[], D_003DA150[], D_003DA0C0[], D_003DA3C0[];
extern char D_002E6550[], D_002E65A0[], D_003DA4F0[], D_003DA530[];
void ResetHudMeter(void *);
void func_001AE970(void *);
void func_001A3070(void *);
void ResetGadgetMenu(void *);
void ResetThiefnet(void *);
void ResetNotice(void *);
void ResetJobList(void *);
void func_001ACB38(void *);
void func_001DA7B0(void *);
static inline void InitDli(DLI *pdli, DL *pdl)
{
    pdli->pdl = pdl;
    pdli->ppv = (void **)pdl;
    pdli->ibDle = pdl->ibDle;
    pdli->pdliNext = D_002DD2FC;
    D_002DD2FC = pdli;
}
static inline void *PvNextDli(DLI *pdli)
{
    void *pv = *pdli->ppv;
    pdli->ppv = (void **)((char *)pv + pdli->ibDle);
    return pv;
}
/** @sly2-annot addr=0x1D9408
 * Purpose: Call each global HUD screen's Reset (slot-13 implementations
 *   0x1D9B30/0x1DAA48/0x1DB7F0/0x1DA320/0x1DCCD8/0x1DA7B0) and walk the active list
 * Info: proposed name ResetScreens (T2); TU screen
 * Confidence: low (draft) [T2 0.40] - evidence: static analysis; sources: r6b_rest
 * See: docs/reference/functions/hud-ui.md#screen-0x1d93d8-0x1de2a0
 * @end-sly2-annot */
void func_001D9408(DL *pdl)
{
    DLI dli;
    LO *plo;
    InitDli(&dli, pdl);
    while ((plo = (LO *)PvNextDli(&dli)) != 0)
        plo->vt->a[11].pfn((char *)plo + plo->vt->a[11].d);
    D_002DD318 = 0;
    ResetHudMeter(D_003D9E80);
    func_001AE970(D_003D9ED0);
    func_001A3070(D_002E1E60);
    ResetGadgetMenu(D_003DA0F0);
    ResetThiefnet(D_003DA150);
    ResetNotice(D_003DA0C0);
    ResetJobList(D_003DA3C0);
    func_001ACB38(D_002E6550);
    func_001DA7B0(D_002E65A0);
    func_001DA7B0(D_003DA4F0);
    func_001DA7B0(D_003DA530);
    D_002DD2FC = dli.pdliNext;
}
} } // namespace M_func_001D9408

/** @sly2-annot addr=0x1D9520
 * Purpose: Call PostLoad (slot 12) on every global HUD screen object (D_003D9E80, D_003DA0F0, D_003DA150,
 *   D_003DA0C0, ...)
 * Info: proposed name PostScreensLoad (T2); TU screen
 * Confidence: low (draft) [T2 0.40] - evidence: static analysis; sources: r6b_rest
 * See: docs/reference/functions/hud-ui.md#screen-0x1d93d8-0x1de2a0
 * @end-sly2-annot */
/* PostLoad (slot 12) of every global screen. */
extern "C" void func_001D9520()
{
    PostHudMeterLoad(&D_003D9E80);
    func_001AE928(&D_003D9ED0);
    func_001A3020(D_002E1E60);
    func_001DA770((SCREEN *)&D_003DA0F0);
    func_001DA770((SCREEN *)&D_003DA150);
    PostNoticeLoad((SCREEN *)&D_003DA0C0);
    PostJobListLoad((SCREEN *)&D_003DA3C0);
    func_001ACAE8(D_002E6550);
    func_001DA770((SCREEN *)D_002E65A0);
    func_001DD690((SCREEN *)&D_003DA4F0);
    func_001DA770((SCREEN *)&D_003DA530);
}

namespace M_func_001D95B8 { extern "C" {
struct VTE { short d; short pad; void (*pfn)(...); };
struct O { int a, b, c; struct VTE *vt; void *p10; };
struct S { struct O *pNew; int a, b; struct O *pCur; };
extern char D_002DFC00[];
/** @sly2-annot addr=0x1D95B8
 * Purpose: If the requested top screen (list head) differs from the current one, notify the old one (vt) and
 *   switch focus
 * Info: proposed name UpdateScreenFocus (T2); TU screen
 * Confidence: low (draft) [T2 0.35] - evidence: static analysis; sources: r6b_rest
 * See: docs/reference/functions/hud-ui.md#screen-0x1d93d8-0x1de2a0
 * @end-sly2-annot */
void func_001D95B8(struct S *p) {
    struct O *po = p->pNew;
    struct O *pOld;
    if (po == p->pCur) return;
    if (p->pCur) {
        p->pCur->p10 = 0;
        {
            struct O *pc = p->pCur;
            struct VTE *e = &pc->vt[9];
            ((void (*)(void *, int, struct O *))e->pfn)((char *)pc + e->d, 0, po);
        }
    }
    pOld = p->pCur;
    p->pCur = po;
    if (po) {
        struct VTE *e = &po->vt[9];
        po->p10 = D_002DFC00;
        ((void (*)(void *, int, struct O *))e->pfn)((char *)po + e->d, 1, pOld);
    }
}
} } // namespace M_func_001D95B8

namespace M_func_001D9648 { extern "C" {
extern struct CLOCK { float t; float dt; float g; } D_002DDED0;
typedef struct { char pad[0x100C]; int x100C; } PAD;
extern int D_002DFBC0;
extern int D_002DFBC4;
extern PAD D_002DFC00;
extern PAD D_002E0D00;
extern char D_003D9ED0[], D_003DA150[], D_003DA0F0[], D_002DDEC0[], D_00443740[], D_003D9E80[];
extern char D_003DA0C0[], D_003DA3C0[], D_002E6550[], D_002E65A0[], D_003DA4F0[], D_003DA530[];
void RemoveGrfusr__Fi(int);
int GrfHandleJoyBtn(PAD *, int);
void func_00187E08(PAD *, int);
int func_001B0290(int);
int FIsDlEmpty__FP2DL(void *);
void UpdatePauseMenu(void *);
void UpdateThiefnet(void *);
void UpdateGadgetMenu(void *);
void UpdateCodes(void *);
void UpdateWipe(void *);
void UpdateHudMeter(void *);
void UpdateNotice(void *);
void UpdateJobList(void *);
void CMemcardDialog_Update(void *);
void func_001ADEE8(void *);
void func_001DA808(void *);
void UpdateTitleScreen(void *);
/** @sly2-annot addr=0x1D9648
 * Purpose: Per-frame: handle the pause/menu buttons (joy D_002DFBC4 flags) and run Update (slot 7) of the HUD
 *   screens
 * Info: proposed name UpdateScreens (T2); TU screen
 * Confidence: low (draft) [T2 0.40] - evidence: static analysis; sources: r6b_rest
 * See: docs/engine/hud-ui.md#2-screens-and-menus-screencpp-0x1d93d8-menu-0x1ae928,
 *   docs/engine/hud-ui.md#5-key-functions, docs/engine/overview.md#2-the-frame-loop-exact-order
 * @end-sly2-annot */
void func_001D9648(void *pdl)
{
    PAD *pad;
    if (D_002DFBC4 & 8) {
        if (D_002DDED0.g == 0.0f || 0.1f < D_002DDED0.t / D_002DDED0.g)
            RemoveGrfusr__Fi(8);
    }
    if (D_002DFBC0 & 2)
        pad = &D_002DFC00;
    else
        pad = &D_002E0D00;
    if (GrfHandleJoyBtn(pad, 0x800)) {
        if (!func_001B0290(0))
            func_00187E08(pad, 0x800);
    } else if (D_002DFBC0 & 2) {
        int r = FIsDlEmpty__FP2DL(pdl);
        int f = 0;
        if (pad->x100C == 1 && 5.0f <= D_002DDED0.t)
            f = 1;
        if (r && f)
            func_001B0290(0);
    }
    UpdatePauseMenu(D_003D9ED0);
    UpdateThiefnet(D_003DA150);
    UpdateGadgetMenu(D_003DA0F0);
    UpdateCodes(D_002DDEC0);
    UpdateWipe(D_00443740);
    UpdateHudMeter(D_003D9E80);
    UpdateNotice(D_003DA0C0);
    UpdateJobList(D_003DA3C0);
    CMemcardDialog_Update(D_002E6550);
    func_001ADEE8(D_002E65A0);
    func_001DA808(D_003DA4F0);
    UpdateTitleScreen(D_003DA530);
}
} } // namespace M_func_001D9648

/** @sly2-annot addr=0x1D9820
 * Purpose: Called from main: run 0x1D9878 on the three main HUD screens, then flush the two HUD DMA streams
 *   (D_002DEA00/D_002DEA28)
 * Info: proposed name DrawUi (EXISTING); TU screen
 * Notes: superseded r6_1D5150 proposal: UpdateScreen? (0.3) | agent/verifier proposes DrawScreens for this
 *   existing name (see existing_name_changes.csv) | RENAME proposed -> DrawUi (existing_name_changes.csv)
 * Confidence: high [EXISTING 0.60] - evidence: arbitration; pre-existing symbol in symbol_addrs.txt; sources:
 *   r6b_rest+arbitration
 * See: docs/engine/hud-ui.md#2-screens-and-menus-screencpp-0x1d93d8-menu-0x1ae928,
 *   docs/engine/hud-ui.md#5-key-functions, docs/engine/overview.md#2-the-frame-loop-exact-order
 * @end-sly2-annot */
extern "C" void DrawUi()
{
    func_001D9878(&D_003D9E80);
    func_001D9878(&D_003DA0F0);
    func_001D9878(&D_003DA150);
    Flush__4DMAS(D_002DEA00);
    Flush__4DMAS(D_002DEA28);
}

/** @sly2-annot addr=0x1D9870
 * Purpose: slot 7 base: empty
 * Info: proposed name UpdateScreen (T2); class SCREEN; TU screen
 * Notes: agent qualifier: (base)
 * Confidence: low (draft) [T2 0.35] - evidence: static analysis; sources: r6b_rest
 * See: docs/reference/functions/hud-ui.md#screen-0x1d93d8-0x1de2a0
 * @end-sly2-annot */
/* screen slot 7 (empty) */
extern "C" void func_001D9870()
{
}

/** @sly2-annot addr=0x1D9878
 * Purpose: slot 8: empty (shared by all 12 screen vtables)
 * Info: proposed name DrawScreen (T2); class SCREEN; TU screen
 * Notes: agent qualifier: (base)
 * Confidence: low (draft) [T2 0.30] - evidence: static analysis; sources: r6b_rest
 * See: docs/engine/hud-ui.md#5-key-functions, docs/reference/functions/hud-ui.md#screen-0x1d93d8-0x1de2a0
 * @end-sly2-annot */
/* screen slot 8 (empty) */
extern "C" void func_001D9878()
{
}

/** @sly2-annot addr=0x1D9880
 * Purpose: Return whether the screen is in the shown-screen list D_003D9E70
 * Info: proposed name FScreenShown (T1); TU screen
 * Notes: top50_round (0.65): Body FFindDlEntry(&D_003D9E70,p) (m2c drops the return). ShowScreen 0x1D98B0 /
 *   HideScreen 0x1D9910 (SCREEN slots 10/11) add/remove from the same list. Callers: pause-menu page tests,
 *   memcard dialog flow
 * Confidence: mid (draft) [T1 0.65] - evidence: semantic review; sources: r6b_rest+top50_round
 * See: docs/reference/functions/hud-ui.md#screen-0x1d93d8-0x1de2a0
 * @end-sly2-annot */
extern "C" void FScreenShown(SCREEN *pscreen)
{
    FFindDlEntry((DL *)&D_003D9E70, pscreen);
}

/** @sly2-annot addr=0x1D98A8
 * Purpose: slot 9: empty
 * Info: proposed name SCREEN_vt9 (T2); class SCREEN; TU screen
 * Notes: agent qualifier: (empty)
 * Confidence: low (draft) [T2 0.25] - evidence: static analysis; sources: r6b_rest
 * See: docs/reference/functions/hud-ui.md#menu-0x1ae928-0x1b0b78,
 *   docs/reference/functions/hud-ui.md#screen-0x1d93d8-0x1de2a0
 * @end-sly2-annot */
/* screen slot 9 (empty) */
extern "C" void func_001D98A8()
{
}

/** @sly2-annot addr=0x1D98B0
 * Purpose: slot 10: move/append the screen to the active list and refocus
 * Info: proposed name ShowScreen (T2); class SCREEN; TU screen
 * Confidence: low (draft) [T2 0.45] - evidence: static analysis; sources: r6b_rest
 * See: docs/engine/hud-ui.md#open-questions, docs/open-questions.md#hud--ui,
 *   docs/reference/functions/hud-ui.md#menu-0x1ae928-0x1b0b78
 * @end-sly2-annot */
/* screen slot 10: (re)insert at the front of the shown list. */
extern "C" void func_001D98B0(SCREEN *pscreen)
{
    if (FFindDlEntry((DL *)&D_003D9E70, pscreen) != 0) {
        RemoveDlEntry((DL *)&D_003D9E70, pscreen);
    }
    PrependDlEntry((DL *)&D_003D9E70, pscreen);
    func_001D95B8((DL *)&D_003D9E70);
}

/** @sly2-annot addr=0x1D9910
 * Purpose: slot 11: remove from the active list and refocus
 * Info: proposed name HideScreen (T2); class SCREEN; TU screen
 * Confidence: low (draft) [T2 0.45] - evidence: static analysis; sources: r6b_rest
 * See: docs/reference/functions/hud-ui.md#menu-0x1ae928-0x1b0b78,
 *   docs/reference/functions/hud-ui.md#screen-0x1d93d8-0x1de2a0
 * @end-sly2-annot */
/* screen slot 11: take off the shown list. */
extern "C" void func_001D9910(SCREEN *pscreen)
{
    if (FFindDlEntry((DL *)&D_003D9E70, pscreen) != 0) {
        RemoveDlEntry((DL *)&D_003D9E70, pscreen);
        func_001D95B8((DL *)&D_003D9E70);
    }
}

namespace M_PostHudMeterLoad { extern "C" {
typedef struct VT { struct { short d; short pad; void (*pfn)(void *, int); } a[64]; } VT;
typedef struct { char pad[0xC]; VT *vt; } W;
typedef struct {
    char pad[0xDFC];
    int c;
    int *pn;
    char padE04[0x1300 - 0xE04];
    int cMax;
} Q;
typedef struct {
    char pad[0x20];
    float x20;
    float x24;
    float x28;
    float x2C;
    int x30;
    Q *q;
    W *pw;
    int ids[5];
} OBJ;
extern struct G D_003DD490;
W *PloFindSwObject__FP2SWi3OIDP2LO(struct G *, int, int, int);
Q *PpoHero(struct G *);
int FIsBasicDerivedFrom__FP5BASIC3CID(Q *, int);
int MpStepplayerCur(Q *);
void PlayWidgetAnim(W *, int);
void SetWidgetAnimPhase(W *, int, float);
/** @sly2-annot addr=0x1D9968
 * Purpose: Health/gadget-power meter (global D_003D9E80): find widget 0x36B (class 0x405), take hero hp
 *   (+0xE00/+0xDFC) and power (MpStepplayerCur/+0x1300) fractions, pick per-character anim ids (Sly
 *   0x33D-341, Murray 0x342-346, Bentley 0x347-34B), set gauge phases 0x34F/0x350
 * Info: proposed name PostHudMeterLoad (T1); TU screen
 * Confidence: mid (draft) [T1 0.60] - evidence: static analysis; sources: r9_bhv_screen
 * See: docs/engine/hud-ui.md#1-widget-hud-objects-hud-tu-0x210eb0-0x21bb88-file-name-unknown,
 *   docs/reference/functions/hud-ui.md#screen-0x1d93d8-0x1de2a0
 * @end-sly2-annot */
void PostHudMeterLoad(OBJ *p)
{
    Q *q;
    W *pw;
    p->pw = PloFindSwObject__FP2SWi3OIDP2LO(&D_003DD490, 0x405, 0x36B, 0);
    p->x30 = 0;
    p->q = 0;
    q = PpoHero(&D_003DD490);
    if (q && FIsBasicDerivedFrom__FP5BASIC3CID(q, 0x34))
        p->q = q;
    if (p->q == 0) {
        p->x20 = p->x24 = 0.0f;
        p->x28 = p->x2C = 0.0f;
        return;
    }
    p->x20 = p->x24 = (float)*p->q->pn / (float)p->q->c;
    p->x28 = p->x2C = (float)MpStepplayerCur(p->q) / (float)p->q->cMax;
    if (FIsBasicDerivedFrom__FP5BASIC3CID(p->q, 0x37)) {
            p->ids[0] = 0x347;
            p->ids[1] = 0x348;
            p->ids[2] = 0x349;
            p->ids[3] = 0x34A;
            p->ids[4] = 0x34B;
    } else if (FIsBasicDerivedFrom__FP5BASIC3CID(p->q, 0x36)) {
            p->ids[0] = 0x342;
            p->ids[1] = 0x343;
            p->ids[2] = 0x344;
            p->ids[3] = 0x345;
            p->ids[4] = 0x346;
    } else {
            p->ids[0] = 0x33D;
            p->ids[1] = 0x33E;
            p->ids[2] = 0x33F;
            p->ids[3] = 0x340;
            p->ids[4] = 0x341;
    }
    pw = p->pw;
    if (pw) {
        pw->vt->a[49].pfn((char *)pw + pw->vt->a[49].d, p->ids[0]);
        PlayWidgetAnim(p->pw, 0x34F);
        PlayWidgetAnim(p->pw, 0x350);
        SetWidgetAnimPhase(p->pw, 0x34F, 1.0f - p->x20);
        SetWidgetAnimPhase(p->pw, 0x350, 1.0f - p->x28);
    }
}
} } // namespace M_PostHudMeterLoad

/** @sly2-annot addr=0x1D9B30
 * Purpose: Reset the meter: state 0, drop widget
 * Info: proposed name ResetHudMeter (T1); TU screen
 * Confidence: mid (draft) [T1 0.55] - evidence: static analysis; sources: r9_bhv_screen
 * See: docs/reference/functions/hud-ui.md#screen-0x1d93d8-0x1de2a0
 * @end-sly2-annot */
extern "C" void ResetHudMeter(SCREEN *pscreen)
{
    SetHudMeterState(pscreen, 0);
    pscreen->n38 = 0;
}

namespace M_SetHudMeterState { extern "C" {
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
int FPowerupAcquired(int);
void PlayWidgetAnim(SUB *, int);
void StopWidgetAnim(SUB *, int);
void SetWidgetAnimPhase(SUB *, int, float);
#define VCALL1(o, n, a) ((void (*)(void *, int))(o)->vt[n].pfn)((char *)(o) + (o)->vt[n].d, a)
/** @sly2-annot addr=0x1D9B60
 * Purpose: Meter state switch: 0/5 hide/appear anims, 1 steady (show hp gauge; show power gauge only if the
 *   character owns a power-up with a cost), 2 damage, 6/7 alternate anims
 * Info: proposed name SetHudMeterState (T1); TU screen
 * Confidence: mid (draft) [T1 0.55] - evidence: static analysis; sources: r9_bhv_screen
 * See: docs/reference/functions/hud-ui.md#screen-0x1d93d8-0x1de2a0
 * @end-sly2-annot */
void SetHudMeterState(OBJ *p, int s)
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
        PlayWidgetAnim(p->x38, 0x34F);
        SetWidgetAnimPhase(p->x38, 0x34F, 1.0f - p->x20);
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
            if (FPowerupAcquired(id)) {
                REC r = D_002BCD00[id];
                if (r.xC > 0) {
                    found = 1;
                    break;
                }
            }
        }
        if (found) {
            PlayWidgetAnim(p->x38, 0x350);
            SetWidgetAnimPhase(p->x38, 0x350, 1.0f - p->x28);
        } else {
            StopWidgetAnim(p->x38, 0x350);
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
} } // namespace M_SetHudMeterState
#undef VCALL1

namespace M_UpdateHudMeter { extern "C" {
typedef struct { char pad0[0xC]; void *vt; } SUB;
typedef struct Q {
    char pad0[0xDFC];
    int xDFC;
    int *xE00;
    char pad[0x10C8 - 0xE04];
    int x10C8;
    char pad2[0x10EC - 0x10CC];
    int x10EC;
    char pad3[0x1300 - 0x10F0];
    int x1300;
} Q;
typedef struct OBJ {
    char pad0[0x20];
    float x20;
    float x24;
    float x28;
    float x2C;
    int x30;
    Q *x34;
    SUB *x38;
    int x3C;
    int x40;
    int x44;
    int x48;
    int x4C;
} OBJ;
extern struct G D_003DD490;
extern struct { float t; float dt; } D_002DDED0;
extern char D_002C3248[];
Q *PpoHero(struct G *);
int FIsBasicDerivedFrom__FP5BASIC3CID(void *, int);
void SetHudMeterState(OBJ *, int);
int MpStepplayerCur(Q *);
int FFloatsNear__Ffff(float, float, float);
float GSmooth__FfffP3SMPPf(void *, int, float, float, float);
int FWidgetAnimPlaying(SUB *, int);
void SetWidgetAnimPhase(SUB *, int, float);
/** @sly2-annot addr=0x1D9DF8
 * Purpose: Meter per-frame (vtable 0x2D8668 slot 7): follow the hero, animate hp drain/fill (state 3) and
 *   power change (state 4) with GSmooth, damage flash (2), special anims when +0x10EC==4 or +0x10C8!=-1,
 *   empty at 0 hp
 * Info: proposed name UpdateHudMeter (T1); TU screen
 * Confidence: mid (draft) [T1 0.60] - evidence: static analysis; sources: r9_bhv_screen
 * See: docs/reference/functions/hud-ui.md#screen-0x1d93d8-0x1de2a0
 * @end-sly2-annot */
void UpdateHudMeter(OBJ *p)
{
    Q *old;
    Q *q;
    if (!p->x38)
        return;
    old = p->x34;
    p->x34 = 0;
    q = PpoHero(&D_003DD490);
    if (q && FIsBasicDerivedFrom__FP5BASIC3CID(q, 0x34))
        p->x34 = q;
    if (!p->x34) {
        p->x20 = p->x24 = 0;
        p->x28 = p->x2C = 0;
        if (old)
            SetHudMeterState(p, 0);
        return;
    }
    if (old != p->x34) {
        if (!old)
            SetHudMeterState(p, 3);
        else
            SetHudMeterState(p, 5);
        if (FIsBasicDerivedFrom__FP5BASIC3CID(p->x34, 0x35)) {
            p->x3C = 0x33D; p->x40 = 0x33E; p->x44 = 0x33F; p->x48 = 0x340; p->x4C = 0x341;
        } else if (FIsBasicDerivedFrom__FP5BASIC3CID(p->x34, 0x37)) {
            p->x3C = 0x347; p->x40 = 0x348; p->x44 = 0x349; p->x48 = 0x34A; p->x4C = 0x34B;
        } else if (FIsBasicDerivedFrom__FP5BASIC3CID(p->x34, 0x36)) {
            p->x3C = 0x342; p->x40 = 0x343; p->x44 = 0x344; p->x48 = 0x345; p->x4C = 0x346;
        }
        p->x20 = p->x24 = (float)*p->x34->xE00 / (float)p->x34->xDFC;
        p->x28 = p->x2C = (float)MpStepplayerCur(p->x34) / (float)p->x34->x1300;
    }
    switch (p->x30) {
    case 1:
    case 6:
    case 7:
        p->x24 = (float)*p->x34->xE00 / (float)p->x34->xDFC;
        p->x2C = (float)MpStepplayerCur(p->x34) / (float)p->x34->x1300;
        if (*p->x34->xE00 <= 0) {
            SetHudMeterState(p, 0);
        } else if (p->x20 > p->x24) {
            SetHudMeterState(p, 2);
        } else if (p->x20 < p->x24) {
            SetHudMeterState(p, 3);
        } else if (!FFloatsNear__Ffff(p->x28, p->x2C, 0.0001f)) {
            SetHudMeterState(p, 4);
        } else if (p->x34->x10EC == 4) {
            SetHudMeterState(p, 7);
        } else if (p->x34->x10C8 != -1) {
            SetHudMeterState(p, 6);
        } else {
            SetHudMeterState(p, 1);
        }
        break;
    case 2:
        if (!FWidgetAnimPlaying(p->x38, p->x40))
            SetHudMeterState(p, 3);
        break;
    case 5:
        if (!FWidgetAnimPlaying(p->x38, p->x44))
            SetHudMeterState(p, 1);
        break;
    case 0:
        p->x24 = (float)*p->x34->xE00 / (float)p->x34->xDFC;
        p->x2C = (float)MpStepplayerCur(p->x34) / (float)p->x34->x1300;
        if (p->x20 < p->x24) {
            p->x20 = p->x24;
            SetWidgetAnimPhase(p->x38, 0x34F, 1.0f - p->x20);
            p->x28 = p->x2C;
            SetWidgetAnimPhase(p->x38, 0x350, 1.0f - p->x28);
            SetHudMeterState(p, 1);
        }
        break;
    case 3:
        p->x24 = (float)*p->x34->xE00 / (float)p->x34->xDFC;
        if (p->x20 < p->x24)
            p->x20 = p->x24;
        else
            p->x20 = GSmooth__FfffP3SMPPf(D_002C3248, 0, p->x20, p->x24, D_002DDED0.dt);
        SetWidgetAnimPhase(p->x38, 0x34F, 1.0f - p->x20);
        if (FFloatsNear__Ffff(p->x20, p->x24, 0.0001f))
            SetHudMeterState(p, 1);
        break;
    case 4:
        p->x2C = (float)MpStepplayerCur(p->x34) / (float)p->x34->x1300;
        if (p->x28 < p->x2C)
            p->x28 = p->x2C;
        else
            p->x28 = GSmooth__FfffP3SMPPf(D_002C3248, 0, p->x28, p->x2C, D_002DDED0.dt);
        SetWidgetAnimPhase(p->x38, 0x350, 1.0f - p->x28);
        if (FFloatsNear__Ffff(p->x28, p->x2C, 0.0001f))
            SetHudMeterState(p, 1);
        break;
    }
}
} } // namespace M_UpdateHudMeter

/** @sly2-annot addr=0x1DA2E0
 * Purpose: Notice banner (global D_003DA0C0): find its widget 0x371
 * Info: proposed name PostNoticeLoad (T1); TU screen
 * Confidence: mid (draft) [T1 0.50] - evidence: static analysis; sources: r9_bhv_screen
 * See: docs/reference/functions/hud-ui.md#screen-0x1d93d8-0x1de2a0
 * @end-sly2-annot */
extern "C" void PostNoticeLoad(SCREEN *pscreen)
{
    PLONOTICE(pscreen) = PloFindSwObject(&D_003DD490, 0x405, (OID)0x371, 0);
}

/** @sly2-annot addr=0x1DA320
 * Purpose: Notice banner reset: no current/queued text, drop widget, state 0
 * Info: proposed name ResetNotice (T1); TU screen
 * Confidence: mid (draft) [T1 0.50] - evidence: static analysis; sources: r9_bhv_screen
 * See: docs/reference/functions/hud-ui.md#screen-0x1d93d8-0x1de2a0
 * @end-sly2-annot */
extern "C" void ResetNotice(SCREEN *pscreen)
{
    pscreen->idText = -1;
    pscreen->idTextQueued = -1;
    PLONOTICE(pscreen) = 0;
    SetNoticeState(pscreen, 0);
}

namespace M_UpdateNotice { extern "C" {
extern struct CLOCK { float t; float dt; } D_002DDED0;
typedef struct { char pad[0x58]; int x58; } W;
typedef struct {
    char pad[0x10];
    int state;
    float t;
    int x18;
    float x1C;
    int x20;
    float x24;
    W *pw;
} OBJ;
void SetNoticeState(OBJ *, int);
int FWidgetVisible(W *);
int FWidgetAnimPlaying(W *, int);
/** @sly2-annot addr=0x1DA350
 * Purpose: Notice banner per-frame: showing -> shown when widget state 2; time out (only while widget hidden)
 *   or swap to the queued text (anim 0x312); hide
 * Info: proposed name UpdateNotice (T1); TU screen
 * Confidence: mid (draft) [T1 0.50] - evidence: static analysis; sources: r9_bhv_screen
 * See: docs/reference/functions/hud-ui.md#screen-0x1d93d8-0x1de2a0
 * @end-sly2-annot */
void UpdateNotice(OBJ *p)
{
    if (p->pw == 0)
        return;
    switch (p->state) {
    case 0:
        break;
    case 1:
        if (p->pw->x58 == 2)
            SetNoticeState(p, 2);
        break;
    case 2:
        if (p->x20 != -1) {
            SetNoticeState(p, 4);
        } else if (p->x1C == 0.0f) {
            SetNoticeState(p, 3);
        } else if (0.0f < p->x1C) {
            if (p->pw && !FWidgetVisible(p->pw))
                p->t += D_002DDED0.dt;
        }
        break;
    case 3:
        if (p->pw->x58 < 2)
            SetNoticeState(p, 0);
        break;
    case 4:
        if (FWidgetAnimPlaying(p->pw, 0x312))
            break;
        if (p->x20 == -1)
            SetNoticeState(p, 0);
        else
            SetNoticeState(p, 2);
        break;
    }
}
} } // namespace M_UpdateNotice

namespace M_PostNotice { extern "C" {
typedef struct {
    char pad[0x10];
    int x10;
    int pad14;
    int x18;
    float x1C;
    int x20;
    float x24;
} OBJ;
void CancelNotice(OBJ *, int);
void SetNoticeState(OBJ *, int);
/** @sly2-annot addr=0x1DA4B8
 * Purpose: Request a notice text n for duration g (queue behind the current one); n=-1 cancels
 * Info: proposed name PostNotice (T1); TU screen
 * Confidence: mid (draft) [T1 0.50] - evidence: static analysis; sources: r9_bhv_screen
 * See: docs/reference/functions/game-progression.md#game-0x1613a8-0x166270,
 *   docs/reference/functions/hud-ui.md#screen-0x1d93d8-0x1de2a0
 * @end-sly2-annot */
void PostNotice(OBJ *p, int n, float g)
{
    if (n == -1) {
        CancelNotice(p, -1);
        return;
    }
    switch (p->x10) {
    case 1:
        SetNoticeState(p, -1);
    case 0:
    case 3:
        p->x18 = n;
        p->x1C = g;
        SetNoticeState(p, 1);
        break;
    case 2:
        if (p->x18 == n) {
            p->x1C = g;
            break;
        }
    case 4:
        p->x20 = n;
        p->x24 = g;
        break;
    }
}
} } // namespace M_PostNotice

namespace M_CancelNotice { extern "C" {
struct S { char pad[0x18]; int n18; int n1C; int n20; };
/** @sly2-annot addr=0x1DA578
 * Purpose: Cancel notice n (current -> timeout 0, or drop the queued one); -1 cancels the queue
 * Info: proposed name CancelNotice (T1); TU screen
 * Confidence: mid (draft) [T1 0.50] - evidence: static analysis; sources: r9_bhv_screen
 * See: docs/reference/functions/hud-ui.md#screen-0x1d93d8-0x1de2a0
 * @end-sly2-annot */
void CancelNotice(struct S *p, int n) {
    if (n == -1) {
        p->n20 = -1;
        p->n1C = 0;
    } else if (n == p->n18) {
        p->n1C = 0;
    } else if (n == p->n20) {
        p->n20 = -1;
    }
}
} } // namespace M_CancelNotice

/** @sly2-annot addr=0x1DA5B0
 * Purpose: Text id being shown or queued
 * Info: proposed name IdsCurNotice (T2); TU screen
 * Confidence: low (draft) [T2 0.45] - evidence: static analysis; sources: r9_bhv_screen
 * See: docs/reference/functions/hud-ui.md#screen-0x1d93d8-0x1de2a0
 * @end-sly2-annot */
/* The text id queued, else the one shown. */
extern "C" int func_001DA5B0(SCREEN *pscreen)
{
    int ids = pscreen->idTextQueued;
    if (ids == -1) {
        ids = pscreen->idText;
    }
    return ids;
}

namespace M_SetNoticeState { extern "C" {
extern struct CLOCK { float t; float dt; } D_002DDED0;
typedef struct VT { struct { short d; short pad; void (*pfn)(void *); } a[64]; } VT;
typedef struct { char pad[0xC]; VT *vt; } LO;
typedef struct {
    char pad[0x10];
    int state;
    float t;
    int x18;
    float x1C;
    int x20;
    float x24;
    LO *x28;
} OBJ;
void StartWidgetMainAnim(LO *, int);
void ShowWidgetArgs(LO *, int, int *);
/** @sly2-annot addr=0x1DA5C8
 * Purpose: Notice state switch: 1 ShowWidgetArgs(text id), 3 hide (widget vt50), 4 swap anim 0x312, 2 main
 *   anim 0x310 after a swap
 * Info: proposed name SetNoticeState (T1); TU screen
 * Confidence: mid (draft) [T1 0.50] - evidence: static analysis; sources: r9_bhv_screen
 * See: docs/reference/functions/hud-ui.md#screen-0x1d93d8-0x1de2a0
 * @end-sly2-annot */
void SetNoticeState(OBJ *p, int n)
{
    LO *plo;
    if (n == p->state)
        return;
    switch (p->state) {
    case 3:
        if (n == 1 && p->t == D_002DDED0.t)
            n = 2;
        break;
    case 4:
        p->x18 = p->x20;
        p->x1C = p->x24;
        p->x20 = -1;
        p->x24 = -1.0f;
        break;
    }
    if (n == 0)
        p->x18 = -1;
    plo = p->x28;
    if (plo) {
        if (p->state == 4 && n == 2)
            StartWidgetMainAnim(plo, 0x310);
        switch (n) {
        case 4:
            StartWidgetMainAnim(p->x28, 0x312);
            break;
        case 3:
            p->x28->vt->a[50].pfn((char *)p->x28 + p->x28->vt->a[50].d);
            break;
        case 1:
            ShowWidgetArgs(p->x28, 1, &p->x18);
            break;
        }
    }
    p->state = n;
    p->t = D_002DDED0.t;
}
} } // namespace M_SetNoticeState

namespace M_func_001DA728 { extern "C" {
extern char D_002D85E8[];
struct S { int a, b, c; void *vt; char pad[0x10]; int n20; };
void func_001DA7D0(struct S *p);
/** @sly2-annot addr=0x1DA728
 * Purpose: Base screen constructor: vtable 0x2D85E8, widget oid(+0x20)=-1, reset
 * Info: proposed name SCREEN_ctor (T2); class SCREEN; TU screen
 * Notes: agent qualifier: (ctor)
 * Confidence: low (draft) [T2 0.40] - evidence: static analysis; sources: r6b_rest
 * See: docs/reference/functions/hud-ui.md#screen-0x1d93d8-0x1de2a0,
 *   docs/reference/functions/save-memcard.md#memcard-0x1aa978-0x1ae150
 * @end-sly2-annot */
struct S *func_001DA728(struct S *p) {
    p->a = 0;
    p->b = 0;
    p->vt = D_002D85E8;
    p->n20 = -1;
    func_001DA7D0(p);
    return p;
}
} } // namespace M_func_001DA728

/** @sly2-annot addr=0x1DA770
 * Purpose: slot 12: resolve widget oid +0x20 to object +0x24 (PloFindSwObject kind 5)
 * Info: proposed name PostScreenLoad (T2); class SCREEN; TU screen
 * Confidence: low (draft) [T2 0.45] - evidence: static analysis; sources: r6b_rest
 * See: docs/reference/functions/hud-ui.md#screen-0x1d93d8-0x1de2a0,
 *   docs/reference/functions/save-memcard.md#memcard-0x1aa978-0x1ae150
 * @end-sly2-annot */
/* screen slot 12: resolve the widget. */
extern "C" void func_001DA770(SCREEN *pscreen)
{
    pscreen->pchild = (SCRCHILD *)PloFindSwObject(&D_003DD490, 5, (OID)pscreen->idTextQueued, 0);
}

/** @sly2-annot addr=0x1DA7B0
 * Purpose: slot 13: drop widget pointer and reset mode
 * Info: proposed name ResetScreen (T2); class SCREEN; TU screen
 * Confidence: low (draft) [T2 0.45] - evidence: static analysis; sources: r6b_rest
 * See: docs/reference/functions/hud-ui.md#screen-0x1d93d8-0x1de2a0,
 *   docs/reference/functions/save-memcard.md#memcard-0x1aa978-0x1ae150
 * @end-sly2-annot */
/* screen slot 13. */
extern "C" void func_001DA7B0(SCREEN *pscreen)
{
    pscreen->pchild = 0;
    func_001DA7D0(pscreen);
}

/** @sly2-annot addr=0x1DA7D0
 * Purpose: state(+0x28)=-1, t=now, SetScreenMode(-1,-1)
 * Info: proposed name ResetScreenMode (T2); TU screen
 * Confidence: low (draft) [T2 0.35] - evidence: static analysis; sources: r6b_rest
 * See: docs/reference/functions/hud-ui.md#screen-0x1d93d8-0x1de2a0,
 *   docs/reference/functions/save-memcard.md#memcard-0x1aa978-0x1ae150
 * @end-sly2-annot */
extern "C" void func_001DA7D0(SCREEN *pscreen)
{
    pscreen->nMode = -1;
    pscreen->tMode = D_002DDED0.tReal;
    func_001DA950(pscreen, -1, -1);
}

namespace M_func_001DA808 { extern "C" {
extern struct CLOCK { float a[16]; } D_002DDED0;
struct S { char p0[0x34]; float g34; };
void func_001D9870(struct S *p);
void func_001DA950(struct S *p, int a, int b);
/** @sly2-annot addr=0x1DA808
 * Purpose: Base update then close when the mode timer (+0x34) expires
 * Info: proposed name UpdateScreenTimeout (T2); TU screen
 * Confidence: low (draft) [T2 0.35] - evidence: static analysis; sources: r6b_rest
 * See: docs/reference/functions/hud-ui.md#screen-0x1d93d8-0x1de2a0,
 *   docs/reference/functions/save-memcard.md#memcard-0x1aa978-0x1ae150
 * @end-sly2-annot */
void func_001DA808(struct S *p) {
    func_001D9870(p);
    if (p->g34 <= D_002DDED0.a[15]) func_001DA950(p, -1, -1);
}
} } // namespace M_func_001DA808

namespace M_UpdateScreenStickDir { extern "C" {
typedef struct OBJ OBJ;
typedef struct { short d; short pad; void (*pfn)(void *, int); } VTE;
typedef struct { char pad[0x103C]; float x; float y; float g; } Q;
struct OBJ {
    char pad[0xC];
    VTE *vt;
    Q *q;
    char pad14[0x28 - 0x14];
    int n;
};
float atan2f(float, float);
/** @sly2-annot addr=0x1DA858
 * Purpose: Turn the left stick into an 8-way direction: call screen vt14(dir) when deflection >0.8, vt14(-1)
 *   when back under 0.5
 * Info: proposed name UpdateScreenStickDir (T1); TU screen
 * Notes: names_v2 C hint not used: LoadTnFromBrx__FP2TNP18CBinaryInputStream
 * Confidence: mid (draft) [T1 0.55] - evidence: static naming pass; sources: r9_bhv_screen
 * See: docs/reference/functions/hud-ui.md#screen-0x1d93d8-0x1de2a0
 * @end-sly2-annot */
void UpdateScreenStickDir(OBJ *p)
{
    Q *q = p->q;
    int n;
    float g;
    if (q == 0)
        return;
    n = p->n;
    g = q->g;
    if (n == -1) {
        if (0.8f < g) {
            int k = (int)(atan2f(q->y, q->x) * 1.2732395f + 8.5f);
            p->vt[14].pfn((char *)p + p->vt[14].d, k % 8);
        }
    } else if (g < 0.5f) {
        p->vt[14].pfn((char *)p + p->vt[14].d, -1);
    }
}
} } // namespace M_UpdateScreenStickDir

namespace M_func_001DA950 { extern "C" {
extern struct CLOCK { float a[16]; } D_002DDED0;
extern float D_002A7984, D_002C3254;
struct S { char p0[0x30]; int n30; float g34; int n38; };
void *PsndStartSound(int n);
void SetSndSourceLo(void *p, int n);
/** @sly2-annot addr=0x1DA950
 * Purpose: Set mode n (+0x30) and arg (+0x38); mode 0 plays sound 0x29 and sets the timeout
 * Info: proposed name SetScreenMode (T2); TU screen
 * Confidence: low (draft) [T2 0.35] - evidence: static analysis; sources: r6b_rest
 * See: docs/reference/functions/hud-ui.md#screen-0x1d93d8-0x1de2a0
 * @end-sly2-annot */
void func_001DA950(struct S *p, int n, int m) {
    p->n38 = m;
    p->n30 = n;
    switch (n) {
    case -1:
        p->g34 = -10.0f;
        break;
    case 0:
        p->g34 = D_002A7984;
        SetSndSourceLo(PsndStartSound(0x29), 0);
        break;
    case 1:
    default:
        p->g34 = D_002DDED0.a[15] + D_002C3254;
        SetSndSourceLo(PsndStartSound(0x36), 0);
        break;
    }
}
} } // namespace M_func_001DA950

/** @sly2-annot addr=0x1DA9E8
 * Purpose: SetScreenMode(-1,-1)
 * Info: proposed name CloseScreen (T2); TU screen
 * Confidence: low (draft) [T2 0.30] - evidence: static analysis; sources: r6b_rest
 * See: docs/reference/functions/hud-ui.md#screen-0x1d93d8-0x1de2a0
 * @end-sly2-annot */
extern "C" void func_001DA9E8(SCREEN *pscreen)
{
    func_001DA950(pscreen, -1, -1);
}

/** @sly2-annot addr=0x1DAA08
 * Purpose: Gadget menu constructor (vtable 0x2D8568) for D_003DA0F0
 * Info: proposed name GADGETMENU_ctor (T1); TU screen
 * Confidence: mid (draft) [T1 0.55] - evidence: static analysis; sources: r9_bhv_screen
 * See: docs/reference/functions/hud-ui.md#screen-0x1d93d8-0x1de2a0
 * @end-sly2-annot */
extern "C" void *GADGETMENU_ctor(void *pv)
{
    GADGETMENU *pgm = (GADGETMENU *)pv;
    func_001DA728(pgm);
    pgm->pvt = D_002D8568;
    ClearGadgetMenu(pgm);
    return pgm;
}

/** @sly2-annot addr=0x1DAA48
 * Purpose: Gadget menu reset (slot 13)
 * Info: proposed name ResetGadgetMenu (T1); TU screen
 * Confidence: mid (draft) [T1 0.55] - evidence: static analysis; sources: r9_bhv_screen
 * See: docs/reference/functions/hud-ui.md#screen-0x1d93d8-0x1de2a0
 * @end-sly2-annot */
extern "C" void ResetGadgetMenu(GADGETMENU *pgm)
{
    func_001DA7B0(pgm);
    ClearGadgetMenu(pgm);
}

/** @sly2-annot addr=0x1DAA78
 * Purpose: Gadget menu: state -1, timestamp
 * Info: proposed name ClearGadgetMenu (T1); TU screen
 * Confidence: mid (draft) [T1 0.50] - evidence: static analysis; sources: r9_bhv_screen
 * See: docs/reference/functions/hud-ui.md#screen-0x1d93d8-0x1de2a0
 * @end-sly2-annot */
extern "C" void ClearGadgetMenu(GADGETMENU *pgm)
{
    pgm->nState = -1;
    pgm->tState = D_002DDED0.tReal;
}

namespace M_UpdateGadgetMenu { extern "C" {
typedef struct { char pad[0x1C]; int fl; } R;
typedef struct {
    char pad[0x10];
    void *x10;
    char pad14[0x24 - 0x14];
    void *x24;
    char pad28[0x30 - 0x28];
    int x30;
    char pad34[0x40 - 0x34];
    int x40;
    char pad44[4];
    int x48;
} OBJ;
extern struct G D_003DD490;
extern int D_002C3258[];
void func_001DA808(OBJ *);
int GrfHandleJoyBtn(void *, int);
void *PpoHero(struct G *);
int FIsBasicDerivedFrom__FP5BASIC3CID(void *, int);
void SetGadgetMenuState(OBJ *, int);
void UpdateScreenStickDir(OBJ *);
R *func_001DAE20(OBJ *, int);
void func_001DA950(OBJ *, int, int);
void *PsndStartSound(int);
void SetSndSourceLo(void *, int);
void SetStepplayerGadget(void *, int, int);
/** @sly2-annot addr=0x1DAA90
 * Purpose: Gadget menu per-frame (slot 7): circle closes, info button shows a message, the 3 slot buttons
 *   (D_002C3258 = 4,1,2) assign the selected power-up to that slot (SetStepplayerGadget) when owned, else
 *   error message 0x32/0x33
 * Info: proposed name UpdateGadgetMenu (T1); TU screen
 * Notes: names_v2 C hint not used: CalculateTnCrv__FP2TNP6VECTORN21
 * Confidence: mid (draft) [T1 0.60] - evidence: static naming pass; sources: r9_bhv_screen
 * See: docs/reference/functions/hud-ui.md#screen-0x1d93d8-0x1de2a0
 * @end-sly2-annot */
void UpdateGadgetMenu(OBJ *p)
{
    void *pv;
    R *r;
    int i;
    func_001DA808(p);
    if (p->x24 == 0)
        return;
    if (p->x40 == -1)
        return;
    if (p->x10 == 0)
        return;
    if (p->x30 && GrfHandleJoyBtn(p->x10, 0x10)) {
        SetGadgetMenuState(p, -1);
        return;
    }
    pv = PpoHero(&D_003DD490);
    if (pv == 0 || !FIsBasicDerivedFrom__FP5BASIC3CID(pv, 0x34)) {
        SetGadgetMenuState(p, -1);
        return;
    }
    UpdateScreenStickDir(p);
    if (GrfHandleJoyBtn(p->x10, 0x7E8)) {
        r = func_001DAE20(p, p->x48);
        if (r == 0 || ((r->fl & 1) == 0))
            func_001DA950(p, 1, 0xE2);
        else
            func_001DA950(p, 1, 0x31);
    }
    for (i = 0; i < 3; i++) {
        if (!GrfHandleJoyBtn(p->x10, D_002C3258[i]))
            continue;
        r = func_001DAE20(p, p->x48);
        if (r == 0 || !(r->fl & 4)) {
            func_001DA950(p, 1, 0x32);
        } else if (((r->fl & 1) == 0)) {
            func_001DA950(p, 1, 0x33);
        } else {
            SetSndSourceLo(PsndStartSound(0x34), 0);
            SetStepplayerGadget(pv, i, p->x48);
        }
    }
}
} } // namespace M_UpdateGadgetMenu

namespace M_func_001DAC58 { extern "C" {
typedef struct { float x, y, dx, dy; char pad[0x10]; } TXT;
typedef struct { char ab[0xA0]; } FONT;
typedef struct { int n; int a; int b; } SLOT;
typedef struct { char pad[0x1180]; SLOT aslot[3]; } Q;
typedef struct { int n; char pad[0x10]; float x; float y; char pad1c[4]; } ENT;
typedef struct {
    char pad[0x24];
    void *x24;
    char pad28[0x4C - 0x28];
    int c;
    ENT *ae;
} OBJ;
extern struct G D_003DD490;
extern float D_002C3264;
extern int D_002C3268[];
int FWidgetVisible(void *);
Q *PpoHero(struct G *);
int FIsBasicDerivedFrom__FP5BASIC3CID(void *, int);
void CTextBox_SetPos(TXT *, float, float);
void CTextBox_SetSize(TXT *, float, float);
void CTextBox_SetTextColor(TXT *, unsigned int *);
void CTextBox_SetHorizontalJust(TXT *, int);
void CTextBox_SetVerticalJust(TXT *, int);
FONT *CRichText_ctor(FONT *, int, float);
void func_002163B0(FONT *, TXT *, int);
static inline float Zero(void){float z;__asm__ volatile("mtc1 $zero,%0":"=f"(z));return z;}
/** @sly2-annot addr=0x1DAC58
 * Purpose: While the gadget menu widget is visible draw the button glyph (&2L / &2l / &2r) next to each
 *   power-up equipped in a slot
 * Info: proposed name DrawGadgetMenuSlotLabels (T2); TU screen
 * Confidence: low (draft) [T2 0.45] - evidence: static analysis; sources: r9_bhv_screen
 * See: docs/reference/functions/hud-ui.md#screen-0x1d93d8-0x1de2a0
 * @end-sly2-annot */
void func_001DAC58(OBJ *p)
{
    Q *q;
    int i;
    if (p->x24 == 0)
        return;
    if (!FWidgetVisible(p->x24))
        return;
    q = PpoHero(&D_003DD490);
    if (q == 0)
        return;
    if (!FIsBasicDerivedFrom__FP5BASIC3CID(q, 0x34))
        return;
    for (i = 0; i < p->c; i++) {
        ENT *pe = &p->ae[i];
        int j;
        for (j = 0; j < 3; j++) {
            if (pe->n == q->aslot[j].n) {
                TXT txt;
                unsigned int rgba[2];
                FONT font;
                {float z=Zero();CTextBox_SetPos(&txt,z,z);}
                CTextBox_SetSize(&txt, 640.0f, 448.0f * 1.1f);
                rgba[0] = 0xFF808080;
                CTextBox_SetTextColor(&txt, rgba);
                CTextBox_SetHorizontalJust(&txt, 0);
                CTextBox_SetVerticalJust(&txt, 0);
                CRichText_ctor(&font, D_002C3268[j], D_002C3264);
                {float x=pe->x;float y=pe->y;txt.y=y;txt.x=x;}
                txt.dx = 32.0f;
                txt.dy = 32.0f;
                func_002163B0(&font, &txt, 0x13);
                break;
            }
        }
    }
}
} } // namespace M_func_001DAC58

namespace M_func_001DAE20 { extern "C" {
struct E { int id; int a[7]; };
struct S { char pad[0x4C]; int c; struct E *ae; };
/** @sly2-annot addr=0x1DAE20
 * Purpose: Find an entry (0x20 bytes) by id in the screen's item list (+0x4C/+0x50)
 * Info: proposed name PentryFindScreen (T2); TU screen
 * Confidence: low (draft) [T2 0.30] - evidence: static analysis; sources: r6b_rest
 * See: docs/reference/functions/hud-ui.md#screen-0x1d93d8-0x1de2a0
 * @end-sly2-annot */
struct E *func_001DAE20(struct S *p, int id) {
    int i;
    for (i = 0; i < p->c; i++) {
        struct E *pe = &p->ae[i];
        if (pe->id == id) return pe;
    }
    return 0;
}
} } // namespace M_func_001DAE20

namespace M_func_001DAE58 { extern "C" {
struct E { int a[4]; int n; int b[3]; };
extern struct E D_002BCD00[];
int NEpisodeBest(void);
/** @sly2-annot addr=0x1DAE58
 * Purpose: Item i of table D_002BCD00 is available when progress (0x210B28) >= its threshold
 * Info: proposed name FScreenItemUnlocked (T2); TU screen
 * Notes: runtime7b V03: returns NEpisodeBest() >= gadget[i].x10 (unlock episode); S71 list matches exactly
 * Confidence: low (draft) [T2 0.35] - evidence: runtime (save-state RAM); sources: r6b_rest+runtime
 * See: docs/reference/functions/hud-ui.md#screen-0x1d93d8-0x1de2a0
 * @end-sly2-annot */
int func_001DAE58(int i) {
    struct E *pe = &D_002BCD00[i];
    if (pe->n == -1) return 0;
    return NEpisodeBest() >= pe->n;
}
} } // namespace M_func_001DAE58

namespace M_FPowerupAcquired { extern "C" {
typedef struct CRef { int tagk; int tag; } CRef;
extern char D_002A79A0[], D_002A79A8[], D_002A79B8[];
void sprintf(char *pch, char *fmt, int n);
void RefGetSaveVarV(CRef *pref, char *a, char *b, char *c, int d);
void func_00129658(CRef *pref, CRef *prefSrc);
void Decref__4CRef(CRef *pref);
/** @sly2-annot addr=0x1DAEA8
 * Purpose: Save var powerUps.mppukfAcquired[n] (has the power-up been bought)
 * Info: proposed name FPowerupAcquired (T1); TU screen
 * Confidence: mid (draft) [T1 0.75] - evidence: static analysis; sources: r9_bhv_screen
 * See: docs/engine/hud-ui.md#5-key-functions, docs/reference/functions/hud-ui.md#screen-0x1d93d8-0x1de2a0,
 *   docs/reference/functions/splice-scripting.md#bif-0x11f440-0x1258f0
 * @end-sly2-annot */
int FPowerupAcquired(int n) {
    CRef ref;
    char ach[0x20];
    CRef tmp;
    int r;
    if (n == -1) return 0;
    ref.tagk = -1;
    sprintf(ach, D_002A79A0, n);
    RefGetSaveVarV(&tmp, D_002A79A8, D_002A79B8, ach, 0);
    func_00129658(&ref, &tmp);
    Decref__4CRef(&tmp);
    r = ref.tag;
    Decref__4CRef(&ref);
    return r;
}
} } // namespace M_FPowerupAcquired

namespace M_SetPowerupAcquired {
struct CRef {
    int m_tagk;
    int m_tag;
    CRef() { m_tagk = -1; }
    CRef(bool f) { m_tagk = 0x17; m_tag = f; }
    CRef(const CRef &);
    ~CRef() { Decref(); }
    CRef &operator=(const CRef &) __asm__("__as__4CRefRC4CRef");
    void Decref() __asm__("Decref__4CRef");
};
struct G { char pad[0xC]; int n; };
extern "C" { extern G D_002DEB30; }
extern "C" { extern char D_002A79A0[]; }
extern "C" { extern char D_002A79A8[]; }
extern "C" { extern char D_002A79B8[]; }
extern "C" { int sprintf(char *, const char *, ...); }
extern "C" { CRef RefSetSaveVarV(const CRef &, const char *, ...); }
extern "C" { void BroadcastGameEvent(G *, int, int); }
/** @sly2-annot addr=0x1DAF38
 * Purpose: Set powerUps.mppukfAcquired[n]; when set broadcast game event 0x3B with n
 * Info: proposed name SetPowerupAcquired (T1); TU screen
 * Confidence: mid (draft) [T1 0.75] - evidence: static analysis; sources: r9_bhv_screen
 * See: docs/engine/hud-ui.md#5-key-functions, docs/engine/main-loop-startup.md#8-key-functions,
 *   docs/reference/functions/hud-ui.md#screen-0x1d93d8-0x1de2a0
 * @end-sly2-annot */
extern "C" void SetPowerupAcquired(int n, int f)
{
    CRef ref;
    char buf[0x20];
    sprintf(buf, D_002A79A0, n);
    ref = RefSetSaveVarV(CRef(f != 0), D_002A79A8, D_002A79B8, buf, 0);
    if (f) {
        D_002DEB30.n = n;
        BroadcastGameEvent(&D_002DEB30, 0x3B, 0);
    }
}
} // namespace M_SetPowerupAcquired

/** @sly2-annot addr=0x1DB010
 * Purpose: Per item: flag 2 = unlocked by episode (FScreenItemUnlocked), flag 4 = acquired
 * Info: proposed name RefreshGadgetMenuItems (T1); TU screen
 * Confidence: mid (draft) [T1 0.60] - evidence: static analysis; sources: r9_bhv_screen
 * See: docs/reference/functions/hud-ui.md#screen-0x1d93d8-0x1de2a0
 * @end-sly2-annot */
extern "C" void RefreshGadgetMenuItems(GADGETMENU *pgm)
{
    int i = 0;
    if (pgm->citem > 0) {
        do {
            GMITEM *pitem = &pgm->aitem[i++];
            pitem->fAvailable = func_001DAE58(pitem->idGadget);
            pitem->fAcquired = FPowerupAcquired(pitem->idGadget);
        } while (i < pgm->citem);
    }
}

namespace M_SetGadgetMenuStickDir {
// Change a selector's input mode and, when the joystick points somewhere, move the selection to the
// enabled item that lies closest along that direction from the current one.
typedef int qword __attribute__((mode(TI)));
union U { qword q; float f[4]; };

struct ITEM {
    int id;
    char pad04[0x10];
    float x, y;     // 0x14, 0x18
    int flags;      // 0x1C, bit 2 = selectable
};
struct INPUT { char pad[0x103C]; float x, y; };
struct SELECTOR {
    char pad00[0x10];
    INPUT *input;   // 0x10
    char pad14[0x14];
    int mode;       // 0x28
    float tChanged; // 0x2C
    char pad30[0x18];
    int selected;   // 0x48
    int count;      // 0x4C
    ITEM *items;    // 0x50
};
struct PROX { float along, distance; };
struct HIT { qword position, other, normal; char pad30[0x10]; float fraction; char pad44[0x1C]; };

// D_002DDF0C is read as a field of the clock struct: a struct field at a fixed address may alias
// sel->mode (a scalar global may not), which keeps the clock load after the mode store.
struct CLOCK { char pad[0x3C]; float t3C; };
extern "C" { extern CLOCK D_002DDED0; }
extern "C" { extern float D_002C3280, D_002C3274, D_002C3278, D_002C327C, D_002A79C8; }

extern "C" {
ITEM *func_001DAE20(SELECTOR *sel, int id);
int func_0014C748(qword point, qword start, qword end, int flags, HIT *hit, float radius);
void FindClosestPointOnLineSegment(qword point, qword start, qword end, qword *unused, float *along, float *distance);
void SelectGadgetMenuItem(SELECTOR *sel, int id);
void func_001DA9E8(SELECTOR *sel);
void *PsndStartSound(int id);
void SetSndSourceLo(void *sound, int flags);
void SetGadgetMenuStickDir(SELECTOR *sel, int mode);
}

static inline qword XYZ(float x, float y, float z, U &home)
{
    qword r, t;
    __asm__("mfc1 %0,%2\n\tmfc1 %1,%3\n\tpextlw %0,%1,%0\n\tmfc1 %1,%4\n\tpcpyld %0,%1,%0"
            : "=&r"(r), "=&r"(t) : "f"(x), "f"(y), "f"(z));
    home.q = r;
    return home.q;
}
static inline qword ToVF(qword v) { qword r; __asm__("qmtc2.ni %1,%0" : "=j"(r) : "r"(v)); return r; }
static inline qword Zero() { qword r; __asm__("vsub.xyzw %0,$vf0,$vf0" : "=j"(r)); return r; }
static inline qword Normalize(qword a)
{
    qword d, one, r, l;
    __asm__("vmul.xyz %0,%2,%2\n\tvaddw.x %1,$vf0,$vf0w\n\tvadday.x ACC,%0,%0y\n\tvmaddz.x %0,%1,%0z"
            : "=j"(d), "=&j"(one) : "j"(a));
    __asm__("vsqrt Q,%1x\n\tvwaitq\n\tvaddq.x %0,$vf0,Q" : "=j"(l) : "j"(d));
    __asm__("vdiv Q,$vf0w,%2x\n\tvwaitq\n\tvmulq.xyzw %0,%1,Q" : "=j"(r) : "j"(a), "j"(l));
    return r;
}
// "rV": the float stays a memory operand until reload, which loads it through the dying %hi register.
static inline qword Splat(const float &f, U &home)
{
    qword s;
    __asm__("qmtc2.ni %1,%0" : "=j"(s) : "rV"(*(const int *)&f));
    home.q = s;
    return home.q;
}
static inline qword Scale(qword a, qword s) { qword r; __asm__("vmulx.xyzw %0,%1,%2x" : "=j"(r) : "j"(a), "j"(s)); return r; }
// b by const reference: the ray lives in a stack slot, so the two Adds are not CSEd and each
// reloads it into the register the result goes to.
static inline qword Add(qword a, const qword &b) { qword r; __asm__("vadd.xyzw %0,%1,%2" : "=j"(r) : "j"(a), "j"(b)); return r; }

/** @sly2-annot addr=0x1DB0C8
 * Purpose: Gadget menu slot 14: move the selection to the selectable item lying closest along the stick
 *   direction
 * Info: proposed name SetGadgetMenuStickDir (T1); TU screen
 * Confidence: mid (draft) [T1 0.60] - evidence: static analysis; sources: r9_bhv_screen
 * See: docs/reference/functions/hud-ui.md#screen-0x1d93d8-0x1de2a0
 * @end-sly2-annot */
void SetGadgetMenuStickDir(SELECTOR *sel, int mode)
{
    U home;
    HIT hit;
    PROX proximity;
    if (mode == sel->mode)
        return;
    sel->mode = mode;
    sel->tChanged = D_002DDED0.t3C;
    if (mode == -1 || sel->selected == -1)
        return;
    ITEM *current = func_001DAE20(sel, sel->selected);
    qword position = XYZ(current->x, current->y * D_002C3280, 0.0f, home);
    qword direction;
    if (sel->input)
        direction = ToVF(XYZ(sel->input->x, -sel->input->y, 0.0f, home));
    else
        direction = Zero();
    qword ray = Scale(Normalize(direction), Splat(D_002C3274, home));
    int bestId = -1;
    float best = D_002A79C8;
    for (int i = 0; i < sel->count; i++) {
        qword origin = ToVF(position);
        ITEM *item = &sel->items[i];
        if (item == current)
            continue;
        if (!(item->flags & 4))
            continue;
        float zero = 0.0f;
        qword point = XYZ(item->x, item->y * D_002C3280, zero, home);
        float score;
        if (func_0014C748(point, origin, Add(origin, ray), 1, &hit, D_002C3278) > 0) {
            score = hit.fraction * D_002C3274;
        } else {
            FindClosestPointOnLineSegment(point, origin, Add(origin, ray), 0, &proximity.along, &proximity.distance);
            if (proximity.distance < D_002C327C) {
                if (zero < proximity.along)
                    score = D_002C3274 + proximity.distance;
                else
                    continue;
            } else
                continue;
        }
        if (score < best) {
            bestId = item->id;
            best = score;
        }
    }
    if (bestId != -1) {
        SelectGadgetMenuItem(sel, bestId);
        func_001DA9E8(sel);
        SetSndSourceLo(PsndStartSound(0x35), 0);
    }
}
} // namespace M_SetGadgetMenuStickDir

namespace M_SelectGadgetMenuItem { extern "C" {
typedef struct {
    int key;
    int a;
    int b;
    int c;
    int cur;
    int pad[2];
    int flags;
} E;
typedef struct {
    char pad0[0x24];
    void *x24;
    char pad28[0x48 - 0x28];
    int key;
    int n;
    E *ae;
} OBJ;
void StopWidgetAnim(void *, int);
void PlayWidgetAnim(void *, int);
/** @sly2-annot addr=0x1DB390
 * Purpose: Select item key and switch each item's highlight anim (selected / acquired / unlocked)
 * Info: proposed name SelectGadgetMenuItem (T1); TU screen
 * Confidence: mid (draft) [T1 0.55] - evidence: static analysis; sources: r9_bhv_screen
 * See: docs/reference/functions/hud-ui.md#screen-0x1d93d8-0x1de2a0
 * @end-sly2-annot */
void SelectGadgetMenuItem(OBJ *p, int key)
{
    int i;
    p->key = key;
    for (i = 0; i < p->n; i++) {
        E *e = &p->ae[i];
        int v;
        if (e->key == key)
            v = e->c;
        else if (e->flags & 4)
            v = e->b;
        else if (e->flags & 2)
            v = e->a;
        else
            v = -1;
        if (v != e->cur) {
            if (p->x24 && e->cur != -1)
                StopWidgetAnim(p->x24, e->cur);
            e->cur = v;
            if (p->x24 && v != -1)
                PlayWidgetAnim(p->x24, v);
        }
    }
}
} } // namespace M_SelectGadgetMenuItem

/** @sly2-annot addr=0x1DB490
 * Purpose: Open the gadget menu page for the current hero: 0 Sly (g_pjt), 1 Bentley (D_002DD5BC), 2 Murray
 *   (D_002F7900)
 * Info: proposed name OpenGadgetMenuForHero (T1); TU screen
 * Confidence: mid (draft) [T1 0.55] - evidence: static analysis; sources: r9_bhv_screen
 * See: docs/reference/functions/hud-ui.md#screen-0x1d93d8-0x1de2a0
 * @end-sly2-annot */
extern "C" void OpenGadgetMenuForHero(GADGETMENU *pgm)
{
    LO *ploHero = PpoHero(&D_003DD490);
    if (ploHero != 0) {
        if (ploHero == D_002E1E40) {
            SetGadgetMenuState(pgm, 0);
            return;
        }
        if (ploHero == D_002DD5BC) {
            SetGadgetMenuState(pgm, 1);
            return;
        }
        if (ploHero == D_002F7900) {
            SetGadgetMenuState(pgm, 2);
        }
    }
}

namespace M_SetGadgetMenuState { extern "C" {
typedef struct { short d; short pad; void *pfn; } VTE;
typedef struct { char pad0[0xC]; VTE *vt; } SUB;
typedef struct {
    int key;
    int a;
    int b;
    int c;
    int cur;
    float x14;
    float x18;
    int flags;
} E;
typedef struct OBJ {
    char pad0[0xC];
    VTE *vt;
    char pad10[0x20 - 0x10];
    int x20;
    SUB *x24;
    char pad28[0x40 - 0x28];
    int x40;
    float x44;
    int x48;
    int n;
    E *ae;
} OBJ;
extern struct CLOCK { float a[16]; } D_002DDED0;
extern char D_003DD490[];
extern E D_002C2DE8[], D_002C3028[], D_002C3148[];
extern float D_002A79CC;
void func_0014D1D0(struct CLOCK *, float, int);
void IncrementSwHandsOff(void *, int);
void DecrementSwHandsOff(void *, int);
void SelectGadgetMenuItem(OBJ *, int);
void func_001DA950(OBJ *, int, int);
void RefreshGadgetMenuItems(OBJ *);
SUB *PloFindSwObject__FP2SWi3OIDP2LO(void *, int, int, int);
void SetGadgetMenuState(OBJ *p, int s);
#define VCALL(o, n) ((void (*)(void *))(o)->vt[n].pfn)((char *)(o) + (o)->vt[n].d)
/** @sly2-annot addr=0x1DB520
 * Purpose: Gadget menu open/close: opening freezes the clock and hands-off and shows the screen; page 0/1/2 =
 *   Sly (widget 0x375, 18 items) / Bentley (0x376, 9) / Murray (0x377, 8); close restores clock
 * Info: proposed name SetGadgetMenuState (T1); TU screen
 * Confidence: mid (draft) [T1 0.60] - evidence: static analysis; sources: r9_bhv_screen
 * See: docs/reference/functions/hud-ui.md#screen-0x1d93d8-0x1de2a0
 * @end-sly2-annot */
void SetGadgetMenuState(OBJ *p, int s)
{
    if (s == p->x40)
        return;
    if (p->x40 == -1) {
        func_0014D1D0(&D_002DDED0, 0.0f, 0);
        IncrementSwHandsOff(D_003DD490, 0);
        VCALL(p, 10);
    }
    SelectGadgetMenuItem(p, -1);
    p->x40 = s;
    p->x44 = D_002DDED0.a[15];
    switch (s) {
    case -1: {
        int i;
        if (p->x24)
            VCALL(p->x24, 50);
        for (i = 0; i < p->n; i++)
            p->ae[i].cur = -1;
        p->n = 0;
        p->ae = 0;
        func_0014D1D0(&D_002DDED0, 1.0f, 0);
        DecrementSwHandsOff(D_003DD490, 0);
        VCALL(p, 11);
        func_001DA950(p, -1, -1);
        p->x24 = 0;
        p->x20 = -1;
        break;
    }
    case 0:
        p->x20 = 0x375;
        p->n = 0x12;
        p->ae = D_002C2DE8;
        break;
    case 1:
        p->x20 = 0x376;
        p->n = 9;
        p->ae = D_002C3028;
        break;
    case 2:
        p->x20 = 0x377;
        p->n = 8;
        p->ae = D_002C3148;
        break;
    }
    if (p->x40 != -1) {
        SUB *e;
        RefreshGadgetMenuItems(p);
        e = PloFindSwObject__FP2SWi3OIDP2LO(D_003DD490, 5, p->x20, 0);
        p->x24 = e;
        if (!e) {
            SetGadgetMenuState(p, -1);
        } else {
            float best;
            int key;
            int i;
            ((void (*)(void *, int))e->vt[49].pfn)((char *)e + e->vt[49].d, 0x318);
            best = D_002A79CC;
            key = -1;
            for (i = 0; i < p->n; i++) {
                E *pe = &p->ae[i];
                if (pe->flags & 4) {
                    float f = pe->x14 + pe->x18;
                    if (f < best) {
                        key = pe->key;
                        best = f;
                    }
                }
            }
            SelectGadgetMenuItem(p, key);
        }
    }
}
} } // namespace M_SetGadgetMenuState
#undef VCALL

namespace M_func_001DB7A8 { extern "C" {
extern char D_002D84E8[];
struct S { int a, b, c; void *vt; char pad[0x10]; int n20; };
struct S *func_001DA728(struct S *p);
void ClearThiefnet(struct S *p);
/** @sly2-annot addr=0x1DB7A8
 * Purpose: Screen subclass with vtable 0x2D84E8, widget oid 0x378, 64-entry selection table
 * Info: proposed name SCREEN378_ctor (T2); class SCREEN378; TU screen
 * Confidence: low (draft) [T2 0.30] - evidence: static analysis; sources: r6b_rest
 * See: docs/reference/functions/hud-ui.md#screen-0x1d93d8-0x1de2a0
 * @end-sly2-annot */
struct S *func_001DB7A8(struct S *p) {
    func_001DA728(p);
    p->vt = D_002D84E8;
    p->n20 = 0x378;
    ClearThiefnet(p);
    return p;
}
} } // namespace M_func_001DB7A8

/** @sly2-annot addr=0x1DB7F0
 * Purpose: ThiefNet shop reset (slot 13)
 * Info: proposed name ResetThiefnet (T1); TU screen
 * Confidence: mid (draft) [T1 0.60] - evidence: static analysis; sources: r9_bhv_screen
 * See: docs/reference/functions/hud-ui.md#screen-0x1d93d8-0x1de2a0
 * @end-sly2-annot */
extern "C" void ResetThiefnet(SCREEN *pscreen)
{
    func_001DA7B0(pscreen);
    ClearThiefnet(pscreen);
}

namespace M_ClearThiefnet { extern "C" {
extern struct CLOCK { float a[16]; } D_002DDED0;
struct P { int a, b; };
struct S { char p0[0x40]; int n40; float g44; int pad; struct P ap[64]; char p1[0x25C-0x24C]; int n25C, n260, n264; };
/** @sly2-annot addr=0x1DB820
 * Purpose: ThiefNet: state -1, clear the 64-entry item list and anim handles
 * Info: proposed name ClearThiefnet (T1); TU screen
 * Confidence: mid (draft) [T1 0.55] - evidence: static analysis; sources: r9_bhv_screen
 * See: docs/reference/functions/hud-ui.md#screen-0x1d93d8-0x1de2a0
 * @end-sly2-annot */
void ClearThiefnet(struct S *p) {
    unsigned int i;
    p->n40 = -1;
    p->g44 = D_002DDED0.a[15];
    p->n25C = -1;
    p->n260 = -1;
    p->n264 = -1;
    for (i = 0; i < 64; i++) {
        p->ap[i].a = -1;
        p->ap[i].b = -1;
    }
}
} } // namespace M_ClearThiefnet

namespace M_UpdateThiefnet {
struct CRef {
    int m_tagk;
    int m_tag;
    CRef() { m_tagk = -1; }
    CRef(int n) { m_tagk = 0x15; m_tag = n; }
    CRef(const CRef &);
    ~CRef() { Decref(); }
    CRef &operator=(const CRef &) __asm__("__as__4CRefRC4CRef");
    void Decref() __asm__("Decref__4CRef");
};
struct REC { int a, b, x8; int pad[5]; };
struct REC12 { int x0, x4, x8; };
struct OBJ {
    char pad0[0x10];
    void *x10;
    char pad14[0x30 - 0x14];
    int x30;
    char pad34[0x40 - 0x34];
    int x40;
    char pad44[0x254 - 0x44];
    int x254;
    int x258;
};
extern "C" {
extern REC D_002BCD00[];
extern REC12 D_002C3288[];
extern char D_002DEB30[];
extern char D_003DD490[];
extern char D_002A79A0[], D_002A79D0[], D_002A79D8[];
void func_001DA808(OBJ *);
int GrfHandleJoyBtn(void *, int);
void SetThiefnetTab(OBJ *, int);
int func_00161798(void *);
void func_001617E0(void *, int);
void func_001DA950(OBJ *, int, int);
void SetPowerupAcquired(int, int);
void RefreshThiefnetItems(OBJ *);
void *PsndStartSound(int);
void SetSndSourceLo(void *, int);
void func_0019F0A8(void *);
void sprintf(char *, const char *, ...);
CRef RefGetSaveVarV(const char *, ...);
CRef RefSetSaveVarV(const CRef &, const char *, ...);
void UpdateScreenStickDir(OBJ *);
void UpdateThiefnetWidgets(OBJ *);
}
/** @sly2-annot addr=0x1DB870
 * Purpose: ThiefNet per-frame (slot 7): tab 0 buy the selected power-up (cost check against coins, confirm,
 *   deduct, SetPowerupAcquired, notify players); tab 1 sell one loot item (loot.mplootkc[i]-1, add its price
 *   to coins); tab switch and close
 * Info: proposed name UpdateThiefnet (T1); TU screen
 * Confidence: mid (draft) [T1 0.65] - evidence: static analysis; sources: r9_bhv_screen
 * See: docs/engine/hud-ui.md#2-screens-and-menus-screencpp-0x1d93d8-menu-0x1ae928,
 *   docs/engine/minigames.md#8-safehouse-and-thiefnet-ccode,
 *   docs/reference/functions/hud-ui.md#screen-0x1d93d8-0x1de2a0
 * @end-sly2-annot */
extern "C" void UpdateThiefnet(OBJ *p)
{
    func_001DA808(p);
    if (p->x40 == -1)
        return;
    if (!p->x10)
        return;
    if (p->x30 && GrfHandleJoyBtn(p->x10, 0x10)) {
        SetThiefnetTab(p, -1);
        return;
    }
    switch (p->x40) {
    case 0:
        if (GrfHandleJoyBtn(p->x10, 0x40)) {
            if (p->x254 != -1) {
                int cost = D_002BCD00[p->x254].x8;
                int have = func_00161798(D_002DEB30);
                if (!p->x30) {
                    func_001DA950(p, -1, -1);
                    func_001617E0(D_002DEB30, have - cost);
                    SetPowerupAcquired(p->x254, 1);
                    RefreshThiefnetItems(p);
                    SetSndSourceLo(PsndStartSound(0x29), 0);
                    func_0019F0A8(D_003DD490);
                } else if (have >= cost) {
                    func_001DA950(p, 0, 0x34);
                } else {
                    func_001DA950(p, 1, 0x35);
                }
            } else {
                SetSndSourceLo(PsndStartSound(0x36), 0);
            }
        }
        break;
    case 1:
        if (GrfHandleJoyBtn(p->x10, 0x40)) {
            if (p->x258 == -1) {
                SetSndSourceLo(PsndStartSound(0x36), 0);
            } else {
                CRef r;
                char buf[32];
                sprintf(buf, D_002A79A0, p->x258);
                r = RefGetSaveVarV(D_002A79D0, D_002A79D8, buf, 0);
                if (r.m_tag > 0) {
                    RefSetSaveVarV(CRef(r.m_tag - 1), D_002A79D0, D_002A79D8, buf, 0);
                    func_001617E0(D_002DEB30, func_00161798(D_002DEB30) + D_002C3288[p->x258].x0);
                    RefreshThiefnetItems(p);
                    SetSndSourceLo(PsndStartSound(0x29), 0);
                }
            }
        }
        break;
    }
    if (!p->x30 && GrfHandleJoyBtn(p->x10, 0x10))
        func_001DA950(p, -1, -1);
    if (GrfHandleJoyBtn(p->x10, 0x80)) {
        func_001DA950(p, -1, -1);
        SetThiefnetTab(p, p->x40 == 0);
    }
    if (p->x30)
        UpdateScreenStickDir(p);
    UpdateThiefnetWidgets(p);
}
} // namespace M_UpdateThiefnet

namespace M_func_001DBBA0 { extern "C" {
extern int D_002C35B4;
extern int D_003DA570;
struct S { char p0[0x48]; int n48; char p1[0x250-0x4C]; int n250; };
/** @sly2-annot addr=0x1DBBA0
 * Purpose: Index past the last visible row (scroll window D_002C35B4 rows, adjusted for scroll arrows)
 * Info: proposed name IThiefnetRowEnd (T2); TU screen
 * Confidence: low (draft) [T2 0.40] - evidence: static analysis; sources: r9_bhv_screen
 * See: docs/reference/functions/hud-ui.md#screen-0x1d93d8-0x1de2a0
 * @end-sly2-annot */
int func_001DBBA0(struct S *p) {
    int n = p->n250 + D_002C35B4;
    if (D_003DA570) {
        n -= (p->n250 > 0);
        n -= (n < p->n48);
    }
    return n;
}
} } // namespace M_func_001DBBA0

namespace M_SelectThiefnetItem { extern "C" {
typedef struct { int n; int a; } ENT;
typedef struct {
    char pad[0x48];
    int c;
    ENT ae[64];
    int i;
    int first;
    int x254;
    int x258;
} OBJ;
extern int D_003DA570;
extern int D_002C35B4;
int func_001DBBA0(OBJ *);
void func_001DA9E8(OBJ *);
static inline int Max(int a, int b) { return a < b ? b : a; }
static inline int Clamp(int n, int lo, int hi)
{
    if (n < lo)
        return lo;
    if (n > hi)
        return hi;
    return n;
}
/** @sly2-annot addr=0x1DBBE0
 * Purpose: Select row i (clamped), scroll the window, set current power-up/loot id
 * Info: proposed name SelectThiefnetItem (T1); TU screen
 * Confidence: mid (draft) [T1 0.55] - evidence: static analysis; sources: r9_bhv_screen
 * See: docs/reference/functions/hud-ui.md#screen-0x1d93d8-0x1de2a0
 * @end-sly2-annot */
void SelectThiefnetItem(OBJ *p, int i)
{
    if (p->c == 0) {
        p->first = 0;
        p->i = -1;
        p->x254 = -1;
        p->x258 = -1;
    } else {
        i = Clamp(i, 0, p->c - 1);
        p->i = i;
        if (D_003DA570) {
            if (i < p->first) {
                p->first = i;
            } else {
                while (func_001DBBA0(p) - 1 < p->i)
                    p->first++;
            }
            if (p->first == 1)
                p->first = 0;
        } else {
            int n = Clamp(p->first, Max(i - D_002C35B4 + 1, 0), i);
            p->first = n;
        }
        p->x254 = p->ae[p->i].n;
        p->x258 = p->ae[p->i].n;
    }
    func_001DA9E8(p);
}
} } // namespace M_SelectThiefnetItem

namespace M_RefreshThiefnetItems {
struct CRef {
    int m_tagk;
    int m_tag;
    CRef() { m_tagk = -1; }
    CRef(const CRef &);
    ~CRef() { Decref(); }
    CRef &operator=(const CRef &) __asm__("__as__4CRefRC4CRef");
    void Decref() __asm__("Decref__4CRef");
};
struct ENT { int n; int a; };
struct OBJ {
    char pad[0x40];
    int x40;
    int pad44;
    unsigned int c;
    ENT ae[0x40];
    int i;
};
extern "C" { extern char D_002A79A0[]; }
extern "C" { extern char D_002A79D0[]; }
extern "C" { extern char D_002A79D8[]; }
extern "C" int sprintf(char *, const char *, ...);
extern "C" CRef RefGetSaveVarV(const char *, ...);
extern "C" int func_001DAE58(int);
extern "C" int FPowerupAcquired(int);
extern "C" void SelectThiefnetItem(OBJ *, int);
/** @sly2-annot addr=0x1DBD10
 * Purpose: Rebuild the list: tab 0 power-ups 7..0x2A unlocked by episode and not yet acquired; tab 1 loot
 *   kinds with count > 0
 * Info: proposed name RefreshThiefnetItems (T1); TU screen
 * Confidence: mid (draft) [T1 0.65] - evidence: static analysis; sources: r9_bhv_screen
 * See: docs/engine/hud-ui.md#2-screens-and-menus-screencpp-0x1d93d8-menu-0x1ae928,
 *   docs/engine/minigames.md#8-safehouse-and-thiefnet-ccode,
 *   docs/reference/functions/hud-ui.md#screen-0x1d93d8-0x1de2a0
 * @end-sly2-annot */
extern "C" void RefreshThiefnetItems(OBJ *p)
{
    {
        int j;
        for (j = 0; j < (int)p->c; j++)
            p->ae[j].n = -1;
    }
    p->c = 0;
    switch (p->x40) {
    case 0: {
        int i;
        for (i = 7; i < 0x2B; i++) {
            if (!func_001DAE58(i))
                continue;
            if (FPowerupAcquired(i))
                continue;
            if (p->c > 0x40)
                break;
            p->ae[p->c++].n = i;
        }
        break;
    }
    case 1: {
        int i;
        for (i = 0; i < 0x40; i++) {
            CRef ref;
            char ach[0x20];
            sprintf(ach, D_002A79A0, i);
            ref = RefGetSaveVarV(D_002A79D0, D_002A79D8, ach, 0);
            if (ref.m_tag == 0)
                continue;
            if (p->c > 0x40)
                break;
            p->ae[p->c++].n = i;
        }
        break;
    }
    }
    SelectThiefnetItem(p, p->i);
}
} // namespace M_RefreshThiefnetItems

namespace M_UpdateThiefnetWidgets { extern "C" {
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
} } // namespace M_UpdateThiefnetWidgets

namespace M_SetThiefnetTab { extern "C" {
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
typedef struct OBJ {
    char pad0[0xC];
    VTE *vt;
    char pad10[0x24 - 0x10];
    SUB *x24;
    char pad28[0x40 - 0x28];
    int x40;
    float x44;
    char pad48[0x25C - 0x48];
    int x25C;
    int x260;
    int x264;
} OBJ;
extern struct CLOCK { float a[16]; } D_002DDED0;
extern char D_003DD490[];
void func_0014D1D0(struct CLOCK *, float, int);
void IncrementSwHandsOff(void *, int);
void DecrementSwHandsOff(void *, int);
void SetWidgetState(SUB *, int);
void SetWidgetRequest(SUB *, int);
void *PsndStartSound(int);
void SetSndSourceLo(void *, int);
void RefreshThiefnetItems(OBJ *);
void SelectThiefnetItem(OBJ *, int);
#define VCALL(o, n) ((void (*)(void *))(o)->vt[n].pfn)((char *)(o) + (o)->vt[n].d)
/** @sly2-annot addr=0x1DC260
 * Purpose: ThiefNet open/tab: -1 close (resume clock, hands-off--, hide), 0 buy page (anims 0x4D4-0x4D6), 1
 *   sell page (0x49B-0x49D); refresh list
 * Info: proposed name SetThiefnetTab (T1); TU screen
 * Confidence: mid (draft) [T1 0.60] - evidence: static analysis; sources: r9_bhv_screen
 * See: docs/engine/hud-ui.md#2-screens-and-menus-screencpp-0x1d93d8-menu-0x1ae928,
 *   docs/engine/minigames.md#8-safehouse-and-thiefnet-ccode,
 *   docs/reference/functions/hud-ui.md#screen-0x1d93d8-0x1de2a0
 * @end-sly2-annot */
void SetThiefnetTab(OBJ *p, int s)
{
    int old;
    if (!p->x24)
        s = -1;
    if (s == p->x40)
        return;
    if (p->x40 == -1) {
        func_0014D1D0(&D_002DDED0, 0.0f, 0);
        IncrementSwHandsOff(D_003DD490, 0);
        VCALL(p, 10);
        SetWidgetState(p->x24, 1);
    }
    old = p->x40;
    p->x40 = s;
    p->x44 = D_002DDED0.a[15];
    switch (s) {
    case -1:
        VCALL(p->x24, 50);
        func_0014D1D0(&D_002DDED0, 1.0f, 0);
        DecrementSwHandsOff(D_003DD490, 0);
        VCALL(p, 11);
        if (p->x24->x58 > 0)
            SetWidgetRequest(p->x24, 1);
        p->x25C = s;
        p->x260 = s;
        p->x264 = s;
        break;
    case 0:
        p->x24->x74 = 0x4D4;
        p->x24->x70 = 0x4D5;
        p->x24->x78 = 0x4D6;
        if (old == 1) {
            SetWidgetState(p->x24, 1);
            SetWidgetState(p->x24, 2);
        } else {
            SetWidgetRequest(p->x24, 2);
        }
        SetSndSourceLo(PsndStartSound(0x35), 0);
        break;
    case 1:
        p->x24->x74 = 0x49B;
        p->x24->x70 = 0x49C;
        p->x24->x78 = 0x49D;
        if (old == 0) {
            SetWidgetState(p->x24, 1);
            SetWidgetState(p->x24, 2);
        } else {
            SetWidgetRequest(p->x24, 2);
        }
        SetSndSourceLo(PsndStartSound(0x35), 0);
        break;
    }
    RefreshThiefnetItems(p);
    SelectThiefnetItem(p, 0);
    ((void (*)(void *, int))p->vt[14].pfn)((char *)p + p->vt[14].d, -1);
}
} } // namespace M_SetThiefnetTab
#undef VCALL

namespace M_func_001DC4B0 { extern "C" {
extern struct CLOCK { float a[16]; } D_002DDED0;
typedef struct {
    char pad[0x28];
    int state;
    float t;
    char pad30[0x48 - 0x30];
    int c;
    char pad4c[0x24C - 0x4C];
    int i;
} OBJ;
void SetThiefnetTab(OBJ *, int);
void SelectThiefnetItem(OBJ *, int);
void *PsndStartSound(int);
void SetSndSourceLo(void *, int);
/** @sly2-annot addr=0x1DC4B0
 * Purpose: ThiefNet slot 14: dir 0 -> sell tab, 4 -> buy tab, 1-3 previous row, 5-7 next row (with sounds)
 * Info: proposed name SetThiefnetStickDir (T2); TU screen
 * Notes: verifier PLAUSIBLE: Screen vtable slot 14 (vtable 0x2D8520 region). State setter for an 8-way
 *   direction: 0 -> 0x1DC260(1), 4 -> 0x1DC260(0), 1-3 select row i-1, 5-7 row i+1, with sounds 0x35 (moved)
 *   / 0x36 (at end). The | T2 forced: verifier: role word not supported by the code
 * Confidence: low (draft) [T2 0.55] - evidence: static analysis; sources: r9_bhv_screen+verdicts_followups
 * See: docs/engine/hud-ui.md#5-key-functions, docs/reference/functions/hud-ui.md#screen-0x1d93d8-0x1de2a0
 * @end-sly2-annot */
void func_001DC4B0(OBJ *p, unsigned int n)
{
    if (n == p->state)
        return;
    p->state = n;
    p->t = D_002DDED0.a[15];
    switch (n) {
    case 0:
        SetThiefnetTab(p, 1);
        break;
    case 4:
        SetThiefnetTab(p, 0);
        break;
    case 1:
    case 2:
    case 3:
        if (p->i > 0) {
            SelectThiefnetItem(p, p->i - 1);
            SetSndSourceLo(PsndStartSound(0x35), 0);
        } else {
            SetSndSourceLo(PsndStartSound(0x36), 0);
        }
        break;
    case 5:
    case 6:
    case 7:
        if (p->i < p->c - 1) {
            SelectThiefnetItem(p, p->i + 1);
            SetSndSourceLo(PsndStartSound(0x35), 0);
        } else {
            SetSndSourceLo(PsndStartSound(0x36), 0);
        }
        break;
    }
}
} } // namespace M_func_001DC4B0

namespace M_BuildGadgetMenuInfoText { extern "C" {
typedef struct { char pad[0x30]; int x30; int x34; int x38; char pad3c[0x48 - 0x3C]; int x48; } GS;
typedef struct { int pad[5]; int x14; int x18; int x1C; } REC;
typedef struct { char pad[0x1C]; int x1C; } OBJ;
extern GS D_003DA0F0;
extern char D_003DD490[];
extern REC D_002BCD00[];
extern char *D_002C3268[];
extern char D_002A7A10[], D_002A7A18[], D_002A7A28[];
char *PchzFindSwLangstr__FP2SWi(void *, int);
char *strcpy(char *, char *);
int sprintf(char *, char *, ...);
OBJ *func_001DAE20(GS *,int);
void *PpoHero(void *);
int IGadgetSlotFromId(void *, int);
/** @sly2-annot addr=0x1DC598
 * Purpose: Text callback for widgets 0x37A/0x37B: popup message, "none", or the selected power-up's name,
 *   slot button and description
 * Info: proposed name BuildGadgetMenuInfoText (T1); TU screen
 * Confidence: mid (draft) [T1 0.50] - evidence: static analysis; sources: r9_bhv_screen
 * See: docs/reference/functions/hud-ui.md#screen-0x1d93d8-0x1de2a0
 * @end-sly2-annot */
void BuildGadgetMenuInfoText(char *out)
{
    char buf[256];
    unsigned char *s;
    if (D_003DA0F0.x30 != -1) {
        strcpy(out, PchzFindSwLangstr__FP2SWi(D_003DD490, D_003DA0F0.x38));
    } else if (D_003DA0F0.x48 == -1) {
        strcpy(out, PchzFindSwLangstr__FP2SWi(D_003DD490, 0xDD));
    } else {
        OBJ *po = func_001DAE20(&D_003DA0F0,D_003DA0F0.x48);
        int v = IGadgetSlotFromId(PpoHero(D_003DD490), D_003DA0F0.x48);
        if ((po->x1C & 1) && v < 0) {
            strcpy(buf, PchzFindSwLangstr__FP2SWi(D_003DD490, 0xE1));
            s = (unsigned char *)D_002A7A10;
        } else {
            char *sz;
            if (v >= 0)
                sz = D_002C3268[v];
            else
                sz = D_002A7A10;
            sprintf(buf, PchzFindSwLangstr__FP2SWi(D_003DD490, D_002BCD00[D_003DA0F0.x48].x1C), sz, sz, sz);
            s = (unsigned char *)PchzFindSwLangstr__FP2SWi(D_003DD490, D_002BCD00[D_003DA0F0.x48].x18);
        }
        sprintf(out, D_002A7A18, PchzFindSwLangstr__FP2SWi(D_003DD490, D_002BCD00[D_003DA0F0.x48].x14),
                      *s ? D_002A7A28 : D_002A7A10, s, buf);
    }
}
} } // namespace M_BuildGadgetMenuInfoText

namespace M_BuildThiefnetPowerupNameText { extern "C" {
struct E { int n; int a; };
struct G2 { char pad[0x4C]; struct E ae[0x40]; int nPad; int i; };
extern struct G2 D_003DA150;
struct T { int a[5]; int id; int b[2]; };
extern struct T D_002BCD00[];
extern struct G D_003DD490;
void *PchzFindSwLangstr__FP2SWi(struct G *g, int n);
void strcpy(char *p, void *q);
/** @sly2-annot addr=0x1DC790
 * Purpose: Text callback rows 0x43C-0x441: power-up name of row i
 * Info: proposed name BuildThiefnetPowerupNameText (T1); TU screen
 * Confidence: mid (draft) [T1 0.50] - evidence: static analysis; sources: r9_bhv_screen
 * See: docs/engine/minigames.md#8-safehouse-and-thiefnet-ccode,
 *   docs/reference/functions/hud-ui.md#screen-0x1d93d8-0x1de2a0
 * @end-sly2-annot */
void BuildThiefnetPowerupNameText(char *pch, int i) {
    struct G2 *pg = &D_003DA150;
    unsigned int n = pg->ae[pg->i + i].n;
    int id = -1;
    if (n < 0x2B) id = D_002BCD00[n].id;
    if (id != -1) strcpy(pch, PchzFindSwLangstr__FP2SWi(&D_003DD490, id));
    else *pch = 0;
}
} } // namespace M_BuildThiefnetPowerupNameText

namespace M_func_001DC820 { extern "C" {
struct E { int n; int a; };
struct G2 { char pad[0x4C]; struct E ae[0x40]; int nPad; int i; };
extern struct G2 D_003DA150;
struct T { int a[2]; char *pch; int b[5]; };
extern struct T D_002BCD00[];
extern char D_002A79A0[];
void sprintf(char *pch, char *fmt, char *s);
/** @sly2-annot addr=0x1DC820
 * Purpose: Text callback rows 0x442-0x447: power-up cost of row i
 * Info: proposed name BuildThiefnetPowerupCostText (T2); TU screen
 * Confidence: low (draft) [T2 0.45] - evidence: static analysis; sources: r9_bhv_screen
 * See: docs/reference/functions/hud-ui.md#screen-0x1d93d8-0x1de2a0
 * @end-sly2-annot */
void func_001DC820(char *pch, int i) {
    struct G2 *pg = &D_003DA150;
    unsigned int n = pg->ae[pg->i + i].n;
    char *s = 0;
    if (n < 0x2B) s = D_002BCD00[n].pch;
    if (s) sprintf(pch, D_002A79A0, s);
    else *pch = 0;
}
} } // namespace M_func_001DC820

namespace M_BuildThiefnetLootNameText {
struct CRef {
    int m_tagk;
    int m_tag;
    CRef() { m_tagk = -1; }
    CRef(const CRef &);
    ~CRef() { Decref(); }
    CRef &operator=(const CRef &) __asm__("__as__4CRefRC4CRef");
    void Decref() __asm__("Decref__4CRef");
};
struct ENT { int n; int a; };
struct TBL {
    char pad[0x4C];
    ENT ae[0x40];
    int i;
    int first;
};
struct REC { char *name; int ids; int pad; };
struct G;
extern "C" { extern G D_003DD490; }
extern "C" { extern TBL D_003DA150; }
extern "C" { extern REC D_002C3288[]; }
extern "C" { extern char D_002A79A0[]; }
extern "C" { extern char D_002A79D0[]; }
extern "C" { extern char D_002A79D8[]; }
extern "C" { extern char D_002A7A30[]; }
extern "C" int sprintf(char *, const char *, ...);
extern "C" char *strcpy(char *, const char *);
extern "C" CRef RefGetSaveVarV(const char *, ...);
extern "C" { char *PchzFindSwLangstr__FP2SWi(G *, int); }
/** @sly2-annot addr=0x1DC890
 * Purpose: Text callback rows 0x448-0x44D: loot name of row i, "%s (%d)" when owned more than once
 * Info: proposed name BuildThiefnetLootNameText (T1); TU screen
 * Confidence: mid (draft) [T1 0.50] - evidence: static analysis; sources: r9_bhv_screen
 * See: docs/reference/functions/hud-ui.md#screen-0x1d93d8-0x1de2a0
 * @end-sly2-annot */
extern "C" void BuildThiefnetLootNameText(char *pch, int k)
{
    int ids = -1;
    int c = 0;
    unsigned int i = D_003DA150.ae[D_003DA150.first + k].n;
    if (i < 0x40) {
        CRef ref;
        char ach[0x20];
        sprintf(ach, D_002A79A0, i);
        ref = RefGetSaveVarV(D_002A79D0, D_002A79D8, ach, 0);
        c = ref.m_tag;
        ids = D_002C3288[i].ids;
    }
    if (ids != -1) {
        strcpy(pch, PchzFindSwLangstr__FP2SWi(&D_003DD490, ids));
        if (c >= 2)
            sprintf(pch, D_002A7A30, pch, c);
    } else {
        *pch = 0;
    }
}
} // namespace M_BuildThiefnetLootNameText

namespace M_func_001DC9C0 { extern "C" {
struct E { int n; int a; };
struct G2 { char pad[0x4C]; struct E ae[0x40]; int nPad; int i; };
extern struct G2 D_003DA150;
struct T { char *pch; int b[2]; };
extern struct T D_002C3288[];
extern char D_002A79A0[];
void sprintf(char *pch, char *fmt, char *s);
/** @sly2-annot addr=0x1DC9C0
 * Purpose: Text callback rows 0x44E-0x453: loot price of row i
 * Info: proposed name BuildThiefnetLootPriceText (T2); TU screen
 * Confidence: low (draft) [T2 0.45] - evidence: static analysis; sources: r9_bhv_screen
 * See: docs/reference/functions/hud-ui.md#screen-0x1d93d8-0x1de2a0
 * @end-sly2-annot */
void func_001DC9C0(char *pch, int i) {
    struct G2 *pg = &D_003DA150;
    unsigned int n = pg->ae[pg->i + i].n;
    char *s = 0;
    if (n < 0x40) s = D_002C3288[n].pch;
    if (s) sprintf(pch, D_002A79A0, s);
    else *pch = 0;
}
} } // namespace M_func_001DC9C0

namespace M_func_001DCA30 { extern "C" {
extern int D_003DA180;
extern struct G D_003DD490;
void *PchzFindSwLangstr__FP2SWi(struct G *g, int n);
void strcpy(char *p, void *q);
/** @sly2-annot addr=0x1DCA30
 * Purpose: Text callback 0x436: popup prompt text by popup mode (-1/0/1 -> ids 0xDE/0xDF/0xE0)
 * Info: proposed name BuildThiefnetPromptText (T2); TU screen
 * Confidence: low (draft) [T2 0.45] - evidence: static analysis; sources: r9_bhv_screen
 * See: docs/reference/functions/hud-ui.md#screen-0x1d93d8-0x1de2a0
 * @end-sly2-annot */
void func_001DCA30(char *pch) {
    int id;
    switch (D_003DA180) {
    case -1: id = 0xDE; break;
    case 0: id = 0xDF; break;
    case 1: id = 0xE0; break;
    default: *pch = 0; return;
    }
    strcpy(pch, PchzFindSwLangstr__FP2SWi(&D_003DD490, id));
}
} } // namespace M_func_001DCA30

namespace M_BuildThiefnetInfoText { extern "C" {
typedef struct { int x0,x4,cost,xC,x10,ids1,ids2,x1C; } REC;
typedef struct { char *name; int ids,pad; } REC2;
typedef struct {
    char pad[0x30];
    int x30,pad34,x38,pad3C,x40;
    char pad44[0x254-0x44];
    int x254,x258;
} TBL;
extern TBL D_003DA150;
extern REC D_002BCD00[];
extern REC2 D_002C3288[];
extern struct G D_003DD490;
extern char D_002DEB30[],D_002A7A38[];
char *PchzFindSwLangstr__FP2SWi(struct G *,int);
int func_00161798(void *);
int sprintf(char *,const char *,...);
char *strcpy(char *,const char *);
static inline void CopyId(char *p,int id)
{
    strcpy(p,PchzFindSwLangstr__FP2SWi(&D_003DD490,id));
}
/** @sly2-annot addr=0x1DCAB0
 * Purpose: Text callback 0x435: popup message, or the selected power-up's name, description and affordability
 *   (0xD9/0xDA), or the loot sell line (0xDB)
 * Info: proposed name BuildThiefnetInfoText (T1); TU screen
 * Confidence: mid (draft) [T1 0.50] - evidence: static analysis; sources: r9_bhv_screen
 * See: docs/engine/minigames.md#8-safehouse-and-thiefnet-ccode,
 *   docs/reference/functions/hud-ui.md#screen-0x1d93d8-0x1de2a0
 * @end-sly2-annot */
void BuildThiefnetInfoText(char *pch)
{
    TBL *pt=&D_003DA150;
    if(pt->x30!=-1) {
        CopyId(pch,pt->x38);
        return;
    } else {
        switch(pt->x40) {
        case 0: {
            unsigned int i=pt->x254;
            if(i<0x2B) {
                if(D_002BCD00[i].ids2!=pt->x30) {
                    int *pc=&D_002BCD00[i].cost;
                    int count=func_00161798(D_002DEB30);
                    int idsT=count<*pc?0xD9:0xDA;
                    struct G *pg=&D_003DD490;
                    char *fmt=D_002A7A38;
                    char *pch1=PchzFindSwLangstr__FP2SWi(pg,D_002BCD00[i].ids1);
                    char *pch2=PchzFindSwLangstr__FP2SWi(pg,D_002BCD00[i].ids2);
                    sprintf(pch,fmt,pch1,pch2,PchzFindSwLangstr__FP2SWi(pg,idsT));
                    return;
                }
            }
            CopyId(pch,0xDC);
            return;
        }
        case 1: {
            unsigned int i=pt->x258;
            if(i<0x40) {
                sprintf(pch,PchzFindSwLangstr__FP2SWi(&D_003DD490,0xDB),D_002C3288[i].name);
                return;
            }
            CopyId(pch,0xE3);
            return;
        }
        default:
            *pch=0;
            return;
        }
    }
}
} } // namespace M_BuildThiefnetInfoText

namespace M_func_001DCC58 { extern "C" {
extern char D_002D8468[];
struct S { int a, b, c; void *vt; char pad[0x10]; int n20; };
struct S *func_001DA728(struct S *p);
void ClearJobList(struct S *p);
/** @sly2-annot addr=0x1DCC58
 * Purpose: Screen subclass with vtable 0x2D8468, widget oid 0x4E0
 * Info: proposed name SCREEN4E0_ctor (T2); class SCREEN4E0; TU screen
 * Confidence: low (draft) [T2 0.30] - evidence: static analysis; sources: r6b_rest
 * See: docs/reference/functions/hud-ui.md#screen-0x1d93d8-0x1de2a0
 * @end-sly2-annot */
struct S *func_001DCC58(struct S *p) {
    func_001DA728(p);
    p->vt = D_002D8468;
    p->n20 = 0x4E0;
    ClearJobList(p);
    return p;
}
} } // namespace M_func_001DCC58

/** @sly2-annot addr=0x1DCCA0
 * Purpose: Job list (D_003DA3C0, widget 0x4E0) post-load: base PostScreenLoad, widget +0xB0=1
 * Info: proposed name PostJobListLoad (T1); TU screen
 * Confidence: mid (draft) [T1 0.55] - evidence: static analysis; sources: r9_bhv_screen
 * See: docs/reference/functions/hud-ui.md#screen-0x1d93d8-0x1de2a0
 * @end-sly2-annot */
extern "C" void PostJobListLoad(SCREEN *pscreen)
{
    func_001DA770(pscreen);
    if (pscreen->pchild != 0) {
        pscreen->pchild->fB0 = 1;
    }
}

/** @sly2-annot addr=0x1DCCD8
 * Purpose: Job list reset (slot 13)
 * Info: proposed name ResetJobList (T1); TU screen
 * Confidence: mid (draft) [T1 0.55] - evidence: static analysis; sources: r9_bhv_screen
 * See: docs/reference/functions/hud-ui.md#screen-0x1d93d8-0x1de2a0
 * @end-sly2-annot */
extern "C" void ResetJobList(SCREEN *pscreen)
{
    func_001DA7B0(pscreen);
    ClearJobList(pscreen);
}

namespace M_ClearJobList { extern "C" {
extern struct CLOCK { float a[16]; } D_002DDED0;
struct S { char p0[0x40]; int n40; float g44; char p1[0x11C-0x48]; int n11C, n120, n124; };
/** @sly2-annot addr=0x1DCD08
 * Purpose: Job list: state -1, timestamp, anim handles -1
 * Info: proposed name ClearJobList (T1); TU screen
 * Confidence: mid (draft) [T1 0.50] - evidence: static analysis; sources: r9_bhv_screen
 * See: docs/reference/functions/hud-ui.md#screen-0x1d93d8-0x1de2a0
 * @end-sly2-annot */
void ClearJobList(struct S *p) {
    p->n40 = -1; //P
    p->g44 = D_002DDED0.a[15]; //P
    p->n11C = -1; //P
    p->n120 = -1; //P
    p->n124 = -1; //P
}
} } // namespace M_ClearJobList

namespace M_UpdateJobList { extern "C" {
typedef struct VT { struct { short d; short pad; void (*pfn)(void *); } a[32]; } VT;
typedef struct { char pad[0xC]; VT *vt; char pad10[0x58 - 0x10]; int x58; } W;
typedef struct {
    char pad[0x10];
    void *x10;
    char pad14[0x24 - 0x14];
    W *pw;
    char pad28[0x40 - 0x28];
    int x40;
    char pad44[4];
    int c;
    char pad4c[0x114 - 0x4C];
    int x114;
} OBJ;
void func_001DA808(OBJ *);
int func_002127C8(void);
void SetWidgetState(W *, int);
void SetWidgetRequest(W *, int);
int GrfHandleJoyBtn(void *, int);
void SetJobListPage(OBJ *, int);
void UpdateScreenStickDir(OBJ *);
void UpdateJobListWidgets(OBJ *);
/** @sly2-annot addr=0x1DCD30
 * Purpose: Job list per-frame (slot 7): wait for widget load; START/circle close; direction buttons toggle
 *   jobs page <-> tasks page
 * Info: proposed name UpdateJobList (T1); TU screen
 * Confidence: mid (draft) [T1 0.60] - evidence: static analysis; sources: r9_bhv_screen
 * See: docs/reference/functions/hud-ui.md#screen-0x1d93d8-0x1de2a0
 * @end-sly2-annot */
void UpdateJobList(OBJ *p)
{
    func_001DA808(p);
    if (p->x40 == -1)
        return;
    if (p->x10 == 0)
        return;
    if (p->pw->x58 == 0) {
        if (!func_002127C8())
            return;
        SetWidgetState(p->pw, 1);
        SetWidgetRequest(p->pw, 2);
        p->pw->vt->a[16].pfn((char *)p->pw + p->pw->vt->a[16].d);
    }
    if (GrfHandleJoyBtn(p->x10, 0x800)) {
        SetJobListPage(p, -1);
        return;
    }
    switch (p->x40) {
    case 0:
        if (GrfHandleJoyBtn(p->x10, 0x10)) {
            SetJobListPage(p, -1);
            return;
        }
        if (p->c && GrfHandleJoyBtn(p->x10, 0xF0)) {
            SetJobListPage(p, 1);
            return;
        }
        break;
    case 1:
        if (!p->x114 && GrfHandleJoyBtn(p->x10, 0x10)) {
            SetJobListPage(p, -1);
            return;
        }
        if (GrfHandleJoyBtn(p->x10, 0xF0)) {
            SetJobListPage(p, 0);
            return;
        }
        break;
    }
    UpdateScreenStickDir(p);
    UpdateJobListWidgets(p);
}
} } // namespace M_UpdateJobList

namespace M_SelectJobListItem { extern "C" {
struct S { char p0[0x48]; int c; char p1[0x118-0x4C]; int i; };
void func_001DA9E8(struct S *p);
static inline int Clamp(int n, int lo, int hi) {
    if (n < lo) return lo;
    if (n > hi) return hi;
    return n;
}
/** @sly2-annot addr=0x1DCE90
 * Purpose: Select row i (clamped)
 * Info: proposed name SelectJobListItem (T1); TU screen
 * Confidence: mid (draft) [T1 0.50] - evidence: static analysis; sources: r9_bhv_screen
 * See: docs/reference/functions/hud-ui.md#screen-0x1d93d8-0x1de2a0
 * @end-sly2-annot */
void SelectJobListItem(struct S *p, int i) {
    int c = p->c;
    int n;
    if (c == 0) n = -1;
    else n = Clamp(i, 0, c - 1);
    p->i = n;
    func_001DA9E8(p);
}
} } // namespace M_SelectJobListItem

namespace M_RefreshJobListEntries {
struct SO {
    char pad0[0x54];
    int x54;
    char pad58[0x5C - 0x58];
    int x5C;
    char pad60[0x6C - 0x60];
    int x6C;
    char pad70[0x7C - 0x70];
    SO *x7C;
};
struct E { SO *o; int a; int b; };
struct OBJ {
    char pad0[0x24];
    void *x24;
    char pad28[0x40 - 0x28];
    int x40;
    char pad44[0x48 - 0x44];
    int n;
    E ae[0x10];
    int x10C;
    SO *x110;
    int x114;
    int x118;
};
extern "C" {
void StopWidgetAnim(void *, int);
void func_002333B4(void *, int, unsigned int);
void SortTasksTopologically(int *, SO ***);
void qsort(void *, int, int, int (*)(const void *, const void *));
int func_00165C28(const void *, const void *);
int func_00163430(SO *);
void memmove(void *, void *, unsigned int);
void SelectJobListItem(OBJ *, int);
}
/** @sly2-annot addr=0x1DCED8
 * Purpose: Sort tasks topologically then by CompareTasks; page 0 = jobs with state > 0, page 1 = tasks of the
 *   selected job up to the first unfinished one; keep the last 10/4
 * Info: proposed name RefreshJobListEntries (T1); TU screen
 * Confidence: mid (draft) [T1 0.60] - evidence: static analysis; sources: r9_bhv_screen
 * See: docs/reference/functions/hud-ui.md#screen-0x1d93d8-0x1de2a0
 * @end-sly2-annot */
extern "C" void RefreshJobListEntries(OBJ *p)
{
    for (int i = 0; i < p->n; i++) {
        E *e = &p->ae[i];
        if (e->a != -1) {
            StopWidgetAnim(p->x24, e->a);
            e->a = -1;
        }
        if (e->b != -1) {
            StopWidgetAnim(p->x24, e->b);
            e->b = -1;
        }
    }
    p->x118 = 0;
    p->n = 0;
    func_002333B4(p->ae, 0, p->n * sizeof(E));
    STACKSCOPE frame;
    int c;
    SO **aso;
    SortTasksTopologically(&c, &aso);
    qsort(aso, c, 4, func_00165C28);
    switch (p->x40) {
    case 0: {
        SO *last = 0;
        for (int i = 0; i < c; i++) {
            SO *so = aso[i]->x7C;
            if (so && last != so) {
                last = so;
                if (func_00163430(so) > 0 && so->x6C != -1) {
                    E *e = &p->ae[p->n++];
                    e->o = so;
                    e->b = -1;
                    e->a = -1;
                    if (p->x110 == so)
                        p->x118 = e - p->ae;
                }
            }
        }
        break;
    }
    case 1: {
        int flag = 0;
        for (int i = 0; i < c; i++) {
            SO *so = aso[i];
            if (p->x110 == so->x7C && so->x5C != -1) {
                if (flag && !so->x54)
                    break;
                E *e = &p->ae[p->n++];
                e->o = so;
                e->b = -1;
                e->a = -1;
                if (so->x54 == 1)
                    flag = 1;
                if (!so->x54)
                    break;
            }
        }
        break;
    }
    }
    if (p->n > p->x10C) {
        memmove(p->ae, &p->ae[p->n - p->x10C], p->x10C * sizeof(E));
        int d = p->n - p->x10C;
        if (d < p->x118)
            p->x118 -= d;
        else
            p->x118 = 0;
        p->n = p->x10C;
    }
    SelectJobListItem(p, p->x118);
}
} // namespace M_RefreshJobListEntries

namespace M_UpdateJobListWidgets { extern "C" {
typedef struct { char pad[0x58]; int x58; } W;
typedef struct { void *p; int a; int b; } ENT;
typedef struct {
    char pad[0x24];
    W *pw;
    char pad28[0x40 - 0x28];
    int kind;
    char pad44[4];
    int c;
    ENT ae[17];
    int sel;
} OBJ;
void SwitchWidgetAnim(W *, int, int *);
int FJobComplete(void *);
int FTaskComplete(void *);
/** @sly2-annot addr=0x1DD1F8
 * Purpose: Row highlight anims (0x4FD+i selected / 0x4E9+i) and completed checkmarks (0x507+i) via
 *   FJobComplete/FTaskComplete
 * Info: proposed name UpdateJobListWidgets (T1); TU screen
 * Confidence: mid (draft) [T1 0.55] - evidence: static analysis; sources: r9_bhv_screen
 * See: docs/reference/functions/hud-ui.md#screen-0x1d93d8-0x1de2a0
 * @end-sly2-annot */
void UpdateJobListWidgets(OBJ *p)
{
    int i;
    if (p->pw->x58 != 2)
        return;
    for (i = 0; i < p->c; i++) {
        int f;
        ENT *pe = &p->ae[i];
        SwitchWidgetAnim(p->pw, i == p->sel ? i + 0x4FD : i + 0x4E9, &pe->a);
        f = 0;
        switch (p->kind) {
        case 0:
            f = FJobComplete(pe->p);
            break;
        case 1:
            f = FTaskComplete(pe->p);
            break;
        }
        SwitchWidgetAnim(p->pw, f ? i + 0x507 : -1, &pe->b);
    }
}
} } // namespace M_UpdateJobListWidgets

namespace M_SetJobListPage { extern "C" {
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
} } // namespace M_SetJobListPage
#undef VCALL

namespace M_SetJobListStickDir { extern "C" {
extern struct CLOCK { float a[16]; } D_002DDED0;
typedef struct {
    char pad[0x28];
    int state;
    float t;
    char pad30[0x48 - 0x30];
    int c;
    char pad4c[0x118 - 0x4C];
    int i;
} OBJ;
void SetJobListPage(OBJ *, int);
void SelectJobListItem(OBJ *, int);
void *PsndStartSound(int);
void SetSndSourceLo(void *, int);
/** @sly2-annot addr=0x1DD558
 * Purpose: Job list slot 14: 0 -> tasks page, 4 -> jobs page, up/down rows
 * Info: proposed name SetJobListStickDir (T1); TU screen
 * Confidence: mid (draft) [T1 0.55] - evidence: static analysis; sources: r9_bhv_screen
 * See: docs/reference/functions/hud-ui.md#screen-0x1d93d8-0x1de2a0
 * @end-sly2-annot */
void SetJobListStickDir(OBJ *p, unsigned int n)
{
    if (n == p->state)
        return;
    p->state = n;
    p->t = D_002DDED0.a[15];
    switch (n) {
    case 0:
        if (p->c)
            SetJobListPage(p, 1);
        break;
    case 4:
        SetJobListPage(p, 0);
        break;
    case 1:
    case 2:
    case 3:
        if (p->i > 0) {
            SelectJobListItem(p, p->i - 1);
            SetSndSourceLo(PsndStartSound(0x35), 0);
        }
        break;
    case 5:
    case 6:
    case 7:
        if (p->i < p->c - 1) {
            SelectJobListItem(p, p->i + 1);
            SetSndSourceLo(PsndStartSound(0x35), 0);
        }
        break;
    }
}
} } // namespace M_SetJobListStickDir

namespace M_func_001DD648 { extern "C" {
extern char D_002D83E8[];
struct S { int a, b, c; void *vt; char pad[0x10]; int n20; };
struct S *func_001DA728(struct S *p);
void func_001DA7D0(struct S *p);
/** @sly2-annot addr=0x1DD648
 * Purpose: Screen subclass with vtable 0x2D83E8, widget oid 0x52D
 * Info: proposed name SCREEN52D_ctor (T2); class SCREEN52D; TU screen
 * Confidence: low (draft) [T2 0.30] - evidence: static analysis; sources: r6b_rest
 * See: docs/reference/functions/hud-ui.md#screen-0x1d93d8-0x1de2a0
 * @end-sly2-annot */
struct S *func_001DD648(struct S *p) {
    func_001DA728(p);
    p->vt = D_002D83E8;
    p->n20 = 0x52D;
    func_001DA7D0(p);
    return p;
}
} } // namespace M_func_001DD648

/** @sly2-annot addr=0x1DD690
 * Purpose: Screen 0x52D (D_003DA4F0) post-load: base PostScreenLoad, widget +0xB0=1
 * Info: proposed name PostScreen52DLoad (T2); TU screen
 * Confidence: low (draft) [T2 0.40] - evidence: static analysis; sources: r9_bhv_screen
 * See: docs/reference/functions/hud-ui.md#screen-0x1d93d8-0x1de2a0
 * @end-sly2-annot */
extern "C" void func_001DD690(SCREEN *pscreen)
{
    func_001DA770(pscreen);
    if (pscreen->pchild != 0) {
        pscreen->pchild->fB0 = 1;
    }
}

namespace M_func_001DD6C8 { extern "C" {
struct VTE { short d; short pad; void (*pfn)(...); };
struct O { int a, b, c; struct VTE *vt; };
struct S { int a, b, c; struct VTE *vt; char pad[0x14]; struct O *po; };
/** @sly2-annot addr=0x1DD6C8
 * Purpose: Show screen 0x52D (vt10) and its widget (vt49 -1)
 * Info: proposed name ShowScreen52D (T2); TU screen
 * Confidence: low (draft) [T2 0.35] - evidence: static analysis; sources: r9_bhv_screen
 * See: docs/reference/functions/hud-ui.md#screen-0x1d93d8-0x1de2a0
 * @end-sly2-annot */
void func_001DD6C8(struct S *p) {
    if (p->po) {
        {
            struct VTE *e = &p->vt[10];
            ((void (*)(void *))e->pfn)((char *)p + e->d);
        }
        {
            struct O *po = p->po;
            struct VTE *e = &po->vt[49];
            ((void (*)(void *, int))e->pfn)((char *)po + e->d, -1);
        }
    }
}
} } // namespace M_func_001DD6C8

namespace M_func_001DD728 { extern "C" {
struct VTE { short d; short pad; void (*pfn)(...); };
struct O { int a, b, c; struct VTE *vt; };
struct S { int a, b, c; struct VTE *vt; char pad[0x14]; struct O *po; };
/** @sly2-annot addr=0x1DD728
 * Purpose: Hide screen 0x52D (vt11) and its widget (vt50)
 * Info: proposed name HideScreen52D (T2); TU screen
 * Confidence: low (draft) [T2 0.35] - evidence: static analysis; sources: r9_bhv_screen
 * See: docs/reference/functions/hud-ui.md#screen-0x1d93d8-0x1de2a0
 * @end-sly2-annot */
void func_001DD728(struct S *p) {
    if (p->po) {
        {
            struct VTE *e = &p->vt[11];
            ((void (*)(void *))e->pfn)((char *)p + e->d);
        }
        {
            struct O *po = p->po;
            struct VTE *e = &po->vt[50];
            ((void (*)(void *))e->pfn)((char *)po + e->d);
        }
    }
}
} } // namespace M_func_001DD728

namespace M_func_001DD780 { extern "C" {
struct O { char pad[0x58]; int n58; char p1[8]; int n64; };
struct S { char pad[0x24]; struct O *po; };
/** @sly2-annot addr=0x1DD780
 * Purpose: Widget fully shown (+0x58==2 and +0x64==2)
 * Info: proposed name FScreen52DShown (T2); TU screen
 * Notes: verifier PLAUSIBLE: '52D' is an opaque placeholder. Returns widget(+0x24)->+0x58 == 2 && +0x64 == 2;
 *   only caller UpdateCd (disc-check state 1) on screen D_003DA4F0. | T2 forced: verifier: placeholder name
 * Confidence: low (draft) [T2 0.35] - evidence: static analysis; sources: r9_bhv_screen+verdicts_followups
 * See: docs/engine/hud-ui.md#5-key-functions, docs/reference/functions/hud-ui.md#screen-0x1d93d8-0x1de2a0
 * @end-sly2-annot */
int func_001DD780(struct S *p) {
    struct O *po = p->po;
    if (!po) return 0;
    if (po->n58 != 2) return 0;
    return po->n64 == 2;
}
} } // namespace M_func_001DD780

/** @sly2-annot addr=0x1DD7B8
 * Purpose: slot 14 of 0x2D83E8: if state(+0x28) changes, store it and the time
 * Info: proposed name SetScreenState (T2); TU screen
 * Confidence: low (draft) [T2 0.35] - evidence: static analysis; sources: r6b_rest
 * See: docs/reference/functions/hud-ui.md#screen-0x1d93d8-0x1de2a0
 * @end-sly2-annot */
/* slot 14: set the mode (stamping the real time). */
extern "C" void func_001DD7B8(SCREEN *pscreen, int nMode)
{
    if (nMode != pscreen->nMode) {
        pscreen->nMode = nMode;
        pscreen->tMode = D_002DDED0.tReal;
    }
}

namespace M_func_001DD7D8 { extern "C" {
extern char D_002D8368[];
struct S { int a, b, c; void *vt; char pad[0x10]; int n20; };
struct S *func_001DA728(struct S *p);
void func_001DA7D0(struct S *p);
/** @sly2-annot addr=0x1DD7D8
 * Purpose: Screen subclass with vtable 0x2D8368, widget oid 0x5D2
 * Info: proposed name SCREEN5D2_ctor (T2); class SCREEN5D2; TU screen
 * Confidence: low (draft) [T2 0.30] - evidence: static analysis; sources: r6b_rest
 * See: docs/reference/functions/hud-ui.md#screen-0x1d93d8-0x1de2a0
 * @end-sly2-annot */
struct S *func_001DD7D8(struct S *p) {
    func_001DA728(p);
    p->vt = D_002D8368;
    p->n20 = 0x5D2;
    func_001DA7D0(p);
    return p;
}
} } // namespace M_func_001DD7D8

namespace M_func_001DD820 { extern "C" {
struct VTE { short d; short pad; void (*pfn)(...); };
struct O { int a, b, c; struct VTE *vt; };
struct S { int a, b, c; struct VTE *vt; char pad[0x14]; struct O *po; };
void EnsureRumbleModeDefault(void);
/** @sly2-annot addr=0x1DD820
 * Purpose: Show the title screen (D_003DA530, widget 0x5D2) and its widget; reset rumble
 * Info: proposed name ShowTitleScreen (T2); TU screen
 * Confidence: low (draft) [T2 0.45] - evidence: static analysis; sources: r9_bhv_screen
 * See: docs/reference/functions/hud-ui.md#screen-0x1d93d8-0x1de2a0
 * @end-sly2-annot */
void func_001DD820(struct S *p) {
    if (p->po) {
        {
            struct VTE *e = &p->vt[10];
            ((void (*)(void *))e->pfn)((char *)p + e->d);
        }
        {
            struct O *po = p->po;
            struct VTE *e = &po->vt[49];
            ((void (*)(void *, int))e->pfn)((char *)po + e->d, -1);
        }
        EnsureRumbleModeDefault();
    }
}
} } // namespace M_func_001DD820

namespace M_HideTitleScreen { extern "C" {
struct VTE { short d; short pad; void (*pfn)(...); };
struct O { int a, b, c; struct VTE *vt; };
struct S { int a, b, c; struct VTE *vt; char pad[0x14]; struct O *po; };
/** @sly2-annot addr=0x1DD888
 * Purpose: Hide the title screen and its widget
 * Info: proposed name HideTitleScreen (T1); TU screen
 * Confidence: mid (draft) [T1 0.50] - evidence: static analysis; sources: r9_bhv_screen
 * See: docs/reference/functions/hud-ui.md#screen-0x1d93d8-0x1de2a0
 * @end-sly2-annot */
void HideTitleScreen(struct S *p) {
    if (p->po) {
        {
            struct VTE *e = &p->vt[11];
            ((void (*)(void *))e->pfn)((char *)p + e->d);
        }
        {
            struct O *po = p->po;
            struct VTE *e = &po->vt[50];
            ((void (*)(void *))e->pfn)((char *)po + e->d);
        }
    }
}
} } // namespace M_HideTitleScreen

namespace M_SelectLanguageItem { extern "C" {
struct S { char p0[8]; void *p8; };
extern char D_003DD490[];
void SetLanguage(void *p);
void WipeToWorldWarp(void *g, int a, int b, int c, int d, int e);
/** @sly2-annot addr=0x1DD8E0
 * Purpose: Language menu item callback: SetLanguage(item) and warp to world 0x189
 * Info: proposed name SelectLanguageItem (T1); TU screen
 * Confidence: mid (draft) [T1 0.55] - evidence: static analysis; sources: r9_bhv_screen
 * See: docs/reference/functions/hud-ui.md#screen-0x1d93d8-0x1de2a0
 * @end-sly2-annot */
void SelectLanguageItem(struct S *p) {
    SetLanguage(p->p8);
    WipeToWorldWarp(D_003DD490, 0, 0x189, -1, 1, -1);
}
} } // namespace M_SelectLanguageItem

namespace M_UpdateTitleScreen { extern "C" {
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
} } // namespace M_UpdateTitleScreen

namespace M_StartNewGame {
extern "C" { void *func_001291A8(void *, const void *); void Decref__4CRef(void *); }
class CRef {
public:
    int m_tagk;
    int m_tag;
    CRef(int tagk, int n) { m_tagk = tagk; m_tag = n; }
    CRef(const CRef &ref) { func_001291A8(this, &ref); }
    ~CRef() { Decref__4CRef(this); }
};
struct G;
extern "C" { extern G D_003D4A60; }
extern "C" { extern void *D_003E0FAC; }
extern "C" { extern void *D_002E1E40; }
extern "C" { extern void *D_002DD5BC; }
extern "C" { extern void *D_002F7900; }
extern "C" {
CRef RefSetGlobalSaveVar(void *, const CRef &, G *);
void BindStepplayerHpMp(void *, int);
void func_00165E38(G *);
}
/* The unused third parameter is deliberate: it is the same SetPv form as func_0013D328 / func_00143AF0,
   and the dummy makes gcc keep the %hi/%lo of D_003D4A60 in s0 across the whole function. */
static inline void SetPv(void *pv, const CRef &r, void *) { CRef arg(r); const CRef &ret = RefSetGlobalSaveVar(pv, arg, &D_003D4A60); }
/** @sly2-annot addr=0x1DDB08
 * Purpose: Clear the save flag at D_003E0FAC, bind hp/power of the three heroes, PlayGameFromGs(g_gs)
 * Info: proposed name StartNewGame (T1); TU screen
 * Confidence: mid (draft) [T1 0.55] - evidence: static analysis; sources: r9_bhv_screen
 * See: docs/reference/functions/hud-ui.md#screen-0x1d93d8-0x1de2a0
 * @end-sly2-annot */
extern "C" void StartNewGame(void)
{
    {
        CRef ref(0x16, 0);
        SetPv(D_003E0FAC, ref, &D_003D4A60);
    }
    if (D_002E1E40)
        BindStepplayerHpMp(D_002E1E40, 1);
    if (D_002DD5BC)
        BindStepplayerHpMp(D_002DD5BC, 1);
    if (D_002F7900)
        BindStepplayerHpMp(D_002F7900, 1);
    func_00165E38(&D_003D4A60);
}
} // namespace M_StartNewGame

/** @sly2-annot addr=0x1DDBD0
 * Purpose: Title screen slot 14: store direction and time
 * Info: proposed name SetTitleScreenStickDir (T2); TU screen
 * Confidence: low (draft) [T2 0.40] - evidence: static analysis; sources: r9_bhv_screen
 * See: docs/reference/functions/hud-ui.md#screen-0x1d93d8-0x1de2a0
 * @end-sly2-annot */
/* slot 14: set the mode (stamping the real time). */
extern "C" void func_001DDBD0(SCREEN *pscreen, int nMode)
{
    if (nMode != pscreen->nMode) {
        pscreen->nMode = nMode;
        pscreen->tMode = D_002DDED0.tReal;
    }
}

namespace M_BuildTitlePromptText {
struct CRef {
    int m_tagk;
    union { int m_tag; float m_g; };
    CRef() { m_tagk = -1; }
    CRef(const CRef &);
    ~CRef() { Decref(); }
    void Decref() __asm__("Decref__4CRef");
};
struct G { char pad[0x3B1C]; void *x3B1C; };
struct H { char pad[0x100C]; int x100C; int x1010; };
struct I { char pad[0xFD54]; void *xFD54; };
extern "C" { extern G D_003DD490; }
extern "C" { extern H D_002DFC00; }
extern "C" { extern I D_002E65E0; }
extern "C" { extern char D_002A7A70[]; }
extern "C" char *strcpy(char *, const char *);
extern "C" char *strcpy1(char *, const char *);
extern "C" int CLanguagesRegion(void);
extern "C" { char *PchzFindSwLangstr__FP2SWi(G *, int); }
extern "C" { CRef RefGetGlobalSaveVar(void *, void *); }
/** @sly2-annot addr=0x1DDBF0
 * Purpose: Text callback 0x5D3: title prompt (continue vs new game, options, language) from string ids
 *   5/6/8/0x56
 * Info: proposed name BuildTitlePromptText (T1); TU screen
 * Confidence: high [T1 0.85] - evidence: static analysis; sources: r9_bhv_screen+verdicts_followups
 * See: docs/engine/hud-ui.md#5-key-functions, docs/reference/functions/hud-ui.md#screen-0x1d93d8-0x1de2a0
 * @end-sly2-annot */
extern "C" void BuildTitlePromptText(char *pch)
{
    float g;
    void *pv;
    if (D_002DFC00.x100C == 1 && D_002DFC00.x1010 == 0) {
        strcpy(pch, PchzFindSwLangstr__FP2SWi(&D_003DD490, 9));
        return;
    }
    g = 0.0f;
    pv = D_002E65E0.xFD54;
    if (pv) {
        CRef ref = RefGetGlobalSaveVar(D_003DD490.x3B1C, pv);
        g = ref.m_g;
    }
    {
        int ids = g != 0.0f ? 6 : 5;
        strcpy(pch, PchzFindSwLangstr__FP2SWi(&D_003DD490, ids));
    }
    strcpy1(pch, D_002A7A70);
    strcpy1(pch, PchzFindSwLangstr__FP2SWi(&D_003DD490, 8));
    if (CLanguagesRegion() >= 2) {
        strcpy1(pch, D_002A7A70);
        strcpy1(pch, PchzFindSwLangstr__FP2SWi(&D_003DD490, 0x56));
    }
}
} // namespace M_BuildTitlePromptText

namespace M_IdsFromWorld { extern "C" {
/** @sly2-annot addr=0x1DDD40
 * Purpose: World index 0-8 -> episode name string id 0x6D-0x75
 * Info: proposed name IdsFromWorld (T1); TU screen
 * Confidence: mid (draft) [T1 0.55] - evidence: static analysis; sources: r9_bhv_screen
 * See: docs/reference/functions/hud-ui.md#screen-0x1d93d8-0x1de2a0
 * @end-sly2-annot */
int IdsFromWorld(int n) {
    int r = -1;
    switch (n) {
    case 0: r = 0x6D; break;
    case 1: r = 0x6E; break;
    case 2: r = 0x6F; break;
    case 3: r = 0x70; break;
    case 4: r = 0x71; break;
    case 5: r = 0x72; break;
    case 6: r = 0x73; break;
    case 7: r = 0x74; break;
    case 8: r = 0x75; break;
    }
    return r;
}
} } // namespace M_IdsFromWorld

/** @sly2-annot addr=0x1DDDB8
 * Purpose: Text callback 0x51B: current episode name
 * Info: proposed name BuildJobListWorldText (T1); TU screen
 * Confidence: high [T1 0.85] - evidence: static analysis; sources: r9_bhv_screen+verdicts_followups
 * See: docs/engine/hud-ui.md#5-key-functions, docs/reference/functions/hud-ui.md#screen-0x1d93d8-0x1de2a0
 * @end-sly2-annot */
extern "C" void BuildJobListWorldText(char *pch)
{
    StrcpyCall(pch, PchzFindSwLangstr(&D_003DD490, IdsFromWorld(NWorldCur())));
}

/** @sly2-annot addr=0x1DDE10
 * Purpose: Text callback 0x51D: selected job string +0x70
 * Info: proposed name BuildJobListJobDescText (T2); TU screen
 * Confidence: low (draft) [T2 0.40] - evidence: static analysis; sources: r9_bhv_screen
 * See: docs/reference/functions/hud-ui.md#screen-0x1d93d8-0x1de2a0
 * @end-sly2-annot */
extern "C" void func_001DDE10(char *pch)
{
    if (D_003DA4D0 != 0) {
        StrcpyCall(pch, PchzFindSwLangstr(&D_003DD490, D_003DA4D0->ids70));
    }
}

/** @sly2-annot addr=0x1DDE58
 * Purpose: Text callback 0x51C: selected job string +0x6C
 * Info: proposed name BuildJobListJobNameText (T2); TU screen
 * Confidence: low (draft) [T2 0.40] - evidence: static analysis; sources: r9_bhv_screen
 * See: docs/reference/functions/hud-ui.md#screen-0x1d93d8-0x1de2a0
 * @end-sly2-annot */
extern "C" void func_001DDE58(char *pch)
{
    if (D_003DA4D0 != 0) {
        StrcpyCall(pch, PchzFindSwLangstr(&D_003DD490, D_003DA4D0->ids6C));
    }
}

namespace M_BuildJobListRowText { extern "C" {
struct O { char pad[0x6C]; int n; };
struct E { struct O *po; int a, b; };
struct G2 { char pad[0x48]; int c; struct E ae[1]; };
extern struct G2 D_003DA3C0;
extern struct G D_003DD490;
void *PchzFindSwLangstr__FP2SWi(struct G *g, int n);
void strcpy(void *p, void *q);
/** @sly2-annot addr=0x1DDEA0
 * Purpose: Text callback rows 0x51E-0x527: job name of row i
 * Info: proposed name BuildJobListRowText (T1); TU screen
 * Confidence: mid (draft) [T1 0.50] - evidence: static analysis; sources: r9_bhv_screen
 * See: docs/reference/functions/hud-ui.md#screen-0x1d93d8-0x1de2a0
 * @end-sly2-annot */
void BuildJobListRowText(void *p, int i) {
    struct G2 *pg = &D_003DA3C0;
    if (i < pg->c) strcpy(p, PchzFindSwLangstr__FP2SWi(&D_003DD490, pg->ae[i].po->n));
}
} } // namespace M_BuildJobListRowText

namespace M_BuildJobListTaskRowText { extern "C" {
struct O { char pad[0x5C]; int n; };
struct E { struct O *po; int a, b; };
struct G2 { char pad[0x48]; int c; struct E ae[1]; };
extern struct G2 D_003DA3C0;
extern struct G D_003DD490;
void *PchzFindSwLangstr__FP2SWi(struct G *g, int n);
void strcpy(void *p, void *q);
/** @sly2-annot addr=0x1DDF00
 * Purpose: Text callback rows 0x528-0x52B: task name of row i
 * Info: proposed name BuildJobListTaskRowText (T1); TU screen
 * Confidence: mid (draft) [T1 0.50] - evidence: static analysis; sources: r9_bhv_screen
 * See: docs/reference/functions/hud-ui.md#screen-0x1d93d8-0x1de2a0
 * @end-sly2-annot */
void BuildJobListTaskRowText(void *p, int i) {
    struct G2 *pg = &D_003DA3C0;
    if (i < pg->c) strcpy(p, PchzFindSwLangstr__FP2SWi(&D_003DD490, pg->ae[i].po->n));
}
} } // namespace M_BuildJobListTaskRowText

/** @sly2-annot addr=0x1DDF60
 * Purpose: Text callback 0x5D7: string 0x40 plus a button-glyph suffix
 * Info: proposed name BuildTitleText5D7 (T2); TU screen
 * Confidence: low (draft) [T2 0.30] - evidence: static analysis; sources: r9_bhv_screen
 * See: docs/reference/functions/hud-ui.md#screen-0x1d93d8-0x1de2a0
 * @end-sly2-annot */
extern "C" void func_001DDF60(char *pch)
{
    StrcpyCall(pch, PchzFindSwLangstr(&D_003DD490, 0x40));
    strcpy1(pch, D_002A7AA8);
}

/** @sly2-annot addr=0x1DDFB0
 * Purpose: Text callback 0x5D5: "%d%%" game completion
 * Info: proposed name BuildTitleCompletionText (T1); TU screen
 * Confidence: mid (draft) [T1 0.50] - evidence: static analysis; sources: r9_bhv_screen
 * See: docs/reference/functions/hud-ui.md#screen-0x1d93d8-0x1de2a0
 * @end-sly2-annot */
extern "C" void BuildTitleCompletionText(char *pch)
{
    sprintf(pch, D_002A7AB8, NCompletionPercentGs(D_003E0EA8, D_003D4A60));
}


namespace M_func_00258348 { extern "C" {
typedef struct R1 { int cref; int x4; int x8; int xC[2]; int x14[1]; } R1;
typedef struct R2 { int cref; } R2;
typedef struct R4 { int cref; int x4; R1 *p8; } R4;
extern char D_004520D0[], D_00452148[];
void func_00257250(void *, int) __asm__("_$_8CVarbPtr");
void Decref__4CRef(void *);
void GcBinding_Unlink(void *);
void GcSidebag_Unlink(void *);
void func_0011CD98(void *, void *);
void func_00257078(void *, int) __asm__("_$_6CFrame");
void func_001AE520(void *);

static inline void Release1(R1 *r)
{
    if (r) {
        if (--r->cref == 0) {
            func_00257250(r->x14, 2);
            Decref__4CRef(r->xC);
            GcBinding_Unlink(r);
            func_0011CD98(D_004520D0, r);
        }
    }
}

static inline void Release4(R4 *r)
{
    if (r) {
        if (--r->cref == 0) {
            Release1(r->p8);
            GcSidebag_Unlink(r);
            func_0011CD98(D_00452148, r);
        }
    }
}

typedef struct S { R2 *p0; R4 *p4; } S;

/** @sly2-annot addr=0x258348
 * Purpose: base destructor: release binding/sidebag refs [game linkonce (inline/template)]
 * Info: proposed name BASIC_dtor (T2); TU game_linkonce
 * Confidence: low (draft) [T2 0.30] - evidence: static analysis; sources: sdk_names
 * See: docs/reference/functions/core-library.md#game_linkonce-0x254f00-0x259de4,
 *   docs/reference/functions/sound.md#sound-0x1faf70-0x1ff3b8
 * @end-sly2-annot */
void func_00258348(S *p, int flags)
{
    Release4(p->p4);
    {
        R2 *r = p->p0;
        if (r) {
            if (--r->cref == 0)
                func_00257078(r, 3);
        }
    }
    if (flags & 1)
        func_001AE520(p);
}
} } // namespace M_func_00258348

// ---- Data owned by this TU, in address order (docs/guides/tu-merging.md "Data"). Defined after the functions
// so no function sees a definition (keeps the code as matched); the namespaced declarations above refer to these.
extern const float D_002A7984 = 3.40282347e+38f;
extern const char D_002A79A0[] = "%d";
extern const char D_002A79A8[] = "powerUps";
extern const char D_002A79B8[] = "mppukfAcquired";
extern const float D_002A79C8 = 3.40282347e+38f;
extern const float D_002A79CC = 3.40282347e+38f;
extern const char D_002A79D0[] = "loot";
extern const char D_002A79D8[] = "mplootkc";
extern const char D_002A7A10[] = "";
extern const char D_002A7A18[] = "%s%s%s. %s";
extern const char D_002A7A28[] = ": ";
extern const char D_002A7A30[] = "%s (%d)";
extern const char D_002A7A38[] = "%s: %s. %s";
extern const char D_002A7A70[] = "\n";
extern const char D_002A7AA8[] = " : &2T&.";
extern const char D_002A7AB8[] = "%d%%";
/* Gadget menu entries (power-up id, three widget oids, -1, x, y, flag); 18/9/8 entries for Sly, Bentley, Murray. */
struct GADGETITEM { int key; int oid0, oid1, oid2; int n; float x, y; int f; };
GADGETITEM D_002C2DE8[18] = {{23, 978, 977, 979, -1, 145.0f, 155.0f, 1}, {24, 983, 982, 984, -1, 225.0f, 155.0f, 1},
    {25, 988, 987, 989, -1, 305.0f, 155.0f, 1}, {26, 993, 992, 994, -1, 385.0f, 155.0f, 1},
    {29, 1008, 1007, 1009, -1, 465.0f, 155.0f, 1}, {30, 1013, 1012, 1014, -1, 545.0f, 155.0f, 1},
    {33, 1028, 1027, 1029, -1, 145.0f, 235.0f, 1}, {34, 1033, 1032, 1034, -1, 225.0f, 235.0f, 1},
    {35, 1038, 1037, 1039, -1, 305.0f, 235.0f, 1}, {37, 1048, 1047, 1049, -1, 385.0f, 235.0f, 1},
    {38, 1053, 1052, 1054, -1, 465.0f, 235.0f, 1}, {39, 1058, 1057, 1059, -1, 545.0f, 235.0f, 1},
    {27, 998, 997, 999, -1, 145.0f, 315.0f, 0}, {28, 1003, 1002, 1004, -1, 225.0f, 315.0f, 0},
    {40, 1063, 1062, 1064, -1, 305.0f, 315.0f, 1}, {41, 1068, 1067, 1069, -1, 385.0f, 315.0f, 1},
    {42, 1073, 1072, 1074, -1, 465.0f, 315.0f, 1}, {31, 1018, 1017, 1019, -1, 545.0f, 315.0f, 1}};
GADGETITEM D_002C3028[9] = {{7, 893, 892, 894, -1, 175.0f, 155.0f, 1}, {9, 903, 902, 904, -1, 285.0f, 155.0f, 1},
    {11, 913, 912, 914, -1, 395.0f, 155.0f, 1}, {13, 923, 922, 924, -1, 505.0f, 155.0f, 1},
    {10, 908, 907, 909, -1, 175.0f, 235.0f, 1}, {12, 918, 917, 919, -1, 285.0f, 235.0f, 1},
    {8, 898, 897, 899, -1, 395.0f, 235.0f, 1}, {14, 928, 927, 929, -1, 505.0f, 235.0f, 1},
    {36, 1043, 1042, 1044, -1, 175.0f, 315.0f, 0}};
GADGETITEM D_002C3148[8] = {{15, 938, 937, 939, -1, 175.0f, 155.0f, 1}, {22, 973, 972, 974, -1, 285.0f, 155.0f, 1},
    {20, 963, 962, 964, -1, 395.0f, 155.0f, 1}, {19, 958, 957, 959, -1, 505.0f, 155.0f, 1},
    {16, 943, 942, 944, -1, 285.0f, 235.0f, 1}, {21, 968, 967, 969, -1, 395.0f, 235.0f, 1},
    {17, 948, 947, 949, -1, 285.0f, 315.0f, 0}, {18, 953, 952, 954, -1, 395.0f, 315.0f, 0}};
SMP D_002C3248 = {100.0f, 0.0f, 0.5f};
float D_002C3254 = 5.0f;
int D_002C3258[3] = {4, 1, 2};
float D_002C3264 = 0.66f;
float D_002C3274 = 750.0f;
float D_002C3278 = 40.0f;
float D_002C327C = 200.0f;
float D_002C3280 = 1.0f;
int D_002C3288[64][3] = {{11, 228, 1114}, {19, 229, 1115}, {27, 230, 1116}, {17, 231, 1117}, {40, 232, 1118},
    {60, 233, 1119}, {14, 234, 1120}, {31, 235, 1121}, {44, 236, 1122}, {22, 237, 1123}, {49, 238, 1124},
    {71, 239, 1125}, {29, 240, 1126}, {54, 241, 1127}, {84, 242, 1128}, {32, 243, 1129}, {62, 244, 1130},
    {97, 245, 1131}, {41, 246, 1132}, {79, 247, 1133}, {120, 248, 1134}, {39, 249, 1135}, {81, 250, 1136},
    {122, 251, 1137}, {45, 252, 1138}, {91, 253, 1139}, {137, 254, 1140}, {42, 255, 1141}, {84, 256, 1142},
    {127, 257, 1143}, {0, 258, 1150}, {0, 259, 1151}, {0, 260, 1152}, {0, 261, 1147}, {0, 262, 1148}, {0, 263, 1149},
    {178, 264, 1144}, {183, 265, 1145}, {232, 266, 1146}, {153, 267, 1153}, {205, 268, 1154}, {246, 269, 1155},
    {249, 270, 1156}, {307, 271, 1157}, {354, 272, 1158}, {352, 273, 1159}, {402, 274, 1160}, {447, 275, 1161},
    {451, 276, 1162}, {504, 277, 1163}, {553, 278, 1164}, {546, 279, 1165}, {601, 280, 1166}, {652, 281, 1167},
    {605, 282, 1168}, {654, 283, 1169}, {704, 284, 1170}, {647, 285, 1171}, {701, 286, 1172}, {748, 287, 1173},
    {699, 288, 1174}, {752, 289, 1175}, {803, 290, 1176}, {726, 291, 1177}};
int D_002C3588[10] = {757, 758, 759, 760, 761, 762, 763, 764, 765, 766};
float D_002C35B0 = 40.0f;
int D_002C35B4 = 6;
int D_002C35B8[14] = {88, 89, 90, 91, 92, 93, 94, 95, 96, 97, 98, 99, 100, 101};
int D_002C35F0[8] = {790, 792, 791, 793, 795, 794, 796, 797};
JOBSEL *D_003DA4D0;
int D_003DA570;
namespace M_UpdateTitleScreen { extern "C" { ITEM D_003DA578[14]; } }   /* one per region language */
