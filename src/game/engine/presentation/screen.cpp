// screen.cpp -- the UI screens (SCREEN widgets: HUD meter, pause menu, notices, gadget menu, thiefnet, job list,
// title...): the shown-screen list, post-load and draw of the global screens, screen modes, the gadget menu and its
// items, the job list texts and the title texts. TU 0x1D93D8..0x1DE2A0 (98 functions with the linkonce copy at
// 0x258348, address order).
// Entry points: PostLoadScreens / ResetScreens (level load and teardown), UpdateScreens and DrawUi (per frame, from
// the main loop), ShowScreen / HideScreen, PostNotice / CancelNotice, and the menu open functions (SetGadgetMenuState,
// SetThiefnetTab, SetJobListPage, ShowTitleScreen); the Build*Text functions are text-table callbacks.
//
// Types: TU-private views for the m2c bodies (SCREEN: vtable +0xC, text ids +0x18/+0x20, widget oid +0x20 / object
// +0x24, mode +0x28 and when it was set +0x2C; the gadget menu and its 0x20-byte items). The hand-written
// per-function files keep their views in namespaces named after each function (as jt.cpp does), their C++-named
// definitions as C-linkage definitions of their literal symbols there.
// The nine global screen objects are defined here (the shown-screen list, then eight screens with inline
// constructors and the linkonce destructor DestroyBasic, in-charge flag 2): g++ generates the 150-instruction
// construct-and-destruct StaticInit_screen and the _GLOBAL_$D$ stub func_001DE280 (cm.cpp's g_cm pattern).
// SLY2_ALIAS: __static_initialization_and_destruction_0=StaticInit_screen
// SLY2_ALIAS: _GLOBAL_$I$*=func_001DE260
// SLY2_ALIAS: _GLOBAL_$D$*=func_001DE280

#include "sly2/basic.h"
#include "sly2/sw.h"
#include "sly2/slotheap.h"
#include "sly2/stackscope.h"
#include "sly2/joy.h"
#include "sly2/screen.h"
#include "sly2/classes.h"
#include "sly2/vu0.h"
#include "sly2/util.h"
#include "sly2/dli.h"
#include "sly2/cref.h"
#include "sly2/joy.h"
#include "sly2/save.h"
#include "sly2/game.h"
#include "sly2/task.h"
#include "gifs.h"
#include "vifs.h"
extern "C" char base_D_003DA150[] __asm__("D_003DA150");   /* base of the address names below: see docs/guides/tu-merging.md "Data" */

/* The global screens, as views of their bytes (their classes' constructors and virtuals have C names). */
extern "C" {
void *InitScreenList(void *pv);      /* the shown-screen list: InitDl(pdl, 0x14) */
void *GADGETMENU_ctor(void *pv);
void *ConstructThiefnetScreen(void *pv);      /* THIEFNET constructor */
void *ConstructJobListScreen(void *pv);      /* JOBLIST constructor */
void *ConstructScreen52D(void *pv);      /* screen 0x52D constructor */
void *ConstructTitleScreen(void *pv);      /* title screen constructor */
void DestroyBasic(void *pv, int grfDelete);
extern char D_002D8668[] __asm__("_vt$8HUDMETER"), D_002D37D0[] __asm__("_vt$9PAUSEMENU"), D_002CA490[] __asm__("_vt$6CID154");   /* vtables of the HUD meter, pause menu and notice */
extern char D_002D8568[] __asm__("_vt$10GADGETMENU");   /* GADGETMENU vtable */
}
struct SCRLIST
{
    char ab[0x10];
    // Construct an empty screen list.
    SCRLIST() { InitScreenList(this); }
};
/* A screen whose base fields are set inline (pframe, psidebag, vtable). */
template <int CB> struct SCRINL
{
    int x0, x4, x8;
    void *pvt;
    char ab[CB - 0x10];
    // Inline screen header: zeroed fields and the given vtable.
    SCRINL(void *pvtInit) { x0 = 0; x4 = 0; pvt = pvtInit; }
    // Destroy the screen object (DestroyBasic with the delete flag 2).
    ~SCRINL() { DestroyBasic(this, 2); }
};
/* A screen constructed by a C-named constructor. */
template <int CB> struct SCRCTOR
{
    char ab[CB];
    // Construct the object with its class constructor.
    SCRCTOR(void *(*pfnCtor)(void *)) { pfnCtor(this); }
    // Destroy the object (DestroyBasic with the delete flag 2).
    ~SCRCTOR() { DestroyBasic(this, 2); }
};
SCRLIST D_003D9E70 __attribute__((aligned(16)));   /* 16-aligned by layout: 0x3D9E70 after g_gs (save, 0x5408 bytes from 0x3D4A60) */
SCRINL<0x50> D_003D9E80(D_002D8668);    /* HUD meter */
SCRINL<0x1F0> D_003D9ED0(D_002D37D0);   /* pause menu */
SCRINL<0x30> D_003DA0C0(D_002CA490);    /* notice */
SCRCTOR<0x60> D_003DA0F0(GADGETMENU_ctor);
SCRCTOR<0x270> D_003DA150(ConstructThiefnetScreen);
SCRCTOR<0x110> D_003DA3C0(ConstructJobListScreen);
/* 0x3DA4D0: the selected job (JOB *, read as D_003DA4D0 below) heads a 0x20-byte object: the next screen starts
 * at 0x3DA4F0. A plain pointer would be emitted after every constructed object (plain .bss goes at the end of the
 * file), so the slot is a 0x20-byte record with an empty inline constructor, emitted here in order and constructed
 * by no code. Only the first word is named. */
// Job-select slot: the selector and its seven ints.
struct JOBSELSLOT { JOB *pjobsel; int an[7]; JOBSELSLOT() {} };
JOBSELSLOT s_jobselslot __asm__("D_003DA4D0");
SCRCTOR<0x40> D_003DA4F0(ConstructScreen52D);
SCRCTOR<0x40> D_003DA530(ConstructTitleScreen);

// 0x1DE008 StaticInit_screen (the nine screens, then the six slot pools; their destruction) and its stubs.
SLY2_INSTANTIATE_SLOT_POOLS()

/* A screen as the m2c bodies read it. */
struct SCRCHILD { char pad0[0xB0]; int unknownB0; };
/* The notice screen keeps an object at +0x28 instead of a mode. */
#define PLONOTICE(screen) (*(LO **)&(screen)->nMode)
/* A gadget menu item (0x20 bytes, the per-hero tables D_002C2DE8/D_002C3028/D_002C3148): gadget id, its highlight
   animations, its place on the page and flags. */
struct GMITEM
{
    int idGadget;           /* +0x00 the power-up id */
    int idAnimUnlocked;     /* +0x04 highlight animations (SelectGadgetMenuItem) */
    int idAnimAcquired;     /* +0x08 */
    int idAnimSelected;     /* +0x0C */
    int idAnimCur;          /* +0x10 the one playing (-1 none) */
    float x;                /* +0x14 position on the page */
    float y;                /* +0x18 */
    uint fEquippable : 1;   /* +0x1C bit 0: can go in a slot (the info / slot buttons) */
    uint fAvailable : 1;    /* +0x1C bit 1 */
    uint fAcquired : 1;     /* +0x1C bit 2 */
    uint fRest : 29;
};
/* screen.cpp's screens on sly2/screen.h's SCREEN (DRAFT names after their functions and constructors; vtables
 * 0x2D8568..0x2D8368). Defined in the reverse of their emission order: g++ emits a TU's vtables in the reverse order
 * of their classes' definitions, and the ELF has them in ascending order TITLE .. GADGETMENU, SCREEN, HUDMETER,
 * SCREENBASE. Field names DRAFT (from the bodies). */
struct GADGETMENU : SCREEN
{
    int nState;             /* +0x40 the page shown (GadgetMenuPage) */
    float tState;           /* +0x44 */
    int idSelected;         /* +0x48 the selected item's gadget id */
    int citem;              /* +0x4C */
    GMITEM *aitem;          /* +0x50 the page's items */
    int unknown54[3];
    virtual void Reset() __asm__("ResetGadgetMenu");   /* slot 13; key function */
    virtual void Update() __asm__("UpdateGadgetMenu");   /* slot 7 */
    virtual void SetMode(int mode) __asm__("SetGadgetMenuStickDir");   /* slot 14 */
};
typedef char assert_GADGETMENU_size[sizeof(GADGETMENU) == 0x60 ? 1 : -1];
/* A job-list row: the job or task object and its two animations. */
struct JLENTRY { void *ptask; int idAnimIcon; int idAnimCheck; };
/* A ThiefNet list row: the power-up (buy tab) or loot kind (sell tab), and its icon animation. */
struct TNITEM { int id; int idAnimIcon; };
struct THIEFNET : SCREEN
{
    int nTab;               /* +0x40 0 buy, 1 sell, -1 closed */
    float tTab;             /* +0x44 */
    int citem;              /* +0x48 */
    TNITEM aitem[64];       /* +0x4C */
    int iSelected;          /* +0x24C the selected row */
    int iFirstVisible;      /* +0x250 the first row shown */
    int idPowerupSelected;  /* +0x254 */
    int idLootSelected;     /* +0x258 */
    int idAnimPopup;        /* +0x25C */
    int idAnimScrollUp;     /* +0x260 */
    int idAnimScrollDown;   /* +0x264 */
    int unknown268[2];
    virtual void Reset() __asm__("ResetThiefnet");   /* slot 13; key function */
    virtual void Update() __asm__("UpdateThiefnet");   /* slot 7 */
    virtual void SetMode(int mode) __asm__("SetThiefnetStickDir");   /* slot 14 */
};
struct JOBLIST : SCREEN
{
    int nPage;              /* +0x40 0 jobs, 1 the selected job's tasks, -1 closed */
    float tPage;            /* +0x44 */
    int centry;             /* +0x48 */
    JLENTRY aentry[16];     /* +0x4C */
    int centryMax;          /* +0x10C rows of the page */
    void *pjobSelected;     /* +0x110 the job whose tasks page 1 lists (D_003DA4D0 in the global) */
    int fFromJobs;          /* +0x114 page 1 was opened from page 0 */
    int iSelected;          /* +0x118 the selected row */
    int idAnimPopup;        /* +0x11C */
    int idAnimScrollUp;     /* +0x120 */
    int idAnimScrollDown;   /* +0x124 */
    virtual void PostLoad() __asm__("PostJobListLoad");   /* slot 12; key function */
    virtual void Update() __asm__("UpdateJobList");   /* slot 7 */
    virtual void Reset() __asm__("ResetJobList");   /* slot 13 */
    virtual void SetMode(int mode) __asm__("SetJobListStickDir");   /* slot 14 */
};
typedef char assert_THIEFNET_size[sizeof(THIEFNET) == 0x270 ? 1 : -1];
typedef char assert_JOBLIST_size[sizeof(JOBLIST) == 0x128 ? 1 : -1];   /* the global is 0x110 plus the selected job's slot and more (s_jobselslot) */
struct SCRN52D : SCREEN   /* the screen of text 0x52D (ConstructScreen52D constructs it) */
{
    virtual void PostLoad() __asm__("PostLoadScreen52D");   /* slot 12; key function */
    virtual void SetMode(int mode) __asm__("SetScreenState");   /* slot 14 */
};
struct TITLE : SCREEN   /* the title screen */
{
    virtual void SetMode(int mode) __asm__("SetTitleScreenStickDir");   /* slot 14; key function */
    virtual void Update() __asm__("UpdateTitleScreen");   /* slot 7 */
};
/* The job list's selected job (+0x6C / +0x70 text ids). */

extern SW D_003DD490;
extern "C" {
extern CLOCK D_002DDED0;
extern DL D_003DA0E0;
extern char D_002E1E60[], D_002E6550[], D_002E65A0[];   /* letterbox, memcard dialog and status */
extern GIFS D_002DEA00;   /* the frame's two packet streams: the GIF one */
extern VIFS D_002DEA28;   /* and the VIF1 one */
extern LO *D_002E1E40, *D_002DD5BC, *D_002F7900;    /* the three heroes */
extern JOB *D_003DA4D0;
extern const char D_002A7AA8[], D_002A7AB8[];   /* a glyph suffix; "%d%%" */
extern char D_003D4A60[];

void PostHudMeterLoad(void *screen);
void InitPauseMenu(void *pmenu);
void InitLetterbox(void *pletterbox);
void PostLoadScreen(SCREEN *screen);
void PostNoticeLoad(SCREEN *screen);
void PostJobListLoad(SCREEN *screen);
void CMemcardDialog_PostLoad(void *pdlg);
void PostLoadScreen52D(SCREEN *screen);
void DrawScreen(void *screen);
void UpdateScreenFocus(DL *pdl);
void SetHudMeterState(HUDMETER *hudmeter, int nState);
void SetNoticeState(SCREEN *screen, int nState);
void ResetScreenMode(SCREEN *screen);
void SetScreenMode(SCREEN *screen, int n0, int n1);
SCREEN *ConstructScreen(SCREEN *screen);
void ClearGadgetMenu(GADGETMENU *gadgetMenu);
int IsScreenItemUnlocked(int idGadget);
int IsPowerupAcquired(int id);
LO *GetHeroPo(SW *psw);
void SetGadgetMenuState(GADGETMENU *gadgetMenu, int nState);
void ClearThiefnet(SCREEN *screen);
void ClearJobList(SCREEN *screen);
int GetWorldNameTextId(int nWorld);
int GetCurrentWorld();
int GetCompletionPercent(void *pgs, void *psave);
int sprintf(char *pch, const char *pchzFormat, ...);
void strcpy1(char *pchDst, const char *pchSrc);
}
/* strcpy as a call: the m2c bodies' sources are not constants, and g++'s builtin strcpy schedules differently
   (memcard.cpp FormatSavePageTextB). */
char *StrcpyCall(char *pchDst, const char *pchSrc) __asm__("strcpy");
void InitDl(DL *pdl, int ibDle);
int IsEntryInDl(DL *pdl, void *pv);
void RemoveDlEntry(DL *pdl, void *pv);
void PrependDlEntry(DL *pdl, void *pv);
LO *FindSwObject(SW *psw, int grffso, OID oid, LO *plo);
char *FindSwLocalizedString(SW *psw, int id);

/* the shown-screen list's constructor (a DL of screens, link at +0x14) */
// Initializes a list of screens (the link field is at +0x14) and returns it. (inferred)
extern "C" void *InitScreenList(void *pv)
{
    InitDl((DL *)pv, 0x14);
    return pv;
}

namespace M_func_001D9408 { extern "C" {
extern int D_002DD318;
extern char D_003D9E80[], D_003D9ED0[], D_002E1E60[], D_003DA0F0[], D_003DA150[], D_003DA0C0[], D_003DA3C0[];
extern char D_002E6550[], D_002E65A0[], D_003DA4F0[], D_003DA530[];
void ResetHudMeter(void *);
void ResetPauseMenu(void *);
void ResetLetterbox(void *);
void ResetGadgetMenu(void *);
void ResetThiefnet(void *);
void ResetNotice(void *);
void ResetJobList(void *);
void CMemcardDialog_Slot13(void *);
void ResetScreen(void *);
// Return the current element of the iterator and advance it along the element's link field.
static inline void *NextListEntry(DLI *pdli)
{
    void *pv = *pdli->m_ppv;
    pdli->m_ppv = (void **)((char *)pv + pdli->m_ibDle);
    return pv;
}
// Resets every HUD screen (each one's slot 13) and the list of shown screens (level teardown). (inferred)
void ResetScreens(DL *pdl)
{
    DLI dli;
    SCREENBASE *plo;
    InitDli(&dli, pdl);
    while ((plo = (SCREENBASE *)NextListEntry(&dli)) != 0)
        plo->Hide();   /* slot 11 */
    D_002DD318 = 0;
    ResetHudMeter(D_003D9E80);
    ResetPauseMenu(D_003D9ED0);
    ResetLetterbox(D_002E1E60);
    ResetGadgetMenu(D_003DA0F0);
    ResetThiefnet(D_003DA150);
    ResetNotice(D_003DA0C0);
    ResetJobList(D_003DA3C0);
    CMemcardDialog_Slot13(D_002E6550);
    ResetScreen(D_002E65A0);
    ResetScreen(D_003DA4F0);
    ResetScreen(D_003DA530);
    D_002DD2FC = dli.m_pdliNext;
}
} } // namespace M_func_001D9408

/* PostLoad (slot 12) of every global screen. */
// After a level loads: runs every HUD screen's post-load (slot 12), which binds each screen to its widget object.
// (inferred)
extern "C" void PostLoadScreens()
{
    PostHudMeterLoad(&D_003D9E80);
    InitPauseMenu(&D_003D9ED0);
    InitLetterbox(D_002E1E60);
    PostLoadScreen((SCREEN *)&D_003DA0F0);
    PostLoadScreen((SCREEN *)&D_003DA150);
    PostNoticeLoad((SCREEN *)&D_003DA0C0);
    PostJobListLoad((SCREEN *)&D_003DA3C0);
    CMemcardDialog_PostLoad(D_002E6550);
    PostLoadScreen((SCREEN *)D_002E65A0);
    PostLoadScreen52D((SCREEN *)&D_003DA4F0);
    PostLoadScreen((SCREEN *)&D_003DA530);
}

namespace M_func_001D95B8 { extern "C" {
struct S { SCREENBASE *requested; int a, b; SCREENBASE *focused; };   /* the shown-screen list: head, focused screen */
extern JOY D_002DFC00;   /* the joypad */
// If the screen at the head of the shown list (the requested top screen) differs from the focused one, tells the old one
// it lost focus and focuses the new one. (inferred)
void UpdateScreenFocus(struct S *list) {
    SCREENBASE *requested = list->requested;
    SCREENBASE *previous;
    if (requested == list->focused) return;
    if (list->focused) {
        list->focused->pjoy = 0;
        list->focused->SetFocus(0, requested);   /* slot 9 */
    }
    previous = list->focused;
    list->focused = requested;
    if (requested) {
        requested->pjoy = &D_002DFC00;
        requested->SetFocus(1, previous);
    }
}
} } // namespace M_func_001D95B8

namespace M_func_001D9648 { extern "C" {
extern CLOCK D_002DDED0;
typedef struct { char pad[0x100C]; int unknown100C; } PAD;
extern int D_002DFBC0;
extern int D_002DFBC4;
extern PAD D_002DFC00;
extern PAD D_002E0D00;
extern char D_003D9ED0[], D_003DA150[], D_003DA0F0[], D_002DDEC0[], D_00443740[], D_003D9E80[];
extern char D_003DA0C0[], D_003DA3C0[], D_002E6550[], D_002E65A0[], D_003DA4F0[], D_003DA530[];
void RemoveGrfusr__Fi(int);
int HandleJoyButtons(PAD *, int);
void ClearJoyButtons(PAD *, int);
int TryOpenPauseMenu(int);
int IsDlEmpty__FP2DL(void *);
void UpdatePauseMenu(void *);
void UpdateThiefnet(void *);
void UpdateGadgetMenu(void *);
void UpdateCodes(void *);
void UpdateWipe(void *);
void UpdateHudMeter(void *);
void UpdateNotice(void *);
void UpdateJobList(void *);
void CMemcardDialog_Update(void *);
void CMcStatus_Update(void *);
void UpdateTimedScreen(void *);
void UpdateTitleScreen(void *);
// Per-frame HUD screen update: handles the pause / menu buttons and runs every screen's Update (slot 7). (inferred)
void UpdateScreens(void *pdl)
{
    PAD *pad;
    if (D_002DFBC4 & 8) {
        if (D_002DDED0.rate == 0.0f || 0.1f < D_002DDED0.time / D_002DDED0.rate)
            RemoveGrfusr__Fi(8);
    }
    if (D_002DFBC0 & 2)
        pad = &D_002DFC00;
    else
        pad = &D_002E0D00;
    if (HandleJoyButtons(pad, 0x800)) {
        if (!TryOpenPauseMenu(0))
            ClearJoyButtons(pad, 0x800);
    } else if (D_002DFBC0 & 2) {
        int r = IsDlEmpty__FP2DL(pdl);
        int f = 0;
        if (pad->unknown100C == 1 && 5.0f <= D_002DDED0.time)
            f = 1;
        if (r && f)
            TryOpenPauseMenu(0);
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
    CMcStatus_Update(D_002E65A0);
    UpdateTimedScreen(D_003DA4F0);
    UpdateTitleScreen(D_003DA530);
}
} } // namespace M_func_001D9648

// Draws the UI (called from the main loop): runs the draw slot of the three main HUD screens, then sends the two HUD DMA
// streams.
extern "C" void DrawUi()
{
    DrawScreen(&D_003D9E80);
    DrawScreen(&D_003DA0F0);
    DrawScreen(&D_003DA150);
    D_002DEA00.Flush();
    D_002DEA28.Flush();
}

/* screen slot 7 (empty) */
// Base screen update (slot 7): nothing.
void SCREENBASE::Update()   /* key function: SCREENBASE's vtable comes out here */
{
}

/* screen slot 8 (empty) */
// Base screen draw (slot 8, shared by all screens): nothing. (inferred)
extern "C" void DrawScreen()
{
}

// True when the screen is on the shown-screen list.
extern "C" void IsScreenShown(SCREEN *screen)
{
    IsEntryInDl((DL *)&D_003D9E70, screen);
}

/* screen slot 9 (empty) */
// Base screen slot 9: nothing. (inferred)
extern "C" void ScreenSlot9Default()
{
}

/* screen slot 10: (re)insert at the front of the shown list. */
// Shows a screen (slot 10): moves or appends it to the shown list, so it gets focus when it is on top. (inferred)
extern "C" void ShowScreen(SCREEN *screen)
{
    if (IsEntryInDl((DL *)&D_003D9E70, screen) != 0) {
        RemoveDlEntry((DL *)&D_003D9E70, screen);
    }
    PrependDlEntry((DL *)&D_003D9E70, screen);
    UpdateScreenFocus((DL *)&D_003D9E70);
}

/* screen slot 11: take off the shown list. */
// Hides a screen (slot 11): removes it from the shown list and refocuses. (inferred)
extern "C" void HideScreen(SCREEN *screen)
{
    if (IsEntryInDl((DL *)&D_003D9E70, screen) != 0) {
        RemoveDlEntry((DL *)&D_003D9E70, screen);
        UpdateScreenFocus((DL *)&D_003D9E70);
    }
}

/* HUD meter states (SetHudMeterState / HUDMETER::Update): hidden, steady, damage flash, health refilling toward the
   target, power changing, switching hero; 6 and 7 are alternate steady states (hero +0x10C8 set / hero +0x10EC == 4). */
enum HudMeterState { HUDMETER_HIDDEN, HUDMETER_STEADY, HUDMETER_DAMAGE, HUDMETER_HEALTH_FILL, HUDMETER_POWER_CHANGE,
    HUDMETER_SWITCH_HERO, HUDMETER_STATE6, HUDMETER_STATE7 };
/* Notice banner states (SetNoticeState): hidden, appearing, shown, hiding, swapping to the queued text. */
enum NoticeState { NOTICE_HIDDEN, NOTICE_APPEARING, NOTICE_SHOWN, NOTICE_HIDING, NOTICE_SWAPPING };

namespace M_PostHudMeterLoad { extern "C" {
typedef struct {
    char pad[0xDFC];
    int maxHealth;
    int *healthPtr;
    char padE04[0x1300 - 0xE04];
    int maxPower;
} Q;
#define HERO(p) ((Q *)(p)->hero)   /* the hero through the STEPPLAYER view */
extern struct G D_003DD490;
WIDGET *FindSwObject__FP2SWi3OIDP2LO(struct G *, int, int, int);
Q *GetHeroPo(struct G *);
int IsBasicDerivedFrom__FP5BASIC3CID(Q *, int);
int GetStepplayerMp(Q *);
void PlayWidgetAnim(WIDGET *, int);
void SetWidgetAnimPhase(WIDGET *, int, float);
// Post-load of the health / gadget-power meter: finds its widget, reads the hero's health and power fractions and picks
// the per-character animations.
void PostHudMeterLoad(HUDMETER *p)
{
    Q *hero;
    WIDGET *widget;
    p->widget = FindSwObject__FP2SWi3OIDP2LO(&D_003DD490, 0x405, 0x36B, 0);
    p->state = 0;
    p->hero = 0;
    hero = GetHeroPo(&D_003DD490);
    if (hero && IsBasicDerivedFrom__FP5BASIC3CID(hero, 0x34))
        p->hero = (STEPPLAYER *)hero;
    if (p->hero == 0) {
        p->shownHealth = p->health = 0.0f;
        p->shownPower = p->power = 0.0f;
        return;
    }
    p->shownHealth = p->health = (float)*HERO(p)->healthPtr / (float)HERO(p)->maxHealth;
    p->shownPower = p->power = (float)GetStepplayerMp(HERO(p)) / (float)HERO(p)->maxPower;
    if (IsBasicDerivedFrom__FP5BASIC3CID(HERO(p), 0x37)) {
            p->idAnimSteady = 0x347;
            p->idAnimDamage = 0x348;
            p->idAnimHide = 0x349;
            p->idAnimState6 = 0x34A;
            p->idAnimState7 = 0x34B;
    } else if (IsBasicDerivedFrom__FP5BASIC3CID(HERO(p), 0x36)) {
            p->idAnimSteady = 0x342;
            p->idAnimDamage = 0x343;
            p->idAnimHide = 0x344;
            p->idAnimState6 = 0x345;
            p->idAnimState7 = 0x346;
    } else {
            p->idAnimSteady = 0x33D;
            p->idAnimDamage = 0x33E;
            p->idAnimHide = 0x33F;
            p->idAnimState6 = 0x340;
            p->idAnimState7 = 0x341;
    }
    widget = p->widget;
    if (widget) {
        widget->Show(p->idAnimSteady);
        PlayWidgetAnim(p->widget, 0x34F);
        PlayWidgetAnim(p->widget, 0x350);
        SetWidgetAnimPhase(p->widget, 0x34F, 1.0f - p->shownHealth);
        SetWidgetAnimPhase(p->widget, 0x350, 1.0f - p->shownPower);
    }
}
} } // namespace M_PostHudMeterLoad

// Resets the meter: state 0, no widget.
extern "C" void ResetHudMeter(HUDMETER *hudmeter)
{
    SetHudMeterState(hudmeter, HUDMETER_HIDDEN);
    hudmeter->widget = 0;
}

namespace M_SetHudMeterState { extern "C" {
typedef struct { int a, b, c, powerCost; int pad[4]; } REC;
extern GMITEM D_002C2DE8[], D_002C3028[], D_002C3148[];
extern REC D_002BCD00[];
int IsBasicDerivedFrom__FP5BASIC3CID(void *, int);
int IsPowerupAcquired(int);
void PlayWidgetAnim(WIDGET *, int);
void StopWidgetAnim(WIDGET *, int);
void SetWidgetAnimPhase(WIDGET *, int, float);
// Switches the meter's state: hide / appear animations, steady (health gauge, plus the power gauge only if the character
// owns a power-up that costs power), damage, and the alternate animations.
void SetHudMeterState(HUDMETER *p, int s)
{
    int old = p->state;
    if (old == s)
        return;
    if (!p->widget)
        return;
    if (old == HUDMETER_DAMAGE || old == HUDMETER_STATE6)
        p->widget->Show(p->idAnimSteady);
    p->state = s;
    switch (s) {
    case HUDMETER_HIDDEN:
    case HUDMETER_SWITCH_HERO:
        p->widget->Show(p->idAnimHide);
        break;
    case HUDMETER_STEADY: {
        int n = 0;
        GMITEM *tbl = 0;
        int found;
        int i;
        p->widget->Show(p->idAnimSteady);
        PlayWidgetAnim(p->widget, 0x34F);
        SetWidgetAnimPhase(p->widget, 0x34F, 1.0f - p->shownHealth);
        if (IsBasicDerivedFrom__FP5BASIC3CID(p->hero, 0x35)) {
            n = 0x12;
            tbl = D_002C2DE8;
        } else if (IsBasicDerivedFrom__FP5BASIC3CID(p->hero, 0x37)) {
            n = 9;
            tbl = D_002C3028;
        } else if (IsBasicDerivedFrom__FP5BASIC3CID(p->hero, 0x36)) {
            n = 8;
            tbl = D_002C3148;
        }
        found = 0;
        for (i = 0; i < n; i++) {
            int id = tbl[i].idGadget;
            if (IsPowerupAcquired(id)) {
                REC r = D_002BCD00[id];
                if (r.powerCost > 0) {
                    found = 1;
                    break;
                }
            }
        }
        if (found) {
            PlayWidgetAnim(p->widget, 0x350);
            SetWidgetAnimPhase(p->widget, 0x350, 1.0f - p->shownPower);
        } else {
            StopWidgetAnim(p->widget, 0x350);
        }
        break;
    }
    case HUDMETER_DAMAGE:
        p->widget->Show(p->idAnimDamage);
        break;
    case HUDMETER_STATE6:
        p->widget->Show(p->idAnimState6);
        break;
    case HUDMETER_STATE7:
        p->widget->Show(p->idAnimState7);
        break;
    }
}
} } // namespace M_SetHudMeterState

namespace M_UpdateHudMeter { extern "C" {
typedef struct { char pad0[0xC]; void *vt; } SUB;
typedef struct Q {
    char pad0[0xDFC];
    int maxHealth;
    int *healthPtr;
    char pad[0x10C8 - 0xE04];
    int unknown10C8;
    char pad2[0x10EC - 0x10CC];
    int unknown10EC;
    char pad3[0x1300 - 0x10F0];
    int maxPower;
} Q;
#define HERO(p) ((Q *)(p)->hero)   /* the hero through the STEPPLAYER view */
extern struct G D_003DD490;
extern CLOCK D_002DDED0;
extern char D_002C3248[];
Q *GetHeroPo(struct G *);
int IsBasicDerivedFrom__FP5BASIC3CID(void *, int);
int GetStepplayerMp(Q *);
int AreFloatsNear__Ffff(float, float, float);
float SmoothValue__FfffP3SMPPf(void *, int, float, float, float);
int IsWidgetAnimPlaying(WIDGET *, int);
void SetWidgetAnimPhase(WIDGET *, int, float);
} } // namespace M_UpdateHudMeter
// Per-frame meter update (slot 7): follows the current hero, animates health drain / fill and power changes smoothly,
// flashes on damage and plays special animations.
void HUDMETER::Update()   /* key function: HUDMETER's vtable comes out here (the namespace's views) */
{
    using namespace M_UpdateHudMeter;
    using M_UpdateHudMeter::D_003DD490;
    using M_UpdateHudMeter::D_002DDED0;
    HUDMETER *p = this;
    Q *old;
    Q *q;
    if (!p->widget)
        return;
    old = HERO(p);
    p->hero = 0;
    q = GetHeroPo(&D_003DD490);
    if (q && IsBasicDerivedFrom__FP5BASIC3CID(q, 0x34))
        p->hero = (STEPPLAYER *)q;
    if (!p->hero) {
        p->shownHealth = p->health = 0;
        p->shownPower = p->power = 0;
        if (old)
            SetHudMeterState(p, HUDMETER_HIDDEN);
        return;
    }
    if (old != HERO(p)) {
        if (!old)
            SetHudMeterState(p, HUDMETER_HEALTH_FILL);
        else
            SetHudMeterState(p, HUDMETER_SWITCH_HERO);
        if (IsBasicDerivedFrom__FP5BASIC3CID(HERO(p), 0x35)) {
            p->idAnimSteady = 0x33D; p->idAnimDamage = 0x33E; p->idAnimHide = 0x33F; p->idAnimState6 = 0x340; p->idAnimState7 = 0x341;
        } else if (IsBasicDerivedFrom__FP5BASIC3CID(HERO(p), 0x37)) {
            p->idAnimSteady = 0x347; p->idAnimDamage = 0x348; p->idAnimHide = 0x349; p->idAnimState6 = 0x34A; p->idAnimState7 = 0x34B;
        } else if (IsBasicDerivedFrom__FP5BASIC3CID(HERO(p), 0x36)) {
            p->idAnimSteady = 0x342; p->idAnimDamage = 0x343; p->idAnimHide = 0x344; p->idAnimState6 = 0x345; p->idAnimState7 = 0x346;
        }
        p->shownHealth = p->health = (float)*HERO(p)->healthPtr / (float)HERO(p)->maxHealth;
        p->shownPower = p->power = (float)GetStepplayerMp(HERO(p)) / (float)HERO(p)->maxPower;
    }
    switch (p->state) {
    case HUDMETER_STEADY:
    case HUDMETER_STATE6:
    case HUDMETER_STATE7:
        p->health = (float)*HERO(p)->healthPtr / (float)HERO(p)->maxHealth;
        p->power = (float)GetStepplayerMp(HERO(p)) / (float)HERO(p)->maxPower;
        if (*HERO(p)->healthPtr <= 0) {
            SetHudMeterState(p, HUDMETER_HIDDEN);
        } else if (p->shownHealth > p->health) {
            SetHudMeterState(p, HUDMETER_DAMAGE);
        } else if (p->shownHealth < p->health) {
            SetHudMeterState(p, HUDMETER_HEALTH_FILL);
        } else if (!AreFloatsNear__Ffff(p->shownPower, p->power, 0.0001f)) {
            SetHudMeterState(p, HUDMETER_POWER_CHANGE);
        } else if (HERO(p)->unknown10EC == 4) {
            SetHudMeterState(p, HUDMETER_STATE7);
        } else if (HERO(p)->unknown10C8 != -1) {
            SetHudMeterState(p, HUDMETER_STATE6);
        } else {
            SetHudMeterState(p, HUDMETER_STEADY);
        }
        break;
    case HUDMETER_DAMAGE:
        if (!IsWidgetAnimPlaying(p->widget, p->idAnimDamage))
            SetHudMeterState(p, HUDMETER_HEALTH_FILL);
        break;
    case HUDMETER_SWITCH_HERO:
        if (!IsWidgetAnimPlaying(p->widget, p->idAnimHide))
            SetHudMeterState(p, HUDMETER_STEADY);
        break;
    case HUDMETER_HIDDEN:
        p->health = (float)*HERO(p)->healthPtr / (float)HERO(p)->maxHealth;
        p->power = (float)GetStepplayerMp(HERO(p)) / (float)HERO(p)->maxPower;
        if (p->shownHealth < p->health) {
            p->shownHealth = p->health;
            SetWidgetAnimPhase(p->widget, 0x34F, 1.0f - p->shownHealth);
            p->shownPower = p->power;
            SetWidgetAnimPhase(p->widget, 0x350, 1.0f - p->shownPower);
            SetHudMeterState(p, HUDMETER_STEADY);
        }
        break;
    case HUDMETER_HEALTH_FILL:
        p->health = (float)*HERO(p)->healthPtr / (float)HERO(p)->maxHealth;
        if (p->shownHealth < p->health)
            p->shownHealth = p->health;
        else
            p->shownHealth = SmoothValue__FfffP3SMPPf(D_002C3248, 0, p->shownHealth, p->health, D_002DDED0.deltaTime);
        SetWidgetAnimPhase(p->widget, 0x34F, 1.0f - p->shownHealth);
        if (AreFloatsNear__Ffff(p->shownHealth, p->health, 0.0001f))
            SetHudMeterState(p, HUDMETER_STEADY);
        break;
    case HUDMETER_POWER_CHANGE:
        p->power = (float)GetStepplayerMp(HERO(p)) / (float)HERO(p)->maxPower;
        if (p->shownPower < p->power)
            p->shownPower = p->power;
        else
            p->shownPower = SmoothValue__FfffP3SMPPf(D_002C3248, 0, p->shownPower, p->power, D_002DDED0.deltaTime);
        SetWidgetAnimPhase(p->widget, 0x350, 1.0f - p->shownPower);
        if (AreFloatsNear__Ffff(p->shownPower, p->power, 0.0001f))
            SetHudMeterState(p, HUDMETER_STEADY);
        break;
    }
}

// Post-load of the notice banner: finds its widget.
extern "C" void PostNoticeLoad(SCREEN *screen)
{
    PLONOTICE(screen) = FindSwObject(&D_003DD490, 0x405, (OID)0x371, 0);
}

// Resets the notice banner: no current or queued text, no widget, state 0.
extern "C" void ResetNotice(SCREEN *screen)
{
    screen->idText = -1;
    screen->idTextQueued = -1;
    PLONOTICE(screen) = 0;
    SetNoticeState(screen, NOTICE_HIDDEN);
}

namespace M_UpdateNotice { extern "C" {
extern CLOCK D_002DDED0;
typedef struct { char pad[0x58]; int visibility; } W;
typedef struct {
    char pad[0x10];
    int state;
    float stateTime;
    int textId;
    float duration;
    int queuedTextId;
    float queuedDuration;
    W *widget;
} OBJ;
void SetNoticeState(OBJ *, int);
int IsWidgetVisible(W *);
int IsWidgetAnimPlaying(W *, int);
// Per-frame notice banner update: marks it shown once the widget is fully visible, times it out (only while hidden),
// or swaps to the queued text; hides it at the end.
void UpdateNotice(OBJ *p)
{
    if (p->widget == 0)
        return;
    switch (p->state) {
    case NOTICE_HIDDEN:
        break;
    case NOTICE_APPEARING:
        if (p->widget->visibility == 2)
            SetNoticeState(p, NOTICE_SHOWN);
        break;
    case NOTICE_SHOWN:
        if (p->queuedTextId != -1) {
            SetNoticeState(p, NOTICE_SWAPPING);
        } else if (p->duration == 0.0f) {
            SetNoticeState(p, NOTICE_HIDING);
        } else if (0.0f < p->duration) {
            if (p->widget && !IsWidgetVisible(p->widget))
                p->stateTime += D_002DDED0.deltaTime;
        }
        break;
    case NOTICE_HIDING:
        if (p->widget->visibility < 2)
            SetNoticeState(p, NOTICE_HIDDEN);
        break;
    case NOTICE_SWAPPING:
        if (IsWidgetAnimPlaying(p->widget, 0x312))
            break;
        if (p->queuedTextId == -1)
            SetNoticeState(p, NOTICE_HIDDEN);
        else
            SetNoticeState(p, NOTICE_SHOWN);
        break;
    }
}
} } // namespace M_UpdateNotice

namespace M_PostNotice { extern "C" {
typedef struct {
    char pad[0x10];
    int state;
    int pad14;
    int textId;
    float duration;
    int queuedTextId;
    float queuedDuration;
} OBJ;
void CancelNotice(OBJ *, int);
void SetNoticeState(OBJ *, int);
// Requests a notice text for a duration, queued behind the current one; -1 cancels.
void PostNotice(OBJ *p, int textId, float duration)
{
    if (textId == -1) {
        CancelNotice(p, -1);
        return;
    }
    switch (p->state) {
    case NOTICE_APPEARING:
        SetNoticeState(p, -1);   /* forces the re-show below (state -1 is no state) */
        /* fall through */
    case NOTICE_HIDDEN:
    case NOTICE_HIDING:
        p->textId = textId;
        p->duration = duration;
        SetNoticeState(p, NOTICE_APPEARING);
        break;
    case NOTICE_SHOWN:
        if (p->textId == textId) {
            p->duration = duration;
            break;
        }
        /* fall through */
    case NOTICE_SWAPPING:
        p->queuedTextId = textId;
        p->queuedDuration = duration;
        break;
    }
}
} } // namespace M_PostNotice

namespace M_CancelNotice { extern "C" {
struct S { char pad[0x18]; int textId; int duration; int queuedTextId; };
// Cancels a notice text: the current one times out now, or the queued one is dropped; -1 cancels the queue.
void CancelNotice(struct S *p, int n) {
    if (n == -1) {
        p->queuedTextId = -1;
        p->duration = 0;
    } else if (n == p->textId) {
        p->duration = 0;
    } else if (n == p->queuedTextId) {
        p->queuedTextId = -1;
    }
}
} } // namespace M_CancelNotice

/* The text id queued, else the one shown. */
// Text id of the notice being shown, or of the queued one. (inferred)
extern "C" int GetCurrentNoticeTextId(SCREEN *screen)
{
    int ids = screen->idTextQueued;
    if (ids == -1) {
        ids = screen->idText;
    }
    return ids;
}

namespace M_SetNoticeState { extern "C" {
extern CLOCK D_002DDED0;
typedef struct {
    char pad[0x10];
    int state;
    float stateTime;
    int textId;
    float duration;
    int queuedTextId;
    float queuedDuration;
    WIDGET *widget;
} OBJ;
void StartWidgetMainAnim(WIDGET *, int);
void ShowWidgetArgs(WIDGET *, int, int *);
// Switches the notice banner's state: show the widget with the text, hide it, swap animation, or main animation after
// a swap.
void SetNoticeState(OBJ *p, int n)
{
    WIDGET *plo;
    if (n == p->state)
        return;
    switch (p->state) {
    case NOTICE_HIDING:
        if (n == NOTICE_APPEARING && p->stateTime == D_002DDED0.time)
            n = 2;
        break;
    case NOTICE_SWAPPING:
        p->textId = p->queuedTextId;
        p->duration = p->queuedDuration;
        p->queuedTextId = -1;
        p->queuedDuration = -1.0f;
        break;
    }
    if (n == NOTICE_HIDDEN)
        p->textId = -1;
    plo = p->widget;
    if (plo) {
        if (p->state == NOTICE_SWAPPING && n == NOTICE_SHOWN)
            StartWidgetMainAnim(plo, 0x310);
        switch (n) {
        case NOTICE_SWAPPING:
            StartWidgetMainAnim(p->widget, 0x312);
            break;
        case NOTICE_HIDING:
            p->widget->Hide();
            break;
        case NOTICE_APPEARING:
            ShowWidgetArgs(p->widget, 1, &p->textId);
            break;
        }
    }
    p->state = n;
    p->stateTime = D_002DDED0.time;
}
} } // namespace M_SetNoticeState

namespace M_func_001DA728 { extern "C" {
extern char D_002D85E8[] __asm__("_vt$6SCREEN");
// Base SCREEN constructor: vtable, no widget (oid -1), then resets the mode.
SCREEN *ConstructScreen(SCREEN *p) {
    p->scriptFrame.m_p = 0;
    p->scriptVariables.m_p = 0;
    *(void **)((char *)p + 0xC) = D_002D85E8;
    p->idTextQueued = -1;
    ResetScreenMode(p);
    return p;
}
} } // namespace M_func_001DA728

/* screen slot 12: resolve the widget. */
// Base screen post-load (slot 12): finds the screen's widget object by its OID.
void SCREEN::PostLoad()   /* key function: SCREEN's vtable comes out here */
{
    SCREEN *screen = (SCREEN *)this;
    screen->pchild = FindSwObject(&D_003DD490, 5, (OID)screen->idTextQueued, 0);
}

/* screen slot 13. */
// Base screen reset (slot 13): forgets the widget and resets the mode. (inferred)
extern "C" void ResetScreen(SCREEN *screen)
{
    screen->pchild = 0;
    ResetScreenMode(screen);
}

// Resets a screen's state to -1, stamps the time and clears its mode. (inferred)
extern "C" void ResetScreenMode(SCREEN *screen)
{
    screen->nMode = -1;
    screen->modeStartTime = D_002DDED0.realTime;
    SetScreenMode(screen, -1, -1);
}

namespace M_func_001DA808 { extern "C" {
extern CLOCK D_002DDED0; /* +0x3C: the real (unscaled) clock */
void UpdateScreenDefault(SCREEN *p);
// Screen update that closes the screen when its mode timer expires. (inferred)
void UpdateTimedScreen(SCREEN *p) {
    UpdateScreenDefault(p);
    if (p->tPopupEnd <= D_002DDED0.realTime) SetScreenMode(p, -1, -1);
}
} } // namespace M_func_001DA808

namespace M_UpdateScreenStickDir { extern "C" {
float atan2f(float, float);
// Turns the left stick into an 8-way direction for menus: calls the screen's slot 14 with the direction when the
// stick is pushed past 0.8, and with -1 when it returns under 0.5.
void UpdateScreenStickDir(SCREEN *p)
{
    JOY *joy = p->pjoy;
    int mode;
    float stickMagnitude;
    if (joy == 0)
        return;
    mode = p->nMode;
    stickMagnitude = joy->leftStickDeflection;
    if (mode == -1) {
        if (0.8f < stickMagnitude) {
            int k = (int)(atan2f(joy->leftStickY, joy->leftStickX) * 1.2732395f + 8.5f);   /* 4/pi: eighths of a turn, rounded */
            p->SetMode(k % 8);
        }
    } else if (stickMagnitude < 0.5f) {
        p->SetMode(-1);
    }
}
} } // namespace M_UpdateScreenStickDir

namespace M_func_001DA950 { extern "C" {
extern CLOCK D_002DDED0; /* +0x3C: the real (unscaled) clock */
extern float D_002A7984, D_002C3254;
void *CreateSfxSound(int timerKind);
void SetSndSourceLo(void *p, int timerKind);
// Sets a screen's mode and argument; mode 0 plays the menu sound and starts the timeout. (inferred)
void SetScreenMode(SCREEN *p, int timerKind, int modeArg) {
    p->idsPopup = modeArg;
    p->nPopup = timerKind;
    switch (timerKind) {
    case -1:
        p->tPopupEnd = -10.0f;
        break;
    case 0:
        p->tPopupEnd = D_002A7984;
        SetSndSourceLo(CreateSfxSound(0x29), 0);
        break;
    case 1:
    default:
        p->tPopupEnd = D_002DDED0.realTime + D_002C3254;
        SetSndSourceLo(CreateSfxSound(0x36), 0);
        break;
    }
}
} } // namespace M_func_001DA950

// Clears a screen's mode (closes it). (inferred)
extern "C" void CloseScreen(SCREEN *screen)
{
    SetScreenMode(screen, -1, -1);
}

// Gadget-menu screen constructor.
extern "C" void *GADGETMENU_ctor(void *pv)
{
    GADGETMENU *gadgetMenu = (GADGETMENU *)pv;
    ConstructScreen(gadgetMenu);
    *(void **)((char *)gadgetMenu + 0xC) = D_002D8568;   /* the vptr, by hand (a C-named constructor) */
    ClearGadgetMenu(gadgetMenu);
    return gadgetMenu;
}

// Gadget-menu reset (slot 13).
void GADGETMENU::Reset()   /* key function */
{
    GADGETMENU *gadgetMenu = (GADGETMENU *)this;
    ResetScreen(gadgetMenu);
    ClearGadgetMenu(gadgetMenu);
}

// Gadget menu: state -1, time stamp.
extern "C" void ClearGadgetMenu(GADGETMENU *gadgetMenu)
{
    gadgetMenu->nState = -1;
    gadgetMenu->tState = D_002DDED0.realTime;
}

/* Gadget menu pages (SetGadgetMenuState): closed, or the page of Sly, Bentley or Murray. */
enum GadgetMenuPage { GADGETMENU_CLOSED = -1, GADGETMENU_SLY, GADGETMENU_BENTLEY, GADGETMENU_MURRAY };

namespace M_UpdateGadgetMenu { extern "C" {
extern struct G D_003DD490;
extern int D_002C3258[];
void UpdateTimedScreen(GADGETMENU *);
int HandleJoyButtons(void *, int);
void *GetHeroPo(struct G *);
int IsBasicDerivedFrom__FP5BASIC3CID(void *, int);
void UpdateScreenStickDir(GADGETMENU *);
GMITEM *FindScreenEntry(GADGETMENU *, int);
void SetScreenMode(GADGETMENU *, int, int);
void *CreateSfxSound(int);
void SetSndSourceLo(void *, int);
void SetStepplayerGadget(void *, int, int);
// Per-frame gadget menu: circle closes, the info button shows a message, and the three slot buttons assign the selected
// power-up (if owned) to that slot.
void UpdateGadgetMenu(GADGETMENU *p)
{
    void *hero;
    GMITEM *r;
    int i;
    UpdateTimedScreen(p);
    if (p->pchild == 0)
        return;
    if (p->nState == -1)
        return;
    if (p->pjoy == 0)
        return;
    if (p->nPopup && HandleJoyButtons(p->pjoy, 0x10)) {
        SetGadgetMenuState(p, GADGETMENU_CLOSED);
        return;
    }
    hero = GetHeroPo(&D_003DD490);
    if (hero == 0 || !IsBasicDerivedFrom__FP5BASIC3CID(hero, 0x34)) {
        SetGadgetMenuState(p, GADGETMENU_CLOSED);
        return;
    }
    UpdateScreenStickDir(p);
    if (HandleJoyButtons(p->pjoy, 0x7E8)) {
        r = FindScreenEntry(p, p->idSelected);
        if (r == 0 || !r->fEquippable)
            SetScreenMode(p, 1, 0xE2);
        else
            SetScreenMode(p, 1, 0x31);
    }
    for (i = 0; i < 3; i++) {
        if (!HandleJoyButtons(p->pjoy, D_002C3258[i]))
            continue;
        r = FindScreenEntry(p, p->idSelected);
        if (r == 0 || !r->fAcquired) {
            SetScreenMode(p, 1, 0x32);
        } else if (!r->fEquippable) {
            SetScreenMode(p, 1, 0x33);
        } else {
            SetSndSourceLo(CreateSfxSound(0x34), 0);
            SetStepplayerGadget(hero, i, p->idSelected);
        }
    }
}
} } // namespace M_UpdateGadgetMenu

namespace M_func_001DAC58 { extern "C" {
typedef struct { float x, y, dx, dy; char pad[0x10]; } TXT;
typedef struct { char ab[0xA0]; } FONT;
typedef struct { int n; int a; int b; } SLOT;
typedef struct { char pad[0x1180]; SLOT gadgetSlots[3]; } Q;
extern struct G D_003DD490;
extern float D_002C3264;
extern int D_002C3268[];
int IsWidgetVisible(void *);
Q *GetHeroPo(struct G *);
int IsBasicDerivedFrom__FP5BASIC3CID(void *, int);
void CTextBox_SetPos(TXT *, float, float);
void CTextBox_SetSize(TXT *, float, float);
void CTextBox_SetTextColor(TXT *, unsigned int *);
void CTextBox_SetHorizontalJust(TXT *, int);
void CTextBox_SetVerticalJust(TXT *, int);
FONT *CRichText_ctor(FONT *, int, float);
void DrawRichText2(FONT *, TXT *, int);
// 0.0f materialised from $zero.
static inline float Zero(void){float z;__asm__ volatile("mtc1 $zero,%0":"=f"(z));return z;}
// While the gadget menu is visible, draws the slot button glyph next to each power-up that is equipped in a slot.
// (inferred)
void DrawGadgetMenuSlotLabels(GADGETMENU *p)
{
    Q *hero;
    int i;
    if (p->pchild == 0)
        return;
    if (!IsWidgetVisible(p->pchild))
        return;
    hero = GetHeroPo(&D_003DD490);
    if (hero == 0)
        return;
    if (!IsBasicDerivedFrom__FP5BASIC3CID(hero, 0x34))
        return;
    for (i = 0; i < p->citem; i++) {
        GMITEM *pe = &p->aitem[i];
        int j;
        for (j = 0; j < 3; j++) {
            if (pe->idGadget == hero->gadgetSlots[j].n) {
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
                DrawRichText2(&font, &txt, 0x13);
                break;
            }
        }
    }
}
} } // namespace M_func_001DAC58

namespace M_func_001DAE20 { extern "C" {
// Finds the item entry (0x20 bytes) with the given gadget id in the screen's item list, or null.
GMITEM *FindScreenEntry(GADGETMENU *p, int gadgetId) {
    int i;
    for (i = 0; i < p->citem; i++) {
        GMITEM *pe = &p->aitem[i];
        if (pe->idGadget == gadgetId) return pe;
    }
    return 0;
}
} } // namespace M_func_001DAE20

namespace M_func_001DAE58 { extern "C" {
struct E { int a[4]; int unlockEpisode; int b[3]; };
extern struct E D_002BCD00[];
int GetBestEpisode(void);
// True when power-up item i is unlocked by the story progress (its episode threshold). (inferred)
// Runtime-checked against the ThiefNet list.
int IsScreenItemUnlocked(int i) {
    struct E *pe = &D_002BCD00[i];
    if (pe->unlockEpisode == -1) return 0;
    return GetBestEpisode() >= pe->unlockEpisode;
}
} } // namespace M_func_001DAE58

namespace M_FPowerupAcquired { extern "C" {
extern char D_002A79A0[], D_002A79A8[], D_002A79B8[];
void sprintf(char *pch, char *fmt, int n);
void GetSaveVarByName(CRefData *pref, char *a, char *b, char *c, int d);
void func_00129658(CRefData *pref, CRefData *prefSrc) __asm__("__as__4CRefRC4CRef");
void Decref__4CRef(CRefData *pref);
// True when the power-up has been bought (save data).
int IsPowerupAcquired(int n) {
    CRefData ref;
    char ach[0x20];
    CRefData tmp;
    int r;
    if (n == -1) return 0;
    ref.m_tagk = -1;
    sprintf(ach, D_002A79A0, n);
    GetSaveVarByName(&tmp, D_002A79A8, D_002A79B8, ach, 0);
    func_00129658(&ref, &tmp);
    Decref__4CRef(&tmp);
    r = ref.u.n;
    Decref__4CRef(&ref);
    return r;
}
} } // namespace M_FPowerupAcquired

namespace M_SetPowerupAcquired {
extern "C" { extern GAME D_002DEB30; }
extern "C" { extern char D_002A79A0[]; }
extern "C" { extern char D_002A79A8[]; }
extern "C" { extern char D_002A79B8[]; }
extern "C" { int sprintf(char *, const char *, ...); }
extern "C" { CRef SetSaveVarByName(const CRef &, const char *, ...); }
extern "C" { void BroadcastGameEvent(GAME *, int, int); }
// Marks a power-up bought or not; buying broadcasts the "power-up acquired" game event.
extern "C" void SetPowerupAcquired(int n, int f)
{
    CRef ref;
    char buf[0x20];
    sprintf(buf, D_002A79A0, n);
    ref = SetSaveVarByName(CRef(f != 0), D_002A79A8, D_002A79B8, buf, 0);
    if (f) {
        D_002DEB30.idGadgetLast = n;
        BroadcastGameEvent(&D_002DEB30, 0x3B, 0);
    }
}
} // namespace M_SetPowerupAcquired

// Refreshes each gadget-menu item's unlocked and acquired flags.
extern "C" void RefreshGadgetMenuItems(GADGETMENU *gadgetMenu)
{
    int i = 0;
    if (gadgetMenu->citem > 0) {
        do {
            GMITEM *pitem = &gadgetMenu->aitem[i++];
            pitem->fAvailable = IsScreenItemUnlocked(pitem->idGadget);
            pitem->fAcquired = IsPowerupAcquired(pitem->idGadget);
        } while (i < gadgetMenu->citem);
    }
}

namespace M_SetGadgetMenuStickDir {
// Change a selector's input mode and, when the joystick points somewhere, move the selection to the
// enabled item that lies closest along that direction from the current one.
typedef int qword __attribute__((mode(TI)));

struct PROX { float along, distance; };
struct HIT { qword position, other, normal; char pad30[0x10]; float fraction; char pad44[0x1C]; };

// D_002DDF0C is read as a field of the clock struct: a struct field at a fixed address may alias
// sel->mode (a scalar global may not), which keeps the clock load after the mode store.
extern "C" { extern CLOCK D_002DDED0; }
extern "C" { extern float D_002C3280, D_002C3274, D_002C3278, D_002C327C, D_002A79C8; }

extern "C" {
GMITEM *FindScreenEntry(GADGETMENU *sel, int id);
int ClipEdgeToSphere(qword point, qword start, qword end, int flags, HIT *hit, float radius);
void FindClosestPointOnLineSegment(qword point, qword start, qword end, qword *unused, float *along, float *distance);
void SelectGadgetMenuItem(GADGETMENU *sel, int id);
void CloseScreen(GADGETMENU *sel);
void *CreateSfxSound(int id);
void SetSndSourceLo(void *sound, int flags);
void SetGadgetMenuStickDir(GADGETMENU *sel, int mode);
}

// Pack x, y, z into a 128-bit GPR value (home is a scratch slot).
static inline qword XYZ(float x, float y, float z, QWROW &home)
{
    qword r, t;
    __asm__("mfc1 %0,%2\n\tmfc1 %1,%3\n\tpextlw %0,%1,%0\n\tmfc1 %1,%4\n\tpcpyld %0,%1,%0"
            : "=&r"(r), "=&r"(t) : "f"(x), "f"(y), "f"(z));
    home.q = r;
    return home.q;
}
// Move a 128-bit GPR value into a VU0 register.
static inline qword ToVF(qword v) { qword r; __asm__("qmtc2.ni %1,%0" : "=j"(r) : "r"(v)); return r; }
// Unit vector along a.
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
static inline qword Splat(const float &f, QWROW &home)
{
    qword s;
    __asm__("qmtc2.ni %1,%0" : "=j"(s) : "rV"(*(const int *)&f));
    home.q = s;
    return home.q;
}
// b by const reference: the ray lives in a stack slot, so the two Adds are not CSEd and each
// reloads it into the register the result goes to.
static inline qword Add(qword a, const qword &b) { qword r; __asm__("vadd.xyzw %0,%1,%2" : "=j"(r) : "j"(a), "j"(b)); return r; }

// Moves the gadget-menu selection to the selectable item lying closest along the stick direction (slot 14).
void SetGadgetMenuStickDir(GADGETMENU *sel, int mode)
{
    QWROW home;
    HIT hit;
    PROX proximity;
    if (mode == sel->nMode)
        return;
    sel->nMode = mode;
    sel->modeStartTime = D_002DDED0.realTime;
    if (mode == -1 || sel->idSelected == -1)
        return;
    GMITEM *current = FindScreenEntry(sel, sel->idSelected);
    qword position = XYZ(current->x, current->y * D_002C3280, 0.0f, home);
    qword direction;
    if (sel->pjoy)
        direction = ToVF(XYZ(sel->pjoy->leftStickX, -sel->pjoy->leftStickY, 0.0f, home));
    else
        direction = VuZero<qword>();
    qword ray = VuScaleByX(Normalize(direction), Splat(D_002C3274, home));
    int bestId = -1;
    float best = D_002A79C8;
    for (int i = 0; i < sel->citem; i++) {
        qword origin = ToVF(position);
        GMITEM *item = &sel->aitem[i];
        if (item == current)
            continue;
        if (!(item->fAcquired))
            continue;
        float zero = 0.0f;
        qword point = XYZ(item->x, item->y * D_002C3280, zero, home);
        float score;
        if (ClipEdgeToSphere(point, origin, Add(origin, ray), 1, &hit, D_002C3278) > 0) {
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
            bestId = item->idGadget;
            best = score;
        }
    }
    if (bestId != -1) {
        SelectGadgetMenuItem(sel, bestId);
        CloseScreen(sel);
        SetSndSourceLo(CreateSfxSound(0x35), 0);
    }
}
} // namespace M_SetGadgetMenuStickDir

namespace M_SelectGadgetMenuItem { extern "C" {
void StopWidgetAnim(void *, int);
void PlayWidgetAnim(void *, int);
// Selects an item and sets every item's highlight animation (selected / acquired / unlocked).
void SelectGadgetMenuItem(GADGETMENU *p, int gadgetId)
{
    int i;
    p->idSelected = gadgetId;
    for (i = 0; i < p->citem; i++) {
        GMITEM *e = &p->aitem[i];
        int v;
        if (e->idGadget == gadgetId)
            v = e->idAnimSelected;
        else if (e->fAcquired)
            v = e->idAnimAcquired;
        else if (e->fAvailable)
            v = e->idAnimUnlocked;
        else
            v = -1;
        if (v != e->idAnimCur) {
            if (p->pchild && e->idAnimCur != -1)
                StopWidgetAnim(p->pchild, e->idAnimCur);
            e->idAnimCur = v;
            if (p->pchild && v != -1)
                PlayWidgetAnim(p->pchild, v);
        }
    }
}
} } // namespace M_SelectGadgetMenuItem

// Opens the gadget-menu page of the current hero (Sly, Bentley or Murray).
extern "C" void OpenGadgetMenuForHero(GADGETMENU *gadgetMenu)
{
    LO *ploHero = GetHeroPo(&D_003DD490);
    if (ploHero != 0) {
        if (ploHero == D_002E1E40) {
            SetGadgetMenuState(gadgetMenu, GADGETMENU_SLY);
            return;
        }
        if (ploHero == D_002DD5BC) {
            SetGadgetMenuState(gadgetMenu, GADGETMENU_BENTLEY);
            return;
        }
        if (ploHero == D_002F7900) {
            SetGadgetMenuState(gadgetMenu, GADGETMENU_MURRAY);
        }
    }
}

namespace M_SetGadgetMenuState { extern "C" {
extern CLOCK D_002DDED0; /* +0x3C: the real (unscaled) clock */
extern char D_003DD490[];
extern GMITEM D_002C2DE8[], D_002C3028[], D_002C3148[];
extern float D_002A79CC;
void SetClockRate(CLOCK *, float, int);
void IncrementSwHandsOff(void *, int);
void DecrementSwHandsOff(void *, int);
void SelectGadgetMenuItem(GADGETMENU *, int);
void SetScreenMode(GADGETMENU *, int, int);
WIDGET *FindSwObject__FP2SWi3OIDP2LO(void *, int, int, int);
void SetGadgetMenuState(GADGETMENU *p, int s);
// Opens or closes the gadget menu: opening freezes the clock, takes player control and shows the hero's page; closing
// restores them.
void SetGadgetMenuState(GADGETMENU *p, int s)
{
    if (s == p->nState)
        return;
    if (p->nState == GADGETMENU_CLOSED) {
        SetClockRate(&D_002DDED0, 0.0f, 0);
        IncrementSwHandsOff(D_003DD490, 0);
        p->Show();
    }
    SelectGadgetMenuItem(p, -1);
    p->nState = s;
    p->tState = D_002DDED0.realTime;
    switch (s) {
    case GADGETMENU_CLOSED: {
        int i;
        if (p->pchild)
            ((WIDGET *)p->pchild)->Hide();
        for (i = 0; i < p->citem; i++)
            p->aitem[i].idAnimCur = -1;
        p->citem = 0;
        p->aitem = 0;
        SetClockRate(&D_002DDED0, 1.0f, 0);
        DecrementSwHandsOff(D_003DD490, 0);
        p->Hide();
        SetScreenMode(p, -1, -1);
        p->pchild = 0;
        p->idTextQueued = -1;
        break;
    }
    case GADGETMENU_SLY:
        p->idTextQueued = 0x375;
        p->citem = 0x12;
        p->aitem = D_002C2DE8;
        break;
    case GADGETMENU_BENTLEY:
        p->idTextQueued = 0x376;
        p->citem = 9;
        p->aitem = D_002C3028;
        break;
    case GADGETMENU_MURRAY:
        p->idTextQueued = 0x377;
        p->citem = 8;
        p->aitem = D_002C3148;
        break;
    }
    if (p->nState != GADGETMENU_CLOSED) {
        WIDGET *e;
        RefreshGadgetMenuItems(p);
        e = FindSwObject__FP2SWi3OIDP2LO(D_003DD490, 5, p->idTextQueued, 0);
        p->pchild = e;
        if (!e) {
            ::SetGadgetMenuState(p, GADGETMENU_CLOSED);   /* the file-scope declaration (this one is ambiguous with it) */
        } else {
            float best;
            int gadgetId;
            int i;
            e->Show(0x318);
            best = D_002A79CC;
            gadgetId = -1;
            for (i = 0; i < p->citem; i++) {
                GMITEM *pe = &p->aitem[i];
                if (pe->fAcquired) {
                    float f = pe->x + pe->y;
                    if (f < best) {
                        gadgetId = pe->idGadget;
                        best = f;
                    }
                }
            }
            SelectGadgetMenuItem(p, gadgetId);
        }
    }
}
} } // namespace M_SetGadgetMenuState

namespace M_func_001DB7A8 { extern "C" {
extern char D_002D84E8[] __asm__("_vt$8THIEFNET");
THIEFNET *ConstructScreen(THIEFNET *p);
void ClearThiefnet(THIEFNET *p);
// Constructor of the ThiefNet shop screen (widget 0x378, a 64-entry item list). (inferred)
THIEFNET *ConstructThiefnetScreen(THIEFNET *p) {
    ConstructScreen(p);
    *(void **)((char *)p + 0xC) = D_002D84E8;
    p->idTextQueued = 0x378;
    ClearThiefnet(p);
    return p;
}
} } // namespace M_func_001DB7A8

// ThiefNet shop reset (slot 13).
void THIEFNET::Reset()   /* key function */
{
    SCREEN *screen = (SCREEN *)this;
    ResetScreen(screen);
    ClearThiefnet(screen);
}

namespace M_ClearThiefnet { extern "C" {
extern CLOCK D_002DDED0; /* +0x3C: the real (unscaled) clock */
// ThiefNet: state -1, empty item list, no animation handles.
void ClearThiefnet(THIEFNET *p) {
    unsigned int i;
    p->nTab = -1;
    p->tTab = D_002DDED0.realTime;
    p->idAnimPopup = -1;
    p->idAnimScrollUp = -1;
    p->idAnimScrollDown = -1;
    for (i = 0; i < 64; i++) {
        p->aitem[i].id = -1;
        p->aitem[i].idAnimIcon = -1;
    }
}
} } // namespace M_ClearThiefnet

namespace M_UpdateThiefnet {
struct REC { int a, b, price; int pad[5]; };
struct REC12 { int sellPrice, x4, price; };
extern "C" {
extern REC D_002BCD00[];
extern REC12 D_002C3288[];
extern char D_002DEB30[];
extern char D_003DD490[];
extern char D_002A79A0[], D_002A79D0[], D_002A79D8[];
void UpdateTimedScreen(THIEFNET *);
int HandleJoyButtons(void *, int);
void SetThiefnetTab(THIEFNET *, int);
int GetCoinCount(void *);
void SetCoinCount(void *, int);
void SetScreenMode(THIEFNET *, int, int);
void SetPowerupAcquired(int, int);
void RefreshThiefnetItems(THIEFNET *);
void *CreateSfxSound(int);
void SetSndSourceLo(void *, int);
void RefreshPlayersAfterPowerupChange(void *);
void sprintf(char *, const char *, ...);
CRef GetSaveVarByName(const char *, ...);
CRef SetSaveVarByName(const CRef &, const char *, ...);
void UpdateScreenStickDir(THIEFNET *);
void UpdateThiefnetWidgets(THIEFNET *);
}
// Per-frame ThiefNet: on the buy tab buys the selected power-up (coin check, confirmation, payment, notification); on the
// sell tab sells one loot item.
extern "C" void UpdateThiefnet(THIEFNET *p)
{
    UpdateTimedScreen(p);
    if (p->nTab == -1)
        return;
    if (!p->pjoy)
        return;
    if (p->nPopup && HandleJoyButtons(p->pjoy, 0x10)) {
        SetThiefnetTab(p, -1);
        return;
    }
    switch (p->nTab) {
    case 0:
        if (HandleJoyButtons(p->pjoy, 0x40)) {
            if (p->idPowerupSelected != -1) {
                int cost = D_002BCD00[p->idPowerupSelected].price;
                int have = GetCoinCount(D_002DEB30);
                if (!p->nPopup) {
                    SetScreenMode(p, -1, -1);
                    SetCoinCount(D_002DEB30, have - cost);
                    SetPowerupAcquired(p->idPowerupSelected, 1);
                    RefreshThiefnetItems(p);
                    SetSndSourceLo(CreateSfxSound(0x29), 0);
                    RefreshPlayersAfterPowerupChange(D_003DD490);
                } else if (have >= cost) {
                    SetScreenMode(p, 0, 0x34);
                } else {
                    SetScreenMode(p, 1, 0x35);
                }
            } else {
                SetSndSourceLo(CreateSfxSound(0x36), 0);
            }
        }
        break;
    case 1:
        if (HandleJoyButtons(p->pjoy, 0x40)) {
            if (p->idLootSelected == -1) {
                SetSndSourceLo(CreateSfxSound(0x36), 0);
            } else {
                CRef r;
                char buf[32];
                sprintf(buf, D_002A79A0, p->idLootSelected);
                r = GetSaveVarByName(D_002A79D0, D_002A79D8, buf, 0);
                if (r.u.n > 0) {
                    SetSaveVarByName(CRef(r.u.n - 1, 0x15), D_002A79D0, D_002A79D8, buf, 0);
                    SetCoinCount(D_002DEB30, GetCoinCount(D_002DEB30) + D_002C3288[p->idLootSelected].sellPrice);
                    RefreshThiefnetItems(p);
                    SetSndSourceLo(CreateSfxSound(0x29), 0);
                }
            }
        }
        break;
    }
    if (!p->nPopup && HandleJoyButtons(p->pjoy, 0x10))
        SetScreenMode(p, -1, -1);
    if (HandleJoyButtons(p->pjoy, 0x80)) {
        SetScreenMode(p, -1, -1);
        SetThiefnetTab(p, p->nTab == 0);
    }
    if (p->nPopup)
        UpdateScreenStickDir(p);
    UpdateThiefnetWidgets(p);
}
} // namespace M_UpdateThiefnet

namespace M_func_001DBBA0 { extern "C" {
extern int D_002C35B4;
extern int D_003DA570;
// Index just past the last visible row of the scrolling list (window size adjusted for the scroll arrows). (inferred)
int GetThiefnetRowEnd(THIEFNET *p) {
    int n = p->iFirstVisible + D_002C35B4;
    if (D_003DA570) {
        n -= (p->iFirstVisible > 0);
        n -= (n < p->citem);
    }
    return n;
}
} } // namespace M_func_001DBBA0

namespace M_SelectThiefnetItem { extern "C" {
extern int D_003DA570;
extern int D_002C35B4;
int GetThiefnetRowEnd(THIEFNET *);
void CloseScreen(THIEFNET *);
// Selects iconAnim row (clamped), scrolls the window to it and records the selected power-up / loot id.
void SelectThiefnetItem(THIEFNET *p, int row)
{
    if (p->citem == 0) {
        p->iFirstVisible = 0;
        p->iSelected = -1;
        p->idPowerupSelected = -1;
        p->idLootSelected = -1;
    } else {
        row = ClampInt(row, 0, p->citem - 1);
        p->iSelected = row;
        if (D_003DA570) {
            if (row < p->iFirstVisible) {
                p->iFirstVisible = row;
            } else {
                while (GetThiefnetRowEnd(p) - 1 < p->iSelected)
                    p->iFirstVisible++;
            }
            if (p->iFirstVisible == 1)
                p->iFirstVisible = 0;
        } else {
            int id = ClampInt(p->iFirstVisible, StdMaxInt(row - D_002C35B4 + 1, 0), row);
            p->iFirstVisible = id;
        }
        p->idPowerupSelected = p->aitem[p->iSelected].id;
        p->idLootSelected = p->aitem[p->iSelected].id;
    }
    CloseScreen(p);
}
} } // namespace M_SelectThiefnetItem

namespace M_RefreshThiefnetItems {
extern "C" { extern char D_002A79A0[]; }
extern "C" { extern char D_002A79D0[]; }
extern "C" { extern char D_002A79D8[]; }
extern "C" int sprintf(char *, const char *, ...);
extern "C" CRef GetSaveVarByName(const char *, ...);
extern "C" int IsScreenItemUnlocked(int);
extern "C" int IsPowerupAcquired(int);
extern "C" void SelectThiefnetItem(THIEFNET *, int);
// Rebuilds the ThiefNet list: buy tab = unlocked power-ups not yet bought; sell tab = loot kinds the player owns.
extern "C" void RefreshThiefnetItems(THIEFNET *p)
{
    {
        int j;
        for (j = 0; j < p->citem; j++)
            p->aitem[j].id = -1;
    }
    p->citem = 0;
    switch (p->nTab) {
    case 0: {
        int i;
        for (i = 7; i < 0x2B; i++) {
            if (!IsScreenItemUnlocked(i))
                continue;
            if (IsPowerupAcquired(i))
                continue;
            if ((unsigned int)p->citem > 0x40   /* the view's unsigned count */)
                break;
            p->aitem[p->citem++].id = i;
        }
        break;
    }
    case 1: {
        int i;
        for (i = 0; i < 0x40; i++) {
            CRef ref;
            char ach[0x20];
            sprintf(ach, D_002A79A0, i);
            ref = GetSaveVarByName(D_002A79D0, D_002A79D8, ach, 0);
            if (ref.u.n == 0)
                continue;
            if ((unsigned int)p->citem > 0x40   /* the view's unsigned count */)
                break;
            p->aitem[p->citem++].id = i;
        }
        break;
    }
    }
    SelectThiefnetItem(p, p->iSelected);
}
} // namespace M_RefreshThiefnetItems

namespace M_UpdateThiefnetWidgets { extern "C" {
typedef struct { float a[8]; } M2;
typedef struct { int visibility; } SUBX;
typedef struct { char pad[0x58]; int visibility; } SUB;
typedef struct { int a, category; int pad[6]; } REC;
extern int D_003DA570;
extern float D_002C35B0;
extern int D_002C3588[];
extern REC D_002BCD00[];
void *func_002333B4(void *, int, unsigned int) __asm__("memset");
int GetThiefnetRowEnd(THIEFNET *);
void SetWidgetAnimXf(void *, int, M2 *);
void SwitchWidgetAnim(void *, int, int *);
void PlayWidgetAnimWithArg(void *, int, int);
void PlayWidgetAnimsWithArg(void *, int, int);
void PlayWidgetAnim(void *, int);
void StopWidgetAnim(void *, int);
// Drives the ThiefNet widget animations: scroll arrows, row icons by item kind, cursor and popup.
void UpdateThiefnetWidgets(THIEFNET *p)
{
    M2 m;
    int end;
    int i;
    if (((SUB *)p->pchild)->visibility != 2)
        return;
    func_002333B4(&m, 0, sizeof(m));
    m.a[3] = 1.0f;
    m.a[0] = 1.0f;
    end = GetThiefnetRowEnd(p);
    if (p->iFirstVisible > 0) {
        int id = p->nTab == 0 ? 0x4DA : 0x4A1;
        if (D_003DA570) {
            SetWidgetAnimXf(p->pchild, id, &m);
            m.a[5] += D_002C35B0;
        }
        SwitchWidgetAnim(p->pchild, id, &p->idAnimScrollUp);
    } else {
        SwitchWidgetAnim(p->pchild, -1, &p->idAnimScrollUp);
    }
    for (i = p->iFirstVisible; i < end; i++) {
        TNITEM *e = &p->aitem[i];
        int k = D_002C3588[i - p->iFirstVisible];
        switch (p->nTab) {
        case 0:
            if (e->id != -1) {
                switch (D_002BCD00[e->id].category) {
                case 7:
                    e->idAnimIcon = 0x456;
                    break;
                case 9:
                    e->idAnimIcon = 0x458;
                    break;
                case 8:
                    e->idAnimIcon = 0x457;
                    break;
                }
                PlayWidgetAnimWithArg(p->pchild, 0x455, k);
                PlayWidgetAnimWithArg(p->pchild, e->idAnimIcon, k);
            } else {
                PlayWidgetAnimsWithArg(p->pchild, 0x455, k);
                PlayWidgetAnimsWithArg(p->pchild, e->idAnimIcon, k);
                e->idAnimIcon = -1;
            }
            break;
        case 1:
            if (e->id != -1)
                PlayWidgetAnimWithArg(p->pchild, 0x455, k);
            else
                PlayWidgetAnimsWithArg(p->pchild, 0x455, k);
            break;
        }
    }
    if (p->iSelected >= 0) {
        PlayWidgetAnim(p->pchild, 0x437);
        func_002333B4(&m, 0, sizeof(m));
        m.a[3] = 1.0f;
        m.a[0] = 1.0f;
        m.a[5] = D_002C35B0 * (float)(p->iSelected - p->iFirstVisible);
        SetWidgetAnimXf(p->pchild, 0x437, &m);
    } else {
        StopWidgetAnim(p->pchild, 0x437);
    }
    if (end < p->citem) {
        int id = p->nTab == 0 ? 0x4DD : 0x4A4;
        if (D_003DA570) {
            SetWidgetAnimXf(p->pchild, id, &m);
            m.a[5] += D_002C35B0;
        }
        SwitchWidgetAnim(p->pchild, id, &p->idAnimScrollDown);
    } else {
        SwitchWidgetAnim(p->pchild, -1, &p->idAnimScrollDown);
    }
    SwitchWidgetAnim(p->pchild, (p->nPopup != -1) ? 0x43B : 0x439, &p->idAnimPopup);
}
} } // namespace M_UpdateThiefnetWidgets

namespace M_SetThiefnetTab { extern "C" {
typedef struct SUB {
    char pad0[0x58];
    int visibility;
    char pad5c[0x70 - 0x5C];
    int pageAnimSecond;     /* +0x70..+0x78: the page's three animation ids (inferred; named by id order) */
    int pageAnimFirst;
    int pageAnimThird;
} SUB;
extern CLOCK D_002DDED0; /* +0x3C: the real (unscaled) clock */
extern char D_003DD490[];
void SetClockRate(CLOCK *, float, int);
void IncrementSwHandsOff(void *, int);
void DecrementSwHandsOff(void *, int);
void SetWidgetState(void *, int);
void SetWidgetRequest(void *, int);
void *CreateSfxSound(int);
void SetSndSourceLo(void *, int);
void RefreshThiefnetItems(THIEFNET *);
void SelectThiefnetItem(THIEFNET *, int);
// Opens ThiefNet on a tab (buy or sell) or closes it (-1), freezing / resuming the clock and player control.
void SetThiefnetTab(THIEFNET *p, int s)
{
    int old;
    if (!p->pchild)
        s = -1;
    if (s == p->nTab)
        return;
    if (p->nTab == -1) {
        SetClockRate(&D_002DDED0, 0.0f, 0);
        IncrementSwHandsOff(D_003DD490, 0);
        p->Show();
        SetWidgetState(p->pchild, 1);
    }
    old = p->nTab;
    p->nTab = s;
    p->tTab = D_002DDED0.realTime;
    switch (s) {
    case -1:
        ((WIDGET *)p->pchild)->Hide();
        SetClockRate(&D_002DDED0, 1.0f, 0);
        DecrementSwHandsOff(D_003DD490, 0);
        p->Hide();
        if (((SUB *)p->pchild)->visibility > 0)
            SetWidgetRequest(p->pchild, 1);
        p->idAnimPopup = s;
        p->idAnimScrollUp = s;
        p->idAnimScrollDown = s;
        break;
    case 0:
        ((SUB *)p->pchild)->pageAnimFirst = 0x4D4;
        ((SUB *)p->pchild)->pageAnimSecond = 0x4D5;
        ((SUB *)p->pchild)->pageAnimThird = 0x4D6;
        if (old == 1) {
            SetWidgetState(p->pchild, 1);
            SetWidgetState(p->pchild, 2);
        } else {
            SetWidgetRequest(p->pchild, 2);
        }
        SetSndSourceLo(CreateSfxSound(0x35), 0);
        break;
    case 1:
        ((SUB *)p->pchild)->pageAnimFirst = 0x49B;
        ((SUB *)p->pchild)->pageAnimSecond = 0x49C;
        ((SUB *)p->pchild)->pageAnimThird = 0x49D;
        if (old == 0) {
            SetWidgetState(p->pchild, 1);
            SetWidgetState(p->pchild, 2);
        } else {
            SetWidgetRequest(p->pchild, 2);
        }
        SetSndSourceLo(CreateSfxSound(0x35), 0);
        break;
    }
    RefreshThiefnetItems(p);
    SelectThiefnetItem(p, 0);
    p->SetMode(-1);
}
} } // namespace M_SetThiefnetTab

// .rodata order (docs/guides/tu-merging.md "Phase 5: data order"): a named constant is emitted where it is
// defined, a function's literals and jump tables with the function, so each constant is defined where the ELF
// has it among the functions, under an asm label (no function sees a definition under the name it reads).
namespace RO_ORDER { extern const float r_D_002A7984 __asm__("D_002A7984") = 3.40282347e+38f; }
/* the three strings of the D_002C3268 table (.data, below), defined here because the ELF has them here */
namespace RO_ORDER { extern const char r_D_002A7988[] __asm__("D_002A7988") = "&2L&."; }
namespace RO_ORDER { extern const char r_D_002A7990[] __asm__("D_002A7990") = "&2l&."; }
namespace RO_ORDER { extern const char r_D_002A7998[] __asm__("D_002A7998") = "&2r&."; }
namespace RO_ORDER { extern const char r_D_002A79A0[] __asm__("D_002A79A0") = "%d"; }
namespace RO_ORDER { extern const char r_D_002A79A8[] __asm__("D_002A79A8") = "powerUps"; }
namespace RO_ORDER { extern const char r_D_002A79B8[] __asm__("D_002A79B8") = "mppukfAcquired"; }
namespace RO_ORDER { extern const float r_D_002A79C8 __asm__("D_002A79C8") = 3.40282347e+38f; }
namespace RO_ORDER { extern const float r_D_002A79CC __asm__("D_002A79CC") = 3.40282347e+38f; }
namespace RO_ORDER { extern const char r_D_002A79D0[] __asm__("D_002A79D0") = "loot"; }
namespace RO_ORDER { extern const char r_D_002A79D8[] __asm__("D_002A79D8") = "mplootkc"; }
namespace M_func_001DC4B0 { extern "C" {
extern CLOCK D_002DDED0; /* +0x3C: the real (unscaled) clock */
void SetThiefnetTab(THIEFNET *, int);
void SelectThiefnetItem(THIEFNET *, int);
void *CreateSfxSound(int);
void SetSndSourceLo(void *, int);
// ThiefNet stick handler (slot 14): left / right switch tabs, up / down move the selection (with sounds). (inferred)
void SetThiefnetStickDir(THIEFNET *p, unsigned int n)
{
    if (n == p->nMode)
        return;
    p->nMode = n;
    p->modeStartTime = D_002DDED0.realTime;
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
        if (p->iSelected > 0) {
            SelectThiefnetItem(p, p->iSelected - 1);
            SetSndSourceLo(CreateSfxSound(0x35), 0);
        } else {
            SetSndSourceLo(CreateSfxSound(0x36), 0);
        }
        break;
    case 5:
    case 6:
    case 7:
        if (p->iSelected < p->citem - 1) {
            SelectThiefnetItem(p, p->iSelected + 1);
            SetSndSourceLo(CreateSfxSound(0x35), 0);
        } else {
            SetSndSourceLo(CreateSfxSound(0x36), 0);
        }
        break;
    }
}
} } // namespace M_func_001DC4B0

namespace M_BuildGadgetMenuInfoText { extern "C" {
typedef struct { int pad[5]; int nameTextId; int detailTextId; int descriptionTextId; } REC;
extern GADGETMENU D_003DA0F0;
extern char D_003DD490[];
extern REC D_002BCD00[];
extern char *D_002C3268[];
extern char D_002A7A10[], D_002A7A18[], D_002A7A28[];
char *FindSwLocalizedString__FP2SWi(void *, int);
char *strcpy(char *, char *);
int sprintf(char *, char *, ...);
GMITEM *FindScreenEntry(GADGETMENU *,int);
void *GetHeroPo(void *);
int GetGadgetSlotFromId(void *, int);
// Text callback for the gadget-menu info boxes: the popup message, "none", or the selected power-up's name, slot button
// and description.
void BuildGadgetMenuInfoText(char *out)
{
    char buf[256];
    unsigned char *s;
    if (D_003DA0F0.nPopup != -1) {
        strcpy(out, FindSwLocalizedString__FP2SWi(D_003DD490, D_003DA0F0.idsPopup));
    } else if (D_003DA0F0.idSelected == -1) {
        strcpy(out, FindSwLocalizedString__FP2SWi(D_003DD490, 0xDD));
    } else {
        GMITEM *po = FindScreenEntry(&D_003DA0F0,D_003DA0F0.idSelected);
        int v = GetGadgetSlotFromId(GetHeroPo(D_003DD490), D_003DA0F0.idSelected);
        if (po->fEquippable && v < 0) {
            strcpy(buf, FindSwLocalizedString__FP2SWi(D_003DD490, 0xE1));
            s = (unsigned char *)D_002A7A10;
        } else {
            char *sz;
            if (v >= 0)
                sz = D_002C3268[v];
            else
                sz = D_002A7A10;
            sprintf(buf, FindSwLocalizedString__FP2SWi(D_003DD490, D_002BCD00[D_003DA0F0.idSelected].descriptionTextId), sz, sz, sz);
            s = (unsigned char *)FindSwLocalizedString__FP2SWi(D_003DD490, D_002BCD00[D_003DA0F0.idSelected].detailTextId);
        }
        sprintf(out, D_002A7A18, FindSwLocalizedString__FP2SWi(D_003DD490, D_002BCD00[D_003DA0F0.idSelected].nameTextId),
                      *s ? D_002A7A28 : D_002A7A10, s, buf);
    }
}
} } // namespace M_BuildGadgetMenuInfoText

namespace M_BuildThiefnetPowerupNameText { extern "C" {
extern THIEFNET D_003DA150;
struct T { int a[5]; int id; int b[2]; };
extern struct T D_002BCD00[];
extern struct G D_003DD490;
void *FindSwLocalizedString__FP2SWi(struct G *g, int n);
void strcpy(char *p, void *q);
// Text callback: the power-up name of list row i.
void BuildThiefnetPowerupNameText(char *pch, int i) {
    THIEFNET *pg = &D_003DA150;
    unsigned int n = pg->aitem[pg->iFirstVisible + i].id;
    int id = -1;
    if (n < 0x2B) id = D_002BCD00[n].id;
    if (id != -1) strcpy(pch, FindSwLocalizedString__FP2SWi(&D_003DD490, id));
    else *pch = 0;
}
} } // namespace M_BuildThiefnetPowerupNameText

namespace M_func_001DC820 { extern "C" {
extern THIEFNET D_003DA150;
struct T { int a[2]; char *pch; int b[5]; };
extern struct T D_002BCD00[];
extern char D_002A79A0[];
void sprintf(char *pch, char *fmt, char *s);
// Text callback: the power-up cost of list row i. (inferred)
void BuildThiefnetPowerupCostText(char *pch, int i) {
    THIEFNET *pg = &D_003DA150;
    unsigned int n = pg->aitem[pg->iFirstVisible + i].id;
    char *s = 0;
    if (n < 0x2B) s = D_002BCD00[n].pch;
    if (s) sprintf(pch, D_002A79A0, s);
    else *pch = 0;
}
} } // namespace M_func_001DC820

namespace M_BuildThiefnetLootNameText {
struct REC { char *name; int ids; int pad; };
struct G;
extern "C" { extern G D_003DD490; }
extern "C" { extern THIEFNET D_003DA150; }
extern "C" { extern REC D_002C3288[]; }
extern "C" { extern char D_002A79A0[]; }
extern "C" { extern char D_002A79D0[]; }
extern "C" { extern char D_002A79D8[]; }
extern "C" { extern char D_002A7A30[]; }
extern "C" int sprintf(char *, const char *, ...);
extern "C" char *strcpy(char *, const char *);
extern "C" CRef GetSaveVarByName(const char *, ...);
extern "C" { char *FindSwLocalizedString__FP2SWi(G *, int); }
// Text callback: the loot name of list row i, with the count when more than one is owned.
extern "C" void BuildThiefnetLootNameText(char *pch, int k)
{
    int ids = -1;
    int c = 0;
    unsigned int i = D_003DA150.aitem[D_003DA150.iFirstVisible + k].id;
    if (i < 0x40) {
        CRef ref;
        char ach[0x20];
        sprintf(ach, D_002A79A0, i);
        ref = GetSaveVarByName(D_002A79D0, D_002A79D8, ach, 0);
        c = ref.u.n;
        ids = D_002C3288[i].ids;
    }
    if (ids != -1) {
        strcpy(pch, FindSwLocalizedString__FP2SWi(&D_003DD490, ids));
        if (c >= 2)
            sprintf(pch, D_002A7A30, pch, c);
    } else {
        *pch = 0;
    }
}
} // namespace M_BuildThiefnetLootNameText

namespace M_func_001DC9C0 { extern "C" {
extern THIEFNET D_003DA150;
struct T { char *pch; int b[2]; };
extern struct T D_002C3288[];
extern char D_002A79A0[];
void sprintf(char *pch, char *fmt, char *s);
// Text callback: the loot sale price of list row i. (inferred)
void BuildThiefnetLootPriceText(char *pch, int i) {
    THIEFNET *pg = &D_003DA150;
    unsigned int n = pg->aitem[pg->iFirstVisible + i].id;
    char *s = 0;
    if (n < 0x40) s = D_002C3288[n].pch;
    if (s) sprintf(pch, D_002A79A0, s);
    else *pch = 0;
}
} } // namespace M_func_001DC9C0

namespace M_func_001DCA30 { extern "C" {
extern THIEFNET D_003DA150;
#define D_003DA180 (*(int *)((char *)base_D_003DA150 + 0x30))   /* screen's object+0x30 */
extern struct G D_003DD490;
void *FindSwLocalizedString__FP2SWi(struct G *g, int n);
void strcpy(char *p, void *q);
// Text callback: the popup prompt text for the popup mode. (inferred)
void BuildThiefnetPromptText(char *pch) {
    int id;
    switch (D_003DA150.nPopup) {
    case -1: id = 0xDE; break;
    case 0: id = 0xDF; break;
    case 1: id = 0xE0; break;
    default: *pch = 0; return;
    }
    strcpy(pch, FindSwLocalizedString__FP2SWi(&D_003DD490, id));
}
} } // namespace M_func_001DCA30
#undef D_003DA180

namespace M_BuildThiefnetInfoText { extern "C" {
typedef struct { int unknown0,category,cost,powerCost,unlockEpisode,nameTextId,detailTextId,descriptionTextId; } REC;   /* a power-up (D_002BCD00) */
typedef struct { char *name; int ids,pad; } REC2;
extern THIEFNET D_003DA150;
extern REC D_002BCD00[];
extern REC2 D_002C3288[];
extern struct G D_003DD490;
extern char D_002DEB30[],D_002A7A38[];
char *FindSwLocalizedString__FP2SWi(struct G *,int);
int GetCoinCount(void *);
int sprintf(char *,const char *,...);
char *strcpy(char *,const char *);
// Copy the localized string `id` into p.
static inline void CopyId(char *p,int id)
{
    strcpy(p,FindSwLocalizedString__FP2SWi(&D_003DD490,id));
}
// Text callback: the popup message, or the selected power-up's name, description and affordability, or the loot sell
// line.
void BuildThiefnetInfoText(char *pch)
{
    THIEFNET *pt=&D_003DA150;
    if(pt->nPopup!=-1) {
        CopyId(pch,pt->idsPopup);
        return;
    } else {
        switch(pt->nTab) {
        case 0: {
            unsigned int i=pt->idPowerupSelected;
            if(i<0x2B) {
                if(D_002BCD00[i].detailTextId!=pt->nPopup) {   /* timerKind is -1 here: "has a detail text" */
                    int *pc=&D_002BCD00[i].cost;
                    int count=GetCoinCount(D_002DEB30);
                    int idsT=count<*pc?0xD9:0xDA;
                    struct G *pg=&D_003DD490;
                    char *fmt=D_002A7A38;
                    char *pch1=FindSwLocalizedString__FP2SWi(pg,D_002BCD00[i].nameTextId);
                    char *pch2=FindSwLocalizedString__FP2SWi(pg,D_002BCD00[i].detailTextId);
                    sprintf(pch,fmt,pch1,pch2,FindSwLocalizedString__FP2SWi(pg,idsT));
                    return;
                }
            }
            CopyId(pch,0xDC);
            return;
        }
        case 1: {
            unsigned int i=pt->idLootSelected;
            if(i<0x40) {
                sprintf(pch,FindSwLocalizedString__FP2SWi(&D_003DD490,0xDB),D_002C3288[i].name);
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
extern char D_002D8468[] __asm__("_vt$7JOBLIST");
JOBLIST *ConstructScreen(JOBLIST *p);
void ClearJobList(JOBLIST *p);
// Constructor of the job-list screen (widget 0x4E0). (inferred)
JOBLIST *ConstructJobListScreen(JOBLIST *p) {
    ConstructScreen(p);
    *(void **)((char *)p + 0xC) = D_002D8468;
    p->idTextQueued = 0x4E0;
    ClearJobList(p);
    return p;
}
} } // namespace M_func_001DCC58

// Job-list post-load: base post-load, then marks the widget.
void JOBLIST::PostLoad()   /* key function */
{
    SCREEN *screen = (SCREEN *)this;
    PostLoadScreen(screen);
    if (screen->pchild != 0) {
        ((SCRCHILD *)screen->pchild)->unknownB0 = 1;
    }
}

// Job-list reset (slot 13).
extern "C" void ResetJobList(SCREEN *screen)
{
    ResetScreen(screen);
    ClearJobList(screen);
}

namespace M_ClearJobList { extern "C" {
extern CLOCK D_002DDED0; /* +0x3C: the real (unscaled) clock */
// Job list: state -1, time stamp, no animation handles.
void ClearJobList(JOBLIST *p) {
    p->nPage = -1; //P
    p->tPage = D_002DDED0.realTime; //P
    p->idAnimPopup = -1; //P
    p->idAnimScrollUp = -1; //P
    p->idAnimScrollDown = -1; //P
}
} } // namespace M_ClearJobList

namespace M_UpdateJobList { extern "C" {
typedef struct VT { struct { short d; short pad; void (*pfn)(void *); } a[32]; } VT;
typedef struct { char pad[0xC]; VT *vt; char pad10[0x58 - 0x10]; int visibility; } W;
void UpdateTimedScreen(JOBLIST *);
int AreWidgetLoadsIdle(void);
void SetWidgetState(void *, int);
void SetWidgetRequest(void *, int);
int HandleJoyButtons(void *, int);
void SetJobListPage(JOBLIST *, int);
void UpdateScreenStickDir(JOBLIST *);
void UpdateJobListWidgets(JOBLIST *);
// Per-frame job list: waits for the widget to load; START / circle closes; direction buttons switch between the jobs page
// and the tasks page.
void UpdateJobList(JOBLIST *p)
{
    UpdateTimedScreen(p);
    if (p->nPage == -1)
        return;
    if (p->pjoy == 0)
        return;
    if (((W *)p->pchild)->visibility == 0) {
        if (!AreWidgetLoadsIdle())
            return;
        SetWidgetState(p->pchild, 1);
        SetWidgetRequest(p->pchild, 2);
        ((W *)p->pchild)->vt->a[16].pfn((char *)p->pchild + ((W *)p->pchild)->vt->a[16].d);
    }
    if (HandleJoyButtons(p->pjoy, 0x800)) {
        SetJobListPage(p, -1);
        return;
    }
    switch (p->nPage) {
    case 0:
        if (HandleJoyButtons(p->pjoy, 0x10)) {
            SetJobListPage(p, -1);
            return;
        }
        if (p->centry && HandleJoyButtons(p->pjoy, 0xF0)) {
            SetJobListPage(p, 1);
            return;
        }
        break;
    case 1:
        if (!p->fFromJobs && HandleJoyButtons(p->pjoy, 0x10)) {
            SetJobListPage(p, -1);
            return;
        }
        if (HandleJoyButtons(p->pjoy, 0xF0)) {
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
void CloseScreen(JOBLIST *p);
// Selects a job-list row (clamped).
void SelectJobListItem(JOBLIST *p, int i) {
    int entryCount = p->centry;
    int n;
    if (entryCount == 0) n = -1;
    else n = ClampInt(i, 0, entryCount - 1);
    p->iSelected = n;
    CloseScreen(p);
}
} } // namespace M_SelectJobListItem

namespace M_RefreshJobListEntries {
struct SO {
    char pad0[0x54];
    int taskState;
    char pad58[0x5C - 0x58];
    int taskTextId;
    char pad60[0x6C - 0x60];
    int jobTextId;
    char pad70[0x7C - 0x70];
    SO *job;
};
extern "C" {
void StopWidgetAnim(void *, int);
void func_002333B4(void *, int, unsigned int) __asm__("memset");
void SortTasksTopologically(int *, SO ***);
void qsort(void *, int, int, int (*)(const void *, const void *));
int CompareTasks(const void *, const void *);
int GetJobState(SO *);
void memmove(void *, void *, unsigned int);
void SelectJobListItem(JOBLIST *, int);
}
// Rebuilds the job list: tasks sorted by dependency then order; page 0 lists started jobs, page 1 the tasks of the
// selected job up to the first unfinished one.
extern "C" void RefreshJobListEntries(JOBLIST *p)
{
    for (int i = 0; i < p->centry; i++) {
        JLENTRY *e = &p->aentry[i];
        if (e->idAnimIcon != -1) {
            StopWidgetAnim(p->pchild, e->idAnimIcon);
            e->idAnimIcon = -1;
        }
        if (e->idAnimCheck != -1) {
            StopWidgetAnim(p->pchild, e->idAnimCheck);
            e->idAnimCheck = -1;
        }
    }
    p->iSelected = 0;
    p->centry = 0;
    func_002333B4(p->aentry, 0, p->centry * sizeof(JLENTRY));
    STACKSCOPE frame;
    int c;
    SO **aso;
    SortTasksTopologically(&c, &aso);
    qsort(aso, c, 4, CompareTasks);
    switch (p->nPage) {
    case 0: {
        SO *last = 0;
        for (int i = 0; i < c; i++) {
            SO *so = aso[i]->job;
            if (so && last != so) {
                last = so;
                if (GetJobState(so) > 0 && so->jobTextId != -1) {
                    JLENTRY *e = &p->aentry[p->centry++];
                    e->ptask = so;
                    e->idAnimCheck = -1;
                    e->idAnimIcon = -1;
                    if (p->pjobSelected == so)
                        p->iSelected = e - p->aentry;
                }
            }
        }
        break;
    }
    case 1: {
        int flag = 0;
        for (int i = 0; i < c; i++) {
            SO *so = aso[i];
            if (p->pjobSelected == so->job && so->taskTextId != -1) {
                if (flag && !so->taskState)
                    break;
                JLENTRY *e = &p->aentry[p->centry++];
                e->ptask = so;
                e->idAnimCheck = -1;
                e->idAnimIcon = -1;
                if (so->taskState == 1)
                    flag = 1;
                if (!so->taskState)
                    break;
            }
        }
        break;
    }
    }
    if (p->centry > p->centryMax) {
        memmove(p->aentry, &p->aentry[p->centry - p->centryMax], p->centryMax * sizeof(JLENTRY));
        int d = p->centry - p->centryMax;
        if (d < p->iSelected)
            p->iSelected -= d;
        else
            p->iSelected = 0;
        p->centry = p->centryMax;
    }
    SelectJobListItem(p, p->iSelected);
}
} // namespace M_RefreshJobListEntries

namespace M_UpdateJobListWidgets { extern "C" {
typedef struct { char pad[0x58]; int visibility; } W;   /* widget: 2 = fully shown */
void SwitchWidgetAnim(void *, int, int *);
int IsJobComplete(void *);
int IsTaskComplete(void *);
// Drives the job-list row highlights and completion check marks.
void UpdateJobListWidgets(JOBLIST *p)
{
    int i;
    if (((W *)p->pchild)->visibility != 2)
        return;
    for (i = 0; i < p->centry; i++) {
        int f;
        JLENTRY *pe = &p->aentry[i];
        SwitchWidgetAnim(p->pchild, i == p->iSelected ? i + 0x4FD : i + 0x4E9, &pe->idAnimIcon);
        f = 0;
        switch (p->nPage) {
        case 0:
            f = IsJobComplete(pe->ptask);
            break;
        case 1:
            f = IsTaskComplete(pe->ptask);
            break;
        }
        SwitchWidgetAnim(p->pchild, f ? i + 0x507 : -1, &pe->idAnimCheck);
    }
}
} } // namespace M_UpdateJobListWidgets

namespace M_SetJobListPage { extern "C" {
typedef struct { short d; short pad; void *pfn; } VTE;
typedef struct SUB {
    char pad0[0xC];
    VTE *vt;
    char pad10[0x58 - 0x10];
    int visibility;
    char pad5c[0x70 - 0x5C];
    int pageAnimSecond;
    int pageAnimFirst;
    int pageAnimThird;
} SUB;
extern CLOCK D_002DDED0; /* +0x3C: the real (unscaled) clock */
extern char D_003DD490[];
void IncrementSwHandsOff(void *, int);
void DecrementSwHandsOff(void *, int);
void SetWidgetState(void *, int);
void SetWidgetRequest(void *, int);
void RefreshJobListEntries(JOBLIST *);
#define VCALL(o, n) ((void (*)(void *))(o)->vt[n].pfn)((char *)(o) + (o)->vt[n].d)
// Opens the job list on a page (jobs or tasks) or closes it (-1), holding player control while open.
void SetJobListPage(JOBLIST *p, int s)
{
    int old;
    if (!p->pchild)
        s = -1;
    if (s == p->nPage)
        return;
    if (p->nPage == -1) {
        IncrementSwHandsOff(D_003DD490, 0);
        p->Show();
    }
    old = p->nPage;
    p->nPage = s;
    p->tPage = D_002DDED0.realTime;
    switch (s) {
    case -1:
        DecrementSwHandsOff(D_003DD490, 0);
        p->Hide();
        if (((SUB *)p->pchild)->visibility > 0)
            SetWidgetRequest(p->pchild, 1);
        p->idAnimPopup = s;
        p->idAnimScrollUp = s;
        p->idAnimScrollDown = s;
        break;
    case 0:
        p->centryMax = 0xA;
        ((SUB *)p->pchild)->pageAnimSecond = 0x4E3;
        ((SUB *)p->pchild)->pageAnimThird = 0x4E2;
        if (old == 1)
            ((SUB *)p->pchild)->pageAnimFirst = 0x4E8;
        else
            ((SUB *)p->pchild)->pageAnimFirst = 0x4E1;
        if (((SUB *)p->pchild)->visibility) {
            SetWidgetState(p->pchild, 1);
            SetWidgetRequest(p->pchild, 2);
            VCALL(((SUB *)p->pchild), 16);
        }
        break;
    case 1: {
        int i = p->iSelected;
        if (i >= 0 && i < p->centry)
            p->pjobSelected = p->aentry[i].ptask;
        else
            p->pjobSelected = g_sw.currentJob;
        p->fFromJobs = (old == 0);
        p->centryMax = 4;
        ((SUB *)p->pchild)->pageAnimSecond = 0x4E6;
        ((SUB *)p->pchild)->pageAnimThird = 0x4E5;
        if (old == 0)
            ((SUB *)p->pchild)->pageAnimFirst = 0x4E7;
        else
            ((SUB *)p->pchild)->pageAnimFirst = 0x4E4;
        if (((SUB *)p->pchild)->visibility) {
            SetWidgetState(p->pchild, 1);
            SetWidgetRequest(p->pchild, 2);
            VCALL(((SUB *)p->pchild), 16);
        }
        break;
    }
    }
    RefreshJobListEntries(p);
    p->SetMode(-1);
}
} } // namespace M_SetJobListPage
#undef VCALL

namespace RO_ORDER { extern const char r_D_002A7A10[] __asm__("D_002A7A10") = ""; }
namespace RO_ORDER { extern const char r_D_002A7A18[] __asm__("D_002A7A18") = "%s%s%s. %s"; }
namespace RO_ORDER { extern const char r_D_002A7A28[] __asm__("D_002A7A28") = ": "; }
namespace RO_ORDER { extern const char r_D_002A7A30[] __asm__("D_002A7A30") = "%s (%d)"; }
namespace RO_ORDER { extern const char r_D_002A7A38[] __asm__("D_002A7A38") = "%s: %s. %s"; }
namespace M_SetJobListStickDir { extern "C" {
extern CLOCK D_002DDED0; /* +0x3C: the real (unscaled) clock */
void SetJobListPage(JOBLIST *, int);
void SelectJobListItem(JOBLIST *, int);
void *CreateSfxSound(int);
void SetSndSourceLo(void *, int);
// Job-list stick handler (slot 14): left / right switch pages, up / down move the selection.
void SetJobListStickDir(JOBLIST *p, unsigned int n)
{
    if (n == p->nMode)
        return;
    p->nMode = n;
    p->modeStartTime = D_002DDED0.realTime;
    switch (n) {
    case 0:
        if (p->centry)
            SetJobListPage(p, 1);
        break;
    case 4:
        SetJobListPage(p, 0);
        break;
    case 1:
    case 2:
    case 3:
        if (p->iSelected > 0) {
            SelectJobListItem(p, p->iSelected - 1);
            SetSndSourceLo(CreateSfxSound(0x35), 0);
        }
        break;
    case 5:
    case 6:
    case 7:
        if (p->iSelected < p->centry - 1) {
            SelectJobListItem(p, p->iSelected + 1);
            SetSndSourceLo(CreateSfxSound(0x35), 0);
        }
        break;
    }
}
} } // namespace M_SetJobListStickDir

namespace M_func_001DD648 { extern "C" {
extern char D_002D83E8[] __asm__("_vt$7SCRN52D");
SCRN52D *ConstructScreen(SCRN52D *p);
void ResetScreenMode(SCRN52D *p);
// Constructor of screen 0x52D (widget 0x52D; its role is unknown). (inferred)
SCRN52D *ConstructScreen52D(SCRN52D *p) {
    ConstructScreen(p);
    *(void **)((char *)p + 0xC) = D_002D83E8;
    p->idTextQueued = 0x52D;
    ResetScreenMode(p);
    return p;
}
} } // namespace M_func_001DD648

// Post-load of screen 0x52D: base post-load, then marks the widget.
void SCRN52D::PostLoad()   /* key function */
{
    SCREEN *screen = (SCREEN *)this;
    PostLoadScreen(screen);
    if (screen->pchild != 0) {
        ((SCRCHILD *)screen->pchild)->unknownB0 = 1;
    }
}

namespace M_func_001DD6C8 { extern "C" {
// Shows screen 0x52D and its widget. (inferred)
void ShowScreen52D(SCRN52D *p) {
    if (p->pchild) {
        p->Show();   /* slot 10 */
        {
            WIDGET *po = (WIDGET *)p->pchild;   /* the widget */
            po->Show(-1);
        }
    }
}
} } // namespace M_func_001DD6C8

namespace M_func_001DD728 { extern "C" {
// Hides screen 0x52D and its widget. (inferred)
void HideScreen52D(SCRN52D *p) {
    if (p->pchild) {
        p->Hide();   /* slot 11 */
        {
            WIDGET *po = (WIDGET *)p->pchild;   /* the widget */
            po->Hide();
        }
    }
}
} } // namespace M_func_001DD728

namespace M_func_001DD780 { extern "C" {
struct O { char pad[0x58]; int visibility; char p1[8]; int requestedVisibility; };   /* widget: +0x58 shown state (2 = fully shown), +0x64 requested (inferred) */
// True when the screen's widget is fully shown. (inferred)
int IsScreen52DShown(SCRN52D *p) {
    struct O *po = (struct O *)p->pchild;
    if (!po) return 0;
    if (po->visibility != 2) return 0;
    return po->requestedVisibility == 2;
}
} } // namespace M_func_001DD780

/* slot 14: set the mode (stamping the real time). */
// Screen 0x52D's slot 14: records a new state and the time it changed. (inferred)
extern "C" void SetScreenState(SCREEN *screen, int mode)
{
    if (mode != screen->nMode) {
        screen->nMode = mode;
        screen->modeStartTime = D_002DDED0.realTime;
    }
}

namespace M_func_001DD7D8 { extern "C" {
extern char D_002D8368[] __asm__("_vt$5TITLE");
TITLE *ConstructScreen(TITLE *p);
void ResetScreenMode(TITLE *p);
// Constructor of the title screen (widget 0x5D2). (inferred)
TITLE *ConstructTitleScreen(TITLE *p) {
    ConstructScreen(p);
    *(void **)((char *)p + 0xC) = D_002D8368;
    p->idTextQueued = 0x5D2;
    ResetScreenMode(p);
    return p;
}
} } // namespace M_func_001DD7D8

namespace M_func_001DD820 { extern "C" {
void EnsureRumbleModeDefault(void);
// Shows the title screen and its widget and resets rumble. (inferred)
void ShowTitleScreen(TITLE *p) {
    if (p->pchild) {
        p->Show();   /* slot 10 */
        {
            WIDGET *po = (WIDGET *)p->pchild;   /* the widget */
            po->Show(-1);
        }
        EnsureRumbleModeDefault();
    }
}
} } // namespace M_func_001DD820

namespace M_HideTitleScreen { extern "C" {
// Hides the title screen and its widget.
void HideTitleScreen(TITLE *p) {
    if (p->pchild) {
        p->Hide();   /* slot 11 */
        {
            WIDGET *po = (WIDGET *)p->pchild;   /* the widget */
            po->Hide();
        }
    }
}
} } // namespace M_HideTitleScreen

namespace M_SelectLanguageItem { extern "C" {
struct S { char p0[8]; void *p8; };
extern char D_003DD490[];
void SetLanguage(void *p);
void WipeToWorldWarp(void *g, int a, int b, int c, int d, int e);
// Language-menu item callback: sets the language and warps to the title world.
void SelectLanguageItem(struct S *p) {
    SetLanguage(p->p8);
    WipeToWorldWarp(D_003DD490, 0, 0x189, -1, 1, -1);
}
} } // namespace M_SelectLanguageItem

namespace M_UpdateTitleScreen { extern "C" {
typedef struct { int ids; void (*pfn)(void); int k; int pad[5]; } ITEM;
extern SAVE D_002E65E0;
extern char D_002E6550[];
extern struct G D_003DD490;
extern char D_003D9ED0[];
extern ITEM D_003DA578[];
extern int D_002C35B8[];
extern char D_002C35F0[];
void SelectLanguageItem(void);
void UpdateTimedScreen(TITLE *);
int HandleJoyButtons(JOY *, int);
int GetJoyButtonsPressed(JOY *, int);
void SetJoyBtnHandled(JOY *, int);
void OpenPauseOptionsPage(int);
void SetMcDialogPhase(void *, int);
void StartGameFromSave(void *);
void StartNewGame(TITLE *);
int CountRegionLanguages(void);
void *FindSwObject__FP2SWi3OIDP2LO(struct G *, int, int, int);
void GetRegionLanguages(int *, int **);
void *func_002333B4(void *, int, int) __asm__("memset");
void PushPauseMenuPage(void *, int, ITEM *, void *, void *);
// Per-frame title screen: START continues from the memory card or starts a new game; SELECT opens the options; X opens
// the language list when the region has several languages.
void UpdateTitleScreen(TITLE *p)
{
    void *pv;
    int c;
    int *ak;
    int i;
    UpdateTimedScreen(p);
    if (p->pchild == 0)
        return;
    if (p->pjoy == 0)
        return;
    if (HandleJoyButtons(p->pjoy, 0x800)) {
        if ((D_002E65E0.currentSlot = D_002E65E0.newestSlot) != 0) {
            SetMcDialogPhase(D_002E6550, 1);
            StartGameFromSave(D_002E65E0.currentSlot);
        } else {
            StartNewGame(p);
        }
        return;
    }
    if (GetJoyButtonsPressed(p->pjoy, 0x100) && (p->pjoy->buttonsHeld & 0x108) == 0x100) {
        SetJoyBtnHandled(p->pjoy, 0x100);
        OpenPauseOptionsPage(0);
        return;
    }
    if (CountRegionLanguages() < 2)
        return;
    if (!HandleJoyButtons(p->pjoy, 0x10))
        return;
    pv = FindSwObject__FP2SWi3OIDP2LO(&D_003DD490, 5, 0x5D6, 0);
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
    PushPauseMenuPage(D_003D9ED0, c, D_003DA578, pv, D_002C35F0);
}
} } // namespace M_UpdateTitleScreen

namespace M_StartNewGame {
extern "C" { void *__4CRefRC4CRef(void *, const void *); void Decref__4CRef(void *); }
struct G;
extern "C" { extern G D_003D4A60; }
extern "C" { 
 }
extern "C" { extern void *D_002E1E40; }
extern "C" { extern void *D_002DD5BC; }
extern "C" { extern void *D_002F7900; }
extern "C" {
CRef SetGlobalSaveVar(void *, const CRef &, G *);
void BindStepplayerHpMp(void *, int);
void StartGameFromSave(G *);
}
/* The unused third parameter is deliberate: it is the same SetPv form as SetBootyContainerKind / ClearStepplayerBootySave,
   and the dummy makes gcc keep the %hi/%lo of D_003D4A60 in s0 across the whole function. */
static inline void SetPv(void *pv, const CRef &r, void *) { CRef arg(r); const CRef &ret = SetGlobalSaveVar(pv, arg, &D_003D4A60); }
// Starts a new game: clears the save flag, binds the three heroes' health and power, and starts play.
extern "C" void StartNewGame(void)
{
    {
        CRef ref(0, 0x16);
        SetPv((*(void * *)&g_sw.psvpDt), ref, &D_003D4A60);
    }
    if (D_002E1E40)
        BindStepplayerHpMp(D_002E1E40, 1);
    if (D_002DD5BC)
        BindStepplayerHpMp(D_002DD5BC, 1);
    if (D_002F7900)
        BindStepplayerHpMp(D_002F7900, 1);
    StartGameFromSave(&D_003D4A60);
}
} // namespace M_StartNewGame

/* slot 14: set the mode (stamping the real time). */
// Title screen stick handler (slot 14): records the direction and the time.
void TITLE::SetMode(int mode)   /* key function */
{
    SCREEN *screen = (SCREEN *)this;
    if (mode != screen->nMode) {
        screen->nMode = mode;
        screen->modeStartTime = D_002DDED0.realTime;
    }
}

namespace M_BuildTitlePromptText {
extern "C" { extern SW D_003DD490; }
extern "C" { extern JOY D_002DFC00; }
extern "C" { 
extern SAVE D_002E65E0;
 }
extern "C" { extern char D_002A7A70[]; }
extern "C" char *strcpy(char *, const char *);
extern "C" char *strcpy1(char *, const char *);
extern "C" int CountRegionLanguages(void);
extern "C" { CRef GetGlobalSaveVar(void *, void *); }
// Text callback: the title prompt (continue or new game, options, language).
extern "C" void BuildTitlePromptText(char *pch)
{
    float g;
    void *pv;
    if (D_002DFC00.joys == 1 && D_002DFC00.joyk == 0) {
        strcpy(pch, FindSwLocalizedString(&D_003DD490, 9));
        return;
    }
    g = 0.0f;
    pv = D_002E65E0.newestSlot;
    if (pv) {
        CRef ref = GetGlobalSaveVar(D_003DD490.psvpDt, pv);
        g = ref.u.g;
    }
    {
        int ids = g != 0.0f ? 6 : 5;
        strcpy(pch, FindSwLocalizedString(&D_003DD490, ids));
    }
    strcpy1(pch, D_002A7A70);
    strcpy1(pch, FindSwLocalizedString(&D_003DD490, 8));
    if (CountRegionLanguages() >= 2) {
        strcpy1(pch, D_002A7A70);
        strcpy1(pch, FindSwLocalizedString(&D_003DD490, 0x56));
    }
}
} // namespace M_BuildTitlePromptText

namespace RO_ORDER { extern const char r_D_002A7A70[] __asm__("D_002A7A70") = "\n"; }
namespace M_IdsFromWorld { extern "C" {
// Episode-name string id of a world index (0-8).
int GetWorldNameTextId(int n) {
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

// Text callback: the current episode's name.
extern "C" void BuildJobListWorldText(char *pch)
{
    StrcpyCall(pch, FindSwLocalizedString(&D_003DD490, GetWorldNameTextId(GetCurrentWorld())));
}

// Text callback: the selected job's description. (inferred)
extern "C" void BuildJobDescriptionText(char *pch)
{
    if (D_003DA4D0 != 0) {
        StrcpyCall(pch, FindSwLocalizedString(&D_003DD490, D_003DA4D0->descriptionTextId));
    }
}

// Text callback: the selected job's name. (inferred)
extern "C" void BuildJobNameText(char *pch)
{
    if (D_003DA4D0 != 0) {
        StrcpyCall(pch, FindSwLocalizedString(&D_003DD490, D_003DA4D0->labelNameId));
    }
}

namespace M_BuildJobListRowText { extern "C" {
struct O { char pad[0x6C]; int n; };
extern JOBLIST D_003DA3C0;
extern struct G D_003DD490;
void *FindSwLocalizedString__FP2SWi(struct G *g, int n);
void strcpy(void *p, void *q);
// Text callback: the job name of row i.
void BuildJobListRowText(void *p, int i) {
    JOBLIST *pg = &D_003DA3C0;
    if (i < pg->centry) strcpy(p, FindSwLocalizedString__FP2SWi(&D_003DD490, ((struct O *)pg->aentry[i].ptask)->n));
}
} } // namespace M_BuildJobListRowText

namespace M_BuildJobListTaskRowText { extern "C" {
struct O { char pad[0x5C]; int n; };
extern JOBLIST D_003DA3C0;
extern struct G D_003DD490;
void *FindSwLocalizedString__FP2SWi(struct G *g, int n);
void strcpy(void *p, void *q);
// Text callback: the task name of row i.
void BuildJobListTaskRowText(void *p, int i) {
    JOBLIST *pg = &D_003DA3C0;
    if (i < pg->centry) strcpy(p, FindSwLocalizedString__FP2SWi(&D_003DD490, ((struct O *)pg->aentry[i].ptask)->n));
}
} } // namespace M_BuildJobListTaskRowText

// Text callback: a title-screen string followed by a button glyph. (inferred)
extern "C" void BuildTitleScreenButtonText(char *pch)
{
    StrcpyCall(pch, FindSwLocalizedString(&D_003DD490, 0x40));
    strcpy1(pch, D_002A7AA8);
}

// Text callback: the game completion percentage.
extern "C" void BuildTitleCompletionText(char *pch)
{
    sprintf(pch, D_002A7AB8, GetCompletionPercent((*(char (*)[])&g_sw.savevars), D_003D4A60));
}


namespace M_func_00258348 { extern "C" {
typedef struct R1 { int cref; int unknown4; int unknown8; int ref[2]; int varbRef[1]; } R1;
typedef struct R2 { int cref; } R2;
typedef struct R4 { int cref; int x4; R1 *p8; } R4;
extern char D_004520D0[] __asm__("_t9CSlotPool2i24i2048$s_pool"), D_00452148[] __asm__("_t9CSlotPool2i12i200$s_pool");
void func_00257250(void *, int) __asm__("_$_t7CRefPtr1Z5CVarb");
void Decref__4CRef(void *);
void Unlink__t6CGcObj1Z5CVarb(void *);
void Unlink__t6CGcObj1Z8CSidebag(void *);
void FreeSlotheapSlot(void *, void *);
void func_00257078(void *, int) __asm__("_$_6CFrame");
void func_001AE520(void *) __asm__("__builtin_delete");

// Drop a reference to an R1 record; on the last one release it and free its slot.
static inline void Release1(R1 *r)
{
    if (r) {
        if (--r->cref == 0) {
            func_00257250(r->varbRef, 2);
            Decref__4CRef(r->ref);
            Unlink__t6CGcObj1Z5CVarb(r);
            FreeSlotheapSlot(D_004520D0, r);
        }
    }
}

// Drop a reference to an R4 record; on the last one release it and free its slot.
static inline void Release4(R4 *r)
{
    if (r) {
        if (--r->cref == 0) {
            Release1(r->p8);
            Unlink__t6CGcObj1Z8CSidebag(r);
            FreeSlotheapSlot(D_00452148, r);
        }
    }
}

typedef struct S { R2 *p0; R4 *p4; } S;

// BASIC's destructor (inline): releases the sidebag and Splice frame references.
inline void DestroyBasic(S *p, int flags)
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
extern const char D_002A7AA8[] = " : &2T&.";
extern const char D_002A7AB8[] = "%d%%";
/* Gadget menu entries (GMITEM: power-up id, three animation ids, -1, x, y, equippable); 18/9/8 entries for Sly,
   Bentley, Murray. */
GMITEM D_002C2DE8[18] = {{23, 978, 977, 979, -1, 145.0f, 155.0f, 1}, {24, 983, 982, 984, -1, 225.0f, 155.0f, 1},
    {25, 988, 987, 989, -1, 305.0f, 155.0f, 1}, {26, 993, 992, 994, -1, 385.0f, 155.0f, 1},
    {29, 1008, 1007, 1009, -1, 465.0f, 155.0f, 1}, {30, 1013, 1012, 1014, -1, 545.0f, 155.0f, 1},
    {33, 1028, 1027, 1029, -1, 145.0f, 235.0f, 1}, {34, 1033, 1032, 1034, -1, 225.0f, 235.0f, 1},
    {35, 1038, 1037, 1039, -1, 305.0f, 235.0f, 1}, {37, 1048, 1047, 1049, -1, 385.0f, 235.0f, 1},
    {38, 1053, 1052, 1054, -1, 465.0f, 235.0f, 1}, {39, 1058, 1057, 1059, -1, 545.0f, 235.0f, 1},
    {27, 998, 997, 999, -1, 145.0f, 315.0f, 0}, {28, 1003, 1002, 1004, -1, 225.0f, 315.0f, 0},
    {40, 1063, 1062, 1064, -1, 305.0f, 315.0f, 1}, {41, 1068, 1067, 1069, -1, 385.0f, 315.0f, 1},
    {42, 1073, 1072, 1074, -1, 465.0f, 315.0f, 1}, {31, 1018, 1017, 1019, -1, 545.0f, 315.0f, 1}};
GMITEM D_002C3028[9] = {{7, 893, 892, 894, -1, 175.0f, 155.0f, 1}, {9, 903, 902, 904, -1, 285.0f, 155.0f, 1},
    {11, 913, 912, 914, -1, 395.0f, 155.0f, 1}, {13, 923, 922, 924, -1, 505.0f, 155.0f, 1},
    {10, 908, 907, 909, -1, 175.0f, 235.0f, 1}, {12, 918, 917, 919, -1, 285.0f, 235.0f, 1},
    {8, 898, 897, 899, -1, 395.0f, 235.0f, 1}, {14, 928, 927, 929, -1, 505.0f, 235.0f, 1},
    {36, 1043, 1042, 1044, -1, 175.0f, 315.0f, 0}};
GMITEM D_002C3148[8] = {{15, 938, 937, 939, -1, 175.0f, 155.0f, 1}, {22, 973, 972, 974, -1, 285.0f, 155.0f, 1},
    {20, 963, 962, 964, -1, 395.0f, 155.0f, 1}, {19, 958, 957, 959, -1, 505.0f, 155.0f, 1},
    {16, 943, 942, 944, -1, 285.0f, 235.0f, 1}, {21, 968, 967, 969, -1, 395.0f, 235.0f, 1},
    {17, 948, 947, 949, -1, 285.0f, 315.0f, 0}, {18, 953, 952, 954, -1, 395.0f, 315.0f, 0}};
SMP D_002C3248 = {100.0f, 0.0f, 0.5f};
float D_002C3254 = 5.0f;
int D_002C3258[3] = {4, 1, 2};
float D_002C3264 = 0.66f;
char *D_002C3268[3] = {(char *)RO_ORDER::r_D_002A7988, (char *)RO_ORDER::r_D_002A7990, (char *)RO_ORDER::r_D_002A7998};
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
int D_003DA570;
namespace M_UpdateTitleScreen { extern "C" { ITEM D_003DA578[14]; } }   /* one per region language */
