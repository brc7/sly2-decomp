/* include/sly2/sndmgr.h -- the sound manager (SNDMGR, g_sndmgr at D_003DD040, engine/sound/sound.cpp) and its file
 * lists (SNDSTREAM, SNDFILE). Layout from sound.cpp's per-function views (they agree on every offset) and the
 * views of menu.cpp (the options menu's volumes). Separate from sly2/snd.h so that its many includers do not get
 * sly2/slotheap.h. DRAFT names (the m2c bodies' and the views' names). */
#ifndef SLY2_SNDMGR_H
#define SLY2_SNDMGR_H
#include "sly2/classes.h"   /* SNDMGR */
#include "sly2/snd.h"
#include "sly2/slotheap.h"
#include "sly2/sw.h"

/* FILEREC: sly2/sw.h's (a file's record in the disc's file table: 16 words; the lookup key words are aw[0], aw[1],
   aw[13] and aw[15], see sound.cpp's BuildStreamFileKey). */

/* DIALOGEVENT: a lip-sync / gesture event of a dialog line (frames at 60 per second since the previous event; the role
   it drives and the value: a state for the speaker's state machines or a gesture weight). */
struct DIALOGEVENT
{
    short framesAfterPrevious;  /* +0x0 */
    short role;                 /* +0x2 */
    union
    {
        int smState;
        float gesture;
    } value;                    /* +0x4 */
};

/* SNDSTREAM: a dialog line. */
struct SNDSTREAM
{
    FILEREC record;            /* +0x00 the current language's file */
    int id;                    /* +0x40 */
    SNDSTREAM *next;           /* +0x44 */
    int speakerId;             /* +0x48 the speaking object's id (StartDialog; -1: none) and the subtitle's speaker */
    int excludeFromHeadset;    /* +0x4C PlayStreamsnd: 0 = plays through the headset when it is enabled (inferred) */
    float duration;            /* +0x50 seconds (subtitles, UpdateDialog) */
    int numEvents;             /* +0x54 */
    DIALOGEVENT *events;       /* +0x58 lip-sync / gesture events */
};

/* SNDFILE: a music track. */
struct SNDFILE
{
    FILEREC record;            /* +0x00 */
    int id;                    /* +0x40 */
    SNDFILE *next;             /* +0x44 */
};

/* SNDMGR::volumes: the channels (GetStreamVolumeChannel, SetSndmgrChannelVolume) and categories (rows: 1 the options
   menu, 2 the pause fade, which pauses a channel's sounds at 0 and which UpdateSndmgr fades while game time is
   stopped). Macros, not enums: an enum constant passed to a function declared per namespace can make the call
   ambiguous for g++ 2.95. */
#define VOLUME_CHANNEL_AMBIENCE 0
#define VOLUME_CHANNEL_MUSIC 1
#define VOLUME_CHANNEL_DIALOG 2
#define VOLUME_CHANNEL_EFFECTS 3
#define VOLUME_CATEGORY_MENU 1
#define VOLUME_CATEGORY_PAUSE_FADE 2

/* SNDMGR itself is defined in sly2/classes.h, at its place in class_registry's vtable emission order. */
/* g_sndmgr, defined by sound.cpp (as its construct-and-destruct form, the whole SNDMGR: the dialog fields at +0x22C
   are read through SNDMGR pointers, so they are members; see sound.cpp's SNDMGRG). */
extern SNDMGR g_sndmgr __asm__("D_003DD040");

typedef char assert_SNDMGR_dirty[(int)&((SNDMGR *)0)->dirtySounds == 0x4C ? 1 : -1];
typedef char assert_SNDMGR_bank[(int)&((SNDMGR *)0)->bank == 0xE8 ? 1 : -1];
typedef char assert_SNDMGR_smooth[(int)&((SNDMGR *)0)->sfxVariationStep == 0x138 ? 1 : -1];
typedef char assert_SNDMGR_tick[(int)&((SNDMGR *)0)->tickNow == 0x1F0 ? 1 : -1];
typedef char assert_SNDMGR_dialog[(int)&((SNDMGR *)0)->dialogSound == 0x200 ? 1 : -1];

#endif
