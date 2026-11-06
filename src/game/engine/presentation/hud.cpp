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
/* A sprite group as DrawSprite..GetSpriteMainState read it: 0x18-byte items at +4, 0x18-byte entries at +8 (the
   entry's linked item index at +8), a count at +0x28. */
struct SPRITEITEM { char pad0[0x18]; };
struct SPRITEENTRY { char pad0[0x8]; short itemIndex; char padA[0x6]; int n10; int n14; };
struct SPRITEGRP
{
    int n0;
    SPRITEITEM *spriteItems;      /* +0x04 */
    SPRITEENTRY *spriteEntries;    /* +0x08 */
    char padC[0x1C];
    int n28;                /* +0x28 */
};

extern SW D_003DD490;           /* g_sw */
extern CLOCK D_002DDED0;        /* g_clock */
extern char D_004435F0[];       /* speaker label (HYPOTHESIS, as letterbox.cpp) */
#undef D_003DA0D8
#define D_003DA0D8 (*(int *)((char *)base_D_003DA0C0 + 0x18))   /* screen's object+0x18 */   /* ticker text id */
extern "C" char D_000000C0[];         /* VU1 microprogram entry at 0xC0 */
char *PchzFindSwLangstr(SW *psw, int id);
int FIsBasicDerivedFrom(BASIC *pbasic, CID cid);
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
void DrawSpriteItems(SPRITEITEM *pitem, int i, int n);
void StopSpriteSlot(SPRITEGRP *spriteGroup, int i);
/* DrawSprite returns what DrawSpriteSlot leaves in v0 (its own body is void) */
int func_002185F8I(SPRITEGRP *spriteGroup, int i, int n) __asm__("DrawSpriteSlot");
void StaticInit_hud(int fInitialize, int nPriority);
}

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
struct X { char pad[0xF20]; int xF20; char padf24[0x18]; float xF3C; };
extern char D_003DD490[];
extern char D_002A8AB8[], D_002A8AC0[], D_002A8AC8[], D_002A8AD0[], D_002A8AD8[];
struct X *GetHeroPo(void *);
int FIsBasicDerivedFrom__FP5BASIC3CID(struct X *, int);
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
void BuildBinocularReadoutText(char *pch)
{
    struct X *x = GetHeroPo(D_003DD490);
    float g;
    if (!x || !FIsBasicDerivedFrom__FP5BASIC3CID(x, 0x34))
        return;
    switch (x->xF20) {
    case 0:
        g = g_cm.cplook.rZoomMax * x->xF3C;
        break;
    case 1:
        g = g_cm.cpzoom.rZoomMax * x->xF3C;
        break;
    }
    if (FIsBasicDerivedFrom__FP5BASIC3CID(x, 0x35)) {
        sprintf(pch, D_002A8AB8, (int)g);
    } else if (FIsBasicDerivedFrom__FP5BASIC3CID(x, 0x37)) {
        sprintf(pch, D_002A8AC0, fptodp(g));
    } else if (FIsBasicDerivedFrom__FP5BASIC3CID(x, 0x36)) {
        int n = (int)g;
        if (n < 4)
            sprintf(pch, D_002A8AC8, n);
        else if (n < 8)
            sprintf(pch, D_002A8AD0, n);
        else
            sprintf(pch, D_002A8AD8, n);
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
extern "C" void BuildReconSelectionText(char *pchz, int i)
{
    JT *pjt;
    int *pnRecon;
    char *pchzRecon;
    pjt = (JT *)GetHeroPo(&D_003DD490);
    if ((pjt != 0) && (FIsBasicDerivedFrom((BASIC *)pjt, (CID)0x34) != 0)) {
        pnRecon = pjt->jtmode.hud.pnUnk14;
        pchzRecon = 0;
        if (pnRecon != 0) {
            if (i != 0) {
                if (i == 1) {
                    pchzRecon = GetReconPendingText(pnRecon);
                }
            } else {
                pchzRecon = GetReconSelectedText(pnRecon);
            }
        }
        if (pchzRecon != 0) {
            strcpy(pchz, pchzRecon);
        }
    }
}

// 0x2111F0 BuildReconTargetText
namespace M_BuildReconTargetText { extern "C" {
struct G;
extern struct G D_003DD490;
struct Z { char pad[0xF8]; int xF8; };
struct Y { char pad[0x74]; int c; struct Z *a[1]; };
struct X { char pad[0xF38]; struct Y *py; };
struct X *GetHeroPo(struct G *g);
int FIsBasicDerivedFrom__FP5BASIC3CID(void *p, int cid);
char *PchzFindSwLangstr__FP2SWi(struct G *g, int id);
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
    struct G *g = &D_003DD490;
    struct X *p = GetHeroPo(g);
    if (p && FIsBasicDerivedFrom__FP5BASIC3CID(p, 0x34)) {
        struct Y *y = p->py;
        if (y && i < y->c)
            strcpy(dst, PchzFindSwLangstr__FP2SWi(g, y->a[i]->xF8));
    }
}
} } // namespace M_BuildReconTargetText

// 0x211298 BuildMenuCarouselText
namespace M_func_00211298 { extern "C" {
struct RESOURCE { int id; const char *text; };
struct G { char gap[0x4640]; int count; struct RESOURCE *resources; };
extern struct G D_003DD490;
struct F { int id; char pad[0x1C]; };
struct E { int c; struct F *af; char pad[0xC]; int x14; char pad2[4]; };
#undef D_003D9EDC
extern struct E D_003D9EDC[] __asm__("D_003D9ED0+0xC");   /* screen's object+0xC, a separate symbol to g++: the base+offset form changes BuildMenuCarouselText (CSE of the base) */
#undef D_003D9EF4
extern int D_003D9EF4 __asm__("D_003D9ED0+0x24");   /* screen's object+0x24; see D_003D9EDC */
const char *PchzFindSwLangstr__FP2SWi(struct G *g, int id);
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
    struct E *e = &D_003D9EDC[D_003D9EF4];
    if (e->c) {
        int n = e->c;
        int j = k + (long)(e->x14 - 3);
        while (j < 0)
            j += n;
        (void)strcpy(dst, PchzFindSwLangstr__FP2SWi(&D_003DD490, e->af[j % n].id));
    }
}
} } // namespace M_func_00211298
#undef D_003D9EDC
#undef D_003D9EF4

// 0x211348 BuildWidgetArgText
namespace M_BuildWidgetArgText { extern "C" {
struct T { char pad[0xCC]; int a[1]; };
extern struct G D_003DD490;
int PchzFindSwLangstr__FP2SWi(struct G *g, int x);
void strcpy(void *p, int v);
/** @sly2-annot addr=0x211348
 * Purpose: STRINGGEN ids 0x2E5-0x2EE: langstr(owner WIDGET->aid[i] at +0xCC)
 * Info: proposed name BuildWidgetArgText (T1); TU hud
 * Confidence: mid (draft) [T1 0.70] - evidence: static analysis; sources: r7c_hud
 * See: docs/engine/hud-ui.md#5-key-functions, docs/reference/functions/hud-ui.md#hud-0x210eb0-0x21bb88
 * @end-sly2-annot */
// HUD texts 0x2E5-0x2EE: the language string of the owning widget's argument i.
void BuildWidgetArgText(void *p, int i, struct T *t)
{
    strcpy(p, PchzFindSwLangstr__FP2SWi(&D_003DD490, t->a[i]));
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
    strcpy(pchz, PchzFindSwLangstr(&D_003DD490, D_003DA0D8));
}

// 0x211420 BuildOperatedObjectText
namespace M_func_00211420 { extern "C" {
struct E { int x0; int x4; int x8; };
extern struct E D_002C3288[];
struct Z { int x0; int x4; };
struct Y { char pad[0x224]; int x224; char pad228[0x1C]; struct Z *x244; };
struct X { char pad[0x4C8]; int x4C8; char pad4cc[0x9A0 - 0x4CC]; struct Y *x9A0; char pad9a4[0x1318 - 0x9A4]; int x1318; };
extern char D_003DD490[];
struct X *GetHeroPo(void *);
int FIsBasicDerivedFrom__FP5BASIC3CID(void *, int);
char *PchzFindSwLangstr__FP2SWi(void *, int);
void strcpy(char *, char *);
/** @sly2-annot addr=0x211420
 * Purpose: STRINGGEN id 0x2F4: name of the object the player operates in mode x4C8==0xC (cid 0x5E table
 *   D_002C3288 / cid 0x5C field / player x1318)
 * Info: proposed name BuildOperatedObjectText (T2); TU hud
 * Confidence: low (draft) [T2 0.30] - evidence: static analysis; sources: r7c_hud
 * See: docs/reference/functions/hud-ui.md#hud-0x210eb0-0x21bb88
 * @end-sly2-annot */
// HUD text 0x2F4: the name of the object the player is operating (in the "operate" mode). (inferred)
void BuildOperatedObjectText(char *pch)
{
    int id = -1;
    struct X *x = GetHeroPo(D_003DD490);
    if (x && FIsBasicDerivedFrom__FP5BASIC3CID(x, 0x34)) {
        struct Y *y = x->x9A0;
        if (y) {
            int k = x->x4C8;
            if (k == 0xC) {
                if (FIsBasicDerivedFrom__FP5BASIC3CID(y, 0x5E))
                    id = D_002C3288[y->x244->x4].x4;
                else if (FIsBasicDerivedFrom__FP5BASIC3CID(y, 0x5C))
                    id = y->x224;
                else
                    id = x->x1318;
            }
        }
    }
    if (id != -1)
        strcpy(pch, PchzFindSwLangstr__FP2SWi(D_003DD490, id));
    else
        *pch = 0;
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
struct S { char pad[0x7C]; int x7c; char pad2[0x90]; int x110; };
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
void ShowModalBoxWidget(struct S *p, int k)
{
    if (p->x7c == -1) {
        ShowWidget(p, k);
        IncrementSwHandsOff(&D_003DD490, 0);
        p->x110 = 1;
        PlayWidgetAnim(p, 0x33A);
    }
}
} } // namespace M_func_00211CA8

// 0x211D08 InitModalBoxWidget
namespace M_func_00211D08 { extern "C" {
struct S { char pad[0xB0]; int f; };
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
void WIDGET127::Init() { using namespace M_func_00211D08; M_func_00211D08::S *p = (M_func_00211D08::S *)this;
    InitLo(p);
    p->f = 1;
}

// 0x211D38 UpdateModalBoxWidget
namespace M_func_00211D38 { extern "C" {
// Actor update: commit completed state after every noninitial guard path.
struct VT { char pad[0x190]; short d; short pd; void (*pf)(void *); };
struct S { char pad[0xC]; struct VT *pvt; char pad10[0x48]; int state; float stateTime; char pad60[0xB0]; int unknown110; int keepsGamePaused; };
extern struct G { char pad[0x3B0C]; void *unknown3B0C; } D_003DD490;
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
void UpdateModalBoxWidget(struct S *p)
{
    int r = IsWidgetIdle(p);
    if (r)
        return;
    UpdateWidget(p);
    if (p->state == 2 && 0.5f < g_clock.tReal - p->stateTime && !FScreenShown(D_002E6550)
        && !FScreenShown(D_003DA4F0) && HandleJoyButtons(D_002DFC00, 0xF0)) {
        DecrementSwHandsOff(&D_003DD490, 0);
        if (p->keepsGamePaused == 0 && D_003DD490.unknown3B0C) {
            D_002DD318++;
            RestartJob(D_003DD490.unknown3B0C);
        }
        p->pvt->pf((char *)p + p->pvt->d);
    }
    p->unknown110 = 1;
    return;
}
} } // namespace M_func_00211D38

// 0x211E40 InitWidget128
namespace M_func_00211E40 { extern "C" {
struct S { char pad[0xB0]; int f; };
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
void WIDGET128::Init() { using namespace M_func_00211E40; M_func_00211E40::S *p = (M_func_00211E40::S *)this;
    InitLo(p);
    p->f = 1;
}

// 0x211E70 AddItemCounterOidA
namespace M_func_00211E70 { extern "C" {
struct E { int a; int b; };
struct S { char pad[0x110]; unsigned int c; struct E ae[16]; };
/** @sly2-annot addr=0x211E70
 * Purpose: CID 0x81: append oid to list A (+0x110, max 16)
 * Info: proposed name AddWidget129OidA (T2); TU hud
 * Notes: names_v2 v1 proposed AddVolbtnPushObject__FP6VOLBTN3OID (0.002) | v2: demoted to T2 (placeholder
 *   token in name: 129)
 * Confidence: low (draft) [T2 0.50] - evidence: static naming pass; sources: r7c_hud
 * See: docs/reference/functions/hud-ui.md#hud-0x210eb0-0x21bb88
 * @end-sly2-annot */
// Adds an object to the item counter widget's first list (at most 16). (inferred)
void AddItemCounterOidA(struct S *p, int v)
{
    unsigned int c = p->c;
    if (c < 16) {
        p->ae[c].a = v;
        p->c = c + 1;
    }
}
} } // namespace M_func_00211E70

// 0x211E98 AddItemCounterOidB
namespace M_func_00211E98 { extern "C" {
struct E { int a; int b; };
struct S { char pad[0x194]; unsigned int c; struct E ae[16]; };
/** @sly2-annot addr=0x211E98
 * Purpose: CID 0x81: append oid to list B (+0x194, max 16)
 * Info: proposed name AddWidget129OidB (T2); TU hud
 * Notes: names_v2 v1 proposed SetButtonRsmg__FP6BUTTONi3OIDN22 (0.000) | v2: demoted to T2 (placeholder token
 *   in name: 129)
 * Confidence: low (draft) [T2 0.50] - evidence: static naming pass; sources: r7c_hud
 * See: docs/reference/functions/hud-ui.md#hud-0x210eb0-0x21bb88
 * @end-sly2-annot */
// Adds an object to the item counter widget's second list (at most 16). (inferred)
void AddItemCounterOidB(struct S *p, int v)
{
    unsigned int c = p->c;
    if (c < 16) {
        p->ae[c].a = v;
        p->c = c + 1;
    }
}
} } // namespace M_func_00211E98

// 0x211EC0 PostLoadItemCounterWidget
namespace M_func_00211EC0 { extern "C" {
struct G;
extern struct G D_003DD490;
struct B { int id; void *p; };
struct S { char pad[0x110]; int c110; struct B a114[0x10]; int c194; struct B a198[1]; };
void PostWidgetLoad(struct S *p);
void *PloFindSwObject__FP2SWi3OIDP2LO(struct G *g, int k, int id, int z);
/** @sly2-annot addr=0x211EC0
 * Purpose: CID 0x81 PostLoad: PostWidgetLoad then resolve both oid lists to LOs (PloFindSwObject grffso 5)
 * Info: proposed name PostWidget129Load (T2); class WIDGET129 slot15; TU hud
 * Notes: v2: demoted to T2 (placeholder token in name: 129)
 * Confidence: low (draft) [T2 0.65] - evidence: static analysis; sources: r7c_hud
 * See: docs/reference/functions/hud-ui.md#hud-0x210eb0-0x21bb88
 * @end-sly2-annot */
} } // namespace M_func_00211EC0
// Item counter widget post-load (class 0x81): base widget post-load, then resolves both OID lists to objects.
void WIDGET129::VUnk14() { using namespace M_func_00211EC0; using M_func_00211EC0::D_003DD490; M_func_00211EC0::S *p = (M_func_00211EC0::S *)this;
    int i;
    PostWidgetLoad(p);
    for (i = 0; i < p->c194; i++) {
        void *q = PloFindSwObject__FP2SWi3OIDP2LO(&D_003DD490, 5, p->a198[i].id, 0);
        if (q)
            p->a198[i].p = q;
    }
    for (i = 0; i < p->c110; i++) {
        void *q = PloFindSwObject__FP2SWi3OIDP2LO(&D_003DD490, 5, p->a114[i].id, 0);
        if (q)
            p->a114[i].p = q;
    }
}

// 0x211F90 UpdateItemCounterWidget
namespace M_func_00211F90 { extern "C" {
struct VT {
    char pad[0x188]; short d49; short pd49; void (*f49)(void *,int);
    short d50; short pd50; void (*f50)(void *);
};
struct O {char pad[0x54];int state;};
struct E {struct O *o;int x4;};
struct S {
    char pad[0xC];struct VT *vt;char pad10[0x100];
    int countA;int x114;struct E a[15];int x190;int countB;int x198;struct E b[0];
};
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
void UpdateItemCounterWidget(struct S *p)
{
    int i;
    int ready=0;
    int readyB=0;
    int loading=0;
    UpdateWidget(p);
    for(i=0;i<p->countB;i++) {
        struct O *o=p->b[i].o;
        if(o) {
            if(o->state==1)loading++;
            else if(IsTaskComplete(o)) {
                ready++;
                readyB++;
            }
        }
    }
    for(i=0;i<p->countA;i++) {
        struct O *o=p->a[i].o;
        if(o) {
            if(o->state==1)loading++;
            else if(IsTaskComplete(o)) ready++;
        }
    }
    if(FWidgetAnimPlaying(p,0x314)) {
        sprintf(D_002C3EC0,D_002A8AA0,readyB);
        sprintf(D_002C3EC0+0x30,D_002A8AA0,p->countB);
        sprintf(D_002C3EC0+0x60,D_002A8AA0,p->countB-readyB);
    }
    if(ready==p->countB+p->countA || !loading) {
        p->vt->f50((char*)p+p->vt->d50);
    } else if(loading>0) {
        p->vt->f49((char*)p+p->vt->d49,-1);
    }
}
} } // namespace M_func_00211F90

// 0x212158 WIDGET_ctor
namespace M_WIDGET_ctor { extern "C" {
struct S { int x0; int x4; char pad[4]; void *vt; char pad2[0x6C]; int x7c; char pad3[0x4C]; int a[10]; char pad4[8]; char xfc[4]; };
extern char D_002DAF30[] __asm__("_vt$6WIDGET");
void InitSl(void *p, int n);
/** @sly2-annot addr=0x212158
 * Purpose: WIDGET ctor: vtable 0x2DAF30, idAnimMain(+0x7C)=-1, 10 arg ids -1, InitSl(+0xFC, 0x68)
 * Info: proposed name WIDGET_ctor (T1); class WIDGET; TU hud
 * Confidence: high [T1 0.85] - evidence: static analysis; sources: r7c_hud+verdicts_followups
 * See: docs/engine/hud-ui.md#5-key-functions, docs/reference/functions/hud-ui.md#hud-0x210eb0-0x21bb88
 * @end-sly2-annot */
struct S *WIDGET_ctor(struct S *p)
{
    unsigned int i;
    p->vt = D_002DAF30;
    p->x7c = -1;
    p->x0 = 0;
    p->x4 = 0;
    for (i = 0; i < 10; i++)
        p->a[i] = -1;
    InitSl(p->xfc, 0x68);
    return p;
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
    int f;
    f = 0;
    if (widget->state < 2) {
        f = widget->request < 2;
    }
    return f;
}

// 0x2121E8 FWidgetAnimPlaying
namespace M_FWidgetAnimPlaying { extern "C" {
struct E { char pad[0x14]; int id; };
struct A { unsigned short c; short pad; struct E *ae; char pad8[0x28]; };
struct S { char pad[0x7C]; int x7C; unsigned short c80; short pad82; struct A *aa; };
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
int FWidgetAnimPlaying(struct S *p, int k)
{
    int j, l;
    if (k == -1)
        return 0;
    if (p->x7C == -1)
        return 0;
    if (k == 0x314)
        return GetSpriteMainState(p->aa) == 0;
    for (j = 0; j < p->c80; j++) {
        struct A *a = &p->aa[j];
        for (l = 0; l < a->c; l++) {
            if (k == a->ae[l].id)
                return FSpriteAnimActive(a);
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
    char pad80[0x10]; float duration; int width; int height; char pad9c[0x20]; int hasFont; struct O *deferredLoad; void *spriteOps;
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
void LoadWidgetFromBrx(struct S *p, void *pbis)
{
    int f;
    p->loopAnimId = 0x310;
    p->showAnimId = 0x30F;
    p->hideAnimId = 0x311;
    LoadLoFromBrx(p, pbis);
    f = S8Read__18CBinaryInputStream(pbis);
    p->duration = F32Read__18CBinaryInputStream(pbis);
    p->width = U32Read__18CBinaryInputStream(pbis);
    p->height = U32Read__18CBinaryInputStream(pbis);
    p->hasFont = U32Read__18CBinaryInputStream(pbis);
    if (f) {
        AREQ req;
        p->state = 0;
        req.cb = 0x50;
        req.cbAlign = 0x10;
        p->spriteOps = D_003E1C70;
        p->deferredLoad = AllocClearedFromWorldHeap(&req);
        p->deferredLoad->owner = p;
        CBinaryInputStream_ReadCb64(pbis, p->deferredLoad);
        AppendSlEntry((*(char (*)[])&g_sw.slWidgetDeferred), p->deferredLoad);
    } else {
        p->state = 1;
        p->spriteOps = D_003E1C80;
        LoadSpriteTable(p, pbis, p, &p->spriteCount, &p->sprites);
        LoadSpriteGroups(p, pbis, p);
    }
    p->mainAnimId = -1;
}
} } // namespace M_LoadWidgetFromBrx

// 0x2123C8 PostWidgetLoad
namespace M_PostWidgetLoad { extern "C" {
struct VT { char pad[0x188]; short d; short pd; void (*pf)(void *, int); };
struct S {
    char pad[0xC]; struct VT *pvt; char pad10[0x30]; unsigned int x40; char pad44[0x29]; unsigned char x6D;
    char pad6e[0x26]; int x94; int x98; float x9C; float xA0; char pada4[4]; int xA8; char padac[0x10];
    int xBC; char padc0[8]; struct S *xC8;
};
extern struct G { char pad[0x3BE8]; void *x3BE8; } D_003DD490;
void PostLoLoad(struct S *p);
void *PloFindSwObject__FP2SWi3OIDP2LO(struct G *, int, int, struct S *);
void SetLoBusyk(struct S *p, int);
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
void PostWidgetLoad(struct S *p)
{
    unsigned int v;
    float w;
    int h;
    PostLoLoad(p);
    if (p->xBC) {
        struct G *pg = &D_003DD490;
        if (!pg->x3BE8)
            pg->x3BE8 = PloFindSwObject__FP2SWi3OIDP2LO(pg, 0x405, 0x374, p);
    }
    p->xC8 = p;
    p->xA8 = -1;
    h = (p->x94 << 9) / 640;
    w = (float)p->x98 * 0.45454547f;
    p->xA0 = 2048.0f - w * 0.5f;
    p->x9C = 2048.0f - (float)h * 0.5f;
    if (p->x6D)
        p->pvt->pf((char *)p + p->pvt->d, -1);
    SetLoBusyk(p, 1);
    v = p->x40;
    if ((v ^ 1) & 1) *(int *)v |= 0x102;
    else p->x40 = v | 0x102;
}
} } // namespace M_PostWidgetLoad

// 0x2124F0 LoadWidgetSprites
namespace M_LoadWidgetSprites {
// Load prioritized sprite files.
struct FRAME {};
typedef struct {int cb,alignment;} MSG;
struct BIS {int grf;char unknown4[0x2C];void *vt;int cbSpool;char *abSpool;int x3c,x40,x44,x48,x4c,x50,x54,x58,x5c,x60;};
typedef struct {int a,b,kind;char filename[0x104];} KEY;
typedef union {MSG msg;char magic[4];} TMP;
typedef struct S S;
typedef struct N {int a,b;char pad8[0x2C];int c;int pad38;int d;S *owner;int priority,last;struct N *next;} N;
struct S {char pad[0x50];int count;void *data;int state;char pad5c[4];int tick;char pad64[0x5C];N *file;};
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
static inline int SetKey(KEY *k,N *n) {
    if(!n)return 0;
    {int a=n->c,b=n->a,c=n->d,d=n->b;
    k->kind=2;
    k->a=(b^a)+0xCA0;
    k->b=d^c;}
    return k->b!=0;
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
extern "C" void LoadWidgetSprites(S *p) {
    WORK work;
    register BIS *bs asm("$17");asm("la %0,%a1":"=r"(bs):"p"(&work.stream));
    int priority=0;
    N *n;
    S *owner;
    int version;
    for(n=(*(N * *)&g_sw.slWidgetDeferred);n;n=n->next) {
        owner=n->owner;
        if(owner->state==0)continue;
        if(owner->state==1 && D_002BE538-owner->tick>=2)continue;
        priority=Max(priority,n->last);
    }
    for(n=(*(N * *)&g_sw.slWidgetDeferred);n;n=n->next) {
        owner=n->owner;
        if(n->priority>=priority && owner->state!=0)SetWidgetState(owner,0);
    }
    D_003E1C70.priority=priority;
    STACKSCOPE_ctor(&work.frame);
    KEY *pk=&work.key;
    MSG *msg=&work.tmp.msg;

    msg->cb=0x10000;msg->alignment=0x40;
    CStreamBis_ctor(bs,0x10000,AllocFromStack(msg),0);
    pk->kind=2;
    InitFileKey(pk);
    if(!SetKey(&work.key,p->file)||!OpenStreamFromFileKey(bs,pk))goto fail;
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
    p->file->priority=D_003E1C70.priority;
    LoadSpriteTable(p,bs,p,&p->count,&p->data);
    LoadSpriteGroups(p,bs,p);
    p->file->last=D_003E1C70.priority;
    func_00184950(bs,2);
    STACKSCOPE_ctor_1AE4A0(&work.frame,2);
}
} // namespace M_LoadWidgetSprites

// 0x212780 UnloadWidgetSprites
namespace M_UnloadWidgetSprites { extern "C" {
struct C { char pad[0x44]; int a; int b; };
struct S { char pad[0x48]; int x48; int x4c; int x50; int x54; char pad2[0x28]; short x80; short pad3; int x84; char pad4[0x38]; struct C *pc; char pad5[0x34]; int xf8; char xfc[4]; };
void ClearSl(void *p);
/** @sly2-annot addr=0x212780
 * Purpose: state 0 entry: drop sprite tables and lists
 * Info: proposed name UnloadWidgetSprites (T1); TU hud
 * Confidence: mid (draft) [T1 0.50] - evidence: static analysis; sources: r7c_hud
 * See: docs/reference/functions/hud-ui.md#hud-0x210eb0-0x21bb88
 * @end-sly2-annot */
// Drops a widget's sprite tables and lists (on entering the unloaded state).
void UnloadWidgetSprites(struct S *p)
{
    struct C *pc;
    p->xf8 = 0;
    p->x50 = 0;
    p->x54 = 0;
    p->x80 = 0;
    p->x84 = 0;
    ClearSl(p->xfc);
    pc = p->pc;
    pc->a = 0;
    pc->b = 0;
}
} } // namespace M_UnloadWidgetSprites

// 0x2127C8 AreWidgetLoadsIdle
namespace M_func_002127C8 { extern "C" {
struct Q { char pad[0x58]; int k; int pad2; int a; int b; };
struct S { char pad[0x40]; struct Q *q; char pad2[8]; struct S *next; };
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
    struct S *p;
    for (p = (*(struct S * *)&g_sw.slWidgetDeferred); p; p = p->next) {
        struct Q *q = p->q;
        int v;
        if (q->k == 0)
            continue;
        if (q->k == 1)
            v = D_002BE538 - q->a;
        else
            v = q->b;
        if (v < 2)
            return 0;
    }
    return 1;
}
} } // namespace M_func_002127C8

// 0x212828 ShowWidgetArgsFromSplice
namespace M_ShowWidgetArgsFromSplice { extern "C" {
struct CRef { int tagk; int tag; };
struct VE { short d; short pad; void (*pf)(void *, int); };
struct S { char pad[0xC]; struct VE *vt; char pad2[0xBC]; char xcc[4]; };
struct L { int a[4]; };
void SetValuesFromSpliceList(struct CRef *r, void *a, int b, int c, struct L *pl, void *d);
void Decref__4CRef(struct CRef *r);
/** @sly2-annot addr=0x212828
 * Purpose: copy Splice list into +0xCC ids (max 10) then ShowWidget(-1)
 * Info: proposed name ShowWidgetArgsFromSplice (T1); TU hud
 * Confidence: mid (draft) [T1 0.50] - evidence: static analysis; sources: r7c_hud
 * See: docs/reference/functions/hud-ui.md#hud-0x210eb0-0x21bb88
 * @end-sly2-annot */
// Shows a widget with argument ids taken from a splice list (at most 10).
void ShowWidgetArgsFromSplice(struct S *p, void *a)
{
    struct CRef r;
    struct L l;
    SetValuesFromSpliceList(&r, a, 0x10, 0xA, &l, p->xcc);
    Decref__4CRef(&r);
    p->vt[49].pf((char *)p + p->vt[49].d, -1);
}
} } // namespace M_ShowWidgetArgsFromSplice

// 0x212880 ShowWidgetArgs
namespace M_ShowWidgetArgs { extern "C" {
struct VE { short d; short pad; void (*pf)(void *, int); };
struct S { char pad[0xC]; struct VE *vt; char pad2[0xBC]; int a[10]; };
/** @sly2-annot addr=0x212880
 * Purpose: copy n ids into +0xCC (max 10) then ShowWidget(-1)
 * Info: proposed name ShowWidgetArgs (T1); TU hud
 * Confidence: mid (draft) [T1 0.55] - evidence: static analysis; sources: r7c_hud
 * See: docs/reference/functions/hud-ui.md#hud-0x210eb0-0x21bb88
 * @end-sly2-annot */
// Shows a widget with n argument ids (at most 10), e.g. the text ids it displays.
void ShowWidgetArgs(struct S *p, int n, int *src)
{
    int i;
    for (i = 0; i < n && (unsigned)i < 10; i++)
        p->a[i] = src[i];
    p->vt[49].pf((char *)p + p->vt[49].d, -1);
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
  void A(int) __asm__("SetWidgetState");
  void Bf(int) __asm__("SetWidgetRequest");
  int F(void *, int) __asm__("FSpriteHasAnim");
  void M(int) __asm__("ShowWidget");
};
// ShowWidget (widget slot 49): requests that a widget show, loading its sprites first if needed; k = the animation
// argument (-1 = default).
void S::M(int k)
{
    if (!state)
        A(1);
    Bf(2);
    loopAnimId = k;
    showAnimId = -1;
    hideAnimId = -1;
    if (k != -1) {
        A(1);
        A(2);
        return;
    }
    loopAnimId = 0x314;
    if (F(mainSprite, 0x30F))
        showAnimId = 0x30F;
    if (F(mainSprite, 0x310))
        loopAnimId = 0x310;
    if (F(mainSprite, 0x311))
        hideAnimId = 0x311;
    if (state == 1)
        v16();
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
    SetWidgetRequest(widget, 1);
    if (widget->hideAnimId == -1) {
        SetWidgetStateW(widget, 1);
    }
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
    if (widget->state == 2) {
        widget->stateTime = D_002DDED0.tReal;
    }
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
        widget->requestTime = D_002DDED0.tReal;
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
    char pad[0x58]; int state; float time; int tick; int mode; float update;
    char pad1[4]; int id0; int id1; int id2; int selected; int x80; void *sprite; float animtime; float duration;
    char pad2[0xA4-0x90]; int flag; char pad3[8]; int pauses;
};
struct G { char pad[0x3BEC]; int count; };
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
void SetWidgetState(struct S *p,int state) {
    if(p->state==state) return;
    if(p->flag>=16) {
        if(p->state<2 && state>=2) D_003DD490.count++;
        else if(state<2 && p->state>=2) D_003DD490.count--;
    }
    if(p->pauses) {
        if(p->state<2 && state>=2) {
            if(++D_002DD318==1) SetClockRate(&D_002DDED0,7,0.0f);
        } else if(state<2 && p->state>=2) {
            if(--D_002DD318==0) SetClockRate(&D_002DDED0,7,1.0f);
        }
    }
    if(p->state==0) LoadWidgetSprites(p);
    p->state=state;
    p->time=D_002DDED0.time;
    p->tick=D_002BE538;
    switch(state) {
    case 0: UnloadWidgetSprites(p); break;
    case 1: StopWidgetMainAnim(p); break;
    case 2: StartWidgetMainAnim(p,p->id0); break;
    case 3: StartWidgetMainAnim(p,p->id1); break;
    case 4: StartWidgetMainAnim(p,p->id2); break;
    }
}
} } // namespace M_SetWidgetState

// 0x212C50 UpdateWidget
namespace M_UpdateWidget { extern "C" {
struct S {
    char pad[0x58]; int state; float time; int tick; int mode; float update;
    char pad1[4]; int id0; int id1; int id2; int selected; int x80; void *sprite; float animtime; float duration;
    char pad2[0xA4-0x90]; int flag; char pad3[8]; int pauses;
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
void UpdateWidget(struct S *p) {
    int state;
    UpdateLo(p);
    state=p->state;
    if(state==1) {
        if(p->mode==2) state=p->id1==-1 ? 2:3;
    } else if(p->update != D_002DDED0.time) {
        switch(state) {
        case 2:
            if(!FWidgetAnimPlaying(p,p->selected)) SetWidgetRequest(p,1);
            if(p->duration>0.0f && p->duration<D_002DDED0.time-p->time) SetWidgetRequest(p,1);
            if(p->mode==1) state=p->id2==-1 ? 1:4;
            break;
        case 3:
            if(!FWidgetAnimPlaying(p,p->id1)) {
                switch(p->mode) {
                case 1: state=p->id2==-1 ? 1:4; break;
                case 2: state=2; break;
                }
            }
            break;
        case 4:
            if(!FWidgetAnimPlaying(p,p->id2)) {
                switch(p->mode) {
                case 1: state=1; break;
                case 2: state=p->id1==-1 ? 2:3; break;
                }
            }
            break;
        }
    }
    SetWidgetState(p,state);
    if(p->selected!=-1) {
        float dt=g_clock.dtReal;
        p->animtime+=dt;
        AdvanceWidgetSprite(p->sprite,dt);
    }
}
} } // namespace M_UpdateWidget

// 0x212E40 SetWidgetAnimPhase
namespace M_SetWidgetAnimPhase { extern "C" {
struct E { char pad[0x14]; int id; };
struct H { char pad[8]; short h; char pada[0xE]; };
struct A { unsigned short c; unsigned short c2; struct E *ae; struct H *ah; char padc[0x24]; };
struct S { char pad[0x80]; unsigned short c80; short pad82; struct A *aa; };
void SetSpriteDefaultPhase(struct A *pa, float g);
void SetSpriteAnimPhase(struct A *pa, int l, float g, int z);
/** @sly2-annot addr=0x212E40
 * Purpose: seek anim id (0x314=main) to fraction g of its length (meters)
 * Info: proposed name SetWidgetAnimPhase (T1); TU hud
 * Confidence: mid (draft) [T1 0.60] - evidence: static analysis; sources: r7c_hud
 * See: docs/engine/hud-ui.md#1-widget-hud-objects-hud-tu-0x210eb0-0x21bb88-file-name-unknown,
 *   docs/engine/hud-ui.md#5-key-functions, docs/reference/functions/hud-ui.md#hud-0x210eb0-0x21bb88
 * @end-sly2-annot */
// Seeks a widget animation (0x314 = main) to a fraction of its length (meters and gauges).
void SetWidgetAnimPhase(struct S *p, int k, float g)
{
    int i, j;
    if (k == 0x314) {
        SetSpriteDefaultPhase(p->aa, g);
        return;
    }
    for (i = 0; i < p->c80; i++) {
        struct A *a = &p->aa[i];
        for (j = 0; j < a->c; j++) {
            struct E *e = &a->ae[j];
            int l = -1;
            if (k == e->id) {
                int m;
                for (m = 0; m < a->c2; m++)
                    if (a->ah[m].h != -1)
                        l = m;
                if (l >= 0)
                    SetSpriteAnimPhase(a, l, g, 0);
            }
        }
    }
}
} } // namespace M_SetWidgetAnimPhase

// 0x212F80 SetWidgetAnimTime
namespace M_func_00212F80 { extern "C" {
struct E { char pad[0x14]; int id; };
struct H { char pad[8]; short h; char pada[0xE]; };
struct A { unsigned short c; unsigned short c2; struct E *ae; struct H *ah; char padc[0x24]; };
struct S { char pad[0x80]; unsigned short c80; short pad82; struct A *aa; };
void SetSpriteDefaultTime(struct A *pa, float g);
void SetSpriteAnimTime(struct A *pa, int l, float g, int z);
// 0x212F78: dead-code scrap (sly2/scrap.h)
SLY2_SCRAP("addiu $sp, $sp, 0xb0");

/** @sly2-annot addr=0x212F80
 * Purpose: like SetWidgetAnimPhase but absolute time
 * Info: proposed name SetWidgetAnimTime (T2); TU hud
 * Confidence: low (draft) [T2 0.45] - evidence: static analysis; sources: r7c_hud
 * See: docs/reference/functions/hud-ui.md#hud-0x210eb0-0x21bb88
 * @end-sly2-annot */
// Seeks a widget animation to an absolute time. (inferred)
void SetWidgetAnimTime(struct S *p, int k, float g)
{
    int i, j;
    if (k == 0x314) {
        SetSpriteDefaultTime(p->aa, g);
        return;
    }
    for (i = 0; i < p->c80; i++) {
        struct A *a = &p->aa[i];
        for (j = 0; j < a->c; j++) {
            struct E *e = &a->ae[j];
            int l = -1;
            if (k == e->id) {
                int m;
                for (m = 0; m < a->c2; m++)
                    if (a->ah[m].h != -1)
                        l = m;
                if (l >= 0)
                    SetSpriteAnimTime(a, l, g, 0);
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
struct S { char pad[0xA4]; int a; char pad2[0xC]; struct S *next; };
/** @sly2-annot addr=0x2137F0
 * Purpose: some widget in exclusive list (*(struct S * *)&g_sw.dlWidgetExclusive) has higher priority (+0x0? field a)
 * Info: proposed name FWidgetObscured (T1); TU hud
 * Notes: names_v2 v1 proposed FUN_001aca68 (0.000)
 * Confidence: high [T1 0.85] - evidence: static naming pass; sources: r7c_hud+verdicts_followups
 * See: docs/engine/hud-ui.md#5-key-functions, docs/reference/functions/hud-ui.md#hud-0x210eb0-0x21bb88
 * @end-sly2-annot */
// True when an exclusive widget of higher priority is shown.
int FWidgetObscured(struct S *p)
{
    struct S *q;
    int unused[4];
    for (q = (*(struct S * *)&g_sw.dlWidgetExclusive); q; q = q->next) {
        if (p->a < q->a)
            return 1;
    }
    return 0;
}
} } // namespace M_FWidgetObscured

// 0x213830 StartWidgetMainAnim
namespace M_StartWidgetMainAnim { extern "C" {
struct VE { short d; short pad; void (*pf)(void *, int); };
struct S { char pad[0xC]; struct VE *vt; char pad2[0x6C]; int x7c; char pad3[4]; void *x84; char pad4[0x24]; int xac; };
void StartSpriteAnimById(void *p);
void SetWidgetExclusive(struct S *p, int a);
/** @sly2-annot addr=0x213830
 * Purpose: start anim id on main sprite (+0x84), slot21(1), join exclusive list if flag, +0x7C = id
 * Info: proposed name StartWidgetMainAnim (T1); TU hud
 * Confidence: mid (draft) [T1 0.50] - evidence: static analysis; sources: r7c_hud
 * See: docs/reference/functions/hud-ui.md#hud-0x210eb0-0x21bb88
 * @end-sly2-annot */
// Starts an animation on the widget's main sprite and joins the exclusive list if the widget is exclusive.
void StartWidgetMainAnim(struct S *p, int a)
{
    StartSpriteAnimById(p->x84);
    p->vt[21].pf((char *)p + p->vt[21].d, 1);
    if (p->xac)
        SetWidgetExclusive(p, 0);
    p->x7c = a;
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
  char pad10[0x6C]; int x7C; char pad80[4]; void *x84; int x88; char pad8C[0x20]; int xAC;
  void F(int) __asm__("SetWidgetExclusive");
  void M() __asm__("StopWidgetMainAnim");
};
// StopWidgetMainAnim: stops the widget's main sprite and leaves the exclusive list.
void S::M()
{
    StopSprite(x84);
    if (xAC)
        F(-1);
    v21(0);
    v3(6, 0, 0, 0, 0);
    x88 = 0;
    x7C = -1;
}
} // namespace M_StopWidgetMainAnim

// 0x213928 StopWidgetAnim
namespace M_StopWidgetAnim { extern "C" {
struct E { char pad[0x14]; int id; };
struct C { char pad[8]; short h8; char pada[0xE]; };
struct A { unsigned short c; unsigned short c2; struct E *ae; struct C *ac; char padc[0x14]; int x20; char pad24[0xC]; };
struct S { char pad[0x80]; unsigned short c80; short pad82; struct A *aa; };
void RestartNestedSprite(struct S *p, int j, int l, int y, int z);
/** @sly2-annot addr=0x213928
 * Purpose: reset the sprite holding anim id back to its rest anim
 * Info: proposed name StopWidgetAnim (T1); TU hud
 * Confidence: mid (draft) [T1 0.60] - evidence: static analysis; sources: r7c_hud
 * See: docs/reference/functions/hud-ui.md#hud-0x210eb0-0x21bb88
 * @end-sly2-annot */
// Returns the sprite that plays an animation id to its rest animation.
void StopWidgetAnim(struct S *p, int k)
{
    int j, l, m;
    for (j = 0; j < p->c80; j++) {
        struct A *a = &p->aa[j];
        for (l = 0; l < a->c; l++) {
            struct E *e = &a->ae[l];
            if (k == e->id) {
                int f = 0;
                for (m = 0; m < a->c2; m++) {
                    if (a->ac[m].h8 != -1)
                        f = 1;
                }
                if (f)
                    RestartNestedSprite(p, j, 0, 0, 0);
                else
                    a->x20 = -1;
                return;
            }
        }
    }
}
} } // namespace M_StopWidgetAnim

// 0x213A08 PlayWidgetAnimsWithArg
namespace M_func_00213A08 { extern "C" {
struct E { char pad[0x14]; int id; };
struct A { unsigned short c; short pad; struct E *ae; char pad8[0x28]; };
struct S { char pad[0x80]; unsigned short c80; short pad82; struct A *aa; };
void RestartNestedSpriteWithArg(struct S *p, int j, int l, int x, int y, int z);
/** @sly2-annot addr=0x213A08
 * Purpose: start every sprite holding anim id with argument x
 * Info: proposed name PlayWidgetAnimsArg (T2); TU hud
 * Notes: names_v2 v1 proposed render_level_info (0.033)
 * Confidence: low (draft) [T2 0.35] - evidence: static naming pass; sources: r7c_hud
 * See: docs/reference/functions/hud-ui.md#hud-0x210eb0-0x21bb88
 * @end-sly2-annot */
// Starts every sprite that has the animation id, with an argument. (inferred)
void PlayWidgetAnimsWithArg(struct S *p, int k, int x)
{
    int j, l;
    for (j = 0; j < p->c80; j++) {
        struct A *a = &p->aa[j];
        for (l = 0; l < a->c; l++) {
            struct E *e = &a->ae[l];
            if (k == e->id)
                RestartNestedSpriteWithArg(p, j, 0, x, 0, 0);
        }
    }
}
} } // namespace M_func_00213A08

// 0x213B00 PlayWidgetAnim
namespace M_PlayWidgetAnim { extern "C" {
struct E { char pad[0x14]; int id; };
struct C { char pad[8]; short h8; char pada[0xE]; };
struct A { unsigned short c; unsigned short c2; struct E *ae; struct C *ac; char padc[0x14]; int x20; char pad24[0xC]; };
struct S { char pad[0x80]; unsigned short c80; short pad82; struct A *aa; };
void RestartNestedSprite(struct S *p, int j, int l, int y, int z);
/** @sly2-annot addr=0x213B00
 * Purpose: find sprite owning anim id and start it (restart in place if instanced)
 * Info: proposed name PlayWidgetAnim (T1); TU hud
 * Notes: names_v2 C proposed SetupDraw__5CFontP8CTextBoxP4GIFS (0.438)
 * Confidence: mid (draft) [T1 0.65] - evidence: static naming pass; sources: r7c_hud
 * See: docs/reference/functions/hud-ui.md#hud-0x210eb0-0x21bb88
 * @end-sly2-annot */
// Starts the animation id on the sprite that owns it (restarting in place if it is instanced).
void PlayWidgetAnim(struct S *p, int k)
{
    int j, l, m;
    if (k == -1)
        return;
    for (j = 0; j < p->c80; j++) {
        struct A *a = &p->aa[j];
        for (l = 0; l < a->c; l++) {
            struct E *e = &a->ae[l];
            if (k == e->id) {
                int f = 0;
                for (m = 0; m < a->c2; m++) {
                    if (a->ac[m].h8 != -1)
                        f = 1;
                }
                if (f)
                    RestartNestedSprite(p, j, l, 0, 0);
                else
                    a->x20 = l;
                return;
            }
        }
    }
}
} } // namespace M_PlayWidgetAnim

// 0x213BD8 PlayWidgetAnimWithArg
namespace M_func_00213BD8 { extern "C" {
struct E { char pad[0x14]; int id; };
struct A { unsigned short c; short pad; struct E *ae; char pad8[0x28]; };
struct S { char pad[0x80]; unsigned short c80; short pad82; struct A *aa; };
void RestartNestedSpriteWithArg(struct S *p, int j, int l, int x, int y, int z);
/** @sly2-annot addr=0x213BD8
 * Purpose: start first anim id match with argument x
 * Info: proposed name PlayWidgetAnimArg (T2); TU hud
 * Notes: names_v2 C proposed DxFromPchz__5CFontPc (0.438)
 * Confidence: low (draft) [T2 0.40] - evidence: static naming pass; sources: r7c_hud
 * See: docs/reference/functions/hud-ui.md#hud-0x210eb0-0x21bb88
 * @end-sly2-annot */
// Starts the first sprite that has the animation id, with an argument. (inferred)
void PlayWidgetAnimWithArg(struct S *p, int k, int x)
{
    int j, l;
    for (j = 0; j < p->c80; j++) {
        struct A *a = &p->aa[j];
        for (l = 0; l < a->c; l++) {
            struct E *e = &a->ae[l];
            if (k == e->id) {
                RestartNestedSpriteWithArg(p, j, l, x, 0, 0);
                return;
            }
        }
    }
}
} } // namespace M_func_00213BD8

// 0x213C68 SetWidgetXf
namespace M_SetWidgetXf { extern "C" {
typedef int qword __attribute__((mode(TI)));
struct M { qword a, b; };
struct VE { short d; short pad; struct M *(*pf)(void *, int, int); };
struct A { struct VE *vt; };
struct S { char pad[0xC4]; struct A *pa; char pad2[0x30]; struct M *pm; };
/** @sly2-annot addr=0x213C68
 * Purpose: set widget 2D transform override (alloc 0x20 M23 at +0xF8)
 * Info: proposed name SetWidgetXf (T1); TU hud
 * Notes: names_v2 v1 proposed FUN_001ad6a8 (0.000)
 * Confidence: mid (draft) [T1 0.55] - evidence: static naming pass; sources: r7c_hud
 * See: docs/reference/functions/hud-ui.md#hud-0x210eb0-0x21bb88
 * @end-sly2-annot */
// Sets the widget's 2D transform override.
void SetWidgetXf(struct S *p, struct M *src)
{
    if (!p->pm) {
        struct A *a = p->pa;
        p->pm = a->vt[1].pf((char *)a + a->vt[1].d, 0x20, 0x1F);
    }
    *p->pm = *src;
}
} } // namespace M_SetWidgetXf

// 0x213CD8 SetWidgetAnimXf
namespace M_SetWidgetAnimXf {
struct M { int a[8]; } __attribute__((aligned(16)));
struct H { virtual void *Alloc(int cb, int k); };
struct E { char pad[0x14]; int id; };
struct A { unsigned short c; short pad; E *ae; char pad8[0x24]; M *x2C; };
struct S { char pad[0x80]; unsigned short c80; short pad82; A *aa; char pad88[0x3C]; H *xC4; };
/** @sly2-annot addr=0x213CD8
 * Purpose: set M23 transform of the sprite that holds anim id
 * Info: proposed name SetWidgetAnimXf (T1); TU hud
 * Confidence: high [T1 0.85] - evidence: static analysis; sources: r7c_hud+verdicts_followups
 * See: docs/engine/hud-ui.md#5-key-functions, docs/reference/functions/hud-ui.md#hud-0x210eb0-0x21bb88
 * @end-sly2-annot */
// Sets the 2D transform of the sprite that plays an animation id.
extern "C" void SetWidgetAnimXf(S *p, int k, M *pm)
{
    int j, l;
    for (j = 0; j < p->c80; j++) {
        A *a = &p->aa[j];
        for (l = 0; l < a->c; l++) {
            E *e = &a->ae[l];
            if (e->id == k) {
                if (!a->x2C)
                    a->x2C = (M *)p->xC4->Alloc(0x20, 0x1F);
                *a->x2C = *pm;
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
void SwitchWidgetAnim(void *p, int k, int *pk)
{
    if (k == *pk)
        return;
    if (*pk != -1)
        StopWidgetAnim(p, *pk);
    *pk = k;
    if (k != -1)
        PlayWidgetAnim(p, k);
}
} } // namespace M_SwitchWidgetAnim

// 0x213E20 FindSpriteItemPose
namespace M_func_00213E20 { extern "C" {
typedef int qword __attribute__((mode(TI)));
struct RGBA { unsigned char r, g, b, a; };
struct QQ { qword a, b; };
struct Z { int x0; int k; char pad8[8]; int id; };
struct Y { struct RGBA c0; struct RGBA c1; char pad8[8]; struct QQ q; struct Z *pz; char pad34[0x24]; int spriteSlot; char pad5c[0x14]; };
struct H { int c; struct Y *ay; char pad8[0x10]; };
struct A { unsigned short c; unsigned short c2; void *ae; struct H *ah; char padc[0x24]; };
struct S { char pad[0x84]; struct A *aa; };
/** @sly2-annot addr=0x213E20
 * Purpose: recursive search of nested sprite k: return its M23 and colours
 * Info: proposed name FFindSpriteItemPose (T2); TU hud
 * Notes: names_v2 B proposed ClineWrapPchz__5CFontPcf (0.856)
 * Confidence: low (draft) [T2 0.40] - evidence: static naming pass; sources: r7c_hud
 * See: docs/reference/functions/hud-ui.md#hud-0x210eb0-0x21bb88
 * @end-sly2-annot */
// Recursively finds nested sprite k and returns its 2D transform and colours. (inferred)
int FindSpriteItemPose(struct S *p, int k, struct QQ *pq, struct RGBA *pc0, struct RGBA *pc1, int c, struct Y *ay)
{
    int i;
    struct Y *y;
    if (!ay) {
        struct H *h = p->aa->ah;
        c = h->c;
        ay = h->ay;
    }
    for (i = 0; i < c; i++) {
        struct Z *z;
        y = &ay[i];
        z = y->pz;
        if (!z || z->k != 4)
            continue;
        if (k == z->id) {
            *pq = y->q;
            *pc0 = y->c0;
            *pc1 = y->c1;
            return 1;
        }
        if (FindSpriteItemPose(p, k, pq, pc0, pc1, p->aa[z->id].ah[y->spriteSlot].c, p->aa[z->id].ah[y->spriteSlot].ay))
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
struct Z { int x0; int k; char pad8[8]; int id; };
struct Y { int c0; int c1; char pad8[8]; QQ q; Z *pz; char pad34[0x24]; int spriteSlot; char pad5c[0xC]; int x68; int x6C; };
struct H { int c; Y *ay; char pad8[0x10]; };
struct A { unsigned short c; unsigned short c2; void *ae; H *ah; char padc[0x24]; };
struct S { char pad[0x84]; A *aa; };
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
extern "C" int RestartNestedSprite(S *p, int k, int z, int c, Y *ay)
{
    int i;
    Y *y;
    if (!ay) {
        H *h = p->aa->ah;
        c = h->c;
        ay = h->ay;
    }
    for (i = 0; i < c; i++) {
        Z *pz;
        y = &ay[i];
        pz = y->pz;
        if (!pz || pz->k != 4)
            continue;
        int oldframe=y->spriteSlot;
        if (pz->id == k) {
            StopSpriteSlot(&p->aa[k], oldframe);
            ClearSle(&y->x68);
            y->spriteSlot = IFreeSpriteSlot(&p->aa[k]);
            StartSpriteAnimAt(&p->aa[k], z, y->spriteSlot, &y->q, 0.0f, RGBA(0x7F7F7F7F), RGBA(0xFFFFFFFF));
            return 1;
        }
        {
            H *h = (H *)(oldframe * sizeof(H) + (int)p->aa[pz->id].ah);
            if (RestartNestedSprite(p, k, z, h->c, h->ay))
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
struct Z { int x0; int k; char pad8[8]; int id; };
struct Y { int c0; int c1; char pad8[8]; QQ q; Z *pz; char pad34[0x24]; int spriteSlot; char pad5c[8]; int x64; int x68; int x6C; };
struct H { int c; Y *ay; char pad8[0x10]; };
struct A { unsigned short c; unsigned short c2; void *ae; H *ah; char padc[0x24]; };
struct S { char pad[0x84]; A *aa; };
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
extern "C" int RestartNestedSpriteWithArg(S *p, int k, int z, int x, int c, Y *ay)
{
    int i;
    Y *y;
    if (!ay) {
        H *h = p->aa->ah;
        c = h->c;
        ay = h->ay;
    }
    for (i = 0; i < c; i++) {
        Z *pz;
        y = &ay[i];
        pz = y->pz;
        if (!pz || pz->k != 4)
            continue;
        int oldframe=y->spriteSlot;
        if (pz->id == k) {
            if(x != y->x64) continue;
            StopSpriteSlot(&p->aa[k], oldframe);
            ClearSle(&y->x68);
            y->spriteSlot = IFreeSpriteSlot(&p->aa[k]);
            StartSpriteAnimAt(&p->aa[k], z, y->spriteSlot, &y->q, 0.0f, RGBA(0x7F7F7F7F), RGBA(0xFFFFFFFF));
            return 1;
        }
        {
            H *h = (H *)(oldframe * sizeof(H) + (int)p->aa[pz->id].ah);
            if (RestartNestedSpriteWithArg(p, k, z, x, h->c, h->ay))
                return 1;
        }
    }
    return 0;
}
} // namespace M_func_00214100

// 0x2142B0 FSpriteHasAnim
namespace M_FSpriteHasAnim { extern "C" {
struct E { char pad[0x14]; int id; };
struct S { unsigned short c; short pad; struct E *ae; };
/** @sly2-annot addr=0x2142B0
 * Purpose: id in sprite anim list
 * Info: proposed name FSpriteHasAnim (T1); TU hud
 * Notes: names_v2 v1 proposed FUN_001ae130 (0.000)
 * Confidence: mid (draft) [T1 0.55] - evidence: static naming pass; sources: r7c_hud
 * See: docs/reference/functions/hud-ui.md#hud-0x210eb0-0x21bb88
 * @end-sly2-annot */
// True when the sprite's animation list contains the id.
int FSpriteHasAnim(void *pv, struct S *p, int id)
{
    int i;
    if (id == -1)
        return 0;
    for (i = 0; i < p->c; i++) {
        if (id == p->ae[i].id)
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
extern "C" void SetWidgetExclusive(WIDGETV *widget, int f)
{
    int fOld;
    fOld = widget->isNonExclusive;
    if (fOld != f) {
        if ((fOld == 0) && (FFindDlEntry(&g_sw.dlWidgetExclusive, widget) != 0)) {
            RemoveDlEntry(&g_sw.dlWidgetExclusive, widget);
        }
        widget->isNonExclusive = f;
        if ((f == 0) && (FFindDlEntry(&g_sw.dlWidgetExclusive, widget) == 0)) {
            AppendDlEntry(&g_sw.dlWidgetExclusive, widget);
        }
    }
}

// 0x214398 LoadSpriteTable
namespace M_LoadSpriteTable { extern "C" {
struct S { char pad[0xC4]; void *xC4; };
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
    *pm = PvAllocClear__FP4HEAPii(p->xC4, (*pn + 1) << 6, D_002C40A4 ? 0x13 : 0x11);
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
 case 0:case 5:default:break;
 case 4:sprite->data.index=U16Read__18CBinaryInputStream(stream);break;
 case 8:
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
 case 3:{
  sprite->data.shader.tag=U32Read__18CBinaryInputStream(stream);sprite->data.shader.slot=U32Read__18CBinaryInputStream(stream);
  sprite->data.shader.size=F32Read__18CBinaryInputStream(stream);sprite->data.shader.scale=F32Read__18CBinaryInputStream(stream);sprite->data.shader.value=F32Read__18CBinaryInputStream(stream);
  sprite->data.shader.flag=S8Read__18CBinaryInputStream(stream);sprite->data.shader.mode=S8Read__18CBinaryInputStream(stream);
  if(sprite->data.shader.mode==1)sprite->data.shader.id=S32Read__18CBinaryInputStream(stream);else if(sprite->data.shader.mode==0)sprite->data.shader.id=S32Read__18CBinaryInputStream(stream);
  if(sprite->data.shader.mode==1&&sprite->data.shader.id==0x2F2)sprite->data.shader.special=sprite->data.shader.mode;
  else sprite->data.shader.special=0;
  break;
 }
 case 2:{
  ReadMatrix__18CBinaryInputStreamP7MATRIX3(stream,&raw);M3 matrix=raw;CopyXY(sprite->data.polygon.matrix,matrix);
  sprite->data.polygon.count=U16Read__18CBinaryInputStream(stream);
  sprite->data.polygon.vertices=(VERTEX *)PvAllocClear__FP4HEAPii(owner->heap,sprite->data.polygon.count*sizeof(VERTEX),0x14);
  Read__18CBinaryInputStreamiPv(stream,sprite->data.polygon.count*sizeof(VERTEX),sprite->data.polygon.vertices);break;
 }
 case 6:case 7:{
  PAIR first,second;
  first.x=S16Read__18CBinaryInputStream(stream);first.y=S16Read__18CBinaryInputStream(stream);second.x=S16Read__18CBinaryInputStream(stream);second.y=S16Read__18CBinaryInputStream(stream);
  int texture=S32Read__18CBinaryInputStream(stream);
  STACKSCOPE inner;GIFS gif;
  gif.AllocSw(16);gif.AddPrimPack(0,1,0xE);
  if(sprite->kind==6){sprite->data.effect.id=texture;sprite->data.effect.handle=0;gif.PackAD(0x47,0x5000F);gif.PackAD(0x42,0x80000000A8L);}
  else{sprite->data.picture.id=texture;gif.PackAD(0x42,0x44);gif.PackAD(0x47,0x50000);}
  gif.AddPrimPack(0x54,9,0x525252521L);gif.PackRGBA(0x80808080);
  if(sprite->kind==6){
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
  if(sprite->kind==6)vif.DetachCopySw(owner->heap,&sprite->data.effect.count,0,&sprite->data.effect.packet);
  else{vif.DetachCopySw(owner->heap,&sprite->data.picture.count,0,&sprite->data.picture.packet);PostSwCallback(&D_003DD490,owner,D_002A8B08,sprite);}
  break;
 }
 case 1:{
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
struct X { char pad[0x14]; unsigned long *px14; };
X *FindShaderById(int);
void __4GIFS(void *);
void AllocSw__4DMASi(void *, int);
void AddPrimPack__4GIFSiiUl(void *, int, int, int);
void PackAD__4GIFSll(void *, int, unsigned long);
void AddPrimEnd__4GIFS(void *);
void DetachCopySw__4DMASPiPP2QWT2i(void *, int, int *, int, int *) __asm__("DetachCopySw__4DMASP4HEAPPiPP2QWT3");   /* DMAS::DetachCopySw(HEAP *, ...) (dmas.cpp) */
}
struct BUF { int a[12]; };
struct P { char pad[0xC4]; int xC4; };
struct Q { int x0; int x4; char pad8[8]; int x10; char pad14[8]; int x1C; int x20; };
extern "C" void BuildSpritePictureTexPacket(P *p, Q *q);
/** @sly2-annot addr=0x2155C0
 * Purpose: kind-7 picture callback: GIF TEX0/TEXA packet for the texture of q->x10
 * Info: proposed name BuildSpritePictureTexPacket (T2); TU hud
 * Confidence: low (draft) [T2 0.35] - evidence: static analysis; sources: r7c_hud
 * See: docs/reference/functions/hud-ui.md#hud-0x210eb0-0x21bb88
 * @end-sly2-annot */
// Picture-sprite callback: the GIF packet that selects its texture. (inferred)
void BuildSpritePictureTexPacket(P *p, Q *q)
{
    STACKSCOPE frame;
    if (q->x4 == 7) {
        q->x1C = 0;
        q->x20 = 0;
        X *x = FindShaderById(q->x10);
        if (x) {
            BUF b;
            __4GIFS(&b);
            AllocSw__4DMASi(&b, 0x10);
            AddPrimPack__4GIFSiiUl(&b, 0, 1, 0xE);
            PackAD__4GIFSll(&b, 6, *x->px14);
            PackAD__4GIFSll(&b, 0x14, 0x60);
            PackAD__4GIFSll(&b, 8, 5);
            AddPrimEnd__4GIFS(&b);
            DetachCopySw__4DMASPiPP2QWT2i(&b, p->xC4, &q->x1C, 0, &q->x20);
        }
    }
}
} // namespace M_func_002155C0

// 0x2156B8 InitSpriteItem
namespace M_InitSpriteItem {
struct RGBA { unsigned int u; RGBA() {} RGBA(unsigned int x) { u = x; } };
struct CP { RGBA c0, c1; };
struct S { CP x0; char pad[0x28]; int x30, x34, x38; CP x3c; void *x44; int x48; CP x4c; int x54, x58, x5c; int pad2; int x64; char x68[4]; };
extern "C" char D_002A3A10[];
extern "C" void ClearSle(void *p);
/** @sly2-annot addr=0x2156B8
 * Purpose: item defaults: colours 0x7F7F7F7F/0xFFFFFFFF, identity M23, ids -1
 * Info: proposed name InitSpriteItem (T1); TU hud
 * Confidence: mid (draft) [T1 0.50] - evidence: static analysis; sources: r7c_hud
 * See: docs/reference/functions/hud-ui.md#hud-0x210eb0-0x21bb88
 * @end-sly2-annot */
// Sets a sprite item's defaults: neutral colours, identity transform, no ids.
extern "C" void InitSpriteItem(S *p)
{
    p->x44 = D_002A3A10;
    p->x30 = 0;
    p->x54 = 0;
    p->x0.c0 = RGBA(0x7F7F7F7F); p->x0.c1 = RGBA(0xFFFFFFFF);
    p->x3c.c0 = RGBA(0x7F7F7F7F); p->x3c.c1 = RGBA(0xFFFFFFFF);
    p->x4c.c0 = RGBA(0x7F7F7F7F); p->x4c.c1 = RGBA(0xFFFFFFFF);
    p->x64 = -1;
    p->x58 = 0;
    p->x34 = 0;
    p->x5c = 0;
    p->x38 = 0;
    p->x48 = 0;
    ClearSle(p->x68);
}
} // namespace M_InitSpriteItem

// 0x215730 SetSpriteItemSource
namespace M_func_00215730 {
struct RGBA2 { unsigned char r, g, b, a; };
struct RGBA {
    unsigned char r, g, b, a;
    RGBA(const RGBA &c) { *(RGBA2 *)this = *(const RGBA2 *)&c; }
} __attribute__((aligned(16)));
struct L { char pad[0x84]; char *pb; };
struct S { int x0; int k; L *pl; int xc; int i; void *x14; };
struct T { char pad[0x30]; S *x30; char pad34[0x24]; int spriteSlot; };
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
extern "C" void SetSpriteItemSource(S *p, T *t, void *q, RGBA c0, RGBA c1)
{
    t->x30 = p;
    switch (p->k) {
    case 4:
        t->spriteSlot = IFreeSpriteSlot(p->pl->pb + p->i * 0x30);
        StartSpriteAnimSlot(p->pl->pb + p->i * 0x30, 0x314, t->spriteSlot, q, c0, c1);
        break;
    case 6:
        if (p->i != -1) {
            void *pv = FindObjectForRenderTarget(p->i);
            if (pv)
                p->x14 = AcquireRenderTarget(pv);
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
struct L { char pad[0x84]; char *pb; };
struct S { int x0; int k; L *pl; int xc; int i; };
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
extern "C" void UpdateSpriteItem(float g, S *p, T *t, void *b, RGBA c0, RGBA c1)
{
    if (p->k == 4)
        UpdateSpriteSlot(p->pl->pb + p->i * 0x30, t->spriteSlot, b, g, c0, c1);
}
} // namespace M_func_00215850

// 0x2158C0 ReleaseSpriteItem
namespace M_func_002158C0 { extern "C" {
struct L { char pad[0x84]; char *pb; };
struct Q { char pad[0x20]; void *x20; };
struct S { int x0; int k; struct L *pl; int xc; int i; struct Q *x14; };
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
void ReleaseSpriteItem(struct S *p, struct T *t)
{
    switch (p->k) {
    case 4:
        StopSpriteSlot(p->pl->pb + p->i * 0x30, t->spriteSlot);
        break;
    case 6:
        if (p->x14) {
            if (p->x14->x20)
                ReleaseRenderTarget(p->x14->x20);
            p->x14 = 0;
        }
        break;
    }
    InitSpriteItem(t);
}
} } // namespace M_func_002158C0

// 0x215960 QueueSpriteItem
namespace M_func_00215960 { extern "C" {
struct L { char pad[0x84]; char *pb; };
struct S { int x0; int k; struct L *pl; int xc; int i; };
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
void QueueSpriteItem(struct S *p, struct T *t, int b)
{
    switch (p->k) {
    case 4:
        DrawSpriteSlot(p->pl->pb + p->i * 0x30, t->spriteSlot, t->drawFlags);
        break;
    case 1:
    case 3:
    case 6:
    case 7:
        t->drawFlags = b;
        PrependSlEntry((char *)p->pl + 0xFC, t);
        break;
    case 2:
    case 5:
        break;
    }
}
} } // namespace M_func_00215960

// 0x2159E0 RenderSpritePayload
namespace M_func_002159E0 { extern "C" {
struct S { int a; int k; };
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
void RenderSpritePayload(struct S *p, int a, int b, int c)
{
    switch (p->k) {
    case 3:
        RenderSpriteText(p, a, b, c);
        break;
    case 1:
        RenderSpriteGeometry(p, a, b, c);
        break;
    case 6:
    case 7:
        RenderSpritePicture(p, a, b, c);
        break;
    case 2:
    case 4:
    case 5:
    case 8:
        break;
    }
}
} } // namespace M_func_002159E0

// 0x215A50 IglyffFromCh__8CFontBrxc
namespace M_IglyffFromCh__8CFontBrxc { extern "C" {
struct CFontBrx { char pad[0x20]; int c; unsigned int *ach; };
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
    int hi = p->c - 1;
    while (lo <= hi) {
        int mid = (hi + lo) / 2;
        int d = ch - p->ach[mid];
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
struct F { char pad[0x54]; char *p54; };
struct G { char pad[0x3BE8]; F *pf; };
extern "C" G D_003DD490;
extern "C" Q D_002A2E10;
extern "C" Q D_002A2E20;
struct S { int x0; int x4; char *x8; char *xC; char *x10; int x14; RGBA x18; char pad1c[0x24]; Q q40; Q q50;
  char pad60[4]; float f64; float f68; int x6C; int x70; int x74; int x78; float f7C; float f80; float f84; float f88;
  int x8C; int x90; int x94; };
extern "C" void ResetRichText2(S *p, int i);
/** @sly2-annot addr=0x215AC0
 * Purpose: rich-text ctor from string + font + scale (offsets 1792/1936)
 * Info: proposed name CRichText_ctor (T1); class CRichText; TU hud
 * Confidence: mid (draft) [T1 0.50] - evidence: static analysis; sources: r7c_hud
 * See: docs/reference/functions/hud-ui.md#hud-0x210eb0-0x21bb88
 * @end-sly2-annot */
// Rich-text constructor from a string, a font and a scale.
extern "C" S *CRichText_ctor(S *p, int a, float f)
{
    p->x0 = a;
    p->f7C = 1.0f;
    p->f80 = 1.0f;
    p->f64 = 1792.0f;
    p->f68 = 1936.0f;
    p->f88 = f;
    p->x6C = 0;
    p->x70 = 0;
    p->x74 = 0;
    p->x78 = 0;
    p->f84 = f * 51.2f;
    p->x90 = 1;
    p->x8C = 1;
    p->x94 = 6;
    p->x8 = p->xC = D_003DD490.pf->p54;
    p->x10 = D_003DD490.pf->p54 + 0x40;
    p->x18 = RGBA(0x80808080);
    p->q40 = D_002A2E10;
    p->q50 = D_002A2E20;
    ResetRichText2(p, 0);
    return p;
}
} // namespace M_CRichText_ctor

// 0x215BA8 ConstructRichText2FromSprite
namespace M_func_00215BA8 {
struct Q { int a[4]; } __attribute__((aligned(16)));
struct Q3 { Q a, b, c; };
struct F { char pad[0x54]; char *p54; };
struct G { char pad[0x3BE8]; F *pf; };
extern "C" G D_003DD490;
struct C8 { char pad[0x9C]; float f9C; float fA0; };
struct P8 { char pad[0xA4]; int xA4; char padA8[0x20]; C8 *pC8; };
struct R { char pad[8]; P8 *p8; char padc[0x10]; int x1C; float f20; float f24; float f28; int x2C; int x30; };
struct S { int x0; int x4; char *x8; char *xC; char *x10; int x14; int x18; char pad1c[0x24]; Q q40; Q q50;
  char pad60[4]; float f64; float f68; int x6C; int x70; float f74; float f78; float f7C; float f80; float f84; float f88;
  int x8C; int x90; int x94; };
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
extern "C" S *ConstructRichText2FromSprite(S *p, int a, R *r, Q3 *pq)
{
    Q3 t = *pq;
    p->x0 = a;
    p->x6C = 0;
    p->x70 = 0;
    p->f74 = r->f24;
    p->f78 = r->f28;
    p->f7C = 1.0f;
    p->f80 = 1.0f;
    p->x90 = r->x2C;
    p->x8C = r->x30;
    p->f64 = r->p8->pC8->f9C;
    p->f68 = r->p8->pC8->fA0;
    p->x94 = r->p8->xA4;
    p->f84 = r->f20;
    p->f88 = p->f84 * 0.01953125f;
    p->x8 = p->xC = D_003DD490.pf->p54;
    p->x10 = D_003DD490.pf->p54 + 0x40;
    p->x18 = r->x1C;
    p->q40 = t.b;
    p->q50 = t.c;
    ResetRichText2(p, 0);
    return p;
}
} // namespace M_func_00215BA8

// 0x215CA8 ResetRichText2
namespace M_func_00215CA8 { extern "C" {
struct RGBA { unsigned char r, g, b, a; };
struct S { int x0; int x4; int x8; int xc; int x10; struct RGBA c1; struct RGBA c0; unsigned int x1c; };
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
void ResetRichText2(struct S *p, int a)
{
    if (a)
        p->x0 = a;
    p->x4 = p->x0;
    p->x8 = p->xc;
    p->c1 = p->c0;
    p->x1c = 0x80000000;
    ((unsigned char *)&p->x1c)[3] = p->c1.a;
    UpdateRichText2Color(p);
}
} } // namespace M_func_00215CA8

// 0x215D00 SetRichText2BaseColor
namespace M_func_00215D00 { extern "C" {
struct RGBA { unsigned char r, g, b, a; };
struct S { char pad[0x14]; struct RGBA c1; struct RGBA c0; };
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
void SetRichText2BaseColor(struct S *p, struct RGBA *pc)
{
    p->c0 = *pc;
    p->c1 = p->c0;
    UpdateRichText2Color(p);
}
} } // namespace M_func_00215D00

// 0x215D40 GetRichText2NextChar
namespace M_func_00215D40 { extern "C" {
struct RGBA { unsigned char r, g, b, a; };
struct S {
    int x0; unsigned char *pch; int x8; int xC; int x10;
    struct RGBA c1; struct RGBA c0; struct RGBA c2;
    char pad20[0x40]; int x60;
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
unsigned char GetRichText2NextChar(struct S *p)
{
    int fDone = 0;
    p->x60 = 0;
    while (!fDone) {
        unsigned char *pch = p->pch;
        unsigned char c = *pch;
        if (c == '&') {
            if (pch[1] == c) {
                p->pch = pch + 1;
                fDone = 1;
            } else if (pch[1] == '.') {
                p->pch = pch + 2;
                p->x8 = p->xC;
                p->c1 = p->c0;
                p->x60 = 1;
                UpdateRichText2Color(p);
            } else {
                p->pch = pch + 2;
                p->x8 = p->x10;
                p->c1 = p->c2;
                p->x60 = 1;
                UpdateRichText2Color(p);
            }
        } else {
            fDone = 1;
        }
    }
    return *p->pch++;
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
int CRichText_Cch(void *p)
{
    int n = 0;
    ResetRichText2(p, 0);
    while (GetRichText2NextChar(p))
        n++;
    return n;
}
} } // namespace M_CRichText_Cch

// 0x215E80 CRichText_Trim
namespace M_CRichText_Trim { extern "C" {
struct S { int a; char *pch; };
void ResetRichText2(struct S *p, int a);
int GetRichText2NextChar(struct S *p);
/** @sly2-annot addr=0x215E80
 * Purpose: truncate after n chars
 * Info: proposed name CRichText_Trim (T1); class CRichText; TU hud
 * Confidence: high [T1 0.85] - evidence: static analysis; sources: r7c_hud+verdicts_followups
 * See: docs/engine/hud-ui.md#5-key-functions, docs/reference/functions/hud-ui.md#hud-0x210eb0-0x21bb88
 * @end-sly2-annot */
// Truncates the rich text after n printable characters.
void CRichText_Trim(struct S *p, int n)
{
    ResetRichText2(p, 0);
    while (GetRichText2NextChar(p) && n != 0)
        n--;
    *p->pch = 0;
}
} } // namespace M_CRichText_Trim

// 0x215ED8 CRichText_Dx
namespace M_CRichText_Dx { extern "C" {
struct S { char pad[8]; void *x8; char pad2[0x7C]; float x88; };
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
        sum += GetFontGlyphWidth2(p->x8, ch) * p->x88;
    return sum;
}
} } // namespace M_CRichText_Dx

// 0x215F40 WrapRichText2Lines
namespace M_func_00215F40 {
extern "C" {
struct S { unsigned char *start; unsigned char *cur; void *font; char pad[0x74-0xC]; float width; char pad1[0x88-0x78]; float scale; };
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
            width+=GetFontGlyphWidth2(p->font,c)*p->scale;
        }
        if(p->width!=0.0f && p->width<width) {
                unsigned char *q=p->cur-1;
                while(q>=p->start && *q!='\n') {
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
    char pad[0x14]; struct RGBA c1; struct RGBA c0;
    char pad1c[4];
    float r, g, b, a2C, x30, x34, x38, a;
    float rA, gA, bA, x4C, x50, x54, x58, aA;
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
void UpdateRichText2Color(struct S *p)
{
    float k = p->aA * 0.007874016f;
    float a;
    a = k * (float)p->c1.a;
    p->r = GMin(p->rA + (float)p->c1.r, 255.0f);
    p->g = GMin(p->gA + (float)p->c1.g, 255.0f);
    p->b = GMin(p->bA + (float)p->c1.b, 255.0f);
    p->a2C = p->x4C;
    p->x30 = p->x50;
    p->x34 = p->x54;
    p->x38 = p->x58;
    p->a = GClamp(a, 0.0f, 1.0f);
}
} } // namespace M_func_002160D8

// 0x2161C8 GetRichText2MaxLineWidth
namespace M_func_002161C8 { extern "C" {
struct S { char pad[8]; void *x8; char padc[0x7C]; float f88; };
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
float GetRichText2MaxLineWidth(struct S *p)
{
    float fMax = 0.0f;
    float fCur = fMax;
    int ch;
    ResetRichText2(p, 0);
    while ((ch = GetRichText2NextChar(p)) != 0) {
        if (ch == 10) {
            fMax = (fMax > fCur) ? fMax : fCur;
            fCur = 0.0f;
        } else {
            fCur += GetFontGlyphWidth2(p->x8, ch) * p->f88;
        }
    }
    fMax = (fMax > fCur) ? fMax : fCur;
    return fMax;
}
} } // namespace M_func_002161C8

// 0x216260 CRichText_GetExtents
namespace M_CRichText_GetExtents { extern "C" {
struct S { char pad[0x84]; float x84; };
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
void CRichText_GetExtents(struct S *p, float *pa, float *pb)
{
    int n = WrapRichText2Lines(p);
    float f = GetRichText2MaxLineWidth(p);
    float g = n * p->x84;
    if (pa)
        *pa = f;
    if (pb)
        *pb = g;
}
} } // namespace M_CRichText_GetExtents

// 0x2162D8 BuildRichTextMatrix
namespace M_func_002162D8 { extern "C" {
struct S { char pad[0x64]; float f64; float f68; };
void *func_002333B4(void *p, int c, int n) __asm__("memset");
/** @sly2-annot addr=0x2162D8
 * Purpose: glyph matrix from 2x3 transform (0.8 / 0.4545 pixel scales, GS offset 786432)
 * Info: proposed name BuildRichTextMatrix (T2); TU hud
 * Notes: names_v2 C proposed SetNoteAchzDraw__FP4NOTEPc (0.638)
 * Confidence: low (draft) [T2 0.40] - evidence: static naming pass; sources: r7c_hud
 * See: docs/reference/functions/hud-ui.md#hud-0x210eb0-0x21bb88
 * @end-sly2-annot */
// Builds a glyph matrix from a 2D transform (pixel aspect scales and the GS screen offset). (inferred)
void BuildRichTextMatrix(struct S *p, float *m, float *v)
{
    func_002333B4(m, 0, 0x40);
    m[0] = v[0] * 0.8f;
    m[1] = v[1] * 0.454545454545f;
    m[4] = v[2] * 0.8f;
    m[5] = v[3] * 0.454545454545f;
    m[8] = v[4] * 0.8f + p->f64 + 786432.0f;
    m[9] = v[5] * 0.454545454545f + p->f68 + 786432.0f;
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
struct RT {char pad[0x6C];float offsetX,offsetY,width,height,sx,sy,lineHeight,fontScale;int vAlign,hAlign,priority;};
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
    matrix.a[0]=p->sx*p->fontScale;
    matrix.a[3]=p->sy*p->fontScale;
    matrix.a[4]=p->offsetX;
    matrix.a[5]=p->offsetY+p->lineHeight*p->sy;
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
struct RT {char pad[8];void *font;char padC[0x14];qword color0,color1;char pad40[0x20];int changed;float cameraX,cameraY,offsetX,offsetY,width,height,sx,sy,lineHeight,fontScale;int vAlign,hAlign,priority;};
struct DATA {unsigned int color;int mode;float priority;unsigned int limit;qword color0,color1;};

void BuildRichTextMatrix(struct RT *p, O *po, T *pt);
void DrawRichText2Glyphs(struct RT *p, O *po, struct DATA *q);
static inline void StoreX(struct RT *p,float value){register float x asm("$f2")=value;asm("swc1 %1,%0":"=m"(p->sx):"f"(x));}
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
    float x=t.a[0],y=t.a[3],g=p->fontScale,shift=p->lineHeight;
    p->sy=y; StoreX(p,x);
    t.a[0] = x*g;
    t.a[3] = y*g;
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
struct RT {char pad[8];void *font;char padc[0x14];qword color0,color1;char pad40[0x20];int changed;float cameraX,cameraY,offsetX,offsetY,width,height,sx,sy,lineHeight,fontScale;int vAlign,hAlign,priority;};
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
    float lineHeight=p->lineHeight*p->sy*0.45454547f;
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
        float extra=p->height*p->sy*0.45454547f-(line+1)*lineHeight;
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
                        float lineWidth=*width*p->sx*0.8f;
                        float extent=p->width*p->sx*0.8f;
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
                    matrix->a[8]+=GetFontGlyphWidth2(p->font,ch)*p->fontScale*p->sx*0.8f;
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
struct S { char pad[0x14]; struct G *ag; };
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
    return i ? &p->ag[i] : 0;
}
} } // namespace M_PglyffFromCh__8CFontBrxc

// 0x216B80 GetFontGlyphWidth2
namespace M_func_00216B80 { extern "C" {
struct S { char pad[0x1C]; float *ag; };
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
    return i ? p->ag[i - 1] : 0.0f;
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
typedef struct {int pad[2];void *owner;char padc[8];int kind,id;int color;float scale,width,height;int pad2c,pad30;} S;
typedef struct {char *text,*cur;void *font,*basefont,*altfont;RGBA color,basecolor,altcolor;char pad20[0x20];qword q40,q50;int changed;float x,y;int hAlign,vAlign;float width,height,sx,sy,dy,fontScale;int field8C,field90,field94;} RT;
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
extern "C" void *PchzFindSwLangstr__FP2SWi(WORLD *,int);
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
    if(p->kind==1){
        uint i;
        for(i=0;i<108;i++)if(D_003E1C88[i].id==p->id)break;
        if(i>=108)*(long *)text=*(const long *)D_002A8B80;
        else D_003E1C88[i].get(text,D_003E1C88[i].context,p->owner);
    }else if(p->kind==0){
        strcpy(text,(const char *)PchzFindSwLangstr__FP2SWi(&D_003DD490,p->id));
    }
    if(p->kind==1 && p->id==0x2EF){
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
    }else if(p->kind==1 && p->id==0x2F1 && D_003DA0C0.state==2){
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
struct CAMERA {char pad[0x9C];float x,y;};
struct OWNER {char pad[0xA4];int priority;char pada8[0x20];struct CAMERA *camera;};
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
    m.a[8]=t->x*0.8f+p->owner->camera->x+786432.0f;
    m.a[9]=t->y*0.45454547f+p->owner->camera->y+786432.0f;
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
struct CAMERA {char pad[0x9C];float x,y;};
struct OWNER {char pad[0xA4];int priority;char pada8[0x20];struct CAMERA *camera;};
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
    case 6:if(!p->geo)return;break;
    case 7:if(!p->count)return;break;
    default:return;
    }
    func_002333B4(&m,0,0x40);
    m.a[0]=t->a*0.8f;m.a[1]=t->b*0.45454547f;
    m.a[4]=t->c*0.8f;m.a[5]=t->d*0.45454547f;
    m.a[8]=t->x*0.8f+p->owner->camera->x+786432.0f;
    m.a[9]=t->y*0.45454547f+p->owner->camera->y+786432.0f;
    m.a[15]=m.a[10]=1.0f;
    data->mode=0;data->limit=0x7FFF;
    tag=D_002DEA28.cur;
    *(ulong *)tag=0x10000000UL;D_002DEA28.cur+=16;
    Unpack(&D_002DEA28,(const int *)&m,16,0x6C04C000);
    Unpack(&D_002DEA28,(const int *)data,12,0x6C03C004);
    if(p->type==6)Direct(&D_002DEA28,p->geo->count,p->geo->data);
    else Direct(&D_002DEA28,p->count,p->buffer);
    Align(&D_002DEA28);
    *(uint *)tag|=((D_002DEA28.cur-tag)>>4)-1;
    *(ulong *)D_002DEA28.cur=0x60000000UL;D_002DEA28.cur+=16;
    e=AllocRenderListEntry();
    e->kind=0x15;e->priority=data->priority+(p->owner->priority<<18);
    e->address=(uint)tag&0x0FFFFFFF;
    if(p->type==6)e->tag=p->tag;else e->tag=p->rawtag;
}
} } // namespace M_func_002177E0

// 0x217BA0 StartSpriteAnimById
namespace M_func_00217BA0 { extern "C" {
struct E { char pad[0x14]; int id; };
struct S { unsigned short c; short pad; struct E *a; char pad8[0x20]; int x28; };
void StartSpriteAnim(struct S *p, int i);
// 0x217B98: dead-code scrap (sly2/scrap.h)
SLY2_SCRAP("sd $zero, 0x10($4)");

/** @sly2-annot addr=0x217BA0
 * Purpose: find anim index by id (0x314 -> 0) and start
 * Info: proposed name StartSpriteAnimById (T2); TU hud
 * Confidence: low (draft) [T2 0.45] - evidence: static analysis; sources: r7c_hud
 * See: docs/reference/functions/hud-ui.md#hud-0x210eb0-0x21bb88
 * @end-sly2-annot */
// Starts the sprite animation with the given id (0x314 = the first). (inferred)
void StartSpriteAnimById(struct S *p, int k)
{
    int i;
    if (p->x28) {
        i = 0;
        if (k != 0x314) {
            for (i = 0; i < p->c; i++)
                if (k == p->a[i].id) break;
            if (i >= p->c) i = 0;
        }
        StartSpriteAnim(p, i);
    }
}
} } // namespace M_func_00217BA0

// 0x217C20 StartSpriteAnim
namespace M_func_00217C20 {
struct T { char pad[8]; short h8; };
struct S { char pad[8]; T *pt; char padc[0x1C]; int x28; };
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
extern "C" void StartSpriteAnim(S *p, int i)
{
    if (p->x28) {
        if (p->pt->h8 >= 0)
            StopSprite(p, i);
        RGBA c0(0x7F7F7F7F), c1(0xFFFFFFFF);
        StartSpriteAnimAt(p, i, 0, D_002A3A10, 0.0f, c0, c1);
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
struct S { unsigned short c; short pad; E *a; char pad8[8]; unsigned long ul10; float f18; int x1C; int x20; };
extern "C" void StartSpriteAnimAt(S *p, int i, int z, void *q, float f, RGBA c0, RGBA c1);
/** @sly2-annot addr=0x217CA8
 * Purpose: start anim k in a reuse slot
 * Info: proposed name StartSpriteAnimSlot (T2); TU hud
 * Confidence: low (draft) [T2 0.35] - evidence: static analysis; sources: r7c_hud
 * See: docs/reference/functions/hud-ui.md#hud-0x210eb0-0x21bb88
 * @end-sly2-annot */
// Starts animation k in a reusable sprite slot. (inferred)
extern "C" void StartSpriteAnimSlot(S *p, int k, int z, void *q, RGBA c0, RGBA c1)
{
    float t = 0.0f;
    int i;
    if (k == 0x314) {
        i = p->x20;
        if (i != -1) {
            p->x20 = -1;
        } else if (p->ul10 == (*(unsigned long *)&g_clock.tickFrame)) {
            t = p->f18;
            i = p->x1C;
        } else {
            p->f18 = t;
            i = 0;
            p->x1C = 0;
        }
    } else {
        for (i = 0; i < p->c; i++)
            if (p->a[i].id == k) break;
        if (i >= p->c) return;
    }
    StartSpriteAnimAt(p, i, z, q, t, c0, c1);
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
struct E { char pad[8]; short h8; short hA; float fC; int x10; void *x14; };
struct F { char pad[0x18]; };
struct S { int x0; F *af; E *ae; char padc[0x20]; M23 *pm; };
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
extern "C" void StartSpriteAnimAt(S *p, int i, int z, M23 *pm, float t, RGBA c0, RGBA c1)
{
    E *e = &p->ae[z];
    M23 m;
    e->h8 = i;
    e->x10 = 0;
    e->fC = t;
    e->hA = 0;
    M23 *pmUse;
    if (p->pm) {
        m = Mul(*pm, *p->pm);
        pmUse = &m;
    } else {
        pmUse = pm;
    }
    AdvanceSpriteAnim(t, &p->af[e->h8], z, pmUse, c0, c1);
}
} // namespace M_func_00217D98

// 0x217EF8 IFreeSpriteSlot
namespace M_IFreeSpriteSlot { extern "C" {
struct E { char pad[8]; short h; char pad2[14]; };
struct S { short a; unsigned short c; char pad[4]; struct E *ae; };
/** @sly2-annot addr=0x217EF8
 * Purpose: first reuse slot with h<0
 * Info: proposed name IFreeSpriteSlot (T1); TU hud
 * Confidence: mid (draft) [T1 0.50] - evidence: static analysis; sources: r7c_hud
 * See: docs/reference/functions/hud-ui.md#hud-0x210eb0-0x21bb88
 * @end-sly2-annot */
// First free reusable sprite slot.
int IFreeSpriteSlot(struct S *p)
{
    int i;
    int c = p->c;
    for (i = 0; i < c; i++) {
        if (p->ae[i].h < 0)
            return i;
    }
    return 0;
}
} } // namespace M_IFreeSpriteSlot

// 0x217F38 ResetSpriteSlotItems
namespace M_func_00217F38 { extern "C" {
struct Y { char pad[0x30]; void *x30; char pad2[0x3C]; };
struct F { int c; struct Y *ay; char pad[0x10]; };
struct S { int x0; int x4; struct F *af; };
void ReleaseSpriteItem(void *p, struct Y *y);
void InitSpriteItem(struct Y *y);
static inline void UpdateAy(int n, struct Y *ay)
{
    int k; for (k = 0; k < n; k++) { struct Y *y = &ay[k]; if (y->x30) ReleaseSpriteItem(y->x30, y); else InitSpriteItem(y); }
}
/** @sly2-annot addr=0x217F38
 * Purpose: reinit / release every item of slot i
 * Info: proposed name ResetSpriteSlotItems (T2); TU hud
 * Confidence: low (draft) [T2 0.45] - evidence: static analysis; sources: r7c_hud
 * See: docs/reference/functions/hud-ui.md#hud-0x210eb0-0x21bb88
 * @end-sly2-annot */
// Resets / releases every item of sprite slot i. (inferred)
void ResetSpriteSlotItems(struct S *p, int i)
{
    UpdateAy(p->af[i].c, p->af[i].ay);
}
} } // namespace M_func_00217F38

// 0x217FB8 SetSpriteDefaultPhase
namespace M_func_00217FB8 { extern "C" {
struct S { char pad[0x28]; int f; };
void SetSpriteAnimPhase(struct S *p, int a, int b);
/** @sly2-annot addr=0x217FB8
 * Purpose: phase seek of default anim
 * Info: proposed name SetSpriteDefaultPhase (T2); TU hud
 * Confidence: low (draft) [T2 0.40] - evidence: static analysis; sources: r7c_hud
 * See: docs/reference/functions/hud-ui.md#hud-0x210eb0-0x21bb88
 * @end-sly2-annot */
// Seeks the default animation by phase. (inferred)
void SetSpriteDefaultPhase(struct S *p)
{
    if (p->f)
        SetSpriteAnimPhase(p, 0, 0);
}
} } // namespace M_func_00217FB8

// 0x217FE0 SetSpriteAnimPhase
namespace M_func_00217FE0 { extern "C" {
struct E { char pad[8]; short h; char pad2[14]; };
struct F { char pad[8]; float f; char pad2[12]; };
struct S { int a; struct F *af; struct E *ae; };
void SetSpriteAnimTime(struct S *p, int i, int j, float f);
/** @sly2-annot addr=0x217FE0
 * Purpose: clamp g to [0,1], seek anim to g*duration
 * Info: proposed name SetSpriteAnimPhase (T2); TU hud
 * Confidence: low (draft) [T2 0.45] - evidence: static analysis; sources: r7c_hud
 * See: docs/reference/functions/hud-ui.md#hud-0x210eb0-0x21bb88
 * @end-sly2-annot */
// Seeks a sprite animation to a fraction (clamped to 0..1) of its duration. (inferred)
void SetSpriteAnimPhase(struct S *p, int i, int j, float f)
{
    int h;
    float c = 0.0f;
    float one = 1.0f;
    c = (f > c) ? f : c;
    c = (c < one) ? c : one;
    h = p->ae[i].h;
    if (h >= 0)
        SetSpriteAnimTime(p, i, j, p->af[h].f * c);
}
} } // namespace M_func_00217FE0

// 0x218038 SetSpriteDefaultTime
namespace M_func_00218038 { extern "C" {
struct S { char pad[0x28]; int f; };
void SetSpriteAnimTime(struct S *p, int a, int b);
/** @sly2-annot addr=0x218038
 * Purpose: time seek of default anim
 * Info: proposed name SetSpriteDefaultTime (T2); TU hud
 * Confidence: low (draft) [T2 0.35] - evidence: static analysis; sources: r7c_hud
 * See: docs/reference/functions/hud-ui.md#hud-0x210eb0-0x21bb88
 * @end-sly2-annot */
// Seeks the default animation by time. (inferred)
void SetSpriteDefaultTime(struct S *p)
{
    if (p->f)
        SetSpriteAnimTime(p, 0, 0);
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
struct Y { RGBA2 c0; RGBA2 c1; char pad8[8]; QQ q; };
struct E { int count;void *items;short h8;short hA;float fC;int x10;void *x14; };
struct F { char pad[8]; float f8; char padc[0xC]; };
struct S;
struct A { int x0; F *af; E *ae; char padc[0x18]; S *ps; char pad28[8]; };
struct S { char pad[0x84]; A *aa; };
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
extern "C" void SetSpriteAnimTime(A *p, int i, Y *py, float g)
{
    QQ q;
    RGBA c0, c1;
    int h = p->ae[i].h8;
    if (h < 0)
        return;
    g = GMin(p->af[h].f8, GMax(0.0f, g));
    if (py) {
        q = py->q;
        *(RGBA2 *)&c0 = py->c0;
        *(RGBA2 *)&c1 = py->c1;
    } else if (!FindSpriteItemPose(p->ps, p - p->ps->aa, &q, &c0, &c1, 0, 0)) {
        return;
    }
    ResetSpriteSlotItems(p, i);
    p->ae[i].fC = 0.0f;
    p->ae[i].hA = 0;
    p->ae[i].x10 = 0;
    // The primary argument object is copied again for the second call.
    RGBA argument0(c0),argument1(c1);
    UpdateSpriteSlot(0.0f, p, i, &q, &argument0, &argument1);
    p->ae[i].x10 = 0;
    argument0=c0;RGBA argument2(c1);
    UpdateSpriteSlot(g, p, i, &q, &argument0, &argument2);
    p->ae[i].x10 = 1;
}
} // namespace M_func_00218060

// 0x218220 AdvanceWidgetSprite
namespace M_func_00218220 {
typedef int qword __attribute__((mode(TI)));
struct RGBA { unsigned int u; RGBA(unsigned int x) {u=x;} } __attribute__((aligned(16)));
struct M23 {float a[6];} __attribute__((aligned(16)));
struct O {char pad[0xF4]; void *follow; M23 *pm;};
struct S {char pad[0x24]; O *owner; int enabled;};
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
extern "C" void AdvanceWidgetSprite(S *p,float dt) {
    if(p->enabled) {
        M23 m=D_002A3A10;
        if(p->owner->follow) {
            qword pos=GetLoWorldPos(p->owner->follow); qword q=ConvertCmWorldToScreen(&D_002DDF40,pos);
            m.a[4] = GetX(q)*320.0f+320.0f;
            m.a[5] = GetY(q)*-246.40001f+246.40001f;
        }
        if(p->owner->pm) m=Mul(m,*p->owner->pm);
        UpdateSpriteSlot(p,0,&m,dt,RGBA(0x7F7F7F7F),RGBA(0xFFFFFFFF));
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
struct E { char pad[8]; short h8; short hA; float fC; int x10; int x14; };
struct F {char pad[8];float duration;char padC[0xC];};
struct S { int x0; F *af; E *ae; char padc[0x20]; M23 *pm; };
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
extern "C" void UpdateSpriteSlot(S *p,int i,M23 *pm,float dt,RGBA c0,RGBA c1) {
    E *e=&p->ae[i];
    int h=e->h8;
    if(h<0)return;
    M23 m;
    M23 *pmUse;
    if(p->pm) {
        m=Mul(*pm,*p->pm);
        pmUse=&m;
    } else pmUse=pm;
    if(e->x10) {
        InterpolateSpriteSlot(dt,&p->af[h],i,pmUse,c0,c1);
    } else {
        if(p->af[h].duration <= e->fC) {
            e->fC=0.0f;
            e->hA=0;
        } else e->fC+=dt;
        AdvanceSpriteAnim(dt,&p->af[h],i,pmUse,c0,c1);
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
    int n;
    n = spriteGroup->n28;
    if (n != 0) {
        n = func_002185F8I(spriteGroup, 0, 0);
    }
    return n;
}

// 0x2185F8 DrawSpriteSlot
/** @sly2-annot addr=0x2185F8
 * Purpose: draw items of a slot
 * Info: proposed name DrawSpriteSlot (T2); TU hud
 * Confidence: low (draft) [T2 0.35] - evidence: static analysis; sources: r7c_hud
 * See: docs/reference/functions/hud-ui.md#hud-0x210eb0-0x21bb88
 * @end-sly2-annot */
/* Starts entry i's linked item (index at +8 of the 0x18-byte entry; negative for none). */
// Draws the items of a sprite slot. (inferred)
extern "C" void DrawSpriteSlot(SPRITEGRP *spriteGroup, int i, int n)
{
    short itemIndex;
    itemIndex = spriteGroup->spriteEntries[i].itemIndex;
    if (itemIndex >= 0) {
        DrawSpriteItems(&spriteGroup->spriteItems[itemIndex], i, n);
    }
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
    if (spriteGroup->n28 != 0) {
        StopSpriteSlot(spriteGroup, 0);
    }
}

// 0x218660 StopSpriteSlot
namespace M_func_00218660 { extern "C" {
struct E { char pad[8]; short h8; short pad2; float fC; int x10; void *x14; };
struct S { char pad[8]; struct E *ae; char padc[4]; unsigned long ul10; float f18; int x1C; int x20; };
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
void StopSpriteSlot(struct S *p, int i)
{
    struct E *e = (struct E*)(i*sizeof(struct E)+(int)p->ae);
    if (e->h8 >= 0) {
        float t=e->fC;
        p->x1C = e->h8;
        p->f18 = t;
        p->ul10 = (*(struct CLOCK *)&g_clock.tickFrame).ticks;
        p->x20 = -1;
        e->h8 = -1;
        e->x10 = 0;
        Release(&e->x14);
    }
    ResetSpriteSlotItems(p, i);
}
} } // namespace M_func_00218660

// 0x218710 FSpriteAnimActive
namespace M_FSpriteAnimActive { extern "C" {
struct B { char pad[8]; short h; char pad2[6]; int p; };
struct S { char pad[8]; struct B *pb; };
/** @sly2-annot addr=0x218710
 * Purpose: h!=0 && !done
 * Info: proposed name FSpriteAnimActive (T1); TU hud
 * Confidence: high [T1 0.85] - evidence: static analysis; sources: r7c_hud+verdicts_followups
 * See: docs/engine/hud-ui.md#5-key-functions, docs/reference/functions/hud-ui.md#hud-0x210eb0-0x21bb88
 * @end-sly2-annot */
// True while the sprite animation is running.
int FSpriteAnimActive(struct S *p)
{
    struct B *b = p->pb;
    if (b->h != 0 && b->p == 0)
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
    return spriteGroup->spriteEntries->n10;
}

// 0x218748 FinishSpriteSoundEvent
namespace M_func_00218748 { extern "C" {
struct E { char pad[0x10]; int f; void *p; };
struct S { char pad[8]; struct E *ae; };
void PauseSnd(void *p);
static inline struct E *Entries(const struct S *p){struct E *value;__asm__("lw %0,8(%1)":"=r"(value):"0"(p),"m"(p->ae));return value;}
static inline void *SoundRead(void *const *slot){register void *value __asm__("$3");__asm__("lw %0,%1":"=r"(value):"m"(*slot));return value;}
static inline struct E *EntryAt(struct E *entries,int index){register struct E *value asm("$2");register int bytes asm("$5")=index*sizeof(struct E);asm("addu %0,%1,%2":"=r"(value):"r"(bytes),"r"(entries));return value;}
/** @sly2-annot addr=0x218748
 * Purpose: mark entry done and stop/pause its sound
 * Info: proposed name FinishSpriteSoundEvent (T2); TU hud
 * Confidence: low (draft) [T2 0.30] - evidence: static analysis; sources: r7c_hud
 * See: docs/reference/functions/hud-ui.md#hud-0x210eb0-0x21bb88
 * @end-sly2-annot */
// Marks a sprite sound event done and stops or pauses its sound. (inferred)
void FinishSpriteSoundEvent(struct S *p, int i)
{
    void *q;
    p->ae[i].f = 1;
    q=SoundRead(&EntryAt(Entries(p),i)->p);
    if (q)
        PauseSnd(q);
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
struct E { char pad[7]; unsigned char c; unsigned char *pb; char padc[0x14]; };
struct S { int x0; struct E *ae; char pad8[8]; void *x10; };
void FinishSpriteSoundEvent(void *p, int k);
/** @sly2-annot addr=0x2190F8
 * Purpose: for action bytes ==7 call 0x218748
 * Info: proposed name FinishSpriteSoundEvents (T2); TU hud
 * Confidence: low (draft) [T2 0.30] - evidence: static analysis; sources: r7c_hud
 * See: docs/reference/functions/hud-ui.md#hud-0x210eb0-0x21bb88
 * @end-sly2-annot */
// Finishes every sound event (action 7) of a sprite key. (inferred)
void FinishSpriteSoundEvents(struct S *p, int i, int k)
{
    struct E *e = &p->ae[i];
    int j;
    for (j = 0; j < e->c; j++) {
        if (e->pb[j] == 7)
            FinishSpriteSoundEvent(p->x10, k);
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
struct Y { char pad[0x30]; void *x30; char pad2[0x3C]; };
struct F { int x0; struct Y *ay; char pad[0x10]; };
struct Q { char pad[8]; struct F *af; };
struct E { char pad[6]; unsigned char c; char pad7[0xD]; unsigned short *pi; char pad18[8]; };
struct S { int x0; struct E *ae; char pad8[8]; struct Q *pq; };
void ReleaseSpriteItem(void *p, struct Y *y);
void InitSpriteItem(struct Y *y);
/** @sly2-annot addr=0x219348
 * Purpose: reset items referenced by a key
 * Info: proposed name ResetSpriteKeyItems (T1); TU hud
 * Confidence: high [T1 0.85] - evidence: static analysis; sources: r7c_hud+verdicts_followups
 * See: docs/engine/hud-ui.md#5-key-functions, docs/reference/functions/hud-ui.md#hud-0x210eb0-0x21bb88
 * @end-sly2-annot */
// Resets the items a sprite key refers to.
void ResetSpriteKeyItems(struct S *p, int i, int j)
{
    struct E *e = &p->ae[i];
    struct Y *ay = p->pq->af[j].ay;
    int k;
    for (k = 0; k < e->c; k++) {
        struct Y *y = &ay[e->pi[k]];
        if (y->x30)
            ReleaseSpriteItem(y->x30, y);
        else
            InitSpriteItem(y);
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
struct Y { char pad[0x30]; void *x30; int x34; char pad2[0x38]; };
struct F { int c; struct Y *ay; char pad[0x10]; };
struct Q { char pad[8]; struct F *af; };
struct S { char pad[0xC]; char *pc; struct Q *pq; };
void QueueSpriteItem(void *p, struct Y *y, int a);
void PrependSlEntry(void *p, struct Y *y);
static inline void Draw(struct S *p, int n, struct Y *ay, int a)
{
    int k;
    for (k = n - 1; k >= 0; k--) {
        struct Y *y = &ay[k];
        if (y->x30)
            QueueSpriteItem(y->x30, y, a);
        else if (y->x34 == 4)
            PrependSlEntry(p->pc + 0xFC, y);
    }
}
/** @sly2-annot addr=0x21A998
 * Purpose: queue items in reverse order
 * Info: proposed name DrawSpriteItems (T2); TU hud
 * Confidence: low (draft) [T2 0.40] - evidence: static analysis; sources: r7c_hud
 * See: docs/reference/functions/hud-ui.md#hud-0x210eb0-0x21bb88
 * @end-sly2-annot */
// Queues a sprite's items for drawing in reverse order. (inferred)
void DrawSpriteItems(struct S *p, int i, int a)
{
    Draw(p, p->pq->af[i].c, p->pq->af[i].ay, a);
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
