/* include/sly2/screen.h -- SCREEN, the base of the UI screens and dialogs (engine/presentation/screen.cpp), with its
 * virtual layout from its ELF vtable (0x2D85E8, 14 slots: BASIC's 6 and 8 of its own; slot 14 pure). Each slot is
 * declared as a member whose asm label is the function the vtable holds there (as sly2/classes.h does).
 * DRAFT names: SCREEN is screen.cpp's view name, the members are VSlot<n> except where a function name says more;
 * the fields are those screen.cpp's and memcard.cpp's bodies read (screen.cpp's file-scope bodies use SCREEN and
 * HUDMETER; its per-function views are not folded yet); memcard.cpp's CMcStatus derives from this class. Since data/address-names-2: SCREEN derives from
 * SCREENBASE (the 11-slot base the HUD meter and the pause menu share) and screen.cpp emits SCREENBASE's, HUDMETER's,
 * SCREEN's and its screens' vtables (their classes are in screen.cpp, after this header). */
#ifndef SLY2_SCREEN_H
#define SLY2_SCREEN_H
#include "sly2/basic.h"

/* A derived class's constructor makes g++ instantiate BASIC's CRefPtr member destructors (cleanup of a partly built
   object), whose pointees are incomplete here: declared as explicit specializations, as sly2/classes.h does. */
template <> CRefPtr<CFrame>::~CRefPtr();
template <> CRefPtr<CSidebag>::~CRefPtr();

/* The base of the screens and HUD elements (DRAFT name SCREENBASE): BASIC plus five slots, vtable 0x2D86D0 (11 slots,
 * screen.cpp). The HUD meter and the pause menu derive from it directly, SCREEN adds slots 12..14. */
struct SCREENBASE : BASIC
{
    JOY *pjoy;              /* +0x10 the joypad it reads (only while focused: UpdateScreenFocus) */
    virtual void Update() __asm__("UpdateScreenDefault");   /* slot 7 (empty); key function (screen.cpp) */
    virtual void Draw() __asm__("DrawScreen");   /* slot 8 */
    virtual void SetFocus(int fFocus, SCREENBASE *pscrOther) __asm__("ScreenSlot9Default");   /* slot 9: focus gained / lost (UpdateScreenFocus) */
    virtual void Show() __asm__("ShowScreen");   /* slot 10 */
    virtual void Hide() __asm__("HideScreen");   /* slot 11 */
};

/* The HUD meter (DRAFT name; vtable 0x2D8668, screen.cpp). */
struct HUDMETER : SCREENBASE
{
    int unknown14[3];       /* +0x14 */
    float shownHealth;      /* +0x20 the fractions shown (they follow health / power) */
    float health;           /* +0x24 */
    float shownPower;       /* +0x28 */
    float power;            /* +0x2C */
    int state;              /* +0x30 */
    STEPPLAYER *hero;       /* +0x34 */
    WIDGET *widget;         /* +0x38 */
    int idAnimSteady;       /* +0x3C the per-character animations: steady, damage flash, hide, the alternate */
    int idAnimDamage;       /* +0x40    steady states 6 and 7 (SetHudMeterState) */
    int idAnimHide;         /* +0x44 */
    int idAnimState6;       /* +0x48 */
    int idAnimState7;       /* +0x4C */
    virtual void Update() __asm__("UpdateHudMeter");   /* slot 7; key function (screen.cpp) */
};
typedef char assert_HUDMETER_size[sizeof(HUDMETER) == 0x50 ? 1 : -1];

/* SCREEN: a UI screen or dialog: the text it shows, the widget object and a mode with its entry time. */
struct SCREEN : SCREENBASE
{
    int unknown14;          /* +0x14 */
    int idText;             /* +0x18 id of the text shown */
    int unknown1C;          /* +0x1C */
    int idTextQueued;       /* +0x20 text to show next; for widget screens the widget oid */
    void *pchild;           /* +0x24 the widget object (WIDGET; the bodies read it through their WIDGET views) */
    int nMode;              /* +0x28 screen mode (-1 after ResetMode); the stick direction of the menus (slot 14) */
    float modeStartTime;    /* +0x2C time the mode was entered */
    int nPopup;             /* +0x30 the message popup (SetScreenMode): -1 none, 0 a prompt, 1 a timed message */
    float tPopupEnd;        /* +0x34 when the popup closes (UpdateTimedScreen) */
    int idsPopup;           /* +0x38 the popup's text id */
    int unknown3C;          /* +0x3C */
    SCREEN() __asm__("ConstructScreen");        /* out of line: screen.cpp */
    void ResetMode() __asm__("ResetScreenMode");   /* mode -1, time now (screen.cpp; DRAFT name ResetScreenMode) */
    virtual void PostLoad() __asm__("PostLoadScreen");   /* slot 12 (screen.cpp: resolves the widget); key function (screen.cpp) */
    virtual void Update() __asm__("UpdateTimedScreen");   /* slot 7 */
    virtual void Reset() __asm__("ResetScreen");   /* slot 13 */
    virtual void SetMode(int nMode) __asm__("__pure_virtual");   /* slot 14 (pure) */
};
typedef char assert_SCREEN_size[sizeof(SCREEN) == 0x40 ? 1 : -1];

#endif
