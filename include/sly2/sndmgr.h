/* include/sly2/sndmgr.h -- the sound manager (SNDMGR, g_sndmgr at D_003DD040, engine/sound/sound.cpp) and its file
 * lists (SNDSTREAM, SNDFILE). Layout from sound.cpp's per-function views (they agree on every offset) and the
 * views of menu.cpp (the options menu's volumes). Separate from sly2/snd.h so that its many includers do not get
 * sly2/slotheap.h. DRAFT names (the m2c bodies' and the views' names). */
#ifndef SLY2_SNDMGR_H
#define SLY2_SNDMGR_H
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

/* SNDMGR: the sound manager (g_sndmgr, D_003DD040, defined by sound.cpp; 0x250 bytes). A BASIC (constructed by
   SNDMGR_ctor; the first 0x10 bytes are BASIC's words and vtable), declared here without the C++ base: the field views
   read it by offset and sound.cpp's definition has its own construct-and-destruct form.
   Volume channels: 0 ambience, 1 music, 2 dialog, 3 bank sound effects (the menu's effects slider sets 0 and 3).
   Volume categories (rows of volumes): 1 the options menu, 2 the pause fade (0 pauses the channel's sounds), 3 a player
   mode's ducking (mode.cpp: 0.6); the rest are 1. */
struct SNDMGR
{
    int abasic[4];               /* +0x000 BASIC (frame, side bag, subscribers, vtable) */
    int isHeadsetEnabled;        /* +0x010 the "use headset" option (save.cpp loads it; menu.cpp shows it) */
    int unknown14;               /* +0x014 cleared by ResetSndmgr only */
    int numSfx;                  /* +0x018 size of the sfx id -> sound index table (ResetSndmgr: -1) */
    SLOTHEAP bankSoundPool;      /* +0x01C */
    SLOTHEAP streamSoundPool;    /* +0x034 */
    DL dirtySounds;              /* +0x04C the sounds with pending requests (UpdateSndmgr flushes them) */
    int isPaused;                /* +0x058 set while the disc is busy (UpdateSndmgr pauses the music / ambience / dialog) */
    int musicLayer2Requests;     /* +0x05C counted requests */
    int musicLayer0Requests;     /* +0x060 */
    int selectedMusicLayer;      /* +0x064 0..2 */
    int muteWhenStoppedFlags;    /* +0x068 bit 1: cleared by a player mode, bit 2: during cutscenes; when both are set (3,
                                    the default) the ambience and music fade out while game time is stopped */
    int isMicLevelQueryBusy;     /* +0x06C */
    float micLevel;              /* +0x070 headset microphone level from the IOP (SUKR command 0xB) */
    int micNoise[3];             /* +0x074 the NOISE the microphone makes for guards (ReplaceHeldNoise, noise.cpp) */
    CLQ micNoiseRangeCurve;      /* +0x080 microphone level -> noise range */
    CLQ micNoiseHeightCurve;     /* +0x090 microphone level -> noise height */
    int isStatusQueryBusy;       /* +0x0A0 */
    int statusResult;            /* +0x0A4 SUKR command 0xC; meaning unknown */
    FILEREC bankRecord;          /* +0x0A8 */
    int bank;                    /* +0x0E8 989snd bank handle */
    SNDSTREAM *firstDialogStream; /* +0x0EC dialog lines */
    SNDFILE *firstMusicFile;     /* +0x0F0 music tracks (PushMusic / FindMusic create kind-4 STREAMSNDs from them) */
    FILEREC *ambienceRecord;     /* +0x0F4 */
    int numSounds;               /* +0x0F8 */
    int *soundKinds;             /* +0x0FC per sound: 2 = looping */
    int *soundIndexFromSfx;      /* +0x100 */
    int playbackMode;            /* +0x104 snd_SetPlaybackMode */
    float musicLayerWeights[3];  /* +0x108 */
    SMP musicLayerSmoothing[3];  /* +0x114 */
    int sfxVariationStep;        /* +0x138 the random walk of StepRandomSfxVariation (-12..12) */
    float volumes[8][4];         /* +0x13C category x channel */
    float masterVolume;          /* +0x1BC */
    float channelVolumes[4];     /* +0x1C0 the product of the volumes of each channel */
    int numAmbienceGroups;       /* +0x1D0 */
    int ambienceGroupIds[6];     /* +0x1D4 */
    int pad1EC;
    long tickNow;                /* +0x1F0 this update's tick */
    long tickLast;               /* +0x1F8 the last update's tick */
    SNDREF dialogSound;          /* +0x200 counted pointers (released by g_sndmgr's destructor) */
    int musicStackCount;         /* +0x204 */
    SNDREF musicStack[8];        /* +0x208 */
    SNDREF ambienceSound;        /* +0x228 */
    BASIC *dialogRoleObjects[6]; /* +0x22C per lip-sync role (3: the speaker object; the others its state machines) */
    int dialogEventIndex;        /* +0x244 */
    float dialogEventTime;       /* +0x248 */
    float dialogElapsed;         /* +0x24C */
};
/* g_sndmgr, defined by sound.cpp (as its construct-and-destruct form). */
extern SNDMGR g_sndmgr __asm__("D_003DD040");

typedef char assert_SNDMGR_size[sizeof(SNDMGR) == 0x250 ? 1 : -1];
typedef char assert_SNDMGR_dirty[(int)&((SNDMGR *)0)->dirtySounds == 0x4C ? 1 : -1];
typedef char assert_SNDMGR_bank[(int)&((SNDMGR *)0)->bank == 0xE8 ? 1 : -1];
typedef char assert_SNDMGR_smooth[(int)&((SNDMGR *)0)->sfxVariationStep == 0x138 ? 1 : -1];
typedef char assert_SNDMGR_tick[(int)&((SNDMGR *)0)->tickNow == 0x1F0 ? 1 : -1];
typedef char assert_SNDMGR_dialog[(int)&((SNDMGR *)0)->dialogSound == 0x200 ? 1 : -1];

#endif
