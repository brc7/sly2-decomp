/* include/sly2/leap.h -- LEAP and CID146, the BASIC-derived helper objects that STEP subclasses embed (MURRAY +0x1890,
 * STEPGUARD +0xDB0 / +0xE20), from app/class_registry.cpp's construction views with the real virtual layout from
 * the ELF vtables (types/views-2). Its own header, included by guard.h and players.h only: player/tu_20B840.cpp
 * (which includes stepplayer.h) still has its own field-level LEAP view. */
#ifndef SLY2_LEAP_H
#define SLY2_LEAP_H
#include "sly2/basic.h"

/* LEAP (cid 145): the leap helper object STEP subclasses embed (MURRAY +0x1890, STEPGUARD +0xDB0). From
   app/class_registry.cpp's construction view; real virtual layout from the ELF vtable.
   145 LEAP (0x70); vtable 0x2DAA10, 6 slots */
struct LEAP : BASIC
{
    char pad10[0x70 - 0x10];
    LEAP() __asm__("ConstructLeap");              /* out of line: player/tu_20B840.cpp */
    virtual void HandleMessage(MSGID msgid, void *pv) __asm__("HandleLeapMessage");   /* slot 5; key function (tu_20B840) */
    virtual CLASSINFO *GetClassInfo() __asm__("GetLeapClassDesc");   /* slot 6 */
};
typedef char assert_LEAP_size[sizeof(LEAP) == 0x70 ? 1 : -1];
/* CID146 (cid 146, DIRANIM): embedded in STEPGUARD at +0xE20. From app/class_registry.cpp.
   146 CID146 (0x40): DIRANIM; vtable 0x2DA9D0, 6 slots */
struct CID146 : BASIC
{
    char pad10[0x40 - 0x10];
    virtual void HandleMessage(MSGID msgid, void *pv) __asm__("HandleCid146Message");   /* slot 5 */
    virtual CLASSINFO *GetClassInfo() __asm__("GetCid146ClassDesc");   /* slot 6 */
};
typedef char assert_CID146_size[sizeof(CID146) == 0x40 ? 1 : -1];

#endif /* SLY2_LEAP_H */
