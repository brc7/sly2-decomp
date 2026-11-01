// letterbox.cpp -- cinematic letterbox bars, driven by a world TIMER object. TU 0x1A3020..0x1A3260 (6 functions).
// SLY2_ALIAS: __static_initialization_and_destruction_0=StaticInit_letterbox
// SLY2_ALIAS: _GLOBAL_$I$*=_GLOBAL_$I$letterbox
#include "sly2/lo.h"
#include "sly2/sw.h"
#include "sly2/slotheap.h"

/* TIMER adds virtuals after LO's; the letterbox calls vtable slots 49 and 50 (VUnkNN = slot NN+1, the sly2/*.h
 * convention). HYPOTHESIS: TIMER : LO (as in Sly 1) and the slot meanings; the slot numbers are CONFIRMED by match. */
struct TIMER : public LO
{
    virtual void VUnk48(int n);     /* slot 49: letterbox passes -1 when showing */
    virtual void VUnk49();          /* slot 50: hide */
};
enum TIMERS { TIMERS_Nil = 0, TIMERS_Running = 1 };   /* HYPOTHESIS: value 1 starts the timer */

struct LETTERBOX { TIMER *ptimer; };
struct SPEAKERLABEL;

extern SW g_sw;                     /* 0x3DD490 */
extern SPEAKERLABEL g_speakerlabel; /* 0x4435F0 (HYPOTHESIS name) */
LO *PloFindSwObject(SW *psw, int grffso, OID oid, LO *ploContext);
void SetTimerTimers(TIMER *ptimer, TIMERS timers);
void InitSpeakerLabel(SPEAKERLABEL *pspeakerlabel);

void InitLetterboxTimer(LETTERBOX *pletterbox)
{
    TIMER *ptimer = (TIMER *)PloFindSwObject(&g_sw, 0x405, (OID)0x373, 0);
    pletterbox->ptimer = ptimer;
    if (ptimer) SetTimerTimers(ptimer, TIMERS_Running);
}

void ClearLetterbox(LETTERBOX *pletterbox)
{
    pletterbox->ptimer = 0;
}

void ShowLetterbox(LETTERBOX *pletterbox)
{
    if (pletterbox->ptimer) {
        InitSpeakerLabel(&g_speakerlabel);
        pletterbox->ptimer->VUnk48(-1);
    }
}

void HideLetterbox(LETTERBOX *pletterbox)
{
    TIMER *ptimer = pletterbox->ptimer;
    if (ptimer) ptimer->VUnk49();
}

SLY2_INSTANTIATE_SLOT_POOLS()
