// hud.cpp -- WIDGET HUD objects, sprite groups and the STRINGGEN text table (docs/engine/hud-ui.md section 1).
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
#include "sly2/cm.h"
#include "sly2/sw.h"
#include "sly2/scrap.h"
#include "sly2/classes.h"
extern "C" char base_D_003DA0C0[] __asm__("D_003DA0C0");   /* base of the address names below: see docs/guides/tu-merging.md "Data" */
extern "C" char base_D_004435F0[] __asm__("D_004435F0");   /* g_subtitle (tu_2275B8), base of D_004435F4 */

/* WIDGET::state values. */
enum WIDGET_STATE
{
    WIDGET_STATE_UNLOADED = 0,
    WIDGET_STATE_HIDDEN = 1,
    WIDGET_STATE_VISIBLE = 2,
    WIDGET_STATE_APPEARING = 3,
    WIDGET_STATE_DISAPPEARING = 4,
};
/* WIDGET::request values. */
enum WIDGET_REQUEST
{
    WIDGET_REQUEST_NONE = 0,
    WIDGET_REQUEST_HIDE = 1,
    WIDGET_REQUEST_SHOW = 2,
};

/* The WIDGET (CID 0x7E, an LO, 0x110 bytes) as the file-scope functions read it. */
struct WIDGETV : public LO
{
    char pad50[0x8];
    int state;             /* +0x58 0 unloaded, 1 hidden, 2 visible, 3 appearing, 4 disappearing */
    float stateTime;           /* +0x5C time of the last state change */
    char pad60[0x4];
    int request;           /* +0x64 1 hide, 2 show */
    float requestTime;         /* +0x68 */
    char pad6C[0x4];
    int loopAnimId;         /* +0x70 */
    int showAnimId;         /* +0x74 */
    int hideAnimId;         /* +0x78 */
    int mainAnimId;         /* +0x7C */
    char pad80[0x28];
    int isNonExclusive;                /* +0xA8 clear: kept on the exclusive list D_003E1060 */
};
/* A sprite group (0x30 bytes) as DrawSprite..GetSpriteMainState read it: the 0x18-byte animations at +4, the
   0x18-byte playback slots at +8 (slot 0 plays the group's main track), an enable flag at +0x28. */
struct SPRITEITEM { char pad0[0x18]; };   /* an animation */
/* a playback slot: the animation playing (-1 none), the key, the time, whether it finished (or holds a seeked
   time), its sound */
struct SPRITEENTRY { char pad0[0x8]; short animIndex; short keyIndex; float time; int isFinished; void *sound; };
struct SPRITEGRP
{
    unsigned short numAnims, numSlots;
    SPRITEITEM *anims;      /* +0x04 */
    SPRITEENTRY *slots;     /* +0x08 */
    char padC[0x1C];
    int isEnabled;          /* +0x28 */
};

extern SW D_003DD490;           /* g_sw */
extern CLOCK D_002DDED0;        /* g_clock */
extern char D_004435F0[];       /* speaker label (HYPOTHESIS, as letterbox.cpp) */
#undef D_003DA0D8
#define D_003DA0D8 (*(int *)((char *)base_D_003DA0C0 + 0x18))   /* screen's object+0x18 */   /* ticker text id */
extern "C" char D_000000C0[];         /* VU1 microprogram entry at 0xC0 */
char *FindSwLocalizedString(SW *psw, int id);
int IsBasicDerivedFrom(BASIC *pbasic, CID cid);
int FFindDlEntry(DL *pdl, void *pv);
void AppendDlEntry(DL *pdl, void *pv);
void RemoveDlEntry(DL *pdl, void *pv);
extern "C" {
char *strcpy(char *, const char *);
PO *GetHeroPo(SW *psw);
char *GetReconPendingText(int *pn);
char *GetReconSelectedText(int *pn);
void GetSubtitleText(char *pchz, int n);
void FormatSubtitleButtonGlyphs(char *pchz, int n);
void SetWidgetRequest(WIDGETV *widget, int request);
void SetWidgetStateW(WIDGETV *widget, int state) __asm__("SetWidgetState");
int FWidgetObscured(WIDGETV *widget);
void DrawSpriteItems(SPRITEITEM *anim, int slot, int drawFlags);
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

/* Class ids tested by the text getters (docs/reference/class-tree.md). */
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
/** @sly2-annot addr=0x210EB0
 * Purpose: STRINGGEN getter: strcpy(pchz, context)
 * Info: proposed name BuildContextText (T1); TU hud
 * Notes: names_v2 v1 proposed AddBtnOffAseg__FP3BTNP3ALO3OID (0.000)
 * Confidence: mid (draft) [T1 0.75] - evidence: static naming pass; sources: r7c_hud
 * See: docs/engine/hud-ui.md#4-source-files, docs/engine/hud-ui.md#5-key-functions,
 *   docs/engine/world-sw.md#9-source-files
 * @end-sly2-annot */
/* STRINGGEN getter: the context is the text itself. */
// Text-table getter: copies the context string given by the caller.
extern "C" void BuildContextText(char *pchz, char *pchzContext)
{
    strcpy(pchz, pchzContext);
}

// 0x210ED8 BuildCoinCountText
namespace M_BuildCoinCountText { extern "C" {
struct CRef { int tagk; int tag; };
extern char D_003D4A60[];
extern char D_002A8AA0[];
void RefGetGlobalSaveVar(struct CRef *r, int *p, void *q);
void Decref__4CRef(struct CRef *r);
void fiprintf(void *f, const char *fmt, ...) __asm__("sprintf");
// 0x210ED0: dead-code scrap (sly2/scrap.h)
SLY2_SCRAP("addiu $sp, $sp, 0x10");

/** @sly2-annot addr=0x210ED8
 * Purpose: STRINGGEN id 0x2CF: sprintf("%d", saved var loot.ccoin) = HUD coin counter text
 * Info: proposed name BuildCoinCountText (T1); TU hud
 * Confidence: high [T1 0.85] - evidence: static analysis; sources: r7c_hud
 * See: docs/engine/hud-ui.md#1-widget-hud-objects-hud-tu-0x210eb0-0x21bb88-file-name-unknown,
 *   docs/engine/hud-ui.md#5-key-functions, docs/reference/functions/hud-ui.md#hud-0x210eb0-0x21bb88
 * @end-sly2-annot */
// HUD text 0x2CF: the player's coin count.
void BuildCoinCountText(void *f, int **pp)
{
    int *p = *pp;
    if (p && *p) {
        char *fmt = D_002A8AA0;
        struct CRef r;
        int v;
        RefGetGlobalSaveVar(&r, p, D_003D4A60);
        v = r.tag;
        Decref__4CRef(&r);
        fiprintf(f, fmt, v);
    }
}
} } // namespace M_BuildCoinCountText

// 0x210F50 BuildClueCountText
namespace M_BuildClueCountText { extern "C" {
extern struct G { char pad[0x4764]; int a; int b; } D_003DD490;
extern char D_002A8AA8[];
void fiprintf(void *f, const char *fmt, ...) __asm__("sprintf");
/** @sly2-annot addr=0x210F50
 * Purpose: STRINGGEN id 0x2D0: sprintf("%d/%d", SW+0x4764, SW+0x4768) = clue bottles collected/total
 * Info: proposed name BuildClueCountText (T1); TU hud
 * Notes: names_v2 v1 proposed AddButtonOffAseg__FP6BUTTON3OID (0.000) | runtime: SW+0x4764/+0x4768 are clue
 *   collected/total counters
 * Confidence: high [T1 0.80] - evidence: runtime (save-state RAM); static naming pass; sources:
 *   r7c_hud+runtime-corrected
 * See: docs/engine/hud-ui.md#5-key-functions, docs/reference/functions/hud-ui.md#hud-0x210eb0-0x21bb88
 * @end-sly2-annot */
// HUD text 0x2D0: clue bottles collected / total in this level.
void BuildClueCountText(void *f)
{
    fiprintf(f, D_002A8AA8, D_003DD490.a, D_003DD490.b);
}
} } // namespace M_BuildClueCountText

// 0x210F80 BuildTimerText
namespace M_BuildTimerText { extern "C" {
extern struct CLOCK { float t; float dt; } D_002DDED0;
extern char D_002A8AB0[];
void fiprintf(void *f, const char *fmt, ...) __asm__("sprintf");
/** @sly2-annot addr=0x210F80
 * Purpose: STRINGGEN id 0x2D1: countdown "%d:%02d" of max(0, SW+0x476C - g_clock.t)
 * Info: proposed name BuildTimerText (T1); TU hud
 * Confidence: high [T1 0.85] - evidence: static analysis; sources: r7c_hud+verdicts_followups
 * See: docs/engine/hud-ui.md#5-key-functions, docs/reference/functions/hud-ui.md#hud-0x210eb0-0x21bb88
 * @end-sly2-annot */
// HUD text 0x2D1: the mission countdown as minutes:seconds (never below zero).
void BuildTimerText(void *f)
{
    float t = -D_002DDED0.t + g_sw.tHudTimerEnd; int n; t = (t < 0.0f) ? 0.0f : t;
    n = (int)t;
    {
        int q = n / 60;
        fiprintf(f, D_002A8AB0, q, n - q * 60);
    }
}
} } // namespace M_BuildTimerText

// 0x210FE0 BuildBinocularReadoutText
namespace M_func_00210FE0 { extern "C" {
struct X { char pad[0xF20]; int modeKind; char padf24[0x18]; float hudZoom; };   /* STEPPLAYER jtmode.nKind (0 look, 1 zoom), jtmode.hud.uZoom */
extern char D_003DD490[];
extern char D_002A8AB8[], D_002A8AC0[], D_002A8AC8[], D_002A8AD0[], D_002A8AD8[];
struct X *GetHeroPo(void *);
int IsBasicDerivedFrom__FP5BASIC3CID(struct X *, int);
int sprintf(char *, char *, ...);
int fptodp(float);
/** @sly2-annot addr=0x210FE0
 * Purpose: STRINGGEN id 0x2DC: per-character readout from STEPPLAYER F20/F3C: JT "x%d", BENTLEY "x%5.3f",
 *   MURRAY "Far"/"Near"/"YIKES"
 * Info: proposed name BuildBinocReadoutText (T2); TU hud
 * Confidence: low (draft) [T2 0.45] - evidence: static analysis; sources: r7c_hud
 * See: docs/engine/player.md#8-look-mode-recon-mode-and-the-ai-partner,
 *   docs/reference/functions/hud-ui.md#hud-0x210eb0-0x21bb88
 * @end-sly2-annot */
// HUD text 0x2DC: the binocucom readout, per character (Sly: zoom "x%d", Bentley: "x%5.3f", Murray: distance words).
// (inferred)
void BuildBinocularReadoutText(char *text)
{
    struct X *hero = GetHeroPo(D_003DD490);
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
} } // namespace M_func_00210FE0

// 0x211148 BuildReconSelectionText
/** @sly2-annot addr=0x211148
 * Purpose: STRINGGEN ids 0x2DD/0x2DE: text of recon selector selected id (ctx 0 -> 0x202730, P+0xBC) or
 *   pending caption (ctx 1 -> 0x2026F8, P+0xC0)
 * Info: proposed name BuildReconSelectionText (T1); TU hud
 * Confidence: mid (draft) [T1 0.50] - evidence: static analysis; sources: r7c_hud
 * See: docs/reference/functions/hud-ui.md#hud-0x210eb0-0x21bb88
 * @end-sly2-annot */
/* STRINGGEN getter (context 0 selected, 1 pending): the recon text of the hero's mode HUD, if the hero is a JT. */
// HUD texts 0x2DD / 0x2DE: the recon selector's selected item, or its pending caption.
extern "C" void BuildReconSelectionText(char *text, int which)
{
    JT *hero = (JT *)GetHeroPo(&D_003DD490);
    if (!hero || !IsBasicDerivedFrom((BASIC *)hero, (CID)CID_STEPPLAYER))
        return;
    int *reconSelector = hero->jtmode.hud.pnUnk14;
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
namespace M_BuildReconTargetText { extern "C" {
struct G;
extern struct G D_003DD490;
struct Z { char pad[0xF8]; int nameTextId; };
struct Y { char pad[0x74]; int numTargets; struct Z *targets[1]; };
struct X { char pad[0xF38]; struct Y *reconSelector; };   /* STEPPLAYER jtmode.hud.pnUnk14 */
struct X *GetHeroPo(struct G *g);
int IsBasicDerivedFrom__FP5BASIC3CID(void *p, int cid);
char *FindSwLocalizedString__FP2SWi(struct G *g, int id);
void strcpy(char *dst, char *src);
/** @sly2-annot addr=0x2111F0
 * Purpose: STRINGGEN ids 0x2DF-0x2E4: langstr of recon target i (P+0x78[i]->idName F8) if i < P+0x74
 * Info: proposed name BuildReconTargetText (T1); TU hud
 * Confidence: mid (draft) [T1 0.50] - evidence: static analysis; sources: r7c_hud
 * See: docs/reference/functions/hud-ui.md#hud-0x210eb0-0x21bb88
 * @end-sly2-annot */
// HUD texts 0x2DF-0x2E4: the name of recon target i, if there is one.
void BuildReconTargetText(char *dst, int i)
{
    struct G *sw = &D_003DD490;
    struct X *hero = GetHeroPo(sw);
    if (hero && IsBasicDerivedFrom__FP5BASIC3CID(hero, CID_STEPPLAYER)) {
        struct Y *selector = hero->reconSelector;
        if (selector && i < selector->numTargets)
            strcpy(dst, FindSwLocalizedString__FP2SWi(sw, selector->targets[i]->nameTextId));
    }
}
} } // namespace M_BuildReconTargetText

// 0x211298 BuildMenuCarouselText
namespace M_func_00211298 { extern "C" {
struct RESOURCE { int id; const char *text; };
struct G { char gap[0x4640]; int count; struct RESOURCE *resources; };
extern struct G D_003DD490;
struct F { int id; char pad[0x1C]; };
struct E { int numEntries; struct F *entries; char pad[0xC]; int selection; char pad2[4]; };   /* a menu page */
#undef D_003D9EDC
extern struct E D_003D9EDC[] __asm__("D_003D9ED0+0xC");   /* screen's object+0xC, a separate symbol to g++: the base+offset form changes BuildMenuCarouselText (CSE of the base) */
#undef D_003D9EF4
extern int D_003D9EF4 __asm__("D_003D9ED0+0x24");   /* screen's object+0x24; see D_003D9EDC */
const char *FindSwLocalizedString__FP2SWi(struct G *g, int id);
char *strcpy(char *dst, const char *src);
/** @sly2-annot addr=0x211298
 * Purpose: STRINGGEN ids 0x2D2-0x2DB: name of entry (k + sel - 3) mod n of the current menu page
 *   D_003D9EDC[D_003D9EF4]
 * Info: proposed name BuildMenuCarouselText (T2); TU hud
 * Confidence: low (draft) [T2 0.35] - evidence: static analysis; sources: r7c_hud
 * See: docs/reference/functions/hud-ui.md#hud-0x210eb0-0x21bb88
 * @end-sly2-annot */
// HUD texts 0x2D2-0x2DB: the name of the menu entry k places from the selection on the current menu page (wrapping).
// (inferred)
void BuildMenuCarouselText(char *dst, int k)
{
    struct E *page = &D_003D9EDC[D_003D9EF4];
    if (page->numEntries) {
        int n = page->numEntries;
        int j = k + (long)(page->selection - 3);   /* slot 3 of the carousel is the selection */
        while (j < 0)
            j += n;
        (void)strcpy(dst, FindSwLocalizedString__FP2SWi(&D_003DD490, page->entries[j % n].id));
    }
}
} } // namespace M_func_00211298
#undef D_003D9EDC
#undef D_003D9EF4

// 0x211348 BuildWidgetArgText
namespace M_BuildWidgetArgText { extern "C" {
struct T { char pad[0xCC]; int argTextIds[1]; };
extern struct G D_003DD490;
int FindSwLocalizedString__FP2SWi(struct G *g, int id);
void strcpy(void *p, int v);
/** @sly2-annot addr=0x211348
 * Purpose: STRINGGEN ids 0x2E5-0x2EE: langstr(owner WIDGET->aid[i] at +0xCC)
 * Info: proposed name BuildWidgetArgText (T1); TU hud
 * Confidence: mid (draft) [T1 0.70] - evidence: static analysis; sources: r7c_hud
 * See: docs/engine/hud-ui.md#5-key-functions, docs/reference/functions/hud-ui.md#hud-0x210eb0-0x21bb88
 * @end-sly2-annot */
// HUD texts 0x2E5-0x2EE: the language string of the owning widget's argument i.
void BuildWidgetArgText(void *text, int i, struct T *widget)
{
    strcpy(text, FindSwLocalizedString__FP2SWi(&D_003DD490, widget->argTextIds[i]));
}
} } // namespace M_BuildWidgetArgText

// 0x211390 BuildSubtitleText
/** @sly2-annot addr=0x211390
 * Purpose: STRINGGEN id 0x2EF: subtitle/speaker text from global D_004435F0 via 0x2277D0
 * Info: proposed name BuildSubtitleText (T2); TU hud
 * Notes: names_v2 v1 proposed AddButtonPushObject__FP6BUTTON3OID (0.000)
 * Confidence: low (draft) [T2 0.45] - evidence: static naming pass; sources: r7c_hud
 * See: docs/reference/functions/hud-ui.md#hud-0x210eb0-0x21bb88
 * @end-sly2-annot */
// HUD text 0x2EF: the current subtitle's text. (inferred)
extern "C" void BuildSubtitleText(int n)
{
    GetSubtitleText(D_004435F0, n);
}

// 0x2113B8 BuildSubtitleButtonText
/** @sly2-annot addr=0x2113B8
 * Purpose: STRINGGEN id 0x2F0: 0x227940(&D_004435F0,pchz) (r8: one char per set flag bit)
 * Info: proposed name BuildSubtitleFlagsText (T2); TU hud
 * Notes: names_v2 v1 proposed AddButtonNoPushObject__FP6BUTTON3OID (0.000)
 * Confidence: low (draft) [T2 0.30] - evidence: static naming pass; sources: r7c_hud
 * See: docs/reference/functions/hud-ui.md#hud-0x210eb0-0x21bb88
 * @end-sly2-annot */
// HUD text 0x2F0: the current subtitle's button glyphs. (inferred)
extern "C" void BuildSubtitleButtonText(int n)
{
    FormatSubtitleButtonGlyphs(D_004435F0, n);
}

// 0x2113E0 BuildTickerText
/** @sly2-annot addr=0x2113E0
 * Purpose: STRINGGEN id 0x2F1: langstr(D_003DA0D8) = message of the scrolling ticker D_003DA0C0
 * Info: proposed name BuildTickerText (T1); TU hud
 * Confidence: mid (draft) [T1 0.55] - evidence: static analysis; sources: r7c_hud
 * See: docs/reference/functions/hud-ui.md#hud-0x210eb0-0x21bb88
 * @end-sly2-annot */
/* STRINGGEN getter: the localized string D_003DA0D8 names. */
// HUD text 0x2F1: the message of the scrolling ticker.
extern "C" void BuildTickerText(char *pchz)
{
    strcpy(pchz, FindSwLocalizedString(&D_003DD490, D_003DA0D8));
}

// 0x211420 BuildOperatedObjectText
namespace M_func_00211420 { extern "C" {
struct E { char *name; int nameTextId; int sellPrice; };
extern struct E D_002C3288[];   /* the loot table (screen.cpp's Thiefnet screens) */
struct Z { int unknown0; int lootIndex; };
struct Y { char pad[0x224]; int keyTextId; char pad228[0x1C]; struct Z *loot; };
/* STEPPLAYER: jtgrab.nState, the grabbed object (inferred), the default text id (inferred) */
struct X { char pad[0x4C8]; int grabState; char pad4cc[0x9A0 - 0x4CC]; struct Y *heldObject; char pad9a4[0x1318 - 0x9A4]; int heldTextId; };
enum { GRAB_STATE_HOLDING = 0xC };   /* (inferred) */
extern char D_003DD490[];
struct X *GetHeroPo(void *);
int IsBasicDerivedFrom__FP5BASIC3CID(void *, int);
char *FindSwLocalizedString__FP2SWi(void *, int);
void strcpy(char *, char *);
/** @sly2-annot addr=0x211420
 * Purpose: STRINGGEN id 0x2F4: name of the object the player operates in mode x4C8==0xC (cid 0x5E table
 *   D_002C3288 / cid 0x5C field / player x1318)
 * Info: proposed name BuildOperatedObjectText (T2); TU hud
 * Confidence: low (draft) [T2 0.30] - evidence: static analysis; sources: r7c_hud
 * See: docs/reference/functions/hud-ui.md#hud-0x210eb0-0x21bb88
 * @end-sly2-annot */
// HUD text 0x2F4: the name of the object the player is operating (in the "operate" mode). (inferred)
void BuildOperatedObjectText(char *text)
{
    int id = -1;
    struct X *hero = GetHeroPo(D_003DD490);
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
        strcpy(text, FindSwLocalizedString__FP2SWi(D_003DD490, id));
    else
        *text = 0;
}
} } // namespace M_func_00211420

// 0x211528 StartupHudGifPackets
namespace M_func_00211528 {
// Initialize the fixed GIF register packets.
extern "C" { extern QW D_004421B0[7]; }
extern "C" { extern QW D_00442240[0x27]; }
extern "C" { extern QW D_00442210[3]; }
extern "C" { extern QW D_004424B0[2]; }
extern "C" { extern QW D_004424D0[2]; }
extern "C" { extern QW D_00442560[4]; }
extern "C" { extern QW D_004425A0[3]; }
extern "C" { extern QW D_004424F0[3]; }
extern "C" { extern QW D_00442520[4]; }
extern "C" { extern int D_002DEDB0; }
extern "C" { extern ulong D_002DEBE0[] __asm__("D_002DEBE0"), D_002DECD0[] __asm__("D_002DECD0"); }   /* gs.cpp's draw environments (sceGsDrawEnv1) */
/** @sly2-annot addr=0x211528
 * Purpose: startup-table entry: builds the static GIF register packets used by sprite/HUD rendering
 * Info: proposed name StartupScreen (T2); TU hud
 * Confidence: low (draft) [T2 0.35] - evidence: static analysis; sources: r7c_hud
 * See: docs/engine/main-loop-startup.md#open-questions, docs/engine/overview.md#1-boot,
 *   docs/reference/functions/hud-ui.md#hud-0x210eb0-0x21bb88
 * @end-sly2-annot */
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
namespace M_BeginRpHud { extern "C" {
extern char D_0025AFA0[], D_0025B870[], D_002DF800[];
extern char D_004421A0[], D_004424B0[], D_004424D0[], D_004424F0[], D_00442520[], D_004425A0[], D_00442560[], D_00442240[];
extern char D_000000C0[], D_00000100[];
void EnsureVu1Code__FP4VIFSPvT1(void *, void *, void *);
void AddDmaCnt__4DMAS(void *);
void AddVifFlush__4VIFS(void *);
void AddDmaCall__4DMASP2QW(void *, void *);
void AddVifBaseOffset__4VIFSii(void *, int, int);
void AddVifUnpack__4VIFS3UPKiPvi(void *, int, int, void *, int);
void AddVifMscal__4VIFSPv(void *, void *);
void AddVifFlusha__4VIFS(void *);
void AddVifDirect__4VIFSiP2QWi(void *, int, void *, int);
void EndDmaCnt__4DMAS(void *);
/** @sly2-annot addr=0x211A80
 * Purpose: Pass 5 begin (kind 21): HUD microprogram + constants + DIRECT HUD GS state
 * Info: proposed name BeginRpHud (T1); TU hud
 * Notes: superseded: SetupSpriteVu1 (r7c_hud)
 * Confidence: high [T1 0.85] - evidence: static analysis; sources: r7c_hud+rendering+verdicts_followups
 * See: docs/engine/hud-ui.md#5-key-functions,
 *   docs/engine/rendering.md#4-the-26-kinds-and-12-passes-cstatic-tables-dumped-from-the-elf-roles-cstatic-unless-marked,
 *   docs/reference/functions/hud-ui.md#hud-0x210eb0-0x21bb88
 * @end-sly2-annot */
// Begins the HUD render pass: uploads the HUD VU1 microprogram and its constants and sets the GS state for HUD drawing.
void BeginRpHud(void *p)
{
    EnsureVu1Code__FP4VIFSPvT1(p, D_0025AFA0, D_0025B870);
    AddDmaCnt__4DMAS(p);
    AddVifFlush__4VIFS(p);
    AddDmaCall__4DMASP2QW(p, D_002DF800);
    AddVifBaseOffset__4VIFSii(p, 0, 0);
    AddVifUnpack__4VIFS3UPKiPvi(p, 0xC, 7, D_004421A0, 0xD);
    AddVifUnpack__4VIFS3UPKiPvi(p, 0xC, 2, D_004424B0, 0x15);
    AddVifUnpack__4VIFS3UPKiPvi(p, 0xC, 2, D_004424D0, 0x17);
    AddVifUnpack__4VIFS3UPKiPvi(p, 0xC, 3, D_004424F0, 0x19);
    AddVifUnpack__4VIFS3UPKiPvi(p, 0xC, 4, D_00442520, 0x1C);
    AddVifUnpack__4VIFS3UPKiPvi(p, 0xC, 3, D_004425A0, 0x20);
    AddVifUnpack__4VIFS3UPKiPvi(p, 0xC, 4, D_00442560, 0x23);
    AddVifBaseOffset__4VIFSii(p, 0x32, 0x1E7);
    AddVifMscal__4VIFSPv(p, D_000000C0);
    AddVifFlusha__4VIFS(p);
    AddVifDirect__4VIFSiP2QWi(p, 0x27, D_00442240, 0);
    AddVifFlusha__4VIFS(p);
    AddVifMscal__4VIFSPv(p, D_00000100);
    AddVifFlusha__4VIFS(p);
    EndDmaCnt__4DMAS(p);
}
} } // namespace M_BeginRpHud

// 0x211C10 EndRpHud
/** @sly2-annot addr=0x211C10
 * Purpose: Pass 5 end: MSCAL 0xC0, FLUSHA
 * Info: proposed name EndRpHud (T1); TU hud
 * Notes: superseded: FinishSpriteVu1 (r7c_hud)
 * Confidence: mid (draft) [T1 0.55] - evidence: static analysis; sources: r7c_hud+rendering
 * See: docs/engine/rendering.md#4-the-26-kinds-and-12-passes-cstatic-tables-dumped-from-the-elf-roles-cstatic-unless-marked,
 *   docs/reference/functions/hud-ui.md#hud-0x210eb0-0x21bb88
 * @end-sly2-annot */
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
// Constructor of widget class 0x82: base widget, then four ids and one more field set to -1 (unused).
WIDGET130::WIDGET130() { anUnk110[0] = -1; anUnk110[1] = -1; anUnk110[2] = -1; anUnk110[3] = -1; nUnk120 = -1; }

// 0x211CA8 ShowModalBoxWidget
namespace M_func_00211CA8 { extern "C" {
struct S { char pad[0x7C]; int mainAnimId; char pad2[0x90]; int isOpen; };   /* isOpen: WIDGET127+0x110 (inferred) */
extern struct G D_003DD490;
void ShowWidget(struct S *p, int k);
void IncrementSwHandsOff(struct G *g, int a);
void PlayWidgetAnim(struct S *p, int a);
/** @sly2-annot addr=0x211CA8
 * Purpose: CID 0x7F slot49 override: if idle, ShowWidget + IncrementSwHandsOff + play anim 0x33A
 * Info: proposed name ShowWidget127 (T2); class WIDGET127 slot49; TU hud
 * Notes: names_v2 v1 proposed RenderBlots__Fv (0.043) | v2: demoted to T2 (placeholder token in name: 127)
 * Confidence: low (draft) [T2 0.50] - evidence: static naming pass; sources: r7c_hud
 * See: docs/reference/functions/hud-ui.md#hud-0x210eb0-0x21bb88
 * @end-sly2-annot */
// Shows the modal message box (class 0x7F, slot 49) if it is idle: shows the widget, takes player control and plays its
// open animation. (inferred)
void ShowModalBoxWidget(struct S *widget, int arg)
{
    if (widget->mainAnimId == -1) {
        ShowWidget(widget, arg);
        IncrementSwHandsOff(&D_003DD490, 0);
        widget->isOpen = 1;
        PlayWidgetAnim(widget, 0x33A);   /* the box's open animation */
    }
}
} } // namespace M_func_00211CA8

// 0x211D08 InitModalBoxWidget
namespace M_func_00211D08 { extern "C" {
struct S { char pad[0xB0]; int fontFlag; };
void InitLo(struct S *p);
/** @sly2-annot addr=0x211D08
 * Purpose: CID 0x7F Init (slot7): InitLo, +0xB0 = 1
 * Info: proposed name InitWidget127 (T2); class WIDGET127 slot7; TU hud
 * Notes: names_v2 v1 proposed PostBlotLoad__FP4BLOT (0.001) | v2: demoted to T2 (placeholder token in name:
 *   127)
 * Confidence: low (draft) [T2 0.60] - evidence: static naming pass; sources: r7c_hud
 * See: docs/reference/functions/hud-ui.md#hud-0x210eb0-0x21bb88
 * @end-sly2-annot */
} } // namespace M_func_00211D08
// Initializes the modal message box (class 0x7F): base init and its "font" flag.
void WIDGET127::Init() { using namespace M_func_00211D08; M_func_00211D08::S *widget = (M_func_00211D08::S *)this;
    InitLo(widget);
    widget->fontFlag = 1;
}

// 0x211D38 UpdateModalBoxWidget
namespace M_func_00211D38 { extern "C" {
// Actor update: commit completed state after every noninitial guard path.
struct VT { char pad[0x190]; short d; short pd; void (*pf)(void *); };
struct S { char pad[0xC]; struct VT *pvt; char pad10[0x48]; int state; float stateTime; char pad60[0xB0]; int isOpen; int keepsGamePaused; };
extern struct G { char pad[0x3B0C]; void *pausedClockOwner; } D_003DD490;   /* SW+0x3B0C: resumed via func_00162DF8 (inferred) */
extern int D_002DD318;
extern char D_002E6550[], D_003DA4F0[], D_002DFC00[];
int IsWidgetIdle(struct S *p);
void UpdateWidget(struct S *p);
int FScreenShown(void *);
int HandleJoyButtons(void *, int);
void DecrementSwHandsOff(struct G *, int);
void RestartJob(void *);
/** @sly2-annot addr=0x211D38
 * Purpose: CID 0x7F Update: modal box; after 0.5 s visible and button mask 0xF0 pressed ->
 *   DecrementSwHandsOff, optional resume via SW+0x3B0C, HideWidget
 * Info: proposed name UpdateWidget127 (T2); class WIDGET127 slot16; TU hud
 * Notes: names_v2 v1 proposed DrawBlot__FP4BLOT (0.016) | v2: demoted to T2 (placeholder token in name: 127)
 * Confidence: low (draft) [T2 0.55] - evidence: static naming pass; sources: r7c_hud
 * See: docs/reference/functions/hud-ui.md#hud-0x210eb0-0x21bb88
 * @end-sly2-annot */
// Per-frame modal message box: once visible for half a second, a face-button press releases player control, optionally
// resumes the game and hides the box. (inferred)
void UpdateModalBoxWidget(struct S *widget)
{
    if (IsWidgetIdle(widget))
        return;
    UpdateWidget(widget);
    if (widget->state == WIDGET_STATE_VISIBLE && 0.5f < g_clock.realTime - widget->stateTime && !FScreenShown(D_002E6550)
        && !FScreenShown(D_003DA4F0) && HandleJoyButtons(D_002DFC00, 0xF0 /* face buttons */)) {
        DecrementSwHandsOff(&D_003DD490, 0);
        if (widget->keepsGamePaused == 0 && D_003DD490.pausedClockOwner) {
            D_002DD318++;
            RestartJob(D_003DD490.pausedClockOwner);
        }
        widget->pvt->pf((char *)widget + widget->pvt->d);   /* HideWidget (slot 50) */
    }
    widget->isOpen = 1;
}
} } // namespace M_func_00211D38

// 0x211E40 InitWidget128
namespace M_func_00211E40 { extern "C" {
struct S { char pad[0xB0]; int fontFlag; };
void InitLo(struct S *p);
/** @sly2-annot addr=0x211E40
 * Purpose: CID 0x80 Init: InitLo, +0xB0 = 1
 * Info: proposed name InitWidget128 (T2); class WIDGET128 slot7; TU hud
 * Notes: names_v2 v1 proposed OnBlotReset__FP4BLOT (0.053) | v2: demoted to T2 (placeholder token in name:
 *   128)
 * Confidence: low (draft) [T2 0.60] - evidence: static naming pass; sources: r7c_hud
 * See: docs/reference/functions/hud-ui.md#hud-0x210eb0-0x21bb88
 * @end-sly2-annot */
} } // namespace M_func_00211E40
// Initializes widget class 0x80: base init and its "font" flag.
void WIDGET128::Init() { using namespace M_func_00211E40; M_func_00211E40::S *widget = (M_func_00211E40::S *)this;
    InitLo(widget);
    widget->fontFlag = 1;
}

// 0x211E70 AddItemCounterOidA
namespace M_func_00211E70 { extern "C" {
struct E { int oid; void *object; };
struct S { char pad[0x110]; unsigned int numEntries; struct E entries[16]; };
/** @sly2-annot addr=0x211E70
 * Purpose: CID 0x81: append oid to list A (+0x110, max 16)
 * Info: proposed name AddWidget129OidA (T2); TU hud
 * Notes: names_v2 v1 proposed AddVolbtnPushObject__FP6VOLBTN3OID (0.002) | v2: demoted to T2 (placeholder
 *   token in name: 129)
 * Confidence: low (draft) [T2 0.50] - evidence: static naming pass; sources: r7c_hud
 * See: docs/reference/functions/hud-ui.md#hud-0x210eb0-0x21bb88
 * @end-sly2-annot */
// Adds an object to the item counter widget's first list (at most 16). (inferred)
void AddItemCounterOidA(struct S *widget, int oid)
{
    unsigned int count = widget->numEntries;
    if (count < 16) {
        widget->entries[count].oid = oid;
        widget->numEntries = count + 1;
    }
}
} } // namespace M_func_00211E70

// 0x211E98 AddItemCounterOidB
namespace M_func_00211E98 { extern "C" {
struct E { int oid; void *object; };
struct S { char pad[0x194]; unsigned int numEntries; struct E entries[16]; };
/** @sly2-annot addr=0x211E98
 * Purpose: CID 0x81: append oid to list B (+0x194, max 16)
 * Info: proposed name AddWidget129OidB (T2); TU hud
 * Notes: names_v2 v1 proposed SetButtonRsmg__FP6BUTTONi3OIDN22 (0.000) | v2: demoted to T2 (placeholder token
 *   in name: 129)
 * Confidence: low (draft) [T2 0.50] - evidence: static naming pass; sources: r7c_hud
 * See: docs/reference/functions/hud-ui.md#hud-0x210eb0-0x21bb88
 * @end-sly2-annot */
// Adds an object to the item counter widget's second list (at most 16). (inferred)
void AddItemCounterOidB(struct S *widget, int oid)
{
    unsigned int count = widget->numEntries;
    if (count < 16) {
        widget->entries[count].oid = oid;
        widget->numEntries = count + 1;
    }
}
} } // namespace M_func_00211E98

// 0x211EC0 PostLoadItemCounterWidget
namespace M_func_00211EC0 { extern "C" {
struct G;
extern struct G D_003DD490;
struct B { int oid; void *object; };
struct S { char pad[0x110]; int numEntriesA; struct B entriesA[0x10]; int numEntriesB; struct B entriesB[1]; };
void PostWidgetLoad(struct S *p);
void *FindSwObject__FP2SWi3OIDP2LO(struct G *g, int k, int id, int z);
/** @sly2-annot addr=0x211EC0
 * Purpose: CID 0x81 PostLoad: PostWidgetLoad then resolve both oid lists to LOs (FindSwObject grffso 5)
 * Info: proposed name PostWidget129Load (T2); class WIDGET129 slot15; TU hud
 * Notes: v2: demoted to T2 (placeholder token in name: 129)
 * Confidence: low (draft) [T2 0.65] - evidence: static analysis; sources: r7c_hud
 * See: docs/reference/functions/hud-ui.md#hud-0x210eb0-0x21bb88
 * @end-sly2-annot */
} } // namespace M_func_00211EC0
// Item counter widget post-load (class 0x81): base widget post-load, then resolves both OID lists to objects.
void WIDGET129::VUnk14() { using namespace M_func_00211EC0; using M_func_00211EC0::D_003DD490; M_func_00211EC0::S *widget = (M_func_00211EC0::S *)this;
    int i;
    PostWidgetLoad(widget);
    for (i = 0; i < widget->numEntriesB; i++) {
        void *object = FindSwObject__FP2SWi3OIDP2LO(&D_003DD490, 5, widget->entriesB[i].oid, 0);
        if (object)
            widget->entriesB[i].object = object;
    }
    for (i = 0; i < widget->numEntriesA; i++) {
        void *object = FindSwObject__FP2SWi3OIDP2LO(&D_003DD490, 5, widget->entriesA[i].oid, 0);
        if (object)
            widget->entriesA[i].object = object;
    }
}

// 0x211F90 UpdateItemCounterWidget
namespace M_func_00211F90 { extern "C" {
struct VT {
    char pad[0x188]; short d49; short pd49; void (*f49)(void *,int);
    short d50; short pd50; void (*f50)(void *);
};
struct O {char pad[0x54];int state;};   /* a task (state 1: active, inferred) */
/* The lists of AddItemCounterOidA/B read one word late: object of entry i, then oid of entry i + 1. */
struct E {struct O *object;int nextOid;};
struct S {
    char pad[0xC];struct VT *vt;char pad10[0x100];
    int countA;int firstOidA;struct E a[15];int lastObjectA;int countB;int firstOidB;struct E b[0];
};
enum { TASK_STATE_ACTIVE = 1 };   /* (inferred) */
extern char D_002C3EC0[0x90];
extern char D_002A8AA0[];
void UpdateWidget(struct S *);
int IsTaskComplete(struct O *);
int FWidgetAnimPlaying(struct S *,int);
int sprintf(char *,char *,...);
/** @sly2-annot addr=0x211F90
 * Purpose: CID 0x81 Update: counts finished/active objects, writes counts into "Widget Item 0/1/2"
 *   (doneB,totalB,remainingB) for text ids 0x2FF..; shows while any active, hides when all done
 * Info: proposed name UpdateWidget129 (T2); class WIDGET129 slot16; TU hud
 * Notes: v2: demoted to T2 (placeholder token in name: 129) | runtime6 G17: counts complete tasks each frame
 *   and formats done/total/remaining | T2: placeholder token 129
 * Confidence: low (draft) [T2 0.65] - evidence: runtime (save-state RAM); sources: r7c_hud+runtime
 * See: docs/engine/game-progression.md#6-objective-counters-job-markers-and-the-episode-menu,
 *   docs/reference/functions/hud-ui.md#hud-0x210eb0-0x21bb88
 * @end-sly2-annot */
// Per-frame item counter: counts finished and active objects, writes done / total / remaining into the widget's item
// texts, shows while any is active and hides when all are done. (inferred)
void UpdateItemCounterWidget(struct S *widget)
{
    int i;
    int numComplete = 0;
    int numCompleteB = 0;
    int numActive = 0;
    UpdateWidget(widget);
    for (i = 0; i < widget->countB; i++) {
        struct O *task = widget->b[i].object;
        if (task) {
            if (task->state == TASK_STATE_ACTIVE)
                numActive++;
            else if (IsTaskComplete(task)) {
                numComplete++;
                numCompleteB++;
            }
        }
    }
    for (i = 0; i < widget->countA; i++) {
        struct O *task = widget->a[i].object;
        if (task) {
            if (task->state == TASK_STATE_ACTIVE)
                numActive++;
            else if (IsTaskComplete(task))
                numComplete++;
        }
    }
    /* the three 0x30-byte "Widget Item" text buffers: done, total, remaining */
    if (FWidgetAnimPlaying(widget, 0x314)) {
        sprintf(D_002C3EC0, D_002A8AA0, numCompleteB);
        sprintf(D_002C3EC0 + 0x30, D_002A8AA0, widget->countB);
        sprintf(D_002C3EC0 + 0x60, D_002A8AA0, widget->countB - numCompleteB);
    }
    if (numComplete == widget->countB + widget->countA || !numActive)
        widget->vt->f50((char *)widget + widget->vt->d50);       /* HideWidget */
    else if (numActive > 0)
        widget->vt->f49((char *)widget + widget->vt->d49, -1);   /* ShowWidget */
}
} } // namespace M_func_00211F90

// 0x212158 WIDGET_ctor
namespace M_WIDGET_ctor { extern "C" {
struct S { void *frame; void *sidebag; char pad[4]; void *vt; char pad2[0x6C]; int mainAnimId; char pad3[0x4C]; int argTextIds[10]; char pad4[8]; char childList[4]; };
extern char D_002DAF30[] __asm__("_vt$6WIDGET");
void InitSl(void *p, int n);
/** @sly2-annot addr=0x212158
 * Purpose: WIDGET ctor: vtable 0x2DAF30, idAnimMain(+0x7C)=-1, 10 arg ids -1, InitSl(+0xFC, 0x68)
 * Info: proposed name WIDGET_ctor (T1); class WIDGET; TU hud
 * Confidence: high [T1 0.85] - evidence: static analysis; sources: r7c_hud+verdicts_followups
 * See: docs/engine/hud-ui.md#5-key-functions, docs/reference/functions/hud-ui.md#hud-0x210eb0-0x21bb88
 * @end-sly2-annot */
struct S *WIDGET_ctor(struct S *widget)
{
    unsigned int i;
    widget->vt = D_002DAF30;
    widget->mainAnimId = -1;
    widget->frame = 0;      /* BASIC's counted pointers */
    widget->sidebag = 0;
    for (i = 0; i < 10; i++)
        widget->argTextIds[i] = -1;
    InitSl(widget->childList, 0x68);   /* InitSl */
    return widget;
}
} } // namespace M_WIDGET_ctor

// 0x2121C8 IsWidgetIdle
/** @sly2-annot addr=0x2121C8
 * Purpose: state(+0x58)<2 && request(+0x64)<2
 * Info: proposed name FWidgetIdle (T2); TU hud
 * Notes: names_v2 v1 proposed SetVolbtnRsmg__FP6VOLBTNi3OIDN22 (0.000)
 * Confidence: low (draft) [T2 0.45] - evidence: static naming pass; sources: r7c_hud
 * See: docs/reference/functions/hud-ui.md#hud-0x210eb0-0x21bb88
 * @end-sly2-annot */
/* Whether the widget is neither shown nor asked to show (state and request both below 2). */
// True when the widget is neither shown nor requested to show. (inferred)
extern "C" int IsWidgetIdle(WIDGETV *widget)
{
    int isIdle = 0;
    if (widget->state < WIDGET_STATE_VISIBLE)
        isIdle = widget->request < WIDGET_REQUEST_SHOW;
    return isIdle;
}

// 0x2121E8 FWidgetAnimPlaying
namespace M_FWidgetAnimPlaying { extern "C" {
struct E { char pad[0x14]; int id; };
struct A { unsigned short numAnims; short pad; struct E *anims; char pad8[0x28]; };   /* a sprite group */
struct S { char pad[0x7C]; int mainAnimId; unsigned short numSpriteGroups; short pad82; struct A *spriteGroups; };
int GetSpriteMainState(struct A *a);
int FSpriteAnimActive(struct A *a);
/** @sly2-annot addr=0x2121E8
 * Purpose: is anim id (0x314=main track) still running on its sprite
 * Info: proposed name FWidgetAnimPlaying (T1); TU hud
 * Notes: names_v2 v1 proposed PostCtrLoad__FP3CTR (0.005)
 * Confidence: mid (draft) [T1 0.60] - evidence: static naming pass; sources: r7c_hud
 * See: docs/reference/functions/hud-ui.md#hud-0x210eb0-0x21bb88
 * @end-sly2-annot */
// True while the widget animation with the given id (0x314 = main track) is still running.
int FWidgetAnimPlaying(struct S *widget, int animId)
{
    int groupIndex, i;
    if (animId == -1)
        return 0;
    if (widget->mainAnimId == -1)
        return 0;
    if (animId == 0x314)   /* the main track */
        return GetSpriteMainState(widget->spriteGroups) == 0;
    for (groupIndex = 0; groupIndex < widget->numSpriteGroups; groupIndex++) {
        struct A *group = &widget->spriteGroups[groupIndex];
        for (i = 0; i < group->numAnims; i++) {
            if (animId == group->anims[i].id)
                return FSpriteAnimActive(group);
        }
    }
    return 0;
}
} } // namespace M_FWidgetAnimPlaying

// 0x212298 LoadWidgetFromBrx
namespace M_LoadWidgetFromBrx { extern "C" {
typedef struct { int cb; int cbAlign; } AREQ;
struct O { char pad[0x40]; void *owner; };
struct S {
    char pad[0x50]; int spriteCount; int sprites; int state; char pad5c[0x14]; int loopAnimId; int showAnimId; int hideAnimId; int mainAnimId;
    char pad80[0x10]; float duration; int width; int height; char pad9c[0x20]; int hasFont; struct O *deferredLoad; void *spriteHeap;
};
extern char D_003E1C70[], D_003E1C80[];

void LoadLoFromBrx(struct S *p, void *pbis);
int S8Read__18CBinaryInputStream(void *pbis);
float F32Read__18CBinaryInputStream(void *pbis);
int U32Read__18CBinaryInputStream(void *pbis);
struct O *AllocClearedFromWorldHeap(AREQ *preq);
void CBinaryInputStream_ReadCb64(void *pbis, struct O *po);
void AppendSlEntry(void *, struct O *po);
void LoadSpriteTable(struct S *p, void *pbis, struct S *p2, int *pa, int *pb);
void LoadSpriteGroups(struct S *p, void *pbis, struct S *p2);
/** @sly2-annot addr=0x212298
 * Purpose: WIDGET LoadFromBrx: default anims 0x310/0x30F/0x311, LoadLoFromBrx, fStreamed, duration, width,
 *   height, fFont; immediate or deferred sprite load
 * Info: proposed name LoadWidgetFromBrx (T1); class WIDGET slot14; TU hud
 * Notes: names_v2 v1 proposed UpdateCtr__FP3CTR (0.007) | level_parser CONFIRMED-parse: payload layout
 *   followed by the parser, 100%% of bytes in 4 levels
 * Confidence: high [T1 0.90] - evidence: static naming pass; sources: r7c_hud+parse-verified
 * See: docs/engine/hud-ui.md#5-key-functions,
 *   docs/formats/level-objects.md#5-per-class-payloads-vtable-slot-14,
 *   docs/reference/functions/hud-ui.md#hud-0x210eb0-0x21bb88
 * @end-sly2-annot */
// Loads a widget: default animation ids, the LO part, whether its sprites are streamed, duration, size and font flag;
// its sprites load now or later.
void LoadWidgetFromBrx(struct S *widget, void *pbis)
{
    int isStreamed;
    widget->loopAnimId = 0x310;
    widget->showAnimId = 0x30F;
    widget->hideAnimId = 0x311;
    LoadLoFromBrx(widget, pbis);
    isStreamed = S8Read__18CBinaryInputStream(pbis);
    widget->duration = F32Read__18CBinaryInputStream(pbis);
    widget->width = U32Read__18CBinaryInputStream(pbis);
    widget->height = U32Read__18CBinaryInputStream(pbis);
    widget->hasFont = U32Read__18CBinaryInputStream(pbis);
    if (isStreamed) {
        AREQ req;
        widget->state = WIDGET_STATE_UNLOADED;
        req.cb = 0x50;
        req.cbAlign = 0x10;
        widget->spriteHeap = D_003E1C70;   /* the streamed-sprite linear heap */
        widget->deferredLoad = AllocClearedFromWorldHeap(&req);
        widget->deferredLoad->owner = widget;
        CBinaryInputStream_ReadCb64(pbis, widget->deferredLoad);
        AppendSlEntry((*(char (*)[])&g_sw.slWidgetDeferred), widget->deferredLoad);
    } else {
        widget->state = WIDGET_STATE_HIDDEN;
        widget->spriteHeap = D_003E1C80;   /* the level heap */
        LoadSpriteTable(widget, pbis, widget, &widget->spriteCount, &widget->sprites);
        LoadSpriteGroups(widget, pbis, widget);
    }
    widget->mainAnimId = -1;
}
} } // namespace M_LoadWidgetFromBrx

// 0x2123C8 PostWidgetLoad
namespace M_PostWidgetLoad { extern "C" {
struct VT { char pad[0x188]; short d; short pd; void (*pf)(void *, int); };
struct S {
    char pad[0xC]; struct VT *pvt; char pad10[0x30]; unsigned int lowFlags; char pad44[0x29]; unsigned char isShownOnLoad;
    char pad6e[0x26]; int width; int height; float screenX; float screenY; char pada4[4]; int isNonExclusive; char padac[0x10];
    int hasFont; char padc0[8]; struct S *ownerWidget;
};
/* lowFlags: LO grfUnk40, either the flag word itself (bit 0 set) or a pointer to it (bit 0 clear) (inferred) */
extern struct G { char pad[0x3BE8]; void *hudFont; } D_003DD490;
void PostLoLoad(struct S *p);
void *FindSwObject__FP2SWi3OIDP2LO(struct G *, int, int, struct S *);
void SetLoBusyKind(struct S *p, int);
/** @sly2-annot addr=0x2123C8
 * Purpose: WIDGET PostLoad: font LO 0x374 into SW+0x3BE8, screen centring from width/height, show if flag,
 *   busy 1, set LO flags 0x102
 * Info: proposed name PostWidgetLoad (T1); class WIDGET slot15; TU hud
 * Notes: names_v2 v1 proposed DrawCtr__FP3CTR (0.037)
 * Confidence: high [T1 0.85] - evidence: static naming pass; sources: r7c_hud
 * See: docs/engine/hud-ui.md#5-key-functions, docs/reference/functions/hud-ui.md#tu_15cd68-0x15cd68-0x15dc80,
 *   docs/reference/functions/hud-ui.md#hud-0x210eb0-0x21bb88
 * @end-sly2-annot */
// Widget post-load: finds the font object, centres the widget on screen from its size, shows it if flagged, marks it
// busy.
void PostWidgetLoad(struct S *widget)
{
    unsigned int flags;
    float scaledHeight;
    int scaledWidth;
    PostLoLoad(widget);
    if (widget->hasFont) {
        struct G *sw = &D_003DD490;
        if (!sw->hudFont)
            sw->hudFont = FindSwObject__FP2SWi3OIDP2LO(sw, 0x405, 0x374, widget);
    }
    widget->ownerWidget = widget;
    widget->isNonExclusive = -1;
    /* centre on the GS drawing area (origin 2048,2048): width from 640 to 512 units, height times 1/2.2 */
    scaledWidth = (widget->width << 9) / 640;
    scaledHeight = (float)widget->height * 0.45454547f;
    widget->screenY = 2048.0f - scaledHeight * 0.5f;
    widget->screenX = 2048.0f - (float)scaledWidth * 0.5f;
    if (widget->isShownOnLoad)
        widget->pvt->pf((char *)widget + widget->pvt->d, -1);   /* ShowWidget */
    SetLoBusyKind(widget, 1);
    flags = widget->lowFlags;
    if ((flags ^ 1) & 1) *(int *)flags |= 0x102;
    else widget->lowFlags = flags | 0x102;
}
} } // namespace M_PostWidgetLoad

// 0x2124F0 LoadWidgetSprites
namespace M_LoadWidgetSprites {
// Load prioritized sprite files.
struct FRAME {};
typedef struct {int cb,alignment;} MSG;
struct BIS {int grf;char unknown4[0x2C];void *vt;int cbSpool;char *abSpool;int unknown3C[10];};
typedef struct {int keyLo,keyHi,kind;char filename[0x104];} KEY;   /* a file lookup key (inferred) */
typedef union {MSG msg;char magic[4];} TMP;
typedef struct S S;
/* A deferred load record (0x50 bytes, g_sw.slWidgetDeferred): four key words read from the BRX, the widget, the
   load priorities. */
typedef struct N {int key0,key1;char pad8[0x2C];int key2;int pad38;int key3;S *owner;int priority,lastPriority;struct N *next;} N;
struct S {char pad[0x50];int spriteCount;void *sprites;int state;char pad5c[4];int stateFrame;char pad64[0x5C];N *deferredLoad;};
typedef struct {char pad[0xC];int priority;} POOL;
extern "C" FRAME *STACKSCOPE_ctor(FRAME *);
extern "C" void STACKSCOPE_ctor_1AE4A0(FRAME *,int) __asm__("_$_10STACKSCOPE");
extern "C" BIS *CStreamBis_ctor(BIS *,int,void *,int);
extern "C" void func_00184950(BIS *,int) __asm__("_$_10CStreamBis");
extern "C" void InitFileKey(KEY *);
extern "C" void *AllocFromStack(MSG *);
extern "C" int OpenStreamFromFileKey(BIS *,KEY *);
extern "C" void SetDecompress__18CBinaryInputStreamiPUc(BIS *,int,void *);
extern "C" unsigned char U8Read__18CBinaryInputStream(BIS *);
extern "C" unsigned short U16Read__18CBinaryInputStream(BIS *);
extern "C" int strncmp(const void *,const void *,unsigned int);
extern "C" void SetWidgetState(S *,int);
extern "C" void LoadSpriteTable(S *,BIS *,S *,int *,void **);
extern "C" void LoadSpriteGroups(S *,BIS *,S *);
extern "C" { 
 }
extern "C" { extern POOL D_003E1C70; }
extern "C" { extern int D_002BE538; }
extern "C" { extern char D_002A8AE8[]; }
static inline int Max(int a,int b) {return a<b?b:a;}
static inline int SetKey(KEY *key,N *load) {
    if(!load)return 0;
    {int k2=load->key2,k0=load->key0,k3=load->key3,k1=load->key1;
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
/** @sly2-annot addr=0x2124F0
 * Purpose: deferred (streamed) sprite file load when a widget leaves state 0
 * Info: proposed name LoadWidgetSprites (T1); TU hud
 * Confidence: mid (draft) [T1 0.55] - evidence: static analysis; sources: r7c_hud
 * See: docs/formats/textures.md#7-open, docs/reference/functions/hud-ui.md#hud-0x210eb0-0x21bb88
 * @end-sly2-annot */
// Loads a streamed widget's sprite file when the widget first leaves the unloaded state.
extern "C" void LoadWidgetSprites(S *widget) {
    WORK work;
    register BIS *bs asm("$17");asm("la %0,%a1":"=r"(bs):"p"(&work.stream));
    int priority=0;
    N *load;
    S *owner;
    int version;
    /* the highest priority among widgets that are loaded and shown (or hidden within the last frame) ... */
    for(load=(*(N * *)&g_sw.slWidgetDeferred);load;load=load->next) {
        owner=load->owner;
        if(owner->state==0)continue;
        if(owner->state==1 && D_002BE538-owner->stateFrame>=2)continue;
        priority=Max(priority,load->lastPriority);
    }
    /* ... evicts every loaded widget loaded at or above it, since the sprite heap is reset to that point */
    for(load=(*(N * *)&g_sw.slWidgetDeferred);load;load=load->next) {
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
    func_00184950(bs,2);
    STACKSCOPE_ctor_1AE4A0(&work.frame,2);
    return;
load:
    widget->deferredLoad->priority=D_003E1C70.priority;
    LoadSpriteTable(widget,bs,widget,&widget->spriteCount,&widget->sprites);
    LoadSpriteGroups(widget,bs,widget);
    widget->deferredLoad->lastPriority=D_003E1C70.priority;
    func_00184950(bs,2);
    STACKSCOPE_ctor_1AE4A0(&work.frame,2);
}
} // namespace M_LoadWidgetSprites

// 0x212780 UnloadWidgetSprites
namespace M_UnloadWidgetSprites { extern "C" {
struct C { char pad[0x44]; int priority; int lastPriority; };   /* the deferred load record */
struct S { char pad[0x48]; int unknown48; int unknown4C; int spriteCount; int sprites; char pad2[0x28]; short numSpriteGroups; short pad3; int spriteGroups; char pad4[0x38]; struct C *deferredLoad; char pad5[0x34]; int xform; char childList[4]; };
void ClearSl(void *p);
/** @sly2-annot addr=0x212780
 * Purpose: state 0 entry: drop sprite tables and lists
 * Info: proposed name UnloadWidgetSprites (T1); TU hud
 * Confidence: mid (draft) [T1 0.50] - evidence: static analysis; sources: r7c_hud
 * See: docs/reference/functions/hud-ui.md#hud-0x210eb0-0x21bb88
 * @end-sly2-annot */
// Drops a widget's sprite tables and lists (on entering the unloaded state).
void UnloadWidgetSprites(struct S *widget)
{
    struct C *load;
    widget->xform = 0;
    widget->spriteCount = 0;
    widget->sprites = 0;
    widget->numSpriteGroups = 0;
    widget->spriteGroups = 0;
    ClearSl(widget->childList);
    load = widget->deferredLoad;
    load->priority = 0;
    load->lastPriority = 0;
}
} } // namespace M_UnloadWidgetSprites

// 0x2127C8 AreWidgetLoadsIdle
namespace M_func_002127C8 { extern "C" {
struct Q { char pad[0x58]; int state; int pad2; int stateFrame; int request; };   /* the widget */
struct S { char pad[0x40]; struct Q *owner; char pad2[8]; struct S *next; };   /* a deferred load record */
extern int D_002BE538;
/** @sly2-annot addr=0x2127C8
 * Purpose: true when no deferred widget load (list (*(struct S * *)&g_sw.slWidgetDeferred)) is younger than 2 frames
 * Info: proposed name FWidgetLoadsIdle (T2); TU hud
 * Notes: names_v2 v1 proposed FUN_001ab600 (0.000)
 * Confidence: low (draft) [T2 0.35] - evidence: static naming pass; sources: r7c_hud
 * See: docs/reference/functions/hud-ui.md#hud-0x210eb0-0x21bb88
 * @end-sly2-annot */
// True when no deferred widget sprite load is younger than two frames (used to wait for widgets before continuing).
// (inferred)
int AreWidgetLoadsIdle(void)
{
    struct S *load;
    for (load = (*(struct S * *)&g_sw.slWidgetDeferred); load; load = load->next) {
        struct Q *widget = load->owner;
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
} } // namespace M_func_002127C8

// 0x212828 ShowWidgetArgsFromSplice
namespace M_ShowWidgetArgsFromSplice { extern "C" {
struct CRef { int tagk; int tag; };
struct VE { short d; short pad; void (*pf)(void *, int); };
struct S { char pad[0xC]; struct VE *vt; char pad2[0xBC]; char argTextIds[4]; };
struct L { int words[4]; };
void SetValuesFromSpliceList(struct CRef *r, void *a, int b, int c, struct L *pl, void *d);
void Decref__4CRef(struct CRef *r);
/** @sly2-annot addr=0x212828
 * Purpose: copy Splice list into +0xCC ids (max 10) then ShowWidget(-1)
 * Info: proposed name ShowWidgetArgsFromSplice (T1); TU hud
 * Confidence: mid (draft) [T1 0.50] - evidence: static analysis; sources: r7c_hud
 * See: docs/reference/functions/hud-ui.md#hud-0x210eb0-0x21bb88
 * @end-sly2-annot */
// Shows a widget with argument ids taken from a splice list (at most 10).
void ShowWidgetArgsFromSplice(struct S *widget, void *spliceList)
{
    struct CRef ref;
    struct L scratch;
    SetValuesFromSpliceList(&ref, spliceList, 0x10, 0xA, &scratch, widget->argTextIds);
    Decref__4CRef(&ref);
    widget->vt[49].pf((char *)widget + widget->vt[49].d, -1);   /* ShowWidget */
}
} } // namespace M_ShowWidgetArgsFromSplice

// 0x212880 ShowWidgetArgs
namespace M_ShowWidgetArgs { extern "C" {
struct VE { short d; short pad; void (*pf)(void *, int); };
struct S { char pad[0xC]; struct VE *vt; char pad2[0xBC]; int argTextIds[10]; };
/** @sly2-annot addr=0x212880
 * Purpose: copy n ids into +0xCC (max 10) then ShowWidget(-1)
 * Info: proposed name ShowWidgetArgs (T1); TU hud
 * Confidence: mid (draft) [T1 0.55] - evidence: static analysis; sources: r7c_hud
 * See: docs/reference/functions/hud-ui.md#hud-0x210eb0-0x21bb88
 * @end-sly2-annot */
// Shows a widget with n argument ids (at most 10), e.g. the text ids it displays.
void ShowWidgetArgs(struct S *widget, int n, int *src)
{
    int i;
    for (i = 0; i < n && (unsigned)i < 10; i++)
        widget->argTextIds[i] = src[i];
    widget->vt[49].pf((char *)widget + widget->vt[49].d, -1);   /* ShowWidget */
}
} } // namespace M_ShowWidgetArgs

// 0x2128E8 ShowWidget
namespace M_ShowWidget {
struct B { char pad[0xC];
  virtual void v1(); virtual void v2(); virtual void v3(); virtual void v4(); virtual void v5();
  virtual void v6(); virtual void v7(); virtual void v8(); virtual void v9(); virtual void v10();
  virtual void v11(); virtual void v12(); virtual void v13(); virtual void v14(); virtual void v15();
  virtual void v16(); };
struct S : B {
  char pad10[0x48]; int state; char pad5c[0x14]; int loopAnimId; int showAnimId; int hideAnimId; char pad7c[8]; void *mainSprite;
  void SetState(int) __asm__("SetWidgetState");
  void SetRequest(int) __asm__("SetWidgetRequest");
  int HasSpriteAnim(void *, int) __asm__("FSpriteHasAnim");
  void Show(int) __asm__("ShowWidget");
};
// ShowWidget (widget slot 49): requests that a widget show, loading its sprites first if needed; k = the animation
// argument (-1 = default).
void S::Show(int animId)
{
    if (state == WIDGET_STATE_UNLOADED)
        SetState(WIDGET_STATE_HIDDEN);
    SetRequest(WIDGET_REQUEST_SHOW);
    loopAnimId = animId;
    showAnimId = -1;
    hideAnimId = -1;
    if (animId != -1) {
        SetState(WIDGET_STATE_HIDDEN);
        SetState(WIDGET_STATE_VISIBLE);
        return;
    }
    /* default animations, where the sprite has them; 0x314 is the main track */
    loopAnimId = 0x314;
    if (HasSpriteAnim(mainSprite, 0x30F))
        showAnimId = 0x30F;
    if (HasSpriteAnim(mainSprite, 0x310))
        loopAnimId = 0x310;
    if (HasSpriteAnim(mainSprite, 0x311))
        hideAnimId = 0x311;
    if (state == WIDGET_STATE_HIDDEN)
        v16();   /* UpdateWidget */
}
} // namespace M_ShowWidget

// 0x2129E8 HideWidget
/** @sly2-annot addr=0x2129E8
 * Purpose: WIDGET slot50: request hide; if no disappear anim go straight to state 1
 * Info: proposed name HideWidget (T1); class WIDGET slot50; TU hud
 * Confidence: mid (draft) [T1 0.70] - evidence: static analysis; sources: r7c_hud
 * See: docs/engine/hud-ui.md#5-key-functions,
 *   docs/reference/functions/game-progression.md#game-0x1613a8-0x166270,
 *   docs/reference/functions/hud-ui.md#hud-0x210eb0-0x21bb88
 * @end-sly2-annot */
// Requests that a widget hide; without a disappear animation it goes straight to hidden.
extern "C" void HideWidget(WIDGETV *widget)
{
    SetWidgetRequest(widget, WIDGET_REQUEST_HIDE);
    if (widget->hideAnimId == -1)
        SetWidgetStateW(widget, WIDGET_STATE_HIDDEN);
}

// 0x212A30 ResetWidgetVisibleTime
/** @sly2-annot addr=0x212A30
 * Purpose: if state==2, state time = g_clock.tReal (restarts the visible-duration timeout)
 * Info: proposed name ResetWidgetVisibleTime (T2); TU hud
 * Notes: names_v2 v1 proposed StopTimer__FP5TIMER (0.006)
 * Confidence: low (draft) [T2 0.40] - evidence: static naming pass; sources: r7c_hud
 * See: docs/reference/functions/game-progression.md#game-0x1613a8-0x166270,
 *   docs/reference/functions/hud-ui.md#hud-0x210eb0-0x21bb88
 * @end-sly2-annot */
/* Restarts the state timer of a visible widget. */
// Restarts a visible widget's display timeout. (inferred)
extern "C" void ResetWidgetVisibleTime(WIDGETV *widget)
{
    if (widget->state == WIDGET_STATE_VISIBLE)
        widget->stateTime = D_002DDED0.realTime;
}

// 0x212A50 SetWidgetRequest
/** @sly2-annot addr=0x212A50
 * Purpose: not written up yet; only the symbol name (pre-existing / Sly 1-derived) describes it.
 * Info: proposed name SetWidgetRequest (EXISTING); TU hud
 * Notes: RENAME proposed -> SetWidgetRequest (existing_name_changes.csv)
 * Confidence: name high [EXISTING 0.75]; purpose not reviewed - evidence: arbitration; pre-existing symbol in
 *   symbol_addrs.txt
 * See: docs/engine/hud-ui.md#1-widget-hud-objects-hud-tu-0x210eb0-0x21bb88-file-name-unknown,
 *   docs/engine/hud-ui.md#5-key-functions, docs/reference/functions/hud-ui.md#hud-0x210eb0-0x21bb88
 * @end-sly2-annot */
/* Requests a state (1 hide, 2 show) and stamps the request time. */
// Sets the widget's requested state (show / hide), acted on by UpdateWidget.
extern "C" void SetWidgetRequest(WIDGETV *widget, int request)
{
    if (widget->request != request) {
        widget->request = request;
        widget->requestTime = D_002DDED0.realTime;
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
namespace M_SetWidgetState { extern "C" {
struct S {
    char pad[0x58]; int state; float stateTime; int stateFrame; int request; float requestTime;
    char pad1[4]; int loopAnimId; int showAnimId; int hideAnimId; int mainAnimId; int numSpriteGroups; void *spriteGroups;
    float mainAnimTime; float visibleTimeout;   /* +0x8C, read as the visible-duration limit (LoadWidgetFromBrx stores its F32 at +0x90) */
    char pad2[0xA4-0x90]; int priority; char pad3[8]; int pausesGame;   /* priority (inferred): >= 16 counts as an on-screen widget */
};
struct G { char pad[0x3BEC]; int numShownWidgets; };
struct Clock { char pad[0x3C]; float time; float dt; };
extern struct G D_003DD490;
extern struct Clock D_002DDED0;
extern int D_002DD318;
extern int D_002BE538;
void SetClockRate(struct Clock *,int,float);
void LoadWidgetSprites(struct S *);
void UnloadWidgetSprites(struct S *);
void StopWidgetMainAnim(struct S *);
void StartWidgetMainAnim(struct S *,int);
/** @sly2-annot addr=0x212A70
 * Purpose: state machine 0 unloaded,1 hidden,2 visible(loop),3 appearing,4 disappearing; counts SW+0x3BEC,
 *   pauses clock when fPause
 * Info: proposed name SetWidgetState (T1); TU hud
 * Confidence: mid (draft) [T1 0.75] - evidence: static analysis; sources: r7c_hud
 * See: docs/engine/hud-ui.md#5-key-functions, docs/reference/functions/hud-ui.md#hud-0x210eb0-0x21bb88
 * @end-sly2-annot */
// The widget state machine: 0 unloaded, 1 hidden, 2 visible, 3 appearing, 4 disappearing; keeps the world's visible
// widget count and pauses the clock for pausing widgets.
void SetWidgetState(struct S *widget, int state)
{
    if (widget->state == state)
        return;
    /* states >= VISIBLE are on screen */
    if (widget->priority >= 16) {
        if (widget->state < WIDGET_STATE_VISIBLE && state >= WIDGET_STATE_VISIBLE)
            D_003DD490.numShownWidgets++;
        else if (state < WIDGET_STATE_VISIBLE && widget->state >= WIDGET_STATE_VISIBLE)
            D_003DD490.numShownWidgets--;
    }
    if (widget->pausesGame) {
        if (widget->state < WIDGET_STATE_VISIBLE && state >= WIDGET_STATE_VISIBLE) {
            if (++D_002DD318 == 1)
                SetClockRate(&D_002DDED0, 7, 0.0f);   /* clock pause slot 7: stop */
        } else if (state < WIDGET_STATE_VISIBLE && widget->state >= WIDGET_STATE_VISIBLE) {
            if (--D_002DD318 == 0)
                SetClockRate(&D_002DDED0, 7, 1.0f);   /* run again */
        }
    }
    if (widget->state == WIDGET_STATE_UNLOADED)
        LoadWidgetSprites(widget);
    widget->state = state;
    widget->stateTime = D_002DDED0.time;
    widget->stateFrame = D_002BE538;
    switch (state) {
    case WIDGET_STATE_UNLOADED: UnloadWidgetSprites(widget); break;
    case WIDGET_STATE_HIDDEN: StopWidgetMainAnim(widget); break;
    case WIDGET_STATE_VISIBLE: StartWidgetMainAnim(widget, widget->loopAnimId); break;
    case WIDGET_STATE_APPEARING: StartWidgetMainAnim(widget, widget->showAnimId); break;
    case WIDGET_STATE_DISAPPEARING: StartWidgetMainAnim(widget, widget->hideAnimId); break;
    }
}
} } // namespace M_SetWidgetState

// 0x212C50 UpdateWidget
namespace M_UpdateWidget { extern "C" {
struct S {
    char pad[0x58]; int state; float stateTime; int stateFrame; int request; float requestTime;
    char pad1[4]; int loopAnimId; int showAnimId; int hideAnimId; int mainAnimId; int numSpriteGroups; void *spriteGroups;
    float mainAnimTime; float visibleTimeout;   /* +0x8C, read as the visible-duration limit (LoadWidgetFromBrx stores its F32 at +0x90) */
    char pad2[0xA4-0x90]; int priority; char pad3[8]; int pausesGame;   /* priority (inferred): >= 16 counts as an on-screen widget */
};
struct Clock { char pad[0x3C]; float time; float dt; };
extern struct Clock D_002DDED0;
void UpdateLo(struct S *);
int FWidgetAnimPlaying(struct S *,int);
void SetWidgetRequest(struct S *,int);
void SetWidgetState(struct S *,int);
void AdvanceWidgetSprite(void *,float);
/** @sly2-annot addr=0x212C50
 * Purpose: WIDGET Update: drives SetWidgetState from request/anim completion and duration, advances main
 *   sprite
 * Info: proposed name UpdateWidget (T1); class WIDGET slot16; TU hud
 * Notes: names_v2 v1 proposed FUN_001abe70 (0.000) | runtime: WIDGET +0x58 state 1 hidden / 2 visible
 *   confirmed
 * Confidence: high [T1 0.90] - evidence: runtime (save-state RAM); static naming pass; sources:
 *   r7c_hud+runtime
 * See: docs/engine/hud-ui.md#5-key-functions, docs/reference/functions/hud-ui.md#hud-0x210eb0-0x21bb88
 * @end-sly2-annot */
// Per-frame widget update: advances the state machine from requests, animation completion and duration, and advances
// the main sprite.
void UpdateWidget(struct S *widget)
{
    int state;
    UpdateLo(widget);
    state = widget->state;
    if (state == WIDGET_STATE_HIDDEN) {
        if (widget->request == WIDGET_REQUEST_SHOW)
            state = widget->showAnimId == -1 ? WIDGET_STATE_VISIBLE : WIDGET_STATE_APPEARING;
    } else if (widget->requestTime != D_002DDED0.time) {   /* not requested this frame */
        switch (state) {
        case WIDGET_STATE_VISIBLE:
            if (!FWidgetAnimPlaying(widget, widget->mainAnimId))
                SetWidgetRequest(widget, WIDGET_REQUEST_HIDE);
            if (widget->visibleTimeout > 0.0f && widget->visibleTimeout < D_002DDED0.time - widget->stateTime)
                SetWidgetRequest(widget, WIDGET_REQUEST_HIDE);
            if (widget->request == WIDGET_REQUEST_HIDE)
                state = widget->hideAnimId == -1 ? WIDGET_STATE_HIDDEN : WIDGET_STATE_DISAPPEARING;
            break;
        case WIDGET_STATE_APPEARING:
            if (!FWidgetAnimPlaying(widget, widget->showAnimId)) {
                switch (widget->request) {
                case WIDGET_REQUEST_HIDE: state = widget->hideAnimId == -1 ? WIDGET_STATE_HIDDEN : WIDGET_STATE_DISAPPEARING; break;
                case WIDGET_REQUEST_SHOW: state = WIDGET_STATE_VISIBLE; break;
                }
            }
            break;
        case WIDGET_STATE_DISAPPEARING:
            if (!FWidgetAnimPlaying(widget, widget->hideAnimId)) {
                switch (widget->request) {
                case WIDGET_REQUEST_HIDE: state = WIDGET_STATE_HIDDEN; break;
                case WIDGET_REQUEST_SHOW: state = widget->showAnimId == -1 ? WIDGET_STATE_VISIBLE : WIDGET_STATE_APPEARING; break;
                }
            }
            break;
        }
    }
    SetWidgetState(widget, state);
    if (widget->mainAnimId != -1) {
        float dt = g_clock.realDeltaTime;
        widget->mainAnimTime += dt;
        AdvanceWidgetSprite(widget->spriteGroups, dt);
    }
}
} } // namespace M_UpdateWidget

// 0x212E40 SetWidgetAnimPhase
namespace M_SetWidgetAnimPhase { extern "C" {
struct E { char pad[0x14]; int id; };
struct H { char pad[8]; short animIndex; char pada[0xE]; };
struct A { unsigned short numAnims; unsigned short numSlots; struct E *anims; struct H *slots; char padc[0x24]; };
struct S { char pad[0x80]; unsigned short numSpriteGroups; short pad82; struct A *spriteGroups; };
void SetSpriteDefaultPhase(struct A *pa, float phase);
void SetSpriteAnimPhase(struct A *pa, int animIndex, float g, int z);
/** @sly2-annot addr=0x212E40
 * Purpose: seek anim id (0x314=main) to fraction g of its length (meters)
 * Info: proposed name SetWidgetAnimPhase (T1); TU hud
 * Confidence: mid (draft) [T1 0.60] - evidence: static analysis; sources: r7c_hud
 * See: docs/engine/hud-ui.md#1-widget-hud-objects-hud-tu-0x210eb0-0x21bb88-file-name-unknown,
 *   docs/engine/hud-ui.md#5-key-functions, docs/reference/functions/hud-ui.md#hud-0x210eb0-0x21bb88
 * @end-sly2-annot */
// Seeks a widget animation (0x314 = main) to a fraction of its length (meters and gauges).
void SetWidgetAnimPhase(struct S *widget, int animId, float phase)
{
    int groupIndex, animIndex;
    if (animId == 0x314) {
        SetSpriteDefaultPhase(widget->spriteGroups, phase);
        return;
    }
    for (groupIndex = 0; groupIndex < widget->numSpriteGroups; groupIndex++) {
        struct A *group = &widget->spriteGroups[groupIndex];
        for (animIndex = 0; animIndex < group->numAnims; animIndex++) {
            struct E *anim = &group->anims[animIndex];
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
} } // namespace M_SetWidgetAnimPhase

// 0x212F80 SetWidgetAnimTime
namespace M_func_00212F80 { extern "C" {
struct E { char pad[0x14]; int id; };
struct H { char pad[8]; short animIndex; char pada[0xE]; };
struct A { unsigned short numAnims; unsigned short numSlots; struct E *anims; struct H *slots; char padc[0x24]; };
struct S { char pad[0x80]; unsigned short numSpriteGroups; short pad82; struct A *spriteGroups; };
void SetSpriteDefaultTime(struct A *pa, float phase);
void SetSpriteAnimTime(struct A *pa, int animIndex, float g, int z);
// 0x212F78: dead-code scrap (sly2/scrap.h)
SLY2_SCRAP("addiu $sp, $sp, 0xb0");

/** @sly2-annot addr=0x212F80
 * Purpose: like SetWidgetAnimPhase but absolute time
 * Info: proposed name SetWidgetAnimTime (T2); TU hud
 * Confidence: low (draft) [T2 0.45] - evidence: static analysis; sources: r7c_hud
 * See: docs/reference/functions/hud-ui.md#hud-0x210eb0-0x21bb88
 * @end-sly2-annot */
// Seeks a widget animation to an absolute time. (inferred)
void SetWidgetAnimTime(struct S *widget, int animId, float phase)
{
    int groupIndex, animIndex;
    if (animId == 0x314) {
        SetSpriteDefaultTime(widget->spriteGroups, phase);
        return;
    }
    for (groupIndex = 0; groupIndex < widget->numSpriteGroups; groupIndex++) {
        struct A *group = &widget->spriteGroups[groupIndex];
        for (animIndex = 0; animIndex < group->numAnims; animIndex++) {
            struct E *anim = &group->anims[animIndex];
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
} } // namespace M_func_00212F80

// 0x2130C0 QueueWidgetRenderSetup
namespace M_QueueWidgetRenderSetup { extern "C" {
// Queue the render initialization payloads in a DMA packet.

typedef unsigned int uint;
typedef unsigned long ulong;
struct S {char pad[0xA4];int priority;};
struct R {char pad[0xC];unsigned char *cur;};
struct E {int kind;float priority;uint address;int tag;};
extern struct R D_002DEA28;
extern char D_00442210[],D_002DF010[];
void CopyAqw__FPvT0i(void *,void *,int);
struct E *AllocRenderListEntry(void);
static inline void Word(struct R *r,uint word){*(uint *)r->cur=word;r->cur+=4;}
static inline void Align(struct R *r,int word){while(((uint)r->cur&15)!=4*word)Word(r,0);}
static inline void Bulk(struct R *r,int n,void *source){CopyAqw__FPvT0i(r->cur,source,n);r->cur+=n*16;}
static inline void Direct(struct R *r,int n,void *source){Align(r,3);Word(r,0x50000000|n);Bulk(r,n,source);}
// 0x2130B8: dead-code scrap (sly2/scrap.h)
SLY2_SCRAP("addiu $sp, $sp, 0xb0");

/** @sly2-annot addr=0x2130C0
 * Purpose: DMA packet with render init payloads (frame/mask GIF data) before widget sprites
 * Info: proposed name QueueWidgetRenderSetup (T1); TU hud
 * Confidence: high [T1 0.85] - evidence: static analysis; sources: r7c_hud+verdicts_followups
 * See: docs/engine/hud-ui.md#5-key-functions, docs/reference/functions/hud-ui.md#hud-0x210eb0-0x21bb88
 * @end-sly2-annot */
// Queues the DMA packet that sets up rendering before the widget sprites.
void QueueWidgetRenderSetup(struct S *p){
    unsigned char *tag=D_002DEA28.cur;
    struct E *e;
    *(ulong *)tag=0x10000000UL;D_002DEA28.cur+=16;
    Word(&D_002DEA28,0x11000000);
    Direct(&D_002DEA28,3,D_00442210);
    Direct(&D_002DEA28,0x25,D_002DF010);
    Word(&D_002DEA28,0x11000000);
    Align(&D_002DEA28,0);
    *(uint *)tag|=((D_002DEA28.cur-tag)>>4)-1;
    *(ulong *)D_002DEA28.cur=0x60000000UL;D_002DEA28.cur+=16;
    e=AllocRenderListEntry();
    e->kind=0x15;e->priority=p->priority<<18;
    e->address=(uint)tag&0x0FFFFFFF;e->tag=0;
}
} } // namespace M_QueueWidgetRenderSetup

// 0x2132D0 FWidgetVisible
/** @sly2-annot addr=0x2132D0
 * Purpose: main anim active and not obscured by a higher-priority exclusive widget
 * Info: proposed name FWidgetVisible (T1); TU hud
 * Notes: names_v2 v1 proposed FUN_001aca30 (0.000)
 * Confidence: mid (draft) [T1 0.60] - evidence: static naming pass; sources: r7c_hud
 * See: docs/reference/functions/hud-ui.md#hud-0x210eb0-0x21bb88
 * @end-sly2-annot */
// True when the widget's main animation is active and no higher-priority exclusive widget hides it.
extern "C" int FWidgetVisible(WIDGETV *widget)
{
    if (widget->mainAnimId == -1) {
        return 0;
    }
    return FWidgetObscured(widget) == 0;
}

// 0x213300 RenderWidget
namespace M_RenderWidget {
// Flush the sprite render queue and pair begin/end VU microprogram packets.
typedef unsigned int uint;
typedef unsigned char uchar;
struct V4 {float x,y,z,w;} __attribute__((aligned(16)));
struct PARAM {
    int mode,pad4;float priority;int padC;V4 offset,scale;
    PARAM(float p){offset.x=0.0f;offset.y=0.0f;offset.z=0.0f;offset.w=0.0f;scale.x=0.0f;scale.y=0.0f;scale.z=0.0f;scale.w=0.0f;priority=p;mode=1;} PARAM(){}
};
struct Q {uchar color0[4],color1[4];char pad8[8];char matrix[0x20];void *payload;int kind;char pad38[0x29];uchar channel;char pad62[6];Q *next;};
struct LIST {Q *first,*last;};
struct S {char pad[0x84];void *sprites;char pad88[0x74];LIST queue;};
struct R {unsigned char *base;int pad[2];unsigned char *cur;};
extern "C" { extern R D_002DEA28; }
extern "C" {
void RenderLoSelf(S *);
int FWidgetVisible(S *);
void DrawSprite(void *);
void QueueWidgetRenderSetup(S *);
void QueueSpriteVuProgram(S *,int,float);
void RenderSpritePayload(void *,void *,PARAM *,int);
void ClearSl(LIST *);
}
static inline void Align(R *r){while((uint)r->cur&63){*(uint *)r->cur=0;r->cur+=4;}}
static inline float Min(float x,float y){return y<x?y:x;}
static inline float Max(float x,float y){return x<y?y:x;}
static inline void Offset(V4 &v,float x,float y,float z,float w){v.x=x-127.0f;v.y=y-127.0f;v.z=z-127.0f;v.w=w-127.0f;}
static inline void SetOffset(V4 &v,const uchar *c){Offset(v,c[0],c[1],c[2],c[3]);}
static inline float ColorScale(int c,float upper){return Max(Min((c-127)*0.007874016f,upper),-1.0f);}
static inline void Scale(V4 &v,int a,int b,int d,int e,float upper){float x=ColorScale(a,upper),y=ColorScale(b,upper),z=ColorScale(d,upper),w=ColorScale(e,upper);v.x=x;v.y=y;v.z=z;v.w=w;}
static inline void SetScale(V4 &v,const Q *q,float upper){Scale(v,q->color1[0],q->color1[1],q->color1[2],q->color1[3],upper);}
static inline Q *Begin(S *p,R *r){Align(r);uint c=(uint)r->cur,b=(uint)r->base;c|=0x30000000;b|=0x30000000;r->cur=(unsigned char *)c;r->base=(unsigned char *)b;Q *pending=0;QueueWidgetRenderSetup(p);return pending;}
static inline void Finish(R *r,S *p){Align(r);uint c=(uint)r->cur,b=(uint)r->base;c&=0x0FFFFFFF;b&=0x0FFFFFFF;r->cur=(unsigned char *)c;r->base=(unsigned char *)b;Q *q=p->queue.first;while(q){Q *old=q;q=q->next;old->next=0;}ClearSl(&p->queue);}
/** @sly2-annot addr=0x213300
 * Purpose: WIDGET RenderSelf: flush the sprite render queue with VU microprogram packets
 * Info: proposed name RenderWidget (T1); class WIDGET slot19; TU hud
 * Confidence: high [T1 0.80] - evidence: static analysis; sources: r7c_hud
 * See: docs/engine/hud-ui.md#5-key-functions, docs/reference/functions/hud-ui.md#hud-0x210eb0-0x21bb88
 * @end-sly2-annot */
} // namespace M_RenderWidget
// Renders a widget: flushes its sprite render queue with the VU microprogram packets.
void WIDGET::RenderSelf(){   /* key function: WIDGET's vtable comes out here (the namespace's views) */
    using namespace M_RenderWidget;
    S *p=(S *)this;
    RenderLoSelf(p);
    if(!FWidgetVisible(p))return;
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

// 0x2137F0 FWidgetObscured
namespace M_FWidgetObscured { extern "C" {
struct S { char pad[0xA4]; int priority; char pad2[0xC]; struct S *next; };
/** @sly2-annot addr=0x2137F0
 * Purpose: some widget in exclusive list (*(struct S * *)&g_sw.dlWidgetExclusive) has higher priority (+0x0? field a)
 * Info: proposed name FWidgetObscured (T1); TU hud
 * Notes: names_v2 v1 proposed FUN_001aca68 (0.000)
 * Confidence: high [T1 0.85] - evidence: static naming pass; sources: r7c_hud+verdicts_followups
 * See: docs/engine/hud-ui.md#5-key-functions, docs/reference/functions/hud-ui.md#hud-0x210eb0-0x21bb88
 * @end-sly2-annot */
// True when an exclusive widget of higher priority is shown.
int FWidgetObscured(struct S *widget)
{
    struct S *other;
    int unused[4];   /* an unused 16-byte stack frame in the original */
    for (other = (*(struct S * *)&g_sw.dlWidgetExclusive); other; other = other->next) {
        if (widget->priority < other->priority)
            return 1;
    }
    return 0;
}
} } // namespace M_FWidgetObscured

// 0x213830 StartWidgetMainAnim
namespace M_StartWidgetMainAnim { extern "C" {
struct VE { short d; short pad; void (*pf)(void *, int); };
struct S { char pad[0xC]; struct VE *vt; char pad2[0x6C]; int mainAnimId; char pad3[4]; void *spriteGroups; char pad4[0x24]; int isExclusive; };
void StartSpriteAnimById(void *p);
void SetWidgetExclusive(struct S *p, int a);
/** @sly2-annot addr=0x213830
 * Purpose: start anim id on main sprite (+0x84), slot21(1), join exclusive list if flag, +0x7C = id
 * Info: proposed name StartWidgetMainAnim (T1); TU hud
 * Confidence: mid (draft) [T1 0.50] - evidence: static analysis; sources: r7c_hud
 * See: docs/reference/functions/hud-ui.md#hud-0x210eb0-0x21bb88
 * @end-sly2-annot */
// Starts an animation on the widget's main sprite and joins the exclusive list if the widget is exclusive.
void StartWidgetMainAnim(struct S *widget, int animId)
{
    StartSpriteAnimById(widget->spriteGroups);
    widget->vt[21].pf((char *)widget + widget->vt[21].d, 1);   /* slot 21 (1 = on) */
    if (widget->isExclusive)
        SetWidgetExclusive(widget, 0);
    widget->mainAnimId = animId;
}
} } // namespace M_StartWidgetMainAnim

// 0x213898 StopWidgetMainAnim
namespace M_StopWidgetMainAnim {
extern "C" void StopSprite(void *p);
struct B { char pad[0xC];
  virtual void v1(); virtual void v2(); virtual void v3(int, int, int, int, int);
  virtual void v4(); virtual void v5(); virtual void v6(); virtual void v7(); virtual void v8();
  virtual void v9(); virtual void v10(); virtual void v11(); virtual void v12(); virtual void v13();
  virtual void v14(); virtual void v15(); virtual void v16(); virtual void v17(); virtual void v18();
  virtual void v19(); virtual void v20(); virtual void v21(int);
};
struct S : B {
  char pad10[0x6C]; int mainAnimId; char pad80[4]; void *spriteGroups; int mainAnimTime /* a float, cleared as a word */; char pad8C[0x20]; int isExclusive;
  void SetExclusive(int) __asm__("SetWidgetExclusive");
  void StopMainAnim() __asm__("StopWidgetMainAnim");
};
// StopWidgetMainAnim: stops the widget's main sprite and leaves the exclusive list.
void S::StopMainAnim()
{
    StopSprite(spriteGroups);
    if (isExclusive)
        SetExclusive(-1);
    v21(0);
    v3(6, 0, 0, 0, 0);   /* HandleSpliceEvent slot: event 6 (inferred) */
    mainAnimTime = 0;
    mainAnimId = -1;
}
} // namespace M_StopWidgetMainAnim

// 0x213928 StopWidgetAnim
namespace M_StopWidgetAnim { extern "C" {
struct E { char pad[0x14]; int id; };
struct C { char pad[8]; short animIndex; char pada[0xE]; };
struct A { unsigned short numAnims; unsigned short numSlots; struct E *anims; struct C *slots; char padc[0x14]; int currentAnim; char pad24[0xC]; };
struct S { char pad[0x80]; unsigned short numSpriteGroups; short pad82; struct A *spriteGroups; };
void RestartNestedSprite(struct S *widget, int groupIndex, int animIndex, int y, int z);
/** @sly2-annot addr=0x213928
 * Purpose: reset the sprite holding anim id back to its rest anim
 * Info: proposed name StopWidgetAnim (T1); TU hud
 * Confidence: mid (draft) [T1 0.60] - evidence: static analysis; sources: r7c_hud
 * See: docs/reference/functions/hud-ui.md#hud-0x210eb0-0x21bb88
 * @end-sly2-annot */
// Returns the sprite that plays an animation id to its rest animation.
void StopWidgetAnim(struct S *widget, int animId)
{
    int groupIndex, animIndex, i;
    for (groupIndex = 0; groupIndex < widget->numSpriteGroups; groupIndex++) {
        struct A *group = &widget->spriteGroups[groupIndex];
        for (animIndex = 0; animIndex < group->numAnims; animIndex++) {
            struct E *anim = &group->anims[animIndex];
            if (animId == anim->id) {
                int isAnySlotPlaying = 0;
                for (i = 0; i < group->numSlots; i++) {
                    if (group->slots[i].animIndex != -1)
                        isAnySlotPlaying = 1;
                }
                if (isAnySlotPlaying)
                    RestartNestedSprite(widget, groupIndex, 0, 0, 0);
                else
                    group->currentAnim = -1;
                return;
            }
        }
    }
}
} } // namespace M_StopWidgetAnim

// 0x213A08 PlayWidgetAnimsWithArg
namespace M_func_00213A08 { extern "C" {
struct E { char pad[0x14]; int id; };
struct A { unsigned short numAnims; short pad; struct E *anims; char pad8[0x28]; };
struct S { char pad[0x80]; unsigned short numSpriteGroups; short pad82; struct A *spriteGroups; };
void RestartNestedSpriteWithArg(struct S *widget, int groupIndex, int animIndex, int x, int y, int z);
/** @sly2-annot addr=0x213A08
 * Purpose: start every sprite holding anim id with argument x
 * Info: proposed name PlayWidgetAnimsArg (T2); TU hud
 * Notes: names_v2 v1 proposed render_level_info (0.033)
 * Confidence: low (draft) [T2 0.35] - evidence: static naming pass; sources: r7c_hud
 * See: docs/reference/functions/hud-ui.md#hud-0x210eb0-0x21bb88
 * @end-sly2-annot */
// Starts every sprite that has the animation id, with an argument. (inferred)
void PlayWidgetAnimsWithArg(struct S *widget, int animId, int arg)
{
    int groupIndex, animIndex;
    for (groupIndex = 0; groupIndex < widget->numSpriteGroups; groupIndex++) {
        struct A *group = &widget->spriteGroups[groupIndex];
        for (animIndex = 0; animIndex < group->numAnims; animIndex++) {
            struct E *anim = &group->anims[animIndex];
            if (animId == anim->id)
                RestartNestedSpriteWithArg(widget, groupIndex, 0, arg, 0, 0);
        }
    }
}
} } // namespace M_func_00213A08

// 0x213B00 PlayWidgetAnim
namespace M_PlayWidgetAnim { extern "C" {
struct E { char pad[0x14]; int id; };
struct C { char pad[8]; short animIndex; char pada[0xE]; };
struct A { unsigned short numAnims; unsigned short numSlots; struct E *anims; struct C *slots; char padc[0x14]; int currentAnim; char pad24[0xC]; };
struct S { char pad[0x80]; unsigned short numSpriteGroups; short pad82; struct A *spriteGroups; };
void RestartNestedSprite(struct S *widget, int groupIndex, int animIndex, int y, int z);
/** @sly2-annot addr=0x213B00
 * Purpose: find sprite owning anim id and start it (restart in place if instanced)
 * Info: proposed name PlayWidgetAnim (T1); TU hud
 * Notes: names_v2 C proposed SetupDraw__5CFontP8CTextBoxP4GIFS (0.438)
 * Confidence: mid (draft) [T1 0.65] - evidence: static naming pass; sources: r7c_hud
 * See: docs/reference/functions/hud-ui.md#hud-0x210eb0-0x21bb88
 * @end-sly2-annot */
// Starts the animation id on the sprite that owns it (restarting in place if it is instanced).
void PlayWidgetAnim(struct S *widget, int animId)
{
    int groupIndex, animIndex, i;
    if (animId == -1)
        return;
    for (groupIndex = 0; groupIndex < widget->numSpriteGroups; groupIndex++) {
        struct A *group = &widget->spriteGroups[groupIndex];
        for (animIndex = 0; animIndex < group->numAnims; animIndex++) {
            struct E *anim = &group->anims[animIndex];
            if (animId == anim->id) {
                int isAnySlotPlaying = 0;
                for (i = 0; i < group->numSlots; i++) {
                    if (group->slots[i].animIndex != -1)
                        isAnySlotPlaying = 1;
                }
                if (isAnySlotPlaying)
                    RestartNestedSprite(widget, groupIndex, animIndex, 0, 0);
                else
                    group->currentAnim = animIndex;
                return;
            }
        }
    }
}
} } // namespace M_PlayWidgetAnim

// 0x213BD8 PlayWidgetAnimWithArg
namespace M_func_00213BD8 { extern "C" {
struct E { char pad[0x14]; int id; };
struct A { unsigned short numAnims; short pad; struct E *anims; char pad8[0x28]; };
struct S { char pad[0x80]; unsigned short numSpriteGroups; short pad82; struct A *spriteGroups; };
void RestartNestedSpriteWithArg(struct S *widget, int groupIndex, int animIndex, int x, int y, int z);
/** @sly2-annot addr=0x213BD8
 * Purpose: start first anim id match with argument x
 * Info: proposed name PlayWidgetAnimArg (T2); TU hud
 * Notes: names_v2 C proposed DxFromPchz__5CFontPc (0.438)
 * Confidence: low (draft) [T2 0.40] - evidence: static naming pass; sources: r7c_hud
 * See: docs/reference/functions/hud-ui.md#hud-0x210eb0-0x21bb88
 * @end-sly2-annot */
// Starts the first sprite that has the animation id, with an argument. (inferred)
void PlayWidgetAnimWithArg(struct S *widget, int animId, int arg)
{
    int groupIndex, animIndex;
    for (groupIndex = 0; groupIndex < widget->numSpriteGroups; groupIndex++) {
        struct A *group = &widget->spriteGroups[groupIndex];
        for (animIndex = 0; animIndex < group->numAnims; animIndex++) {
            struct E *anim = &group->anims[animIndex];
            if (animId == anim->id) {
                RestartNestedSpriteWithArg(widget, groupIndex, animIndex, arg, 0, 0);
                return;
            }
        }
    }
}
} } // namespace M_func_00213BD8

// 0x213C68 SetWidgetXf
namespace M_SetWidgetXf { extern "C" {
typedef int qword __attribute__((mode(TI)));
struct M { qword a, b; };   /* a 2x3 matrix */
struct VE { short d; short pad; struct M *(*pf)(void *, int, int); };
struct A { struct VE *vt; };   /* a heap; slot 1 allocates */
struct S { char pad[0xC4]; struct A *spriteHeap; char pad2[0x30]; struct M *xform; };
/** @sly2-annot addr=0x213C68
 * Purpose: set widget 2D transform override (alloc 0x20 M23 at +0xF8)
 * Info: proposed name SetWidgetXf (T1); TU hud
 * Notes: names_v2 v1 proposed FUN_001ad6a8 (0.000)
 * Confidence: mid (draft) [T1 0.55] - evidence: static naming pass; sources: r7c_hud
 * See: docs/reference/functions/hud-ui.md#hud-0x210eb0-0x21bb88
 * @end-sly2-annot */
// Sets the widget's 2D transform override.
void SetWidgetXf(struct S *widget, struct M *src)
{
    if (!widget->xform) {
        struct A *heap = widget->spriteHeap;
        widget->xform = heap->vt[1].pf((char *)heap + heap->vt[1].d, 0x20, 0x1F);
    }
    *widget->xform = *src;
}
} } // namespace M_SetWidgetXf

// 0x213CD8 SetWidgetAnimXf
namespace M_SetWidgetAnimXf {
struct M { int a[8]; } __attribute__((aligned(16)));
struct H { virtual void *Alloc(int cb, int animId); };
struct E { char pad[0x14]; int id; };
struct A { unsigned short numAnims; short pad; E *anims; char pad8[0x24]; M *xform; };
struct S { char pad[0x80]; unsigned short numSpriteGroups; short pad82; A *spriteGroups; char pad88[0x3C]; H *spriteHeap; };
/** @sly2-annot addr=0x213CD8
 * Purpose: set M23 transform of the sprite that holds anim id
 * Info: proposed name SetWidgetAnimXf (T1); TU hud
 * Confidence: high [T1 0.85] - evidence: static analysis; sources: r7c_hud+verdicts_followups
 * See: docs/engine/hud-ui.md#5-key-functions, docs/reference/functions/hud-ui.md#hud-0x210eb0-0x21bb88
 * @end-sly2-annot */
// Sets the 2D transform of the sprite that plays an animation id.
extern "C" void SetWidgetAnimXf(S *widget, int animId, M *xform)
{
    int groupIndex, animIndex;
    for (groupIndex = 0; groupIndex < widget->numSpriteGroups; groupIndex++) {
        A *group = &widget->spriteGroups[groupIndex];
        for (animIndex = 0; animIndex < group->numAnims; animIndex++) {
            E *anim = &group->anims[animIndex];
            if (anim->id == animId) {
                if (!group->xform)
                    group->xform = (M *)widget->spriteHeap->Alloc(0x20, 0x1F);
                *group->xform = *xform;
                return;
            }
        }
    }
}
} // namespace M_SetWidgetAnimXf

// 0x213DB0 SwitchWidgetAnim
namespace M_SwitchWidgetAnim { extern "C" {
void StopWidgetAnim(void *p, int k);
void PlayWidgetAnim(void *p, int k);
/** @sly2-annot addr=0x213DB0
 * Purpose: stop old *pid, play new id, store
 * Info: proposed name SwitchWidgetAnim (T1); TU hud
 * Confidence: mid (draft) [T1 0.55] - evidence: static analysis; sources: r7c_hud
 * See: docs/reference/functions/hud-ui.md#hud-0x210eb0-0x21bb88
 * @end-sly2-annot */
// Stops the animation in *pk, plays the new one and stores its id.
void SwitchWidgetAnim(void *widget, int animId, int *currentAnimId)
{
    if (animId == *currentAnimId)
        return;
    if (*currentAnimId != -1)
        StopWidgetAnim(widget, *currentAnimId);
    *currentAnimId = animId;
    if (animId != -1)
        PlayWidgetAnim(widget, animId);
}
} } // namespace M_SwitchWidgetAnim

// 0x213E20 FindSpriteItemPose
namespace M_func_00213E20 { extern "C" {
typedef int qword __attribute__((mode(TI)));
struct RGBA { unsigned char r, g, b, a; };
struct QQ { qword a, b; };
struct Z { int unknown0; int kind; char pad8[8]; int groupIndex; };   /* an item descriptor; kind 4 = a nested sprite group */
struct Y { struct RGBA color0; struct RGBA color1; char pad8[8]; struct QQ matrix; struct Z *descriptor; char pad34[0x24]; int spriteSlot; char pad5c[0x14]; };
struct H { int numItems; struct Y *items; char pad8[0x10]; };
struct A { unsigned short numAnims; unsigned short numSlots; void *anims; struct H *slots; char padc[0x24]; };
struct S { char pad[0x84]; struct A *spriteGroups; };
/** @sly2-annot addr=0x213E20
 * Purpose: recursive search of nested sprite k: return its M23 and colours
 * Info: proposed name FFindSpriteItemPose (T2); TU hud
 * Notes: names_v2 B proposed ClineWrapPchz__5CFontPcf (0.856)
 * Confidence: low (draft) [T2 0.40] - evidence: static naming pass; sources: r7c_hud
 * See: docs/reference/functions/hud-ui.md#hud-0x210eb0-0x21bb88
 * @end-sly2-annot */
// Recursively finds nested sprite k and returns its 2D transform and colours. (inferred)
int FindSpriteItemPose(struct S *widget, int groupIndex, struct QQ *matrix, struct RGBA *color0, struct RGBA *color1, int numItems, struct Y *items)
{
    int i;
    struct Y *item;
    if (!items) {
        struct H *slot = widget->spriteGroups->slots;
        numItems = slot->numItems;
        items = slot->items;
    }
    for (i = 0; i < numItems; i++) {
        struct Z *descriptor;
        item = &items[i];
        descriptor = item->descriptor;
        if (!descriptor || descriptor->kind != SPRITE_KIND_NESTED)
            continue;
        if (groupIndex == descriptor->groupIndex) {
            *matrix = item->matrix;
            *color0 = item->color0;
            *color1 = item->color1;
            return 1;
        }
        if (FindSpriteItemPose(widget, groupIndex, matrix, color0, color1, widget->spriteGroups[descriptor->groupIndex].slots[item->spriteSlot].numItems, widget->spriteGroups[descriptor->groupIndex].slots[item->spriteSlot].items))
            return 1;
    }
    return 0;
}
} } // namespace M_func_00213E20

// 0x213F70 RestartNestedSprite
namespace M_func_00213F70 {
typedef int qword __attribute__((mode(TI)));
struct RGBA { unsigned int u; RGBA(unsigned int x) { u = x; } } __attribute__((aligned(16)));
struct QQ { qword a, b; };
struct Z { int unknown0; int kind; char pad8[8]; int groupIndex; };   /* an item descriptor; kind 4 = a nested sprite group */
struct Y { int color0; int color1; char pad8[8]; QQ matrix; Z *descriptor; char pad34[0x24]; int spriteSlot; char pad5c[0xC]; int link; int unknown6C; };
struct H { int numItems; Y *items; char pad8[0x10]; };
struct A { unsigned short numAnims; unsigned short numSlots; void *anims; H *slots; char padc[0x24]; };
struct S { char pad[0x84]; A *spriteGroups; };
extern "C" void StopSpriteSlot(A *pa, int i);
extern "C" void ClearSle(int *p);
extern "C" int IFreeSpriteSlot(A *pa);
extern "C" void StartSpriteAnimAt(A *pa, int i, int z, void *q, float f, const RGBA &c0, const RGBA &c1);
/** @sly2-annot addr=0x213F70
 * Purpose: recursively find nested-sprite item k and restart it at anim z
 * Info: proposed name RestartNestedSprite (T2); TU hud
 * Notes: names_v2 v1 proposed FUN_001ad970 (0.000)
 * Confidence: low (draft) [T2 0.40] - evidence: static naming pass; sources: r7c_hud
 * See: docs/reference/functions/hud-ui.md#hud-0x210eb0-0x21bb88
 * @end-sly2-annot */
// Recursively finds nested sprite item k and restarts it on animation z. (inferred)
extern "C" int RestartNestedSprite(S *widget, int groupIndex, int animIndex, int numItems, Y *items)
{
    int i;
    Y *item;
    if (!items) {
        H *slot = widget->spriteGroups->slots;
        numItems = slot->numItems;
        items = slot->items;
    }
    for (i = 0; i < numItems; i++) {
        Z *descriptor;
        item = &items[i];
        descriptor = item->descriptor;
        if (!descriptor || descriptor->kind != SPRITE_KIND_NESTED)
            continue;
        int oldSlot=item->spriteSlot;
        if (descriptor->groupIndex == groupIndex) {
            StopSpriteSlot(&widget->spriteGroups[groupIndex], oldSlot);
            ClearSle(&item->link);
            item->spriteSlot = IFreeSpriteSlot(&widget->spriteGroups[groupIndex]);
            StartSpriteAnimAt(&widget->spriteGroups[groupIndex], animIndex, item->spriteSlot, &item->matrix, 0.0f, RGBA(0x7F7F7F7F), RGBA(0xFFFFFFFF));
            return 1;
        }
        {
            /* &spriteGroups[...].slots[oldSlot], spelled as byte arithmetic: the indexed form schedules differently */
            H *slot = (H *)(oldSlot * sizeof(H) + (int)widget->spriteGroups[descriptor->groupIndex].slots);
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
struct RGBA { unsigned int u; RGBA(unsigned int x) { u = x; } } __attribute__((aligned(16)));
struct QQ { qword a, b; };
struct Z { int unknown0; int kind; char pad8[8]; int groupIndex; };   /* an item descriptor; kind 4 = a nested sprite group */
struct Y { int color0; int color1; char pad8[8]; QQ matrix; Z *descriptor; char pad34[0x24]; int spriteSlot; char pad5c[8]; int arg; int link; int unknown6C; };
struct H { int numItems; Y *items; char pad8[0x10]; };
struct A { unsigned short numAnims; unsigned short numSlots; void *anims; H *slots; char padc[0x24]; };
struct S { char pad[0x84]; A *spriteGroups; };
extern "C" void StopSpriteSlot(A *pa, int i);
extern "C" void ClearSle(int *p);
extern "C" int IFreeSpriteSlot(A *pa);
extern "C" void StartSpriteAnimAt(A *pa, int i, int z, void *q, float f, const RGBA &c0, const RGBA &c1);
/** @sly2-annot addr=0x214100
 * Purpose: as RestartNestedSprite with extra argument
 * Info: proposed name RestartNestedSpriteArg (T2); TU hud
 * Notes: names_v2 v1 proposed DrawLogo__FP4LOGO (0.003)
 * Confidence: low (draft) [T2 0.35] - evidence: static naming pass; sources: r7c_hud
 * See: docs/reference/functions/hud-ui.md#hud-0x210eb0-0x21bb88
 * @end-sly2-annot */
// RestartNestedSprite with an extra argument. (inferred)
extern "C" int RestartNestedSpriteWithArg(S *widget, int groupIndex, int animIndex, int arg, int numItems, Y *items)
{
    int i;
    Y *item;
    if (!items) {
        H *slot = widget->spriteGroups->slots;
        numItems = slot->numItems;
        items = slot->items;
    }
    for (i = 0; i < numItems; i++) {
        Z *descriptor;
        item = &items[i];
        descriptor = item->descriptor;
        if (!descriptor || descriptor->kind != SPRITE_KIND_NESTED)
            continue;
        int oldSlot=item->spriteSlot;
        if (descriptor->groupIndex == groupIndex) {
            if (arg != item->arg)
                continue;
            StopSpriteSlot(&widget->spriteGroups[groupIndex], oldSlot);
            ClearSle(&item->link);
            item->spriteSlot = IFreeSpriteSlot(&widget->spriteGroups[groupIndex]);
            StartSpriteAnimAt(&widget->spriteGroups[groupIndex], animIndex, item->spriteSlot, &item->matrix, 0.0f, RGBA(0x7F7F7F7F), RGBA(0xFFFFFFFF));
            return 1;
        }
        {
            /* &spriteGroups[...].slots[oldSlot], spelled as byte arithmetic: the indexed form schedules differently */
            H *slot = (H *)(oldSlot * sizeof(H) + (int)widget->spriteGroups[descriptor->groupIndex].slots);
            if (RestartNestedSpriteWithArg(widget, groupIndex, animIndex, arg, slot->numItems, slot->items))
                return 1;
        }
    }
    return 0;
}
} // namespace M_func_00214100

// 0x2142B0 FSpriteHasAnim
namespace M_FSpriteHasAnim { extern "C" {
struct E { char pad[0x14]; int id; };
struct S { unsigned short numAnims; short pad; struct E *anims; };   /* a sprite group */
/** @sly2-annot addr=0x2142B0
 * Purpose: id in sprite anim list
 * Info: proposed name FSpriteHasAnim (T1); TU hud
 * Notes: names_v2 v1 proposed FUN_001ae130 (0.000)
 * Confidence: mid (draft) [T1 0.55] - evidence: static naming pass; sources: r7c_hud
 * See: docs/reference/functions/hud-ui.md#hud-0x210eb0-0x21bb88
 * @end-sly2-annot */
// True when the sprite's animation list contains the id.
int FSpriteHasAnim(void *pv, struct S *group, int id)
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
} } // namespace M_FSpriteHasAnim

// 0x214300 SetWidgetExclusive
/** @sly2-annot addr=0x214300
 * Purpose: flag +0xA8; value 0 = in exclusive list g_sw.dlWidgetExclusive (suppresses lower-priority widgets)
 * Info: proposed name SetWidgetExclusive (T2); TU hud
 * Notes: names_v2 v1 proposed UpdateAttract__FP7ATTRACT (0.024)
 * Confidence: low (draft) [T2 0.45] - evidence: static naming pass; sources: r7c_hud
 * See: docs/reference/functions/hud-ui.md#hud-0x210eb0-0x21bb88
 * @end-sly2-annot */
/* Sets the widget's +0xA8 flag; a widget with the flag clear is kept on the exclusive list g_sw.dlWidgetExclusive. */
// Sets whether the widget is exclusive (an exclusive widget hides lower-priority widgets while shown). (inferred)
extern "C" void SetWidgetExclusive(WIDGETV *widget, int isNonExclusive)
{
    int wasNonExclusive = widget->isNonExclusive;
    if (wasNonExclusive == isNonExclusive)
        return;
    if (!wasNonExclusive && FFindDlEntry(&g_sw.dlWidgetExclusive, widget))
        RemoveDlEntry(&g_sw.dlWidgetExclusive, widget);
    widget->isNonExclusive = isNonExclusive;
    if (!isNonExclusive && !FFindDlEntry(&g_sw.dlWidgetExclusive, widget))
        AppendDlEntry(&g_sw.dlWidgetExclusive, widget);
}

// 0x214398 LoadSpriteTable
namespace M_LoadSpriteTable { extern "C" {
struct S { char pad[0xC4]; void *spriteHeap; };
extern int D_002C40A4;
int U16Read__18CBinaryInputStream(void *a);
int PvAllocClear__FP4HEAPii(void *heap, int cb, int k);
void LoadSpriteDescriptor(struct S *p, void *a, void *b, int n, int m);
/** @sly2-annot addr=0x214398
 * Purpose: read sprite count, alloc (n+1)*0x40, read each descriptor
 * Info: proposed name LoadSpriteTable (T1); TU hud
 * Notes: names_v2 v1 proposed FUN_001ae510 (0.000) | level_parser CONFIRMED-parse: payload layout followed by
 *   the parser, 100%% of bytes in 4 levels
 * Confidence: mid (draft) [T1 0.75] - evidence: static naming pass; sources: r7c_hud+parse-verified
 * See: docs/engine/hud-ui.md#5-key-functions,
 *   docs/formats/level-objects.md#8-widget-payload-loadwidgetfrombrx-0x212298-cparse,
 *   docs/reference/functions/hud-ui.md#hud-0x210eb0-0x21bb88
 * @end-sly2-annot */
// Loads a sprite table: the count, then each sprite descriptor.
void LoadSpriteTable(struct S *p, void *a, void *b, int *pn, int *pm)
{
    int i;
    *pn = U16Read__18CBinaryInputStream(a);
    *pm = PvAllocClear__FP4HEAPii(p->spriteHeap, (*pn + 1) << 6, D_002C40A4 ? 0x13 : 0x11);
    for (i = 1; i <= *pn; i++)
        LoadSpriteDescriptor(p, a, b, *pn, *pm);
}
} } // namespace M_LoadSpriteTable

// 0x214460 LoadSpriteGroups
namespace M_LoadSpriteGroups {
// Deserialize sprite groups, reusable frames and their animation keys.
typedef unsigned int uint;
typedef unsigned short ushort;
typedef unsigned char uchar;
struct BIS;
struct HEAP;
struct ROOT;
struct GROUP;
struct FRAME {char fields[0x70];};
struct V3 {float x,y,z;} __attribute__((aligned(16)));
struct M3 {V3 row[3];};
struct M23 {float a[6];} __attribute__((aligned(16)));
struct RGBA_RAW {uchar r,g,b,a;};
union RGBA {uint word;RGBA_RAW raw;RGBA(uint value){word=value;}} __attribute__((aligned(16)));
struct COLORS {RGBA_RAW first,second;};
struct ITEM {ushort a0,a2,a4;short a6;char pad8[8];M23 matrix;uchar flags,mode;char pad32[2];int id;char pad38[8];};
struct TAG {uint kind;union {int signedBits;uint bits;} value;};
struct KEY {float time;uchar itemCount,colorCount,shortCount,byteCount;uchar *bytes;ITEM *items;COLORS *colors;ushort *shorts;uchar tagCount;char pad19[3];TAG *tags;};
struct ANIM {ushort count,pad2;KEY *keys;float duration;ROOT *owner;GROUP *group;int id;};
struct REUSE {int count;FRAME *frames;short index,phase;void *previous;float elapsed;int pad14;};
struct GROUP {ushort animCount,reuseCount;ANIM *animations;REUSE *reuse;char padC[0x14];int index;ROOT *owner;int first;int pad2C;};
struct ROOT {char pad[0x80];ushort groupCount,pad82;GROUP *groups;};
struct OWNER {char pad[0xC4];HEAP *heap;};
extern "C" {
int U16Read__18CBinaryInputStream(BIS *);
uint U32Read__18CBinaryInputStream(BIS *);
int U8Read__18CBinaryInputStream(BIS *);
int S16Read__18CBinaryInputStream(BIS *);
int S32Read__18CBinaryInputStream(BIS *);
float F32Read__18CBinaryInputStream(BIS *);
void ReadMatrix__18CBinaryInputStreamP7MATRIX3(BIS *,M3 *);
void *PvAllocClear__FP4HEAPii(HEAP *,int,int);
void InitSpriteItem(FRAME *);
}
static inline void CopyXY(M23 *dst,const M3 &src){
 dst->a[0]=src.row[0].x;dst->a[1]=src.row[0].y;
 dst->a[2]=src.row[1].x;dst->a[3]=src.row[1].y;
 dst->a[4]=src.row[2].x;dst->a[5]=src.row[2].y;
}
/** @sly2-annot addr=0x214460
 * Purpose: deserialize sprite groups, reusable frames and animation keys
 * Info: proposed name LoadSpriteGroups (T1); TU hud
 * Notes: level_parser CONFIRMED-parse: payload layout followed by the parser, 100%% of bytes in 4 levels
 * Confidence: mid (draft) [T1 0.75] - evidence: static analysis; sources: r7c_hud+parse-verified
 * See: docs/engine/hud-ui.md#5-key-functions,
 *   docs/formats/level-objects.md#8-widget-payload-loadwidgetfrombrx-0x212298-cparse,
 *   docs/reference/functions/hud-ui.md#hud-0x210eb0-0x21bb88
 * @end-sly2-annot */
// Loads sprite groups, their reusable frames and animation keys.
extern "C" void LoadSpriteGroups(OWNER *owner,BIS *stream,ROOT *root){
 int i,j,k,m;
 root->groupCount=U16Read__18CBinaryInputStream(stream);
 root->groups=(GROUP *)PvAllocClear__FP4HEAPii(owner->heap,(int)root->groupCount*0x30,0x12);
 for(i=0;i<root->groupCount;i++){
  GROUP *group=&root->groups[i];
  group->first=(i==0);group->owner=root;
  group->reuseCount=U16Read__18CBinaryInputStream(stream);
  group->reuse=(REUSE *)PvAllocClear__FP4HEAPii(owner->heap,(int)group->reuseCount*0x18,0x12);
  int count=U16Read__18CBinaryInputStream(stream);
  for(j=0;j<group->reuseCount;){
   int next=j+1;
   REUSE *reuse=&group->reuse[j];
   reuse->previous=0;reuse->index=-1;reuse->phase=0;reuse->elapsed=0.0f;
   reuse->count=count;
   reuse->frames=(FRAME *)PvAllocClear__FP4HEAPii(owner->heap,count*0x70,0x10);
   for(m=0;m<reuse->count;m++)InitSpriteItem(&reuse->frames[m]);
   j=next;
  }
  group->animCount=U16Read__18CBinaryInputStream(stream);
  group->animations=(ANIM *)PvAllocClear__FP4HEAPii(owner->heap,(int)group->animCount*0x18,0x12);
  group->index=-1;
  for(j=0;j<group->animCount;j++){
   ANIM *animation=&group->animations[j];
   animation->owner=root;animation->group=group;
   animation->duration=F32Read__18CBinaryInputStream(stream);
   animation->id=S32Read__18CBinaryInputStream(stream);
   animation->count=U16Read__18CBinaryInputStream(stream);
   animation->keys=(KEY *)PvAllocClear__FP4HEAPii(owner->heap,(int)animation->count*0x20,0x12);
   for(k=0;k<animation->count;k++){
    KEY *key=&animation->keys[k];
    key->time=F32Read__18CBinaryInputStream(stream);
    key->itemCount=U8Read__18CBinaryInputStream(stream);
    key->items=(ITEM *)PvAllocClear__FP4HEAPii(owner->heap,(int)key->itemCount*0x40,0x12);
    for(m=0;m<key->itemCount;m++){
     ITEM *item=&key->items[m];
     item->a0=U16Read__18CBinaryInputStream(stream);item->a2=U16Read__18CBinaryInputStream(stream);item->a4=U16Read__18CBinaryInputStream(stream);item->a6=S16Read__18CBinaryInputStream(stream);
     item->flags=U8Read__18CBinaryInputStream(stream);item->mode=U8Read__18CBinaryInputStream(stream);item->id=S32Read__18CBinaryInputStream(stream);
     M3 raw;ReadMatrix__18CBinaryInputStreamP7MATRIX3(stream,&raw);M3 matrix=raw;CopyXY(&item->matrix,matrix);
    }
    key->colorCount=U8Read__18CBinaryInputStream(stream);
    key->colors=(COLORS *)PvAllocClear__FP4HEAPii(owner->heap,(int)key->colorCount*8,0x12);
    for(m=0;m<key->colorCount;m++){
     COLORS *colors=&key->colors[m];
     RGBA second(U32Read__18CBinaryInputStream(stream));RGBA first(U32Read__18CBinaryInputStream(stream));
     colors->second=*(RGBA_RAW *)&second;colors->first=*(RGBA_RAW *)&first;
    }
    key->shortCount=U8Read__18CBinaryInputStream(stream);
    key->shorts=(ushort *)PvAllocClear__FP4HEAPii(owner->heap,(int)key->shortCount*2,0x12);
    for(m=0;m<key->shortCount;m++){
     ushort *dst=&key->shorts[m];*dst=U16Read__18CBinaryInputStream(stream);
    }
    key->byteCount=U8Read__18CBinaryInputStream(stream);
    key->bytes=(uchar *)PvAllocClear__FP4HEAPii(owner->heap,(int)key->byteCount,0x12);
    for(int byteIndex=0;byteIndex<key->byteCount;byteIndex++)key->bytes[byteIndex]=U8Read__18CBinaryInputStream(stream);
    key->tagCount=U8Read__18CBinaryInputStream(stream);
    key->tags=(TAG *)PvAllocClear__FP4HEAPii(owner->heap,(int)key->tagCount*8,0x12);
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
struct OWNER {char pad[0xC4];HEAP *heap;};
struct ROOT;
struct WORLD;
struct SPRITE;
struct M3 {qword r[3];};
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
extern "C" {
int U16Read__18CBinaryInputStream(BIS *);
uint U32Read__18CBinaryInputStream(BIS *);
int U8Read__18CBinaryInputStream(BIS *);
int S16Read__18CBinaryInputStream(BIS *);
int S8Read__18CBinaryInputStream(BIS *);
int S32Read__18CBinaryInputStream(BIS *);
float F32Read__18CBinaryInputStream(BIS *);
void ReadMatrix__18CBinaryInputStreamP7MATRIX3(BIS *,M3 *);
void Read__18CBinaryInputStreamiPv(BIS *,int,void *);
void *PvAllocClear__FP4HEAPii(HEAP *,int,int);
void *AllocClearedFromStack(ALLOCREQ *);
void LoadSpriteTable(OWNER *,BIS *,ROOT *,int *,SPRITE **);
void PostSwCallback(WORLD *,OWNER *,PMF,SPRITE *);
}
extern "C" { extern int D_002C40A4; }
extern "C" { extern PMF D_002A8B08; }
extern "C" { extern WORLD D_003DD490; }
extern "C" { extern QW D_002A2E10; }
extern "C" { extern char D_00000650[],D_00000140[]; }
static inline void *Acquire(ALLOCREQ *request,int bytes,int alignment){request->cb=bytes;request->cbAlign=alignment;return AllocClearedFromStack(request);}
static inline void CopyXY(M23 &dst,const M3 &src){
 const float *a=(const float *)&src.r[0],*b=(const float *)&src.r[1],*c=(const float *)&src.r[2];
 dst.a[0]=a[0];dst.a[1]=a[1];dst.a[2]=b[0];dst.a[3]=b[1];dst.a[4]=c[0];dst.a[5]=c[1];
}
/** @sly2-annot addr=0x2149D8
 * Purpose: read one sprite descriptor (kinds 1 geometry,2 polygon,3 shader,4 index,6/7 picture,8 nested) and
 *   build packets
 * Info: proposed name LoadSpriteDescriptor (T1); TU hud
 * Notes: level_parser CONFIRMED-parse: payload layout followed by the parser, 100%% of bytes in 4 levels
 * Confidence: mid (draft) [T1 0.75] - evidence: static analysis; sources: r7c_hud+parse-verified
 * See: docs/engine/hud-ui.md#5-key-functions,
 *   docs/formats/level-objects.md#8-widget-payload-loadwidgetfrombrx-0x212298-cparse,
 *   docs/reference/functions/hud-ui.md#hud-0x210eb0-0x21bb88
 * @end-sly2-annot */
// Loads one sprite descriptor (geometry, polygon, shader, index, picture or nested sprite) and builds its packets.
extern "C" void LoadSpriteDescriptor(OWNER *const owner,BIS *stream,ROOT *root,int,SPRITE *sprites){
 STACKSCOPE outer;M3 raw;
 int id=U16Read__18CBinaryInputStream(stream);SPRITE *sprite=&sprites[id];sprite->root=root;sprite->id=id;
 sprite->kind=U32Read__18CBinaryInputStream(stream);
 switch(sprite->kind){
 case SPRITE_KIND_NONE:case SPRITE_KIND_UNUSED5:default:break;
 case SPRITE_KIND_NESTED:sprite->data.index=U16Read__18CBinaryInputStream(stream);break;
 case SPRITE_KIND_TABLE:
  D_002C40A4=1;
  LoadSpriteTable(owner,stream,root,&sprite->data.nested.count,&sprite->data.nested.children);
  sprite->data.nested.wordCount=U32Read__18CBinaryInputStream(stream);
  sprite->data.nested.words=(uint *)PvAllocClear__FP4HEAPii(owner->heap,sprite->data.nested.wordCount*4,0x13);
  int i;
  for(i=0;i<sprite->data.nested.wordCount;i++)sprite->data.nested.words[i]=U32Read__18CBinaryInputStream(stream);
  sprite->data.nested.floatCount=U32Read__18CBinaryInputStream(stream);
  int j;
  sprite->data.nested.floats=(float *)PvAllocClear__FP4HEAPii(owner->heap,sprite->data.nested.floatCount*4,0x13);
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
  ReadMatrix__18CBinaryInputStreamP7MATRIX3(stream,&raw);M3 matrix=raw;CopyXY(sprite->data.polygon.matrix,matrix);
  sprite->data.polygon.count=U16Read__18CBinaryInputStream(stream);
  sprite->data.polygon.vertices=(VERTEX *)PvAllocClear__FP4HEAPii(owner->heap,sprite->data.polygon.count*sizeof(VERTEX),0x14);
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
  else{vif.DetachCopySw(owner->heap,&sprite->data.picture.count,0,&sprite->data.picture.packet);PostSwCallback(&D_003DD490,owner,D_002A8B08,sprite);}
  break;
 }
 case SPRITE_KIND_GEOMETRY:{
  sprite->data.geometry.count=U16Read__18CBinaryInputStream(stream);
  RECORD *records;GROUP *groups;int i;
  { ALLOCREQ request;
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
extern "C" {
void STACKSCOPE_ctor(void *p);
void STACKSCOPE_ctor_1AE4A0(void *p, int n) __asm__("_$_10STACKSCOPE");
struct X { char pad[0x14]; unsigned long *tex0; };   /* a texture: its GS TEX0 register value */
X *FindShaderById(int);
void __4GIFS(void *);
void AllocSw__4DMASi(void *, int);
void AddPrimPack__4GIFSiiUl(void *, int, int, int);
void PackAD__4GIFSll(void *, int, unsigned long);
void AddPrimEnd__4GIFS(void *);
void DetachCopySw__4DMASPiPP2QWT2i(void *, int, int *, int, int *) __asm__("DetachCopySw__4DMASP4HEAPPiPP2QWT3");   /* DMAS::DetachCopySw(HEAP *, ...) (dmas.cpp) */
}
struct BUF { int a[12]; };
struct P { char pad[0xC4]; int spriteHeap; };
/* a picture sprite descriptor: kind, texture id, then the TEX packet built here */
struct Q { int id; int kind; char pad8[8]; int textureId; char pad14[8]; int texPacketCount; int texPacket; };
extern "C" void BuildSpritePictureTexPacket(P *p, Q *q);
/** @sly2-annot addr=0x2155C0
 * Purpose: kind-7 picture callback: GIF TEX0/TEXA packet for the texture of q->x10
 * Info: proposed name BuildSpritePictureTexPacket (T2); TU hud
 * Confidence: low (draft) [T2 0.35] - evidence: static analysis; sources: r7c_hud
 * See: docs/reference/functions/hud-ui.md#hud-0x210eb0-0x21bb88
 * @end-sly2-annot */
// Picture-sprite callback: the GIF packet that selects its texture. (inferred)
void BuildSpritePictureTexPacket(P *owner, Q *sprite)
{
    STACKSCOPE frame;
    if (sprite->kind == SPRITE_KIND_PICTURE) {
        sprite->texPacketCount = 0;
        sprite->texPacket = 0;
        X *texture = FindShaderById(sprite->textureId);
        if (texture) {
            BUF gifs;
            __4GIFS(&gifs);
            AllocSw__4DMASi(&gifs, 0x10);
            AddPrimPack__4GIFSiiUl(&gifs, 0, 1, 0xE);
            PackAD__4GIFSll(&gifs, 6, *texture->tex0);   /* TEX0_1 */
            PackAD__4GIFSll(&gifs, 0x14, 0x60);          /* TEX1_1 */
            PackAD__4GIFSll(&gifs, 8, 5);                /* CLAMP_1 */
            AddPrimEnd__4GIFS(&gifs);
            DetachCopySw__4DMASPiPP2QWT2i(&gifs, owner->spriteHeap, &sprite->texPacketCount, 0, &sprite->texPacket);
        }
    }
}
} // namespace M_func_002155C0

// 0x2156B8 InitSpriteItem
namespace M_InitSpriteItem {
struct RGBA { unsigned int u; RGBA() {} RGBA(unsigned int x) { u = x; } };
struct CP { RGBA c0, c1; };
/* A sprite frame item (0x70 bytes): three colour pairs, its descriptor, its slot in a nested group, an argument,
   a list link. */
struct S { CP colors; char pad[0x28]; int descriptor, unknown34, unknown38; CP colorsB; void *matrix; int unknown48; CP colorsC; int unknown54, spriteSlot, unknown5C; int pad2; int arg; char link[4]; };
extern "C" char D_002A3A10[];
extern "C" void ClearSle(void *p);
/** @sly2-annot addr=0x2156B8
 * Purpose: item defaults: colours 0x7F7F7F7F/0xFFFFFFFF, identity M23, ids -1
 * Info: proposed name InitSpriteItem (T1); TU hud
 * Confidence: mid (draft) [T1 0.50] - evidence: static analysis; sources: r7c_hud
 * See: docs/reference/functions/hud-ui.md#hud-0x210eb0-0x21bb88
 * @end-sly2-annot */
// Sets a sprite item's defaults: neutral colours, identity transform, no ids.
extern "C" void InitSpriteItem(S *item)
{
    item->matrix = D_002A3A10;   /* the identity 2x3 matrix (inferred) */
    item->descriptor = 0;
    item->unknown54 = 0;
    /* colour pairs: offset 0x7F (neutral), scale 0xFF (full) per channel */
    item->colors.c0 = RGBA(0x7F7F7F7F); item->colors.c1 = RGBA(0xFFFFFFFF);
    item->colorsB.c0 = RGBA(0x7F7F7F7F); item->colorsB.c1 = RGBA(0xFFFFFFFF);
    item->colorsC.c0 = RGBA(0x7F7F7F7F); item->colorsC.c1 = RGBA(0xFFFFFFFF);
    item->arg = -1;
    item->spriteSlot = 0;
    item->unknown34 = 0;
    item->unknown5C = 0;
    item->unknown38 = 0;
    item->unknown48 = 0;
    ClearSle(item->link);
}
} // namespace M_InitSpriteItem

// 0x215730 SetSpriteItemSource
namespace M_func_00215730 {
struct RGBA2 { unsigned char r, g, b, a; };
struct RGBA {
    unsigned char r, g, b, a;
    RGBA(const RGBA &c) { *(RGBA2 *)this = *(const RGBA2 *)&c; }
} __attribute__((aligned(16)));
struct L { char pad[0x84]; char *spriteGroups; };   /* the owning widget */
/* a sprite descriptor: index is the nested group (NESTED) or the texture resource id (EFFECT) */
struct S { int id; int kind; L *owner; int unknownC; int index; void *texture; };
struct T { char pad[0x30]; S *descriptor; char pad34[0x24]; int spriteSlot; };   /* a frame item */
extern "C" int IFreeSpriteSlot(void *p);
extern "C" void StartSpriteAnimSlot(void *p, int k, int z, void *q, RGBA c0, RGBA c1);
extern "C" void *FindObjectForRenderTarget(int);
extern "C" void *AcquireRenderTarget(void *);
/** @sly2-annot addr=0x215730
 * Purpose: bind item to payload by kind (4 nested sprite: start anim 0x314)
 * Info: proposed name SetSpriteItemSource (T2); TU hud
 * Notes: names_v2 C proposed PostLoad__8CFontBrxP3GSB (0.638)
 * Confidence: low (draft) [T2 0.35] - evidence: static naming pass; sources: r7c_hud
 * See: docs/reference/functions/hud-ui.md#hud-0x210eb0-0x21bb88
 * @end-sly2-annot */
// Binds a sprite item to its payload by kind (a nested sprite starts its main animation). (inferred)
extern "C" void SetSpriteItemSource(S *sprite, T *item, void *matrix, RGBA c0, RGBA c1)
{
    item->descriptor = sprite;
    switch (sprite->kind) {
    case SPRITE_KIND_NESTED:   /* groups are 0x30 bytes */
        item->spriteSlot = IFreeSpriteSlot(sprite->owner->spriteGroups + sprite->index * 0x30);
        StartSpriteAnimSlot(sprite->owner->spriteGroups + sprite->index * 0x30, 0x314, item->spriteSlot, matrix, c0, c1);
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
    RGBA(const RGBA &c) { *(RGBA2 *)this = *(const RGBA2 *)&c; }
} __attribute__((aligned(16)));
struct L { char pad[0x84]; char *spriteGroups; };
struct S { int id; int kind; L *owner; int unknownC; int index; };
struct T { char pad[0x58]; int spriteSlot; };
extern "C" void UpdateSpriteSlot(void *p, int a, void *b, float g, RGBA c0, RGBA c1);
/** @sly2-annot addr=0x215850
 * Purpose: forward frame update (time, M23, colours) to nested sprite
 * Info: proposed name UpdateSpriteItem (T2); TU hud
 * Notes: names_v2 v1 proposed FUN_001aea08 (0.000)
 * Confidence: low (draft) [T2 0.40] - evidence: static naming pass; sources: r7c_hud
 * See: docs/reference/functions/hud-ui.md#hud-0x210eb0-0x21bb88
 * @end-sly2-annot */
// Forwards a frame update (time, transform, colours) to a nested sprite item. (inferred)
extern "C" void UpdateSpriteItem(float time, S *sprite, T *item, void *matrix, RGBA c0, RGBA c1)
{
    if (sprite->kind == SPRITE_KIND_NESTED)
        UpdateSpriteSlot(sprite->owner->spriteGroups + sprite->index * 0x30, item->spriteSlot, matrix, time, c0, c1);
}
} // namespace M_func_00215850

// 0x2158C0 ReleaseSpriteItem
namespace M_func_002158C0 { extern "C" {
struct L { char pad[0x84]; char *spriteGroups; };
struct Q { char pad[0x20]; void *resource; };   /* a texture reference */
struct S { int id; int kind; struct L *owner; int unknownC; int index; struct Q *texture; };
struct T { char pad[0x58]; int spriteSlot; };
void StopSpriteSlot(void *p, int a);
void ReleaseRenderTarget(void *p);
void InitSpriteItem(struct T *t);
/** @sly2-annot addr=0x2158C0
 * Purpose: release item payload (stop nested slot / texture ref) and reinit
 * Info: proposed name ReleaseSpriteItem (T2); TU hud
 * Confidence: low (draft) [T2 0.40] - evidence: static analysis; sources: r7c_hud
 * See: docs/reference/functions/hud-ui.md#hud-0x210eb0-0x21bb88
 * @end-sly2-annot */
// Releases a sprite item's payload (stops a nested sprite, drops a texture reference) and resets it. (inferred)
void ReleaseSpriteItem(struct S *sprite, struct T *item)
{
    switch (sprite->kind) {
    case SPRITE_KIND_NESTED:
        StopSpriteSlot(sprite->owner->spriteGroups + sprite->index * 0x30, item->spriteSlot);
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
} } // namespace M_func_002158C0

// 0x215960 QueueSpriteItem
namespace M_func_00215960 { extern "C" {
struct L { char pad[0x84]; char *spriteGroups; };
struct S { int id; int kind; struct L *owner; int unknownC; int index; };
struct T { char pad[0x58]; int spriteSlot; char pad5c[5]; unsigned char drawFlags; };
void DrawSpriteSlot(void *p, int a, int b);
void PrependSlEntry(void *p, struct T *t);
/** @sly2-annot addr=0x215960
 * Purpose: draw item: nested sprite drawn recursively, other kinds prepended to draw list
 * Info: proposed name QueueSpriteItem (T2); TU hud
 * Confidence: low (draft) [T2 0.40] - evidence: static analysis; sources: r7c_hud
 * See: docs/reference/functions/hud-ui.md#hud-0x210eb0-0x21bb88
 * @end-sly2-annot */
// Queues a sprite item for drawing (nested sprites draw recursively). (inferred)
void QueueSpriteItem(struct S *sprite, struct T *item, int drawFlags)
{
    switch (sprite->kind) {
    case SPRITE_KIND_NESTED:
        DrawSpriteSlot(sprite->owner->spriteGroups + sprite->index * 0x30, item->spriteSlot, item->drawFlags);
        break;
    case SPRITE_KIND_GEOMETRY:
    case SPRITE_KIND_TEXT:
    case SPRITE_KIND_EFFECT:
    case SPRITE_KIND_PICTURE:
        item->drawFlags = drawFlags;
        PrependSlEntry((char *)sprite->owner + 0xFC, item);   /* the widget's render queue (+0xFC) */
        break;
    case SPRITE_KIND_POLYGON:
    case SPRITE_KIND_UNUSED5:
        break;
    }
}
} } // namespace M_func_00215960

// 0x2159E0 RenderSpritePayload
namespace M_func_002159E0 { extern "C" {
struct S { int id; int kind; };
void RenderSpriteText(struct S *p, int a, int b, int c);
void RenderSpriteGeometry(struct S *p, int a, int b, int c);
void RenderSpritePicture(struct S *p, int a, int b, int c);
/** @sly2-annot addr=0x2159E0
 * Purpose: dispatch by kind: 3 text (0x216BC8), 1 geometry (0x2174C8), else picture (0x2177E0)
 * Info: proposed name RenderSpritePayload (T2); TU hud
 * Confidence: low (draft) [T2 0.45] - evidence: static analysis; sources: r7c_hud
 * See: docs/reference/functions/hud-ui.md#hud-0x210eb0-0x21bb88
 * @end-sly2-annot */
// Renders a queued sprite payload by kind: text, geometry or picture. (inferred)
void RenderSpritePayload(struct S *sprite, int matrix, int params, int channel)
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
} } // namespace M_func_002159E0

// 0x215A50 IglyffFromCh__8CFontBrxc
namespace M_IglyffFromCh__8CFontBrxc { extern "C" {
struct CFontBrx { char pad[0x20]; int numGlyphs; unsigned int *glyphChars; };   /* glyphChars sorted, for the binary search */
/** @sly2-annot addr=0x215A50
 * Purpose: not written up yet; only the symbol name (pre-existing / Sly 1-derived) describes it.
 * Info: proposed name IglyffFromCh__8CFontBrxc (EXISTING); TU hud
 * Notes: RENAME proposed -> IglyffFromCh__8CFontBrxc (existing_name_changes.csv)
 * Confidence: name high [EXISTING 0.60]; purpose not reviewed - evidence: arbitration; pre-existing symbol in
 *   symbol_addrs.txt
 * See: docs/engine/hud-ui.md#5-key-functions, docs/reference/functions/hud-ui.md#hud-0x210eb0-0x21bb88
 * @end-sly2-annot */
// Glyph index of a character in a font (0 when the font has no glyph for it, inferred).
int IglyffFromCh__8CFontBrxc(struct CFontBrx *p, unsigned char ch)
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
} } // namespace M_IglyffFromCh__8CFontBrxc

// 0x215AC0 CRichText_ctor
namespace M_CRichText_ctor {
struct RGBA { unsigned int u; RGBA() {} RGBA(unsigned int x) { u = x; } };
struct Q { int a[4]; } __attribute__((aligned(16)));
struct F { char pad[0x54]; char *fonts; };   /* the HUD font object: 0x40-byte fonts */
struct G { char pad[0x3BE8]; F *hudFont; };
extern "C" G D_003DD490;
extern "C" Q D_002A2E10;
extern "C" Q D_002A2E20;
/* CRichText2 (0x98 bytes; the field names are the same in every view of this file):
   text, cursor, current / base / alternate font, current / base / alternate colour, the two float colour vectors
   the glyphs draw with and their base values, a style-change flag, the screen origin, the box (offset, size,
   scale), line height, font scale, alignment (0 top/left, 1 centre, 2 bottom/right) and render priority. */
struct S { int text; int cur; char *font; char *baseFont; char *altFont; int color; RGBA baseColor; char pad1c[0x24]; Q baseColor0; Q baseColor1;
  char pad60[4]; float originX; float originY; int offsetX; int offsetY; int width; int height; float scaleX; float scaleY; float lineHeight; float fontScale;
  int vAlign; int hAlign; int priority; };
extern "C" void ResetRichText2(S *p, int i);
/** @sly2-annot addr=0x215AC0
 * Purpose: rich-text ctor from string + font + scale (offsets 1792/1936)
 * Info: proposed name CRichText_ctor (T1); class CRichText; TU hud
 * Confidence: mid (draft) [T1 0.50] - evidence: static analysis; sources: r7c_hud
 * See: docs/reference/functions/hud-ui.md#hud-0x210eb0-0x21bb88
 * @end-sly2-annot */
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
    rt->font = rt->baseFont = D_003DD490.hudFont->fonts;
    rt->altFont = D_003DD490.hudFont->fonts + 0x40;
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
struct F { char pad[0x54]; char *fonts; };
struct G { char pad[0x3BE8]; F *hudFont; };
extern "C" G D_003DD490;
struct C8 { char pad[0x9C]; float screenX; float screenY; };   /* the widget whose screen position text uses */
struct P8 { char pad[0xA4]; int priority; char padA8[0x20]; C8 *ownerWidget; };   /* the owning widget */
/* a text sprite descriptor (RenderSpriteText's S) */
struct R { char pad[8]; P8 *owner; char padc[0x10]; int color; float scale; float width; float height; int hAlign; int vAlign; };
struct S { int text; int cur; char *font; char *baseFont; char *altFont; int color; int baseColor; char pad1c[0x24]; Q baseColor0; Q baseColor1;
  char pad60[4]; float originX; float originY; int offsetX; int offsetY; float width; float height; float scaleX; float scaleY; float lineHeight; float fontScale;
  int vAlign; int hAlign; int priority; };
extern "C" void ResetRichText2(S *p, int i);
/** @sly2-annot addr=0x215BA8
 * Purpose: rich-text ctor from a sprite text element (width/height/colour)
 * Info: proposed name CRichText_ctor_215BA8 (T2); class CRichText; TU hud
 * Notes: names_v2 C proposed DrawBlot__FP4BLOT (0.638) | agent qualifier: (sprite text) | COLLISION:
 *   CRichText_ctor already used (same name proposed for 0x215AC0); renamed to CRichText_ctor_215BA8
 * Confidence: low (draft) [T2 0.45] - evidence: static naming pass; sources: r7c_hud
 * See: docs/reference/functions/hud-ui.md#hud-0x210eb0-0x21bb88
 * @end-sly2-annot */
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
    rt->font = rt->baseFont = D_003DD490.hudFont->fonts;
    rt->altFont = D_003DD490.hudFont->fonts + 0x40;
    rt->baseColor = sprite->color;
    rt->baseColor0 = styleCopy.b;
    rt->baseColor1 = styleCopy.c;
    ResetRichText2(rt, 0);
    return rt;
}
} // namespace M_func_00215BA8

// 0x215CA8 ResetRichText2
namespace M_func_00215CA8 { extern "C" {
struct RGBA { unsigned char r, g, b, a; };
struct S { int text; int cur; int font; int baseFont; int altFont; struct RGBA color; struct RGBA baseColor; unsigned int altColor; };
void UpdateRichText2Color(struct S *p);
/** @sly2-annot addr=0x215CA8
 * Purpose: Reset the HUD rich-text cursor: optional new text, pchCur=achz, font cur=base, colour cur=base,
 *   colour word 0x80000000|alpha, UpdateColor
 * Info: proposed name CRichText2_Reset (T2); class CRichText; TU hud
 * Notes: names_v2 C proposed ShowBlot__FP4BLOT (0.438) | COLLISION: CRichText_Reset already used at 0x15F970
 *   (existing symbol); renamed CRichText_Reset_215CA8 | v3: superseded CRichText_Reset_215CA8 | top50_round
 *   (0.50): Same role as Sly 1 CRichText::Reset (m_pchCur=m_achz; m_pfontCur=m_pfontBase;
 *   m_rgbaCur=m_rgbaBase). But this is a second class: Reset__9CRichText already exists at 0x15F970 with a
 *   different layout (colours at +0x150..+0 | T2: placeholder token CRichText2
 * Confidence: low (draft) [T2 0.50] - evidence: Sly 1 / Lilly reference; semantic review; static naming pass;
 *   sources: r7c_hud+top50_round
 * See: docs/reference/functions/hud-ui.md#hud-0x210eb0-0x21bb88
 * @end-sly2-annot */
// Resets the rich-text cursor: optional new text, start of text, base font and colour. (inferred)
void ResetRichText2(struct S *rt, int text)
{
    if (text)
        rt->text = text;
    rt->cur = rt->text;
    rt->font = rt->baseFont;
    rt->color = rt->baseColor;
    rt->altColor = 0x80000000;                         /* black ... */
    ((unsigned char *)&rt->altColor)[3] = rt->color.a;   /* ... at the current alpha */
    UpdateRichText2Color(rt);
}
} } // namespace M_func_00215CA8

// 0x215D00 SetRichText2BaseColor
namespace M_func_00215D00 { extern "C" {
struct RGBA { unsigned char r, g, b, a; };
struct S { char pad[0x14]; struct RGBA color; struct RGBA baseColor; };
void UpdateRichText2Color(struct S *p);
/** @sly2-annot addr=0x215D00
 * Purpose: set base and current colour
 * Info: proposed name CRichText2_SetBaseColor (T2); class CRichText; TU hud
 * Notes: names_v2 C proposed HideBlot__FP4BLOT (0.167) | COLLISION: CRichText_SetBaseColor already used at
 *   0x15F9A8 (existing symbol); renamed CRichText_SetBaseColor_215D00 | v3: superseded
 *   CRichText_SetBaseColor_215D00 | top50 finding 8: hud-TU rich text (colours +0x14/+0x18) is a second
 *   class; Reset__9CRichText 0x15F970 keeps CRichText. CRichText2 is a placeholder class word (T2); resolves
 *   the v2 T3 collision | T2: placeholder token CRichText2
 * Confidence: low (draft) [T2 0.85] - evidence: semantic review; static naming pass; sources:
 *   r7c_hud+verdicts_followups+top50_round(CRichText2 class split)
 * See: docs/engine/hud-ui.md#5-key-functions, docs/reference/functions/hud-ui.md#hud-0x210eb0-0x21bb88
 * @end-sly2-annot */
// Sets the rich text's base and current colour. (inferred)
void SetRichText2BaseColor(struct S *rt, struct RGBA *color)
{
    rt->baseColor = *color;
    rt->color = rt->baseColor;
    UpdateRichText2Color(rt);
}
} } // namespace M_func_00215D00

// 0x215D40 GetRichText2NextChar
namespace M_func_00215D40 { extern "C" {
struct RGBA { unsigned char r, g, b, a; };
struct S {
    int text; unsigned char *cur; int font; int baseFont; int altFont;
    struct RGBA color; struct RGBA baseColor; struct RGBA altColor;
    char pad20[0x40]; int changed;
};
void UpdateRichText2Color(struct S *p);
/** @sly2-annot addr=0x215D40
 * Purpose: next printable char, processing &-escape codes
 * Info: proposed name CRichText2_ChNext (T2); class CRichText; TU hud
 * Notes: COLLISION: CRichText_ChNext already used at 0x15F9D0 (existing symbol); renamed
 *   CRichText_ChNext_215D40 | v3: superseded CRichText_ChNext_215D40 | top50 finding 8: hud-TU rich text
 *   (colours +0x14/+0x18) is a second class; Reset__9CRichText 0x15F970 keeps CRichText. CRichText2 is a
 *   placeholder class word (T2); resolves the v2 T3 collision | T2: placeholder token CRichText2
 * Confidence: low (draft) [T2 0.70] - evidence: semantic review; sources: r7c_hud+top50_round(CRichText2
 *   class split)
 * See: docs/reference/functions/hud-ui.md#hud-0x210eb0-0x21bb88
 * @end-sly2-annot */
// Returns the next printable character, processing '&' escape codes (colour, font, glyph changes) on the way.
// (inferred)
unsigned char GetRichText2NextChar(struct S *rt)
{
    int isDone = 0;
    rt->changed = 0;
    while (!isDone) {
        unsigned char *pch = rt->cur;
        unsigned char c = *pch;
        if (c == '&') {
            if (pch[1] == c) {            /* "&&": a literal '&' */
                rt->cur = pch + 1;
                isDone = 1;
            } else if (pch[1] == '.') {   /* "&.": back to the base font and colour */
                rt->cur = pch + 2;
                rt->font = rt->baseFont;
                rt->color = rt->baseColor;
                rt->changed = 1;
                UpdateRichText2Color(rt);
            } else {                      /* "&x": the alternate font and colour */
                rt->cur = pch + 2;
                rt->font = rt->altFont;
                rt->color = rt->altColor;
                rt->changed = 1;
                UpdateRichText2Color(rt);
            }
        } else {
            isDone = 1;
        }
    }
    return *rt->cur++;
}
} } // namespace M_func_00215D40

// 0x215E30 CRichText_Cch
namespace M_CRichText_Cch { extern "C" {
void ResetRichText2(void *p, int a);
int GetRichText2NextChar(void *p);
/** @sly2-annot addr=0x215E30
 * Purpose: count characters
 * Info: proposed name CRichText_Cch (T1); class CRichText; TU hud
 * Notes: names_v2 B proposed FIncludeBlotForPeg__FP4BLOTT0 (0.856)
 * Confidence: mid (draft) [T1 0.75] - evidence: static naming pass; sources: r7c_hud
 * See: docs/engine/hud-ui.md#5-key-functions, docs/reference/functions/hud-ui.md#hud-0x210eb0-0x21bb88
 * @end-sly2-annot */
// Number of printable characters in the rich text.
int CRichText_Cch(void *rt)
{
    int count = 0;
    ResetRichText2(rt, 0);
    while (GetRichText2NextChar(rt))
        count++;
    return count;
}
} } // namespace M_CRichText_Cch

// 0x215E80 CRichText_Trim
namespace M_CRichText_Trim { extern "C" {
struct S { int text; char *cur; };
void ResetRichText2(struct S *p, int a);
int GetRichText2NextChar(struct S *p);
/** @sly2-annot addr=0x215E80
 * Purpose: truncate after n chars
 * Info: proposed name CRichText_Trim (T1); class CRichText; TU hud
 * Confidence: high [T1 0.85] - evidence: static analysis; sources: r7c_hud+verdicts_followups
 * See: docs/engine/hud-ui.md#5-key-functions, docs/reference/functions/hud-ui.md#hud-0x210eb0-0x21bb88
 * @end-sly2-annot */
// Truncates the rich text after n printable characters.
void CRichText_Trim(struct S *rt, int n)
{
    ResetRichText2(rt, 0);
    while (GetRichText2NextChar(rt) && n != 0)
        n--;
    *rt->cur = 0;
}
} } // namespace M_CRichText_Trim

// 0x215ED8 CRichText_Dx
namespace M_CRichText_Dx { extern "C" {
struct S { char pad[8]; void *font; char pad2[0x7C]; float fontScale; };
void ResetRichText2(struct S *p, int a);
int GetRichText2NextChar(struct S *p);
float GetFontGlyphWidth2(void *p, int ch);
/** @sly2-annot addr=0x215ED8
 * Purpose: sum of glyph widths * scale
 * Info: proposed name CRichText_Dx (T1); class CRichText; TU hud
 * Confidence: mid (draft) [T1 0.70] - evidence: static analysis; sources: r7c_hud
 * See: docs/engine/hud-ui.md#5-key-functions, docs/reference/functions/hud-ui.md#hud-0x210eb0-0x21bb88
 * @end-sly2-annot */
// Width of the rich text (glyph widths times scale).
float CRichText_Dx(struct S *p)
{
    float sum = 0.0f;
    int ch;
    ResetRichText2(p, 0);
    while ((ch = GetRichText2NextChar(p)) != 0)
        sum += GetFontGlyphWidth2(p->font, ch) * p->fontScale;
    return sum;
}
} } // namespace M_CRichText_Dx

// 0x215F40 WrapRichText2Lines
namespace M_func_00215F40 {
extern "C" {
struct S { unsigned char *text; unsigned char *cur; void *font; char pad[0x74-0xC]; float width; char pad1[0x88-0x78]; float fontScale; };
extern unsigned char D_002AA6A1[] __asm__("_ctype_+1");
void ResetRichText2(struct S *,unsigned char *);
unsigned char GetRichText2NextChar(struct S *);
float GetFontGlyphWidth2(void *,unsigned char);
static inline int NewlineValue(){int value;__asm__("addiu %0,$zero,10":"=r"(value));return value;}
static inline void StoreByte(unsigned char *p,int c){*p=c;}
/** @sly2-annot addr=0x215F40
 * Purpose: count lines, inserting newlines when width exceeded
 * Info: proposed name CRichText2_ClineWrap (T2); class CRichText; TU hud
 * Notes: COLLISION: CRichText_ClineWrap already used at 0x15FD68 (existing symbol); renamed
 *   CRichText_ClineWrap_215F40 | v3: superseded CRichText_ClineWrap_215F40 | top50 finding 8: hud-TU rich
 *   text (colours +0x14/+0x18) is a second class; Reset__9CRichText 0x15F970 keeps CRichText. CRichText2 is a
 *   placeholder class word (T2); resolves the v2 T3 collision | T2: placeholder token CRichText2
 * Confidence: low (draft) [T2 0.55] - evidence: semantic review; sources: r7c_hud+top50_round(CRichText2
 *   class split)
 * See: docs/reference/functions/hud-ui.md#hud-0x210eb0-0x21bb88
 * @end-sly2-annot */
// Counts lines, inserting line breaks where a line would exceed the box width. (inferred)
int WrapRichText2Lines(struct S *p) {
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

}
} // namespace M_func_00215F40

// 0x2160D8 UpdateRichText2Color
namespace M_func_002160D8 { extern "C" {
struct RGBA { unsigned char r, g, b, a; };
struct S {
    char pad[0x14]; struct RGBA color; struct RGBA baseColor;
    char pad1c[4];
    /* the two vectors the glyphs draw with (an offset and a scale, as RenderWidget's PARAM) and their bases */
    float color0R, color0G, color0B, color0W, color1R, color1G, color1B, color1A;
    float baseColor0R, baseColor0G, baseColor0B, baseColor0W, baseColor1R, baseColor1G, baseColor1B, baseColor1A;
};
static inline float GMin(float a, float b) { return b < a ? b : a; }
static inline float GClamp(float g, float gMin, float gMax)
{
    if (g < gMin) return gMin;
    if (gMax < g) return gMax;
    return g;
}
/** @sly2-annot addr=0x2160D8
 * Purpose: convert current RGBA to clamped float colours
 * Info: proposed name CRichText_UpdateColor (T2); class CRichText; TU hud
 * Confidence: low (draft) [T2 0.35] - evidence: static analysis; sources: r7c_hud
 * See: docs/reference/functions/hud-ui.md#hud-0x210eb0-0x21bb88
 * @end-sly2-annot */
// Converts the current colour to clamped float colours for drawing. (inferred)
void UpdateRichText2Color(struct S *rt)
{
    float alphaScale = rt->baseColor1A * 0.007874016f;   /* 1 / 127 */
    float alpha;
    alpha = alphaScale * (float)rt->color.a;
    rt->color0R = GMin(rt->baseColor0R + (float)rt->color.r, 255.0f);
    rt->color0G = GMin(rt->baseColor0G + (float)rt->color.g, 255.0f);
    rt->color0B = GMin(rt->baseColor0B + (float)rt->color.b, 255.0f);
    rt->color0W = rt->baseColor0W;
    rt->color1R = rt->baseColor1R;
    rt->color1G = rt->baseColor1G;
    rt->color1B = rt->baseColor1B;
    rt->color1A = GClamp(alpha, 0.0f, 1.0f);
}
} } // namespace M_func_002160D8

// 0x2161C8 GetRichText2MaxLineWidth
namespace M_func_002161C8 { extern "C" {
struct S { char pad[8]; void *font; char padc[0x7C]; float fontScale; };
void ResetRichText2(struct S *p, int i);
int GetRichText2NextChar(struct S *p);
float GetFontGlyphWidth2(void *p, int ch);
/** @sly2-annot addr=0x2161C8
 * Purpose: widest line
 * Info: proposed name CRichText2_DxMaxLine (T2); class CRichText; TU hud
 * Notes: names_v2 C proposed GetExtents__9CRichTextPfT1f (0.188) | COLLISION: CRichText_DxMaxLine already
 *   used at 0x15FF40 (existing symbol); renamed CRichText_DxMaxLine_2161C8 | v3: superseded
 *   CRichText_DxMaxLine_2161C8 | top50 finding 8: hud-TU rich text (colours +0x14/+0x18) is a second class;
 *   Reset__9CRichText 0x15F970 keeps CRichText. CRichText2 is a placeholder class word (T2); resolves the v2
 *   T3 collision | T2: placeholder token CRichText2
 * Confidence: low (draft) [T2 0.70] - evidence: semantic review; static naming pass; sources:
 *   r7c_hud+top50_round(CRichText2 class split)
 * See: docs/reference/functions/hud-ui.md#hud-0x210eb0-0x21bb88
 * @end-sly2-annot */
// Width of the widest line. (inferred)
float GetRichText2MaxLineWidth(struct S *rt)
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
} } // namespace M_func_002161C8

// 0x216260 CRichText_GetExtents
namespace M_CRichText_GetExtents { extern "C" {
struct S { char pad[0x84]; float lineHeight; };
int WrapRichText2Lines(struct S *p);
float GetRichText2MaxLineWidth(struct S *p);
/** @sly2-annot addr=0x216260
 * Purpose: dx = DxMaxLine, dy = lines*lineheight
 * Info: proposed name CRichText_GetExtents (T1); class CRichText; TU hud
 * Notes: names_v2 C proposed PostNoteLoad__FP4NOTE (0.521)
 * Confidence: mid (draft) [T1 0.70] - evidence: static naming pass; sources: r7c_hud
 * See: docs/engine/hud-ui.md#5-key-functions, docs/reference/functions/hud-ui.md#hud-0x210eb0-0x21bb88
 * @end-sly2-annot */
// Width (widest line) and height (lines x line height) of the rich text.
void CRichText_GetExtents(struct S *rt, float *width, float *height)
{
    int numLines = WrapRichText2Lines(rt);
    float maxWidth = GetRichText2MaxLineWidth(rt);
    float totalHeight = numLines * rt->lineHeight;
    if (width)
        *width = maxWidth;
    if (height)
        *height = totalHeight;
}
} } // namespace M_CRichText_GetExtents

// 0x2162D8 BuildRichTextMatrix
namespace M_func_002162D8 { extern "C" {
struct S { char pad[0x64]; float originX; float originY; };
void *func_002333B4(void *p, int c, int n) __asm__("memset");
/** @sly2-annot addr=0x2162D8
 * Purpose: glyph matrix from 2x3 transform (0.8 / 0.4545 pixel scales, GS offset 786432)
 * Info: proposed name BuildRichTextMatrix (T2); TU hud
 * Notes: names_v2 C proposed SetNoteAchzDraw__FP4NOTEPc (0.638)
 * Confidence: low (draft) [T2 0.40] - evidence: static naming pass; sources: r7c_hud
 * See: docs/reference/functions/hud-ui.md#hud-0x210eb0-0x21bb88
 * @end-sly2-annot */
// Builds a glyph matrix from a 2D transform (pixel aspect scales and the GS screen offset). (inferred)
void BuildRichTextMatrix(struct S *rt, float *m, float *xf)
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
} } // namespace M_func_002162D8

// 0x2163B0 DrawRichText2
namespace M_func_002163B0 {
// Configure rich-text rendering and queue the standard render payloads.
typedef unsigned int uint;
typedef unsigned long ulong;
typedef int qword __attribute__((mode(TI)));
typedef struct {unsigned char r,g,b,a;} RGBA;
struct RT {char pad[0x6C];float offsetX,offsetY,width,height,scaleX,scaleY,lineHeight,fontScale;int vAlign,hAlign,priority;};
struct CONFIG {float offsetX,offsetY,width,height;RGBA color;int hAlign,vAlign;};
extern "C" void *func_002333B4(void *,int,int) __asm__("memset");
struct M23 {float a[8];enum INIT {Uninitialized,Zero};M23(INIT init=Uninitialized){if(init==Zero)func_002333B4(this,0,sizeof(*this));}} __attribute__((aligned(16)));
struct M4 {qword r[4];};
struct DATA {RGBA color;int mode;float priority;int limit;qword v10,v20;};
struct R {char pad[0xC];unsigned char *cur;};
struct E {int kind;float priority;uint address;int tag;};
extern "C" { extern struct R D_002DEA28; }
extern "C" { extern char D_00442210[],D_002DF010[]; }
extern "C" void CopyAqw__FPvT0i(void *,void *,int);
extern "C" struct E *AllocRenderListEntry(void);
static inline void Word(struct R *r,uint word){*(uint *)r->cur=word;r->cur+=4;}
static inline void Align(struct R *r,int word){while(((uint)r->cur&15)!=4*word)Word(r,0);}
static inline void Bulk(struct R *r,int n,void *source){CopyAqw__FPvT0i(r->cur,source,n);r->cur+=n*16;}
static inline void Direct(struct R *r,int n,void *source){Align(r,3);Word(r,0x50000000|n);Bulk(r,n,source);}

extern "C" void SetRichText2BaseColor(struct RT *,RGBA *);
extern "C" void BuildRichTextMatrix(struct RT *,struct M4 *,struct M23 *);
extern "C" void DrawRichText2Glyphs(struct RT *,struct M4 *,struct DATA *);
extern "C" { extern qword D_002A2E10,D_002A2E20; }
/** @sly2-annot addr=0x2163B0
 * Purpose: configure box/alignment/colour and queue rich text
 * Info: proposed name CRichText2_Draw (T2); class CRichText; TU hud
 * Notes: names_v2 B proposed DrawTitle__FP5TITLE (0.932) | COLLISION: CRichText_Draw already used at 0x15FFD0
 *   (existing symbol); renamed CRichText_Draw_2163B0 | v3: superseded CRichText_Draw_2163B0 | top50 finding
 *   8: hud-TU rich text (colours +0x14/+0x18) is a second class; Reset__9CRichText 0x15F970 keeps CRichText.
 *   CRichText2 is a placeholder class word (T2); resolves the v2 T3 collision | T2: placeholder token
 *   CRichText2
 * Confidence: low (draft) [T2 0.50] - evidence: semantic review; static naming pass; sources:
 *   r7c_hud+top50_round(CRichText2 class split)
 * See: docs/reference/functions/hud-ui.md#hud-0x210eb0-0x21bb88
 * @end-sly2-annot */
// Configures box, alignment and colour and queues the rich text for drawing. (inferred)
extern "C" void DrawRichText2(struct RT *p,struct CONFIG *config,int priority){
    RGBA color;
    unsigned char *tag;
    struct E *e;
    if(!config)return;
    p->offsetX=config->offsetX;p->offsetY=config->offsetY;p->width=config->width;p->height=config->height;
    p->hAlign=config->hAlign;p->vAlign=config->vAlign;p->priority=priority;
    color=config->color;SetRichText2BaseColor(p,&color);
    struct M23 matrix(M23::Zero);
    struct M4 world;
    struct DATA style;
    matrix.a[0]=p->scaleX*p->fontScale;
    matrix.a[3]=p->scaleY*p->fontScale;
    matrix.a[4]=p->offsetX;
    matrix.a[5]=p->offsetY+p->lineHeight*p->scaleY;
    BuildRichTextMatrix(p,&world,&matrix);
    style.v10=D_002A2E10;style.v20=D_002A2E20;style.priority=16.0f;
    DrawRichText2Glyphs(p,&world,&style);
    tag=D_002DEA28.cur;
    *(ulong *)tag=0x10000000UL;D_002DEA28.cur+=16;
    Word(&D_002DEA28,0x11000000);
    Direct(&D_002DEA28,3,D_00442210);
    Direct(&D_002DEA28,0x25,D_002DF010);
    Word(&D_002DEA28,0x11000000);
    Align(&D_002DEA28,0);
    *(uint *)tag|=((D_002DEA28.cur-tag)>>4)-1;
    *(ulong *)D_002DEA28.cur=0x60000000UL;D_002DEA28.cur+=16;
    e=AllocRenderListEntry();
    e->kind=0x15;e->priority=p->priority<<18;
    e->address=(uint)tag&0x0FFFFFFF;e->tag=0;
}
} // namespace M_func_002163B0

// 0x2166B0 DrawRichText2AtXf
namespace M_func_002166B0 { extern "C" {
typedef int qword __attribute__((mode(TI)));
typedef struct { float a[6]; } __attribute__((aligned(16))) T;
typedef struct { float a[16]; } __attribute__((aligned(16))) O;
struct RT {char pad[8];void *font;char padC[0x14];qword color0,color1;char pad40[0x20];int changed;float originX,originY,offsetX,offsetY,width,height,scaleX,scaleY,lineHeight,fontScale;int vAlign,hAlign,priority;};
struct DATA {unsigned int color;int mode;float priority;unsigned int limit;qword color0,color1;};

void BuildRichTextMatrix(struct RT *p, O *po, T *pt);
void DrawRichText2Glyphs(struct RT *p, O *po, struct DATA *q);
static inline void StoreX(struct RT *p,float value){register float x asm("$f2")=value;asm("swc1 %1,%0":"=m"(p->scaleX):"f"(x));}
/** @sly2-annot addr=0x2166B0
 * Purpose: draw at a sprite transform (font scale, line height)
 * Info: proposed name CRichText_DrawXf (T2); class CRichText; TU hud
 * Notes: names_v2 C proposed PostTotalsLoad__FP6TOTALS (0.438)
 * Confidence: low (draft) [T2 0.40] - evidence: static naming pass; sources: r7c_hud
 * See: docs/reference/functions/hud-ui.md#hud-0x210eb0-0x21bb88
 * @end-sly2-annot */
// Draws rich text at a sprite transform (font scale, line height). (inferred)
void DrawRichText2AtXf(struct RT *p, T *pt, struct DATA *q)
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
} } // namespace M_func_002166B0

// 0x216750 DrawRichText2Glyphs
namespace M_func_00216750 { extern "C" {
// Align, clip, and queue rich-text glyphs with their current colors.
typedef unsigned int uint;
typedef unsigned long ulong;
typedef int qword __attribute__((mode(TI)));
struct RT {char pad[8];void *font;char padc[0x14];qword color0,color1;char pad40[0x20];int changed;float originX,originY,offsetX,offsetY,width,height,scaleX,scaleY,lineHeight,fontScale;int vAlign,hAlign,priority;};
union M4 {qword r[4];float a[16];};
struct DATA {uint color;int mode;float priority;uint limit;qword color0,color1;};
struct GLYPH {int a;void *data;char pad8[0xA];unsigned short mode;char pad14[8];void *address;char pad20[0x20];};
struct R {char pad[0xC];unsigned char *cur;};
struct E {int kind;float priority;uint address;int tag;};
extern struct R D_002DEA28;
extern float D_002C40A8,D_002C40AC,D_002C40B0,D_002C40B4;
void *func_002333B4(void *,int,int) __asm__("memset");
void ResetRichText2(struct RT *,char *);
int GetRichText2NextChar(struct RT *);
float GetFontGlyphWidth2(void *,int);
struct GLYPH *PglyffFromCh__8CFontBrxc(void *,int);
void BuildGlyphPacket(struct GLYPH *,union M4 *,struct DATA *,int);
struct E *AllocRenderListEntry(void);
static inline void Word(struct R *r,uint word){*(uint *)r->cur=word;r->cur+=4;}
static inline void Align(struct R *r,int word){while(((uint)r->cur&15)!=4*word)Word(r,0);}
static inline int Visible(union M4 *matrix,const float *left){int visible=0;if(*left<=matrix->a[8] && matrix->a[8]<=D_002C40AC && D_002C40B0<=matrix->a[9] && matrix->a[9]<=D_002C40B4)visible=1;return visible;}
/** @sly2-annot addr=0x216750
 * Purpose: align, clip and queue glyphs with colours
 * Info: proposed name CRichText_DrawGlyphs (T2); class CRichText; TU hud
 * Notes: names_v2 C proposed EdgeRect__8CFontBrxP9CTextEdgeP8CTextBoxP4GIFS (0.638)
 * Confidence: low (draft) [T2 0.45] - evidence: static naming pass; sources: r7c_hud
 * See: docs/reference/functions/hud-ui.md#hud-0x210eb0-0x21bb88
 * @end-sly2-annot */
// Aligns, clips and queues the glyphs with their colours. (inferred)
void DrawRichText2Glyphs(struct RT *p,union M4 *matrix,struct DATA *style){
    float widths[32];
    unsigned char *tag=D_002DEA28.cur;
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
                glyph=PglyffFromCh__8CFontBrxc(p->font,ch);
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
    *(ulong *)D_002DEA28.cur=0x60000000UL;D_002DEA28.cur+=16;
    {
        struct E *e=AllocRenderListEntry();
        e->kind=0x15;e->priority=style->priority+(p->priority<<18);e->address=(uint)tag&0x0FFFFFFF;e->tag=0;
    }
}
} } // namespace M_func_00216750

// 0x216B40 PglyffFromCh__8CFontBrxc
namespace M_PglyffFromCh__8CFontBrxc { extern "C" {
struct G { char pad[0x40]; };
struct S { char pad[0x14]; struct G *glyphs; };
int IglyffFromCh__8CFontBrxc(struct S *p, unsigned char ch);
/** @sly2-annot addr=0x216B40
 * Purpose: GLYFF* = &glyphs[i] for i = 0x215A50(ch)
 * Info: proposed name CFontBrx_PglyffFromCh (T1); TU hud
 * Notes: names_v2 C proposed ShowTotalsQMARK (0.167) | agent qualifier: pointer form | COLLISION:
 *   FindGlyphForChar already used (same name proposed for 0x15F8A8); renamed to PglyffFromCh_216B40 | superseded:
 *   PglyffFromCh_216B40 (arbitration) | arbitration knock-on rename: CONFIRMED static: char arg (andi 0xff),
 *   returns GLYFF* or NULL = Sly1 CFontBrx::FindGlyphForChar contract. | arbitration mangling: compile a
 *   declaration in the scratch build and compare before committing
 * Confidence: mid (draft) [T1 0.60] - evidence: Sly 1 / Lilly reference; arbitration; static naming pass;
 *   sources: r7c_hud+arbitration
 * See: docs/engine/hud-ui.md#5-key-functions, docs/reference/functions/hud-ui.md#hud-0x210eb0-0x21bb88
 * @end-sly2-annot */
struct G *PglyffFromCh__8CFontBrxc(struct S *p, unsigned char ch)
{
    int i = IglyffFromCh__8CFontBrxc(p, ch);
    return i ? &p->glyphs[i] : 0;
}
} } // namespace M_PglyffFromCh__8CFontBrxc

// 0x216B80 GetFontGlyphWidth2
namespace M_func_00216B80 { extern "C" {
struct S { char pad[0x1C]; float *glyphWidths; };
int IglyffFromCh__8CFontBrxc(struct S *p, unsigned char ch);
/** @sly2-annot addr=0x216B80
 * Purpose: width of glyph for ch (widths[i-1])
 * Info: proposed name CFontBrx_DxFromCh_216B80 (T3); class CFontBrx; TU hud
 * Notes: names_v2 C proposed HideTotalsQMARK (0.638) | COLLISION: CFontBrx_DxFromCh already used at 0x15F0F0
 *   (existing symbol); renamed CFontBrx_DxFromCh_216B80
 * Confidence: low (draft) [T3 0.65] - evidence: static naming pass; sources: r7c_hud
 * See: docs/reference/functions/hud-ui.md#hud-0x210eb0-0x21bb88
 * @end-sly2-annot */
// Width of a character's glyph in the font. (inferred)
float GetFontGlyphWidth2(struct S *p, unsigned char ch)
{
    int i = IglyffFromCh__8CFontBrxc(p, ch);
    return i ? p->glyphWidths[i - 1] : 0.0f;
}
} } // namespace M_func_00216B80

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
typedef struct {char pad[0x3BE8];void *font;} WORLD;
typedef struct {int id;void (*get)(char *,void *,void *);void *context;} STRINGGEN;
typedef struct {char pad10[0x10];int state;float time;int pad18;float finish;} SCROLL;
typedef struct {float t,dt;} CLOCK;
extern "C" { extern WORLD D_003DD490; }
extern "C" { extern STRINGGEN D_003E1C88[]; }
extern "C" { extern const char D_002A8B80[],D_002A8B88[]; }
extern "C" { extern char D_004435F0[]; }
#define D_004435F4 (*(RGBA *)((char *)base_D_004435F0 + 4))   /* g_subtitle's colour (tu_2275B8's SPEAKERLABEL +0x4) */
extern "C" { extern SCROLL D_003DA0C0; }
extern "C" { extern CLOCK D_002DDED0; }
extern "C" { extern const float D_002C40B8; }
extern "C" void *FindSwLocalizedString__FP2SWi(WORLD *,int);
extern "C" char *strcpy(char *,const char *);
extern "C" char *strcpy1(char *,const char *);
extern "C" RT *ConstructRichText2FromSprite(RT *,char *,S *,DATA *);
extern "C" int WrapRichText2Lines(RT *);
extern "C" void TrimSubtitleToVoiceProgress(void *,RT *);
extern "C" void ResetRichText2(RT *,char *);
extern "C" int GetRichText2NextChar(RT *);
extern "C" void SetRichText2BaseColor(RT *,RGBA *);
extern "C" float CRichText_Dx(RT *);
extern "C" float GModPositive__Fff(float,float);
extern "C" void SetNoticeState(SCROLL *,int);
extern "C" void DrawRichText2AtXf(RT *,T *,DATA *);
static inline float Max(float a,float b){return a<b?b:a;}
enum { TEXT_SOURCE_LANGSTR = 0, TEXT_SOURCE_TABLE = 1 };
enum { TEXT_ID_SUBTITLE = 0x2EF, TEXT_ID_TICKER = 0x2F1 };   /* BuildSubtitleText, BuildTickerText */

/** @sly2-annot addr=0x216BC8
 * Purpose: sprite text: STRINGGEN text (kind 1, 108-entry table D_003E1C88) or langstr (kind 0); subtitle
 *   (0x2EF) and ticker (0x2F1) special cases
 * Info: proposed name RenderSpriteText (T1); TU hud
 * Confidence: mid (draft) [T1 0.60] - evidence: static analysis; sources: r7c_hud
 * See: docs/reference/functions/hud-ui.md#hud-0x210eb0-0x21bb88
 * @end-sly2-annot */
// Renders a text sprite: a text-table entry or a language string, with special cases for subtitles and the ticker.
extern "C" void RenderSpriteText(S *p,T *t,DATA *data,int unused){
    char text[0x400];
    RT rt;
    DATA style;
    char marquee[2*sizeof(text)+3*5];
    if(!D_003DD490.font)return;
    text[0]=0;
    if(p->kind==TEXT_SOURCE_TABLE){
        uint i;
        for(i=0;i<108;i++)if(D_003E1C88[i].id==p->id)break;
        if(i>=108)*(long *)text=*(const long *)D_002A8B80;
        else D_003E1C88[i].get(text,D_003E1C88[i].context,p->owner);
    }else if(p->kind==TEXT_SOURCE_LANGSTR){
        strcpy(text,(const char *)FindSwLocalizedString__FP2SWi(&D_003DD490,p->id));
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
        paddingWidth=CRichText_Dx(&rt)*scale;
        strcpy1(marquee,text);
        ResetRichText2(&rt,marquee);
        textWidth=CRichText_Dx(&rt)*scale;
        strcpy1(marquee,D_002A8B88);
        strcpy1(marquee,text);
        strcpy1(marquee,D_002A8B88);
        elapsed=D_002DDED0.t-D_003DA0C0.time;
        if(elapsed<D_002C40B8){
            data->fade*=elapsed/D_002C40B8;
        }else{
            elapsed-=D_002C40B8;
            period=textWidth*0.01f;
            position=GModPositive__Fff(elapsed,period);
            if(D_003DA0C0.finish>=0.0f){
                float finish=Max(D_003DA0C0.finish,(textWidth+Max(0.0f,textWidth-p->width*scale)+paddingWidth)*0.01f);
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
namespace M_func_00217058 { extern "C" {
// Build a matrix/parameter DMA packet ending in the supplied call tag.
struct R { char pad[0xC]; unsigned char *cur; };
struct S { char pad[0x12]; unsigned short mode; char pad1[8]; void *address; };
struct D { int a; int mode; int b; int xC; char pad[0x20]; };
extern struct R D_002DEA28;
static inline void CopyWords(int *d,const int *s,int n){
    int rem=n&3,cBulk=n-rem,i;
    for(i=0;i<rem;i++)*d++=*s++;
    for(i=0;i<cBulk;i+=4){int a=s[0],b=s[1],c=s[2],e=s[3];s+=4;d[0]=a;d[1]=b;d[2]=c;d[3]=e;d+=4;}
}
static inline void Word(struct R *r,unsigned int word) {
    *(unsigned int *)r->cur=word;
    r->cur+=4;
}
static inline void Align(struct R *r) {
    while((int)r->cur&15) { *(int *)r->cur=0; r->cur+=4; }
}
static inline void Unpack(struct R *r,const int *s,int n,unsigned int header){Word(r,header);CopyWords((int *)r->cur,s,n);r->cur+=n*4;}
/** @sly2-annot addr=0x217058
 * Purpose: matrix/parameter DMA packet ending in a call tag
 * Info: proposed name BuildGlyphPacket (T2); TU hud
 * Notes: names_v2 C proposed DrawTotals__FP6TOTALS (0.438)
 * Confidence: low (draft) [T2 0.35] - evidence: static naming pass; sources: r7c_hud
 * See: docs/reference/functions/hud-ui.md#hud-0x210eb0-0x21bb88
 * @end-sly2-annot */
// Builds the matrix / parameter DMA packet for a glyph run, ending in a call tag. (inferred)
void BuildGlyphPacket(struct S *p,const int *matrix,struct D *data,int flags) {
    unsigned char *tag=D_002DEA28.cur;
    *(unsigned long *)tag=0x10000000UL;
    D_002DEA28.cur+=16;
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
    *(int *)tag |= ((D_002DEA28.cur-tag)>>4)-1;
    Align(&D_002DEA28);
    *(unsigned long *)D_002DEA28.cur=0x50000000UL | ((unsigned long)p->address<<32);
    D_002DEA28.cur+=16;
}
} } // namespace M_func_00217058

// 0x217298 QueueSpriteVuProgram
namespace M_func_00217298 { extern "C" {
// Queue the selected VU microprogram pair in a DMA packet.
typedef unsigned int uint;
typedef unsigned long ulong;
struct R {char pad[0xC];unsigned char *cur;};
struct S {char pad[0xA4];int priority;};
struct E {int kind;float priority;uint address;int pad;};
extern struct R D_002DEA28;
extern char D_00000000[],D_00000030[],D_00000060[],D_00000090[],D_000000C0[],D_00000100[];
struct E *AllocRenderListEntry(void);
static inline void Word(struct R *r,uint word){*(uint *)r->cur=word;r->cur+=4;}
static inline void Mscal(struct R *r,void *entry){Word(r,0x14000000|((uint)entry>>3));}
static inline void Align(struct R *r){while((uint)r->cur&15){*(uint *)r->cur=0;r->cur+=4;}}
/** @sly2-annot addr=0x217298
 * Purpose: queue VU microprogram pair for render mode
 * Info: proposed name QueueSpriteVuProgram (T2); TU hud
 * Notes: names_v2 C proposed DrawLogo__FP4LOGO (0.638)
 * Confidence: low (draft) [T2 0.40] - evidence: static naming pass; sources: r7c_hud
 * See: docs/reference/functions/hud-ui.md#hud-0x210eb0-0x21bb88
 * @end-sly2-annot */
// Queues the VU microprogram pair for a sprite render mode. (inferred)
void QueueSpriteVuProgram(struct S *p,int mode,float priority){
    unsigned char *tag=D_002DEA28.cur;
    struct E *e;
    *(ulong *)tag=0x10000000UL;
    D_002DEA28.cur+=16;
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
    *(uint *)tag|=((D_002DEA28.cur-tag)>>4)-1;
    *(ulong *)D_002DEA28.cur=0x60000000UL;
    D_002DEA28.cur+=16;
    e=AllocRenderListEntry();
    e->kind=0x15;e->priority=priority+(p->priority<<18);e->address=(uint)tag&0x0FFFFFFF;e->pad=0;
}
} } // namespace M_func_00217298

// 0x2174C8 RenderSpriteGeometry
namespace M_RenderSpriteGeometry { extern "C" {
// Queue transformed sprite parameters with optional fade limiting.
typedef unsigned int uint;
typedef unsigned long ulong;
struct POSITION {char pad[0x9C];float screenX,screenY;};   /* PostWidgetLoad's ownerWidget */
struct OWNER {char pad[0xA4];int priority;char pada8[0x20];struct POSITION *ownerWidget;};
struct S {char pad[8];struct OWNER *owner;char padc[6];unsigned short mode;int count;char pad18[4];int tag;};
struct T {float a,b,c,d,x,y;};
struct M4 {float a[16];};
struct D {int a,mode;float priority;uint limit;char pad10[0x1C];float fade;};
struct R {char pad[0xC];unsigned char *cur;};
struct E {int kind;float priority;uint address;int tag;};
extern struct R D_002DEA28;
void *func_002333B4(void *,int,int) __asm__("memset");
struct E *AllocRenderListEntry(void);
static inline int Min(int a,int b){return a<b?a:b;}
static inline void Word(struct R *r,uint word){*(uint *)r->cur=word;r->cur+=4;}
static inline void Align(struct R *r){while((uint)r->cur&15){*(uint *)r->cur=0;r->cur+=4;}}
static inline void CopyWords(int *d,const int *s,int n){int rem=n&3,cBulk=n-rem,i;for(i=0;i<rem;i++)*d++=*s++;for(i=0;i<cBulk;i+=4){int a=s[0],b=s[1],c=s[2],e=s[3];s+=4;d[0]=a;d[1]=b;d[2]=c;d[3]=e;d+=4;}}
static inline void Unpack(struct R *r,const int *s,int n,uint header){Word(r,header);CopyWords((int *)r->cur,s,n);r->cur+=n*4;}
/** @sly2-annot addr=0x2174C8
 * Purpose: transformed sprite parameters with optional fade
 * Info: proposed name RenderSpriteGeometry (T1); TU hud
 * Confidence: high [T1 0.85] - evidence: static analysis; sources: r7c_hud+verdicts_followups
 * See: docs/engine/hud-ui.md#5-key-functions, docs/reference/functions/hud-ui.md#hud-0x210eb0-0x21bb88
 * @end-sly2-annot */
// Renders a geometry sprite with its transform and optional fade.
void RenderSpriteGeometry(struct S *p,struct T *t,struct D *data,int fade){
    struct M4 m;
    unsigned char *tag;
    struct E *e;
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
            int n=Min((int)(p->count*old),p->count);
            data->limit=n>=3?n:3;
            if(data->limit>0x7FFFU)data->limit=0x7FFF;
        }
    }
    tag=D_002DEA28.cur;
    *(ulong *)tag=0x10000000UL;D_002DEA28.cur+=16;
    Unpack(&D_002DEA28,(const int *)&m,16,0x6C04C000);
    Unpack(&D_002DEA28,(const int *)data,12,0x6C03C004);
    Align(&D_002DEA28);
    *(uint *)tag|=((D_002DEA28.cur-tag)>>4)-1;
    *(ulong *)D_002DEA28.cur=0x60000000UL;D_002DEA28.cur+=16;
    e=AllocRenderListEntry();
    e->kind=0x15;e->priority=data->priority+(p->owner->priority<<18);
    e->address=(uint)tag&0x0FFFFFFF;
    e->tag=p->tag;
}
} } // namespace M_RenderSpriteGeometry

// 0x2177E0 RenderSpritePicture
namespace M_func_002177E0 { extern "C" {
// Queue transformed sprite parameters and a direct GIF payload.
typedef unsigned int uint;
typedef unsigned long ulong;
struct POSITION {char pad[0x9C];float screenX,screenY;};   /* PostWidgetLoad's ownerWidget */
struct OWNER {char pad[0xA4];int priority;char pada8[0x20];struct POSITION *ownerWidget;};
struct GEO {char pad[0x18];int count;void *data;};
struct S {int pad,type;struct OWNER *owner;char padc[8];union {struct GEO *geo;int raw;};int rawtag;union {int tag;int count;};void *buffer;};
struct T {float a,b,c,d,x,y;};
struct M4 {float a[16];};
struct D {int a,mode;float priority;uint limit;char pad10[0x1C];float fade;};
struct R {char pad[0xC];unsigned char *cur;};
struct E {int kind;float priority;uint address;int tag;};
extern struct R D_002DEA28;
void *func_002333B4(void *,int,int) __asm__("memset");
struct E *AllocRenderListEntry(void);
static inline void Word(struct R *r,uint word){*(uint *)r->cur=word;r->cur+=4;}
static inline void Align(struct R *r){while((uint)r->cur&15){*(uint *)r->cur=0;r->cur+=4;}}
static inline void CopyWords(int *d,const int *s,int n){int rem=n&3,cBulk=n-rem,i;for(i=0;i<rem;i++)*d++=*s++;for(i=0;i<cBulk;i+=4){int a=s[0],b=s[1],c=s[2],e=s[3];s+=4;d[0]=a;d[1]=b;d[2]=c;d[3]=e;d+=4;}}
static inline void Unpack(struct R *r,const int *s,int n,uint header){Word(r,header);CopyWords((int *)r->cur,s,n);r->cur+=n*4;}

void CopyAqw__FPvT0i(void *,void *,int);
static inline void AlignTo(struct R *r,int word){while(((uint)r->cur&15)!=4*word)Word(r,0);}
static inline void Direct(struct R *r,int n,void *source){AlignTo(r,3);Word(r,0x50000000|n);CopyAqw__FPvT0i(r->cur,source,n);r->cur+=n*16;}
/** @sly2-annot addr=0x2177E0
 * Purpose: transformed sprite parameters + direct GIF payload
 * Info: proposed name RenderSpritePicture (T2); TU hud
 * Confidence: low (draft) [T2 0.40] - evidence: static analysis; sources: r7c_hud
 * See: docs/reference/functions/hud-ui.md#hud-0x210eb0-0x21bb88
 * @end-sly2-annot */
// Renders a picture sprite: transformed parameters plus a direct GIF payload. (inferred)
void RenderSpritePicture(struct S *p,struct T *t,struct D *data,int unused){
    struct M4 m;
    unsigned char *tag;
    struct E *e;
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
    tag=D_002DEA28.cur;
    *(ulong *)tag=0x10000000UL;D_002DEA28.cur+=16;
    Unpack(&D_002DEA28,(const int *)&m,16,0x6C04C000);
    Unpack(&D_002DEA28,(const int *)data,12,0x6C03C004);
    if(p->type==SPRITE_KIND_EFFECT)Direct(&D_002DEA28,p->geo->count,p->geo->data);
    else Direct(&D_002DEA28,p->count,p->buffer);
    Align(&D_002DEA28);
    *(uint *)tag|=((D_002DEA28.cur-tag)>>4)-1;
    *(ulong *)D_002DEA28.cur=0x60000000UL;D_002DEA28.cur+=16;
    e=AllocRenderListEntry();
    e->kind=0x15;e->priority=data->priority+(p->owner->priority<<18);
    e->address=(uint)tag&0x0FFFFFFF;
    if(p->type==SPRITE_KIND_EFFECT)e->tag=p->tag;else e->tag=p->rawtag;
}
} } // namespace M_func_002177E0

// 0x217BA0 StartSpriteAnimById
namespace M_func_00217BA0 { extern "C" {
struct E { char pad[0x14]; int id; };
struct S { unsigned short numAnims; short pad; struct E *anims; char pad8[0x20]; int isEnabled; };
void StartSpriteAnim(struct S *group, int i);
// 0x217B98: dead-code scrap (sly2/scrap.h)
SLY2_SCRAP("sd $zero, 0x10($4)");

/** @sly2-annot addr=0x217BA0
 * Purpose: find anim index by id (0x314 -> 0) and start
 * Info: proposed name StartSpriteAnimById (T2); TU hud
 * Confidence: low (draft) [T2 0.45] - evidence: static analysis; sources: r7c_hud
 * See: docs/reference/functions/hud-ui.md#hud-0x210eb0-0x21bb88
 * @end-sly2-annot */
// Starts the sprite animation with the given id (0x314 = the first). (inferred)
void StartSpriteAnimById(struct S *group, int animId)
{
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
} } // namespace M_func_00217BA0

// 0x217C20 StartSpriteAnim
namespace M_func_00217C20 {
struct T { char pad[8]; short animIndex; };   /* slot 0 */
struct S { char pad[8]; T *slots; char padc[0x1C]; int isEnabled; };
struct RGBA { unsigned int u; RGBA(unsigned int x) { u = x; } } __attribute__((aligned(16)));
extern "C" char D_002A3A10[];
extern "C" void StopSprite(S *p, int i);
extern "C" void StartSpriteAnimAt(S *p, int i, int z, void *q, float f, const RGBA &c0, const RGBA &c1);
/** @sly2-annot addr=0x217C20
 * Purpose: stop if instanced, start anim i at t=0 with default colours
 * Info: proposed name StartSpriteAnim (T2); TU hud
 * Notes: names_v2 C proposed UpdateAttract__FP7ATTRACT (0.167)
 * Confidence: low (draft) [T2 0.45] - evidence: static naming pass; sources: r7c_hud
 * See: docs/reference/functions/hud-ui.md#hud-0x210eb0-0x21bb88
 * @end-sly2-annot */
// Starts sprite animation i from time 0 with default colours (stopping it first if instanced). (inferred)
extern "C" void StartSpriteAnim(S *group, int animIndex)
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
    RGBA(const RGBA &c) { *(RGBA2 *)this = *(const RGBA2 *)&c; }
} __attribute__((aligned(16)));
struct E { char pad[0x14]; int id; };
struct S { unsigned short numAnims; short pad; E *anims; char pad8[8]; unsigned long lastStartFrame; float lastStartTime; int lastStartAnim; int pendingAnim; };   /* a sprite group (inferred: the main-track restart state) */
extern "C" void StartSpriteAnimAt(S *p, int i, int z, void *q, float f, RGBA c0, RGBA c1);
/** @sly2-annot addr=0x217CA8
 * Purpose: start anim k in a reuse slot
 * Info: proposed name StartSpriteAnimSlot (T2); TU hud
 * Confidence: low (draft) [T2 0.35] - evidence: static analysis; sources: r7c_hud
 * See: docs/reference/functions/hud-ui.md#hud-0x210eb0-0x21bb88
 * @end-sly2-annot */
// Starts animation k in a reusable sprite slot. (inferred)
extern "C" void StartSpriteAnimSlot(S *group, int animId, int slot, void *matrix, RGBA c0, RGBA c1)
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
    RGBA(const RGBA &c) { *(RGBA2 *)this = *(const RGBA2 *)&c; }
} __attribute__((aligned(16)));
struct M23 { float a[6]; } __attribute__((aligned(16)));
struct E { char pad[8]; short animIndex; short keyIndex; float time; int isFinished; void *unknown14; };
struct F { char pad[0x18]; };
struct S { int counts; F *anims; E *slots; char padc[0x20]; M23 *xform; };
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
extern "C" void AdvanceSpriteAnim(float,F *pf, int z, M23 *pm, RGBA c0, RGBA c1);
/** @sly2-annot addr=0x217D98
 * Purpose: select initial frame/time then draw
 * Info: proposed name StartSpriteAnimAt (T2); TU hud
 * Notes: names_v2 C proposed DrawLineScreen__FUiUiUiUiUiUiG4RGBAi (0.168)
 * Confidence: low (draft) [T2 0.40] - evidence: static naming pass; sources: r7c_hud
 * See: docs/reference/functions/hud-ui.md#hud-0x210eb0-0x21bb88
 * @end-sly2-annot */
// Starts a sprite animation at a given frame / time and draws it. (inferred)
extern "C" void StartSpriteAnimAt(S *group, int animIndex, int slot, M23 *matrix, float t, RGBA c0, RGBA c1)
{
    E *state = &group->slots[slot];
    M23 m;
    state->animIndex = animIndex;
    state->isFinished = 0;
    state->time = t;
    state->keyIndex = 0;
    M23 *pmUse;
    if (group->xform) {
        m = Mul(*matrix, *group->xform);
        pmUse = &m;
    } else {
        pmUse = matrix;
    }
    AdvanceSpriteAnim(t, &group->anims[state->animIndex], slot, pmUse, c0, c1);
}
} // namespace M_func_00217D98

// 0x217EF8 IFreeSpriteSlot
namespace M_IFreeSpriteSlot { extern "C" {
struct E { char pad[8]; short animIndex; char pad2[14]; };
struct S { short numAnims; unsigned short numSlots; char pad[4]; struct E *slots; };
/** @sly2-annot addr=0x217EF8
 * Purpose: first reuse slot with h<0
 * Info: proposed name IFreeSpriteSlot (T1); TU hud
 * Confidence: mid (draft) [T1 0.50] - evidence: static analysis; sources: r7c_hud
 * See: docs/reference/functions/hud-ui.md#hud-0x210eb0-0x21bb88
 * @end-sly2-annot */
// First free reusable sprite slot.
int IFreeSpriteSlot(struct S *group)
{
    int i;
    int count = group->numSlots;
    for (i = 0; i < count; i++) {
        if (group->slots[i].animIndex < 0   /* free */)
            return i;
    }
    return 0;
}
} } // namespace M_IFreeSpriteSlot

// 0x217F38 ResetSpriteSlotItems
namespace M_func_00217F38 { extern "C" {
struct Y { char pad[0x30]; void *descriptor; char pad2[0x3C]; };   /* a frame item */
struct F { int numItems; struct Y *items; char pad[0x10]; };
struct S { int counts; int anims; struct F *slots; };
void ReleaseSpriteItem(void *p, struct Y *y);
void InitSpriteItem(struct Y *y);
static inline void ReleaseItems(int n, struct Y *items)
{
    int k; for (k = 0; k < n; k++) { struct Y *item = &items[k]; if (item->descriptor) ReleaseSpriteItem(item->descriptor, item); else InitSpriteItem(item); }
}
/** @sly2-annot addr=0x217F38
 * Purpose: reinit / release every item of slot i
 * Info: proposed name ResetSpriteSlotItems (T2); TU hud
 * Confidence: low (draft) [T2 0.45] - evidence: static analysis; sources: r7c_hud
 * See: docs/reference/functions/hud-ui.md#hud-0x210eb0-0x21bb88
 * @end-sly2-annot */
// Resets / releases every item of sprite slot i. (inferred)
void ResetSpriteSlotItems(struct S *group, int slot)
{
    ReleaseItems(group->slots[slot].numItems, group->slots[slot].items);
}
} } // namespace M_func_00217F38

// 0x217FB8 SetSpriteDefaultPhase
namespace M_func_00217FB8 { extern "C" {
struct S { char pad[0x28]; int isEnabled; };
void SetSpriteAnimPhase(struct S *p, int a, int b);
/** @sly2-annot addr=0x217FB8
 * Purpose: phase seek of default anim
 * Info: proposed name SetSpriteDefaultPhase (T2); TU hud
 * Confidence: low (draft) [T2 0.40] - evidence: static analysis; sources: r7c_hud
 * See: docs/reference/functions/hud-ui.md#hud-0x210eb0-0x21bb88
 * @end-sly2-annot */
// Seeks the default animation by phase. (inferred)
void SetSpriteDefaultPhase(struct S *group)
{
    if (group->isEnabled)
        SetSpriteAnimPhase(group, 0, 0);
}
} } // namespace M_func_00217FB8

// 0x217FE0 SetSpriteAnimPhase
namespace M_func_00217FE0 { extern "C" {
struct E { char pad[8]; short animIndex; char pad2[14]; };
struct F { char pad[8]; float duration; char pad2[12]; };
struct S { int counts; struct F *anims; struct E *slots; };
void SetSpriteAnimTime(struct S *p, int i, int j, float f);
/** @sly2-annot addr=0x217FE0
 * Purpose: clamp g to [0,1], seek anim to g*duration
 * Info: proposed name SetSpriteAnimPhase (T2); TU hud
 * Confidence: low (draft) [T2 0.45] - evidence: static analysis; sources: r7c_hud
 * See: docs/reference/functions/hud-ui.md#hud-0x210eb0-0x21bb88
 * @end-sly2-annot */
// Seeks a sprite animation to a fraction (clamped to 0..1) of its duration. (inferred)
void SetSpriteAnimPhase(struct S *group, int slot, int item, float phase)
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
} } // namespace M_func_00217FE0

// 0x218038 SetSpriteDefaultTime
namespace M_func_00218038 { extern "C" {
struct S { char pad[0x28]; int isEnabled; };
void SetSpriteAnimTime(struct S *p, int a, int b);
/** @sly2-annot addr=0x218038
 * Purpose: time seek of default anim
 * Info: proposed name SetSpriteDefaultTime (T2); TU hud
 * Confidence: low (draft) [T2 0.35] - evidence: static analysis; sources: r7c_hud
 * See: docs/reference/functions/hud-ui.md#hud-0x210eb0-0x21bb88
 * @end-sly2-annot */
// Seeks the default animation by time. (inferred)
void SetSpriteDefaultTime(struct S *group)
{
    if (group->isEnabled)
        SetSpriteAnimTime(group, 0, 0);
}
} } // namespace M_func_00218038

// 0x218060 SetSpriteAnimTime
namespace M_func_00218060 {
// Refresh a reused sprite at zero time, then at its clamped requested time.
typedef int qword __attribute__((mode(TI)));
struct RGBA2 { unsigned char r, g, b, a; };
struct RGBA {
    unsigned char r, g, b, a;
    RGBA() {}
    RGBA(const RGBA &c) { *(RGBA2 *)this = *(const RGBA2 *)&c; }
    RGBA &operator=(const RGBA &c){*(RGBA2 *)this=*(const RGBA2 *)&c;return *this;}
} __attribute__((aligned(16)));
struct QQ { qword a, b; };
struct Y { RGBA2 color0; RGBA2 color1; char pad8[8]; QQ matrix; };   /* a frame item */
struct E { int count;void *items;short animIndex;short keyIndex;float time;int isFinished;void *unknown14; };
struct F { char pad[8]; float duration; char padc[0xC]; };
struct S;
struct A { int counts; F *anims; E *slots; char padc[0x18]; S *owner; char pad28[8]; };   /* a sprite group */
struct S { char pad[0x84]; A *spriteGroups; };
extern "C" int FindSpriteItemPose(S *p, int k, QQ *pq, RGBA *pc0, RGBA *pc1, int c, void *ay);
extern "C" void ResetSpriteSlotItems(A *p, int i);
// F12 is independent of the integer slots. These pointers address real
// independent caller copies of the nontrivial by-value colors (A3/T0).
extern "C" void UpdateSpriteSlot(float g, A *p, int i, QQ *pq, const RGBA *c0, const RGBA *c1);
static inline float GMin(float a, float b) { return b < a ? b : a; }
static inline float GMax(float a, float b) { return a < b ? b : a; }
/** @sly2-annot addr=0x218060
 * Purpose: refresh reused sprite at t=0 then at clamped time
 * Info: proposed name SetSpriteAnimTime (T2); TU hud
 * Confidence: low (draft) [T2 0.40] - evidence: static analysis; sources: r7c_hud
 * See: docs/reference/functions/hud-ui.md#hud-0x210eb0-0x21bb88
 * @end-sly2-annot */
// Seeks a reused sprite animation to a clamped time (refreshing it at time 0 first). (inferred)
extern "C" void SetSpriteAnimTime(A *group, int slot, Y *item, float time)
{
    QQ q;
    RGBA c0, c1;
    int animIndex = group->slots[slot].animIndex;
    if (animIndex < 0)
        return;
    time = GMin(group->anims[animIndex].duration, GMax(0.0f, time));
    if (item) {
        q = item->matrix;
        *(RGBA2 *)&c0 = item->color0;
        *(RGBA2 *)&c1 = item->color1;
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
struct RGBA { unsigned int u; RGBA(unsigned int x) {u=x;} } __attribute__((aligned(16)));
struct M23 {float a[6];} __attribute__((aligned(16)));
struct O {char pad[0xF4]; void *followObject; M23 *xform;};   /* the widget: world object to follow (inferred), transform override */
struct S {char pad[0x24]; O *owner; int isEnabled;};   /* the main sprite group */
extern "C" M23 D_002A3A10;
struct G;
extern "C" G D_002DDF40;
extern "C" qword GetLoWorldPos(void *);
extern "C" qword ConvertCmWorldToScreen(void *,qword);
extern "C" void UpdateSpriteSlot(S *,int,M23 *,float,const RGBA &,const RGBA &);
static inline float GetX(qword q) {float f; __asm__("mtc1 %1,%0":"=f"(f):"r"(q));return f;}
static inline float GetY(qword q) {float f; __asm__("prot3w %0,%1":"=r"(f):"r"(q));return f;}
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
/** @sly2-annot addr=0x218220
 * Purpose: advance main sprite by dt; world-anchored position via PosLoWorld + ConvertCmWorldToScreen
 * Info: proposed name AdvanceWidgetSprite (T2); TU hud
 * Confidence: low (draft) [T2 0.45] - evidence: static analysis; sources: r7c_hud
 * See: docs/reference/functions/hud-ui.md#hud-0x210eb0-0x21bb88
 * @end-sly2-annot */
// Advances the widget's main sprite by dt; a world-anchored widget follows its object's screen position. (inferred)
extern "C" void AdvanceWidgetSprite(S *group,float dt) {
    if(group->isEnabled) {
        M23 m=D_002A3A10;
        if(group->owner->followObject) {
            qword worldPos=GetLoWorldPos(group->owner->followObject); qword screenPos=ConvertCmWorldToScreen(&D_002DDF40,worldPos);
            m.a[4] = GetX(screenPos)*320.0f+320.0f;
            m.a[5] = GetY(screenPos)*-246.40001f+246.40001f;
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
    RGBA(const RGBA &c) { *(RGBA2 *)this = *(const RGBA2 *)&c; }
} __attribute__((aligned(16)));
struct M23 { float a[6]; } __attribute__((aligned(16)));
struct E { char pad[8]; short animIndex; short keyIndex; float time; int isFinished; int sound; };
struct F {char pad[8];float duration;char padC[0xC];};
struct S { int counts; F *anims; E *slots; char padc[0x20]; M23 *xform; };
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
extern "C" void InterpolateSpriteSlot(float,F *,int,M23 *,RGBA,RGBA);
extern "C" void AdvanceSpriteAnim(float,F *,int,M23 *,RGBA,RGBA);
/** @sly2-annot addr=0x2183E8
 * Purpose: forward update to 0x2188B0 or 0x219FE0
 * Info: proposed name UpdateSpriteSlot (T2); TU hud
 * Confidence: low (draft) [T2 0.35] - evidence: static analysis; sources: r7c_hud
 * See: docs/reference/functions/hud-ui.md#hud-0x210eb0-0x21bb88
 * @end-sly2-annot */
// Forwards a sprite slot's update to the reusable or the full animation path. (inferred)
extern "C" void UpdateSpriteSlot(S *group,int slot,M23 *matrix,float dt,RGBA c0,RGBA c1) {
    E *state=&group->slots[slot];
    int animIndex=state->animIndex;
    if(animIndex<0)return;
    M23 m;
    M23 *pmUse;
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
/** @sly2-annot addr=0x2185D0
 * Purpose: draw sprite (slot 0)
 * Info: proposed name DrawSprite (T2); TU hud
 * Confidence: low (draft) [T2 0.40] - evidence: static analysis; sources: r7c_hud
 * See: docs/reference/functions/hud-ui.md#hud-0x210eb0-0x21bb88
 * @end-sly2-annot */
// Draws a sprite group (slot 0). (inferred)
extern "C" int DrawSprite(SPRITEGRP *spriteGroup)
{
    int result = spriteGroup->isEnabled;
    if (result)
        result = func_002185F8I(spriteGroup, 0, 0);
    return result;
}

// 0x2185F8 DrawSpriteSlot
/** @sly2-annot addr=0x2185F8
 * Purpose: draw items of a slot
 * Info: proposed name DrawSpriteSlot (T2); TU hud
 * Confidence: low (draft) [T2 0.35] - evidence: static analysis; sources: r7c_hud
 * See: docs/reference/functions/hud-ui.md#hud-0x210eb0-0x21bb88
 * @end-sly2-annot */
// Draws the items of a sprite slot's current animation, if it plays one. (inferred)
extern "C" void DrawSpriteSlot(SPRITEGRP *spriteGroup, int slot, int drawFlags)
{
    short animIndex = spriteGroup->slots[slot].animIndex;
    if (animIndex >= 0)
        DrawSpriteItems(&spriteGroup->anims[animIndex], slot, drawFlags);
}

// 0x218638 StopSprite
/** @sly2-annot addr=0x218638
 * Purpose: stop slot 0
 * Info: proposed name StopSprite (T2); TU hud
 * Confidence: low (draft) [T2 0.40] - evidence: static analysis; sources: r7c_hud
 * See: docs/reference/functions/hud-ui.md#hud-0x210eb0-0x21bb88
 * @end-sly2-annot */
// Stops a sprite group (slot 0). (inferred)
extern "C" void StopSprite(SPRITEGRP *spriteGroup)
{
    if (spriteGroup->isEnabled)
        StopSpriteSlot(spriteGroup, 0);
}

// 0x218660 StopSpriteSlot
namespace M_func_00218660 { extern "C" {
struct E { char pad[8]; short animIndex; short pad2; float time; int isFinished; void *sound; };
struct S { char pad[8]; struct E *slots; char padc[4]; unsigned long lastStartFrame; float lastStartTime; int lastStartAnim; int pendingAnim; };
struct CLOCK { unsigned long ticks; };
void StopSnd(void *p, float f);
void RcPtr_Assign(void **pp, void *next);
void ResetSpriteSlotItems(struct S *p, int i);
static inline void Release(void **pp)
{
    if (pp && *pp) {
        StopSnd(*pp, 0.0f);
        RcPtr_Assign(pp, 0);
    }
}
/** @sly2-annot addr=0x218660
 * Purpose: stop slot sounds (StopSnd) and reset its items
 * Info: proposed name StopSpriteSlot (T2); TU hud
 * Confidence: low (draft) [T2 0.45] - evidence: static analysis; sources: r7c_hud
 * See: docs/reference/functions/hud-ui.md#hud-0x210eb0-0x21bb88
 * @end-sly2-annot */
// Stops a sprite slot's sounds and resets its items. (inferred)
void StopSpriteSlot(struct S *group, int slot)
{
    struct E *state = (struct E*)(slot*sizeof(struct E)+(int)group->slots);   /* &group->slots[slot] */
    if (state->animIndex >= 0) {
        /* remember where it stopped, so a main-track restart this frame resumes it (StartSpriteAnimSlot) */
        float t=state->time;
        group->lastStartAnim = state->animIndex;
        group->lastStartTime = t;
        group->lastStartFrame = (*(struct CLOCK *)&g_clock.frameTick).ticks;
        group->pendingAnim = -1;
        state->animIndex = -1;
        state->isFinished = 0;
        Release(&state->sound);
    }
    ResetSpriteSlotItems(group, slot);
}
} } // namespace M_func_00218660

// 0x218710 FSpriteAnimActive
namespace M_FSpriteAnimActive { extern "C" {
struct B { char pad[8]; short animIndex; char pad2[6]; int isFinished; };
struct S { char pad[8]; struct B *slots; };
/** @sly2-annot addr=0x218710
 * Purpose: h!=0 && !done
 * Info: proposed name FSpriteAnimActive (T1); TU hud
 * Confidence: high [T1 0.85] - evidence: static analysis; sources: r7c_hud+verdicts_followups
 * See: docs/engine/hud-ui.md#5-key-functions, docs/reference/functions/hud-ui.md#hud-0x210eb0-0x21bb88
 * @end-sly2-annot */
// True while the sprite animation is running.
int FSpriteAnimActive(struct S *group)
{
    struct B *mainSlot = group->slots;
    if (mainSlot->animIndex != 0 && mainSlot->isFinished == 0)
        return 1;
    return 0;
}
} } // namespace M_FSpriteAnimActive

// 0x218738 GetSpriteMainState
/** @sly2-annot addr=0x218738
 * Purpose: returns pt(+8)->x10
 * Info: proposed name GetSpriteMainState (T2); TU hud
 * Confidence: low (draft) [T2 0.25] - evidence: static analysis; sources: r7c_hud
 * See: docs/reference/functions/hud-ui.md#hud-0x210eb0-0x21bb88
 * @end-sly2-annot */
// State of the sprite group's main animation. (inferred)
extern "C" int GetSpriteMainState(SPRITEGRP *spriteGroup)
{
    return spriteGroup->slots->isFinished;
}

// 0x218748 FinishSpriteSoundEvent
namespace M_func_00218748 { extern "C" {
struct E { char pad[0x10]; int isFinished; void *sound; };   /* a playback slot */
struct S { char pad[8]; struct E *slots; };
void PauseSnd(void *p);
static inline struct E *Entries(const struct S *p){struct E *value;__asm__("lw %0,8(%1)":"=r"(value):"0"(p),"m"(p->slots));return value;}
static inline void *SoundRead(void *const *slot){register void *value __asm__("$3");__asm__("lw %0,%1":"=r"(value):"m"(*slot));return value;}
static inline struct E *EntryAt(struct E *entries,int index){register struct E *value asm("$2");register int bytes asm("$5")=index*sizeof(struct E);asm("addu %0,%1,%2":"=r"(value):"r"(bytes),"r"(entries));return value;}
/** @sly2-annot addr=0x218748
 * Purpose: mark entry done and stop/pause its sound
 * Info: proposed name FinishSpriteSoundEvent (T2); TU hud
 * Confidence: low (draft) [T2 0.30] - evidence: static analysis; sources: r7c_hud
 * See: docs/reference/functions/hud-ui.md#hud-0x210eb0-0x21bb88
 * @end-sly2-annot */
// Marks a sprite sound event done and stops or pauses its sound. (inferred)
void FinishSpriteSoundEvent(struct S *p, int slot)
{
    void *sound;
    p->slots[slot].isFinished = 1;
    sound=SoundRead(&EntryAt(Entries(p),slot)->sound);
    if (sound)
        PauseSnd(sound);
}
} } // namespace M_func_00218748

// 0x218790 CombineSpriteColors
namespace M_CombineSpriteColors { extern "C" {
static inline float GMin(float a, float b) { return b < a ? b : a; }
static inline float GMax(float a, float b) { return a < b ? b : a; }
/** @sly2-annot addr=0x218790
 * Purpose: combine parent/child additive and multiplicative RGBA
 * Info: proposed name CombineSpriteColors (T1); TU hud
 * Confidence: mid (draft) [T1 0.50] - evidence: static analysis; sources: r7c_hud
 * See: docs/reference/functions/hud-ui.md#hud-0x210eb0-0x21bb88
 * @end-sly2-annot */
// Combines parent and child colours (additive and multiplicative).
void CombineSpriteColors(unsigned char *pa, unsigned char *pb, unsigned char *pc, unsigned char *pd,
                   unsigned char *pout0, unsigned char *pout1)
{
    int i;
    for (i = 0; i < 4; i++) {
        float x = GMax(GMin((float)(pa[i] - 127) * 0.007874016f, 1.0f), -1.0f);
        float y = GMax(GMin((float)(pc[i] - 127) * 0.007874016f, 1.0f), -1.0f);
        float e = (float)pb[i] - 127.0f;
        float d = (float)pd[i] - 127.0f;
        pout0[i] = (int)(GMax(GMin(x * y, 1.0f), -1.0f) * 127.0f + 128.0f);
        pout1[i] = (int)(GMax(GMin(x * d + e, 128.0f), -127.0f) + 127.0f);
    }
}
} } // namespace M_CombineSpriteColors

// 0x2188B0 InterpolateSpriteSlot
namespace M_func_002188B0 {
// Update reusable sprite entries at their cached animation time.
// UpdateSpriteItem's float (frame duration) is its THIRD argument in the source: that order of the call's argument moves is what matches.
typedef unsigned char uchar;
typedef unsigned short ushort;
struct RGBA_RAW {uchar r,g,b,a;};
struct RGBA {uchar r,g,b,a;RGBA(){}RGBA(const RGBA &c){*(RGBA_RAW *)this=*(const RGBA_RAW *)&c;}RGBA(const RGBA_RAW &c){*(RGBA_RAW *)this=c;}RGBA &operator=(const RGBA &c){*(RGBA_RAW *)this=*(const RGBA_RAW *)&c;return *this;}} __attribute__((aligned(16)));
struct M23 {float a[6];} __attribute__((aligned(16)));
struct SPRITE;
struct ITEM {RGBA_RAW additive,multiply;char pad8[8];M23 matrix;SPRITE *sprite;int kind;float previousTime;RGBA_RAW previousAdditive,previousMultiply;M23 *previousMatrix;float nextTime;RGBA_RAW nextAdditive,nextMultiply;M23 *nextMatrix;int index;float cachedTime;uchar interpolate;char pad61[7];int flags,pad6C;};
struct REUSE {int count;ITEM *items;ushort index,phase;float time;int field10,field14;};
struct ANIM;
struct GROUP {ushort animCount,reuseCount;ANIM *animations;REUSE *reuse;char padC[0x14];int index;void *owner;int first,pad2C;};
struct ANIM {ushort count,pad2;void *keys;float duration;void *owner;GROUP *group;int id;};
extern "C" {
void ClearSle(int *);
void CombineSpriteColors(RGBA *,RGBA *,RGBA *,RGBA *,RGBA *,RGBA *);
void UpdateSpriteItem(SPRITE *,ITEM *,float,M23 *,RGBA,RGBA);
}
static inline int NLerp(float t,int previous,int next){
 if(previous==next)return next;
 if(previous<next)return previous+(int)(t*((float)next+0.999f-(float)previous));
 return next+(int)((1.0f-t)*((float)previous+0.999f-(float)next));
}
static inline RGBA BlendColors(float t,RGBA previous,RGBA next){
 RGBA result;
 result.r=NLerp(t,previous.r,next.r);result.g=NLerp(t,previous.g,next.g);result.b=NLerp(t,previous.b,next.b);result.a=NLerp(t,previous.a,next.a);
 return result;
}
static inline M23 BlendMatrix(float t,const M23 &previous,const M23 &next){
 M23 result;
 for(int n=0;n<6;n++)result.a[n]=(1.0f-t)*previous.a[n]+t*next.a[n];
 return result;
}
static inline M23 Mul(const M23 &q,const M23 &n){
 float m0=q.a[0],m1=q.a[1],m2=q.a[2],m3=q.a[3],m4=q.a[4],m5=q.a[5];M23 r;
 r.a[0]=m0*n.a[0]+m1*n.a[2];r.a[1]=m0*n.a[1]+m1*n.a[3];r.a[2]=m2*n.a[0]+m3*n.a[2];r.a[3]=m2*n.a[1]+m3*n.a[3];r.a[4]=m4*n.a[0]+m5*n.a[2]+n.a[4];r.a[5]=m4*n.a[1]+m5*n.a[3]+n.a[5];return r;
}
/** @sly2-annot addr=0x2188B0
 * Purpose: update reusable sprite entries at cached time
 * Info: proposed name InterpolateSpriteSlot (T2); TU hud
 * Confidence: low (draft) [T2 0.40] - evidence: static analysis; sources: r7c_hud
 * See: docs/reference/functions/hud-ui.md#hud-0x210eb0-0x21bb88
 * @end-sly2-annot */
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
namespace M_func_002190F8 { extern "C" {
struct E { char pad[7]; unsigned char numActions; unsigned char *actions; char padc[0x14]; };   /* an animation key */
struct S { int numKeys; struct E *keys; char pad8[8]; void *group; };   /* an animation */
void FinishSpriteSoundEvent(void *p, int k);
/** @sly2-annot addr=0x2190F8
 * Purpose: for action bytes ==7 call 0x218748
 * Info: proposed name FinishSpriteSoundEvents (T2); TU hud
 * Confidence: low (draft) [T2 0.30] - evidence: static analysis; sources: r7c_hud
 * See: docs/reference/functions/hud-ui.md#hud-0x210eb0-0x21bb88
 * @end-sly2-annot */
// Finishes every sound event (action 7) of a sprite key. (inferred)
void FinishSpriteSoundEvents(struct S *anim, int key, int slot)
{
    struct E *keyData = &anim->keys[key];
    int j;
    for (j = 0; j < keyData->numActions; j++) {
        if (keyData->actions[j] == 7)   /* a sound action */
            FinishSpriteSoundEvent(anim->group, slot);
    }
}
} } // namespace M_func_002190F8

// 0x219190 HandleSpriteKeyEvents
namespace M_HandleSpriteKeyEvents {
// Dispatch sprite actions and manage sound slots.
struct O;
struct VT { char pad[0x40]; short d; short pd; int (*kind)(void *); };
struct M { char pad[0xC]; struct VT *vt; };
struct E { char pad[0x14]; struct M *pm; };
struct R { char pad[8]; struct E *ae; };
struct A { int kind; int id; };
struct F { char pad[0x18]; unsigned char count; char pad1[3]; struct A *aa; };
struct S { char pad[4]; struct F *af; char pad1[8]; struct R *r; };
extern "C" { extern char D_003DD040[]; }
extern "C" struct M *PsndStartSound(int id);
extern "C" void SetSndSourceLo(struct M *, struct O *);
extern "C" void StopSnd(struct M *, float);
extern "C" void RcPtr_Assign(struct M **pp, struct M *m);
extern "C" void *StartDialog(void *p,int a,int b);
extern "C" void StopDialog(void *p);
static inline M *SlotSound(M *const &slot){register M *value asm("$4");asm("lw %0,0(%1)":"=r"(value):"r"(&slot),"m"(slot));return value;}
static inline void Stop(M **pp){if(pp){M *sound=SlotSound(*pp);if(sound){StopSnd(sound,0.0f);RcPtr_Assign(pp,0);}}}
static inline unsigned int Presence(M *const &slot){register unsigned int value asm("$3");__asm__("lw %0,%1":"=r"(value):"m"(slot));return value;}
static inline int EntryOffset(int index,int stride){int value;__asm__("mult %0,%1,%2":"=r"(value):"r"(index),"r"(stride):"hi","lo");return value;}
static inline int ActionKind(const A &action){register int value asm("$3");asm("lw %0,%1":"=r"(value):"m"(action.kind));return value;}
static inline int StopOffset(int index,int stride){register int value asm("$5");asm("mult %0,%1,%2":"=r"(value):"r"(index),"r"(stride):"hi","lo");return value;}
static inline R *StopReceiver(const S *owner){register R *value asm("$3");asm("lw %0,%1":"=r"(value):"m"(owner->r));return value;}
static inline E *StopArray(const R *receiver){register E *value asm("$4");asm("lw %0,%1":"=r"(value):"m"(receiver->ae));return value;}
static inline M **SlotAddress(E *entry){M **value;asm("addiu %0,%0,%2":"=r"(value):"0"(entry),"i"(sizeof(E)-sizeof(M *)));return value;}
static inline E *StopEntry(E *array,int offset){register E *value asm("$2");asm("addu %0,%1,%2":"=r"(value):"r"(offset),"r"(array));return value;}
/** @sly2-annot addr=0x219190
 * Purpose: sprite key actions: start/stop sound, StartDialog/StopDialog
 * Info: proposed name HandleSpriteKeyEvents (T1); TU hud
 * Confidence: mid (draft) [T1 0.50] - evidence: static analysis; sources: r7c_hud
 * See: docs/reference/functions/hud-ui.md#hud-0x210eb0-0x21bb88
 * @end-sly2-annot */
// Runs a sprite key's actions: start / stop sounds, start / stop dialog.
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
            struct M *m=PsndStartSound(a->id);
            SetSndSourceLo(m,0);
            if(m->vt->kind((char *)m+m->vt->d)==2)
                RcPtr_Assign(&((E *)(createOffset+(int)p->r->ae))->pm,m);
            break;
        }
        case 1: {
            R *receiver=StopReceiver(p);int offset=StopOffset(i,stride);
            E *e=StopEntry(StopArray(receiver),offset);
            if(Presence(e->pm)) Stop(SlotAddress(e));
            break;
        }
        case 2: StartDialog(D_003DD040,a->id,0); break;
        case 3: StopDialog(D_003DD040); break;
        }
    } while(++j<f->count);
}
} // namespace M_HandleSpriteKeyEvents

// 0x219348 ResetSpriteKeyItems
namespace M_ResetSpriteKeyItems { extern "C" {
struct Y { char pad[0x30]; void *descriptor; char pad2[0x3C]; };   /* a frame item */
struct F { int numItems; struct Y *items; char pad[0x10]; };   /* a playback slot */
struct Q { char pad[8]; struct F *slots; };   /* the group */
struct E { char pad[6]; unsigned char numItemRefs; char pad7[0xD]; unsigned short *itemRefs; char pad18[8]; };   /* an animation key */
struct S { int numKeys; struct E *keys; char pad8[8]; struct Q *group; };   /* an animation */
void ReleaseSpriteItem(void *p, struct Y *y);
void InitSpriteItem(struct Y *y);
/** @sly2-annot addr=0x219348
 * Purpose: reset items referenced by a key
 * Info: proposed name ResetSpriteKeyItems (T1); TU hud
 * Confidence: high [T1 0.85] - evidence: static analysis; sources: r7c_hud+verdicts_followups
 * See: docs/engine/hud-ui.md#5-key-functions, docs/reference/functions/hud-ui.md#hud-0x210eb0-0x21bb88
 * @end-sly2-annot */
// Resets the items a sprite key refers to.
void ResetSpriteKeyItems(struct S *anim, int key, int slot)
{
    struct E *keyData = &anim->keys[key];
    struct Y *items = anim->group->slots[slot].items;
    int k;
    for (k = 0; k < keyData->numItemRefs; k++) {
        struct Y *item = &items[keyData->itemRefs[k]];
        if (item->descriptor)
            ReleaseSpriteItem(item->descriptor, item);
        else
            InitSpriteItem(item);
    }
}
} } // namespace M_ResetSpriteKeyItems

// 0x219400 ApplySpriteKeyPoses
namespace M_ApplySpriteKeyPoses {
// ApplySpriteKeyPoses: apply one animation key's poses to the reuse slot's items (sprite animation, see AdvanceSpriteAnim).
// For every pose of key `phase`: reset the item's "previous" state (colours default to additive 0x7F7F7F7F / multiply
// 0xFFFFFFFF unless the pose has a COLORS entry), look in the following keys for the same item to find its "next"
// pose, then interpolate matrix and colours between them at the clamped time (GLimitLm), combine with the parent
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
    RGBA() {}
    RGBA(const RGBA &c) { *(RGBA_RAW *)this = *(const RGBA_RAW *)&c; }
    RGBA(const RGBA_RAW &c) { *(RGBA_RAW *)this = c; }
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
extern "C" {
void ClearSle(int *);
void *func_002333B4(void *, int, unsigned int) __asm__("memset");
void SetSpriteItemSource(SPRITE *, ITEM *, M23 *, RGBA, RGBA);
void ApplySpriteKeyPoses(float, float, ANIM *, int, int, M23 *, const RGBA *, const RGBA *);
void ReleaseSpriteItem(SPRITE *, ITEM *);
void CombineSpriteColors(RGBA *, RGBA *, RGBA *, RGBA *, RGBA *, RGBA *);
void UpdateMatchingSprite(float, SPRITE *, ITEM *, M23 *, RGBA, RGBA) __asm__("UpdateSpriteItem");
void UpdateCurrentSprite(float, SPRITE *, ITEM *, M23 *, RGBA, RGBA) __asm__("UpdateSpriteItem");
}

// Lerp one colour channel; the +0.999 rounds towards the far end.
static inline int NLerp(float t, int previous, int next)
{
    if (previous == next)
        return next;
    if (previous < next)
        return previous + (int)(t * ((float)next + 0.999f - (float)previous));
    return next + (int)((1.0f - t) * ((float)previous + 0.999f - (float)next));
}

static inline RGBA BlendColors(float t, RGBA previous, RGBA next)
{
    RGBA result;
    result.r = NLerp(t, previous.r, next.r);
    result.g = NLerp(t, previous.g, next.g);
    result.b = NLerp(t, previous.b, next.b);
    result.a = NLerp(t, previous.a, next.a);
    return result;
}

static inline M23 BlendMatrix(float t, const M23 &previous, const M23 &next)
{
    M23 result;
    for (int n = 0; n < 6; n++)
        result.a[n] = (1.0f - t) * previous.a[n] + t * next.a[n];
    return result;
}

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
    LM(Uninitialized) {}
    LM() {}
};
float GLimitLm(LM *, float) __asm__("GLimitLm__FP2LMf");

/** @sly2-annot addr=0x219400
 * Purpose: apply one animation key poses to items
 * Info: proposed name ApplySpriteKeyPoses (T1); TU hud
 * Confidence: mid (draft) [T1 0.50] - evidence: static analysis; sources: r7c_hud
 * See: docs/reference/functions/hud-ui.md#hud-0x210eb0-0x21bb88
 * @end-sly2-annot */
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
                limited = GLimitLm(&copy, duration);
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
    RGBA() {}
    RGBA(const RGBA &c) { *(RGBA_RAW *)this = *(const RGBA_RAW *)&c; }
    RGBA(const RGBA_RAW &c) { *(RGBA_RAW *)this = c; }
    const RGBA_RAW &Raw() const { return *(const RGBA_RAW *)this; }
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

extern "C" {
void ClearSle(int *);
void HandleSpriteKeyEvents(ANIM *, int, int);
void FinishSpriteSoundEvents(ANIM *, int, int);
void ResetSpriteKeyItems(ANIM *, int, int);
void ApplySpriteKeyPoses(float, float, ANIM *, int, int, M23 *, const RGBA *, const RGBA *);
void ReleaseSpriteItem(SPRITE *, ITEM *);
void CombineSpriteColors(RGBA *, RGBA *, RGBA *, RGBA *, RGBA *, RGBA *);
void UpdateSpriteItem(float, SPRITE *, ITEM *, M23 *, RGBA, RGBA);
}

// Lerp one colour channel; the +0.999 rounds towards the far end.
static inline int NLerp(float t, int previous, int next)
{
    if (previous == next)
        return next;
    if (previous < next)
        return previous + (int)(t * ((float)next + 0.999f - (float)previous));
    return next + (int)((1.0f - t) * ((float)previous + 0.999f - (float)next));
}

static inline RGBA BlendColors(float t, RGBA previous, RGBA next)
{
    RGBA result;
    result.r = NLerp(t, previous.r, next.r);
    result.g = NLerp(t, previous.g, next.g);
    result.b = NLerp(t, previous.b, next.b);
    result.a = NLerp(t, previous.a, next.a);
    return result;
}

static inline M23 BlendMatrix(float t, const M23 &previous, const M23 &next)
{
    M23 result;
    for (int n = 0; n < 6; n++)
        result.a[n] = (1.0f - t) * previous.a[n] + t * next.a[n];
    return result;
}

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

/** @sly2-annot addr=0x219FE0
 * Purpose: advance sprite animation to slot time and redraw changed items
 * Info: proposed name AdvanceSpriteAnim (T1); TU hud
 * Confidence: mid (draft) [T1 0.50] - evidence: static analysis; sources: r7c_hud
 * See: docs/reference/functions/hud-ui.md#hud-0x210eb0-0x21bb88
 * @end-sly2-annot */
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
namespace M_func_0021A998 { extern "C" {
struct Y { char pad[0x30]; void *descriptor; int kind; char pad2[0x38]; };   /* a frame item */
struct F { int numItems; struct Y *items; char pad[0x10]; };   /* a playback slot */
struct Q { char pad[8]; struct F *slots; };   /* the group */
struct S { char pad[0xC]; char *owner; struct Q *group; };   /* an animation; owner = the widget */
void QueueSpriteItem(void *descriptor, struct Y *item, int drawFlags);
void PrependSlEntry(void *p, struct Y *y);
static inline void Draw(struct S *anim, int n, struct Y *items, int drawFlags)
{
    int k;
    for (k = n - 1; k >= 0; k--) {
        struct Y *item = &items[k];
        if (item->descriptor)
            QueueSpriteItem(item->descriptor, item, drawFlags);
        else if (item->kind == 4)   /* no descriptor: queued as is (inferred) */
            PrependSlEntry(anim->owner + 0xFC, item);   /* the widget's render queue */
    }
}
/** @sly2-annot addr=0x21A998
 * Purpose: queue items in reverse order
 * Info: proposed name DrawSpriteItems (T2); TU hud
 * Confidence: low (draft) [T2 0.40] - evidence: static analysis; sources: r7c_hud
 * See: docs/reference/functions/hud-ui.md#hud-0x210eb0-0x21bb88
 * @end-sly2-annot */
// Queues a sprite's items for drawing in reverse order. (inferred)
void DrawSpriteItems(struct S *anim, int slot, int drawFlags)
{
    Draw(anim, anim->group->slots[slot].numItems, anim->group->slots[slot].items, drawFlags);
}
} } // namespace M_func_0021A998

// 0x21AA60 StaticInit_hud
namespace M_StaticInit_hud {
// Initialize sprite string generators and their supporting heaps.
struct LINEAR {void *vtable;int limit;void *buffer;int used;};
struct HEAP {void *vtable;int field4;};
struct POOL;
extern "C" void *func_002333B4(void *,int,unsigned int) __asm__("memset");
struct STRINGGEN {
 int id;void (*get)(char *,void *,void *);void *context;
 STRINGGEN(){func_002333B4(this,0,sizeof(*this));}
};
extern "C" {
LINEAR *__11CLinearHeapiPUc(LINEAR *,int,void *);
POOL *InitSlotheap(POOL *,int,int,int);
void BuildContextText(char *,void *,void *);
}
extern "C" { extern LINEAR D_003E1C70; }
extern "C" { extern HEAP D_003E1C80; }
extern "C" { extern char D_003E21A0[],D_002CA578[] __asm__("_vt$7CSwHeap"); }
extern "C" { extern STRINGGEN D_003E1C88[108]; }
extern "C" { extern void *D_002C40A0; }
extern "C" { extern int D_002DD2E0 __asm__("__sn_t9CSlotPool2i20i8192$s_pool"),D_002DD2E4 __asm__("__sn_t9CSlotPool2i24i2048$s_pool"),D_002DD2E8 __asm__("__sn_t9CSlotPool2i40i512$s_pool"),D_002DD2EC __asm__("__sn_t9CSlotPool2i28i2048$s_pool"),D_002DD2F0 __asm__("__sn_t9CSlotPool2i12i200$s_pool"),D_002DD2F4 __asm__("__sn_t9CSlotPool2i20i256$s_pool"); }
extern "C" { extern POOL D_004520A8 __asm__("_t9CSlotPool2i20i8192$s_pool"),D_004520D0 __asm__("_t9CSlotPool2i24i2048$s_pool"),D_004520F8 __asm__("_t9CSlotPool2i40i512$s_pool"),D_00452120 __asm__("_t9CSlotPool2i28i2048$s_pool"),D_00452148 __asm__("_t9CSlotPool2i12i200$s_pool"),D_00452170 __asm__("_t9CSlotPool2i20i256$s_pool"); }
// Existing constant records are twelve bytes each; the linker separates them by sixteen.
extern "C" { extern const STRINGGEN D_002A8B90; }
extern "C" { extern const STRINGGEN D_002A8BA0; }
extern "C" { extern const STRINGGEN D_002A8BB0; }
extern "C" { extern const STRINGGEN D_002A8BC0; }
extern "C" { extern const STRINGGEN D_002A8BD0; }
extern "C" { extern const STRINGGEN D_002A8BE0; }
extern "C" { extern const STRINGGEN D_002A8BF0; }
extern "C" { extern const STRINGGEN D_002A8C00; }
extern "C" { extern const STRINGGEN D_002A8C10; }
extern "C" { extern const STRINGGEN D_002A8C20; }
extern "C" { extern const STRINGGEN D_002A8C30; }
extern "C" { extern const STRINGGEN D_002A8C40; }
extern "C" { extern const STRINGGEN D_002A8C50; }
extern "C" { extern const STRINGGEN D_002A8C60; }
extern "C" { extern const STRINGGEN D_002A8C70; }
extern "C" { extern const STRINGGEN D_002A8C80; }
extern "C" { extern const STRINGGEN D_002A8C90; }
extern "C" { extern const STRINGGEN D_002A8CA0; }
extern "C" { extern const STRINGGEN D_002A8CB0; }
extern "C" { extern const STRINGGEN D_002A8CC0; }
extern "C" { extern const STRINGGEN D_002A8CD0; }
extern "C" { extern const STRINGGEN D_002A8CE0; }
extern "C" { extern const STRINGGEN D_002A8CF0; }
extern "C" { extern const STRINGGEN D_002A8D00; }
extern "C" { extern const STRINGGEN D_002A8D10; }
extern "C" { extern const STRINGGEN D_002A8D20; }
extern "C" { extern const STRINGGEN D_002A8D30; }
extern "C" { extern const STRINGGEN D_002A8D40; }
extern "C" { extern const STRINGGEN D_002A8D50; }
extern "C" { extern const STRINGGEN D_002A8D60; }
extern "C" { extern const STRINGGEN D_002A8D70; }
extern "C" { extern const STRINGGEN D_002A8D80; }
extern "C" { extern const STRINGGEN D_002A8D90; }
extern "C" { extern const STRINGGEN D_002A8DA0; }
extern "C" { extern const STRINGGEN D_002A8DB0; }
extern "C" { extern const STRINGGEN D_002A8DC0; }
extern "C" { extern const STRINGGEN D_002A8DD0; }
extern "C" { extern const STRINGGEN D_002A8DE0; }
extern "C" { extern const STRINGGEN D_002A8DF0; }
extern "C" { extern const STRINGGEN D_002A8E00; }
extern "C" { extern const STRINGGEN D_002A8E10; }
extern "C" { extern const STRINGGEN D_002A8E20; }
extern "C" { extern const STRINGGEN D_002A8E30; }
extern "C" { extern const STRINGGEN D_002A8E40; }
extern "C" { extern const STRINGGEN D_002A8E50; }
extern "C" { extern const STRINGGEN D_002A8E60; }
extern "C" { extern const STRINGGEN D_002A8E70; }
extern "C" { extern const STRINGGEN D_002A8E80; }
extern "C" { extern const STRINGGEN D_002A8E90; }
extern "C" { extern const STRINGGEN D_002A8EA0; }
extern "C" { extern const STRINGGEN D_002A8EB0; }
extern "C" { extern const STRINGGEN D_002A8EC0; }
extern "C" { extern const STRINGGEN D_002A8ED0; }
extern "C" { extern const STRINGGEN D_002A8EE0; }
extern "C" { extern const STRINGGEN D_002A8EF0; }
extern "C" { extern const STRINGGEN D_002A8F00; }
extern "C" { extern const STRINGGEN D_002A8F10; }
extern "C" { extern const STRINGGEN D_002A8F20; }
extern "C" { extern const STRINGGEN D_002A8F30; }
extern "C" { extern const STRINGGEN D_002A8F40; }
extern "C" { extern const STRINGGEN D_002A8F50; }
extern "C" { extern const STRINGGEN D_002A8F60; }
extern "C" { extern const STRINGGEN D_002A8F70; }
extern "C" { extern const STRINGGEN D_002A8F80; }
extern "C" { extern const STRINGGEN D_002A8F90; }
extern "C" { extern const STRINGGEN D_002A8FA0; }
extern "C" { extern const STRINGGEN D_002A8FB0; }
extern "C" { extern const STRINGGEN D_002A8FC0; }
extern "C" { extern const STRINGGEN D_002A8FD0; }
extern "C" { extern const STRINGGEN D_002A8FE0; }
extern "C" { extern const STRINGGEN D_002A8FF0; }
extern "C" { extern const STRINGGEN D_002A9000; }
extern "C" { extern const STRINGGEN D_002A9010; }
extern "C" { extern const STRINGGEN D_002A9020; }
extern "C" { extern const STRINGGEN D_002A9030; }
extern "C" { extern const STRINGGEN D_002A9040; }
extern "C" { extern const STRINGGEN D_002A9050; }
extern "C" { extern const STRINGGEN D_002A9060; }
extern "C" { extern const STRINGGEN D_002A9070; }
extern "C" { extern const STRINGGEN D_002A9080; }
extern "C" { extern const STRINGGEN D_002A9090; }
extern "C" { extern const STRINGGEN D_002A90A0; }
extern "C" { extern const STRINGGEN D_002A90B0; }
extern "C" { extern const STRINGGEN D_002A90C0; }
extern "C" { extern const STRINGGEN D_002A90D0; }
extern "C" { extern const STRINGGEN D_002A90E0; }
extern "C" { extern const STRINGGEN D_002A90F0; }
extern "C" { extern const STRINGGEN D_002A9100; }
extern "C" { extern const STRINGGEN D_002A9110; }
extern "C" { extern const STRINGGEN D_002A9120; }
extern "C" { extern const STRINGGEN D_002A9130; }
extern "C" { extern const STRINGGEN D_002A9140; }
extern "C" { extern const STRINGGEN D_002A9150; }
extern "C" { extern const STRINGGEN D_002A9160; }
extern "C" { extern const STRINGGEN D_002A9170; }
extern "C" { extern const STRINGGEN D_002A9180; }
extern "C" { extern const STRINGGEN D_002A9190; }
extern "C" { extern const STRINGGEN D_002A91A0; }
extern "C" { extern const STRINGGEN D_002A91B0; }
extern "C" { extern const STRINGGEN D_002A91C0; }
extern "C" { extern const STRINGGEN D_002A91D0; }
extern "C" { extern const STRINGGEN D_002A91E0; }
extern "C" { extern const STRINGGEN D_002A91F0; }
extern "C" { extern const STRINGGEN D_002A9200; }
extern "C" { extern const STRINGGEN D_002A9210; }
extern "C" { extern const STRINGGEN D_002A9220; }
extern "C" { extern const STRINGGEN D_002A9230; }
/** @sly2-annot addr=0x21AA60
 * Purpose: compiler-generated __static_initialization_and_destruction_0 of TU hud
 * Info: proposed name StaticInit_hud (T1); TU hud
 * Notes: real local symbol name is __static_initialization_and_destruction_0 (not unique; renamed per TU)
 * Confidence: high [T1 0.95] - evidence: static analysis; sources: tu_map(structural)+r7c_hud
 * See: docs/engine/hud-ui.md#1-widget-hud-objects-hud-tu-0x210eb0-0x21bb88-file-name-unknown
 * @end-sly2-annot */
// The TU's static initializer (generated by g++): the HUD linear heap, a level-heap object and the 108-entry text table.
extern "C" void StaticInit_hud(int initialize,int priority){
 if(priority==0xFFFF&&initialize){
  __11CLinearHeapiPUc(&D_003E1C70,0x60000,D_003E21A0);
  D_003E1C80.vtable=D_002CA578;D_003E1C80.field4=-1;
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
/** @sly2-annot addr=0x21BB68
 * Purpose: compiler-generated global constructor stub (calls static init with (1,0xFFFF))
 * Info: proposed name _GLOBAL_$I$hud (T1); TU hud
 * Confidence: high [T1 0.95] - evidence: static analysis; sources: tu_map(structural)+r7c_hud
 * @end-sly2-annot */
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
void p_func_00211420() __asm__("BuildOperatedObjectText");
void p_func_002155C0() __asm__("BuildSpritePictureTexPacket");
extern "C" char d_D_002C3EC0[] __asm__("D_002C3EC0");
extern "C" char d_D_003DD490[] __asm__("D_003DD490");
extern "C" {
extern const unsigned int D_002A8B90[4] = {0x2CF, (unsigned int)p_BuildCoinCountText, (unsigned int)(d_D_003DD490 + 0x3B20), 0};
extern const unsigned int D_002A8BA0[4] = {0x2D0, (unsigned int)p_BuildClueCountText, 0, 0};
extern const unsigned int D_002A8BB0[4] = {0x2D1, (unsigned int)p_BuildTimerText, 0, 0};
extern const unsigned int D_002A8BC0[4] = {0x2D2, (unsigned int)p_func_00211298, 0, 0};
extern const unsigned int D_002A8BD0[4] = {0x2D3, (unsigned int)p_func_00211298, 1, 0};
extern const unsigned int D_002A8BE0[4] = {0x2D4, (unsigned int)p_func_00211298, 2, 0};
extern const unsigned int D_002A8BF0[4] = {0x2D5, (unsigned int)p_func_00211298, 3, 0};
extern const unsigned int D_002A8C00[4] = {0x2D6, (unsigned int)p_func_00211298, 4, 0};
extern const unsigned int D_002A8C10[4] = {0x2D7, (unsigned int)p_func_00211298, 5, 0};
extern const unsigned int D_002A8C20[4] = {0x2D8, (unsigned int)p_func_00211298, 6, 0};
extern const unsigned int D_002A8C30[4] = {0x2D9, (unsigned int)p_func_00211298, 7, 0};
extern const unsigned int D_002A8C40[4] = {0x2DA, (unsigned int)p_func_00211298, 8, 0};
extern const unsigned int D_002A8C50[4] = {0x2DB, (unsigned int)p_func_00211298, 9, 0};
extern const unsigned int D_002A8C60[4] = {0x2DC, (unsigned int)p_func_00210FE0, 0, 0};
extern const unsigned int D_002A8C70[4] = {0x2DD, (unsigned int)p_BuildReconSelectionText, 0, 0};
extern const unsigned int D_002A8C80[4] = {0x2DE, (unsigned int)p_BuildReconSelectionText, 1, 0};
extern const unsigned int D_002A8C90[4] = {0x2DF, (unsigned int)p_BuildReconTargetText, 0, 0};
extern const unsigned int D_002A8CA0[4] = {0x2E0, (unsigned int)p_BuildReconTargetText, 1, 0};
extern const unsigned int D_002A8CB0[4] = {0x2E1, (unsigned int)p_BuildReconTargetText, 2, 0};
extern const unsigned int D_002A8CC0[4] = {0x2E2, (unsigned int)p_BuildReconTargetText, 3, 0};
extern const unsigned int D_002A8CD0[4] = {0x2E3, (unsigned int)p_BuildReconTargetText, 4, 0};
extern const unsigned int D_002A8CE0[4] = {0x2E4, (unsigned int)p_BuildReconTargetText, 5, 0};
extern const unsigned int D_002A8CF0[4] = {0x2E5, (unsigned int)p_BuildWidgetArgText, 0, 0};
extern const unsigned int D_002A8D00[4] = {0x2E6, (unsigned int)p_BuildWidgetArgText, 1, 0};
extern const unsigned int D_002A8D10[4] = {0x2E7, (unsigned int)p_BuildWidgetArgText, 2, 0};
extern const unsigned int D_002A8D20[4] = {0x2E8, (unsigned int)p_BuildWidgetArgText, 3, 0};
extern const unsigned int D_002A8D30[4] = {0x2E9, (unsigned int)p_BuildWidgetArgText, 4, 0};
extern const unsigned int D_002A8D40[4] = {0x2EA, (unsigned int)p_BuildWidgetArgText, 5, 0};
extern const unsigned int D_002A8D50[4] = {0x2EB, (unsigned int)p_BuildWidgetArgText, 6, 0};
extern const unsigned int D_002A8D60[4] = {0x2EC, (unsigned int)p_BuildWidgetArgText, 7, 0};
extern const unsigned int D_002A8D70[4] = {0x2ED, (unsigned int)p_BuildWidgetArgText, 8, 0};
extern const unsigned int D_002A8D80[4] = {0x2EE, (unsigned int)p_BuildWidgetArgText, 9, 0};
extern const unsigned int D_002A8D90[4] = {0x2FF, (unsigned int)p_BuildContextText, (unsigned int)d_D_002C3EC0, 0};
extern const unsigned int D_002A8DA0[4] = {0x300, (unsigned int)p_BuildContextText, (unsigned int)(d_D_002C3EC0 + 0x30), 0};
extern const unsigned int D_002A8DB0[4] = {0x301, (unsigned int)p_BuildContextText, (unsigned int)(d_D_002C3EC0 + 0x60), 0};
extern const unsigned int D_002A8DC0[4] = {0x302, (unsigned int)p_BuildContextText, (unsigned int)(d_D_002C3EC0 + 0x90), 0};
extern const unsigned int D_002A8DD0[4] = {0x303, (unsigned int)p_BuildContextText, (unsigned int)(d_D_002C3EC0 + 0xC0), 0};
extern const unsigned int D_002A8DE0[4] = {0x304, (unsigned int)p_BuildContextText, (unsigned int)(d_D_002C3EC0 + 0xF0), 0};
extern const unsigned int D_002A8DF0[4] = {0x305, (unsigned int)p_BuildContextText, (unsigned int)(d_D_002C3EC0 + 0x120), 0};
extern const unsigned int D_002A8E00[4] = {0x306, (unsigned int)p_BuildContextText, (unsigned int)(d_D_002C3EC0 + 0x150), 0};
extern const unsigned int D_002A8E10[4] = {0x307, (unsigned int)p_BuildContextText, (unsigned int)(d_D_002C3EC0 + 0x180), 0};
extern const unsigned int D_002A8E20[4] = {0x308, (unsigned int)p_BuildContextText, (unsigned int)(d_D_002C3EC0 + 0x1B0), 0};
extern const unsigned int D_002A8E30[4] = {0x2EF, (unsigned int)p_func_00211390, 0, 0};
extern const unsigned int D_002A8E40[4] = {0x2F0, (unsigned int)p_func_002113B8, 0, 0};
extern const unsigned int D_002A8E50[4] = {0x37B, (unsigned int)p_BuildGadgetMenuInfoText, 0, 0};
extern const unsigned int D_002A8E60[4] = {0x37A, (unsigned int)p_BuildGadgetMenuInfoText, 0, 0};
extern const unsigned int D_002A8E70[4] = {0x43C, (unsigned int)p_BuildThiefnetPowerupNameText, 0, 0};
extern const unsigned int D_002A8E80[4] = {0x43D, (unsigned int)p_BuildThiefnetPowerupNameText, 1, 0};
extern const unsigned int D_002A8E90[4] = {0x43E, (unsigned int)p_BuildThiefnetPowerupNameText, 2, 0};
extern const unsigned int D_002A8EA0[4] = {0x43F, (unsigned int)p_BuildThiefnetPowerupNameText, 3, 0};
extern const unsigned int D_002A8EB0[4] = {0x440, (unsigned int)p_BuildThiefnetPowerupNameText, 4, 0};
extern const unsigned int D_002A8EC0[4] = {0x441, (unsigned int)p_BuildThiefnetPowerupNameText, 5, 0};
extern const unsigned int D_002A8ED0[4] = {0x442, (unsigned int)p_func_001DC820, 0, 0};
extern const unsigned int D_002A8EE0[4] = {0x443, (unsigned int)p_func_001DC820, 1, 0};
extern const unsigned int D_002A8EF0[4] = {0x444, (unsigned int)p_func_001DC820, 2, 0};
extern const unsigned int D_002A8F00[4] = {0x445, (unsigned int)p_func_001DC820, 3, 0};
extern const unsigned int D_002A8F10[4] = {0x446, (unsigned int)p_func_001DC820, 4, 0};
extern const unsigned int D_002A8F20[4] = {0x447, (unsigned int)p_func_001DC820, 5, 0};
extern const unsigned int D_002A8F30[4] = {0x436, (unsigned int)p_func_001DCA30, 0, 0};
extern const unsigned int D_002A8F40[4] = {0x435, (unsigned int)p_BuildThiefnetInfoText, 0, 0};
extern const unsigned int D_002A8F50[4] = {0x448, (unsigned int)p_BuildThiefnetLootNameText, 0, 0};
extern const unsigned int D_002A8F60[4] = {0x449, (unsigned int)p_BuildThiefnetLootNameText, 1, 0};
extern const unsigned int D_002A8F70[4] = {0x44A, (unsigned int)p_BuildThiefnetLootNameText, 2, 0};
extern const unsigned int D_002A8F80[4] = {0x44B, (unsigned int)p_BuildThiefnetLootNameText, 3, 0};
extern const unsigned int D_002A8F90[4] = {0x44C, (unsigned int)p_BuildThiefnetLootNameText, 4, 0};
extern const unsigned int D_002A8FA0[4] = {0x44D, (unsigned int)p_BuildThiefnetLootNameText, 5, 0};
extern const unsigned int D_002A8FB0[4] = {0x44E, (unsigned int)p_func_001DC9C0, 0, 0};
extern const unsigned int D_002A8FC0[4] = {0x44F, (unsigned int)p_func_001DC9C0, 1, 0};
extern const unsigned int D_002A8FD0[4] = {0x450, (unsigned int)p_func_001DC9C0, 2, 0};
extern const unsigned int D_002A8FE0[4] = {0x451, (unsigned int)p_func_001DC9C0, 3, 0};
extern const unsigned int D_002A8FF0[4] = {0x452, (unsigned int)p_func_001DC9C0, 4, 0};
extern const unsigned int D_002A9000[4] = {0x453, (unsigned int)p_func_001DC9C0, 5, 0};
extern const unsigned int D_002A9010[4] = {0x2F1, (unsigned int)p_BuildTickerText, 0, 0};
extern const unsigned int D_002A9020[4] = {0x51B, (unsigned int)p_BuildJobListWorldText, 0, 0};
extern const unsigned int D_002A9030[4] = {0x51C, (unsigned int)p_func_001DDE58, 0, 0};
extern const unsigned int D_002A9040[4] = {0x51D, (unsigned int)p_func_001DDE10, 0, 0};
extern const unsigned int D_002A9050[4] = {0x51E, (unsigned int)p_BuildJobListRowText, 0, 0};
extern const unsigned int D_002A9060[4] = {0x51F, (unsigned int)p_BuildJobListRowText, 1, 0};
extern const unsigned int D_002A9070[4] = {0x520, (unsigned int)p_BuildJobListRowText, 2, 0};
extern const unsigned int D_002A9080[4] = {0x521, (unsigned int)p_BuildJobListRowText, 3, 0};
extern const unsigned int D_002A9090[4] = {0x522, (unsigned int)p_BuildJobListRowText, 4, 0};
extern const unsigned int D_002A90A0[4] = {0x523, (unsigned int)p_BuildJobListRowText, 5, 0};
extern const unsigned int D_002A90B0[4] = {0x524, (unsigned int)p_BuildJobListRowText, 6, 0};
extern const unsigned int D_002A90C0[4] = {0x525, (unsigned int)p_BuildJobListRowText, 7, 0};
extern const unsigned int D_002A90D0[4] = {0x526, (unsigned int)p_BuildJobListRowText, 8, 0};
extern const unsigned int D_002A90E0[4] = {0x527, (unsigned int)p_BuildJobListRowText, 9, 0};
extern const unsigned int D_002A90F0[4] = {0x528, (unsigned int)p_BuildJobListTaskRowText, 0, 0};
extern const unsigned int D_002A9100[4] = {0x529, (unsigned int)p_BuildJobListTaskRowText, 1, 0};
extern const unsigned int D_002A9110[4] = {0x52A, (unsigned int)p_BuildJobListTaskRowText, 2, 0};
extern const unsigned int D_002A9120[4] = {0x52B, (unsigned int)p_BuildJobListTaskRowText, 3, 0};
extern const unsigned int D_002A9130[4] = {0x2F2, (unsigned int)p_func_001AD7E0, 0, 0};
extern const unsigned int D_002A9140[4] = {0x2F3, (unsigned int)p_func_001AD820, 0, 0};
extern const unsigned int D_002A9150[4] = {0x5CB, (unsigned int)p_func_001AD928, 0, 0};
extern const unsigned int D_002A9160[4] = {0x5CC, (unsigned int)p_func_001AD980, 0, 0};
extern const unsigned int D_002A9170[4] = {0x5CD, (unsigned int)p_func_001ADA20, 0, 0};
extern const unsigned int D_002A9180[4] = {0x5CE, (unsigned int)p_func_001ADBB8, 0, 0};
extern const unsigned int D_002A9190[4] = {0x5CF, (unsigned int)p_func_001ADC40, 0, 0};
extern const unsigned int D_002A91A0[4] = {0x5D0, (unsigned int)p_func_001ADC40, 1, 0};
extern const unsigned int D_002A91B0[4] = {0x5D1, (unsigned int)p_func_001ADC40, 2, 0};
extern const unsigned int D_002A91C0[4] = {0x5D3, (unsigned int)p_BuildTitlePromptText, 0, 0};
extern const unsigned int D_002A91D0[4] = {0x5D7, (unsigned int)p_func_001DDF60, 0, 0};
extern const unsigned int D_002A91E0[4] = {0x5D5, (unsigned int)p_BuildTitleCompletionText, 0, 0};
extern const unsigned int D_002A91F0[4] = {0x52F, (unsigned int)p_FormatEpisodePercent, 0, 0};
extern const unsigned int D_002A9200[4] = {0x530, (unsigned int)p_FormatEpisodePlayTime, 0, 0};
extern const unsigned int D_002A9210[4] = {0x2F4, (unsigned int)p_func_00211420, 0, 0};
extern const unsigned int D_002A9220[4] = {0x5BF, (unsigned int)p_func_0015DA88, 1, 0};
extern const unsigned int D_002A9230[4] = {0x5C0, (unsigned int)p_func_0015DA88, 0, 0};
}
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
