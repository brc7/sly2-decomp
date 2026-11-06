/* include/sly2/snd.h -- the sound instances (engine/sound/sound.cpp): CID151 (the registry's SND class, 0x60 bytes) and
 * its two kinds, with their virtual layouts from the ELF vtables (0x2D9D00, 0x2D9C70, 0x2D9BE0; 16 slots: BASIC's 6
 * and 10 of CID151's own, slots 8..15 pure in CID151). sound.cpp is the key TU of all three. Each slot is declared as a
 * member whose asm label is the function the vtable holds there (as sly2/classes.h does).
 * DRAFT names: BANKSND and STREAMSND after their functions (IncrefBanksnd, StartStreamsnd...), the members VSlot<n>
 * except Start (StartSnd). The classes are in emission order (sound.cpp emits STREAMSND, BANKSND, CID151). */
#ifndef SLY2_SND_H
#define SLY2_SND_H
#include "sly2/basic.h"

struct CID151 : BASIC
{
    char pad10[0x60 - 0x10];
    virtual void Start() __asm__("StartSnd");   /* slot 7 (the first new virtual); key function (sound.cpp) */
    virtual CLASSINFO *Pclassinfo() __asm__("GetCid151ClassDesc");   /* slot 6 */
    virtual void VSlot8() __asm__("__pure_virtual");   /* slot 8 (pure) */
    virtual void VSlot9() __asm__("__pure_virtual");   /* slot 9 (pure) */
    virtual void VSlot10() __asm__("__pure_virtual");   /* slot 10 (pure) */
    virtual void VSlot11() __asm__("__pure_virtual");   /* slot 11 (pure) */
    virtual void VSlot12() __asm__("__pure_virtual");   /* slot 12 (pure) */
    virtual void VSlot13() __asm__("__pure_virtual");   /* slot 13 (pure) */
    virtual void VSlot14() __asm__("__pure_virtual");   /* slot 14 (pure) */
    virtual void VSlot15() __asm__("__pure_virtual");   /* slot 15 (pure) */
    virtual void VSlot16() __asm__("UpdateSndPositional");   /* slot 16 */
};
typedef char assert_CID151_size[sizeof(CID151) == 0x60 ? 1 : -1];

struct BANKSND : CID151
{
    virtual void VUnk00() __asm__("IncrefBanksnd");   /* slot 1; key function (sound.cpp) */
    virtual void VUnk01() __asm__("DecrefBanksnd");   /* slot 2 */
    virtual void VSlot8() __asm__("GetBanksndKind");   /* slot 8 */
    virtual void VSlot9() __asm__("PlayBanksnd");   /* slot 9 */
    virtual void VSlot10() __asm__("StopBanksnd");   /* slot 10 */
    virtual void VSlot11() __asm__("PauseBanksnd");   /* slot 11 */
    virtual void VSlot12() __asm__("ContinueBanksnd");   /* slot 12 */
    virtual void VSlot13() __asm__("ApplyBanksndParams");   /* slot 13 */
    virtual void VSlot14() __asm__("BANKSND_QueryStatus");   /* slot 14 */
    virtual void VSlot15() __asm__("SetBankSoundCulled");   /* slot 15 */
};

struct STREAMSND : CID151
{
    virtual void VUnk00() __asm__("IncrefStreamsnd");   /* slot 1; key function (sound.cpp) */
    virtual void VUnk01() __asm__("DecrefStreamsnd");   /* slot 2 */
    virtual void Start() __asm__("StartStreamsnd");   /* slot 7 */
    virtual void VSlot8() __asm__("GetStreamSoundKind");   /* slot 8 */
    virtual void VSlot9() __asm__("PlayStreamsnd");   /* slot 9 */
    virtual void VSlot10() __asm__("StopStreamSound");   /* slot 10 */
    virtual void VSlot11() __asm__("PauseStreamSound");   /* slot 11 */
    virtual void VSlot12() __asm__("ResumeStreamSound");   /* slot 12 */
    virtual void VSlot13() __asm__("ApplyStreamsndParams");   /* slot 13 */
    virtual void VSlot14() __asm__("QueryStreamSoundStatus");   /* slot 14 */
    virtual void VSlot15() __asm__("SetStreamSoundCulled");   /* slot 15 */
    virtual void VSlot16() __asm__("UpdateStreamsndPositional");   /* slot 16 */
};

#endif
