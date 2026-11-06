/* include/sly2/screen.h -- SCREEN, the base of the UI screens and dialogs (engine/presentation/screen.cpp), with its
 * virtual layout from its ELF vtable (0x2D85E8, 14 slots: BASIC's 6 and 8 of its own; slot 14 pure). Each slot is
 * declared as a member whose asm label is the function the vtable holds there (as sly2/classes.h does).
 * DRAFT names: SCREEN is screen.cpp's view name, the members are VSlot<n> except where a function name says more;
 * the fields are those screen.cpp's and memcard.cpp's bodies read. screen.cpp still uses its own view (its vtable is
 * not emitted yet); memcard.cpp's CMcStatus derives from this class. */
#ifndef SLY2_SCREEN_H
#define SLY2_SCREEN_H
#include "sly2/basic.h"

/* A derived class's constructor makes g++ instantiate BASIC's CRefPtr member destructors (cleanup of a partly built
   object), whose pointees are incomplete here: declared as explicit specializations, as sly2/classes.h does. */
template <> CRefPtr<CFrame>::~CRefPtr();
template <> CRefPtr<CSidebag>::~CRefPtr();

struct SCREEN : BASIC
{
    char pad10[0x18 - 0x10];
    int idText;             /* +0x18 */
    char pad1C[4];
    int idTextQueued;       /* +0x20; for widget screens the widget oid */
    void *pchild;           /* +0x24 the widget object */
    int nMode;              /* +0x28 */
    float tMode;            /* +0x2C */
    SCREEN() __asm__("func_001DA728");        /* out of line: screen.cpp */
    void ResetMode() __asm__("func_001DA7D0");   /* mode -1, time now (screen.cpp; DRAFT name ResetScreenMode) */
    virtual void Update() __asm__("func_001DA808");   /* slot 7 */
    virtual void VSlot8() __asm__("func_001D9878");   /* slot 8 */
    virtual void VSlot9() __asm__("func_001D98A8");   /* slot 9 */
    virtual void VSlot10() __asm__("func_001D98B0");   /* slot 10 */
    virtual void VSlot11() __asm__("func_001D9910");   /* slot 11 */
    virtual void PostLoad() __asm__("func_001DA770");   /* slot 12 (screen.cpp: resolves the widget) */
    virtual void VSlot13() __asm__("func_001DA7B0");   /* slot 13 */
    virtual void SetMode(int nMode) __asm__("func_0023C820");   /* slot 14 (pure) */
};

#endif
