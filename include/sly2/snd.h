/* include/sly2/snd.h -- the sound instances (engine/sound/sound.cpp): CID151 (the registry's SND class, 0x60 bytes) and
 * its two kinds, with their virtual layouts from the ELF vtables (0x2D9D00, 0x2D9C70, 0x2D9BE0; 16 slots: BASIC's 6
 * and 10 of CID151's own, slots 8..15 pure in CID151). sound.cpp is the key TU of all three. Each slot is declared as a
 * member whose asm label is the function the vtable holds there (as sly2/classes.h does).
 * DRAFT names: BANKSND and STREAMSND after their functions (IncrefBanksnd, StartStreamsnd...), the members VSlot<n>
 * except Start (StartSnd). The classes are in emission order (sound.cpp emits STREAMSND, BANKSND, CID151). */
#ifndef SLY2_SND_H
#define SLY2_SND_H
#include "sly2/basic.h"

/* CID151: a sound instance (the registry's SND class; placeholder name): base of bank and stream sounds. */
struct CID151 : BASIC
{
    char pad10[0x60 - 0x10];
    virtual void Start() __asm__("StartSnd");   /* slot 7 (the first new virtual); key function (sound.cpp) */
    virtual CLASSINFO *GetClassInfo() __asm__("GetCid151ClassDesc");   /* slot 6 */
    virtual void GetKind() __asm__("__pure_virtual");   /* slot 8 (pure) */
    virtual void Play() __asm__("__pure_virtual");   /* slot 9 (pure) */
    virtual void Stop() __asm__("__pure_virtual");   /* slot 10 (pure) */
    virtual void Pause() __asm__("__pure_virtual");   /* slot 11 (pure) */
    virtual void Resume() __asm__("__pure_virtual");   /* slot 12 (pure) */
    virtual void ApplyParams() __asm__("__pure_virtual");   /* slot 13 (pure) */
    virtual void QueryStatus() __asm__("__pure_virtual");   /* slot 14 (pure) */
    virtual void SetCulled() __asm__("__pure_virtual");   /* slot 15 (pure) */
    virtual void UpdatePositional() __asm__("UpdateSndPositional");   /* slot 16 */
};
typedef char assert_CID151_size[sizeof(CID151) == 0x60 ? 1 : -1];

/* BANKSND: a sound played from a loaded sound bank (DRAFT name). */
struct BANKSND : CID151
{
    virtual void AddRef() __asm__("IncrefBanksnd");   /* slot 1; key function (sound.cpp) */
    virtual void Release() __asm__("DecrefBanksnd");   /* slot 2 */
    virtual void GetKind() __asm__("GetBanksndKind");   /* slot 8 */
    virtual void Play() __asm__("PlayBanksnd");   /* slot 9 */
    virtual void Stop() __asm__("StopBanksnd");   /* slot 10 */
    virtual void Pause() __asm__("PauseBanksnd");   /* slot 11 */
    virtual void Resume() __asm__("ContinueBanksnd");   /* slot 12 */
    virtual void ApplyParams() __asm__("ApplyBanksndParams");   /* slot 13 */
    virtual void QueryStatus() __asm__("BANKSND_QueryStatus");   /* slot 14 */
    virtual void SetCulled() __asm__("SetBankSoundCulled");   /* slot 15 */
};

/* STREAMSND: a sound streamed from disc (DRAFT name). */
struct STREAMSND : CID151
{
    virtual void AddRef() __asm__("IncrefStreamsnd");   /* slot 1; key function (sound.cpp) */
    virtual void Release() __asm__("DecrefStreamsnd");   /* slot 2 */
    virtual void Start() __asm__("StartStreamsnd");   /* slot 7 */
    virtual void GetKind() __asm__("GetStreamSoundKind");   /* slot 8 */
    virtual void Play() __asm__("PlayStreamsnd");   /* slot 9 */
    virtual void Stop() __asm__("StopStreamSound");   /* slot 10 */
    virtual void Pause() __asm__("PauseStreamSound");   /* slot 11 */
    virtual void Resume() __asm__("ResumeStreamSound");   /* slot 12 */
    virtual void ApplyParams() __asm__("ApplyStreamsndParams");   /* slot 13 */
    virtual void QueryStatus() __asm__("QueryStreamSoundStatus");   /* slot 14 */
    virtual void SetCulled() __asm__("SetStreamSoundCulled");   /* slot 15 */
    virtual void UpdatePositional() __asm__("UpdateStreamsndPositional");   /* slot 16 */
};

#endif
