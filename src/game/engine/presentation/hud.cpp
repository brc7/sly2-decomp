// hud.cpp -- WIDGET HUD objects, sprite groups and the STRINGGEN text table (docs/systems/ui-and-hud.md section 1).
// TU 0x210EB0..0x21BB88 (124 functions, address order).
// Entry points: the WIDGET class slots (LoadWidgetFromBrx, PostWidgetLoad, UpdateWidget, ShowWidget / HideWidget,
// WIDGET::RenderSelf), sprite-group loading and animation (LoadSpriteGroups, StartSpriteAnim, AdvanceSpriteAnim),
// the rich-text layout used by text sprites, BeginRpHud / EndRpHud (the HUD render pass) and the Build*Text
// callbacks of the STRINGGEN text table.
// The per-function views of most functions are kept verbatim in namespaces named after the function (see
// docs/guides/tu-merging.md "Representation decisions"); the former m2c bodies are written at file scope against
// the views below.
// StaticInit_hud (0x21AA60, 1090 instructions: the 0x60000-byte CLinearHeap D_003E1C70, a level-heap object D_003E1C80
// with a hand-stored vtable, and the 108-entry STRINGGEN table D_003E1C88, 107 records copied from .rodata
// 0x2A8B90.. and one built at run time) keeps the hand-written per-function form, with its _GLOBAL_$I$ stub
// func_0021BB68; the file defines no other static object, so g++ generates no initializer of its own. Spelling the
// three objects as file-scope definitions is open.
#include "sly2/lo.h"
#include "sly2/jt.h"
#include "sly2/stepplayer.h"
#include "sly2/stackscope.h"
#include "gifs.h"
#include "vifs.h"
#include "sly2/render.h"
#include "sly2/shd.h"
#include "sly2/cm.h"
#include "sly2/sw.h"
#include "sly2/cref.h"
#include "sly2/scrap.h"
#include "sly2/classes.h"
#include "sly2/hud.h"
#include "sly2/richtext.h"
#include "heap.h"
#include "sly2/vu0.h"
#include "sly2/util.h"
#include "sly2/clock.h"
#include "sly2/joy.h"
#include "sly2/save.h"
#include "sly2/sndmgr.h"
#include "sly2/vtables.h"
extern "C" char base_D_003DA0C0[] __asm__("D_003DA0C0");   /* base of the address names below: see docs/guides/tu-merging.md "Data" */
extern "C" char base_D_004435F0[] __asm__("D_004435F0");   /* g_subtitle (subtitle), base of D_004435F4 */

extern char D_004435F0[];       /* speaker label (HYPOTHESIS, as letterbox.cpp) */
#undef D_003DA0D8
#define D_003DA0D8 (*(int *)((char *)base_D_003DA0C0 + 0x18))   /* screen's object+0x18 */   /* ticker text id */
extern "C" char D_000000C0[];         /* VU1 microprogram entry at 0xC0 */
char *FindSwLocalizedString(SW *psw, int id);
int IsBasicDerivedFrom(BASIC *pbasic, CID cid);
int IsEntryInDl(DL *pdl, void *pv);
void AppendDlEntry(DL *pdl, void *pv);
void RemoveDlEntry(DL *pdl, void *pv);
extern "C" {
char *strcpy(char *, const char *);
PO *GetHeroPo(SW *psw);
char *GetReconPendingText(int *pn);
char *GetReconSelectedText(int *pn);
void GetSubtitleText(char *pchz, int n);
void FormatSubtitleButtonGlyphs(char *pchz, int n);
void SetWidgetRequest(WIDGET *widget, int request);
void SetWidgetState(WIDGET *widget, int state);
int HasSpriteAnim(void *pv, SPRITEGRP *group, int id);
int IsWidgetObscured(WIDGET *widget);
int IsWidgetIdle(WIDGET *widget);
int GetSpriteMainState(SPRITEGRP *spriteGroup);
int IsSpriteAnimActive(SPRITEGRP *spriteGroup);
void StopSprite(SPRITEGRP *spriteGroup);
void SetWidgetExclusive(WIDGET *widget, int isNonExclusive);
void DrawSpriteItems(SPRITEANIM *anim, int slot, int drawFlags);
void StopSpriteSlot(SPRITEGRP *spriteGroup, int i);
/* DrawSprite returns what DrawSpriteSlot leaves in v0 (its own body is void) */
int func_002185F8I(SPRITEGRP *spriteGroup, int i, int n) __asm__("DrawSpriteSlot");
void StaticInit_hud(int fInitialize, int nPriority);
}

/* Sprite descriptor kinds (LoadSpriteDescriptor; the payload union is in its namespace). */
enum SPRITE_KIND
{
    SPRITE_KIND_NONE = 0,
    SPRITE_KIND_GEOMETRY = 1,   /* VU1 line / strip records */
    SPRITE_KIND_POLYGON = 2,
    SPRITE_KIND_TEXT = 3,       /* drawn by RenderSpriteText */
    SPRITE_KIND_NESTED = 4,     /* shows another sprite group */
    SPRITE_KIND_UNUSED5 = 5,
    SPRITE_KIND_EFFECT = 6,     /* a textured quad from a texture resource */
    SPRITE_KIND_PICTURE = 7,    /* a textured quad from a texture id */
    SPRITE_KIND_TABLE = 8,      /* a nested sprite table with word / float parameters */
};

/* Class ids tested by the text getters (docs/systems/object-model.md section 1). */
enum HUD_CID
{
    CID_STEPPLAYER = 0x34,   /* any playable character */
    CID_JT = 0x35,           /* Sly */
    CID_MURRAY = 0x36,
    CID_BENTLEY = 0x37,
    CID_KEY = 0x5C,
    CID_LOOT = 0x5E,         /* a pickpocketed item */
};

// 0x210EB0 BuildContextText
/* STRINGGEN getter: the context is the text itself. */
// Text-table getter: copies the context string given by the caller.
extern "C" void BuildContextText(char *pchz, char *pchzContext)
{
    strcpy(pchz, pchzContext);
}

// 0x210ED8 BuildCoinCountText
namespace M_BuildCoinCountText {
extern char D_002A8AA0[] __asm__("D_002A8AA0");
void GetGlobalSaveVar(CRefData *r, int *p, void *q) __asm__("GetGlobalSaveVar");
void Decref__4CRef(CRefData *r) __asm__("Decref__4CRef");
void fiprintf(void *f, const char *fmt, ...) __asm__("sprintf");
// 0x210ED0: dead-code scrap (sly2/scrap.h)
extern "C" { SLY2_SCRAP("addiu $sp, $sp, 0x10"); }

// HUD text 0x2CF: the player's coin count.
extern "C" void BuildCoinCountText(void *f, int **pp)
{
    int *p = *pp;
    if (p && *p) {
        char *fmt = D_002A8AA0;
        CRefData r;
        int v;
        GetGlobalSaveVar(&r, p, &g_gs);
        v = r.u.n;
        Decref__4CRef(&r);
        fiprintf(f, fmt, v);
    }
}
} // namespace M_BuildCoinCountText

// 0x210F50 BuildClueCountText
namespace M_BuildClueCountText {
extern char D_002A8AA8[] __asm__("D_002A8AA8");
void fiprintf(void *f, const char *fmt, ...) __asm__("sprintf");
// HUD text 0x2D0: clue bottles collected / total in this level. Runtime-checked counters SW+0x4764 / +0x4768.
extern "C" void BuildClueCountText(void *f)
{
    fiprintf(f, D_002A8AA8, g_sw.numCluesCollected, g_sw.numClues);
}
} // namespace M_BuildClueCountText

// 0x210F80 BuildTimerText
namespace M_BuildTimerText {
extern char D_002A8AB0[] __asm__("D_002A8AB0");
void fiprintf(void *f, const char *fmt, ...) __asm__("sprintf");
// HUD text 0x2D1: the mission countdown as minutes:seconds (never below zero).
extern "C" void BuildTimerText(void *f)
{
    float t = -g_clock.time + g_sw.hudTimerEndTime; int n; t = (t < 0.0f) ? 0.0f : t;
    n = (int)t;
    {
        int q = n / 60;
        fiprintf(f, D_002A8AB0, q, n - q * 60);
    }
}
} // namespace M_BuildTimerText

// 0x210FE0 BuildBinocularReadoutText
namespace M_func_00210FE0 {
struct X : STEP { char pad[0xF20 - 0x2C0]; int modeKind; char padf24[0x18]; float hudZoom; };   /* STEPPLAYER jtmode.kind (0 look, 1 zoom), jtmode.hud.zoom */
extern char D_002A8AB8[] __asm__("D_002A8AB8"), D_002A8AC0[] __asm__("D_002A8AC0"), D_002A8AC8[] __asm__("D_002A8AC8"), D_002A8AD0[] __asm__("D_002A8AD0"), D_002A8AD8[] __asm__("D_002A8AD8");
int IsBasicDerivedFrom__FP5BASIC3CID(struct X *, int) __asm__("IsBasicDerivedFrom__FP5BASIC3CID");
int sprintf(char *, char *, ...) __asm__("sprintf");
int fptodp(float) __asm__("fptodp");
// HUD text 0x2DC: the binocucom readout, per character (Sly: zoom "x%d", Bentley: "x%5.3f", Murray: distance words).
// (inferred)
extern "C" void BuildBinocularReadoutText(char *text)
{
    struct X *hero = (struct X *)GetHeroPo(&g_sw);
    float zoom;
    if (!hero || !IsBasicDerivedFrom__FP5BASIC3CID(hero, CID_STEPPLAYER))
        return;
    switch (hero->modeKind) {
    case 0:
        zoom = g_cm.cplook.rZoomMax * hero->hudZoom;
        break;
    case 1:
        zoom = g_cm.cpzoom.rZoomMax * hero->hudZoom;
        break;
    }
    if (IsBasicDerivedFrom__FP5BASIC3CID(hero, CID_JT)) {
        sprintf(text, D_002A8AB8, (int)zoom);
    } else if (IsBasicDerivedFrom__FP5BASIC3CID(hero, CID_BENTLEY)) {
        sprintf(text, D_002A8AC0, fptodp(zoom));
    } else if (IsBasicDerivedFrom__FP5BASIC3CID(hero, CID_MURRAY)) {
        int distance = (int)zoom;
        if (distance < 4)
            sprintf(text, D_002A8AC8, distance);
        else if (distance < 8)
            sprintf(text, D_002A8AD0, distance);
        else
            sprintf(text, D_002A8AD8, distance);
    }
}
} // namespace M_func_00210FE0

// 0x211148 BuildReconSelectionText
/* STRINGGEN getter (context 0 selected, 1 pending): the recon text of the hero's mode HUD, if the hero is a JT. */
// HUD texts 0x2DD / 0x2DE: the recon selector's selected item, or its pending caption.
// Also known as: StrPhotoScript (upstream readable / Sly 1 name; StrPhotoScript__FPci).
extern "C" void BuildReconSelectionText(char *text, int which)
{
    JT *hero = (JT *)GetHeroPo(&g_sw);
    if (!hero || !IsBasicDerivedFrom((BASIC *)hero, (CID)CID_STEPPLAYER))
        return;
    int *reconSelector = hero->jtmode.hud.unknown14;
    char *reconText = 0;
    if (reconSelector) {
        if (which != 0) {
            if (which == 1)
                reconText = GetReconPendingText(reconSelector);
        } else {
            reconText = GetReconSelectedText(reconSelector);
        }
    }
    if (reconText)
        strcpy(text, reconText);
}

// 0x2111F0 BuildReconTargetText
namespace M_BuildReconTargetText {
struct Z { char pad[0xF8]; int nameTextId; };
struct Y { char pad[0x74]; int numTargets; struct Z *targets[1]; };
struct X : STEP { char pad[0xF38 - 0x2C0]; struct Y *reconSelector; };   /* STEPPLAYER jtmode.hud.unknown14 */
int IsBasicDerivedFrom__FP5BASIC3CID(void *p, int cid) __asm__("IsBasicDerivedFrom__FP5BASIC3CID");
void strcpy(char *dst, char *src) __asm__("strcpy");
// HUD texts 0x2DF-0x2E4: the name of recon target i, if there is one.
extern "C" void BuildReconTargetText(char *dst, int i)
{
    SW *sw = &g_sw;
    struct X *hero = (struct X *)GetHeroPo(sw);
    if (hero && IsBasicDerivedFrom__FP5BASIC3CID(hero, CID_STEPPLAYER)) {
        struct Y *selector = hero->reconSelector;
        if (selector && i < selector->numTargets)
            strcpy(dst, FindSwLocalizedString(sw, selector->targets[i]->nameTextId));
    }
}
} // namespace M_BuildReconTargetText

// 0x211298 BuildMenuCarouselText
namespace M_func_00211298 {
struct F { int id; char pad[0x1C]; };
struct E { int numEntries; struct F *entries; char pad[0xC]; int selection; char pad2[4]; };   /* a menu page */
#undef D_003D9EDC
extern struct E D_003D9EDC[] __asm__("D_003D9ED0+0xC");   /* screen's object+0xC, a separate symbol to g++: the base+offset form changes BuildMenuCarouselText (CSE of the base) */
#undef D_003D9EF4
extern int D_003D9EF4 __asm__("D_003D9ED0+0x24");   /* screen's object+0x24; see D_003D9EDC */
char *strcpy(char *dst, const char *src) __asm__("strcpy");
// HUD texts 0x2D2-0x2DB: the name of the menu entry k places from the selection on the current menu page (wrapping).
// (inferred)
extern "C" void BuildMenuCarouselText(char *dst, int k)
{
    struct E *page = &D_003D9EDC[D_003D9EF4];
    if (page->numEntries) {
        int n = page->numEntries;
        int j = k + (long)(page->selection - 3);   /* slot 3 of the carousel is the selection */
        while (j < 0)
            j += n;
        (void)strcpy(dst, FindSwLocalizedString(&g_sw, page->entries[j % n].id));
    }
}
} // namespace M_func_00211298
#undef D_003D9EDC
#undef D_003D9EF4

// 0x211348 BuildWidgetArgText
namespace M_BuildWidgetArgText {
void strcpy(void *p, int v) __asm__("strcpy");
} // namespace M_BuildWidgetArgText
// HUD texts 0x2E5-0x2EE: the language string of the owning widget's argument i.
extern "C" void BuildWidgetArgText(void *text, int i, WIDGET *widget)
{
    using namespace M_BuildWidgetArgText;
    strcpy(text, (int)FindSwLocalizedString(&g_sw, widget->argTextIds[i]));
}

// 0x211390 BuildSubtitleText
// HUD text 0x2EF: the current subtitle's text. (inferred)
extern "C" void BuildSubtitleText(int n)
{
    GetSubtitleText(D_004435F0, n);
}

// 0x2113B8 BuildSubtitleButtonText
// HUD text 0x2F0: the current subtitle's button glyphs. (inferred)
extern "C" void BuildSubtitleButtonText(int n)
{
    FormatSubtitleButtonGlyphs(D_004435F0, n);
}

// 0x2113E0 BuildTickerText
/* STRINGGEN getter: the localized string D_003DA0D8 names. */
// HUD text 0x2F1: the message of the scrolling ticker.
extern "C" void BuildTickerText(char *pchz)
{
    strcpy(pchz, FindSwLocalizedString(&g_sw, D_003DA0D8));
}

// 0x211420 BuildHeldItemText
namespace M_func_00211420 {
struct E { char *name; int nameTextId; int sellPrice; };
extern struct E D_002C3288[] __asm__("D_002C3288");   /* the loot table (screen.cpp's Thiefnet screens) */
struct Z { int unknown0; int lootIndex; };
struct Y { char pad[0x224]; int keyTextId; char pad228[0x1C]; struct Z *loot; };
/* STEPPLAYER: jtgrab.state, the grabbed object (inferred), the default text id (inferred) */
struct X : STEP { char pad[0x4C8 - 0x2C0]; int grabState; char pad4cc[0x9A0 - 0x4CC]; struct Y *heldObject; char pad9a4[0x1318 - 0x9A4]; int heldTextId; };
enum { GRAB_STATE_HOLDING = 0xC };   /* (inferred) */
int IsBasicDerivedFrom__FP5BASIC3CID(void *, int) __asm__("IsBasicDerivedFrom__FP5BASIC3CID");
char *FindSwLocalizedString__FP2SWi(void *, int) __asm__("FindSwLocalizedString__FP2SWi");
void strcpy(char *, char *) __asm__("strcpy");
// HUD text 0x2F4: the name of the object the player is operating (in the "operate" mode). (inferred)
extern "C" void BuildHeldItemText(char *text)
{
    int id = -1;
    struct X *hero = (struct X *)GetHeroPo(&g_sw);
    if (hero && IsBasicDerivedFrom__FP5BASIC3CID(hero, CID_STEPPLAYER)) {
        struct Y *held = hero->heldObject;
        if (held) {
            int grabState = hero->grabState;
            if (grabState == GRAB_STATE_HOLDING) {
                if (IsBasicDerivedFrom__FP5BASIC3CID(held, CID_LOOT))
                    id = D_002C3288[held->loot->lootIndex].nameTextId;
                else if (IsBasicDerivedFrom__FP5BASIC3CID(held, CID_KEY))
                    id = held->keyTextId;
                else
                    id = hero->heldTextId;
            }
        }
    }
    if (id != -1)
        strcpy(text, FindSwLocalizedString__FP2SWi(&g_sw, id));
    else
        *text = 0;
}
} // namespace M_func_00211420

// 0x211528 StartupHudGifPackets
namespace M_func_00211528 {
// Initialize the fixed GIF register packets.
extern QW D_004421B0[7] __asm__("D_004421B0");
extern QW D_00442240[0x27] __asm__("D_00442240");
extern QW D_00442210[3] __asm__("D_00442210");
extern QW D_004424B0[2] __asm__("D_004424B0");
extern QW D_004424D0[2] __asm__("D_004424D0");
extern QW D_00442560[4] __asm__("D_00442560");
extern QW D_004425A0[3] __asm__("D_004425A0");
extern QW D_004424F0[3] __asm__("D_004424F0");
extern QW D_00442520[4] __asm__("D_00442520");
extern int D_002DEDB0 __asm__("D_002DEDB0");
extern ulong D_002DEBE0[] __asm__("D_002DEBE0"), D_002DECD0[] __asm__("D_002DECD0");   /* gs.cpp's draw environments (sceGsDrawEnv1) */
// Boot-time setup (startup table): builds the static GIF register packets used by sprite and HUD rendering. (inferred)
extern "C" void StartupHudGifPackets(){
 GIFS setup;
 QW *data=D_004421B0;
 QW *header=(QW *)((int)data-0x10);
 setup.AllocStatic(7,data);
 ((QW *)((int)data-0x10))->aul[0]=1UL<<60;header->aul[1]=5;
 setup.AddPrimBulk(0x44,0,1,1,0);header[1].aul[0]|=1;
 setup.AddPrimBulk(0x84,0,1,1,0);header[2].aul[0]|=1;
 setup.AddPrimBulk(0x45,0,1,1,0);header[3].aul[0]|=1;
 setup.AddPrimBulk(0x85,0,1,1,0);header[4].aul[0]|=1;
 setup.AddPrimBulk(0x42,0,1,1,0);header[5].aul[0]|=1;
 setup.AddPrimBulk(0x82,0,1,1,0);header[6].aul[0]|=1;
 GIFS tiles;
 tiles.AllocStatic(0x27,D_00442240);
 tiles.AddPrimPack(0x46,1,0xE);
 tiles.PackAD(0x4A,1);tiles.PackAD(0x42,0x4A);tiles.PackAD(0x47,0x31001);
 for(int i=0;i<0x200;i+=0x20){tiles.PackAD(5,0x79000000L|(0x7000+i*0x10));tiles.PackAD(5,0x87000000L|(0x7200+i*0x10));}
 tiles.PackAD(0x4A,0);tiles.PackAD(0x42,0x44);tiles.PackAD(0x47,0x50000);
 tiles.AddPrimEnd();
 GIFS frame;
 ulong *active=(D_002DEDB0&1)?&D_002DECD0[2]:&D_002DEBE0[2];   /* the draw environment's zbuf (+0x10) */
 ulong frameWord=*active;
 frame.AllocStatic(3,D_00442210);
 frame.AddPrimPack(0,1,0xE);frame.PackAD(0x3F,0);frame.PackAD(0x4E,0x3A000000L|(frameWord&0x1FF));
 frame.AddPrimEnd();
 GIFS unmask;
 unmask.AllocStatic(2,D_004424B0);
 unmask.AddPrimPack(0,1,0xE);unmask.PackAD(0x4C,0);unmask.AddPrimEnd();
 GIFS mask;
 mask.AllocStatic(2,D_004424D0);
 mask.AddPrimPack(0,1,0xE);mask.PackAD(0x4C,0xFF00000000000000UL);mask.AddPrimEnd();
 GIFS restore;
 restore.AllocStatic(4,D_00442560);
 restore.AddPrimPack(0,1,0xE);restore.PackAD(0x4A,0);restore.PackAD(0x42,0x44);restore.PackAD(0x47,0x50000);restore.AddPrimEnd();
 GIFS region;
 region.AllocStatic(3,D_004425A0);
 region.AddPrimPack(0,1,0xE);region.PackAD(0x42,0x44);region.PackAD(0x47,0x74000);region.AddPrimEnd();
 GIFS test;
 test.AllocStatic(3,D_004424F0);
 test.AddPrimPack(0,1,0xE);test.PackAD(0x42,0x4A);test.PackAD(0x47,0x30000);test.AddPrimEnd();
 GIFS enable;
 enable.AllocStatic(4,D_00442520);
 enable.AddPrimPack(0,1,0xE);enable.PackAD(0x4A,1);enable.PackAD(0x42,0x4A);enable.PackAD(0x47,0x31001);enable.AddPrimEnd();
}
} // namespace M_func_00211528

// 0x211A80 BeginRpHud
void EnsureVu1Code(VIFS *pvifs, void *pvStart, void *pvEnd);   /* uploads a VU1 microprogram once */
namespace M_BeginRpHud {
extern char D_0025AFA0[] __asm__("D_0025AFA0"), D_0025B870[] __asm__("D_0025B870");
extern QW D_002DF800[] __asm__("D_002DF800");
extern char D_004421A0[] __asm__("D_004421A0"), D_004424B0[] __asm__("D_004424B0"), D_004424D0[] __asm__("D_004424D0"), D_004424F0[] __asm__("D_004424F0"), D_00442520[] __asm__("D_00442520"), D_004425A0[] __asm__("D_004425A0"), D_00442560[] __asm__("D_00442560");
extern QW D_00442240[] __asm__("D_00442240");
extern char D_000000C0[] __asm__("D_000000C0"), D_00000100[] __asm__("D_00000100");   /* VU1 micro memory addresses */
// Begins the HUD render pass: uploads the HUD VU1 microprogram and its constants and sets the GS state for HUD drawing.
extern "C" void BeginRpHud(VIFS *p)
{
    EnsureVu1Code(p, D_0025AFA0, D_0025B870);
    p->AddDmaCnt();
    p->AddVifFlush();
    p->AddDmaCall(D_002DF800);
    p->AddVifBaseOffset(0, 0);
    p->AddVifUnpack(UPK_V4_32, 7, D_004421A0, 0xD);
    p->AddVifUnpack(UPK_V4_32, 2, D_004424B0, 0x15);
    p->AddVifUnpack(UPK_V4_32, 2, D_004424D0, 0x17);
    p->AddVifUnpack(UPK_V4_32, 3, D_004424F0, 0x19);
    p->AddVifUnpack(UPK_V4_32, 4, D_00442520, 0x1C);
    p->AddVifUnpack(UPK_V4_32, 3, D_004425A0, 0x20);
    p->AddVifUnpack(UPK_V4_32, 4, D_00442560, 0x23);
    p->AddVifBaseOffset(0x32, 0x1E7);
    p->AddVifMscal(D_000000C0);
    p->AddVifFlusha();
    p->AddVifDirect(0x27, D_00442240, 0);
    p->AddVifFlusha();
    p->AddVifMscal(D_00000100);
    p->AddVifFlusha();
    p->EndDmaCnt();
}
} // namespace M_BeginRpHud

// 0x211C10 EndRpHud
/* Ends the HUD render pass: run the VU1 microprogram at 0xC0, then FLUSHA. */
// Ends the HUD render pass (starts the microprogram, flushes).
extern "C" void EndRpHud(VIFS *pvifs)
{
    pvifs->AddDmaCnt();
    pvifs->AddVifMscal(D_000000C0);
    pvifs->AddVifFlusha();
    pvifs->EndDmaCnt();
}

// 0x211C58 ConstructWidget130
/* WIDGET130's constructor (sly2/classes.h): WIDGET_ctor, the vptr (_vt$9WIDGET130, class_registry's), the fields. */
// Constructor of widget class 0x82 (the recon selector prompt): base widget, then its five choices -1.
WIDGET130::WIDGET130() { actorChoice = -1; risingChoice = -1; fallingChoice = -1; nearChoice = -1; emptyChoice = -1; }

// 0x211CA8 ShowModalBoxWidget
namespace M_func_00211CA8 {
void IncrementSwHandsOff(SW *g, int a) __asm__("IncrementSwHandsOff");
void PlayWidgetAnim(WIDGET *p, int a) __asm__("PlayWidgetAnim");
} // namespace M_func_00211CA8
// Shows the modal message box (class 0x7F, slot 49) if it is idle: shows the widget, takes player control and plays its
// open animation. (inferred)
// Also known as: ShowFailureScreen (upstream readable / Sly 1 name; ShowFailureScreen__FP8SCREENRTi).
void WIDGET127::Show(int idLabel)
{
    using namespace M_func_00211CA8;
    if (mainAnimId == -1) {
        WIDGET::Show(idLabel);
        IncrementSwHandsOff(&g_sw, 0);
        isOpen = 1;
        PlayWidgetAnim(this, 0x33A);   /* the box's open animation */
    }
}

// 0x211D08 InitModalBoxWidget
// Initializes the modal message box (class 0x7F): base init, and it pauses the game while shown.
void WIDGET127::Init()
{
    LO::Init();
    pausesGame = 1;
}

// 0x211D38 UpdateModalBoxWidget
namespace M_func_00211D38 {
extern int D_002DD318 __asm__("D_002DD318");
extern char D_002E6550[] __asm__("D_002E6550"), D_003DA4F0[] __asm__("D_003DA4F0");
int IsScreenShown(void *) __asm__("IsScreenShown");
int HandleJoyButtons(void *, int) __asm__("HandleJoyButtons");
void DecrementSwHandsOff(SW *, int) __asm__("DecrementSwHandsOff");
void RestartJob(void *) __asm__("RestartJob");
} // namespace M_func_00211D38
// Per-frame modal message box: once visible for half a second, a face-button press releases player control, optionally
// resumes the game and hides the box. (inferred)
// Also known as: UpdateFailureScreen (upstream readable / Sly 1 name; UpdateFailureScreen__FP8SCREENRT).
void WIDGET127::Update()
{
    using namespace M_func_00211D38;
    if (IsWidgetIdle(this))
        return;
    WIDGET::Update();
    if (state == WIDGET_STATE_VISIBLE && 0.5f < g_clock.realTime - stateTime && !IsScreenShown(D_002E6550)
        && !IsScreenShown(D_003DA4F0) && HandleJoyButtons(&g_joy, 0xF0 /* face buttons */)) {
        DecrementSwHandsOff(&g_sw, 0);
        if (keepsGamePaused == 0 && g_sw.currentJob) {
            D_002DD318++;
            RestartJob(g_sw.currentJob);
        }
        Hide();
    }
    isOpen = 1;
}

// 0x211E40 InitWidget128
// Initializes widget class 0x80 (the job completion banner): base init, and it pauses the game while shown.
void WIDGET128::Init()
{
    LO::Init();
    pausesGame = 1;
}

// 0x211E70 AddItemCounterOidA
// Adds an object to the item counter widget's first list (at most 16). (inferred)
extern "C" void AddItemCounterOidA(WIDGET129 *widget, int oid)
{
    unsigned int count = widget->numEntriesA;
    if (count < 16) {
        widget->entriesA[count].oid = (OID)oid;
        widget->numEntriesA = count + 1;
    }
}

// 0x211E98 AddItemCounterOidB
// Adds an object to the item counter widget's second list (at most 16). (inferred)
extern "C" void AddItemCounterOidB(WIDGET129 *widget, int oid)
{
    unsigned int count = widget->numEntriesB;
    if (count < 16) {
        widget->entriesB[count].oid = (OID)oid;
        widget->numEntriesB = count + 1;
    }
}

// 0x211EC0 PostLoadItemCounterWidget
namespace M_func_00211EC0 {
void *FindSwObject__FP2SWi3OIDP2LO(SW *g, int k, int id, int z) __asm__("FindSwObject__FP2SWi3OIDP2LO");
} // namespace M_func_00211EC0
// Item counter widget post-load (class 0x81): base widget post-load, then resolves both OID lists to objects.
void WIDGET129::PostLoad()
{
    using namespace M_func_00211EC0;
    int i;
    WIDGET::PostLoad();
    for (i = 0; i < numEntriesB; i++) {
        void *object = FindSwObject__FP2SWi3OIDP2LO(&g_sw, 5, entriesB[i].oid, 0);
        if (object)
            entriesB[i].task = (TASK *)object;
    }
    for (i = 0; i < numEntriesA; i++) {
        void *object = FindSwObject__FP2SWi3OIDP2LO(&g_sw, 5, entriesA[i].oid, 0);
        if (object)
            entriesA[i].task = (TASK *)object;
    }
}

// 0x211F90 UpdateItemCounterWidget
namespace M_func_00211F90 {
enum { TASK_STATE_ACTIVE = 1 };   /* (inferred) */
extern char D_002C3EC0[0x90] __asm__("D_002C3EC0");
extern char D_002A8AA0[] __asm__("D_002A8AA0");
int IsTaskComplete(TASK *) __asm__("IsTaskComplete");
int IsWidgetAnimPlaying(WIDGET *,int) __asm__("IsWidgetAnimPlaying");
int sprintf(char *,char *,...) __asm__("sprintf");
} // namespace M_func_00211F90
// Per-frame item counter: counts finished and active objects, writes done / total / remaining into the widget's item
// texts, shows while any is active and hides when all are done. (inferred)
// Runtime-checked: it counts the complete tasks.
// Also known as: UpdateTaskCounter (upstream readable / Sly 1 name; UpdateTaskCounter__FP8SCREENRT).
void WIDGET129::Update()
{
    using namespace M_func_00211F90;
    int i;
    int numComplete = 0;
    int numCompleteB = 0;
    int numActive = 0;
    WIDGET::Update();
    for (i = 0; i < numEntriesB; i++) {
        TASK *task = entriesB[i].task;
        if (task) {
            if (task->state == TASK_STATE_ACTIVE)
                numActive++;
            else if (IsTaskComplete(task)) {
                numComplete++;
                numCompleteB++;
            }
        }
    }
    for (i = 0; i < numEntriesA; i++) {
        TASK *task = entriesA[i].task;
        if (task) {
            if (task->state == TASK_STATE_ACTIVE)
                numActive++;
            else if (IsTaskComplete(task))
                numComplete++;
        }
    }
    /* the three 0x30-byte "Widget Item" text buffers: done, total, remaining */
    if (IsWidgetAnimPlaying(this, 0x314)) {
        sprintf(D_002C3EC0, D_002A8AA0, numCompleteB);
        sprintf(D_002C3EC0 + 0x30, D_002A8AA0, numEntriesB);
        sprintf(D_002C3EC0 + 0x60, D_002A8AA0, numEntriesB - numCompleteB);
    }
    if (numComplete == numEntriesB + numEntriesA || !numActive)
        Hide();
    else if (numActive > 0)
        Show(-1);
}

// 0x212158 WIDGET_ctor
namespace M_WIDGET_ctor {
struct S { void *frame; void *sidebag; char pad[4]; void *vt; char pad2[0x6C]; int mainAnimId; char pad3[0x4C]; int argTextIds[10]; char pad4[8]; char childList[4]; };
void InitSl(void *p, int n) __asm__("InitSl");
// WIDGET constructor: vtable, no main animation (-1), the ten argument text ids -1 and an empty child list.
extern "C" struct S *WIDGET_ctor(struct S *widget)
{
    unsigned int i;
    SLY2_SET_VPTR(widget, WIDGET);
    widget->mainAnimId = -1;
    widget->frame = 0;      /* BASIC's counted pointers */
    widget->sidebag = 0;
    for (i = 0; i < 10; i++)
        widget->argTextIds[i] = -1;
    InitSl(widget->childList, 0x68);   /* InitSl */
    return widget;
}
} // namespace M_WIDGET_ctor

// 0x2121C8 IsWidgetIdle
/* Whether the widget is neither shown nor asked to show (state and request both below 2). */
// True when the widget is neither shown nor requested to show. (inferred)
// Also known as: FScreenObjDone (upstream readable / Sly 1 name; FScreenObjDone__FP2LO).
extern "C" int IsWidgetIdle(WIDGET *widget)
{
    int isIdle = 0;
    if (widget->state < WIDGET_STATE_VISIBLE)
        isIdle = widget->request < WIDGET_REQUEST_SHOW;
    return isIdle;
}

// 0x2121E8 IsWidgetAnimPlaying
// True while the widget animation with the given id (0x314 = main track) is still running.
// Also known as: FSpriteAnimPlaying (upstream readable / Sly 1 name; FSpriteAnimPlaying__FP2LOi).
extern "C" int IsWidgetAnimPlaying(WIDGET *widget, int animId)
{
    int groupIndex, i;
    if (animId == -1)
        return 0;
    if (widget->mainAnimId == -1)
        return 0;
    if (animId == 0x314)   /* the main track */
        return GetSpriteMainState(widget->spriteGroups) == 0;
    for (groupIndex = 0; groupIndex < widget->numSpriteGroups; groupIndex++) {
        SPRITEGRP *group = &widget->spriteGroups[groupIndex];
        for (i = 0; i < group->numAnims; i++) {
            if (animId == group->anims[i].id)
                return IsSpriteAnimActive(group);
        }
    }
    return 0;
}

// 0x212298 LoadWidgetFromBrx
namespace M_LoadWidgetFromBrx {
typedef struct { int cb; int cbAlign; } AREQ;
extern char D_003E1C70[] __asm__("D_003E1C70"), D_003E1C80[] __asm__("D_003E1C80");
int S8Read__18CBinaryInputStream(void *pbis) __asm__("S8Read__18CBinaryInputStream");
float F32Read__18CBinaryInputStream(void *pbis) __asm__("F32Read__18CBinaryInputStream");
int U32Read__18CBinaryInputStream(void *pbis) __asm__("U32Read__18CBinaryInputStream");
WIDGETLOAD *AllocClearedFromWorldHeap(AREQ *preq) __asm__("AllocClearedFromWorldHeap");
void CBinaryInputStream_ReadCb64(void *pbis, WIDGETLOAD *po) __asm__("CBinaryInputStream_ReadCb64");
void AppendSlEntry(void *, WIDGETLOAD *po) __asm__("AppendSlEntry");
void LoadSpriteTable(WIDGET *p, void *pbis, WIDGET *p2, int *pa, SPRITEDESC **pb) __asm__("LoadSpriteTable");
void LoadSpriteGroups(WIDGET *p, void *pbis, WIDGET *p2) __asm__("LoadSpriteGroups");
} // namespace M_LoadWidgetFromBrx
// Loads a widget: default animation ids, the LO part, whether its sprites are streamed, duration, size and font flag;
// its sprites load now or later. Layout parse-verified: a parser following it reads every byte of 4 levels.
// Also known as: LoadScreenObjFromBrx (upstream readable / Sly 1 name;
//   LoadScreenObjFromBrx__FP9SCREENOBJP18CBinaryInputStream).
void WIDGET::LoadFromBrx(CBinaryInputStream *pbis)
{
    using namespace M_LoadWidgetFromBrx;
    int isStreamed;
    loopAnimId = 0x310;
    showAnimId = 0x30F;
    hideAnimId = 0x311;
    LO::LoadFromBrx(pbis);
    isStreamed = S8Read__18CBinaryInputStream(pbis);
    duration = F32Read__18CBinaryInputStream(pbis);
    width = U32Read__18CBinaryInputStream(pbis);
    height = U32Read__18CBinaryInputStream(pbis);
    hasFont = U32Read__18CBinaryInputStream(pbis);
    if (isStreamed) {
        M_LoadWidgetFromBrx::AREQ req;
        state = WIDGET_STATE_UNLOADED;
        req.cb = 0x50;
        req.cbAlign = 0x10;
        spriteHeap = (HEAP *)D_003E1C70;   /* the streamed-sprite linear heap */
        deferredLoad = AllocClearedFromWorldHeap(&req);
        deferredLoad->owner = this;
        CBinaryInputStream_ReadCb64(pbis, deferredLoad);
        AppendSlEntry((*(char (*)[])&g_sw.slWidgetDeferred), deferredLoad);
    } else {
        state = WIDGET_STATE_HIDDEN;
        spriteHeap = (HEAP *)D_003E1C80;   /* the level heap */
        LoadSpriteTable(this, pbis, this, &spriteCount, &sprites);
        LoadSpriteGroups(this, pbis, this);
    }
    mainAnimId = -1;
}

// 0x2123C8 PostWidgetLoad
namespace M_PostWidgetLoad {
LO *FindSwObject__FP2SWi3OIDP2LO(SW *, int, int, LO *) __asm__("FindSwObject__FP2SWi3OIDP2LO");
void SetLoBusyKind(LO *p, int) __asm__("SetLoBusyKind");
} // namespace M_PostWidgetLoad
// Widget post-load: finds the font object, centres the widget on screen from its size, shows it if flagged, marks it
// busy.
void WIDGET::PostLoad()
{
    using namespace M_PostWidgetLoad;
    unsigned int flags;
    float scaledHeight;
    int scaledWidth;
    LO::PostLoad();
    if (hasFont) {
        SW *sw = &g_sw;
        if (!sw->ploHudFont)
            sw->ploHudFont = FindSwObject__FP2SWi3OIDP2LO(sw, 0x405, 0x374, this);
    }
    ownerWidget = this;
    isNonExclusive = -1;
    /* centre on the GS drawing area (origin 2048,2048): width from 640 to 512 units, height times 1/2.2 */
    scaledWidth = (width << 9) / 640;
    scaledHeight = (float)height * 0.45454547f;
    screenY = 2048.0f - scaledHeight * 0.5f;
    screenX = 2048.0f - (float)scaledWidth * 0.5f;
    if (isShownOnLoad)
        Show(-1);
    SetLoBusyKind(this, 1);
    /* LO unknown40: either the flag word itself (bit 0 set) or a pointer to it (bit 0 clear) (inferred) */
    flags = unknown40;
    if ((flags ^ 1) & 1) *(int *)flags |= 0x102;
    else unknown40 = flags | 0x102;
}

// 0x2124F0 LoadWidgetSprites
namespace M_LoadWidgetSprites {
// Load prioritized sprite files.
struct FRAME {};
typedef struct {int cb,alignment;} MSG;
struct BIS {int grf;char unknown4[0x2C];void *vt;int cbSpool;char *abSpool;int unknown3C[10];};
typedef struct {int keyLo,keyHi,kind;char filename[0x104];} KEY;   /* a file lookup key (inferred) */
typedef union {MSG msg;char magic[4];} TMP;
typedef struct {char pad[0xC];int priority;} POOL;
extern FRAME *STACKSCOPE_ctor(FRAME *) __asm__("STACKSCOPE_ctor");
extern void STACKSCOPE_ctor_1AE4A0(FRAME *,int) __asm__("_$_10STACKSCOPE");
extern BIS *CStreamBis_ctor(BIS *,int,void *,int) __asm__("CStreamBis_ctor");
extern void DestroyCStreamBis(BIS *,int) __asm__("_$_10CStreamBis");
extern void InitFileKey(KEY *) __asm__("InitFileKey");
extern void *AllocFromStack(MSG *) __asm__("AllocFromStack");
extern int OpenStreamFromFileKey(BIS *,KEY *) __asm__("OpenStreamFromFileKey");
extern void SetDecompress__18CBinaryInputStreamiPUc(BIS *,int,void *) __asm__("SetDecompress__18CBinaryInputStreamiPUc");
extern unsigned char U8Read__18CBinaryInputStream(BIS *) __asm__("U8Read__18CBinaryInputStream");
extern unsigned short U16Read__18CBinaryInputStream(BIS *) __asm__("U16Read__18CBinaryInputStream");
extern int strncmp(const void *,const void *,unsigned int) __asm__("strncmp");
extern void LoadSpriteTable(WIDGET *,BIS *,WIDGET *,int *,SPRITEDESC **) __asm__("LoadSpriteTable");
extern void LoadSpriteGroups(WIDGET *,BIS *,WIDGET *) __asm__("LoadSpriteGroups");


extern POOL D_003E1C70 __asm__("D_003E1C70");
extern int D_002BE538 __asm__("D_002BE538");
extern char D_002A8AE8[] __asm__("D_002A8AE8");
// Build the file's decryption key from its load record (kind 2, lo = (k0^k2)+0xCA0, hi = k1^k3); returns whether the
// key is usable (hi != 0).
static inline int SetKey(KEY *key,WIDGETLOAD *load) {
    if(!load)return 0;
    {int k2=load->file.aw[13],k0=load->file.aw[0],k3=load->file.aw[15],k1=load->file.aw[1];
    key->kind=2;
    key->keyLo=(k0^k2)+0xCA0;
    key->keyHi=k1^k3;}
    return key->keyHi!=0;
}
struct WORK {
 FRAME frame;
 TMP tmp __attribute__((aligned(16)));
 BIS stream __attribute__((aligned(16)));
 KEY key __attribute__((aligned(16)));
};
// Loads a streamed widget's sprite file when the widget first leaves the unloaded state.
extern "C" void LoadWidgetSprites(WIDGET *widget) {
    WORK work;
    register BIS *bs asm("$17");asm("la %0,%a1":"=r"(bs):"p"(&work.stream));
    int priority=0;
    WIDGETLOAD *load;
    WIDGET *owner;
    int version;
    /* the highest priority among widgets that are loaded and shown (or hidden within the last frame) ... */
    for(load=(*(WIDGETLOAD **)&g_sw.slWidgetDeferred);load;load=load->next) {
        owner=load->owner;
        if(owner->state==0)continue;
        if(owner->state==1 && D_002BE538-owner->stateFrame>=2)continue;
        priority=StdMaxInt(priority,load->lastPriority);
    }
    /* ... evicts every loaded widget loaded at or above it, since the sprite heap is reset to that point */
    for(load=(*(WIDGETLOAD **)&g_sw.slWidgetDeferred);load;load=load->next) {
        owner=load->owner;
        if(load->priority>=priority && owner->state!=0)SetWidgetState(owner,0);
    }
    D_003E1C70.priority=priority;
    STACKSCOPE_ctor(&work.frame);
    KEY *key=&work.key;
    MSG *msg=&work.tmp.msg;

    msg->cb=0x10000;msg->alignment=0x40;
    CStreamBis_ctor(bs,0x10000,AllocFromStack(msg),0);
    key->kind=2;
    InitFileKey(key);
    if(!SetKey(&work.key,widget->deferredLoad)||!OpenStreamFromFileKey(bs,key))goto fail;
    SetDecompress__18CBinaryInputStreamiPUc(bs,0x4000,(void *)0x70000000);
    work.tmp.magic[0]=U8Read__18CBinaryInputStream(bs);
    work.tmp.magic[1]=U8Read__18CBinaryInputStream(bs);
    work.tmp.magic[2]=U8Read__18CBinaryInputStream(bs);
    work.tmp.magic[3]=U8Read__18CBinaryInputStream(bs);
    version=U16Read__18CBinaryInputStream(bs);
    if(!strncmp(work.tmp.magic,D_002A8AE8,4)&&version==6)goto load;
fail:
    DestroyCStreamBis(bs,2);
    STACKSCOPE_ctor_1AE4A0(&work.frame,2);
    return;
load:
    widget->deferredLoad->priority=D_003E1C70.priority;
    LoadSpriteTable(widget,bs,widget,&widget->spriteCount,&widget->sprites);
    LoadSpriteGroups(widget,bs,widget);
    widget->deferredLoad->lastPriority=D_003E1C70.priority;
    DestroyCStreamBis(bs,2);
    STACKSCOPE_ctor_1AE4A0(&work.frame,2);
}
} // namespace M_LoadWidgetSprites

// 0x212780 UnloadWidgetSprites
namespace M_UnloadWidgetSprites {
void ClearSl(SL *psl) __asm__("ClearSl");
} // namespace M_UnloadWidgetSprites
// Drops a widget's sprite tables and lists (on entering the unloaded state).
extern "C" void UnloadWidgetSprites(WIDGET *widget)
{
    using namespace M_UnloadWidgetSprites;
    WIDGETLOAD *load;
    widget->xform = 0;
    widget->spriteCount = 0;
    widget->sprites = 0;
    widget->numSpriteGroups = 0;
    widget->spriteGroups = 0;
    ClearSl(&widget->renderQueue);
    load = widget->deferredLoad;
    load->priority = 0;
    load->lastPriority = 0;
}

// 0x2127C8 AreWidgetLoadsIdle
namespace M_func_002127C8 {
extern int D_002BE538 __asm__("D_002BE538");
} // namespace M_func_002127C8
// True when no deferred widget sprite load is younger than two frames (used to wait for widgets before continuing).
// (inferred)
extern "C" int AreWidgetLoadsIdle(void)
{
    using namespace M_func_002127C8;
    WIDGETLOAD *load;
    for (load = (*(WIDGETLOAD * *)&g_sw.slWidgetDeferred); load; load = load->next) {
        WIDGET *widget = load->owner;
        int value;
        if (widget->state == WIDGET_STATE_UNLOADED)
            continue;
        if (widget->state == WIDGET_STATE_HIDDEN)
            value = D_002BE538 - widget->stateFrame;   /* frames since hidden */
        else
            value = widget->request;
        if (value < 2)
            return 0;
    }
    return 1;
}

// 0x212828 ShowWidgetArgsFromSplice
namespace M_ShowWidgetArgsFromSplice {
struct L { int words[4]; };
void SetValuesFromSpliceList(CRefData *r, void *a, int b, int c, struct L *pl, void *d) __asm__("SetValuesFromSpliceList");
void Decref__4CRef(CRefData *r) __asm__("Decref__4CRef");
} // namespace M_ShowWidgetArgsFromSplice
// Shows a widget with argument ids taken from a splice list (at most 10).
extern "C" void ShowWidgetArgsFromSplice(WIDGET *widget, void *spliceList)
{
    using namespace M_ShowWidgetArgsFromSplice;
    CRefData ref;
    struct L scratch;
    SetValuesFromSpliceList(&ref, spliceList, 0x10, 0xA, &scratch, widget->argTextIds);
    Decref__4CRef(&ref);
    widget->Show(-1);
}

// 0x212880 ShowWidgetArgs
// Shows a widget with n argument ids (at most 10), e.g. the text ids it displays.
extern "C" void ShowWidgetArgs(WIDGET *widget, int n, int *src)
{
    int i;
    for (i = 0; i < n && (unsigned)i < 10; i++)
        widget->argTextIds[i] = src[i];
    widget->Show(-1);
}

// 0x2128E8 ShowWidget
// ShowWidget (widget slot 49): requests that a widget show, loading its sprites first if needed; animId = the
// animation to loop (-1 = default).
void WIDGET::Show(int animId)
{
    if (state == WIDGET_STATE_UNLOADED)
        SetWidgetState(this, WIDGET_STATE_HIDDEN);
    SetWidgetRequest(this, WIDGET_REQUEST_SHOW);
    loopAnimId = animId;
    showAnimId = -1;
    hideAnimId = -1;
    if (animId != -1) {
        SetWidgetState(this, WIDGET_STATE_HIDDEN);
        SetWidgetState(this, WIDGET_STATE_VISIBLE);
        return;
    }
    /* default animations, where the sprite has them; 0x314 is the main track */
    loopAnimId = 0x314;
    if (HasSpriteAnim(this, spriteGroups, 0x30F))
        showAnimId = 0x30F;
    if (HasSpriteAnim(this, spriteGroups, 0x310))
        loopAnimId = 0x310;
    if (HasSpriteAnim(this, spriteGroups, 0x311))
        hideAnimId = 0x311;
    if (state == WIDGET_STATE_HIDDEN)
        Update();
}

// 0x2129E8 HideWidget
// HideWidget (widget slot 50): requests that a widget hide; without a disappear animation it goes straight to hidden.
void WIDGET::Hide()
{
    SetWidgetRequest(this, WIDGET_REQUEST_HIDE);
    if (hideAnimId == -1)
        SetWidgetState(this, WIDGET_STATE_HIDDEN);
}

// 0x212A30 ResetWidgetVisibleTime
/* Restarts the state timer of a visible widget. */
// Restarts a visible widget's display timeout. (inferred)
// Also known as: RefreshScreenObj (upstream readable / Sly 1 name; RefreshScreenObj__FP2LO).
extern "C" void ResetWidgetVisibleTime(WIDGET *widget)
{
    if (widget->state == WIDGET_STATE_VISIBLE)
        widget->stateTime = g_clock.realTime;
}

// 0x212A50 SetWidgetRequest
/* Requests a state (1 hide, 2 show) and stamps the request time. */
// Sets the widget's requested state (show / hide), acted on by UpdateWidget.
// Also known as: SetTimerTimers (upstream readable / Sly 1 name; SetTimerTimers__FP5TIMER6TIMERS).
extern "C" void SetWidgetRequest(WIDGET *widget, int request)
{
    if (widget->request != request) {
        widget->request = request;
        widget->requestTime = g_clock.realTime;
    }
}

// .rodata order (docs/guides/tu-merging.md "Phase 5: data order"): a named constant is emitted where it is
// defined, a function's literals and jump tables with the function, so each constant is defined where the ELF
// has it among the functions, under an asm label (no function sees a definition under the name it reads).
namespace RO_ORDER { extern const char r_D_002A8AA0[] __asm__("D_002A8AA0") = "%d"; }
namespace RO_ORDER { extern const char r_D_002A8AA8[] __asm__("D_002A8AA8") = "%d/%d"; }
namespace RO_ORDER { extern const char r_D_002A8AB0[] __asm__("D_002A8AB0") = "%d:%02d"; }
namespace RO_ORDER { extern const char r_D_002A8AB8[] __asm__("D_002A8AB8") = "x%d"; }
namespace RO_ORDER { extern const char r_D_002A8AC0[] __asm__("D_002A8AC0") = "x%5.3f"; }
namespace RO_ORDER { extern const char r_D_002A8AC8[] __asm__("D_002A8AC8") = "Far"; }
namespace RO_ORDER { extern const char r_D_002A8AD0[] __asm__("D_002A8AD0") = "Near"; }
namespace RO_ORDER { extern const char r_D_002A8AD8[] __asm__("D_002A8AD8") = "YIKES"; }
namespace RO_ORDER { extern const char r_D_002A8AE0[] __asm__("D_002A8AE0") = "&2S&."; }   /* the string D_002C40A0 points at (emitted with its definition in the original) */
namespace RO_ORDER { extern const char r_D_002A8AE8[] __asm__("D_002A8AE8") = "ZRB."; }
// 0x212A70 SetWidgetState
namespace M_SetWidgetState {
extern int D_002DD318 __asm__("D_002DD318");
extern int D_002BE538 __asm__("D_002BE538");
void SetClockRate(CLOCK *,int,float) __asm__("SetClockRate");
void LoadWidgetSprites(WIDGET *) __asm__("LoadWidgetSprites");
void StopWidgetMainAnim(WIDGET *) __asm__("StopWidgetMainAnim");
void StartWidgetMainAnim(WIDGET *,int) __asm__("StartWidgetMainAnim");
} // namespace M_SetWidgetState
// The widget state machine: 0 unloaded, 1 hidden, 2 visible, 3 appearing, 4 disappearing; keeps the world's visible
// widget count and pauses the clock for pausing widgets.
// Also known as: SetState (upstream readable / Sly 1 name; SetState__FP8SCREENRTi).
extern "C" void SetWidgetState(WIDGET *widget, int state)
{
    using namespace M_SetWidgetState;
    if (widget->state == state)
        return;
    /* states >= VISIBLE are on screen */
    if (widget->priority >= 16) {
        if (widget->state < WIDGET_STATE_VISIBLE && state >= WIDGET_STATE_VISIBLE)
            g_sw.numShownWidgets++;
        else if (state < WIDGET_STATE_VISIBLE && widget->state >= WIDGET_STATE_VISIBLE)
            g_sw.numShownWidgets--;
    }
    if (widget->pausesGame) {
        if (widget->state < WIDGET_STATE_VISIBLE && state >= WIDGET_STATE_VISIBLE) {
            if (++D_002DD318 == 1)
                SetClockRate(&g_clock, 7, 0.0f);   /* clock pause slot 7: stop */
        } else if (state < WIDGET_STATE_VISIBLE && widget->state >= WIDGET_STATE_VISIBLE) {
            if (--D_002DD318 == 0)
                SetClockRate(&g_clock, 7, 1.0f);   /* run again */
        }
    }
    if (widget->state == WIDGET_STATE_UNLOADED)
        LoadWidgetSprites(widget);
    widget->state = state;
    widget->stateTime = g_clock.realTime;
    widget->stateFrame = D_002BE538;
    switch (state) {
    case WIDGET_STATE_UNLOADED: UnloadWidgetSprites(widget); break;
    case WIDGET_STATE_HIDDEN: StopWidgetMainAnim(widget); break;
    case WIDGET_STATE_VISIBLE: StartWidgetMainAnim(widget, widget->loopAnimId); break;
    case WIDGET_STATE_APPEARING: StartWidgetMainAnim(widget, widget->showAnimId); break;
    case WIDGET_STATE_DISAPPEARING: StartWidgetMainAnim(widget, widget->hideAnimId); break;
    }
}

// 0x212C50 UpdateWidget
namespace M_UpdateWidget {
void AdvanceWidgetSprite(void *,float) __asm__("AdvanceWidgetSprite");
} // namespace M_UpdateWidget
// Per-frame widget update: advances the state machine from requests, animation completion and duration, and advances
// the main sprite. Runtime-checked: state 1 hidden, 2 visible.
void WIDGET::Update()
{
    using namespace M_UpdateWidget;
    int nextState;
    LO::Update();
    nextState = state;
    if (nextState == WIDGET_STATE_HIDDEN) {
        if (request == WIDGET_REQUEST_SHOW)
            nextState = showAnimId == -1 ? WIDGET_STATE_VISIBLE : WIDGET_STATE_APPEARING;
    } else if (requestTime != g_clock.realTime) {   /* not requested this frame */
        switch (nextState) {
        case WIDGET_STATE_VISIBLE:
            if (!IsWidgetAnimPlaying(this, mainAnimId))
                SetWidgetRequest(this, WIDGET_REQUEST_HIDE);
            if (visibleTimeout > 0.0f && visibleTimeout < g_clock.realTime - stateTime)
                SetWidgetRequest(this, WIDGET_REQUEST_HIDE);
            if (request == WIDGET_REQUEST_HIDE)
                nextState = hideAnimId == -1 ? WIDGET_STATE_HIDDEN : WIDGET_STATE_DISAPPEARING;
            break;
        case WIDGET_STATE_APPEARING:
            if (!IsWidgetAnimPlaying(this, showAnimId)) {
                switch (request) {
                case WIDGET_REQUEST_HIDE: nextState = hideAnimId == -1 ? WIDGET_STATE_HIDDEN : WIDGET_STATE_DISAPPEARING; break;
                case WIDGET_REQUEST_SHOW: nextState = WIDGET_STATE_VISIBLE; break;
                }
            }
            break;
        case WIDGET_STATE_DISAPPEARING:
            if (!IsWidgetAnimPlaying(this, hideAnimId)) {
                switch (request) {
                case WIDGET_REQUEST_HIDE: nextState = WIDGET_STATE_HIDDEN; break;
                case WIDGET_REQUEST_SHOW: nextState = showAnimId == -1 ? WIDGET_STATE_VISIBLE : WIDGET_STATE_APPEARING; break;
                }
            }
            break;
        }
    }
    SetWidgetState(this, nextState);
    if (mainAnimId != -1) {
        float dt = g_clock.realDeltaTime;
        mainAnimTime += dt;
        AdvanceWidgetSprite(spriteGroups, dt);
    }
}

// 0x212E40 SetWidgetAnimPhase
namespace M_SetWidgetAnimPhase {
void SetSpriteDefaultPhase(SPRITEGRP *pa, float phase) __asm__("SetSpriteDefaultPhase");
void SetSpriteAnimPhase(SPRITEGRP *pa, int animIndex, float g, int z) __asm__("SetSpriteAnimPhase");
} // namespace M_SetWidgetAnimPhase
// Seeks a widget animation (0x314 = main) to a fraction of its length (meters and gauges).
extern "C" void SetWidgetAnimPhase(WIDGET *widget, int animId, float phase)
{
    using namespace M_SetWidgetAnimPhase;
    int groupIndex, animIndex;
    if (animId == 0x314) {
        SetSpriteDefaultPhase(widget->spriteGroups, phase);
        return;
    }
    for (groupIndex = 0; groupIndex < widget->numSpriteGroups; groupIndex++) {
        SPRITEGRP *group = &widget->spriteGroups[groupIndex];
        for (animIndex = 0; animIndex < group->numAnims; animIndex++) {
            SPRITEANIM *anim = &group->anims[animIndex];
            int slotIndex = -1;
            if (animId == anim->id) {
                int i;
                for (i = 0; i < group->numSlots; i++)
                    if (group->slots[i].animIndex != -1)
                        slotIndex = i;
                if (slotIndex >= 0)
                    SetSpriteAnimPhase(group, slotIndex, phase, 0);
            }
        }
    }
}

// 0x212F80 SetWidgetAnimTime
namespace M_func_00212F80 {
void SetSpriteDefaultTime(SPRITEGRP *pa, float phase) __asm__("SetSpriteDefaultTime");
void SetSpriteAnimTime(SPRITEGRP *pa, int animIndex, float g, int z) __asm__("SetSpriteAnimTime");
// 0x212F78: dead-code scrap (sly2/scrap.h)
extern "C" { SLY2_SCRAP("addiu $sp, $sp, 0xb0"); }
} // namespace M_func_00212F80
// Seeks a widget animation to an absolute time. (inferred)
// Also known as: SetScreenObjTime (upstream readable / Sly 1 name; SetScreenObjTime__FP2LOif).
extern "C" void SetWidgetAnimTime(WIDGET *widget, int animId, float phase)
{
    using namespace M_func_00212F80;
    int groupIndex, animIndex;
    if (animId == 0x314) {
        SetSpriteDefaultTime(widget->spriteGroups, phase);
        return;
    }
    for (groupIndex = 0; groupIndex < widget->numSpriteGroups; groupIndex++) {
        SPRITEGRP *group = &widget->spriteGroups[groupIndex];
        for (animIndex = 0; animIndex < group->numAnims; animIndex++) {
            SPRITEANIM *anim = &group->anims[animIndex];
            int slotIndex = -1;
            if (animId == anim->id) {
                int i;
                for (i = 0; i < group->numSlots; i++)
                    if (group->slots[i].animIndex != -1)
                        slotIndex = i;
                if (slotIndex >= 0)
                    SetSpriteAnimTime(group, slotIndex, phase, 0);
            }
        }
    }
}

// 0x2130C0 QueueWidgetRenderSetup
namespace M_QueueWidgetRenderSetup {
// Queue the render initialization payloads in a DMA packet.

extern VIFS D_002DEA28 __asm__("D_002DEA28");
extern char D_00442210[] __asm__("D_00442210"),D_002DF010[] __asm__("D_002DF010");
void CopyAqw__FPvT0i(void *,void *,int) __asm__("CopyAqw__FPvT0i");
RPL *AllocRenderListEntry(void) __asm__("AllocRenderListEntry");
// Append one word to the packet.
extern "C" { static inline void Word(VIFS *r,uint word){*(uint *)r->m_pb=word;r->m_pb+=4;} }
// Pad the packet with zero words until the cursor is at word `word` of a quadword.
extern "C" { static inline void Align(VIFS *r,int word){while(((uint)r->m_pb&15)!=4*word)Word(r,0);} }
// Append n quadwords from source to the packet.
extern "C" { static inline void Bulk(VIFS *r,int n,void *source){CopyAqw__FPvT0i(r->m_pb,source,n);r->m_pb+=n*16;} }
// Append a VIF DIRECT code for n quadwords (aligned so the data starts on a quadword) followed by the data.
extern "C" { static inline void Direct(VIFS *r,int n,void *source){Align(r,3);Word(r,0x50000000|n);Bulk(r,n,source);} }
// 0x2130B8: dead-code scrap (sly2/scrap.h)
extern "C" { SLY2_SCRAP("addiu $sp, $sp, 0xb0"); }
} // namespace M_QueueWidgetRenderSetup
// Queues the DMA packet that sets up rendering before the widget sprites.
extern "C" void QueueWidgetRenderSetup(WIDGET *p){
    using namespace M_QueueWidgetRenderSetup;
    unsigned char *tag=D_002DEA28.m_pb;
    RPL *e;
    *(ulong *)tag=0x10000000UL;D_002DEA28.m_pb+=16;
    Word(&D_002DEA28,0x11000000);
    Direct(&D_002DEA28,3,D_00442210);
    Direct(&D_002DEA28,0x25,D_002DF010);
    Word(&D_002DEA28,0x11000000);
    Align(&D_002DEA28,0);
    *(uint *)tag|=((D_002DEA28.m_pb-tag)>>4)-1;
    *(ulong *)D_002DEA28.m_pb=0x60000000UL;D_002DEA28.m_pb+=16;
    e=AllocRenderListEntry();
    e->kind=0x15;e->depth=p->priority<<18;
    e->pbCall=(unsigned char *)((uint)tag&0x0FFFFFFF);e->pvCall=0;
}

// 0x2132D0 IsWidgetVisible
// True when the widget's main animation is active and no higher-priority exclusive widget hides it.
// Also known as: FVisible (upstream readable / Sly 1 name; FVisible__FP8SCREENRT).
extern "C" int IsWidgetVisible(WIDGET *widget)
{
    if (widget->mainAnimId == -1) {
        return 0;
    }
    return IsWidgetObscured(widget) == 0;
}

// 0x213300 RenderWidget
namespace M_RenderWidget {
// Flush the sprite render queue and pair begin/end VU microprogram packets.
typedef unsigned int uint;
typedef unsigned char uchar;
struct V4 {float x,y,z,w;} __attribute__((aligned(16)));
struct PARAM {
    int mode,pad4;float priority;int padC;V4 offset,scale;
    // Widget color parameters: zero offset and scale, with the given priority and mode 1.
    PARAM(float p){offset.x=0.0f;offset.y=0.0f;offset.z=0.0f;offset.w=0.0f;scale.x=0.0f;scale.y=0.0f;scale.z=0.0f;scale.w=0.0f;priority=p;mode=1;} PARAM(){}
};
struct Q {uchar color0[4],color1[4];char pad8[8];char matrix[0x20];void *payload;int kind;char pad38[0x29];uchar channel;char pad62[6];Q *next;};
struct LIST {Q *first,*last;};
/* WIDGET island: a WIDGET-typed p (even the same pointer) moves the Align loop's registers (291/316 off) */
struct WIDGETIsland {char pad[0x84];void *sprites;char pad88[0x74];LIST queue;};
extern VIFS D_002DEA28 __asm__("D_002DEA28");

void RenderLoSelf(WIDGETIsland *) __asm__("RenderLoSelf");
int IsWidgetVisible(WIDGETIsland *) __asm__("IsWidgetVisible");
void DrawSprite(void *) __asm__("DrawSprite");
void QueueWidgetRenderSetup(WIDGETIsland *) __asm__("QueueWidgetRenderSetup");
void QueueSpriteVuProgram(WIDGETIsland *,int,float) __asm__("QueueSpriteVuProgram");
void RenderSpritePayload(void *,void *,PARAM *,int) __asm__("RenderSpritePayload");
void ClearSl(LIST *) __asm__("ClearSl");

// Pad the packet with zero words to a 64-byte boundary.
static inline void Align(VIFS *r){while((uint)r->m_pb&63){*(uint *)r->m_pb=0;r->m_pb+=4;}}
// Store the color offset (each channel minus 127).
static inline void Offset(V4 &v,float x,float y,float z,float w){v.x=x-127.0f;v.y=y-127.0f;v.z=z-127.0f;v.w=w-127.0f;}
// Set the color offset from four color bytes.
static inline void SetOffset(V4 &v,const uchar *c){Offset(v,c[0],c[1],c[2],c[3]);}
// Map a color byte to a scale factor ((c - 127) / 127) clamped to [-1, upper].
static inline float ColorScale(int c,float upper){return StdMax(StdMin((c-127)*0.007874016f,upper),-1.0f);}
// Store the four channel scale factors.
static inline void Scale(V4 &v,int a,int b,int d,int e,float upper){float x=ColorScale(a,upper),y=ColorScale(b,upper),z=ColorScale(d,upper),w=ColorScale(e,upper);v.x=x;v.y=y;v.z=z;v.w=w;}
// Set the color scale from the widget's color1 bytes.
static inline void SetScale(V4 &v,const Q *q,float upper){Scale(v,q->color1[0],q->color1[1],q->color1[2],q->color1[3],upper);}
// Switch the packet pointers to the uncached mirror (0x30000000), then queue the widget's render setup.
static inline Q *Begin(WIDGETIsland *p,VIFS *r){Align(r);uint c=(uint)r->m_pb,b=(uint)r->m_pbMax;c|=0x30000000;b|=0x30000000;r->m_pb=(unsigned char *)c;r->m_pbMax=(unsigned char *)b;Q *pending=0;QueueWidgetRenderSetup(p);return pending;}
// Switch the packet pointers back to cached addresses and empty the widget's render queue.
static inline void Finish(VIFS *r,WIDGETIsland *p){Align(r);uint c=(uint)r->m_pb,b=(uint)r->m_pbMax;c&=0x0FFFFFFF;b&=0x0FFFFFFF;r->m_pb=(unsigned char *)c;r->m_pbMax=(unsigned char *)b;Q *q=p->queue.first;while(q){Q *old=q;q=q->next;old->next=0;}ClearSl(&p->queue);}
} // namespace M_RenderWidget
// Renders a widget: flushes its sprite render queue with the VU microprogram packets.
void WIDGET::RenderSelf(){   /* key function: WIDGET's vtable comes out here */
    using namespace M_RenderWidget;
    WIDGETIsland *p=(WIDGETIsland *)this;
    RenderLoSelf(p);
    if(!IsWidgetVisible(p))return;
    DrawSprite(p->sprites);
    float priority=16.0f;
    Q *pending=Begin(p,&D_002DEA28);
    Q *q=p->queue.first;while(q){
        if(q->kind==1){
            PARAM param(priority);
            QueueSpriteVuProgram(p,1,priority-1.0f);
            pending=q;
            RenderSpritePayload(q->payload,q->matrix,&param,q->channel);
            QueueSpriteVuProgram(p,3,param.priority+1.0f);
        }else if(q->kind==4){
            float one=1.0f;
            PARAM param(priority);
            QueueSpriteVuProgram(p,2,priority-one);
            RenderSpritePayload(pending->payload,pending->matrix,&param,pending->channel);
            pending=0;
            QueueSpriteVuProgram(p,4,param.priority+one);
            if(q->payload){
                PARAM second;
                SetOffset(second.offset,q->color0);
                SetScale(second.scale,q,one);second.priority=priority+=1024.0f;second.mode=1;
                RenderSpritePayload(q->payload,q->matrix,&second,q->channel);
            }
        }else if(q->payload){
            PARAM param;
            SetOffset(param.offset,q->color0);
            SetScale(param.scale,q,1.0f);param.priority=priority;param.mode=1;
            RenderSpritePayload(q->payload,q->matrix,&param,q->channel);
        }
    q=q->next;priority+=1024.0f;
    }
    Finish(&D_002DEA28,p);
}

// 0x2137F0 IsWidgetObscured
// True when an exclusive widget of higher priority is shown.
extern "C" int IsWidgetObscured(WIDGET *widget)
{
    WIDGET *other;
    int unused[4];   /* an unused 16-byte stack frame in the original */
    for (other = (*(WIDGET * *)&g_sw.dlWidgetExclusive); other; other = (WIDGET *)other->exclusiveLink.next) {
        if (widget->priority < other->priority)
            return 1;
    }
    return 0;
}

// 0x213830 StartWidgetMainAnim
namespace M_StartWidgetMainAnim {
void StartSpriteAnimById(SPRITEGRP *p) __asm__("StartSpriteAnimById");
} // namespace M_StartWidgetMainAnim
// Starts an animation on the widget's main sprite and joins the exclusive list if the widget is exclusive.
// Also known as: StartPart (upstream readable / Sly 1 name; StartPart__FP8SCREENRTi).
extern "C" void StartWidgetMainAnim(WIDGET *widget, int animId)
{
    using namespace M_StartWidgetMainAnim;
    StartSpriteAnimById(widget->spriteGroups);
    widget->SetFlag8(1);
    if (widget->isExclusive)
        SetWidgetExclusive(widget, 0);
    widget->mainAnimId = animId;
}

// 0x213898 StopWidgetMainAnim
// Stops the widget's main sprite and leaves the exclusive list.
extern "C" void StopWidgetMainAnim(WIDGET *widget)
{
    StopSprite(widget->spriteGroups);
    if (widget->isExclusive)
        SetWidgetExclusive(widget, -1);
    widget->SetFlag8(0);
    widget->HandleSpliceEvent(6, 0, 0, 0, 0);   /* Splice event 6 (inferred) */
    widget->mainAnimTime = 0;
    widget->mainAnimId = -1;
}

// 0x213928 StopWidgetAnim
namespace M_StopWidgetAnim {
void RestartNestedSprite(WIDGET *widget, int groupIndex, int animIndex, int y, int z) __asm__("RestartNestedSprite");
} // namespace M_StopWidgetAnim
// Returns the sprite that plays an animation id to its rest animation.
extern "C" void StopWidgetAnim(WIDGET *widget, int animId)
{
    using namespace M_StopWidgetAnim;
    int groupIndex, animIndex, i;
    for (groupIndex = 0; groupIndex < widget->numSpriteGroups; groupIndex++) {
        SPRITEGRP *group = &widget->spriteGroups[groupIndex];
        for (animIndex = 0; animIndex < group->numAnims; animIndex++) {
            SPRITEANIM *anim = &group->anims[animIndex];
            if (animId == anim->id) {
                int isAnySlotPlaying = 0;
                for (i = 0; i < group->numSlots; i++) {
                    if (group->slots[i].animIndex != -1)
                        isAnySlotPlaying = 1;
                }
                if (isAnySlotPlaying)
                    RestartNestedSprite(widget, groupIndex, 0, 0, 0);
                else
                    group->pendingAnim = -1;
                return;
            }
        }
    }
}

// 0x213A08 PlayWidgetAnimsWithArg
namespace M_func_00213A08 {
void RestartNestedSpriteWithArg(WIDGET *widget, int groupIndex, int animIndex, int x, int y, int z) __asm__("RestartNestedSpriteWithArg");
} // namespace M_func_00213A08
// Starts every sprite that has the animation id, with an argument. (inferred)
extern "C" void PlayWidgetAnimsWithArg(WIDGET *widget, int animId, int arg)
{
    using namespace M_func_00213A08;
    int groupIndex, animIndex;
    for (groupIndex = 0; groupIndex < widget->numSpriteGroups; groupIndex++) {
        SPRITEGRP *group = &widget->spriteGroups[groupIndex];
        for (animIndex = 0; animIndex < group->numAnims; animIndex++) {
            SPRITEANIM *anim = &group->anims[animIndex];
            if (animId == anim->id)
                RestartNestedSpriteWithArg(widget, groupIndex, 0, arg, 0, 0);
        }
    }
}

// 0x213B00 PlayWidgetAnim
namespace M_PlayWidgetAnim {
void RestartNestedSprite(WIDGET *widget, int groupIndex, int animIndex, int y, int z) __asm__("RestartNestedSprite");
} // namespace M_PlayWidgetAnim
// Starts the animation id on the sprite that owns it (restarting in place if it is instanced).
extern "C" void PlayWidgetAnim(WIDGET *widget, int animId)
{
    using namespace M_PlayWidgetAnim;
    int groupIndex, animIndex, i;
    if (animId == -1)
        return;
    for (groupIndex = 0; groupIndex < widget->numSpriteGroups; groupIndex++) {
        SPRITEGRP *group = &widget->spriteGroups[groupIndex];
        for (animIndex = 0; animIndex < group->numAnims; animIndex++) {
            SPRITEANIM *anim = &group->anims[animIndex];
            if (animId == anim->id) {
                int isAnySlotPlaying = 0;
                for (i = 0; i < group->numSlots; i++) {
                    if (group->slots[i].animIndex != -1)
                        isAnySlotPlaying = 1;
                }
                if (isAnySlotPlaying)
                    RestartNestedSprite(widget, groupIndex, animIndex, 0, 0);
                else
                    group->pendingAnim = animIndex;
                return;
            }
        }
    }
}

// 0x213BD8 PlayWidgetAnimWithArg
namespace M_func_00213BD8 {
void RestartNestedSpriteWithArg(WIDGET *widget, int groupIndex, int animIndex, int x, int y, int z) __asm__("RestartNestedSpriteWithArg");
} // namespace M_func_00213BD8
// Starts the first sprite that has the animation id, with an argument. (inferred)
extern "C" void PlayWidgetAnimWithArg(WIDGET *widget, int animId, int arg)
{
    using namespace M_func_00213BD8;
    int groupIndex, animIndex;
    for (groupIndex = 0; groupIndex < widget->numSpriteGroups; groupIndex++) {
        SPRITEGRP *group = &widget->spriteGroups[groupIndex];
        for (animIndex = 0; animIndex < group->numAnims; animIndex++) {
            SPRITEANIM *anim = &group->anims[animIndex];
            if (animId == anim->id) {
                RestartNestedSpriteWithArg(widget, groupIndex, animIndex, arg, 0, 0);
                return;
            }
        }
    }
}

// 0x213C68 SetWidgetXf
// Sets the widget's 2D transform override.
extern "C" void SetWidgetXf(WIDGET *widget, SPRITEXF *src)
{
    if (!widget->xform)
        widget->xform = (SPRITEXF *)widget->spriteHeap->Alloc(0x20, 0x1F);
    *widget->xform = *src;
}

// 0x213CD8 SetWidgetAnimXf
// Sets the 2D transform of the sprite that plays an animation id.
extern "C" void SetWidgetAnimXf(WIDGET *widget, int animId, SPRITEXF *xform)
{
    int groupIndex, animIndex;
    for (groupIndex = 0; groupIndex < widget->numSpriteGroups; groupIndex++) {
        SPRITEGRP *group = &widget->spriteGroups[groupIndex];
        for (animIndex = 0; animIndex < group->numAnims; animIndex++) {
            SPRITEANIM *anim = &group->anims[animIndex];
            if (anim->id == animId) {
                if (!group->xform)
                    group->xform = (SPRITEXF *)widget->spriteHeap->Alloc(0x20, 0x1F);
                *group->xform = *xform;
                return;
            }
        }
    }
}

// 0x213DB0 SwitchWidgetAnim
// Stops the animation in *pk, plays the new one and stores its id.
// Also known as: SelectScreenObjPartHandle (upstream readable / Sly 1 name; SelectScreenObjPartHandle__FP2LOiPi).
extern "C" void SwitchWidgetAnim(WIDGET *widget, int animId, int *currentAnimId)
{
    if (animId == *currentAnimId)
        return;
    if (*currentAnimId != -1)
        StopWidgetAnim(widget, *currentAnimId);
    *currentAnimId = animId;
    if (animId != -1)
        PlayWidgetAnim(widget, animId);
}

// 0x213E20 FindSpriteItemPose
// Recursively finds nested sprite k and returns its 2D transform and colours. (inferred)
extern "C" int FindSpriteItemPose(WIDGET *widget, int groupIndex, SPRITEXF *matrix, struct RGBA *color0, struct RGBA *color1, int numItems, SPRITEITEM *items)
{
    int i;
    SPRITEITEM *item;
    if (!items) {
        SPRITESLOT *slot = widget->spriteGroups->slots;
        numItems = slot->numItems;
        items = slot->items;
    }
    for (i = 0; i < numItems; i++) {
        SPRITEDESC *descriptor;
        item = &items[i];
        descriptor = item->descriptor;
        if (!descriptor || descriptor->kind != SPRITE_KIND_NESTED)
            continue;
        if (groupIndex == descriptor->index) {
            *matrix = item->matrix;
            *color0 = item->color0;
            *color1 = item->color1;
            return 1;
        }
        if (FindSpriteItemPose(widget, groupIndex, matrix, color0, color1, widget->spriteGroups[descriptor->index].slots[item->spriteSlot].numItems, widget->spriteGroups[descriptor->index].slots[item->spriteSlot].items))
            return 1;
    }
    return 0;
}

// 0x213F70 RestartNestedSprite
namespace M_func_00213F70 {
typedef int qword __attribute__((mode(TI)));
// Packed color word, quadword aligned.
struct RGBA { unsigned int u; RGBA(unsigned int x) { u = x; } } __attribute__((aligned(16)));
extern void ClearSle(SLE *p) __asm__("ClearSle");
extern int FindFreeSpriteSlot(SPRITEGRP *pa) __asm__("FindFreeSpriteSlot");
extern void StartSpriteAnimAt(SPRITEGRP *pa, int i, int z, void *q, float f, const RGBA &c0, const RGBA &c1) __asm__("StartSpriteAnimAt");
// Recursively finds nested sprite item k and restarts it on animation z. (inferred)
extern "C" int RestartNestedSprite(WIDGET *widget, int groupIndex, int animIndex, int numItems, SPRITEITEM *items)
{
    int i;
    SPRITEITEM *item;
    if (!items) {
        SPRITESLOT *slot = widget->spriteGroups->slots;
        numItems = slot->numItems;
        items = slot->items;
    }
    for (i = 0; i < numItems; i++) {
        SPRITEDESC *descriptor;
        item = &items[i];
        descriptor = item->descriptor;
        if (!descriptor || descriptor->kind != SPRITE_KIND_NESTED)
            continue;
        int oldSlot=item->spriteSlot;
        if (descriptor->index == groupIndex) {
            StopSpriteSlot(&widget->spriteGroups[groupIndex], oldSlot);
            ClearSle(&item->link);
            item->spriteSlot = FindFreeSpriteSlot(&widget->spriteGroups[groupIndex]);
            StartSpriteAnimAt(&widget->spriteGroups[groupIndex], animIndex, item->spriteSlot, &item->matrix, 0.0f, RGBA(0x7F7F7F7F), RGBA(0xFFFFFFFF));
            return 1;
        }
        {
            /* &spriteGroups[...].slots[oldSlot], spelled as byte arithmetic: the indexed form schedules differently */
            SPRITESLOT *slot = (SPRITESLOT *)(oldSlot * sizeof(SPRITESLOT) + (int)widget->spriteGroups[descriptor->index].slots);
            if (RestartNestedSprite(widget, groupIndex, animIndex, slot->numItems, slot->items))
                return 1;
        }
    }
    return 0;
}
} // namespace M_func_00213F70

// 0x214100 RestartNestedSpriteWithArg
namespace M_func_00214100 {
typedef int qword __attribute__((mode(TI)));
// Packed color word, quadword aligned.
struct RGBA { unsigned int u; RGBA(unsigned int x) { u = x; } } __attribute__((aligned(16)));
extern void ClearSle(SLE *p) __asm__("ClearSle");
extern int FindFreeSpriteSlot(SPRITEGRP *pa) __asm__("FindFreeSpriteSlot");
extern void StartSpriteAnimAt(SPRITEGRP *pa, int i, int z, void *q, float f, const RGBA &c0, const RGBA &c1) __asm__("StartSpriteAnimAt");
// RestartNestedSprite with an extra argument. (inferred)
// Also known as: FRestartAt (upstream readable / Sly 1 name; FRestartAt__13CScreenPlayeriiiiPv).
extern "C" int RestartNestedSpriteWithArg(WIDGET *widget, int groupIndex, int animIndex, int arg, int numItems, SPRITEITEM *items)
{
    int i;
    SPRITEITEM *item;
    if (!items) {
        SPRITESLOT *slot = widget->spriteGroups->slots;
        numItems = slot->numItems;
        items = slot->items;
    }
    for (i = 0; i < numItems; i++) {
        SPRITEDESC *descriptor;
        item = &items[i];
        descriptor = item->descriptor;
        if (!descriptor || descriptor->kind != SPRITE_KIND_NESTED)
            continue;
        int oldSlot=item->spriteSlot;
        if (descriptor->index == groupIndex) {
            if (arg != item->arg)
                continue;
            StopSpriteSlot(&widget->spriteGroups[groupIndex], oldSlot);
            ClearSle(&item->link);
            item->spriteSlot = FindFreeSpriteSlot(&widget->spriteGroups[groupIndex]);
            StartSpriteAnimAt(&widget->spriteGroups[groupIndex], animIndex, item->spriteSlot, &item->matrix, 0.0f, RGBA(0x7F7F7F7F), RGBA(0xFFFFFFFF));
            return 1;
        }
        {
            /* &spriteGroups[...].slots[oldSlot], spelled as byte arithmetic: the indexed form schedules differently */
            SPRITESLOT *slot = (SPRITESLOT *)(oldSlot * sizeof(SPRITESLOT) + (int)widget->spriteGroups[descriptor->index].slots);
            if (RestartNestedSpriteWithArg(widget, groupIndex, animIndex, arg, slot->numItems, slot->items))
                return 1;
        }
    }
    return 0;
}
} // namespace M_func_00214100

// 0x2142B0 HasSpriteAnim
// True when the sprite's animation list contains the id.
// Also known as: FRootHasPart (upstream readable / Sly 1 name; FRootHasPart__FP8SCREENRTPvi).
extern "C" int HasSpriteAnim(void *pv, SPRITEGRP *group, int id)
{
    int i;
    if (id == -1)
        return 0;
    for (i = 0; i < group->numAnims; i++) {
        if (id == group->anims[i].id)
            return 1;
    }
    return 0;
}

// 0x214300 SetWidgetExclusive
/* Sets the widget's +0xA8 flag; a widget with the flag clear is kept on the exclusive list g_sw.dlWidgetExclusive. */
// Sets whether the widget is exclusive (an exclusive widget hides lower-priority widgets while shown). (inferred)
// Also known as: SetScreenObjListed (upstream readable / Sly 1 name; SetScreenObjListed__FP2LOi).
extern "C" void SetWidgetExclusive(WIDGET *widget, int isNonExclusive)
{
    int wasNonExclusive = widget->isNonExclusive;
    if (wasNonExclusive == isNonExclusive)
        return;
    if (!wasNonExclusive && IsEntryInDl(&g_sw.dlWidgetExclusive, widget))
        RemoveDlEntry(&g_sw.dlWidgetExclusive, widget);
    widget->isNonExclusive = isNonExclusive;
    if (!isNonExclusive && !IsEntryInDl(&g_sw.dlWidgetExclusive, widget))
        AppendDlEntry(&g_sw.dlWidgetExclusive, widget);
}

// 0x214398 LoadSpriteTable
namespace M_LoadSpriteTable {
extern int D_002C40A4 __asm__("D_002C40A4");
int U16Read__18CBinaryInputStream(void *a) __asm__("U16Read__18CBinaryInputStream");
int AllocZeroedFromHeap__FP4HEAPii(void *heap, int cb, int k) __asm__("AllocZeroedFromHeap__FP4HEAPii");
void LoadSpriteDescriptor(WIDGET *p, void *a, void *b, int n, int m) __asm__("LoadSpriteDescriptor");
} // namespace M_LoadSpriteTable
// Loads a sprite table: the count, then each sprite descriptor.
// Layout parse-verified: a parser following it reads every byte of 4 levels.
// Also known as: LoadElemsFromBrx (upstream readable / Sly 1 name; LoadElemsFromBrx__FPvP18CBinaryInputStreamT0PiT3).
extern "C" void LoadSpriteTable(WIDGET *p, void *a, void *b, int *pn, int *pm)
{
    using namespace M_LoadSpriteTable;
    int i;
    *pn = U16Read__18CBinaryInputStream(a);
    *pm = AllocZeroedFromHeap__FP4HEAPii(p->spriteHeap, (*pn + 1) << 6, D_002C40A4 ? 0x13 : 0x11);
    for (i = 1; i <= *pn; i++)
        LoadSpriteDescriptor(p, a, b, *pn, *pm);
}

// 0x214460 LoadSpriteGroups
namespace M_LoadSpriteGroups {
// Deserialize sprite groups, reusable frames and their animation keys.
typedef unsigned int uint;
typedef unsigned short ushort;
typedef unsigned char uchar;
struct BIS;
struct ROOT;
struct GROUP;
struct FRAME {char fields[0x70];};
struct V3 {float x,y,z;} __attribute__((aligned(16)));
struct M3 {V3 row[3];};
struct M23 {float a[6];} __attribute__((aligned(16)));
struct RGBA_RAW {uchar r,g,b,a;};
// Packed color word, quadword aligned.
union RGBA {uint word;RGBA_RAW raw;RGBA(uint value){word=value;}} __attribute__((aligned(16)));
struct COLORS {RGBA_RAW first,second;};
struct ITEM {ushort a0,a2,a4;short a6;char pad8[8];M23 matrix;uchar flags,mode;char pad32[2];int id;char pad38[8];};
struct TAG {uint kind;union {int signedBits;uint bits;} value;};
struct KEY {float time;uchar itemCount,colorCount,shortCount,byteCount;uchar *bytes;ITEM *items;COLORS *colors;ushort *shorts;uchar tagCount;char pad19[3];TAG *tags;};
struct ANIM {ushort count,pad2;KEY *keys;float duration;ROOT *owner;GROUP *group;int id;};
struct REUSE {int count;FRAME *frames;short index,phase;void *previous;float elapsed;int pad14;};
struct GROUP {ushort animCount,reuseCount;ANIM *animations;REUSE *reuse;char padC[0x14];int index;ROOT *owner;int first;int pad2C;};
struct ROOT {char pad[0x80];ushort groupCount,pad82;GROUP *groups;};

int U16Read__18CBinaryInputStream(BIS *) __asm__("U16Read__18CBinaryInputStream");
uint U32Read__18CBinaryInputStream(BIS *) __asm__("U32Read__18CBinaryInputStream");
int U8Read__18CBinaryInputStream(BIS *) __asm__("U8Read__18CBinaryInputStream");
int S16Read__18CBinaryInputStream(BIS *) __asm__("S16Read__18CBinaryInputStream");
int S32Read__18CBinaryInputStream(BIS *) __asm__("S32Read__18CBinaryInputStream");
float F32Read__18CBinaryInputStream(BIS *) __asm__("F32Read__18CBinaryInputStream");
void ReadMatrix__18CBinaryInputStreamP7MATRIX3(BIS *,M3 *) __asm__("ReadMatrix__18CBinaryInputStreamP7MATRIX3");
void *AllocZeroedFromHeap__FP4HEAPii(HEAP *,int,int) __asm__("AllocZeroedFromHeap__FP4HEAPii");
void InitSpriteItem(FRAME *) __asm__("InitSpriteItem");

// Copy the xy parts of the three rows of src into a 2x3 affine matrix.
static inline void CopyXY(M23 *dst,const M3 &src){
 dst->a[0]=src.row[0].x;dst->a[1]=src.row[0].y;
 dst->a[2]=src.row[1].x;dst->a[3]=src.row[1].y;
 dst->a[4]=src.row[2].x;dst->a[5]=src.row[2].y;
}
// Loads sprite groups, their reusable frames and animation keys.
// Layout parse-verified: a parser following it reads every byte of 4 levels.
// Also known as: LoadGroupsFromBrx (upstream readable / Sly 1 name;
//   LoadGroupsFromBrx__FP5OWNERP18CBinaryInputStreamP4ROOT).
extern "C" void LoadSpriteGroups(WIDGET *owner,BIS *stream,ROOT *root){
 int i,j,k,m;
 root->groupCount=U16Read__18CBinaryInputStream(stream);
 root->groups=(GROUP *)AllocZeroedFromHeap__FP4HEAPii(owner->spriteHeap,(int)root->groupCount*0x30,0x12);
 for(i=0;i<root->groupCount;i++){
  GROUP *group=&root->groups[i];
  group->first=(i==0);group->owner=root;
  group->reuseCount=U16Read__18CBinaryInputStream(stream);
  group->reuse=(REUSE *)AllocZeroedFromHeap__FP4HEAPii(owner->spriteHeap,(int)group->reuseCount*0x18,0x12);
  int count=U16Read__18CBinaryInputStream(stream);
  for(j=0;j<group->reuseCount;){
   int next=j+1;
   REUSE *reuse=&group->reuse[j];
   reuse->previous=0;reuse->index=-1;reuse->phase=0;reuse->elapsed=0.0f;
   reuse->count=count;
   reuse->frames=(FRAME *)AllocZeroedFromHeap__FP4HEAPii(owner->spriteHeap,count*0x70,0x10);
   for(m=0;m<reuse->count;m++)InitSpriteItem(&reuse->frames[m]);
   j=next;
  }
  group->animCount=U16Read__18CBinaryInputStream(stream);
  group->animations=(ANIM *)AllocZeroedFromHeap__FP4HEAPii(owner->spriteHeap,(int)group->animCount*0x18,0x12);
  group->index=-1;
  for(j=0;j<group->animCount;j++){
   ANIM *animation=&group->animations[j];
   animation->owner=root;animation->group=group;
   animation->duration=F32Read__18CBinaryInputStream(stream);
   animation->id=S32Read__18CBinaryInputStream(stream);
   animation->count=U16Read__18CBinaryInputStream(stream);
   animation->keys=(KEY *)AllocZeroedFromHeap__FP4HEAPii(owner->spriteHeap,(int)animation->count*0x20,0x12);
   for(k=0;k<animation->count;k++){
    KEY *key=&animation->keys[k];
    key->time=F32Read__18CBinaryInputStream(stream);
    key->itemCount=U8Read__18CBinaryInputStream(stream);
    key->items=(ITEM *)AllocZeroedFromHeap__FP4HEAPii(owner->spriteHeap,(int)key->itemCount*0x40,0x12);
    for(m=0;m<key->itemCount;m++){
     ITEM *item=&key->items[m];
     item->a0=U16Read__18CBinaryInputStream(stream);item->a2=U16Read__18CBinaryInputStream(stream);item->a4=U16Read__18CBinaryInputStream(stream);item->a6=S16Read__18CBinaryInputStream(stream);
     item->flags=U8Read__18CBinaryInputStream(stream);item->mode=U8Read__18CBinaryInputStream(stream);item->id=S32Read__18CBinaryInputStream(stream);
     M3 raw;ReadMatrix__18CBinaryInputStreamP7MATRIX3(stream,&raw);M3 matrix=raw;CopyXY(&item->matrix,matrix);
    }
    key->colorCount=U8Read__18CBinaryInputStream(stream);
    key->colors=(COLORS *)AllocZeroedFromHeap__FP4HEAPii(owner->spriteHeap,(int)key->colorCount*8,0x12);
    for(m=0;m<key->colorCount;m++){
     COLORS *colors=&key->colors[m];
     RGBA second(U32Read__18CBinaryInputStream(stream));RGBA first(U32Read__18CBinaryInputStream(stream));
     colors->second=*(RGBA_RAW *)&second;colors->first=*(RGBA_RAW *)&first;
    }
    key->shortCount=U8Read__18CBinaryInputStream(stream);
    key->shorts=(ushort *)AllocZeroedFromHeap__FP4HEAPii(owner->spriteHeap,(int)key->shortCount*2,0x12);
    for(m=0;m<key->shortCount;m++){
     ushort *dst=&key->shorts[m];*dst=U16Read__18CBinaryInputStream(stream);
    }
    key->byteCount=U8Read__18CBinaryInputStream(stream);
    key->bytes=(uchar *)AllocZeroedFromHeap__FP4HEAPii(owner->spriteHeap,(int)key->byteCount,0x12);
    for(int byteIndex=0;byteIndex<key->byteCount;byteIndex++)key->bytes[byteIndex]=U8Read__18CBinaryInputStream(stream);
    key->tagCount=U8Read__18CBinaryInputStream(stream);
    key->tags=(TAG *)AllocZeroedFromHeap__FP4HEAPii(owner->spriteHeap,(int)key->tagCount*8,0x12);
    for(m=0;m<key->tagCount;m++){
     key->tags[m].kind=U8Read__18CBinaryInputStream(stream);
     if(key->tags[m].kind<2)key->tags[m].value.signedBits=S32Read__18CBinaryInputStream(stream);
     else key->tags[m].value.bits=(uint)S32Read__18CBinaryInputStream(stream);
    }
   }
  }
 }
}
} // namespace M_LoadSpriteGroups

namespace RO_ORDER {   /* pointers through asm-labelled declarations */
void p_func_002155C0() __asm__("BuildSpritePictureTexPacket");
extern const unsigned int r_D_002A8B08[2] __asm__("D_002A8B08") = {0xFFFF0000, (unsigned int)p_func_002155C0};
}
// 0x2149D8 LoadSpriteDescriptor
namespace M_LoadSpriteDescriptor {
// LoadSpriteDescriptor: reads one sprite descriptor from the stream into sprites[id] and builds its packet data.
// Kinds: 4 index, 8 nested children + word/float tables, 3 shader, 2 polygon (2x3 matrix + vertices),
// 6/7 textured rectangle (GIF packet; 7 also posts the owner's callback), 1 geometry records grouped
// into VU1 packets (microprogram 0x140).
// How it matched (Lilly's agent, T1615), from codex18fresh1 (17 diffs):
// - the scopes are real out-of-line STACKSCOPEs (0x1AE478 / 0x1AE4A0), which keeps &inner in s3;
// - owner is a const parameter: its spill slot is unchanging, so its reloads move above the stores;
// - the texture id is stored in both rectangle arms (a global pseudo: s2), float literals, natural loops,
//   bytes[n*3+k] indexing, packetCount / samples / groups update order;
// - j is declared in case 8 after the float count is read and reused by case 1's packet loop: its pseudo
//   number puts the gcse temp for j+1 between &packetCount and &packet, which gives their spill slots.
typedef int qword __attribute__((mode(TI)));
struct BIS;
struct OWNER : LO { char pad[0xC4 - 0x50]; HEAP *heap;};
struct ROOT;
struct SPRITE;
struct M23 {float a[6];} __attribute__((aligned(16)));
struct PAIR {short x,y;};
struct RAWCOLOR {uchar r,g,b,a;};
struct VERTEX {uchar data[0x18];};
struct NESTED {int count;SPRITE *children;int floatCount;float *floats;int wordCount;uint *words;};
struct SHADER {int tag,mode,id;uint slot;float size,value,scale;int flag,special;};
struct POLYGON {M23 matrix;ushort count,pad22;VERTEX *vertices;};
struct EFFECT {int id;void *handle;int count;QW *packet;};
struct PICTURE {int id,count;QW *packet;};
struct GEOMETRY {ushort count,groups;int samples,packetCount;QW *packet;};
union PAYLOAD {int index;NESTED nested;SHADER shader;POLYGON polygon;EFFECT effect;PICTURE picture;GEOMETRY geometry;};
struct SPRITE {int id,kind;ROOT *root;int fieldC;PAYLOAD data;};
struct RECORD {int kind,id;RAWCOLOR color;uchar count,padD[3];int budget;uchar format,pad15[3];PAIR *coordinates;uchar extra,pad1D[3];uchar *extras;};
struct GROUP {int count,budget,samples;RECORD *begin,*end;};
// Native two-word member callback, stored as {delta,index,function}.
typedef void (OWNER::*PMF)(SPRITE *);

int U16Read__18CBinaryInputStream(BIS *) __asm__("U16Read__18CBinaryInputStream");
uint U32Read__18CBinaryInputStream(BIS *) __asm__("U32Read__18CBinaryInputStream");
int U8Read__18CBinaryInputStream(BIS *) __asm__("U8Read__18CBinaryInputStream");
int S16Read__18CBinaryInputStream(BIS *) __asm__("S16Read__18CBinaryInputStream");
int S8Read__18CBinaryInputStream(BIS *) __asm__("S8Read__18CBinaryInputStream");
int S32Read__18CBinaryInputStream(BIS *) __asm__("S32Read__18CBinaryInputStream");
float F32Read__18CBinaryInputStream(BIS *) __asm__("F32Read__18CBinaryInputStream");
void ReadMatrix__18CBinaryInputStreamP7MATRIX3(BIS *,MATRIX3Q *) __asm__("ReadMatrix__18CBinaryInputStreamP7MATRIX3");
void Read__18CBinaryInputStreamiPv(BIS *,int,void *) __asm__("Read__18CBinaryInputStreamiPv");
void *AllocZeroedFromHeap__FP4HEAPii(HEAP *,int,int) __asm__("AllocZeroedFromHeap__FP4HEAPii");
void *AllocClearedFromStack(SWALLOC *) __asm__("AllocClearedFromStack");
void LoadSpriteTable(OWNER *,BIS *,ROOT *,int *,SPRITE **) __asm__("LoadSpriteTable");
void PostSwCallback(SW *,OWNER *,PMF,SPRITE *) __asm__("PostSwCallback");

extern int D_002C40A4 __asm__("D_002C40A4");
extern PMF D_002A8B08 __asm__("D_002A8B08");
extern QW D_002A2E10 __asm__("D_002A2E10");
extern char D_00000650[] __asm__("D_00000650"),D_00000140[] __asm__("D_00000140");
// Fill the request and allocate bytes with the given alignment.
static inline void *Acquire(SWALLOC *request,int bytes,int alignment){request->cb=bytes;request->cbAlign=alignment;return AllocClearedFromStack(request);}
// Copy the xy parts of the three rows of src into a 2x3 affine matrix.
static inline void CopyXY(M23 &dst,const MATRIX3Q &src){
 const float *a=(const float *)&src.r[0].q,*b=(const float *)&src.r[1].q,*c=(const float *)&src.r[2].q;
 dst.a[0]=a[0];dst.a[1]=a[1];dst.a[2]=b[0];dst.a[3]=b[1];dst.a[4]=c[0];dst.a[5]=c[1];
}
// Loads one sprite descriptor (geometry, polygon, shader, index, picture or nested sprite) and builds its packets.
// Layout parse-verified: a parser following it reads every byte of 4 levels.
// Also known as: LoadElemFromBrx (upstream readable / Sly 1 name;
//   LoadElemFromBrx__FP5OWNERP18CBinaryInputStreamP4ROOTiP6SPRITE).
extern "C" void LoadSpriteDescriptor(OWNER *const owner,BIS *stream,ROOT *root,int,SPRITE *sprites){
 STACKSCOPE outer;MATRIX3Q raw;
 int id=U16Read__18CBinaryInputStream(stream);SPRITE *sprite=&sprites[id];sprite->root=root;sprite->id=id;
 sprite->kind=U32Read__18CBinaryInputStream(stream);
 switch(sprite->kind){
 case SPRITE_KIND_NONE:case SPRITE_KIND_UNUSED5:default:break;
 case SPRITE_KIND_NESTED:sprite->data.index=U16Read__18CBinaryInputStream(stream);break;
 case SPRITE_KIND_TABLE:
  D_002C40A4=1;
  LoadSpriteTable(owner,stream,root,&sprite->data.nested.count,&sprite->data.nested.children);
  sprite->data.nested.wordCount=U32Read__18CBinaryInputStream(stream);
  sprite->data.nested.words=(uint *)AllocZeroedFromHeap__FP4HEAPii(owner->heap,sprite->data.nested.wordCount*4,0x13);
  int i;
  for(i=0;i<sprite->data.nested.wordCount;i++)sprite->data.nested.words[i]=U32Read__18CBinaryInputStream(stream);
  sprite->data.nested.floatCount=U32Read__18CBinaryInputStream(stream);
  int j;
  sprite->data.nested.floats=(float *)AllocZeroedFromHeap__FP4HEAPii(owner->heap,sprite->data.nested.floatCount*4,0x13);
  for(j=0;j<sprite->data.nested.floatCount;j++)sprite->data.nested.floats[j]=F32Read__18CBinaryInputStream(stream);
  D_002C40A4=0;break;
 case SPRITE_KIND_TEXT:{
  sprite->data.shader.tag=U32Read__18CBinaryInputStream(stream);sprite->data.shader.slot=U32Read__18CBinaryInputStream(stream);
  sprite->data.shader.size=F32Read__18CBinaryInputStream(stream);sprite->data.shader.scale=F32Read__18CBinaryInputStream(stream);sprite->data.shader.value=F32Read__18CBinaryInputStream(stream);
  sprite->data.shader.flag=S8Read__18CBinaryInputStream(stream);sprite->data.shader.mode=S8Read__18CBinaryInputStream(stream);
  if(sprite->data.shader.mode==1)sprite->data.shader.id=S32Read__18CBinaryInputStream(stream);else if(sprite->data.shader.mode==0)sprite->data.shader.id=S32Read__18CBinaryInputStream(stream);
  if(sprite->data.shader.mode==1&&sprite->data.shader.id==0x2F2)sprite->data.shader.special=sprite->data.shader.mode;
  else sprite->data.shader.special=0;
  break;
 }
 case SPRITE_KIND_POLYGON:{
  ReadMatrix__18CBinaryInputStreamP7MATRIX3(stream,&raw);MATRIX3Q matrix=raw;CopyXY(sprite->data.polygon.matrix,matrix);
  sprite->data.polygon.count=U16Read__18CBinaryInputStream(stream);
  sprite->data.polygon.vertices=(VERTEX *)AllocZeroedFromHeap__FP4HEAPii(owner->heap,sprite->data.polygon.count*sizeof(VERTEX),0x14);
  Read__18CBinaryInputStreamiPv(stream,sprite->data.polygon.count*sizeof(VERTEX),sprite->data.polygon.vertices);break;
 }
 case SPRITE_KIND_EFFECT:case SPRITE_KIND_PICTURE:{
  PAIR first,second;
  first.x=S16Read__18CBinaryInputStream(stream);first.y=S16Read__18CBinaryInputStream(stream);second.x=S16Read__18CBinaryInputStream(stream);second.y=S16Read__18CBinaryInputStream(stream);
  int texture=S32Read__18CBinaryInputStream(stream);
  STACKSCOPE inner;GIFS gif;
  gif.AllocSw(16);gif.AddPrimPack(0,1,0xE);
  if(sprite->kind==SPRITE_KIND_EFFECT){sprite->data.effect.id=texture;sprite->data.effect.handle=0;gif.PackAD(0x47,0x5000F);gif.PackAD(0x42,0x80000000A8L);}
  else{sprite->data.picture.id=texture;gif.PackAD(0x42,0x44);gif.PackAD(0x47,0x50000);}
  gif.AddPrimPack(0x54,9,0x525252521L);gif.PackRGBA(0x80808080);
  if(sprite->kind==SPRITE_KIND_EFFECT){
   gif.PackSTQ(0.0f,0.0f,0.0f);gif.PackXYZ(first.x,first.y,0);
   gif.PackSTQ(1.0f,0.0f,0.0f);gif.PackXYZ(second.x,first.y,0);
   gif.PackSTQ(0.0f,1.0f,0.0f);gif.PackXYZ(first.x,second.y,0);
   gif.PackSTQ(1.0f,1.0f,0.0f);gif.PackXYZ(second.x,second.y,0);
   gif.AddPrimPack(0,1,0xE);gif.PackAD(0x42,0x44);gif.PackAD(0x47,0x50000);
  }
  else{
   gif.PackSTQ(0.0f,1.0f,0.0f);gif.PackXYZ(first.x,first.y,0);
   gif.PackSTQ(1.0f,1.0f,0.0f);gif.PackXYZ(second.x,first.y,0);
   gif.PackSTQ(0.0f,0.0f,0.0f);gif.PackXYZ(first.x,second.y,0);
   gif.PackSTQ(1.0f,0.0f,0.0f);gif.PackXYZ(second.x,second.y,0);
  }
  gif.EndPrim(1);
  VIFS vif;vif.AllocSw(20);vif.AddDmaCnt();
  vif.AddVifUnpack(UPK_V4_32,((uint)gif.m_pb-(uint)gif.m_ab)>>4,gif.m_ab,7);
  vif.AddVifMscal(D_00000650);vif.EndDmaCnt();vif.AddDmaRet();
  if(sprite->kind==SPRITE_KIND_EFFECT)vif.DetachCopySw(owner->heap,&sprite->data.effect.count,0,&sprite->data.effect.packet);
  else{vif.DetachCopySw(owner->heap,&sprite->data.picture.count,0,&sprite->data.picture.packet);PostSwCallback(&g_sw,owner,D_002A8B08,sprite);}
  break;
 }
 case SPRITE_KIND_GEOMETRY:{
  sprite->data.geometry.count=U16Read__18CBinaryInputStream(stream);
  RECORD *records;GROUP *groups;int i;
  { SWALLOC request;
  records=(RECORD *)Acquire(&request,sprite->data.geometry.count*sizeof(RECORD),16);
  groups=(GROUP *)Acquire(&request,sprite->data.geometry.count*sizeof(GROUP),16);
  for(i=0;i<sprite->data.geometry.count;i++){
   RECORD *record=&records[i];
   record->count=U8Read__18CBinaryInputStream(stream);record->format=U8Read__18CBinaryInputStream(stream);
   record->kind=U8Read__18CBinaryInputStream(stream);record->id=U8Read__18CBinaryInputStream(stream);record->extra=U8Read__18CBinaryInputStream(stream);
   record->budget=record->count+2;
   record->coordinates=(PAIR *)Acquire(&request,record->count*sizeof(PAIR),16);
   record->extras=(uchar *)Acquire(&request,record->count,16);
   for(int k=0;k<record->count;k++){
    PAIR *coordinate=&record->coordinates[k];*(uint *)&record->color=U32Read__18CBinaryInputStream(stream);
    record->extras[k]=U8Read__18CBinaryInputStream(stream);
    coordinate->x=S16Read__18CBinaryInputStream(stream);coordinate->y=S16Read__18CBinaryInputStream(stream);
   }
  }
  }
  sprite->data.geometry.groups=0;sprite->data.geometry.packetCount=0;sprite->data.geometry.samples=0;
  for(i=0;i<sprite->data.geometry.count;){
   GROUP *group=&groups[sprite->data.geometry.groups];
   group->count=0;group->samples=0;group->budget=0;group->begin=&records[i];
   while(i<sprite->data.geometry.count&&group->budget+records[i].budget<0xBD){
    group->budget+=records[i].budget;group->samples+=records[i].count;group->count++;i++;group->end=&records[i];
   }
   int estimate=(group->samples+1)/4+group->samples/16+(group->count*3+3)/4+3;
   sprite->data.geometry.packetCount+=estimate;
   sprite->data.geometry.samples+=group->samples;
   sprite->data.geometry.groups++;
  }
  VIFS vif;j=0;vif.AllocSw(sprite->data.geometry.packetCount*2);vif.AddDmaCnt();vif.AddVifStrow(&D_002A2E10);
  for(;j<sprite->data.geometry.groups;j++){
   GROUP *group=&groups[j];
   // V2_32 copies eight bytes; only the first header component is written.
   int n=0;QW header;header.an[0]=group->count;vif.AddVifUnpack(UPK_V2_32,1,&header,7);vif.AddVifStcycl(1,2);
   uint *bytes,*colors;
   vif.UnpackHelper(UPK_V3_8,group->count,8,0,&bytes);vif.UnpackHelper(UPK_V4_8,group->count,9,0,&colors);
   int address=group->count*2+8;
   vif.AddVifStcycl(1,1);
   int base=address;
   vif.AddVifStmask(0x50505050);
   for(RECORD *record=group->begin;record!=group->end;record++){
    ((uchar *)bytes)[n*3]=record->kind;((uchar *)bytes)[n*3+1]=record->count;((uchar *)bytes)[n*3+2]=record->format;
    *(RAWCOLOR *)&colors[n]=record->color;n++;
    vif.AddVifUnpack((UPK)(UPK_V2_16+16),record->count,record->coordinates,address);address+=record->count;
   }
   vif.AddVifStmask(0x3F3F3F3F);address=base;
   for(RECORD *record=group->begin;record!=group->end;record++){
    if(record->extra)vif.AddVifUnpack((UPK)(UPK_S8+16),record->count,record->extras,address);
    address+=record->count;
   }
   vif.AddVifStmask(0);vif.AddVifMscal(D_00000140);
  }
  vif.EndDmaCnt();vif.AddDmaRet();vif.DetachCopySw(owner->heap,&sprite->data.geometry.packetCount,0,&sprite->data.geometry.packet);break;
 }
 }
}
} // namespace M_LoadSpriteDescriptor

// 0x2155C0 BuildSpritePictureTexPacket
namespace M_func_002155C0 {

void STACKSCOPE_ctor(void *p) __asm__("STACKSCOPE_ctor");
void STACKSCOPE_ctor_1AE4A0(void *p, int n) __asm__("_$_10STACKSCOPE");
SHDBLIP *FindShaderById(int oid) __asm__("FindShaderById");

/* a picture sprite descriptor: kind, texture id, then the TEX packet built here */
struct Q { int id; int kind; char pad8[8]; int textureId; char pad14[8]; int texPacketCount; QW *texPacket; };
extern void BuildSpritePictureTexPacket(WIDGET *p, Q *q) __asm__("BuildSpritePictureTexPacket");
// Picture-sprite callback: the GIF packet that selects its texture. (inferred)
void BuildSpritePictureTexPacket(WIDGET *owner, Q *sprite)
{
    STACKSCOPE frame;
    if (sprite->kind == SPRITE_KIND_PICTURE) {
        sprite->texPacketCount = 0;
        sprite->texPacket = 0;
        SHDBLIP *texture = FindShaderById(sprite->textureId);
        if (texture) {
            GIFS gifs;
            gifs.AllocSw(0x10);
            gifs.AddPrimPack(0, 1, 0xE);
            gifs.PackAD(6, *texture->atex0);  /* TEX0_1: its first frame's */
            gifs.PackAD(0x14, 0x60);          /* TEX1_1 */
            gifs.PackAD(8, 5);                /* CLAMP_1 */
            gifs.AddPrimEnd();
            gifs.DetachCopySw(owner->spriteHeap, &sprite->texPacketCount, 0, &sprite->texPacket);
        }
    }
}
} // namespace M_func_002155C0

// 0x2156B8 InitSpriteItem
namespace M_InitSpriteItem {
extern SPRITEXF D_002A3A10 __asm__("D_002A3A10");   /* the identity 2x3 matrix (inferred) */
extern void ClearSle(void *p) __asm__("ClearSle");
// Stores a colour as one aligned word (RGBA itself is byte-aligned, so a struct copy would be stored unaligned).
static inline void SetRgbaBits(RGBA &c, unsigned int bits) { *(unsigned int *)&c = bits; }
// Sets a sprite item's defaults: neutral colours, identity transform, no ids.
extern "C" void InitSpriteItem(SPRITEITEM *item)
{
    item->previousMatrix = &D_002A3A10;
    item->descriptor = 0;
    item->nextMatrix = 0;
    /* colour pairs: offset 0x7F (neutral), scale 0xFF (full) per channel */
    SetRgbaBits(item->color0, 0x7F7F7F7F); SetRgbaBits(item->color1, 0xFFFFFFFF);
    SetRgbaBits(item->previousColor0, 0x7F7F7F7F); SetRgbaBits(item->previousColor1, 0xFFFFFFFF);
    SetRgbaBits(item->nextColor0, 0x7F7F7F7F); SetRgbaBits(item->nextColor1, 0xFFFFFFFF);
    item->arg = -1;
    item->spriteSlot = 0;
    item->kind = 0;
    item->cachedTime = 0;
    item->previousTime = 0;
    item->nextTime = 0;
    ClearSle(&item->link);
}
} // namespace M_InitSpriteItem

// 0x215730 SetSpriteItemSource
namespace M_func_00215730 {
struct RGBA2 { unsigned char r, g, b, a; };
struct RGBA {
    unsigned char r, g, b, a;
    // Copy a color as one word.
    RGBA(const RGBA &c) { *(RGBA2 *)this = *(const RGBA2 *)&c; }
} __attribute__((aligned(16)));
extern int FindFreeSpriteSlot(void *p) __asm__("FindFreeSpriteSlot");
extern void StartSpriteAnimSlot(void *p, int k, int z, void *q, RGBA c0, RGBA c1) __asm__("StartSpriteAnimSlot");
extern void *FindObjectForRenderTarget(int) __asm__("FindObjectForRenderTarget");
extern void *AcquireRenderTarget(void *) __asm__("AcquireRenderTarget");
// Binds a sprite item to its payload by kind (a nested sprite starts its main animation). (inferred)
extern "C" void SetSpriteItemSource(SPRITEDESC *sprite, SPRITEITEM *item, void *matrix, RGBA c0, RGBA c1)
{
    item->descriptor = sprite;
    switch (sprite->kind) {
    case SPRITE_KIND_NESTED:
        item->spriteSlot = FindFreeSpriteSlot(&sprite->owner->spriteGroups[sprite->index]);
        StartSpriteAnimSlot(&sprite->owner->spriteGroups[sprite->index], 0x314, item->spriteSlot, matrix, c0, c1);
        break;
    case SPRITE_KIND_EFFECT:
        if (sprite->index != -1) {
            void *resource = FindObjectForRenderTarget(sprite->index);
            if (resource)
                sprite->texture = AcquireRenderTarget(resource);
        }
        break;
    }
}
} // namespace M_func_00215730

// 0x215850 UpdateSpriteItem
namespace M_func_00215850 {
// Forward a sprite frame update with its duration and colors.
struct RGBA2 { unsigned char r, g, b, a; };
struct RGBA {
    unsigned char r, g, b, a;
    // Copy a color as one word.
    RGBA(const RGBA &c) { *(RGBA2 *)this = *(const RGBA2 *)&c; }
} __attribute__((aligned(16)));
extern void UpdateSpriteSlot(void *p, int a, void *b, float g, RGBA c0, RGBA c1) __asm__("UpdateSpriteSlot");
// Forwards a frame update (time, transform, colours) to a nested sprite item. (inferred)
extern "C" void UpdateSpriteItem(float time, SPRITEDESC *sprite, SPRITEITEM *item, void *matrix, RGBA c0, RGBA c1)
{
    if (sprite->kind == SPRITE_KIND_NESTED)
        UpdateSpriteSlot(&sprite->owner->spriteGroups[sprite->index], item->spriteSlot, matrix, time, c0, c1);
}
} // namespace M_func_00215850

// 0x2158C0 ReleaseSpriteItem
namespace M_func_002158C0 {
struct Q { char pad[0x20]; void *resource; };   /* a texture reference */
struct S { int id; int kind; WIDGET *owner; int unknownC; int index; struct Q *texture; };
void StopSpriteSlot(void *p, int a) __asm__("StopSpriteSlot");
void ReleaseRenderTarget(void *p) __asm__("ReleaseRenderTarget");
void InitSpriteItem(SPRITEITEM *item) __asm__("InitSpriteItem");
// Releases a sprite item's payload (stops a nested sprite, drops a texture reference) and resets it. (inferred)
extern "C" void ReleaseSpriteItem(struct S *sprite, SPRITEITEM *item)
{
    switch (sprite->kind) {
    case SPRITE_KIND_NESTED:
        StopSpriteSlot(&sprite->owner->spriteGroups[sprite->index], item->spriteSlot);
        break;
    case SPRITE_KIND_EFFECT:
        if (sprite->texture) {
            if (sprite->texture->resource)
                ReleaseRenderTarget(sprite->texture->resource);
            sprite->texture = 0;
        }
        break;
    }
    InitSpriteItem(item);
}
} // namespace M_func_002158C0

// 0x215960 QueueSpriteItem
namespace M_func_00215960 {
void DrawSpriteSlot(void *p, int a, int b) __asm__("DrawSpriteSlot");
void PrependSlEntry(SL *psl, SPRITEITEM *item) __asm__("PrependSlEntry");
// Queues a sprite item for drawing (nested sprites draw recursively). (inferred)
extern "C" void QueueSpriteItem(SPRITEDESC *sprite, SPRITEITEM *item, int drawFlags)
{
    switch (sprite->kind) {
    case SPRITE_KIND_NESTED:
        DrawSpriteSlot(&sprite->owner->spriteGroups[sprite->index], item->spriteSlot, item->drawFlags);
        break;
    case SPRITE_KIND_GEOMETRY:
    case SPRITE_KIND_TEXT:
    case SPRITE_KIND_EFFECT:
    case SPRITE_KIND_PICTURE:
        item->drawFlags = drawFlags;
        PrependSlEntry(&sprite->owner->renderQueue, item);
        break;
    case SPRITE_KIND_POLYGON:
    case SPRITE_KIND_UNUSED5:
        break;
    }
}
} // namespace M_func_00215960

// 0x2159E0 RenderSpritePayload
namespace M_func_002159E0 {
struct S { int id; int kind; };
void RenderSpriteText(struct S *p, int a, int b, int c) __asm__("RenderSpriteText");
void RenderSpriteGeometry(struct S *p, int a, int b, int c) __asm__("RenderSpriteGeometry");
void RenderSpritePicture(struct S *p, int a, int b, int c) __asm__("RenderSpritePicture");
// Renders a queued sprite payload by kind: text, geometry or picture. (inferred)
extern "C" void RenderSpritePayload(struct S *sprite, int matrix, int params, int channel)
{
    switch (sprite->kind) {
    case SPRITE_KIND_TEXT:
        RenderSpriteText(sprite, matrix, params, channel);
        break;
    case SPRITE_KIND_GEOMETRY:
        RenderSpriteGeometry(sprite, matrix, params, channel);
        break;
    case SPRITE_KIND_EFFECT:
    case SPRITE_KIND_PICTURE:
        RenderSpritePicture(sprite, matrix, params, channel);
        break;
    case SPRITE_KIND_POLYGON:
    case SPRITE_KIND_NESTED:
    case SPRITE_KIND_UNUSED5:
    case SPRITE_KIND_TABLE:
        break;
    }
}
} // namespace M_func_002159E0

// 0x215A50 GetGlyphIndex__8CFontBrxc
namespace M_IglyffFromCh__8CFontBrxc {
struct CFontBrx { char pad[0x20]; int numGlyphs; unsigned int *glyphChars; };   /* glyphChars sorted, for the binary search */
// Glyph index of a character in a font (0 when the font has no glyph for it, inferred).
// (Sly 1-derived name: CFontBrx::PglyffFromCh.)
// Also known as: PglyffFromCh (upstream readable / Sly 1 name).
extern "C" int GetGlyphIndex__8CFontBrxc(struct CFontBrx *p, unsigned char ch)
{
    int lo = 0;
    int hi = p->numGlyphs - 1;
    while (lo <= hi) {
        int mid = (hi + lo) / 2;
        int d = ch - p->glyphChars[mid];
        if (d == 0)
            return mid + 1;
        if (d > 0)
            lo = mid + 1;
        else
            hi = mid - 1;
    }
    return 0;
}
} // namespace M_IglyffFromCh__8CFontBrxc

// 0x215AC0 CRichText_ctor
namespace M_CRichText_ctor {
// Packed color word.
struct RGBA { unsigned int u; RGBA() {} RGBA(unsigned int x) { u = x; } };
struct Q { int a[4]; } __attribute__((aligned(16)));
extern Q D_002A2E10 __asm__("D_002A2E10");
extern Q D_002A2E20 __asm__("D_002A2E20");
/* CRichText2 (0x98 bytes; the field names are the same in every view of this file):
   text, cursor, current / base / alternate font, current / base / alternate colour, the two float colour vectors
   the glyphs draw with and their base values, a style-change flag, the screen origin, the box (offset, size,
   scale), line height, font scale, alignment (0 top/left, 1 centre, 2 bottom/right) and render priority. */
struct S { int text; int cur; char *font; char *baseFont; char *altFont; int color; RGBA baseColor; char pad1c[0x24]; Q baseColor0; Q baseColor1;
  char pad60[4]; float originX; float originY; int offsetX; int offsetY; int width; int height; float scaleX; float scaleY; float lineHeight; float fontScale;
  int vAlign; int hAlign; int priority; };
extern void ResetRichText2(S *p, int i) __asm__("ResetRichText2");
// Rich-text constructor from a string, a font and a scale.
extern "C" S *CRichText_ctor(S *rt, int text, float fontScale)
{
    rt->text = text;
    rt->scaleX = 1.0f;
    rt->scaleY = 1.0f;
    rt->originX = 1792.0f;   /* GS drawing area origin 2048 minus 256 / 112 */
    rt->originY = 1936.0f;
    rt->fontScale = fontScale;
    rt->offsetX = 0;
    rt->offsetY = 0;
    rt->width = 0;
    rt->height = 0;
    rt->lineHeight = fontScale * 51.2f;   /* 51.2 units per line at scale 1 */
    rt->hAlign = 1;
    rt->vAlign = 1;
    rt->priority = 6;
    rt->font = rt->baseFont = (char *)((WIDGET *)g_sw.ploHudFont)->sprites;
    rt->altFont = (char *)(((WIDGET *)g_sw.ploHudFont)->sprites + 1);
    rt->baseColor = RGBA(0x80808080);
    rt->baseColor0 = D_002A2E10;
    rt->baseColor1 = D_002A2E20;
    ResetRichText2(rt, 0);
    return rt;
}
} // namespace M_CRichText_ctor

// 0x215BA8 ConstructRichText2FromSprite
namespace M_func_00215BA8 {
struct Q { int a[4]; } __attribute__((aligned(16)));
struct Q3 { Q a, b, c; };
/* a text sprite descriptor (RenderSpriteText's S) */
struct R { char pad[8]; WIDGET *owner; char padc[0x10]; int color; float scale; float width; float height; int hAlign; int vAlign; };
struct S { int text; int cur; char *font; char *baseFont; char *altFont; int color; int baseColor; char pad1c[0x24]; Q baseColor0; Q baseColor1;
  char pad60[4]; float originX; float originY; int offsetX; int offsetY; float width; float height; float scaleX; float scaleY; float lineHeight; float fontScale;
  int vAlign; int hAlign; int priority; };
extern void ResetRichText2(S *p, int i) __asm__("ResetRichText2");
// Rich-text constructor from a sprite text element (width, height, colour). (inferred)
extern "C" S *ConstructRichText2FromSprite(S *rt, int text, R *sprite, Q3 *style)
{
    Q3 styleCopy = *style;
    rt->text = text;
    rt->offsetX = 0;
    rt->offsetY = 0;
    rt->width = sprite->width;
    rt->height = sprite->height;
    rt->scaleX = 1.0f;
    rt->scaleY = 1.0f;
    rt->hAlign = sprite->hAlign;
    rt->vAlign = sprite->vAlign;
    rt->originX = sprite->owner->ownerWidget->screenX;
    rt->originY = sprite->owner->ownerWidget->screenY;
    rt->priority = sprite->owner->priority;
    rt->lineHeight = sprite->scale;
    rt->fontScale = rt->lineHeight * 0.01953125f;   /* 1 / 51.2 */
    rt->font = rt->baseFont = (char *)((WIDGET *)g_sw.ploHudFont)->sprites;
    rt->altFont = (char *)(((WIDGET *)g_sw.ploHudFont)->sprites + 1);
    rt->baseColor = sprite->color;
    rt->baseColor0 = styleCopy.b;
    rt->baseColor1 = styleCopy.c;
    ResetRichText2(rt, 0);
    return rt;
}
} // namespace M_func_00215BA8

// 0x215CA8 ResetRichText2
namespace M_func_00215CA8 {
void UpdateRichText2Color(CRichText2 *p) __asm__("UpdateRichText2Color");
// Resets the rich-text cursor: optional new text, start of text, base font and colour. (inferred)
// Same role as Sly 1's CRichText::Reset, but this HUD rich text is a second class (Reset__9CRichText is 0x15F970).
extern "C" void ResetRichText2(CRichText2 *rt, uchar *text)
{
    if (text)
        rt->text = text;
    rt->cursor = rt->text;
    rt->font = rt->baseFont;
    rt->color = rt->baseColor;
    *(uint *)&rt->altColor = 0x80000000;               /* black ... */
    rt->altColor.alpha = rt->color.alpha;              /* ... at the current alpha */
    UpdateRichText2Color(rt);
}
} // namespace M_func_00215CA8

// 0x215D00 SetRichText2BaseColor
namespace M_func_00215D00 {
void UpdateRichText2Color(CRichText2 *p) __asm__("UpdateRichText2Color");
// Sets the rich text's base and current colour. (inferred)
extern "C" void SetRichText2BaseColor(CRichText2 *rt, struct RGBA *color)
{
    rt->baseColor = *color;
    rt->color = rt->baseColor;
    UpdateRichText2Color(rt);
}
} // namespace M_func_00215D00

// 0x215D40 GetRichText2NextChar
namespace M_func_00215D40 {
void UpdateRichText2Color(CRichText2 *p) __asm__("UpdateRichText2Color");
// Returns the next printable character, processing '&' escape codes (colour, font, glyph changes) on the way.
// (inferred)
extern "C" unsigned char GetRichText2NextChar(CRichText2 *rt)
{
    int isDone = 0;
    rt->changed = 0;
    while (!isDone) {
        unsigned char *pch = rt->cursor;
        unsigned char c = *pch;
        if (c == '&') {
            if (pch[1] == c) {            /* "&&": a literal '&' */
                rt->cursor = pch + 1;
                isDone = 1;
            } else if (pch[1] == '.') {   /* "&.": back to the base font and colour */
                rt->cursor = pch + 2;
                rt->font = rt->baseFont;
                rt->color = rt->baseColor;
                rt->changed = 1;
                UpdateRichText2Color(rt);
            } else {                      /* "&x": the alternate font and colour */
                rt->cursor = pch + 2;
                rt->font = rt->altFont;
                rt->color = rt->altColor;
                rt->changed = 1;
                UpdateRichText2Color(rt);
            }
        } else {
            isDone = 1;
        }
    }
    return *rt->cursor++;
}
} // namespace M_func_00215D40

// 0x215E30 CRichText_CountChars
namespace M_CRichText_Cch {
void ResetRichText2(void *p, int a) __asm__("ResetRichText2");
int GetRichText2NextChar(void *p) __asm__("GetRichText2NextChar");
// Number of printable characters in the rich text.
extern "C" int CRichText_CountChars(void *rt)
{
    int count = 0;
    ResetRichText2(rt, 0);
    while (GetRichText2NextChar(rt))
        count++;
    return count;
}
} // namespace M_CRichText_Cch

// 0x215E80 CRichText_Trim
namespace M_CRichText_Trim {
struct S { int text; char *cur; };
void ResetRichText2(struct S *p, int a) __asm__("ResetRichText2");
int GetRichText2NextChar(struct S *p) __asm__("GetRichText2NextChar");
// Truncates the rich text after n printable characters.
extern "C" void CRichText_Trim(struct S *rt, int n)
{
    ResetRichText2(rt, 0);
    while (GetRichText2NextChar(rt) && n != 0)
        n--;
    *rt->cur = 0;
}
} // namespace M_CRichText_Trim

// 0x215ED8 CRichText_GetWidth
namespace M_CRichText_Dx {
struct S { char pad[8]; void *font; char pad2[0x7C]; float fontScale; };
void ResetRichText2(struct S *p, int a) __asm__("ResetRichText2");
int GetRichText2NextChar(struct S *p) __asm__("GetRichText2NextChar");
float GetFontGlyphWidth2(void *p, int ch) __asm__("GetFontGlyphWidth2");
// Width of the rich text (glyph widths times scale).
// Also known as: DxTextUi (upstream readable / Sly 1 name; DxTextUi__FP7FONTBRX).
extern "C" float CRichText_GetWidth(struct S *p)
{
    float sum = 0.0f;
    int ch;
    ResetRichText2(p, 0);
    while ((ch = GetRichText2NextChar(p)) != 0)
        sum += GetFontGlyphWidth2(p->font, ch) * p->fontScale;
    return sum;
}
} // namespace M_CRichText_Dx

// 0x215F40 WrapRichText2Lines
namespace M_func_00215F40 {

struct S { unsigned char *text; unsigned char *cur; void *font; char pad[0x74-0xC]; float width; char pad1[0x88-0x78]; float fontScale; };
extern unsigned char D_002AA6A1[] __asm__("_ctype_+1");
void ResetRichText2(struct S *,unsigned char *) __asm__("ResetRichText2");
unsigned char GetRichText2NextChar(struct S *) __asm__("GetRichText2NextChar");
float GetFontGlyphWidth2(void *,unsigned char) __asm__("GetFontGlyphWidth2");
// The newline character (10) materialised in a register.
extern "C" { static inline int NewlineValue(){int value;__asm__("addiu %0,$zero,10":"=r"(value));return value;} }
// Store one byte.
extern "C" { static inline void StoreByte(unsigned char *p,int c){*p=c;} }
// Counts lines, inserting line breaks where a line would exceed the box width. (inferred)
// Also known as: StrWrapped (upstream readable / Sly 1 name; StrWrapped__FP7FONTBRX).
extern "C" int WrapRichText2Lines(struct S *p) {
    float width=0.0f;
    int lines=1;
    unsigned char c;
    int newline=NewlineValue();
    ResetRichText2(p,0);
    while ((c=GetRichText2NextChar(p))!=0) {
        if(c=='\n') { width=0.0f; lines++; }
        else {
            width+=GetFontGlyphWidth2(p->font,c)*p->fontScale;
        }
        if(p->width!=0.0f && p->width<width) {
                unsigned char *q=p->cur-1;
                while(q>=p->text && *q!='\n') {
                    if((*q==' ' || *q=='\t') && !(D_002AA6A1[q[1]]&0x10)) {
                        p->cur=q+1;
                        StoreByte(q,newline);
                        lines++;
                        width=0.0f;
                        break;
                    }
                    q--;
                }
                if(width!=0.0f && *p->cur) {
                    StoreByte(p->cur,newline); lines++; width=0.0f;
                }
            }

    }
    return lines;
}


} // namespace M_func_00215F40

// 0x2160D8 UpdateRichText2Color
namespace M_func_002160D8 {
struct S {
    char pad[0x14]; struct RGBA color; struct RGBA baseColor;
    char pad1c[4];
    /* the two vectors the glyphs draw with (an offset and a scale, as RenderWidget's PARAM) and their bases */
    float color0R, color0G, color0B, color0W, color1R, color1G, color1B, color1A;
    float baseColor0R, baseColor0G, baseColor0B, baseColor0W, baseColor1R, baseColor1G, baseColor1B, baseColor1A;
};
// Converts the current colour to clamped float colours for drawing. (inferred)
extern "C" void UpdateRichText2Color(struct S *rt)
{
    float alphaScale = rt->baseColor1A * 0.007874016f;   /* 1 / 127 */
    float alpha;
    alpha = alphaScale * (float)rt->color.alpha;
    rt->color0R = StdMin(rt->baseColor0R + (float)rt->color.red, 255.0f);
    rt->color0G = StdMin(rt->baseColor0G + (float)rt->color.green, 255.0f);
    rt->color0B = StdMin(rt->baseColor0B + (float)rt->color.blue, 255.0f);
    rt->color0W = rt->baseColor0W;
    rt->color1R = rt->baseColor1R;
    rt->color1G = rt->baseColor1G;
    rt->color1B = rt->baseColor1B;
    rt->color1A = Clamp(alpha, 0.0f, 1.0f);
}
} // namespace M_func_002160D8

// 0x2161C8 GetRichText2MaxLineWidth
namespace M_func_002161C8 {
struct S { char pad[8]; void *font; char padc[0x7C]; float fontScale; };
void ResetRichText2(struct S *p, int i) __asm__("ResetRichText2");
int GetRichText2NextChar(struct S *p) __asm__("GetRichText2NextChar");
float GetFontGlyphWidth2(void *p, int ch) __asm__("GetFontGlyphWidth2");
// Width of the widest line. (inferred)
extern "C" float GetRichText2MaxLineWidth(struct S *rt)
{
    float maxWidth = 0.0f;
    float lineWidth = maxWidth;
    int ch;
    ResetRichText2(rt, 0);
    while ((ch = GetRichText2NextChar(rt)) != 0) {
        if (ch == '\n') {
            maxWidth = (maxWidth > lineWidth) ? maxWidth : lineWidth;
            lineWidth = 0.0f;
        } else {
            lineWidth += GetFontGlyphWidth2(rt->font, ch) * rt->fontScale;
        }
    }
    maxWidth = (maxWidth > lineWidth) ? maxWidth : lineWidth;
    return maxWidth;
}
} // namespace M_func_002161C8

// 0x216260 CRichText_GetExtents
namespace M_CRichText_GetExtents {
struct S { char pad[0x84]; float lineHeight; };
int WrapRichText2Lines(struct S *p) __asm__("WrapRichText2Lines");
float GetRichText2MaxLineWidth(struct S *p) __asm__("GetRichText2MaxLineWidth");
// Width (widest line) and height (lines x line height) of the rich text.
extern "C" void CRichText_GetExtents(struct S *rt, float *width, float *height)
{
    int numLines = WrapRichText2Lines(rt);
    float maxWidth = GetRichText2MaxLineWidth(rt);
    float totalHeight = numLines * rt->lineHeight;
    if (width)
        *width = maxWidth;
    if (height)
        *height = totalHeight;
}
} // namespace M_CRichText_GetExtents

// 0x2162D8 BuildRichTextMatrix
namespace M_func_002162D8 {
struct S { char pad[0x64]; float originX; float originY; };
void *func_002333B4(void *p, int c, int n) __asm__("memset");
// Builds a glyph matrix from a 2D transform (pixel aspect scales and the GS screen offset). (inferred)
extern "C" void BuildRichTextMatrix(struct S *rt, float *m, float *xf)
{
    /* HUD units to GS pixels: x * 0.8 (640 -> 512), y / 2.2; 786432 (3 * 2^18) is the float-to-fixed bias */
    func_002333B4(m, 0, 0x40);
    m[0] = xf[0] * 0.8f;
    m[1] = xf[1] * 0.454545454545f;
    m[4] = xf[2] * 0.8f;
    m[5] = xf[3] * 0.454545454545f;
    m[8] = xf[4] * 0.8f + rt->originX + 786432.0f;
    m[9] = xf[5] * 0.454545454545f + rt->originY + 786432.0f;
    m[10] = 1.0f;
    m[15] = 1.0f;
}
} // namespace M_func_002162D8

// 0x2163B0 DrawRichText2
namespace M_func_002163B0 {
// Configure rich-text rendering and queue the standard render payloads.
typedef unsigned int uint;
typedef unsigned long ulong;
typedef int qword __attribute__((mode(TI)));
typedef struct {unsigned char r,g,b,a;} RGBA;
struct RT {char pad[0x6C];float offsetX,offsetY,width,height,scaleX,scaleY,lineHeight,fontScale;int vAlign,hAlign,priority;};
struct CONFIG {float offsetX,offsetY,width,height;RGBA color;int hAlign,vAlign;};
extern void *func_002333B4(void *,int,int) __asm__("memset");
// 2x3 affine matrix (padded to 8 floats), optionally zeroed.
struct M23 {float a[8];enum INIT {Uninitialized,Zero};M23(INIT init=Uninitialized){if(init==Zero)func_002333B4(this,0,sizeof(*this));}} __attribute__((aligned(16)));
struct DATA {RGBA color;int mode;float priority;int limit;qword v10,v20;};
extern VIFS D_002DEA28 __asm__("D_002DEA28");
extern char D_00442210[] __asm__("D_00442210"),D_002DF010[] __asm__("D_002DF010");
extern void CopyAqw__FPvT0i(void *,void *,int) __asm__("CopyAqw__FPvT0i");
extern RPL *AllocRenderListEntry(void) __asm__("AllocRenderListEntry");
// Append one word to the packet.
static inline void Word(VIFS *r,uint word){*(uint *)r->m_pb=word;r->m_pb+=4;}
// Pad the packet with zero words until the cursor is at word `word` of a quadword.
static inline void Align(VIFS *r,int word){while(((uint)r->m_pb&15)!=4*word)Word(r,0);}
// Append n quadwords from source to the packet.
static inline void Bulk(VIFS *r,int n,void *source){CopyAqw__FPvT0i(r->m_pb,source,n);r->m_pb+=n*16;}
// Append a VIF DIRECT code for n quadwords followed by the data.
static inline void Direct(VIFS *r,int n,void *source){Align(r,3);Word(r,0x50000000|n);Bulk(r,n,source);}

extern void SetRichText2BaseColor(struct RT *,RGBA *) __asm__("SetRichText2BaseColor");
extern void BuildRichTextMatrix(struct RT *,struct MATRIX4Q *,struct M23 *) __asm__("BuildRichTextMatrix");
extern void DrawRichText2Glyphs(struct RT *,struct MATRIX4Q *,struct DATA *) __asm__("DrawRichText2Glyphs");
extern qword D_002A2E10 __asm__("D_002A2E10"),D_002A2E20 __asm__("D_002A2E20");
// Configures box, alignment and colour and queues the rich text for drawing. (inferred)
extern "C" void DrawRichText2(struct RT *p,struct CONFIG *config,int priority){
    RGBA color;
    unsigned char *tag;
    RPL *e;
    if(!config)return;
    p->offsetX=config->offsetX;p->offsetY=config->offsetY;p->width=config->width;p->height=config->height;
    p->hAlign=config->hAlign;p->vAlign=config->vAlign;p->priority=priority;
    color=config->color;SetRichText2BaseColor(p,&color);
    struct M23 matrix(M23::Zero);
    struct MATRIX4Q world;
    struct DATA style;
    matrix.a[0]=p->scaleX*p->fontScale;
    matrix.a[3]=p->scaleY*p->fontScale;
    matrix.a[4]=p->offsetX;
    matrix.a[5]=p->offsetY+p->lineHeight*p->scaleY;
    BuildRichTextMatrix(p,&world,&matrix);
    style.v10=D_002A2E10;style.v20=D_002A2E20;style.priority=16.0f;
    DrawRichText2Glyphs(p,&world,&style);
    tag=D_002DEA28.m_pb;
    *(ulong *)tag=0x10000000UL;D_002DEA28.m_pb+=16;
    Word(&D_002DEA28,0x11000000);
    Direct(&D_002DEA28,3,D_00442210);
    Direct(&D_002DEA28,0x25,D_002DF010);
    Word(&D_002DEA28,0x11000000);
    Align(&D_002DEA28,0);
    *(uint *)tag|=((D_002DEA28.m_pb-tag)>>4)-1;
    *(ulong *)D_002DEA28.m_pb=0x60000000UL;D_002DEA28.m_pb+=16;
    e=AllocRenderListEntry();
    e->kind=0x15;e->depth=p->priority<<18;
    e->pbCall=(unsigned char *)((uint)tag&0x0FFFFFFF);e->pvCall=0;
}
} // namespace M_func_002163B0

// 0x2166B0 DrawRichText2AtXf
namespace M_func_002166B0 {
typedef int qword __attribute__((mode(TI)));
typedef struct { float a[6]; } __attribute__((aligned(16))) T;
typedef struct { float a[16]; } __attribute__((aligned(16))) O;
struct RT {char pad[8];void *font;char padC[0x14];qword color0,color1;char pad40[0x20];int changed;float originX,originY,offsetX,offsetY,width,height,scaleX,scaleY,lineHeight,fontScale;int vAlign,hAlign,priority;};
struct DATA {unsigned int color;int mode;float priority;unsigned int limit;qword color0,color1;};

void BuildRichTextMatrix(struct RT *p, O *po, T *pt) __asm__("BuildRichTextMatrix");
void DrawRichText2Glyphs(struct RT *p, O *po, struct DATA *q) __asm__("DrawRichText2Glyphs");
// Store the x scale (register-pinned to $f2).
extern "C" { static inline void StoreX(struct RT *p,float value){register float x asm("$f2")=value;asm("swc1 %1,%0":"=m"(p->scaleX):"f"(x));} }
// Draws rich text at a sprite transform (font scale, line height). (inferred)
extern "C" void DrawRichText2AtXf(struct RT *p, T *pt, struct DATA *q)
{
    T t = *pt;
    O o;
    float x=t.a[0],y=t.a[3],fontScale=p->fontScale,shift=p->lineHeight;
    p->scaleY=y; StoreX(p,x);
    t.a[0] = x*fontScale;
    t.a[3] = y*fontScale;
    t.a[5] += shift*y;
    BuildRichTextMatrix(p, &o, &t);
    DrawRichText2Glyphs(p, &o, q);
}
} // namespace M_func_002166B0

// 0x216750 DrawRichText2Glyphs
namespace M_func_00216750 {
// Align, clip, and queue rich-text glyphs with their current colors.
typedef unsigned int uint;
typedef unsigned long ulong;
typedef int qword __attribute__((mode(TI)));
struct RT {char pad[8];void *font;char padc[0x14];qword color0,color1;char pad40[0x20];int changed;float originX,originY,offsetX,offsetY,width,height,scaleX,scaleY,lineHeight,fontScale;int vAlign,hAlign,priority;};
union M4 {qword r[4];float a[16];};
struct DATA {uint color;int mode;float priority;uint limit;qword color0,color1;};
struct GLYPH {int a;void *data;char pad8[0xA];unsigned short mode;char pad14[8];void *address;char pad20[0x20];};
extern VIFS D_002DEA28 __asm__("D_002DEA28");
extern float D_002C40A8 __asm__("D_002C40A8"),D_002C40AC __asm__("D_002C40AC"),D_002C40B0 __asm__("D_002C40B0"),D_002C40B4 __asm__("D_002C40B4");
void *func_002333B4(void *,int,int) __asm__("memset");
void ResetRichText2(struct RT *,char *) __asm__("ResetRichText2");
int GetRichText2NextChar(struct RT *) __asm__("GetRichText2NextChar");
float GetFontGlyphWidth2(void *,int) __asm__("GetFontGlyphWidth2");
struct GLYPH *GetGlyph__8CFontBrxc(void *,int) __asm__("GetGlyph__8CFontBrxc");
void BuildGlyphPacket(struct GLYPH *,union M4 *,struct DATA *,int) __asm__("BuildGlyphPacket");
RPL *AllocRenderListEntry(void) __asm__("AllocRenderListEntry");
// Append one word to the packet.
extern "C" { static inline void Word(VIFS *r,uint word){*(uint *)r->m_pb=word;r->m_pb+=4;} }
// Pad the packet with zero words until the cursor is at word `word` of a quadword.
extern "C" { static inline void Align(VIFS *r,int word){while(((uint)r->m_pb&15)!=4*word)Word(r,0);} }
// True when the matrix translation (a[8], a[9]) lies inside the bounds [*left, D_002C40AC] x [D_002C40B0, D_002C40B4].
extern "C" { static inline int Visible(union M4 *matrix,const float *left){int visible=0;if(*left<=matrix->a[8] && matrix->a[8]<=D_002C40AC && D_002C40B0<=matrix->a[9] && matrix->a[9]<=D_002C40B4)visible=1;return visible;} }
// Aligns, clips and queues the glyphs with their colours. (inferred)
extern "C" void DrawRichText2Glyphs(struct RT *p,union M4 *matrix,struct DATA *style){
    float widths[32];
    unsigned char *tag=D_002DEA28.m_pb;
    int line=0;
    float lineHeight=p->lineHeight*p->scaleY*0.45454547f;
    float startX=matrix->a[8];
    style->color=0;
    func_002333B4(widths,0,sizeof(widths));
    ResetRichText2(p,0);
    {int ch;
    while((ch=GetRichText2NextChar(p))!=0){
        if(ch=='\n')line++;
        else widths[line]+=GetFontGlyphWidth2(p->font,ch)*p->fontScale;
    }
    }
    if(p->vAlign){
        float extra=p->height*p->scaleY*0.45454547f-(line+1)*lineHeight;
        if(p->vAlign==2)matrix->a[9]+=extra;
        else matrix->a[9]+=extra*0.5f;
    }
    ResetRichText2(p,0);
    {
        int flags=3,newLine=1,drawCount=0,until=2,ch;
        float *width=widths;
        while((ch=GetRichText2NextChar(p))!=0){
            if(p->changed){until=drawCount+3;flags|=2;}
            if(ch=='\n'){
                matrix->a[9]+=lineHeight;newLine=1;width++;
            }else{
                struct GLYPH *glyph;
                if(newLine){
                    if(!p->hAlign)matrix->a[8]=startX;
                    else{
                        float lineWidth=*width*p->scaleX*0.8f;
                        float extent=p->width*p->scaleX*0.8f;
                        if(p->hAlign==2)matrix->a[8]=startX+(extent-lineWidth);
                        else matrix->a[8]=startX+(extent-lineWidth)*0.5f;
                    }
                    newLine=0;
                }
                glyph=GetGlyph__8CFontBrxc(p->font,ch);
                if(glyph){
                    int visible=Visible(matrix,&D_002C40A8);
                    if(glyph->data && visible){
                        if(flags&2){style->color0=p->color0;style->color1=p->color1;}
                        BuildGlyphPacket(glyph,matrix,style,flags);
                        drawCount++;
                        if(drawCount>=until)flags=0;
                    }
                    matrix->a[8]+=GetFontGlyphWidth2(p->font,ch)*p->fontScale*p->scaleX*0.8f;
                }
            }
        }
    }
    Align(&D_002DEA28,0);
    *(ulong *)D_002DEA28.m_pb=0x60000000UL;D_002DEA28.m_pb+=16;
    {
        RPL *e=AllocRenderListEntry();
        e->kind=0x15;e->depth=style->priority+(p->priority<<18);e->pbCall=(unsigned char *)((uint)tag&0x0FFFFFFF);e->pvCall=0;
    }
}
} // namespace M_func_00216750

// 0x216B40 GetGlyph__8CFontBrxc
namespace M_PglyffFromCh__8CFontBrxc {
struct G { char pad[0x40]; };
struct S { char pad[0x14]; struct G *glyphs; };
int GetGlyphIndex__8CFontBrxc(struct S *p, unsigned char ch) __asm__("GetGlyphIndex__8CFontBrxc");
// Returns the glyph record for a character, or null when the font has none (Sly 1's
// CFontBrx::FindGlyphForChar contract).
extern "C" struct G *GetGlyph__8CFontBrxc(struct S *p, unsigned char ch)
{
    int i = GetGlyphIndex__8CFontBrxc(p, ch);
    return i ? &p->glyphs[i] : 0;
}
} // namespace M_PglyffFromCh__8CFontBrxc

// 0x216B80 GetFontGlyphWidth2
namespace M_func_00216B80 {
struct S { char pad[0x1C]; float *glyphWidths; };
int GetGlyphIndex__8CFontBrxc(struct S *p, unsigned char ch) __asm__("GetGlyphIndex__8CFontBrxc");
// Width of a character's glyph in the font. (inferred)
extern "C" float GetFontGlyphWidth2(struct S *p, unsigned char ch)
{
    int i = GetGlyphIndex__8CFontBrxc(p, ch);
    return i ? p->glyphWidths[i - 1] : 0.0f;
}
} // namespace M_func_00216B80

// 0x216BC8 RenderSpriteText
namespace M_RenderSpriteText {
// Render generated sprite text, wrapped speech, or the scrolling ticker.
typedef unsigned int uint;
typedef int qword __attribute__((mode(TI)));
typedef struct {unsigned char r,g,b,a;} RGBA;
typedef struct {float a[8];} __attribute__((aligned(16))) T;
typedef union {qword q[3];struct {RGBA color;int mode;float priority;uint limit;char pad10[0x1C];float fade;};} DATA;
typedef struct {int pad[2];void *owner;char padc[8];int kind,id;int color;float scale,width,height;int hAlign,vAlign;} S;   /* kind: 0 language string, 1 text table */
typedef struct {char *text,*cur;void *font,*baseFont,*altFont;RGBA color,baseColor,altColor;char pad20[0x20];qword baseColor0,baseColor1;int changed;float originX,originY,offsetX,offsetY,width,height,scaleX,scaleY,lineHeight,fontScale;int vAlign,hAlign,priority;} RT;
typedef struct {int id;void (*get)(char *,void *,void *);void *context;} STRINGGEN;
typedef struct {char pad10[0x10];int state;float time;int pad18;float finish;} SCROLL;
extern STRINGGEN D_003E1C88[] __asm__("D_003E1C88");
extern const char D_002A8B80[] __asm__("D_002A8B80"),D_002A8B88[] __asm__("D_002A8B88");
extern char D_004435F0[] __asm__("D_004435F0");
#define D_004435F4 (*(RGBA *)((char *)base_D_004435F0 + 4))   /* g_subtitle's colour (subtitle's SPEAKERLABEL +0x4) */
extern SCROLL D_003DA0C0 __asm__("D_003DA0C0");
extern const float D_002C40B8 __asm__("D_002C40B8");
extern char *strcpy(char *,const char *) __asm__("strcpy");
extern char *strcpy1(char *,const char *) __asm__("strcpy1");
extern RT *ConstructRichText2FromSprite(RT *,char *,S *,DATA *) __asm__("ConstructRichText2FromSprite");
extern int WrapRichText2Lines(RT *) __asm__("WrapRichText2Lines");
extern void TrimSubtitleToVoiceProgress(void *,RT *) __asm__("TrimSubtitleToVoiceProgress");
extern void ResetRichText2(RT *,char *) __asm__("ResetRichText2");
extern int GetRichText2NextChar(RT *) __asm__("GetRichText2NextChar");
extern void SetRichText2BaseColor(RT *,RGBA *) __asm__("SetRichText2BaseColor");
extern float CRichText_GetWidth(RT *) __asm__("CRichText_GetWidth");
extern float ModPositive__Fff(float,float) __asm__("ModPositive__Fff");
extern void SetNoticeState(SCROLL *,int) __asm__("SetNoticeState");
extern void DrawRichText2AtXf(RT *,T *,DATA *) __asm__("DrawRichText2AtXf");
enum { TEXT_SOURCE_LANGSTR = 0, TEXT_SOURCE_TABLE = 1 };
enum { TEXT_ID_SUBTITLE = 0x2EF, TEXT_ID_TICKER = 0x2F1 };   /* BuildSubtitleText, BuildTickerText */

// Renders a text sprite: a text-table entry or a language string, with special cases for subtitles and the ticker.
extern "C" void RenderSpriteText(S *p,T *t,DATA *data,int unused){
    char text[0x400];
    RT rt;
    DATA style;
    char marquee[2*sizeof(text)+3*5];
    if(!g_sw.ploHudFont)return;
    text[0]=0;
    if(p->kind==TEXT_SOURCE_TABLE){
        uint i;
        for(i=0;i<108;i++)if(D_003E1C88[i].id==p->id)break;
        if(i>=108)*(long *)text=*(const long *)D_002A8B80;
        else D_003E1C88[i].get(text,D_003E1C88[i].context,p->owner);
    }else if(p->kind==TEXT_SOURCE_LANGSTR){
        strcpy(text,(const char *)FindSwLocalizedString(&g_sw,p->id));
    }
    if(p->kind==TEXT_SOURCE_TABLE && p->id==TEXT_ID_SUBTITLE){
        int lines=1,ch,visible;
        unsigned char *first;
        style=*data;
        ConstructRichText2FromSprite(&rt,text,p,&style);
        WrapRichText2Lines(&rt);
        TrimSubtitleToVoiceProgress(D_004435F0,&rt);
        ResetRichText2(&rt,0);
        while((ch=GetRichText2NextChar(&rt))!=0)if(ch=='\n')lines++;
        visible=(int)(p->height/p->scale);
        first=(unsigned char *)rt.text;
        while(visible<lines && *first){
            while(*first && *first!='\n')first++;
            lines--;
            if(*first)first++;
        }
        ResetRichText2(&rt,(char *)first);
        style.color=D_004435F4;
        SetRichText2BaseColor(&rt,&style.color);
        DrawRichText2AtXf(&rt,t,data);
    }else if(p->kind==TEXT_SOURCE_TABLE && p->id==TEXT_ID_TICKER && D_003DA0C0.state==2){
        float scale,paddingWidth,textWidth,elapsed,period,position;
        style=*data;
        ConstructRichText2FromSprite(&rt,text,p,&style);
        strcpy(marquee,D_002A8B88);
        ResetRichText2(&rt,marquee);
        scale=t->a[0];
        paddingWidth=CRichText_GetWidth(&rt)*scale;
        strcpy1(marquee,text);
        ResetRichText2(&rt,marquee);
        textWidth=CRichText_GetWidth(&rt)*scale;
        strcpy1(marquee,D_002A8B88);
        strcpy1(marquee,text);
        strcpy1(marquee,D_002A8B88);
        elapsed=g_clock.time-D_003DA0C0.time;
        if(elapsed<D_002C40B8){
            data->fade*=elapsed/D_002C40B8;
        }else{
            elapsed-=D_002C40B8;
            period=textWidth*0.01f;
            position=ModPositive__Fff(elapsed,period);
            if(D_003DA0C0.finish>=0.0f){
                float finish=StdMax(D_003DA0C0.finish,(textWidth+StdMax(0.0f,textWidth-p->width*scale)+paddingWidth)*0.01f);
                if(elapsed>=finish)SetNoticeState(&D_003DA0C0,3);
            }
            t->a[4]-=position/period*textWidth;
        }
        DrawRichText2AtXf(&rt,t,data);
    }else{
        style=*data;
        ConstructRichText2FromSprite(&rt,text,p,&style);
        WrapRichText2Lines(&rt);
        DrawRichText2AtXf(&rt,t,data);
    }
}
} // namespace M_RenderSpriteText

// 0x217058 BuildGlyphPacket
namespace M_func_00217058 {
// Build a matrix/parameter DMA packet ending in the supplied call tag.
struct S { char pad[0x12]; unsigned short mode; char pad1[8]; void *address; };
struct D { int a; int mode; int b; int xC; char pad[0x20]; };
extern VIFS D_002DEA28 __asm__("D_002DEA28");
// Copy n words: the n mod 4 leftovers first, then whole quadwords.
extern "C" { static inline void CopyWords(int *d,const int *s,int n){
    int rem=n&3,cBulk=n-rem,i;
    for(i=0;i<rem;i++)*d++=*s++;
    for(i=0;i<cBulk;i+=4){int a=s[0],b=s[1],c=s[2],e=s[3];s+=4;d[0]=a;d[1]=b;d[2]=c;d[3]=e;d+=4;}
} }
// Append one word to the packet.
extern "C" { static inline void Word(VIFS *r,unsigned int word) {
    *(unsigned int *)r->m_pb=word;
    r->m_pb+=4;
} }
// Pad the packet with zero words to a quadword boundary.
extern "C" { static inline void Align(VIFS *r) {
    while((int)r->m_pb&15) { *(int *)r->m_pb=0; r->m_pb+=4; }
} }
// Append a VIF code header and then n words from s.
extern "C" { static inline void Unpack(VIFS *r,const int *s,int n,unsigned int header){Word(r,header);CopyWords((int *)r->m_pb,s,n);r->m_pb+=n*4;} }
// Builds the matrix / parameter DMA packet for a glyph run, ending in a call tag. (inferred)
extern "C" void BuildGlyphPacket(struct S *p,const int *matrix,struct D *data,int flags) {
    unsigned char *tag=D_002DEA28.m_pb;
    *(unsigned long *)tag=0x10000000UL;
    D_002DEA28.m_pb+=16;
    if(flags&1) {
        Unpack(&D_002DEA28,matrix,16,0x6C04C000);
    } else {
        Unpack(&D_002DEA28,matrix+8,4,0x6C01C002);
    }
    data->xC=0x7FFF;
    if(p->mode>=2)data->mode=1;else data->mode=0;
    if((flags&2) || data->mode) {
        Unpack(&D_002DEA28,(const int *)data,12,0x6C03C004);
    }
    Align(&D_002DEA28);
    *(int *)tag |= ((D_002DEA28.m_pb-tag)>>4)-1;
    Align(&D_002DEA28);
    *(unsigned long *)D_002DEA28.m_pb=0x50000000UL | ((unsigned long)p->address<<32);
    D_002DEA28.m_pb+=16;
}
} // namespace M_func_00217058

// 0x217298 QueueSpriteVuProgram
namespace M_func_00217298 {
// Queue the selected VU microprogram pair in a DMA packet.
typedef unsigned int uint;
typedef unsigned long ulong;
extern VIFS D_002DEA28 __asm__("D_002DEA28");
extern char D_00000000[] __asm__("D_00000000"),D_00000030[] __asm__("D_00000030"),D_00000060[] __asm__("D_00000060"),D_00000090[] __asm__("D_00000090"),D_000000C0[] __asm__("D_000000C0"),D_00000100[] __asm__("D_00000100");
RPL *AllocRenderListEntry(void) __asm__("AllocRenderListEntry");
// Append one word to the packet.
extern "C" { static inline void Word(VIFS *r,uint word){*(uint *)r->m_pb=word;r->m_pb+=4;} }
// Append a VIF MSCAL code starting the VU1 microprogram at entry.
extern "C" { static inline void Mscal(VIFS *r,void *entry){Word(r,0x14000000|((uint)entry>>3));} }
// Pad the packet with zero words to a quadword boundary.
extern "C" { static inline void Align(VIFS *r){while((uint)r->m_pb&15){*(uint *)r->m_pb=0;r->m_pb+=4;}} }
// Queues the VU microprogram pair for a sprite render mode. (inferred)
extern "C" void QueueSpriteVuProgram(WIDGET *p,int mode,float priority){
    unsigned char *tag=D_002DEA28.m_pb;
    RPL *e;
    *(ulong *)tag=0x10000000UL;
    D_002DEA28.m_pb+=16;
    switch(mode){
    case 1:
        Word(&D_002DEA28,0x13000000);Mscal(&D_002DEA28,D_00000000);Mscal(&D_002DEA28,D_000000C0);Word(&D_002DEA28,0x13000000);break;
    case 2:
        Word(&D_002DEA28,0x13000000);Mscal(&D_002DEA28,D_00000030);Mscal(&D_002DEA28,D_000000C0);Word(&D_002DEA28,0x13000000);break;
    case 3:
        Word(&D_002DEA28,0x13000000);Mscal(&D_002DEA28,D_00000100);Mscal(&D_002DEA28,D_00000060);Word(&D_002DEA28,0x13000000);break;
    case 4:
        Word(&D_002DEA28,0x13000000);Mscal(&D_002DEA28,D_00000100);Mscal(&D_002DEA28,D_00000090);Word(&D_002DEA28,0x13000000);break;
    }
    Align(&D_002DEA28);
    *(uint *)tag|=((D_002DEA28.m_pb-tag)>>4)-1;
    *(ulong *)D_002DEA28.m_pb=0x60000000UL;
    D_002DEA28.m_pb+=16;
    e=AllocRenderListEntry();
    e->kind=0x15;e->depth=priority+(p->priority<<18);e->pbCall=(unsigned char *)((uint)tag&0x0FFFFFFF);e->pvCall=0;
}
} // namespace M_func_00217298

// 0x2174C8 RenderSpriteGeometry
namespace M_RenderSpriteGeometry {
// Queue transformed sprite parameters with optional fade limiting.
typedef unsigned int uint;
typedef unsigned long ulong;
struct S {char pad[8];WIDGET *owner;char padc[6];unsigned short mode;int count;char pad18[4];int tag;};
struct T {float a,b,c,d,x,y;};
struct M4 {float a[16];};
struct D {int a,mode;float priority;uint limit;char pad10[0x1C];float fade;};
extern VIFS D_002DEA28 __asm__("D_002DEA28");
void *func_002333B4(void *,int,int) __asm__("memset");
RPL *AllocRenderListEntry(void) __asm__("AllocRenderListEntry");
// Append one word to the packet.
extern "C" { static inline void Word(VIFS *r,uint word){*(uint *)r->m_pb=word;r->m_pb+=4;} }
// Pad the packet with zero words to a quadword boundary.
extern "C" { static inline void Align(VIFS *r){while((uint)r->m_pb&15){*(uint *)r->m_pb=0;r->m_pb+=4;}} }
// Copy n words: the n mod 4 leftovers first, then whole quadwords.
extern "C" { static inline void CopyWords(int *d,const int *s,int n){int rem=n&3,cBulk=n-rem,i;for(i=0;i<rem;i++)*d++=*s++;for(i=0;i<cBulk;i+=4){int a=s[0],b=s[1],c=s[2],e=s[3];s+=4;d[0]=a;d[1]=b;d[2]=c;d[3]=e;d+=4;}} }
// Append a VIF code header and then n words from s.
extern "C" { static inline void Unpack(VIFS *r,const int *s,int n,uint header){Word(r,header);CopyWords((int *)r->m_pb,s,n);r->m_pb+=n*4;} }
// Renders a geometry sprite with its transform and optional fade.
// Also known as: StateOfSlot (upstream readable / Sly 1 name; StateOfSlot__FPvP7VECTOR2T0i).
extern "C" void RenderSpriteGeometry(struct S *p,struct T *t,struct D *data,int fade){
    struct M4 m;
    unsigned char *tag;
    RPL *e;
    func_002333B4(&m,0,0x40);
    m.a[0]=t->a*0.8f;m.a[1]=t->b*0.45454547f;
    m.a[4]=t->c*0.8f;m.a[5]=t->d*0.45454547f;
    m.a[8]=t->x*0.8f+p->owner->ownerWidget->screenX+786432.0f;
    m.a[9]=t->y*0.45454547f+p->owner->ownerWidget->screenY+786432.0f;
    m.a[15]=m.a[10]=1.0f;
    if(p->mode>=2)data->mode=1;else data->mode=0;
    data->limit=0x7FFF;
    if(fade){
        float old=data->fade;
        data->fade=1.0f;
        if(old<1.0f){
            int n=MinInt((int)(p->count*old),p->count);
            data->limit=n>=3?n:3;
            if(data->limit>0x7FFFU)data->limit=0x7FFF;
        }
    }
    tag=D_002DEA28.m_pb;
    *(ulong *)tag=0x10000000UL;D_002DEA28.m_pb+=16;
    Unpack(&D_002DEA28,(const int *)&m,16,0x6C04C000);
    Unpack(&D_002DEA28,(const int *)data,12,0x6C03C004);
    Align(&D_002DEA28);
    *(uint *)tag|=((D_002DEA28.m_pb-tag)>>4)-1;
    *(ulong *)D_002DEA28.m_pb=0x60000000UL;D_002DEA28.m_pb+=16;
    e=AllocRenderListEntry();
    e->kind=0x15;e->depth=data->priority+(p->owner->priority<<18);
    e->pbCall=(unsigned char *)((uint)tag&0x0FFFFFFF);
    e->pvCall=(void *)p->tag;
}
} // namespace M_RenderSpriteGeometry

// 0x2177E0 RenderSpritePicture
namespace M_func_002177E0 {
// Queue transformed sprite parameters and a direct GIF payload.
typedef unsigned int uint;
typedef unsigned long ulong;
struct GEO {char pad[0x18];int count;void *data;};
struct S {int pad,type;WIDGET *owner;char padc[8];union {struct GEO *geo;int raw;};int rawtag;union {int tag;int count;};void *buffer;};
struct T {float a,b,c,d,x,y;};
struct M4 {float a[16];};
struct D {int a,mode;float priority;uint limit;char pad10[0x1C];float fade;};
extern VIFS D_002DEA28 __asm__("D_002DEA28");
void *func_002333B4(void *,int,int) __asm__("memset");
RPL *AllocRenderListEntry(void) __asm__("AllocRenderListEntry");
// Append one word to the packet.
extern "C" { static inline void Word(VIFS *r,uint word){*(uint *)r->m_pb=word;r->m_pb+=4;} }
// Pad the packet with zero words to a quadword boundary.
extern "C" { static inline void Align(VIFS *r){while((uint)r->m_pb&15){*(uint *)r->m_pb=0;r->m_pb+=4;}} }
// Copy n words: the n mod 4 leftovers first, then whole quadwords.
extern "C" { static inline void CopyWords(int *d,const int *s,int n){int rem=n&3,cBulk=n-rem,i;for(i=0;i<rem;i++)*d++=*s++;for(i=0;i<cBulk;i+=4){int a=s[0],b=s[1],c=s[2],e=s[3];s+=4;d[0]=a;d[1]=b;d[2]=c;d[3]=e;d+=4;}} }
// Append a VIF code header and then n words from s.
extern "C" { static inline void Unpack(VIFS *r,const int *s,int n,uint header){Word(r,header);CopyWords((int *)r->m_pb,s,n);r->m_pb+=n*4;} }

void CopyAqw__FPvT0i(void *,void *,int) __asm__("CopyAqw__FPvT0i");
// Pad the packet with zero words until the cursor is at word `word` of a quadword.
extern "C" { static inline void AlignTo(VIFS *r,int word){while(((uint)r->m_pb&15)!=4*word)Word(r,0);} }
// Append a VIF DIRECT code for n quadwords followed by the data.
extern "C" { static inline void Direct(VIFS *r,int n,void *source){AlignTo(r,3);Word(r,0x50000000|n);CopyAqw__FPvT0i(r->m_pb,source,n);r->m_pb+=n*16;} }
// Renders a picture sprite: transformed parameters plus a direct GIF payload. (inferred)
extern "C" void RenderSpritePicture(struct S *p,struct T *t,struct D *data,int unused){
    struct M4 m;
    unsigned char *tag;
    RPL *e;
    switch(p->type){
    case SPRITE_KIND_EFFECT:if(!p->geo)return;break;
    case SPRITE_KIND_PICTURE:if(!p->count)return;break;
    default:return;
    }
    func_002333B4(&m,0,0x40);
    m.a[0]=t->a*0.8f;m.a[1]=t->b*0.45454547f;
    m.a[4]=t->c*0.8f;m.a[5]=t->d*0.45454547f;
    m.a[8]=t->x*0.8f+p->owner->ownerWidget->screenX+786432.0f;
    m.a[9]=t->y*0.45454547f+p->owner->ownerWidget->screenY+786432.0f;
    m.a[15]=m.a[10]=1.0f;
    data->mode=0;data->limit=0x7FFF;
    tag=D_002DEA28.m_pb;
    *(ulong *)tag=0x10000000UL;D_002DEA28.m_pb+=16;
    Unpack(&D_002DEA28,(const int *)&m,16,0x6C04C000);
    Unpack(&D_002DEA28,(const int *)data,12,0x6C03C004);
    if(p->type==SPRITE_KIND_EFFECT)Direct(&D_002DEA28,p->geo->count,p->geo->data);
    else Direct(&D_002DEA28,p->count,p->buffer);
    Align(&D_002DEA28);
    *(uint *)tag|=((D_002DEA28.m_pb-tag)>>4)-1;
    *(ulong *)D_002DEA28.m_pb=0x60000000UL;D_002DEA28.m_pb+=16;
    e=AllocRenderListEntry();
    e->kind=0x15;e->depth=data->priority+(p->owner->priority<<18);
    e->pbCall=(unsigned char *)((uint)tag&0x0FFFFFFF);
    if(p->type==SPRITE_KIND_EFFECT)e->pvCall=(void *)p->tag;else e->pvCall=(void *)p->rawtag;
}
} // namespace M_func_002177E0

// 0x217BA0 StartSpriteAnimById
namespace M_func_00217BA0 {
void StartSpriteAnim(SPRITEGRP *group, int i) __asm__("StartSpriteAnim");
// 0x217B98: dead-code scrap (sly2/scrap.h)
extern "C" { SLY2_SCRAP("sd $zero, 0x10($4)"); }
} // namespace M_func_00217BA0
// Starts the sprite animation with the given id (0x314 = the first). (inferred)
extern "C" void StartSpriteAnimById(SPRITEGRP *group, int animId)
{
    using namespace M_func_00217BA0;
    int i;
    if (group->isEnabled) {
        i = 0;
        if (animId != 0x314) {
            for (i = 0; i < group->numAnims; i++)
                if (animId == group->anims[i].id) break;
            if (i >= group->numAnims) i = 0;
        }
        StartSpriteAnim(group, i);
    }
}

// 0x217C20 StartSpriteAnim
namespace M_func_00217C20 {
// Packed color word, quadword aligned.
struct RGBA { unsigned int u; RGBA(unsigned int x) { u = x; } } __attribute__((aligned(16)));
extern char D_002A3A10[] __asm__("D_002A3A10");
extern void StopSprite(SPRITEGRP *p, int i) __asm__("StopSprite");
extern void StartSpriteAnimAt(SPRITEGRP *p, int i, int z, void *q, float f, const RGBA &c0, const RGBA &c1) __asm__("StartSpriteAnimAt");
// Starts sprite animation i from time 0 with default colours (stopping it first if instanced). (inferred)
extern "C" void StartSpriteAnim(SPRITEGRP *group, int animIndex)
{
    if (group->isEnabled) {
        if (group->slots->animIndex >= 0)
            StopSprite(group, animIndex);
        RGBA c0(0x7F7F7F7F), c1(0xFFFFFFFF);
        StartSpriteAnimAt(group, animIndex, 0, D_002A3A10, 0.0f, c0, c1);
    }
}
} // namespace M_func_00217C20

// 0x217CA8 StartSpriteAnimSlot
namespace M_func_00217CA8 {
struct RGBA2 { unsigned char r, g, b, a; };
struct RGBA {
    unsigned char r, g, b, a;
    // Copy a color as one word.
    RGBA(const RGBA &c) { *(RGBA2 *)this = *(const RGBA2 *)&c; }
} __attribute__((aligned(16)));
extern void StartSpriteAnimAt(SPRITEGRP *p, int i, int z, void *q, float f, RGBA c0, RGBA c1) __asm__("StartSpriteAnimAt");
// Starts animation k in a reusable sprite slot. (inferred)
extern "C" void StartSpriteAnimSlot(SPRITEGRP *group, int animId, int slot, void *matrix, RGBA c0, RGBA c1)
{
    float t = 0.0f;
    int i;
    if (animId == 0x314) {
        i = group->pendingAnim;
        if (i != -1) {
            group->pendingAnim = -1;
        } else if (group->lastStartFrame == (*(unsigned long *)&g_clock.frameTick)) {
            t = group->lastStartTime;
            i = group->lastStartAnim;
        } else {
            group->lastStartTime = t;
            i = 0;
            group->lastStartAnim = 0;
        }
    } else {
        for (i = 0; i < group->numAnims; i++)
            if (group->anims[i].id == animId) break;
        if (i >= group->numAnims) return;
    }
    StartSpriteAnimAt(group, i, slot, matrix, t, c0, c1);
}
} // namespace M_func_00217CA8

// 0x217D98 StartSpriteAnimAt
namespace M_func_00217D98 {
// Select the initial sprite frame/time, then forward that same F12 value for drawing.
struct RGBA2 { unsigned char r, g, b, a; };
struct RGBA {
    unsigned char r, g, b, a;
    // Copy a color as one word.
    RGBA(const RGBA &c) { *(RGBA2 *)this = *(const RGBA2 *)&c; }
} __attribute__((aligned(16)));
// Compose two 2x3 affine matrices (q then n).
static inline SPRITEXF Mul(const SPRITEXF &q, const SPRITEXF &n)
{
    float m0 = q.a[0], m1 = q.a[1], m2 = q.a[2], m3 = q.a[3], m4 = q.a[4], m5 = q.a[5];
    SPRITEXF r;
    r.a[0] = m0 * n.a[0] + m1 * n.a[2];
    r.a[1] = m0 * n.a[1] + m1 * n.a[3];
    r.a[2] = m2 * n.a[0] + m3 * n.a[2];
    r.a[3] = m2 * n.a[1] + m3 * n.a[3];
    r.a[4] = m4 * n.a[0] + m5 * n.a[2] + n.a[4];
    r.a[5] = m4 * n.a[1] + m5 * n.a[3] + n.a[5];
    return r;
}
extern void AdvanceSpriteAnim(float,SPRITEANIM *pf, int z, SPRITEXF *pm, RGBA c0, RGBA c1) __asm__("AdvanceSpriteAnim");
// Starts a sprite animation at a given frame / time and draws it. (inferred)
extern "C" void StartSpriteAnimAt(SPRITEGRP *group, int animIndex, int slot, SPRITEXF *matrix, float t, RGBA c0, RGBA c1)
{
    SPRITESLOT *state = &group->slots[slot];
    SPRITEXF m;
    state->animIndex = animIndex;
    state->isFinished = 0;
    state->time = t;
    state->keyIndex = 0;
    SPRITEXF *pmUse;
    if (group->xform) {
        m = Mul(*matrix, *group->xform);
        pmUse = &m;
    } else {
        pmUse = matrix;
    }
    AdvanceSpriteAnim(t, &group->anims[state->animIndex], slot, pmUse, c0, c1);
}
} // namespace M_func_00217D98

// 0x217EF8 FindFreeSpriteSlot
// First free reusable sprite slot.
extern "C" int FindFreeSpriteSlot(SPRITEGRP *group)
{
    int i;
    int count = group->numSlots;
    for (i = 0; i < count; i++) {
        if (group->slots[i].animIndex < 0   /* free */)
            return i;
    }
    return 0;
}

// 0x217F38 ResetSpriteSlotItems
namespace M_func_00217F38 {
void ReleaseSpriteItem(void *p, SPRITEITEM *y) __asm__("ReleaseSpriteItem");
void InitSpriteItem(SPRITEITEM *y) __asm__("InitSpriteItem");
// Release each item's sprite descriptor.
extern "C" { static inline void ReleaseItems(int n, SPRITEITEM *items)
{
    int k; for (k = 0; k < n; k++) { SPRITEITEM *item = &items[k]; if (item->descriptor) ReleaseSpriteItem(item->descriptor, item); else InitSpriteItem(item); }
} }
// Resets / releases every item of sprite slot i. (inferred)
extern "C" void ResetSpriteSlotItems(SPRITEGRP *group, int slot)
{
    ReleaseItems(group->slots[slot].numItems, group->slots[slot].items);
}
} // namespace M_func_00217F38

// 0x217FB8 SetSpriteDefaultPhase
namespace M_func_00217FB8 {
void SetSpriteAnimPhase(SPRITEGRP *p, int a, int b) __asm__("SetSpriteAnimPhase");
} // namespace M_func_00217FB8
// Seeks the default animation by phase. (inferred)
extern "C" void SetSpriteDefaultPhase(SPRITEGRP *group)
{
    using namespace M_func_00217FB8;
    if (group->isEnabled)
        SetSpriteAnimPhase(group, 0, 0);
}

// 0x217FE0 SetSpriteAnimPhase
namespace M_func_00217FE0 {
void SetSpriteAnimTime(SPRITEGRP *p, int i, int j, float f) __asm__("SetSpriteAnimTime");
// Seeks a sprite animation to a fraction (clamped to 0..1) of its duration. (inferred)
extern "C" void SetSpriteAnimPhase(SPRITEGRP *group, int slot, int item, float phase)
{
    int animIndex;
    float clamped = 0.0f;
    float one = 1.0f;
    clamped = (phase > clamped) ? phase : clamped;
    clamped = (clamped < one) ? clamped : one;
    animIndex = group->slots[slot].animIndex;
    if (animIndex >= 0)
        SetSpriteAnimTime(group, slot, item, group->anims[animIndex].duration * clamped);
}
} // namespace M_func_00217FE0

// 0x218038 SetSpriteDefaultTime
namespace M_func_00218038 {
void SetSpriteAnimTime(SPRITEGRP *p, int a, int b) __asm__("SetSpriteAnimTime");
} // namespace M_func_00218038
// Seeks the default animation by time. (inferred)
extern "C" void SetSpriteDefaultTime(SPRITEGRP *group)
{
    using namespace M_func_00218038;
    if (group->isEnabled)
        SetSpriteAnimTime(group, 0, 0);
}

// 0x218060 SetSpriteAnimTime
namespace M_func_00218060 {
// Refresh a reused sprite at zero time, then at its clamped requested time.
typedef int qword __attribute__((mode(TI)));
struct RGBA2 { unsigned char r, g, b, a; };
struct RGBA {
    unsigned char r, g, b, a;
    // Uninitialised color.
    RGBA() {}
    // Copy a color as one word.
    RGBA(const RGBA &c) { *(RGBA2 *)this = *(const RGBA2 *)&c; }
    // Assign a color as one word.
    RGBA &operator=(const RGBA &c){*(RGBA2 *)this=*(const RGBA2 *)&c;return *this;}
} __attribute__((aligned(16)));
extern int FindSpriteItemPose(WIDGET *p, int k, SPRITEXF *pq, RGBA *pc0, RGBA *pc1, int c, void *ay) __asm__("FindSpriteItemPose");
extern void ResetSpriteSlotItems(SPRITEGRP *p, int i) __asm__("ResetSpriteSlotItems");
// F12 is independent of the integer slots. These pointers address real
// independent caller copies of the nontrivial by-value colors (A3/T0).
extern void UpdateSpriteSlot(float g, SPRITEGRP *p, int i, SPRITEXF *pq, const RGBA *c0, const RGBA *c1) __asm__("UpdateSpriteSlot");
// Seeks a reused sprite animation to a clamped time (refreshing it at time 0 first). (inferred)
// Also known as: SeekEntry (upstream readable / Sly 1 name; SeekEntry__13CScreenPlayeriPvf).
extern "C" void SetSpriteAnimTime(SPRITEGRP *group, int slot, SPRITEITEM *item, float time)
{
    SPRITEXF q;
    RGBA c0, c1;
    int animIndex = group->slots[slot].animIndex;
    if (animIndex < 0)
        return;
    time = StdMin(group->anims[animIndex].duration, StdMax(0.0f, time));
    if (item) {
        q = item->matrix;
        *(::RGBA *)&c0 = item->color0;
        *(::RGBA *)&c1 = item->color1;
    } else if (!FindSpriteItemPose(group->owner, group - group->owner->spriteGroups, &q, &c0, &c1, 0, 0)) {
        return;
    }
    ResetSpriteSlotItems(group, slot);
    group->slots[slot].time = 0.0f;
    group->slots[slot].keyIndex = 0;
    group->slots[slot].isFinished = 0;
    // The primary argument object is copied again for the second call.
    RGBA argument0(c0),argument1(c1);
    UpdateSpriteSlot(0.0f, group, slot, &q, &argument0, &argument1);
    group->slots[slot].isFinished = 0;
    argument0=c0;RGBA argument2(c1);
    UpdateSpriteSlot(time, group, slot, &q, &argument0, &argument2);
    group->slots[slot].isFinished = 1;
}
} // namespace M_func_00218060

// 0x218220 AdvanceWidgetSprite
namespace M_func_00218220 {
typedef int qword __attribute__((mode(TI)));
// Packed color word, quadword aligned.
struct RGBA { unsigned int u; RGBA(unsigned int x) {u=x;} } __attribute__((aligned(16)));
extern SPRITEXF D_002A3A10 __asm__("D_002A3A10");
extern qword GetLoWorldPos(void *) __asm__("GetLoWorldPos");
extern qword ConvertCmWorldToScreen(void *,qword) __asm__("ConvertCmWorldToScreen");
extern void UpdateSpriteSlot(SPRITEGRP *,int,SPRITEXF *,float,const RGBA &,const RGBA &) __asm__("UpdateSpriteSlot");
// Compose two 2x3 affine matrices (q then n).
static inline SPRITEXF Mul(const SPRITEXF &q, const SPRITEXF &n)
{
    float m0 = q.a[0], m1 = q.a[1], m2 = q.a[2], m3 = q.a[3], m4 = q.a[4], m5 = q.a[5];
    SPRITEXF r;
    r.a[0] = m0 * n.a[0] + m1 * n.a[2];
    r.a[1] = m0 * n.a[1] + m1 * n.a[3];
    r.a[2] = m2 * n.a[0] + m3 * n.a[2];
    r.a[3] = m2 * n.a[1] + m3 * n.a[3];
    r.a[4] = m4 * n.a[0] + m5 * n.a[2] + n.a[4];
    r.a[5] = m4 * n.a[1] + m5 * n.a[3] + n.a[5];
    return r;
}
// Advances the widget's main sprite by dt; a world-anchored widget follows its object's screen position. (inferred)
extern "C" void AdvanceWidgetSprite(SPRITEGRP *group,float dt) {
    if(group->isEnabled) {
        SPRITEXF m=D_002A3A10;
        if(group->owner->followObject) {
            qword worldPos=GetLoWorldPos(group->owner->followObject); qword screenPos=ConvertCmWorldToScreen(&g_cm,worldPos);
            m.a[4] = QwordX(screenPos)*320.0f+320.0f;
            m.a[5] = QwordY(screenPos)*-246.40001f+246.40001f;
        }
        if(group->owner->xform) m=Mul(m,*group->owner->xform);
        UpdateSpriteSlot(group,0,&m,dt,RGBA(0x7F7F7F7F),RGBA(0xFFFFFFFF));
    }
}
} // namespace M_func_00218220

// 0x2183E8 UpdateSpriteSlot
namespace M_func_002183E8 {
struct RGBA2 { unsigned char r, g, b, a; };
struct RGBA {
    unsigned char r, g, b, a;
    // Copy a color as one word.
    RGBA(const RGBA &c) { *(RGBA2 *)this = *(const RGBA2 *)&c; }
} __attribute__((aligned(16)));
// Compose two 2x3 affine matrices (q then n).
static inline SPRITEXF Mul(const SPRITEXF &q, const SPRITEXF &n)
{
    float m0 = q.a[0], m1 = q.a[1], m2 = q.a[2], m3 = q.a[3], m4 = q.a[4], m5 = q.a[5];
    SPRITEXF r;
    r.a[0] = m0 * n.a[0] + m1 * n.a[2];
    r.a[1] = m0 * n.a[1] + m1 * n.a[3];
    r.a[2] = m2 * n.a[0] + m3 * n.a[2];
    r.a[3] = m2 * n.a[1] + m3 * n.a[3];
    r.a[4] = m4 * n.a[0] + m5 * n.a[2] + n.a[4];
    r.a[5] = m4 * n.a[1] + m5 * n.a[3] + n.a[5];
    return r;
}
extern void InterpolateSpriteSlot(float,SPRITEANIM *,int,SPRITEXF *,RGBA,RGBA) __asm__("InterpolateSpriteSlot");
extern void AdvanceSpriteAnim(float,SPRITEANIM *,int,SPRITEXF *,RGBA,RGBA) __asm__("AdvanceSpriteAnim");
// Forwards a sprite slot's update to the reusable or the full animation path. (inferred)
// Also known as: Advance (upstream readable / Sly 1 name; Advance__13CScreenPlayeriP3M23f4RGBAT4).
extern "C" void UpdateSpriteSlot(SPRITEGRP *group,int slot,SPRITEXF *matrix,float dt,RGBA c0,RGBA c1) {
    SPRITESLOT *state=&group->slots[slot];
    int animIndex=state->animIndex;
    if(animIndex<0)return;
    SPRITEXF m;
    SPRITEXF *pmUse;
    if(group->xform) {
        m=Mul(*matrix,*group->xform);
        pmUse=&m;
    } else pmUse=matrix;
    if(state->isFinished) {   /* held: redraw at the stored time */
        InterpolateSpriteSlot(dt,&group->anims[animIndex],slot,pmUse,c0,c1);
    } else {
        if(group->anims[animIndex].duration <= state->time) {
            state->time=0.0f;   /* loop */
            state->keyIndex=0;
        } else state->time+=dt;
        AdvanceSpriteAnim(dt,&group->anims[animIndex],slot,pmUse,c0,c1);
    }
}
} // namespace M_func_002183E8

// 0x2185D0 DrawSprite
// Draws a sprite group (slot 0). (inferred)
extern "C" int DrawSprite(SPRITEGRP *spriteGroup)
{
    int result = spriteGroup->isEnabled;
    if (result)
        result = func_002185F8I(spriteGroup, 0, 0);
    return result;
}

// 0x2185F8 DrawSpriteSlot
// Draws the items of a sprite slot's current animation, if it plays one. (inferred)
extern "C" void DrawSpriteSlot(SPRITEGRP *spriteGroup, int slot, int drawFlags)
{
    short animIndex = spriteGroup->slots[slot].animIndex;
    if (animIndex >= 0)
        DrawSpriteItems(&spriteGroup->anims[animIndex], slot, drawFlags);
}

// 0x218638 StopSprite
// Stops a sprite group (slot 0). (inferred)
extern "C" void StopSprite(SPRITEGRP *spriteGroup)
{
    if (spriteGroup->isEnabled)
        StopSpriteSlot(spriteGroup, 0);
}

// 0x218660 StopSpriteSlot
namespace M_func_00218660 {
struct CLOCKIsland { unsigned long ticks; };   /* CLOCK island: time/deltaTime read as one 64-bit word (ld) */
void StopSnd(void *p, float f) __asm__("StopSnd");
void RcPtr_Assign(void **pp, void *next) __asm__("RcPtr_Assign");
void ResetSpriteSlotItems(SPRITEGRP *p, int i) __asm__("ResetSpriteSlotItems");
// Stop the sound and drop the reference, if any.
extern "C" { static inline void Release(void **pp)
{
    if (pp && *pp) {
        StopSnd(*pp, 0.0f);
        RcPtr_Assign(pp, 0);
    }
} }
// Stops a sprite slot's sounds and resets its items. (inferred)
// Also known as: StopEntrySound (upstream readable / Sly 1 name; StopEntrySound__FP8SENTRYRTi).
extern "C" void StopSpriteSlot(SPRITEGRP *group, int slot)
{
    SPRITESLOT *state = (SPRITESLOT*)(slot*sizeof(SPRITESLOT)+(int)group->slots);   /* &group->slots[slot] */
    if (state->animIndex >= 0) {
        /* remember where it stopped, so a main-track restart this frame resumes it (StartSpriteAnimSlot) */
        float t=state->time;
        group->lastStartAnim = state->animIndex;
        group->lastStartTime = t;
        group->lastStartFrame = g_clock.frameTick;
        group->pendingAnim = -1;
        state->animIndex = -1;
        state->isFinished = 0;
        Release(&state->sound);
    }
    ResetSpriteSlotItems(group, slot);
}
} // namespace M_func_00218660

// 0x218710 IsSpriteAnimActive
// True while the sprite animation is running.
extern "C" int IsSpriteAnimActive(SPRITEGRP *group)
{
    SPRITESLOT *mainSlot = group->slots;
    if (mainSlot->animIndex != 0 && mainSlot->isFinished == 0)
        return 1;
    return 0;
}

// 0x218738 GetSpriteMainState
// State of the sprite group's main animation. (inferred)
extern "C" int GetSpriteMainState(SPRITEGRP *spriteGroup)
{
    return spriteGroup->slots->isFinished;
}

// 0x218748 FinishSpriteSoundEvent
namespace M_func_00218748 {
void PauseSnd(void *p) __asm__("PauseSnd");
// The record's entry array (word at +8).
extern "C" { static inline SPRITESLOT *Entries(const SPRITEGRP *p){SPRITESLOT *value;__asm__("lw %0,8(%1)":"=r"(value):"0"(p),"m"(p->slots));return value;} }
// Load a sound pointer from its slot into $3.
extern "C" { static inline void *SoundRead(void *const *slot){register void *value __asm__("$3");__asm__("lw %0,%1":"=r"(value):"m"(*slot));return value;} }
// Address of entry `index` (register-pinned arithmetic).
extern "C" { static inline SPRITESLOT *EntryAt(SPRITESLOT *entries,int index){register SPRITESLOT *value asm("$2");register int bytes asm("$5")=index*sizeof(SPRITESLOT);asm("addu %0,%1,%2":"=r"(value):"r"(bytes),"r"(entries));return value;} }
} // namespace M_func_00218748
// Marks a sprite sound event done and stops or pauses its sound. (inferred)
extern "C" void FinishSpriteSoundEvent(SPRITEGRP *p, int slot)
{
    using namespace M_func_00218748;
    void *sound;
    p->slots[slot].isFinished = 1;
    sound=SoundRead(&EntryAt(Entries(p),slot)->sound);
    if (sound)
        PauseSnd(sound);
}

// 0x218790 CombineSpriteColors
namespace M_CombineSpriteColors {
// Combines parent and child colours (additive and multiplicative).
// Also known as: CombineColors (upstream readable / Sly 1 name; CombineColors__FPUcN50).
extern "C" void CombineSpriteColors(unsigned char *pa, unsigned char *pb, unsigned char *pc, unsigned char *pd,
                   unsigned char *pout0, unsigned char *pout1)
{
    int i;
    for (i = 0; i < 4; i++) {
        float x = StdMax(StdMin((float)(pa[i] - 127) * 0.007874016f, 1.0f), -1.0f);
        float y = StdMax(StdMin((float)(pc[i] - 127) * 0.007874016f, 1.0f), -1.0f);
        float e = (float)pb[i] - 127.0f;
        float d = (float)pd[i] - 127.0f;
        pout0[i] = (int)(StdMax(StdMin(x * y, 1.0f), -1.0f) * 127.0f + 128.0f);
        pout1[i] = (int)(StdMax(StdMin(x * d + e, 128.0f), -127.0f) + 127.0f);
    }
}
} // namespace M_CombineSpriteColors

// 0x2188B0 InterpolateSpriteSlot
namespace M_func_002188B0 {
// Update reusable sprite entries at their cached animation time.
// UpdateSpriteItem's float (frame duration) is its THIRD argument in the source: that order of the call's argument moves is what matches.
typedef unsigned char uchar;
typedef unsigned short ushort;
struct RGBA_RAW {uchar r,g,b,a;};
// Color bytes copied as one word.
struct RGBA {uchar r,g,b,a;RGBA(){}RGBA(const RGBA &c){*(RGBA_RAW *)this=*(const RGBA_RAW *)&c;}RGBA(const RGBA_RAW &c){*(RGBA_RAW *)this=c;}RGBA &operator=(const RGBA &c){*(RGBA_RAW *)this=*(const RGBA_RAW *)&c;return *this;}} __attribute__((aligned(16)));
struct M23 {float a[6];} __attribute__((aligned(16)));
struct SPRITE;
struct ITEM {RGBA_RAW additive,multiply;char pad8[8];M23 matrix;SPRITE *sprite;int kind;float previousTime;RGBA_RAW previousAdditive,previousMultiply;M23 *previousMatrix;float nextTime;RGBA_RAW nextAdditive,nextMultiply;M23 *nextMatrix;int index;float cachedTime;uchar interpolate;char pad61[7];int flags,pad6C;};
struct REUSE {int count;ITEM *items;ushort index,phase;float time;int field10,field14;};
struct ANIM;
struct GROUP {ushort animCount,reuseCount;ANIM *animations;REUSE *reuse;char padC[0x14];int index;void *owner;int first,pad2C;};
struct ANIM {ushort count,pad2;void *keys;float duration;void *owner;GROUP *group;int id;};

void ClearSle(int *) __asm__("ClearSle");
void CombineSpriteColors(RGBA *,RGBA *,RGBA *,RGBA *,RGBA *,RGBA *) __asm__("CombineSpriteColors");
void UpdateSpriteItem(SPRITE *,ITEM *,float,M23 *,RGBA,RGBA) __asm__("UpdateSpriteItem");

// Interpolate each channel between previous and next at t.
static inline RGBA BlendColors(float t,RGBA previous,RGBA next){
 RGBA result;
 result.r=LerpInt(t,previous.r,next.r);result.g=LerpInt(t,previous.g,next.g);result.b=LerpInt(t,previous.b,next.b);result.a=LerpInt(t,previous.a,next.a);
 return result;
}
// Interpolate the six matrix terms between previous and next at t.
static inline M23 BlendMatrix(float t,const M23 &previous,const M23 &next){
 M23 result;
 for(int n=0;n<6;n++)result.a[n]=(1.0f-t)*previous.a[n]+t*next.a[n];
 return result;
}
// Compose two 2x3 affine matrices (q then n).
static inline M23 Mul(const M23 &q,const M23 &n){
 float m0=q.a[0],m1=q.a[1],m2=q.a[2],m3=q.a[3],m4=q.a[4],m5=q.a[5];M23 r;
 r.a[0]=m0*n.a[0]+m1*n.a[2];r.a[1]=m0*n.a[1]+m1*n.a[3];r.a[2]=m2*n.a[0]+m3*n.a[2];r.a[3]=m2*n.a[1]+m3*n.a[3];r.a[4]=m4*n.a[0]+m5*n.a[2]+n.a[4];r.a[5]=m4*n.a[1]+m5*n.a[3]+n.a[5];return r;
}
// Updates reusable sprite entries at the cached time. (inferred)
extern "C" void InterpolateSpriteSlot(float duration,ANIM *animation,int index,M23 *parent,RGBA additive,RGBA multiply){
 int i=0;
 REUSE *reuse=(REUSE *)(index*sizeof(REUSE)+(int)animation->group->reuse);
 int count=reuse->count;ITEM *items=reuse->items;float time=reuse->time;
 M23 matrix;RGBA outputAdditive,outputMultiply,currentMultiply,currentAdditive;
 for(;i<count;i++){
  ITEM *item=&items[i];
  if(!item->sprite){item->cachedTime=time;ClearSle(&item->flags);continue;}
  if(item->interpolate&&item->nextMatrix){
   float fraction=(time-item->previousTime)/(item->nextTime-item->previousTime);
   matrix=BlendMatrix(fraction,*item->previousMatrix,*item->nextMatrix);
   currentAdditive=BlendColors(fraction,RGBA(item->previousAdditive),RGBA(item->nextAdditive));
   currentMultiply=BlendColors(fraction,RGBA(item->previousMultiply),RGBA(item->nextMultiply));
  }
  else{matrix=*item->previousMatrix;*(RGBA_RAW *)&currentAdditive=item->previousAdditive;*(RGBA_RAW *)&currentMultiply=item->previousMultiply;}
  matrix=Mul(matrix,*parent);
  {
   RGBA inputMultiply(multiply),inputAdditive(additive),localMultiply(currentMultiply),localAdditive(currentAdditive);
   CombineSpriteColors(&inputMultiply,&inputAdditive,&localMultiply,&localAdditive,&outputMultiply,&outputAdditive);
  }
  if(item->sprite)UpdateSpriteItem(item->sprite,item,duration,&matrix,outputAdditive,outputMultiply);
  item->cachedTime=time;item->matrix=matrix;
  item->additive=*(RGBA_RAW *)&outputAdditive;item->multiply=*(RGBA_RAW *)&outputMultiply;
  ClearSle(&item->flags);
 }
}
} // namespace M_func_002188B0

// 0x2190F8 FinishSpriteSoundEvents
namespace M_func_002190F8 {
void FinishSpriteSoundEvent(void *p, int k) __asm__("FinishSpriteSoundEvent");
// Finishes every sound event (action 7) of a sprite key. (inferred)
extern "C" void FinishSpriteSoundEvents(SPRITEANIM *anim, int key, int slot)
{
    SPRITEKEY *keyData = &anim->keys[key];
    int j;
    for (j = 0; j < keyData->numActions; j++) {
        if (keyData->actions[j] == 7)   /* a sound action */
            FinishSpriteSoundEvent(anim->group, slot);
    }
}
} // namespace M_func_002190F8

// 0x219190 HandleSpriteKeyEvents
namespace M_HandleSpriteKeyEvents {
// Dispatch sprite actions and manage sound slots.
struct O;
struct E { char pad[0x14]; CID151 *pm; };
struct R { char pad[8]; struct E *ae; };
struct A { int kind; int id; };
struct F { char pad[0x18]; unsigned char count; char pad1[3]; struct A *aa; };
struct S { char pad[4]; struct F *af; char pad1[8]; struct R *r; };
extern CID151 *CreateSfxSound(int id) __asm__("CreateSfxSound");
extern void SetSndSourceLo(CID151 *, struct O *) __asm__("SetSndSourceLo");
extern void StopSnd(CID151 *, float) __asm__("StopSnd");
extern void RcPtr_Assign(CID151 **pp, CID151 *m) __asm__("RcPtr_Assign");
extern void *StartDialog(void *p,int a,int b) __asm__("StartDialog");
extern void StopDialog(void *p) __asm__("StopDialog");
// Load the sound from its slot into $4.
static inline CID151 *SlotSound(CID151 *const &slot){register CID151 *value asm("$4");asm("lw %0,0(%1)":"=r"(value):"r"(&slot),"m"(slot));return value;}
// Stop the slot's sound and drop the reference, if any.
static inline void Stop(CID151 **pp){if(pp){CID151 *sound=SlotSound(*pp);if(sound){StopSnd(sound,0.0f);RcPtr_Assign(pp,0);}}}
// Load the slot word (non-zero when a sound is present) into $3.
static inline unsigned int Presence(CID151 *const &slot){register unsigned int value asm("$3");__asm__("lw %0,%1":"=r"(value):"m"(slot));return value;}
// index * stride (mult).
static inline int EntryOffset(int index,int stride){int value;__asm__("mult %0,%1,%2":"=r"(value):"r"(index),"r"(stride):"hi","lo");return value;}
// Load the action's kind into $3.
static inline int ActionKind(const A &action){register int value asm("$3");asm("lw %0,%1":"=r"(value):"m"(action.kind));return value;}
// index * stride (mult) into $5.
static inline int StopOffset(int index,int stride){register int value asm("$5");asm("mult %0,%1,%2":"=r"(value):"r"(index),"r"(stride):"hi","lo");return value;}
// Load the owner's receiver into $3.
static inline R *StopReceiver(const S *owner){register R *value asm("$3");asm("lw %0,%1":"=r"(value):"m"(owner->r));return value;}
// Load the receiver's entry array into $4.
static inline E *StopArray(const R *receiver){register E *value asm("$4");asm("lw %0,%1":"=r"(value):"m"(receiver->ae));return value;}
// Address of the sound slot at the end of an entry.
static inline CID151 **SlotAddress(E *entry){CID151 **value;asm("addiu %0,%0,%2":"=r"(value):"0"(entry),"i"(sizeof(E)-sizeof(CID151 *)));return value;}
// Address of the entry at byte offset `offset` in the array.
static inline E *StopEntry(E *array,int offset){register E *value asm("$2");asm("addu %0,%1,%2":"=r"(value):"r"(offset),"r"(array));return value;}
// Runs a sprite key's actions: start / stop sounds, start / stop dialog.
// Also known as: RunScreenKeyEvents (upstream readable / Sly 1 name; RunScreenKeyEvents__FP8SENTRYRTii).
extern "C" void HandleSpriteKeyEvents(struct S *p,int fi,int i) {
    struct F *f=&p->af[fi];
    int j=0;
    if(!f->count)return;
    int stride=sizeof(E);
    int createOffset=EntryOffset(i,stride);
    do {
        struct A *a=(struct A *)(j*sizeof(A)+(int)f->aa);
        switch(ActionKind(*a)) {
        case 0: {
            CID151 *m=CreateSfxSound(a->id);
            SetSndSourceLo(m,0);
            if(m->GetKind()==2)
                RcPtr_Assign(&((E *)(createOffset+(int)p->r->ae))->pm,m);
            break;
        }
        case 1: {
            R *receiver=StopReceiver(p);int offset=StopOffset(i,stride);
            E *e=StopEntry(StopArray(receiver),offset);
            if(Presence(e->pm)) Stop(SlotAddress(e));
            break;
        }
        case 2: StartDialog(&g_sndmgr,a->id,0); break;
        case 3: StopDialog(&g_sndmgr); break;
        }
    } while(++j<f->count);
}
} // namespace M_HandleSpriteKeyEvents

// 0x219348 ResetSpriteKeyItems
namespace M_ResetSpriteKeyItems {
void ReleaseSpriteItem(void *p, SPRITEITEM *y) __asm__("ReleaseSpriteItem");
void InitSpriteItem(SPRITEITEM *y) __asm__("InitSpriteItem");
// Resets the items a sprite key refers to.
extern "C" void ResetSpriteKeyItems(SPRITEANIM *anim, int key, int slot)
{
    SPRITEKEY *keyData = &anim->keys[key];
    SPRITEITEM *items = anim->group->slots[slot].items;
    int k;
    for (k = 0; k < keyData->numItemRefs; k++) {
        SPRITEITEM *item = &items[keyData->itemRefs[k]];
        if (item->descriptor)
            ReleaseSpriteItem(item->descriptor, item);
        else
            InitSpriteItem(item);
    }
}
} // namespace M_ResetSpriteKeyItems

// 0x219400 ApplySpriteKeyPoses
namespace M_ApplySpriteKeyPoses {
// ApplySpriteKeyPoses: apply one animation key's poses to the reuse slot's items (sprite animation, see AdvanceSpriteAnim).
// For every pose of key `phase`: reset the item's "previous" state (colours default to additive 0x7F7F7F7F / multiply
// 0xFFFFFFFF unless the pose has a COLORS entry), look in the following keys for the same item to find its "next"
// pose, then interpolate matrix and colours between them at the clamped time (ClampToLimits), combine with the parent
// matrix/colours (CombineSpriteColors) and draw through UpdateSpriteItem (same sprite) or replace the sprite (ReleaseSpriteItem +
// SetSpriteItemSource). Finally the item's matrix/colours/pose bytes are saved and the link kinds are set (1 for the
// linking pose, 4 for the item it links to).
//
// STAND-IN (see LoadOptionFromBrx__FPvP5EOPIDiP18CBinaryInputStream.cpp for the same trick): the spilled gcse reaching pseudos (j+1, &item->flags, &outputAdditive
// and the hoisted frame addresses) get their spill slots/registers from the gcse expression hash-table size
// S = max_cuid/2|1. This spelling has S = 279 after cse1; the game's order needs S = 277/283/289. `if (fNever)` is dead code
// (cprop folds it, jump deletes it; the final code is unchanged) that only supplies the missing insns (S = 283).
// What the game really had there is unknown.
typedef unsigned int uint;
typedef unsigned char uchar;
typedef unsigned short ushort;
struct RGBA_RAW { uchar r, g, b, a; };
struct RGBA {
    uchar r, g, b, a;
    // Uninitialised color.
    RGBA() {}
    // Copy a color as one word.
    RGBA(const RGBA &c) { *(RGBA_RAW *)this = *(const RGBA_RAW *)&c; }
    // Color from its raw word.
    RGBA(const RGBA_RAW &c) { *(RGBA_RAW *)this = c; }
    // Assign a color as one word.
    RGBA &operator=(const RGBA &c) { *(RGBA_RAW *)this = *(const RGBA_RAW *)&c; return *this; }
} __attribute__((aligned(16)));
struct M23 { float a[6]; } __attribute__((aligned(16)));
struct SPRITE { int id, kind; void *group; int fieldC, index; void *extra; char pad18[0x28]; };
struct ITEM {
    RGBA_RAW additive, multiply;
    char pad8[8];
    M23 matrix;
    SPRITE *sprite;
    int kind;
    float previousTime;
    RGBA_RAW previousAdditive, previousMultiply;
    M23 *previousMatrix;
    float nextTime;
    RGBA_RAW nextAdditive, nextMultiply;
    M23 *nextMatrix;
    int index;
    float cachedTime;
    uchar interpolate, mode;
    char pad62[2];
    int field64;
    int flags, pad6C;
};
struct REUSE { int count; ITEM *items; ushort index, phase; float time; int field10, field14; };
struct ANIM;
struct GROUP {
    ushort animCount, reuseCount;
    ANIM *animations;
    REUSE *reuse;
    char padC[0x14];
    int index;
    void *owner;
    int first, pad2C;
};
struct KEYPOSE {
    ushort id, index, offset;
    short color;
    char pad8[8];
    M23 matrix;
    uchar interpolate, mode;
    char pad32[2];
    int selected;
    char pad38[8];
};
struct COLORS { RGBA_RAW multiply, additive; };
struct TAG { int kind, value; };
struct KEY {
    float time;
    uchar itemCount, colorCount, shortCount, byteCount;
    uchar *bytes;
    KEYPOSE *items;
    COLORS *colors;
    ushort *shorts;
    uchar tagCount;
    char pad19[3];
    TAG *tags;
};
struct ROOT { char pad[0x54]; SPRITE *sprites; };
struct ANIM { ushort count, pad2; KEY *keys; float duration; ROOT *owner; GROUP *group; int id; };

void ClearSle(int *) __asm__("ClearSle");
void *func_002333B4(void *, int, unsigned int) __asm__("memset");
void SetSpriteItemSource(SPRITE *, ITEM *, M23 *, RGBA, RGBA) __asm__("SetSpriteItemSource");
extern "C" { void ApplySpriteKeyPoses(float, float, ANIM *, int, int, M23 *, const RGBA *, const RGBA *); }
void ReleaseSpriteItem(SPRITE *, ITEM *) __asm__("ReleaseSpriteItem");
void CombineSpriteColors(RGBA *, RGBA *, RGBA *, RGBA *, RGBA *, RGBA *) __asm__("CombineSpriteColors");
void UpdateMatchingSprite(float, SPRITE *, ITEM *, M23 *, RGBA, RGBA) __asm__("UpdateSpriteItem");
void UpdateCurrentSprite(float, SPRITE *, ITEM *, M23 *, RGBA, RGBA) __asm__("UpdateSpriteItem");


// Lerp one colour channel; the +0.999 rounds towards the far end.

// Interpolate each channel between previous and next at t.
static inline RGBA BlendColors(float t, RGBA previous, RGBA next)
{
    RGBA result;
    result.r = LerpInt(t, previous.r, next.r);
    result.g = LerpInt(t, previous.g, next.g);
    result.b = LerpInt(t, previous.b, next.b);
    result.a = LerpInt(t, previous.a, next.a);
    return result;
}

// Interpolate the six matrix terms between previous and next at t.
static inline M23 BlendMatrix(float t, const M23 &previous, const M23 &next)
{
    M23 result;
    for (int n = 0; n < 6; n++)
        result.a[n] = (1.0f - t) * previous.a[n] + t * next.a[n];
    return result;
}

// Compose two 2x3 affine matrices (q then n).
static inline M23 Mul(const M23 &q, const M23 &n)
{
    float m0 = q.a[0], m1 = q.a[1], m2 = q.a[2], m3 = q.a[3], m4 = q.a[4], m5 = q.a[5];
    M23 r;
    r.a[0] = m0 * n.a[0] + m1 * n.a[2];
    r.a[1] = m0 * n.a[1] + m1 * n.a[3];
    r.a[2] = m2 * n.a[0] + m3 * n.a[2];
    r.a[3] = m2 * n.a[1] + m3 * n.a[3];
    r.a[4] = m4 * n.a[0] + m5 * n.a[2] + n.a[4];
    r.a[5] = m4 * n.a[1] + m5 * n.a[3] + n.a[5];
    return r;
}

enum Uninitialized { Uninit };
struct LM {
    float lo, hi;
    // Uninitialised limit.
    LM(Uninitialized) {}
    // Uninitialised limit.
    LM() {}
};
float ClampToLimits(LM *, float) __asm__("ClampToLimits__FP2LMf");

// Applies one animation key's poses (transforms, colours) to the sprite items.
extern "C" void ApplySpriteKeyPoses(float time, float duration, ANIM *animation, int phase, int index, M23 *parent, const RGBA *additive, const RGBA *multiply)
{
    int fNever = 0;
    int j = 0;
    ITEM *items = animation->group->reuse[index].items;
    KEY *key = &animation->keys[phase];
    M23 matrix;
    RGBA outputAdditive, outputMultiply, currentAdditive, currentMultiply;

    for (; j < key->itemCount; j++) {
        KEYPOSE *pose = &key->items[j];
        int entryIndex = pose->index, linkOffset = pose->offset, id = pose->id;
        ITEM *item = (ITEM *)(entryIndex * sizeof(ITEM) + (int)items);
        // declared here (not folded into the stores): loop.c only hoists the two constants if they live across the colour test
        uint defaultAdditive = 0x7F7F7F7F, defaultMultiply = 0xFFFFFFFF;
        COLORS *colors = 0;
        if (pose->color >= 0)
            colors = &key->colors[pose->color];
        uint additiveWord = defaultAdditive;
        *(uint *)&item->previousAdditive = additiveWord;
        *(uint *)&currentAdditive = additiveWord;
        uint multiplyWord = defaultMultiply;
        *(uint *)&item->previousMultiply = multiplyWord;
        *(uint *)&currentMultiply = multiplyWord;
        if (colors) {
            item->previousAdditive = colors->additive;
            *(RGBA_RAW *)&currentAdditive = item->previousAdditive;
            item->previousMultiply = colors->multiply;
            *(RGBA_RAW *)&currentMultiply = item->previousMultiply;
        }
        item->previousMatrix = &pose->matrix;
        item->previousTime = key->time;
        item->nextMatrix = 0;
        item->nextTime = 0.0f;

        // find this item's pose in a later key
        if (pose->interpolate) {
            int nextKey = phase + 1;
            if (nextKey < animation->count) {
                do {
                    KEY *future = &animation->keys[nextKey];
                    int afterKey = nextKey + 1;
                    for (int n = 0; n < future->itemCount; n++) {
                        KEYPOSE *futurePose = (KEYPOSE *)(n * sizeof(KEYPOSE) + (int)future->items);
                        if (futurePose->index == entryIndex) {
                            item->nextMatrix = &futurePose->matrix;
                            item->nextTime = future->time;
                            int futureColor = future->items[n].color;
                            if (futureColor >= 0) {
                                item->nextAdditive = future->colors[futureColor].additive;
                                item->nextMultiply = future->colors[futureColor].multiply;
                            } else {
                                item->nextAdditive = item->previousAdditive;
                                item->nextMultiply = item->previousMultiply;
                            }
                            break;
                        }
                    }
                    nextKey = afterKey;
                } while (nextKey < animation->count && !item->nextMatrix);
            }
        }

        if (pose->interpolate && item->nextMatrix && item->previousTime < item->nextTime) {
            LM copy(Uninit);
            float limited;
            {
                LM bounds;
                LM *pb = &bounds;
                func_002333B4(pb, 0, 8);
                bounds.lo = item->previousTime;
                bounds.hi = item->nextTime;
                copy = bounds;
                limited = ClampToLimits(&copy, duration);
            }
            float fraction = (limited - item->previousTime) / (item->nextTime - item->previousTime);
            matrix = BlendMatrix(fraction, *item->previousMatrix, *item->nextMatrix);
            currentAdditive = BlendColors(fraction, RGBA(item->previousAdditive), RGBA(item->nextAdditive));
            currentMultiply = BlendColors(fraction, RGBA(item->previousMultiply), RGBA(item->nextMultiply));
        } else {
            matrix = *item->previousMatrix;
            *(RGBA_RAW *)&currentAdditive = item->previousAdditive;
            *(RGBA_RAW *)&currentMultiply = item->previousMultiply;
        }
        matrix = Mul(matrix, *parent);
        {
            RGBA inputMultiply(*multiply), inputAdditive(*additive), localMultiply(currentMultiply), localAdditive(currentAdditive);
            CombineSpriteColors(&inputMultiply, &inputAdditive, &localMultiply, &localAdditive, &outputMultiply, &outputAdditive);
        }
        if (id > 0) {
            if (item->sprite) {
                if (item->sprite->id == id) {
                    UpdateMatchingSprite(duration, item->sprite, item, &matrix, outputAdditive, outputMultiply);
                } else {
                    ReleaseSpriteItem(item->sprite, item);
                    SetSpriteItemSource(&animation->owner->sprites[id], item, &matrix, outputAdditive, outputMultiply);
                }
            } else {
                SetSpriteItemSource(&animation->owner->sprites[id], item, &matrix, outputAdditive, outputMultiply);
            }
        } else {
            UpdateCurrentSprite(duration, item->sprite, item, &matrix, outputAdditive, outputMultiply);
        }
        item->cachedTime = time;
        item->matrix = matrix;
        item->additive = *(RGBA_RAW *)&outputAdditive;
        item->multiply = *(RGBA_RAW *)&outputMultiply;
        item->interpolate = pose->interpolate;
        item->mode = pose->mode;
        item->field64 = pose->selected;
        ClearSle(&item->flags);
        if (linkOffset) {
            item->kind = 1;
            items[entryIndex + linkOffset].kind = 4;
        } else if (item->kind != 4)
            item->kind = 0;
        if (fNever)
            ClearSle(0);
    }
}
} // namespace M_ApplySpriteKeyPoses

// 0x219FE0 AdvanceSpriteAnim
namespace M_AdvanceSpriteAnim {
// Advance a sprite animation to the reuse slot's time and redraw every item whose cached
// time changed: run the due key (events + pose setup via ApplySpriteKeyPoses), then for each item
// either reset it (first pass) or interpolate its matrix/colours between the previous and
// next poses, combine with the parent matrix/colours and draw it through UpdateSpriteItem.
typedef unsigned char uchar;
typedef unsigned short ushort;
struct RGBA_RAW { uchar r, g, b, a; };
struct RGBA {
    uchar r, g, b, a;
    // Uninitialised color.
    RGBA() {}
    // Copy a color as one word.
    RGBA(const RGBA &c) { *(RGBA_RAW *)this = *(const RGBA_RAW *)&c; }
    // Color from its raw word.
    RGBA(const RGBA_RAW &c) { *(RGBA_RAW *)this = c; }
    // The color as its raw word.
    const RGBA_RAW &Raw() const { return *(const RGBA_RAW *)this; }
    // Assign a color as one word.
    RGBA &operator=(const RGBA &c) { *(RGBA_RAW *)this = *(const RGBA_RAW *)&c; return *this; }
} __attribute__((aligned(16)));
struct M23 { float a[6]; } __attribute__((aligned(16)));
struct SPRITE;
struct ITEM {
    RGBA_RAW additive, multiply;
    char pad8[8];
    M23 matrix;
    SPRITE *sprite;
    int kind;
    float previousTime;
    RGBA_RAW previousAdditive, previousMultiply;
    M23 *previousMatrix;
    float nextTime;
    RGBA_RAW nextAdditive, nextMultiply;
    M23 *nextMatrix;
    int index;
    float cachedTime;
    uchar interpolate, mode;
    char pad62[2];
    int field64;
    int flags, pad6C;
};
struct REUSE { int count; ITEM *items; ushort index, phase; float time; int field10, field14; };
struct ANIM;
struct GROUP {
    ushort animCount, reuseCount;
    ANIM *animations;
    REUSE *reuse;
    char padC[0x14];
    int index;
    void *owner;
    int first, pad2C;
};
struct KEYPOSE {
    ushort id, index, offset;
    short color;
    char pad8[8];
    M23 matrix;
    uchar interpolate, mode;
    char pad32[2];
    int selected;
    char pad38[8];
};
struct COLORS { RGBA_RAW multiply, additive; };
struct TAG { int kind, value; };
struct KEY {
    float time;
    uchar itemCount, colorCount, shortCount, byteCount;
    uchar *bytes;
    KEYPOSE *items;
    COLORS *colors;
    ushort *shorts;
    uchar tagCount;
    char pad19[3];
    TAG *tags;
};
struct ANIM { ushort count, pad2; KEY *keys; float duration; void *owner; GROUP *group; int id; };


void ClearSle(int *) __asm__("ClearSle");
void HandleSpriteKeyEvents(ANIM *, int, int) __asm__("HandleSpriteKeyEvents");
void FinishSpriteSoundEvents(ANIM *, int, int) __asm__("FinishSpriteSoundEvents");
void ResetSpriteKeyItems(ANIM *, int, int) __asm__("ResetSpriteKeyItems");
void ApplySpriteKeyPoses(float, float, ANIM *, int, int, M23 *, const RGBA *, const RGBA *) __asm__("ApplySpriteKeyPoses");
void ReleaseSpriteItem(SPRITE *, ITEM *) __asm__("ReleaseSpriteItem");
void CombineSpriteColors(RGBA *, RGBA *, RGBA *, RGBA *, RGBA *, RGBA *) __asm__("CombineSpriteColors");
void UpdateSpriteItem(float, SPRITE *, ITEM *, M23 *, RGBA, RGBA) __asm__("UpdateSpriteItem");


// Lerp one colour channel; the +0.999 rounds towards the far end.

// Interpolate each channel between previous and next at t.
static inline RGBA BlendColors(float t, RGBA previous, RGBA next)
{
    RGBA result;
    result.r = LerpInt(t, previous.r, next.r);
    result.g = LerpInt(t, previous.g, next.g);
    result.b = LerpInt(t, previous.b, next.b);
    result.a = LerpInt(t, previous.a, next.a);
    return result;
}

// Interpolate the six matrix terms between previous and next at t.
static inline M23 BlendMatrix(float t, const M23 &previous, const M23 &next)
{
    M23 result;
    for (int n = 0; n < 6; n++)
        result.a[n] = (1.0f - t) * previous.a[n] + t * next.a[n];
    return result;
}

// Compose two 2x3 affine matrices (q then n).
static inline M23 Mul(const M23 &q, const M23 &n)
{
    float m0 = q.a[0], m1 = q.a[1], m2 = q.a[2], m3 = q.a[3], m4 = q.a[4], m5 = q.a[5];
    M23 r;
    r.a[0] = m0 * n.a[0] + m1 * n.a[2];
    r.a[1] = m0 * n.a[1] + m1 * n.a[3];
    r.a[2] = m2 * n.a[0] + m3 * n.a[2];
    r.a[3] = m2 * n.a[1] + m3 * n.a[3];
    r.a[4] = m4 * n.a[0] + m5 * n.a[2] + n.a[4];
    r.a[5] = m4 * n.a[1] + m5 * n.a[3] + n.a[5];
    return r;
}

// Advances a sprite animation to the slot time and redraws the items that changed.
extern "C" void AdvanceSpriteAnim(float duration, ANIM *animation, int index, M23 *parent, RGBA additive, RGBA multiply)
{
    // items is read last: that read carries the reuse pointer's REG_DEAD note, which is
    // what makes the scheduler issue its load first (as in the game).
    int count = animation->group->reuse[index].count;
    float time = animation->group->reuse[index].time;
    int phase = animation->group->reuse[index].phase;
    ITEM *items = animation->group->reuse[index].items;
    float processed = time;
    int first = phase == 0;
    M23 matrix;
    RGBA outputAdditive, outputMultiply, currentAdditive, keyMultiply, currentMultiply;

    do {
        if (phase < animation->count && animation->keys[phase].time <= time) {
            processed = animation->keys[phase].time;
            HandleSpriteKeyEvents(animation, phase, index);
            FinishSpriteSoundEvents(animation, phase, index);
            ResetSpriteKeyItems(animation, phase, index);
            currentAdditive = additive;
            keyMultiply = multiply;
            ApplySpriteKeyPoses(processed, duration, animation, phase, index, parent, &currentAdditive, &keyMultiply);
            phase++;
            animation->group->reuse[index].phase++;
            if (animation->count < animation->group->reuse[index].phase)
                animation->group->reuse[index].phase = animation->count;
        }
        for (int i = 0; i < count; i++) {
            ITEM *item = &items[i];
            if (item->sprite && item->cachedTime != processed) {
                if (first)
                    ReleaseSpriteItem(item->sprite, item);
                else {
                    if (item->interpolate && item->nextMatrix) {
                        float fraction = (processed - item->previousTime) / (item->nextTime - item->previousTime);
                        matrix = BlendMatrix(fraction, *item->previousMatrix, *item->nextMatrix);
                        currentAdditive = BlendColors(fraction, RGBA(item->previousAdditive), RGBA(item->nextAdditive));
                        currentMultiply = BlendColors(fraction, RGBA(item->previousMultiply), RGBA(item->nextMultiply));
                    } else {
                        matrix = *item->previousMatrix;
                        *(RGBA_RAW *)&currentAdditive = item->previousAdditive;
                        *(RGBA_RAW *)&currentMultiply = item->previousMultiply;
                    }
                    matrix = Mul(matrix, *parent);
                    {
                        RGBA inputMultiply(multiply), inputAdditive(additive), localMultiply(currentMultiply), localAdditive(currentAdditive);
                        CombineSpriteColors(&inputMultiply, &inputAdditive, &localMultiply, &localAdditive, &outputMultiply, &outputAdditive);
                    }
                    UpdateSpriteItem(duration, item->sprite, item, &matrix, outputAdditive, outputMultiply);
                    item->cachedTime = processed;
                    item->matrix = matrix;
                    item->additive = outputAdditive.Raw();
                    item->multiply = outputMultiply.Raw();
                    ClearSle(&item->flags);
                }
            }
        }
        first = 0;
    } while (phase < animation->count && animation->keys[phase].time <= time);
}
} // namespace M_AdvanceSpriteAnim

// 0x21A998 DrawSpriteItems
namespace M_func_0021A998 {
void QueueSpriteItem(void *descriptor, SPRITEITEM *item, int drawFlags) __asm__("QueueSpriteItem");
void PrependSlEntry(SL *psl, SPRITEITEM *y) __asm__("PrependSlEntry");
// Queue a sprite's items for drawing, last first: items with a descriptor through QueueSpriteItem, descriptor-less
// kind-4 items straight onto the widget's render queue.
extern "C" { static inline void Draw(SPRITEANIM *anim, int n, SPRITEITEM *items, int drawFlags)
{
    int k;
    for (k = n - 1; k >= 0; k--) {
        SPRITEITEM *item = &items[k];
        if (item->descriptor)
            QueueSpriteItem(item->descriptor, item, drawFlags);
        else if (item->kind == 4)   /* no descriptor: queued as is (inferred) */
            PrependSlEntry(&anim->owner->renderQueue, item);
    }
} }
// Queues a sprite's items for drawing in reverse order. (inferred)
extern "C" void DrawSpriteItems(SPRITEANIM *anim, int slot, int drawFlags)
{
    Draw(anim, anim->group->slots[slot].numItems, anim->group->slots[slot].items, drawFlags);
}
} // namespace M_func_0021A998

// 0x21AA60 StaticInit_hud
namespace M_StaticInit_hud {
// Initialize sprite string generators and their supporting heaps.
struct LINEAR {void *vtable;int limit;void *buffer;int used;};
struct HEAP {void *vtable;int field4;};
struct POOL;
extern void *func_002333B4(void *,int,unsigned int) __asm__("memset");
struct STRINGGEN {
 int id;void (*get)(char *,void *,void *);void *context;
 // Zero the whole string generator.
 STRINGGEN(){func_002333B4(this,0,sizeof(*this));}
};

LINEAR *__11CLinearHeapiPUc(LINEAR *,int,void *) __asm__("__11CLinearHeapiPUc");
POOL *InitSlotheap(POOL *,int,int,int) __asm__("InitSlotheap");
void BuildContextText(char *,void *,void *) __asm__("BuildContextText");

extern LINEAR D_003E1C70 __asm__("D_003E1C70");
extern HEAP D_003E1C80 __asm__("D_003E1C80");
extern char D_003E21A0[] __asm__("D_003E21A0");
extern STRINGGEN D_003E1C88[108] __asm__("D_003E1C88");
extern void *D_002C40A0 __asm__("D_002C40A0");
extern int D_002DD2E0 __asm__("__sn_t9CSlotPool2i20i8192$s_pool"),D_002DD2E4 __asm__("__sn_t9CSlotPool2i24i2048$s_pool"),D_002DD2E8 __asm__("__sn_t9CSlotPool2i40i512$s_pool"),D_002DD2EC __asm__("__sn_t9CSlotPool2i28i2048$s_pool"),D_002DD2F0 __asm__("__sn_t9CSlotPool2i12i200$s_pool"),D_002DD2F4 __asm__("__sn_t9CSlotPool2i20i256$s_pool");
extern POOL D_004520A8 __asm__("_t9CSlotPool2i20i8192$s_pool"),D_004520D0 __asm__("_t9CSlotPool2i24i2048$s_pool"),D_004520F8 __asm__("_t9CSlotPool2i40i512$s_pool"),D_00452120 __asm__("_t9CSlotPool2i28i2048$s_pool"),D_00452148 __asm__("_t9CSlotPool2i12i200$s_pool"),D_00452170 __asm__("_t9CSlotPool2i20i256$s_pool");
// Existing constant records are twelve bytes each; the linker separates them by sixteen.
extern const STRINGGEN D_002A8B90 __asm__("D_002A8B90");
extern const STRINGGEN D_002A8BA0 __asm__("D_002A8BA0");
extern const STRINGGEN D_002A8BB0 __asm__("D_002A8BB0");
extern const STRINGGEN D_002A8BC0 __asm__("D_002A8BC0");
extern const STRINGGEN D_002A8BD0 __asm__("D_002A8BD0");
extern const STRINGGEN D_002A8BE0 __asm__("D_002A8BE0");
extern const STRINGGEN D_002A8BF0 __asm__("D_002A8BF0");
extern const STRINGGEN D_002A8C00 __asm__("D_002A8C00");
extern const STRINGGEN D_002A8C10 __asm__("D_002A8C10");
extern const STRINGGEN D_002A8C20 __asm__("D_002A8C20");
extern const STRINGGEN D_002A8C30 __asm__("D_002A8C30");
extern const STRINGGEN D_002A8C40 __asm__("D_002A8C40");
extern const STRINGGEN D_002A8C50 __asm__("D_002A8C50");
extern const STRINGGEN D_002A8C60 __asm__("D_002A8C60");
extern const STRINGGEN D_002A8C70 __asm__("D_002A8C70");
extern const STRINGGEN D_002A8C80 __asm__("D_002A8C80");
extern const STRINGGEN D_002A8C90 __asm__("D_002A8C90");
extern const STRINGGEN D_002A8CA0 __asm__("D_002A8CA0");
extern const STRINGGEN D_002A8CB0 __asm__("D_002A8CB0");
extern const STRINGGEN D_002A8CC0 __asm__("D_002A8CC0");
extern const STRINGGEN D_002A8CD0 __asm__("D_002A8CD0");
extern const STRINGGEN D_002A8CE0 __asm__("D_002A8CE0");
extern const STRINGGEN D_002A8CF0 __asm__("D_002A8CF0");
extern const STRINGGEN D_002A8D00 __asm__("D_002A8D00");
extern const STRINGGEN D_002A8D10 __asm__("D_002A8D10");
extern const STRINGGEN D_002A8D20 __asm__("D_002A8D20");
extern const STRINGGEN D_002A8D30 __asm__("D_002A8D30");
extern const STRINGGEN D_002A8D40 __asm__("D_002A8D40");
extern const STRINGGEN D_002A8D50 __asm__("D_002A8D50");
extern const STRINGGEN D_002A8D60 __asm__("D_002A8D60");
extern const STRINGGEN D_002A8D70 __asm__("D_002A8D70");
extern const STRINGGEN D_002A8D80 __asm__("D_002A8D80");
extern const STRINGGEN D_002A8D90 __asm__("D_002A8D90");
extern const STRINGGEN D_002A8DA0 __asm__("D_002A8DA0");
extern const STRINGGEN D_002A8DB0 __asm__("D_002A8DB0");
extern const STRINGGEN D_002A8DC0 __asm__("D_002A8DC0");
extern const STRINGGEN D_002A8DD0 __asm__("D_002A8DD0");
extern const STRINGGEN D_002A8DE0 __asm__("D_002A8DE0");
extern const STRINGGEN D_002A8DF0 __asm__("D_002A8DF0");
extern const STRINGGEN D_002A8E00 __asm__("D_002A8E00");
extern const STRINGGEN D_002A8E10 __asm__("D_002A8E10");
extern const STRINGGEN D_002A8E20 __asm__("D_002A8E20");
extern const STRINGGEN D_002A8E30 __asm__("D_002A8E30");
extern const STRINGGEN D_002A8E40 __asm__("D_002A8E40");
extern const STRINGGEN D_002A8E50 __asm__("D_002A8E50");
extern const STRINGGEN D_002A8E60 __asm__("D_002A8E60");
extern const STRINGGEN D_002A8E70 __asm__("D_002A8E70");
extern const STRINGGEN D_002A8E80 __asm__("D_002A8E80");
extern const STRINGGEN D_002A8E90 __asm__("D_002A8E90");
extern const STRINGGEN D_002A8EA0 __asm__("D_002A8EA0");
extern const STRINGGEN D_002A8EB0 __asm__("D_002A8EB0");
extern const STRINGGEN D_002A8EC0 __asm__("D_002A8EC0");
extern const STRINGGEN D_002A8ED0 __asm__("D_002A8ED0");
extern const STRINGGEN D_002A8EE0 __asm__("D_002A8EE0");
extern const STRINGGEN D_002A8EF0 __asm__("D_002A8EF0");
extern const STRINGGEN D_002A8F00 __asm__("D_002A8F00");
extern const STRINGGEN D_002A8F10 __asm__("D_002A8F10");
extern const STRINGGEN D_002A8F20 __asm__("D_002A8F20");
extern const STRINGGEN D_002A8F30 __asm__("D_002A8F30");
extern const STRINGGEN D_002A8F40 __asm__("D_002A8F40");
extern const STRINGGEN D_002A8F50 __asm__("D_002A8F50");
extern const STRINGGEN D_002A8F60 __asm__("D_002A8F60");
extern const STRINGGEN D_002A8F70 __asm__("D_002A8F70");
extern const STRINGGEN D_002A8F80 __asm__("D_002A8F80");
extern const STRINGGEN D_002A8F90 __asm__("D_002A8F90");
extern const STRINGGEN D_002A8FA0 __asm__("D_002A8FA0");
extern const STRINGGEN D_002A8FB0 __asm__("D_002A8FB0");
extern const STRINGGEN D_002A8FC0 __asm__("D_002A8FC0");
extern const STRINGGEN D_002A8FD0 __asm__("D_002A8FD0");
extern const STRINGGEN D_002A8FE0 __asm__("D_002A8FE0");
extern const STRINGGEN D_002A8FF0 __asm__("D_002A8FF0");
extern const STRINGGEN D_002A9000 __asm__("D_002A9000");
extern const STRINGGEN D_002A9010 __asm__("D_002A9010");
extern const STRINGGEN D_002A9020 __asm__("D_002A9020");
extern const STRINGGEN D_002A9030 __asm__("D_002A9030");
extern const STRINGGEN D_002A9040 __asm__("D_002A9040");
extern const STRINGGEN D_002A9050 __asm__("D_002A9050");
extern const STRINGGEN D_002A9060 __asm__("D_002A9060");
extern const STRINGGEN D_002A9070 __asm__("D_002A9070");
extern const STRINGGEN D_002A9080 __asm__("D_002A9080");
extern const STRINGGEN D_002A9090 __asm__("D_002A9090");
extern const STRINGGEN D_002A90A0 __asm__("D_002A90A0");
extern const STRINGGEN D_002A90B0 __asm__("D_002A90B0");
extern const STRINGGEN D_002A90C0 __asm__("D_002A90C0");
extern const STRINGGEN D_002A90D0 __asm__("D_002A90D0");
extern const STRINGGEN D_002A90E0 __asm__("D_002A90E0");
extern const STRINGGEN D_002A90F0 __asm__("D_002A90F0");
extern const STRINGGEN D_002A9100 __asm__("D_002A9100");
extern const STRINGGEN D_002A9110 __asm__("D_002A9110");
extern const STRINGGEN D_002A9120 __asm__("D_002A9120");
extern const STRINGGEN D_002A9130 __asm__("D_002A9130");
extern const STRINGGEN D_002A9140 __asm__("D_002A9140");
extern const STRINGGEN D_002A9150 __asm__("D_002A9150");
extern const STRINGGEN D_002A9160 __asm__("D_002A9160");
extern const STRINGGEN D_002A9170 __asm__("D_002A9170");
extern const STRINGGEN D_002A9180 __asm__("D_002A9180");
extern const STRINGGEN D_002A9190 __asm__("D_002A9190");
extern const STRINGGEN D_002A91A0 __asm__("D_002A91A0");
extern const STRINGGEN D_002A91B0 __asm__("D_002A91B0");
extern const STRINGGEN D_002A91C0 __asm__("D_002A91C0");
extern const STRINGGEN D_002A91D0 __asm__("D_002A91D0");
extern const STRINGGEN D_002A91E0 __asm__("D_002A91E0");
extern const STRINGGEN D_002A91F0 __asm__("D_002A91F0");
extern const STRINGGEN D_002A9200 __asm__("D_002A9200");
extern const STRINGGEN D_002A9210 __asm__("D_002A9210");
extern const STRINGGEN D_002A9220 __asm__("D_002A9220");
extern const STRINGGEN D_002A9230 __asm__("D_002A9230");
// The TU's static initializer (generated by g++): the HUD linear heap, a level-heap object and the 108-entry text table.
extern "C" void StaticInit_hud(int initialize,int priority){
 if(priority==0xFFFF&&initialize){
  __11CLinearHeapiPUc(&D_003E1C70,0x60000,D_003E21A0);
  D_003E1C80.vtable=SLY2_VTBL(CSwHeap);D_003E1C80.field4=-1;
  STRINGGEN *destination=D_003E1C88;
  *destination++=D_002A8B90;
  *destination++=D_002A8BA0;
  *destination++=D_002A8BB0;
  *destination++=D_002A8BC0;
  *destination++=D_002A8BD0;
  *destination++=D_002A8BE0;
  *destination++=D_002A8BF0;
  *destination++=D_002A8C00;
  *destination++=D_002A8C10;
  *destination++=D_002A8C20;
  *destination++=D_002A8C30;
  *destination++=D_002A8C40;
  *destination++=D_002A8C50;
  *destination++=D_002A8C60;
  *destination++=D_002A8C70;
  *destination++=D_002A8C80;
  *destination++=D_002A8C90;
  *destination++=D_002A8CA0;
  *destination++=D_002A8CB0;
  *destination++=D_002A8CC0;
  *destination++=D_002A8CD0;
  *destination++=D_002A8CE0;
  *destination++=D_002A8CF0;
  *destination++=D_002A8D00;
  *destination++=D_002A8D10;
  *destination++=D_002A8D20;
  *destination++=D_002A8D30;
  *destination++=D_002A8D40;
  *destination++=D_002A8D50;
  *destination++=D_002A8D60;
  *destination++=D_002A8D70;
  *destination++=D_002A8D80;
  *destination++=D_002A8D90;
  *destination++=D_002A8DA0;
  *destination++=D_002A8DB0;
  *destination++=D_002A8DC0;
  *destination++=D_002A8DD0;
  *destination++=D_002A8DE0;
  *destination++=D_002A8DF0;
  *destination++=D_002A8E00;
  *destination++=D_002A8E10;
  *destination++=D_002A8E20;
  *destination++=D_002A8E30;
  *destination++=D_002A8E40;
  *destination++=D_002A8E50;
  *destination++=D_002A8E60;
  *destination++=D_002A8E70;
  *destination++=D_002A8E80;
  *destination++=D_002A8E90;
  *destination++=D_002A8EA0;
  *destination++=D_002A8EB0;
  *destination++=D_002A8EC0;
  *destination++=D_002A8ED0;
  *destination++=D_002A8EE0;
  *destination++=D_002A8EF0;
  *destination++=D_002A8F00;
  *destination++=D_002A8F10;
  *destination++=D_002A8F20;
  *destination++=D_002A8F30;
  *destination++=D_002A8F40;
  *destination++=D_002A8F50;
  *destination++=D_002A8F60;
  *destination++=D_002A8F70;
  *destination++=D_002A8F80;
  *destination++=D_002A8F90;
  *destination++=D_002A8FA0;
  *destination++=D_002A8FB0;
  *destination++=D_002A8FC0;
  *destination++=D_002A8FD0;
  *destination++=D_002A8FE0;
  *destination++=D_002A8FF0;
  *destination++=D_002A9000;
  *destination++=D_002A9010;
  *destination++=D_002A9020;
  *destination++=D_002A9030;
  *destination++=D_002A9040;
  *destination++=D_002A9050;
  *destination++=D_002A9060;
  *destination++=D_002A9070;
  *destination++=D_002A9080;
  *destination++=D_002A9090;
  *destination++=D_002A90A0;
  *destination++=D_002A90B0;
  *destination++=D_002A90C0;
  *destination++=D_002A90D0;
  *destination++=D_002A90E0;
  *destination++=D_002A90F0;
  *destination++=D_002A9100;
  *destination++=D_002A9110;
  *destination++=D_002A9120;
  *destination++=D_002A9130;
  *destination++=D_002A9140;
  *destination++=D_002A9150;
  *destination++=D_002A9160;
  *destination++=D_002A9170;
  *destination++=D_002A9180;
  *destination++=D_002A9190;
  *destination++=D_002A91A0;
  *destination++=D_002A91B0;
  *destination++=D_002A91C0;
  *destination++=D_002A91D0;
  *destination++=D_002A91E0;
  *destination++=D_002A91F0;
  *destination++=D_002A9200;
  *destination++=D_002A9210;
  *destination++=D_002A9220;
  *destination++=D_002A9230;
  STRINGGEN special;
  special.id=0x5C1;special.get=BuildContextText;special.context=D_002C40A0;
  *destination=special;
  if(++D_002DD2E0==1)InitSlotheap(&D_004520A8,0x14,0x2000,0x10);
  if(++D_002DD2E4==1)InitSlotheap(&D_004520D0,0x18,0x800,0x10);
  if(++D_002DD2E8==1)InitSlotheap(&D_004520F8,0x28,0x200,0x10);
  if(++D_002DD2EC==1)InitSlotheap(&D_00452120,0x1C,0x800,0x10);
  if(++D_002DD2F0==1)InitSlotheap(&D_00452148,0xC,0xC8,0x10);
  if(++D_002DD2F4==1)InitSlotheap(&D_00452170,0x14,0x100,0x10);
 }
}
} // namespace M_StaticInit_hud

// 0x21BB68 func_0021BB68
/* _GLOBAL_$I$ stub: runs the static initializer above. */
// The _GLOBAL_$I$ stub g++ generates for this TU: runs StaticInit_hud.
extern "C" void func_0021BB68(void)
{
    StaticInit_hud(1, 0xFFFF);
}


// ---- Data owned by this TU, in address order (docs/guides/tu-merging.md "Data"). Defined after the functions
// so no function sees a definition (keeps the code as matched); the namespaced declarations above refer to these.
extern const char D_002A8B80[] = "Unknown";
extern const char D_002A8B88[] = "     ";








// ---- Pointer constants and records, by address name (docs/guides/tu-merging.md "Data"): words as the ELF has
// them, pointers through asm-labelled declarations. Non-virtual pointer-to-member constants are 8 bytes
// {delta 0, index -1, pfn}; the definitions sit in their own namespace so no declaration above has to agree.
namespace ANPTR_hud {
void p_BuildClueCountText() __asm__("BuildClueCountText");
void p_BuildCoinCountText() __asm__("BuildCoinCountText");
void p_BuildContextText() __asm__("BuildContextText");
void p_BuildGadgetMenuInfoText() __asm__("BuildGadgetMenuInfoText");
void p_BuildJobListRowText() __asm__("BuildJobListRowText");
void p_BuildJobListTaskRowText() __asm__("BuildJobListTaskRowText");
void p_BuildJobListWorldText() __asm__("BuildJobListWorldText");
void p_BuildReconSelectionText() __asm__("BuildReconSelectionText");
void p_BuildReconTargetText() __asm__("BuildReconTargetText");
void p_BuildThiefnetInfoText() __asm__("BuildThiefnetInfoText");
void p_BuildThiefnetLootNameText() __asm__("BuildThiefnetLootNameText");
void p_BuildThiefnetPowerupNameText() __asm__("BuildThiefnetPowerupNameText");
void p_BuildTickerText() __asm__("BuildTickerText");
void p_BuildTimerText() __asm__("BuildTimerText");
void p_BuildTitleCompletionText() __asm__("BuildTitleCompletionText");
void p_BuildTitlePromptText() __asm__("BuildTitlePromptText");
void p_BuildWidgetArgText() __asm__("BuildWidgetArgText");
void p_FormatEpisodePercent() __asm__("FormatEpisodePercent");
void p_FormatEpisodePlayTime() __asm__("FormatEpisodePlayTime");
void p_func_0015DA88() __asm__("FormatEpisodeMenuPageText");
void p_func_001AD7E0() __asm__("FormatMcCardNameText");
void p_func_001AD820() __asm__("FormatMcDialogText");
void p_func_001AD928() __asm__("FormatSavePageTextA");
void p_func_001AD980() __asm__("FormatSaveCompletionText");
void p_func_001ADA20() __asm__("FormatSavePageTextB");
void p_func_001ADBB8() __asm__("FormatSaveTimeText");
void p_func_001ADC40() __asm__("FormatSaveSlotSummaryText");
void p_func_001DC820() __asm__("BuildThiefnetPowerupCostText");
void p_func_001DC9C0() __asm__("BuildThiefnetLootPriceText");
void p_func_001DCA30() __asm__("BuildThiefnetPromptText");
void p_func_001DDE10() __asm__("BuildJobDescriptionText");
void p_func_001DDE58() __asm__("BuildJobNameText");
void p_func_001DDF60() __asm__("BuildTitleScreenButtonText");
void p_func_00210FE0() __asm__("BuildBinocularReadoutText");
void p_func_00211298() __asm__("BuildMenuCarouselText");
void p_func_00211390() __asm__("BuildSubtitleText");
void p_func_002113B8() __asm__("BuildSubtitleButtonText");
void p_func_00211420() __asm__("BuildHeldItemText");
void p_func_002155C0() __asm__("BuildSpritePictureTexPacket");
extern char d_D_002C3EC0[] __asm__("D_002C3EC0");

extern const unsigned int D_002A8B90[4] __asm__("D_002A8B90") = {0x2CF, (unsigned int)p_BuildCoinCountText, (unsigned int)&g_sw.psvpCcoin, 0};
extern const unsigned int D_002A8BA0[4] __asm__("D_002A8BA0") = {0x2D0, (unsigned int)p_BuildClueCountText, 0, 0};
extern const unsigned int D_002A8BB0[4] __asm__("D_002A8BB0") = {0x2D1, (unsigned int)p_BuildTimerText, 0, 0};
extern const unsigned int D_002A8BC0[4] __asm__("D_002A8BC0") = {0x2D2, (unsigned int)p_func_00211298, 0, 0};
extern const unsigned int D_002A8BD0[4] __asm__("D_002A8BD0") = {0x2D3, (unsigned int)p_func_00211298, 1, 0};
extern const unsigned int D_002A8BE0[4] __asm__("D_002A8BE0") = {0x2D4, (unsigned int)p_func_00211298, 2, 0};
extern const unsigned int D_002A8BF0[4] __asm__("D_002A8BF0") = {0x2D5, (unsigned int)p_func_00211298, 3, 0};
extern const unsigned int D_002A8C00[4] __asm__("D_002A8C00") = {0x2D6, (unsigned int)p_func_00211298, 4, 0};
extern const unsigned int D_002A8C10[4] __asm__("D_002A8C10") = {0x2D7, (unsigned int)p_func_00211298, 5, 0};
extern const unsigned int D_002A8C20[4] __asm__("D_002A8C20") = {0x2D8, (unsigned int)p_func_00211298, 6, 0};
extern const unsigned int D_002A8C30[4] __asm__("D_002A8C30") = {0x2D9, (unsigned int)p_func_00211298, 7, 0};
extern const unsigned int D_002A8C40[4] __asm__("D_002A8C40") = {0x2DA, (unsigned int)p_func_00211298, 8, 0};
extern const unsigned int D_002A8C50[4] __asm__("D_002A8C50") = {0x2DB, (unsigned int)p_func_00211298, 9, 0};
extern const unsigned int D_002A8C60[4] __asm__("D_002A8C60") = {0x2DC, (unsigned int)p_func_00210FE0, 0, 0};
extern const unsigned int D_002A8C70[4] __asm__("D_002A8C70") = {0x2DD, (unsigned int)p_BuildReconSelectionText, 0, 0};
extern const unsigned int D_002A8C80[4] __asm__("D_002A8C80") = {0x2DE, (unsigned int)p_BuildReconSelectionText, 1, 0};
extern const unsigned int D_002A8C90[4] __asm__("D_002A8C90") = {0x2DF, (unsigned int)p_BuildReconTargetText, 0, 0};
extern const unsigned int D_002A8CA0[4] __asm__("D_002A8CA0") = {0x2E0, (unsigned int)p_BuildReconTargetText, 1, 0};
extern const unsigned int D_002A8CB0[4] __asm__("D_002A8CB0") = {0x2E1, (unsigned int)p_BuildReconTargetText, 2, 0};
extern const unsigned int D_002A8CC0[4] __asm__("D_002A8CC0") = {0x2E2, (unsigned int)p_BuildReconTargetText, 3, 0};
extern const unsigned int D_002A8CD0[4] __asm__("D_002A8CD0") = {0x2E3, (unsigned int)p_BuildReconTargetText, 4, 0};
extern const unsigned int D_002A8CE0[4] __asm__("D_002A8CE0") = {0x2E4, (unsigned int)p_BuildReconTargetText, 5, 0};
extern const unsigned int D_002A8CF0[4] __asm__("D_002A8CF0") = {0x2E5, (unsigned int)p_BuildWidgetArgText, 0, 0};
extern const unsigned int D_002A8D00[4] __asm__("D_002A8D00") = {0x2E6, (unsigned int)p_BuildWidgetArgText, 1, 0};
extern const unsigned int D_002A8D10[4] __asm__("D_002A8D10") = {0x2E7, (unsigned int)p_BuildWidgetArgText, 2, 0};
extern const unsigned int D_002A8D20[4] __asm__("D_002A8D20") = {0x2E8, (unsigned int)p_BuildWidgetArgText, 3, 0};
extern const unsigned int D_002A8D30[4] __asm__("D_002A8D30") = {0x2E9, (unsigned int)p_BuildWidgetArgText, 4, 0};
extern const unsigned int D_002A8D40[4] __asm__("D_002A8D40") = {0x2EA, (unsigned int)p_BuildWidgetArgText, 5, 0};
extern const unsigned int D_002A8D50[4] __asm__("D_002A8D50") = {0x2EB, (unsigned int)p_BuildWidgetArgText, 6, 0};
extern const unsigned int D_002A8D60[4] __asm__("D_002A8D60") = {0x2EC, (unsigned int)p_BuildWidgetArgText, 7, 0};
extern const unsigned int D_002A8D70[4] __asm__("D_002A8D70") = {0x2ED, (unsigned int)p_BuildWidgetArgText, 8, 0};
extern const unsigned int D_002A8D80[4] __asm__("D_002A8D80") = {0x2EE, (unsigned int)p_BuildWidgetArgText, 9, 0};
extern const unsigned int D_002A8D90[4] __asm__("D_002A8D90") = {0x2FF, (unsigned int)p_BuildContextText, (unsigned int)d_D_002C3EC0, 0};
extern const unsigned int D_002A8DA0[4] __asm__("D_002A8DA0") = {0x300, (unsigned int)p_BuildContextText, (unsigned int)(d_D_002C3EC0 + 0x30), 0};
extern const unsigned int D_002A8DB0[4] __asm__("D_002A8DB0") = {0x301, (unsigned int)p_BuildContextText, (unsigned int)(d_D_002C3EC0 + 0x60), 0};
extern const unsigned int D_002A8DC0[4] __asm__("D_002A8DC0") = {0x302, (unsigned int)p_BuildContextText, (unsigned int)(d_D_002C3EC0 + 0x90), 0};
extern const unsigned int D_002A8DD0[4] __asm__("D_002A8DD0") = {0x303, (unsigned int)p_BuildContextText, (unsigned int)(d_D_002C3EC0 + 0xC0), 0};
extern const unsigned int D_002A8DE0[4] __asm__("D_002A8DE0") = {0x304, (unsigned int)p_BuildContextText, (unsigned int)(d_D_002C3EC0 + 0xF0), 0};
extern const unsigned int D_002A8DF0[4] __asm__("D_002A8DF0") = {0x305, (unsigned int)p_BuildContextText, (unsigned int)(d_D_002C3EC0 + 0x120), 0};
extern const unsigned int D_002A8E00[4] __asm__("D_002A8E00") = {0x306, (unsigned int)p_BuildContextText, (unsigned int)(d_D_002C3EC0 + 0x150), 0};
extern const unsigned int D_002A8E10[4] __asm__("D_002A8E10") = {0x307, (unsigned int)p_BuildContextText, (unsigned int)(d_D_002C3EC0 + 0x180), 0};
extern const unsigned int D_002A8E20[4] __asm__("D_002A8E20") = {0x308, (unsigned int)p_BuildContextText, (unsigned int)(d_D_002C3EC0 + 0x1B0), 0};
extern const unsigned int D_002A8E30[4] __asm__("D_002A8E30") = {0x2EF, (unsigned int)p_func_00211390, 0, 0};
extern const unsigned int D_002A8E40[4] __asm__("D_002A8E40") = {0x2F0, (unsigned int)p_func_002113B8, 0, 0};
extern const unsigned int D_002A8E50[4] __asm__("D_002A8E50") = {0x37B, (unsigned int)p_BuildGadgetMenuInfoText, 0, 0};
extern const unsigned int D_002A8E60[4] __asm__("D_002A8E60") = {0x37A, (unsigned int)p_BuildGadgetMenuInfoText, 0, 0};
extern const unsigned int D_002A8E70[4] __asm__("D_002A8E70") = {0x43C, (unsigned int)p_BuildThiefnetPowerupNameText, 0, 0};
extern const unsigned int D_002A8E80[4] __asm__("D_002A8E80") = {0x43D, (unsigned int)p_BuildThiefnetPowerupNameText, 1, 0};
extern const unsigned int D_002A8E90[4] __asm__("D_002A8E90") = {0x43E, (unsigned int)p_BuildThiefnetPowerupNameText, 2, 0};
extern const unsigned int D_002A8EA0[4] __asm__("D_002A8EA0") = {0x43F, (unsigned int)p_BuildThiefnetPowerupNameText, 3, 0};
extern const unsigned int D_002A8EB0[4] __asm__("D_002A8EB0") = {0x440, (unsigned int)p_BuildThiefnetPowerupNameText, 4, 0};
extern const unsigned int D_002A8EC0[4] __asm__("D_002A8EC0") = {0x441, (unsigned int)p_BuildThiefnetPowerupNameText, 5, 0};
extern const unsigned int D_002A8ED0[4] __asm__("D_002A8ED0") = {0x442, (unsigned int)p_func_001DC820, 0, 0};
extern const unsigned int D_002A8EE0[4] __asm__("D_002A8EE0") = {0x443, (unsigned int)p_func_001DC820, 1, 0};
extern const unsigned int D_002A8EF0[4] __asm__("D_002A8EF0") = {0x444, (unsigned int)p_func_001DC820, 2, 0};
extern const unsigned int D_002A8F00[4] __asm__("D_002A8F00") = {0x445, (unsigned int)p_func_001DC820, 3, 0};
extern const unsigned int D_002A8F10[4] __asm__("D_002A8F10") = {0x446, (unsigned int)p_func_001DC820, 4, 0};
extern const unsigned int D_002A8F20[4] __asm__("D_002A8F20") = {0x447, (unsigned int)p_func_001DC820, 5, 0};
extern const unsigned int D_002A8F30[4] __asm__("D_002A8F30") = {0x436, (unsigned int)p_func_001DCA30, 0, 0};
extern const unsigned int D_002A8F40[4] __asm__("D_002A8F40") = {0x435, (unsigned int)p_BuildThiefnetInfoText, 0, 0};
extern const unsigned int D_002A8F50[4] __asm__("D_002A8F50") = {0x448, (unsigned int)p_BuildThiefnetLootNameText, 0, 0};
extern const unsigned int D_002A8F60[4] __asm__("D_002A8F60") = {0x449, (unsigned int)p_BuildThiefnetLootNameText, 1, 0};
extern const unsigned int D_002A8F70[4] __asm__("D_002A8F70") = {0x44A, (unsigned int)p_BuildThiefnetLootNameText, 2, 0};
extern const unsigned int D_002A8F80[4] __asm__("D_002A8F80") = {0x44B, (unsigned int)p_BuildThiefnetLootNameText, 3, 0};
extern const unsigned int D_002A8F90[4] __asm__("D_002A8F90") = {0x44C, (unsigned int)p_BuildThiefnetLootNameText, 4, 0};
extern const unsigned int D_002A8FA0[4] __asm__("D_002A8FA0") = {0x44D, (unsigned int)p_BuildThiefnetLootNameText, 5, 0};
extern const unsigned int D_002A8FB0[4] __asm__("D_002A8FB0") = {0x44E, (unsigned int)p_func_001DC9C0, 0, 0};
extern const unsigned int D_002A8FC0[4] __asm__("D_002A8FC0") = {0x44F, (unsigned int)p_func_001DC9C0, 1, 0};
extern const unsigned int D_002A8FD0[4] __asm__("D_002A8FD0") = {0x450, (unsigned int)p_func_001DC9C0, 2, 0};
extern const unsigned int D_002A8FE0[4] __asm__("D_002A8FE0") = {0x451, (unsigned int)p_func_001DC9C0, 3, 0};
extern const unsigned int D_002A8FF0[4] __asm__("D_002A8FF0") = {0x452, (unsigned int)p_func_001DC9C0, 4, 0};
extern const unsigned int D_002A9000[4] __asm__("D_002A9000") = {0x453, (unsigned int)p_func_001DC9C0, 5, 0};
extern const unsigned int D_002A9010[4] __asm__("D_002A9010") = {0x2F1, (unsigned int)p_BuildTickerText, 0, 0};
extern const unsigned int D_002A9020[4] __asm__("D_002A9020") = {0x51B, (unsigned int)p_BuildJobListWorldText, 0, 0};
extern const unsigned int D_002A9030[4] __asm__("D_002A9030") = {0x51C, (unsigned int)p_func_001DDE58, 0, 0};
extern const unsigned int D_002A9040[4] __asm__("D_002A9040") = {0x51D, (unsigned int)p_func_001DDE10, 0, 0};
extern const unsigned int D_002A9050[4] __asm__("D_002A9050") = {0x51E, (unsigned int)p_BuildJobListRowText, 0, 0};
extern const unsigned int D_002A9060[4] __asm__("D_002A9060") = {0x51F, (unsigned int)p_BuildJobListRowText, 1, 0};
extern const unsigned int D_002A9070[4] __asm__("D_002A9070") = {0x520, (unsigned int)p_BuildJobListRowText, 2, 0};
extern const unsigned int D_002A9080[4] __asm__("D_002A9080") = {0x521, (unsigned int)p_BuildJobListRowText, 3, 0};
extern const unsigned int D_002A9090[4] __asm__("D_002A9090") = {0x522, (unsigned int)p_BuildJobListRowText, 4, 0};
extern const unsigned int D_002A90A0[4] __asm__("D_002A90A0") = {0x523, (unsigned int)p_BuildJobListRowText, 5, 0};
extern const unsigned int D_002A90B0[4] __asm__("D_002A90B0") = {0x524, (unsigned int)p_BuildJobListRowText, 6, 0};
extern const unsigned int D_002A90C0[4] __asm__("D_002A90C0") = {0x525, (unsigned int)p_BuildJobListRowText, 7, 0};
extern const unsigned int D_002A90D0[4] __asm__("D_002A90D0") = {0x526, (unsigned int)p_BuildJobListRowText, 8, 0};
extern const unsigned int D_002A90E0[4] __asm__("D_002A90E0") = {0x527, (unsigned int)p_BuildJobListRowText, 9, 0};
extern const unsigned int D_002A90F0[4] __asm__("D_002A90F0") = {0x528, (unsigned int)p_BuildJobListTaskRowText, 0, 0};
extern const unsigned int D_002A9100[4] __asm__("D_002A9100") = {0x529, (unsigned int)p_BuildJobListTaskRowText, 1, 0};
extern const unsigned int D_002A9110[4] __asm__("D_002A9110") = {0x52A, (unsigned int)p_BuildJobListTaskRowText, 2, 0};
extern const unsigned int D_002A9120[4] __asm__("D_002A9120") = {0x52B, (unsigned int)p_BuildJobListTaskRowText, 3, 0};
extern const unsigned int D_002A9130[4] __asm__("D_002A9130") = {0x2F2, (unsigned int)p_func_001AD7E0, 0, 0};
extern const unsigned int D_002A9140[4] __asm__("D_002A9140") = {0x2F3, (unsigned int)p_func_001AD820, 0, 0};
extern const unsigned int D_002A9150[4] __asm__("D_002A9150") = {0x5CB, (unsigned int)p_func_001AD928, 0, 0};
extern const unsigned int D_002A9160[4] __asm__("D_002A9160") = {0x5CC, (unsigned int)p_func_001AD980, 0, 0};
extern const unsigned int D_002A9170[4] __asm__("D_002A9170") = {0x5CD, (unsigned int)p_func_001ADA20, 0, 0};
extern const unsigned int D_002A9180[4] __asm__("D_002A9180") = {0x5CE, (unsigned int)p_func_001ADBB8, 0, 0};
extern const unsigned int D_002A9190[4] __asm__("D_002A9190") = {0x5CF, (unsigned int)p_func_001ADC40, 0, 0};
extern const unsigned int D_002A91A0[4] __asm__("D_002A91A0") = {0x5D0, (unsigned int)p_func_001ADC40, 1, 0};
extern const unsigned int D_002A91B0[4] __asm__("D_002A91B0") = {0x5D1, (unsigned int)p_func_001ADC40, 2, 0};
extern const unsigned int D_002A91C0[4] __asm__("D_002A91C0") = {0x5D3, (unsigned int)p_BuildTitlePromptText, 0, 0};
extern const unsigned int D_002A91D0[4] __asm__("D_002A91D0") = {0x5D7, (unsigned int)p_func_001DDF60, 0, 0};
extern const unsigned int D_002A91E0[4] __asm__("D_002A91E0") = {0x5D5, (unsigned int)p_BuildTitleCompletionText, 0, 0};
extern const unsigned int D_002A91F0[4] __asm__("D_002A91F0") = {0x52F, (unsigned int)p_FormatEpisodePercent, 0, 0};
extern const unsigned int D_002A9200[4] __asm__("D_002A9200") = {0x530, (unsigned int)p_FormatEpisodePlayTime, 0, 0};
extern const unsigned int D_002A9210[4] __asm__("D_002A9210") = {0x2F4, (unsigned int)p_func_00211420, 0, 0};
extern const unsigned int D_002A9220[4] __asm__("D_002A9220") = {0x5BF, (unsigned int)p_func_0015DA88, 1, 0};
extern const unsigned int D_002A9230[4] __asm__("D_002A9230") = {0x5C0, (unsigned int)p_func_0015DA88, 0, 0};

} // namespace ANPTR_hud

// ---- hud's .data in address order (tu-merging.md "Phase 6: one link mode"): one block after the functions (definition order is emission order); the declarations above are what the functions see.
char D_002C3EC0[10][0x30] = {"Widget Item 0", "Widget Item 1", "Widget Item 2", "Widget Item 3", "Widget Item 4",
    "Widget Item 5", "Widget Item 6", "Widget Item 7", "Widget Item 8", "Widget Item 9"};
extern "C" { void *D_002C40A0 = (void *)RO_ORDER::r_D_002A8AE0; }   /* the string StaticInit_hud reads (0x2A8AE0) */
int D_002C40A4 = 0;
float D_002C40A8 __attribute__((aligned(8))) = 788096.0f;
float D_002C40AC = 788864.0f;
float D_002C40B0 = 788240.0f;
float D_002C40B4 = 788720.0f;
float D_002C40B8 = 0.25f;

// ---- hud's .bss in address order (tu-merging.md "Data": .bss gaps): 0x3E1C70..0x4425D0, the first .bss after sw's
// in TU order. StaticInit_hud keeps its hand-written form and constructs the first three objects at run time, so
// they are defined here as constructor-less storage of their layout (a constructor would make g++ generate an
// initializer of its own): the CLinearHeap (LINEAR view, 0x10), the level-heap object (HEAP view, 8) and the
// 108-entry STRINGGEN table (12-byte records). The heap's 0x60000-byte buffer is a qword array (16-aligned, as
// 0x3E21A0 after the table's end 0x3E2198 shows), then the buffers only hud names (former commons; sizes are the
// gaps to the next name, 16-aligned qword arrays). Uninitialized objects without a constructor are emitted at the
// end of the file in first-declaration order: the asm-labelled names below are declared nowhere else, so their
// order is this block's; what the functions see are the extern declarations above.
namespace BSS_hud {
struct STRINGGENREC { int id; void (*get)(char *, void *, void *); void *context; };
M_StaticInit_hud::LINEAR b_003E1C70 __asm__("D_003E1C70");
M_StaticInit_hud::HEAP b_003E1C80 __asm__("D_003E1C80");
STRINGGENREC b_003E1C88[108] __asm__("D_003E1C88");
qword b_003E21A0[0x60000 / 16] __asm__("D_003E21A0");
qword b_004421A0 __asm__("D_004421A0");
qword b_004421B0[6] __asm__("D_004421B0");
qword b_00442210[3] __asm__("D_00442210");
qword b_00442240[39] __asm__("D_00442240");
qword b_004424B0[2] __asm__("D_004424B0");
qword b_004424D0[2] __asm__("D_004424D0");
qword b_004424F0[3] __asm__("D_004424F0");
qword b_00442520[4] __asm__("D_00442520");
qword b_00442560[4] __asm__("D_00442560");
qword b_004425A0[3] __asm__("D_004425A0");
}

// ---- hud's named constant in the first part of .rodata (0x2A3A10, after sw's world-name table; tu-merging.md
// "Phase 6", .rodata.early as cd/menu/sw): the identity M23 {1, 0, 0, 1, 0, 0} (padded to 0x20) the widget and sprite
// code start from. Under an asm label so that the declarations above (char[], M23) need not agree.
namespace RO_EARLY_HUD {
extern const float r_D_002A3A10[8] __asm__("D_002A3A10") __attribute__((section(".rodata.early"), aligned(16))) =
    {1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f};
}
