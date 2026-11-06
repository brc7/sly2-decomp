/* include/sly2/screen.h -- SCREEN, the base of the UI screens and dialogs (engine/presentation/screen.cpp), with its
 * virtual layout from its ELF vtable (0x2D85E8, 14 slots: BASIC's 6 and 8 of its own; slot 14 pure). Each slot is
 * declared as a member whose asm label is the function the vtable holds there (as sly2/classes.h does).
 * DRAFT names: SCREEN is screen.cpp's view name, the members are VSlot<n> except where a function name says more;
 * the fields are those screen.cpp's and memcard.cpp's bodies read. screen.cpp still uses its own view (its vtable is
 * not emitted yet); memcard.cpp's CMcStatus derives from this class. Since data/address-names-2: SCREEN derives from
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
    virtual void Update() __asm__("func_001D9870");   /* slot 7 (empty); key function (screen.cpp) */
    virtual void VSlot8() __asm__("DrawScreen");   /* slot 8 */
    virtual void VSlot9() __asm__("ScreenSlot9Default");   /* slot 9 */
    virtual void VSlot10() __asm__("ShowScreen");   /* slot 10 */
    virtual void VSlot11() __asm__("HideScreen");   /* slot 11 */
};

/* The HUD meter (DRAFT name; vtable 0x2D8668, screen.cpp). */
struct HUDMETER : SCREENBASE
{
    virtual void Update() __asm__("UpdateHudMeter");   /* slot 7; key function (screen.cpp) */
};

struct SCREEN : SCREENBASE
{
    char pad10[0x18 - 0x10];
    int idText;             /* +0x18 */
    char pad1C[4];
    int idTextQueued;       /* +0x20; for widget screens the widget oid */
    void *pchild;           /* +0x24 the widget object */
    int nMode;              /* +0x28 */
    float tMode;            /* +0x2C */
    SCREEN() __asm__("ConstructScreen");        /* out of line: screen.cpp */
    void ResetMode() __asm__("ResetScreenMode");   /* mode -1, time now (screen.cpp; DRAFT name ResetScreenMode) */
    virtual void PostLoad() __asm__("func_001DA770");   /* slot 12 (screen.cpp: resolves the widget); key function (screen.cpp) */
    virtual void Update() __asm__("UpdateTimedScreen");   /* slot 7 */
    virtual void VSlot13() __asm__("ResetScreen");   /* slot 13 */
    virtual void SetMode(int nMode) __asm__("__pure_virtual");   /* slot 14 (pure) */
};

#endif
