/* include/sly2/classes.h -- the classes of the class registry (app/class_registry.cpp's factories and descriptor
 * getters) that no other sly2 header defines, at file scope under their real names, so that their constructors
 * reference the real vtables (_vt$<len><NAME>, named in config/symbol_addrs.txt) and no alias line is needed.
 *
 * Each class: its parent (class-tree.md), its size (the descriptor's cb, asserted), what construction shows (the
 * members that are constructed, out-of-line constructors under their current symbols), and its real virtual layout
 * from the ELF vtable: the overrides of inherited slots and its new slots, each declared as a member whose asm label
 * is the function the vtable holds at that slot. The first declared virtual is the class's key function (g++ 2.95:
 * the first non-inline, non-pure virtual): its key TU defines it as a member and so emits the vtable
 * (data/vtables; GetClassInfo stays first where class_registry, the key TU, defines only the getter as a C function).
 * Only an override, or a new virtual preceded by nothing but overrides, may move to the front: new virtuals take
 * their slots in declaration order. The classes are defined in emission order: g++ emits a TU's vtables in the
 * reverse order of their classes' definitions, so each key TU's classes come in descending ELF address order
 * (bases first). Members inherit the names and signatures of the slot's first declaration
 * (sly2/lo.h, so.h, ...); slots no header names are VSlot<n>() (n = 8-byte slot index; slot 0 is null). The ten
 * grab-spot classes override GRABSPOT's slots as their secondary vtables show (delta = -offset of GRABSPOT).
 * The classes the other headers define (BASIC, LO, XFM, PROXY, SHAPE, TN, TARGET, VOL, DETECTOR, EMITTER, EXPL,
 * EXPLO, SO, CID148, CID150, PO, POCTL, STEP, LEAP, CID146, BHV, STEPPLAYER, JT, MURRAY, BENTLEY, STEPGUARD, ASEG,
 * ASEGBL, SM, ASEGA, SMA, JOB, TASK, CM) and SW (sly2/sw.h, types/views-3) carry the same in their headers.
 * Generated from the ELF and class_registry's construction views (types/views-2); see
 * docs/guides/tu-merging.md, "Views unified". */
#ifndef SLY2_CLASSES_H
#define SLY2_CLASSES_H
/* The headers and the first classes in the order class_registry (their key TU) emits its vtables, the reverse of
   0x2C8940..0x2CA550's order: SNDMGR, CM, TICKER, ASEGBL, CID149 and CID150 (so.h), POCTL (po.h), EXPLO
   (emitter.h), then GUARD and the rest below. */
#include "sly2/basic.h"
#include "sly2/lo.h"

#include "sly2/slotheap.h"   /* SNDMGR's pools */
#include "sly2/snd.h"        /* SNDREF */
#include "sly2/sw.h"         /* FILEREC (sw.h emits no vtable: SW's key function is a C definition) */
struct SNDSTREAM; struct SNDFILE;

/* 152 SNDMGR (was CID152; 0x250): the sound manager (find-sound-object~; g_sndmgr, D_003DD040, defined by sound.cpp,
   declared in sly2/sndmgr.h); vtable 0x2CA510, 6 slots. A BASIC (constructed by
   SNDMGR_ctor, which stores BASIC's words and vtable itself); sound.cpp's definition has its own construct-and-destruct
   form.
   Volume channels: 0 ambience, 1 music, 2 dialog, 3 bank sound effects (the menu's effects slider sets 0 and 3).
   Volume categories (rows of volumes): 1 the options menu, 2 the pause fade (0 pauses the channel's sounds), 3 a player
   mode's ducking (mode.cpp: 0.6); the rest are 1. */
struct SNDMGR : public BASIC
{
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
    virtual CLASSINFO *GetClassInfo() __asm__("GetCid152ClassDesc");   /* slot 6 */
};
typedef char assert_SNDMGR_size[sizeof(SNDMGR) == 0x250 ? 1 : -1];

#include "sly2/cm.h"

/* 154 TICKER (was CID154; 0x30): ticker / hint box (find-ticker-object~); vtable 0x2CA490, 6 slots */
struct TICKER : BASIC
{
    char pad10[0x30 - 0x10];
    virtual CLASSINFO *GetClassInfo() __asm__("GetCid154ClassDesc");   /* slot 6 */
};
typedef char assert_CID154_size[sizeof(TICKER) == 0x30 ? 1 : -1];

#include "sly2/aseg.h"
#include "sly2/so.h"
#include "sly2/po.h"
#include "sly2/emitter.h"
#include "sly2/players.h"
#include "sly2/guard.h"
#include "sly2/task.h"

extern "C" void *memset(void *pv, int b, unsigned int cb);

/* The constructors make g++ instantiate the destructors of the classes' CRefPtr members (for the cleanup of a
   partly constructed object), whose pointees (CFrame's CVarb, CSidebag, SND) are incomplete here. No includer of
   this header destroys a class object, so the three destructors are explicit specializations, declared and never
   defined or called. */
template <> CRefPtr<CFrame>::~CRefPtr();
template <> CRefPtr<CSidebag>::~CRefPtr();
template <> CRefPtr<SND>::~CRefPtr();

/* BASIC (sly2/basic.h): both counted pointers cleared by member constructors, then the vptr. As member
   constructors, not stores in the body: STEPGUARD_ctor's embedded CID146 addresses its +0x4 clear from the
   enclosing object (s0-based) only this way. */
inline BASIC::BASIC() : scriptFrame(refptrNull), scriptVariables(refptrNull) {}
/* PO (sly2/po.h): the one PO field construction sets. */
inline PO::PO() { wakeSound = 0; }

/* The grab-spot mix-in (second base of the grab-spot classes): GRABSPOTDATA (0x20 bytes, object/proxy.cpp) and
   its own vptr at +0x20 (vtable 0x2D28D8, 6 slots; hidden-types.md B2.12). Slot names from gameplay/player/grabspot.cpp;
   GetOwner, GetClosestParam and GetGrabPos have their signatures (names/vu-objects-2: from the overrides and
   grabspot.cpp's calls); the data fields are named from InitGrabspot and the classes' post-loads (xf.cpp's GRABSPOTDATA).
   A class with a secondary base must override at least one of its virtuals exactly when its secondary vtable in the
   ELF differs from the base's own: with -fno-rtti g++ reuses the base's vtable (_vt$8GRABSPOT) for a secondary base
   whose virtuals the class does not override, which changes the constructor's vptr stores. Every grab-spot class
   overrides slot 6 (an 8-byte function at 0x2599E8..); CARRYSPOT/CID37 override nothing of GRABSPOT and reuse CID35's
   (0x2D1EA8 in the ELF for all three). Those slot-6 functions are free inline functions of xfm.h/so.h (the owner
   getters, link/linkonce-misc); the GetOwner labels below name them exactly, so the key TU that emits a class's vtable
   emits its getter too (.gnu.linkonce.t.<symbol>), the only object that does. */
struct GRABSPOT
{
    TN *ptn;                /* +0x00 camera tunnel of the spot (PostLoad turns its TNS override off) */
    int unknown04;
    float sBusy;            /* +0x08 busy radius; InitGrabspot sets FLT_MAX, the barrel 300 */
    int noSafePlace;        /* +0x0C nonzero: holding the spot records no safe place (grab.cpp RecordJtGrabPlace) (inferred); was unknown0C */
    int nPriority;          /* +0x10 (CID35's Init sets 1, CARRYSPOT's -1) */
    int nUnk14;             /* +0x14 InitGrabspot sets -1 */
    struct LO *ploGroup;    /* +0x18 an object reference (option 781, otyp 0x2002, through the OwnFields accessors) */
    int grfPlayers;         /* +0x1C players that may grab it (1 Sly, 2 Bentley, 4 Murray; InitGrabspot sets 7) */
    virtual int GetGrabRange(float *pasRange) __asm__("GetGrabspotWrapRange");     /* slot 1; key function (grabspot.cpp) */
    virtual void GetClosestParam(STEPPLAYER *pstepplayer, float *psParam) __asm__("GetGrabspotClosestParam");         /* slot 2 */
    virtual void GetGrabPos(float sParam, STEPPLAYER *pstepplayer, qword *ppos, float *pgHeading) __asm__("GetGrabspotGrabPosition");              /* slot 3 */
    virtual void AdjustScore(float *pgScore) __asm__("AdjustGrabspotScore");             /* slot 4 */
    virtual void GetSparklePos(qword *ppos, qword *pdv, float sParam) __asm__("GetGrabspotSparklePosition");           /* slot 5 */
    virtual LO *GetOwner() __asm__("GetGrabspotOwner");                     /* slot 6 */
};

/* A blip group's size polynomial over the normalised life (the EMITB's scale CLQ prescaled by 1 / lifetime). */
struct BLIPGPOLY { float a, b, c; };
/* A blip group's life parameters as uploaded to the VU (blip.cpp InitS: the second word is 1 / lifetime). */
struct BLIPGLIFE { int unknown0; float inverseLifetime; int unknown8; };
/* One entry of a blip group's age table (the VU's per-frame records; uploaded with AddVifUnpackRefs).
   The empty constructor is load-bearing: BLIPG's implicit constructor (class_registry.cpp NewBlipg) keeps the
   original's 32-step empty loop, which g++ 2.95 emits for an array member whose element class declares a constructor
   (the class used to declare its bytes as 32 such 0x1C-byte records). */
union BLIPGAGE
{
    qword q;
    struct { long l; int n; int pad; } s;
    // Uninitialised entry.
    BLIPGAGE() {}
};

struct PATHLINK;
void ClearPathlinkend(void *pv) __asm__("ClearPointerPair");   /* guardbhv: zeroes the first two words (C name ClearPointerPair, declared differently by its callers) */
/* A path-link endpoint / go-to goal (0x20), constructed by an inline constructor that calls ClearPointerPair
   (guardbhv; it zeroes the first two words; as an asm-labelled out-of-line constructor g++ keeps `this` live across
   the call: one move more in ExecuteBhvScriptCommand, two swapped in SetBhvcombatState): a shape, an object, or (both NULL) the point at +0x10. PATHLINK holds six, BHVGOTO three (goal,
   waypoint, relocate goal), BHV191 one; guardscript/bhvsearch build goals on the stack for SetBhvgotoGoal. */
struct PATHLINKEND
{
    SHAPE *shape;           /* +0x00 the goal shape (GetPathlinkEndpointPos projects onto it) */
    LO *object;             /* +0x04 the goal object */
    int unknown8[2];
    qword pos;              /* +0x10 the goal point */
    PATHLINKEND() { ClearPathlinkend(this); }
};

/* BHVGUARD's mode record (0x70, at +0x30; BHVGUARD.md): the awareness mode and its argument, or for the zap modes the
   zap. SetBhvguardMode copies it whole; IsBhvguardModeSame compares two. Its inline constructor clears it. */
struct BHVGUARDREC : BHVGUARDRECDATA
{
    // Zero the whole guard behaviour record.
    BHVGUARDREC() { memset(this, 0, sizeof(*this)); }
};

/* 84 SENSOR (0x190); vtable 0x2D89A0, 75 slots */
struct SENSOR : SO
{
    int numNoTriggerObjects;    /* +0x150 objects that do not trigger it (at most 7; sensor.cpp) */
    OID noTriggerObjects[7];
    int numNoTriggerClasses;    /* +0x170 classes that do not trigger it (at most 7) */
    CID noTriggerClasses[7];
    virtual void Init() __asm__("InitSensor");   /* slot 7; key function (sensor.cpp) */
    virtual CLASSINFO *GetClassInfo() __asm__("GetSensorClassDesc");   /* slot 6 */
    virtual int ShouldIgnoreIntersection(SO *psoOther) __asm__("ShouldSensorIgnoreIntersection");   /* slot 61 */
};
typedef char assert_SENSOR_size[sizeof(SENSOR) == 0x190 ? 1 : -1];

/* 90 DPRIZE (0x230) and its classes COIN, MEDICINE, KEY, LOOT, CLUE, with CLUEPROXY and VAULT (key TU prize.cpp):
   sly2/prize.h */
#include "sly2/prize.h"

/* 126 WIDGET (0x110); vtable 0x2DAF30, 50 slots: sly2/widget.h (files that cannot include this header use it) */
#include "sly2/widget.h"

/* 138 SAA (0x50); vtable 0x2D9420, 10 slots */
struct SAAF;   /* the shader-animation parameters (Sly 1 symbols InitShuffle__FP7SHUFFLEP4SAAF...) */
struct SAA : BASIC
{
    float updateTime;       /* +0x10 [S] (shdanim.cpp SAAV) */
    int id;                 /* +0x14 [S] */
    int unknown18;
    int numFrames;          /* +0x1C [S] */
    uint grf;               /* +0x20 [S] flags (glob.cpp SAAFIELDS) */
    short uOffset;          /* +0x24 [S] uv offset (1/4096) */
    short vOffset;          /* +0x26 [S] */
    unsigned short frame;   /* +0x28 [S] */
    short unknown2A;
    int isDirty;            /* +0x2C [S] */
    int isGlobsetOwned;     /* +0x30 [S] 1: loaded with a model GLOBSET (glob.cpp), 0: the shared shader table (shd.cpp; InitSaa puts those on g_sw.slSaa); name check: 1 in 3716/3863 nonzero */
    SL instances;           /* +0x34 [S] its copy records (shd.h COPYREC, linked through pcopyrecNext) */
    SAA *next;              /* +0x40 [S] next in g_sw.slSaa */
    int cqwMain;            /* +0x44 [S] (glob.cpp SAAFIELDS) */
    void *pvMain;           /* +0x48 [S] */
    int unknown4C;
    virtual void Init(SAAF *psaaf) __asm__("InitSaa");   /* slot 7 (its first new virtual); key function (shdanim.cpp) */
    virtual CLASSINFO *GetClassInfo() __asm__("GetSaaClassDesc");   /* slot 6 */
    virtual void Update() __asm__("UpdateSaa");   /* slot 8 */
    virtual void GetCompletion() __asm__("GetSaaCompletion__FP3SAA");   /* slot 9 */
    virtual int DoesMatchOid(int *poid) __asm__("DoesSaaMatchOid");   /* slot 10: does the animation drive this oid (a shader's; glob.cpp passes &SHD::oid) */
};
typedef char assert_SAA_size[sizeof(SAA) == 0x50 ? 1 : -1];
typedef char assert_SAA_grf[SLY2_OFFSETOF(SAA, grf) == 0x20 ? 1 : -1];
typedef char assert_SAA_instances[SLY2_OFFSETOF(SAA, instances) == 0x34 ? 1 : -1];
typedef char assert_SAA_pvMain[SLY2_OFFSETOF(SAA, pvMain) == 0x48 ? 1 : -1];

/* 151 CID151 (0x60): SND (sound instance) and its kinds BANKSND / STREAMSND; vtable 0x2D9D00, 16 slots (sly2/snd.h,
   key TU sound.cpp) */
#include "sly2/snd.h"

/* 58 GUARD (0x1010): plain guard; vtable 0x2C9DC0, 89 slots */
struct GUARD : STEPGUARD
{
    virtual CLASSINFO *GetClassInfo() __asm__("GetGuardClassDesc");   /* slot 6 */
};
typedef char assert_GUARD_size[sizeof(GUARD) == 0x1010 ? 1 : -1];

/* 44 CID44 (0x140): camera marker (CPCAMERA target); vtable 0x2CA9F0, 50 slots */
struct CID44 : XFM
{
    CID44 *lookAtTarget;    /* +0x0E0 look-at target (0: use the object's matrix) (camera.cpp) */
    undefined4 unknownE4[7];
    qword axis100;          /* +0x100 initialized to (1, 0, 0, 0) (InitCamera; meaning unknown) */
    qword axis110;          /* +0x110 initialized to g_normalZ (meaning unknown) */
    int cutOnEnd;           /* +0x120 cut the camera when the policy ends */
    int hasPolicy;          /* +0x124 the policy is set while in the hierarchy */
    float focalLength;      /* +0x128 focal length (field of view = 2 atan(18 / focalLength)) */
    int fovRequests;        /* +0x12C > 0: set the field of view from focalLength */
    float fov;              /* +0x130 field of view set on activation (0: keep) */
    undefined4 unknown134[3];
    virtual void Init() __asm__("InitCamera");   /* slot 7 */
    virtual CLASSINFO *GetClassInfo() __asm__("GetCid44ClassDesc");   /* slot 6 */
    virtual void AddHierarchy() __asm__("AddCameraHierarchy");   /* slot 11 */
    virtual void RemoveHierarchy() __asm__("RemoveCameraHierarchy");   /* slot 12 */
    virtual void PostLoad() __asm__("PostCameraLoad");   /* slot 15 */
    virtual void *GetAnimatedField(int id) __asm__("GetCameraFocalLengthField");   /* slot 43 */
    virtual void AcquireAnimatedField(int id) __asm__("IncrementCameraFovRequests");   /* slot 44 */
    virtual void ReleaseAnimatedField(int id) __asm__("DecrementCameraFovRequests");   /* slot 45 */
};
typedef char assert_CID44_size[sizeof(CID44) == 0x140 ? 1 : -1];
typedef char assert_CID44_fov[SLY2_OFFSETOF(CID44, fov) == 0x130 ? 1 : -1];   /* camera.cpp */

/* 43 CID43 (0x100): ambient-sound XFM (OnAdd/OnRemove start/stop); vtable 0x2CABD8, 50 slots */
struct AMBSND;   /* amb.cpp: a sound the emitter plays (0x14 bytes) */
struct AMBREC;   /* amb.cpp: a linked sound record (0x10 bytes) */
struct CID43 : XFM
{
    /* The emitter's fields (amb.cpp's AMB view; declared here so the class has its size on every target). */
    AMBSND *currentSound;       /* +0xE0 */
    int soundCount;             /* +0xE4 */
    AMBSND *sounds;             /* +0xE8 at most 8 */
    void **playingSounds;       /* +0xEC playing sounds, one per AMBSND */
    int linkedCount;            /* +0xF0 */
    AMBREC *linkedSounds;       /* +0xF4 at most 8 */
    char padF8[0x100 - 0xF8];
    virtual void Init() __asm__("InitAmb");   /* slot 7 */
    virtual CLASSINFO *GetClassInfo() __asm__("GetCid43ClassDesc");   /* slot 6 */
    virtual void LoadFromBrx(CBinaryInputStream *pbis) __asm__("LoadAmbFromBrx");   /* slot 14 */
    virtual void PostLoad() __asm__("PostAmbLoad");   /* slot 15 */
    virtual void OnRemove() __asm__("OnAmbRemove");   /* slot 22 */
    virtual void OnAdd() __asm__("OnAmbAdd");   /* slot 23 */
};
typedef char assert_CID43_size[sizeof(CID43) == 0x100 ? 1 : -1];

/* 86 BARRIER (0x180); vtable 0x2CAF88, 75 slots */
struct BARRIER : SO
{
    int numObjects;             /* +0x150 objects named by the barrier (barrier.cpp) */
    OID objects[4];             /* +0x154 */
    int numClasses;             /* +0x164 classes named by the barrier */
    CID classes[4];             /* +0x168 */
    int letsNamedThrough;       /* +0x178 named objects pass (else: only named objects are stopped) */
    SO *impactObject;           /* +0x17C the object reported by the current impact event */
    virtual void Init() __asm__("InitBarrier");   /* slot 7 */
    virtual CLASSINFO *GetClassInfo() __asm__("GetBarrierClassDesc");   /* slot 6 */
    virtual void PostLoad() __asm__("PostBarrierLoad");   /* slot 15 */
    virtual void AdjustNewXp(XP *pxp, int ixpd) __asm__("NotifyBarrierImpact");   /* slot 55 */
    virtual int ShouldIgnoreIntersection(SO *psoOther) __asm__("ShouldBarrierIgnoreIntersection");   /* slot 61 */
};
typedef char assert_BARRIER_size[sizeof(BARRIER) == 0x180 ? 1 : -1];

/* 28 BLIPG (0x460); vtable 0x2CB5D8, 51 slots */
struct BLIPG : XFM
{
    struct EMITTER *plo;    /* +0xE0 the subscribed object (an EMITTER, CID 0x1D) */
    char padE4[0xF0 - 0xE4];
    qword boundsCenter;     /* +0xF0 bounding sphere centre */
    float boundsRadius;     /* +0x100 */
    char pad104[0x108 - 0x104];
    void *unknown108;
    char pad10C[0x110 - 0x10C];
    BLIPGPOLY poly;         /* +0x110 size over the normalised life */
    void *pad11C;
    float maxSize;          /* +0x120 the largest |size| over a life (pads the bounds) */
    char pad124[0x130 - 0x124];
    char alphaCurve[0x10];  /* +0x130 alpha CLQ over the normalised life */
    char unknown140[4];     /* +0x140 (zeroed, uploaded to the VU) */
    float frameRate;        /* +0x144 texture frames per second */
    char pad148[8];
    BLIPGLIFE lifeParams;   /* +0x150 */
    int pad15C;
    int motionKind;         /* +0x160 motion kind: 0 matrix, 1 plain, 2 bezier */
    int pad164[(0x16C - 0x164) / 4];
    void *unknown16C;
    char emitdv[0x30];      /* +0x170 EMITDV */
    int numFrames;          /* +0x1A0 texture animation frames (a power of two, <= 16) */
    int pad1A4[(0x1A8 - 0x1A4) / 4];
    void *unknown1A8;
    int pad1AC[(0x1B0 - 0x1AC) / 4];
    BLIPGAGE age[0x20];     /* +0x1B0 */
    int numColors;          /* +0x3B0 colour count */
    unsigned char colors[0x20][4];   /* +0x3B4 */
    int hasColorRanges;     /* +0x434 colours come in (start, end) pairs */
    void *gifPacket;        /* +0x438 GIF packet (blip.cpp D_002DD5C0 / D_002DD600) */
    int renderKind;         /* +0x43C render kind */
    float flyingScale;      /* +0x440 (renderKind 1) */
    int numLiveParticles;   /* +0x444 live particles */
    DL dl;                  /* +0x448 the group's BLIPBATCHes (sly2/blip.h) */
    DLE dle;                /* +0x454 on the frame's blip group list (blip.cpp D_002DEAF0) */
    virtual void Release() __asm__("ReleaseBlipg");   /* slot 2 */
    virtual CLASSINFO *GetClassInfo() __asm__("GetBlipgClassDesc");   /* slot 6 */
    virtual void Init() __asm__("InitBlipg__FP5BLIPG");   /* slot 7 */
    virtual void RemoveHierarchy() __asm__("OnBlipgRemove__FP5BLIPG");   /* slot 12 */
    virtual void Update() __asm__("UpdateBlipg");   /* slot 16 */
    virtual void UpdateXfWorld() __asm__("ProjectBlipgTransform");   /* slot 17 */
    virtual void RenderSelf() __asm__("RenderBlipgSelf");   /* slot 19 */
    virtual void SubscribeObject(LO *plo) __asm__("SubscribeBlipgObject");   /* slot 51: LO::SubscribeObject plus the group's own bookkeeping (blip.cpp) */
};
typedef char assert_BLIPG_size[sizeof(BLIPG) == 0x460 ? 1 : -1];
typedef char assert_BLIPG_age[SLY2_OFFSETOF(BLIPG, age) == 0x1B0 ? 1 : -1];
typedef char assert_BLIPG_dle[SLY2_OFFSETOF(BLIPG, dle) == 0x454 ? 1 : -1];

/* 79 BOMB (0x210): also guard rock ammo; vtable 0x2CB780, 77 slots */
struct BOMB : SO
{
    /* bomb.cpp's BOMBV names these (declared here so the class has its size on every target). */
    int k;                      /* +0x150 0 armed, 1 absorbing... (TakeBombDamage) */
    float tDetonate;            /* +0x154 */
    float detonateSpeed;        /* +0x158 detonates on impact above this speed (UpdateBomb) */
    float sRadius;              /* +0x15C blast radius */
    float sRadiusHero;          /* +0x160 blast radius against the hero */
    int noBlast;                /* +0x164 detonation without blast damage */
    LO *blastListener;          /* +0x168 told about the blast (vtable slot 51) */
    float blastListenerArg;     /* +0x16C */
    int detonationPoseMode;     /* +0x170 */
    int isDetonationPrepared;   /* +0x174 */
    int pad178[2];
    qword posDetonate;          /* +0x180 */
    MATRIX3Q matDetonate;       /* +0x190 */
    qword velocityAtDetonation; /* +0x1C0 */
    qword angularVelocityAtDetonation;   /* +0x1D0 */
    int makesNoise;             /* +0x1E0 */
    int detonatesOnContact;     /* +0x1E4 */
    LO *ploStuckTo;             /* +0x1E8 the object a sticky bomb is attached to: ApplyBombThrow (slot 77) parents the bomb
                                   to it; a contact bomb that is stuck does not detonate on contact (Sly 1:
                                   AttachStickyProjectile). Was lastThrower. */
    int zapKind;                /* +0x1EC */
    int blastDamage;            /* +0x1F0 */
    int fCustomXps;             /* +0x1F4 */
    int cxp;                    /* +0x1F8 */
    void *axp;                  /* +0x1FC bomb.cpp's CXP records */
    LO *thrower;            /* +0x200 [S] the object that threw the bomb (a guard ignores its own team's: TakeStepguardDamage);
                               bomb.cpp's owner (itself until thrown) */
    int unknown204[3];
    virtual void HandleMessage(MSGID msgid, void *pv) __asm__("HandleBombMessage");   /* slot 5 */
    virtual CLASSINFO *GetClassInfo() __asm__("GetBombClassDesc");   /* slot 6 */
    virtual void Init() __asm__("InitBomb");   /* slot 7 */
    virtual void PostLoad() __asm__("PostBombLoad");   /* slot 15 */
    virtual void Update() __asm__("UpdateBomb");   /* slot 16 */
    virtual void OnProxyAcquire(LO *plo) __asm__("ResetBomb");   /* slot 26 */
    virtual void AdjustXps() __asm__("HandleBombContact");   /* slot 58 */
    virtual void AddCustomXps(SOPART *ppart, int cbsp, BSP *absp, BSP *pbsp, XP **ppxpFirst) __asm__("AddBombCustomXps");   /* slot 60 */
    virtual int ShouldIgnoreIntersection(SO *psoOther) __asm__("ShouldBombIgnoreIntersection");   /* slot 61 */
    virtual int TakeDamage(ZPR *pzpr) __asm__("TakeBombDamage");   /* slot 75 */
    virtual void CopyFromSource() __asm__("CloneBomb");   /* slot 76 */
    virtual void ApplyThrow() __asm__("ApplyBombThrow");   /* slot 77 */
};
typedef char assert_BOMB_size[sizeof(BOMB) == 0x210 ? 1 : -1];

/* 103 BOOTY (0x190): replaces cid_table ALARM; vtable 0x2CBBA0, 75 slots */
/* BOOTYE: one booty entry (booty.cpp AddBootyEntry). hasTimer: carrying it runs the HUD timer; allowedDuringJob: may be put
   in a container during a job; containerKind: the container kind (CID37V::acceptedKind) it fits; partsId: SO parts shown
   while it is current, -1 none. (inferred) */
struct BOOTYE
{
    int id;
    int unknown4;
    int dropsOnHit;             /* +0x08 any hit makes the carrier drop it (hurt.cpp TakeJtDamage; else only a fatal one) */
    int hasTimer;
    int allowedDuringJob;
    int containerKind;
    int partsId;
    unsigned int c1C;           /* objects in aoid */
    OID aoid[3];
};
typedef char assert_BOOTYE_size[sizeof(BOOTYE) == 0x2C ? 1 : -1];
struct BOOTY : SO
{
    /* The booty's own fields (booty.cpp's BOOTYV view reads them; declared here so the class has its size on every
       target). Option rows: 1005 (+0x164, an object of class 0x1E), 1006 (+0x170), 1011 (+0x15C, a CID37), 1012
       (+0x160, a CID 0x34 player), 342 (+0x184). */
    int cbootye;                /* +0x150 entries in abootye (at most 0x20) */
    BOOTYE *abootye;            /* +0x154 the entry table (SW heap) */
    BOOTYE *pbootyeCur;         /* +0x158 the current entry, 0 none */
    struct CID37 *pcont;        /* +0x15C the container holding it (state 1, and the one it returns to) */
    struct STEPPLAYER *phero;   /* +0x160 the player carrying it */
    struct EXPL *unknown164;    /* +0x164 an effect exploded (ExplodeExplso, grf 1 and the booty as reference) when it
                                   leaves the dropped state (option 1005: a CID 0x1E, EXPL) */
    char pad168[0x170 - 0x168];
    VECTOR4 vScale;             /* +0x170 scale while carried (InitBooty: the default scale vector) */
    float tDrop;                /* +0x180 length of the scale pop after a pick-up (0.3 s) */
    int st;                     /* +0x184 state: -1 inactive, 0 idle, 1 in container, 2 / 3 transfer, 4 carried,
                                   5 dropped and returning, 6 marker (SetBootyState) */
    float tState;               /* +0x188 g_clock.time of the last state change */
    struct RIP *pmatch;         /* +0x18C the rip that flies it between holders (NewRipInGroup) */
    virtual void PostLoad() __asm__("PostBootyLoad");   /* slot 15 */
    virtual CLASSINFO *GetClassInfo() __asm__("GetBootyClassDesc");   /* slot 6 */
    virtual void HandleMessage(MSGID msgid, void *pv) __asm__("HandleBootyMessage");   /* slot 5 */
    virtual void Init() __asm__("InitBooty");   /* slot 7 */
    virtual void LoadFromBrx(CBinaryInputStream *pbis) __asm__("LoadBootyFromBrx");   /* slot 14 */
    virtual void Update() __asm__("UpdateBooty");   /* slot 16 */
    virtual void RenderSelf() __asm__("RenderBootySelf");   /* slot 19 */
    virtual void OnProxyRelease(LO *plo) __asm__("ClearBootyState");   /* slot 27 */
    virtual void AdjustNewXp(XP *pxp, int ixpd) __asm__("AdjustBootyNewXp");   /* slot 55 */
    virtual int ShouldIgnoreIntersection(SO *psoOther) __asm__("ShouldBootyIgnoreIntersection");   /* slot 61 */
    virtual int TakeDamage(ZPR *pzpr) __asm__("TakeBootyDamage");   /* slot 75 */
};
typedef char assert_BOOTY_size[sizeof(BOOTY) == 0x190 ? 1 : -1];

/* 104 CID104 (0x180): buffer effects / frame-capture transition; vtable 0x2CC548, 75 slots */
struct CID104 : SO
{
    char pad150[0x180 - 0x150];
    virtual void PostLoad() __asm__("PostFrameCaptureLoad");   /* slot 15 */
    virtual CLASSINFO *GetClassInfo() __asm__("GetCid104ClassDesc");   /* slot 6 */
    virtual void HandleMessage(MSGID msgid, void *pv) __asm__("HandleFrameCaptureMessage");   /* slot 5 */
    virtual void Init() __asm__("InitFrameCapture");   /* slot 7 */
    virtual void Update() __asm__("UpdateFrameCapture");   /* slot 16 */
};
typedef char assert_CID104_size[sizeof(CID104) == 0x180 ? 1 : -1];

/* RSMG: a state-machine trigger of a button, exit or warp (Sly 1 RSMG): when triggered, the state machine oidSM under
   oidRoot goes to oidTriggerGoal; when untriggered, to oidUntriggerGoal (-1: none). AddOrUpdateRsmg fills one. */
struct RSMG
{
    OID oidRoot;
    OID oidSM;
    OID oidTriggerGoal;
    OID oidUntriggerGoal;
};
typedef char assert_RSMG_size[sizeof(RSMG) == 0x10 ? 1 : -1];

/* TASKSTATEREQ: a (task, state) pair; ApplyTaskStates requests each state of its task (game.cpp; the states are
   game.cpp's enum TASKSTATE). Name inferred. */
struct TASKSTATEREQ
{
    OID oidTask;
    int state;
};

/* ASHE/ASH: a list of animation segments to play (oids at load, resolved to the segments by PostAshLoad). From
   src/game/gameplay/objects/button.cpp; names from its functions (AddAshEntry, PostAshLoad). */
struct ASHE
{
    union { OID oid; ASEG *paseg; } u;
    int ensureArg;          /* last argument of EnsureAsegPlayer (inferred: a segment flag) */
};
struct ASH
{
    int count;
    ASHE entries[8];
};

/* BTN: the trigger logic BUTTON and VOLBTN share (button.cpp): it plays its "on"/"off" segment lists, fires a relay,
   applies rsmgs and task states and tells its object. States: button.cpp's BTN_STATE. */
struct BTN
{
    LO *plo;                    /* +0x00 the button object */
    ASH animLists[2];           /* +0x04 played on trigger (0) and on untrigger (1) */
    float segmentSpeed;         /* +0x8C (1.0; negative plays backwards) */
    int state;                  /* +0x90 BTN_STATE (-1 until the first SetBtnButtons) */
    float stateTime;            /* +0x94 time of the last state change */
    float resetDelay;           /* +0x98 seconds a pressed button waits before it resets (1.0; negative: never) */
    int isSilent;               /* +0x9C no click sound on trigger */
    int seeksBySpeedSign;       /* +0xA0 seek the segments by the sign of segmentSpeed */
    int staysTriggered;         /* +0xA4 trigger does not move on to BTN_PRESSED */
    void *relayArg;             /* +0xA8 relay argument (a volume button stores the first object inside) */
    void *relay;                /* +0xAC fired on trigger */
    int numRsmgs;               /* +0xB0 */
    RSMG rsmgs[8];              /* +0xB4 */
    int numTaskStates;          /* +0x134 */
    TASKSTATEREQ taskStates[4];    /* +0x138 applied on trigger */
};
typedef char assert_BTN_size[sizeof(BTN) == 0x158 ? 1 : -1];

/* 83 BUTTON (0x300): instances: bouncers, buoys; vtable 0x2CC950, 75 slots. An SO pressed by contact, a player state
   or a worker (button.cpp's BUTTON_PRESS_KIND); layout from button.cpp (the former BUTTONV view). */
struct BUTTON : SO
{
    BTN btn;                    /* +0x150 */
    int pressKind;              /* +0x2A8 BUTTON_PRESS_KIND */
    int numPushObjects;         /* +0x2AC objects that may press it (none: any) */
    LO *pushObjects[8];         /* +0x2B0 */
    int numPushClasses;         /* +0x2D0 classes that may press it (none: any) */
    CID pushClasses[8];         /* +0x2D4 */
    struct HND *phndPress;      /* +0x2F4 the hang handle Sly presses it by hanging from (compared with JT's) */
    LO *pusher;                 /* +0x2F8 the object pressing it */
    int unk2FC;                 /* +0x2FC unused by button.cpp */
    virtual void Clone(LO *ploBase) __asm__("CloneButton");   /* slot 41 */
    virtual CLASSINFO *GetClassInfo() __asm__("GetButtonClassDesc");   /* slot 6 */
    virtual void Init() __asm__("InitButton");   /* slot 7 */
    virtual void RemoveHierarchy() __asm__("RemoveButtonHierarchy");   /* slot 12 */
    virtual void LoadFromBrx(CBinaryInputStream *pbis) __asm__("LoadButtonFromBrx");   /* slot 14 */
    virtual void PostLoad() __asm__("PostButtonLoad");   /* slot 15 */
    virtual void Update() __asm__("UpdateButton");   /* slot 16 */
    virtual int TakeDamage(ZPR *pzpr) __asm__("TakeButtonDamage");   /* slot 75 */
};
typedef char assert_BUTTON_size[sizeof(BUTTON) == 0x300 ? 1 : -1];
typedef char assert_BUTTON_pusher[SLY2_OFFSETOF(BUTTON, pusher) == 0x2F8 ? 1 : -1];

/* 24 VOLBTN (0x2D0): REVIEW, high; vtable 0x2CC7B0, 50 slots */
/* A VOL pressed by objects inside it; layout from button.cpp (the former VOLBTNV view). */
struct VOLBTN : VOL
{
    BTN btn;                    /* +0x130 */
    int numPushObjects;         /* +0x288 objects that may press it */
    LO *pushObjects[8];         /* +0x28C */
    int numPushClasses;         /* +0x2AC classes that may press it */
    CID pushClasses[4];         /* +0x2B0 */
    int numInside;              /* +0x2C0 objects inside (sorted), kept when tracksInside */
    LO **objectsInside;         /* +0x2C4 */
    int tracksInside;           /* +0x2C8 */
    int unk2CC;                 /* +0x2CC unused by button.cpp */
    virtual void Clone(LO *ploBase) __asm__("CloneVolbtn");   /* slot 41 */
    virtual CLASSINFO *GetClassInfo() __asm__("GetVolbtnClassDesc");   /* slot 6 */
    virtual void Init() __asm__("InitVolbtn");   /* slot 7 */
    virtual void RemoveHierarchy() __asm__("RemoveVolbtnHierarchy");   /* slot 12 */
    virtual void LoadFromBrx(CBinaryInputStream *pbis) __asm__("LoadVolbtnFromBrx");   /* slot 14 */
    virtual void PostLoad() __asm__("PostVolbtnLoad");   /* slot 15 */
    virtual void Update() __asm__("UpdateVolbtn");   /* slot 16 */
};
typedef char assert_VOLBTN_size[sizeof(VOLBTN) == 0x2D0 ? 1 : -1];
typedef char assert_VOLBTN_tracksInside[SLY2_OFFSETOF(VOLBTN, tracksInside) == 0x2C8 ? 1 : -1];

/* 25 CID25 (0x2D0): VOLBTN variant; vtable 0x2C9C20, 50 slots */
struct CID25 : VOLBTN
{
    virtual CLASSINFO *GetClassInfo() __asm__("GetCid25ClassDesc");   /* slot 6 */
};
typedef char assert_CID25_size[sizeof(CID25) == 0x2D0 ? 1 : -1];

/* 20 CID20 (0xF0): hand-attach XFM (adjust_hand_jt / _bentley, 2 per level); vtable 0x2C9A80, 50 slots */
struct CID20 : XFM
{
    char padE0[0xF0 - 0xE0];
    virtual CLASSINFO *GetClassInfo() __asm__("GetCid20ClassDesc");   /* slot 6 */
};
typedef char assert_CID20_size[sizeof(CID20) == 0xF0 ? 1 : -1];

/* A station of a ROUTE route (12 bytes): its kind (0 walk to it, 1 leap to it), the oid of its object, the object. */
struct ROUTESTATION
{
    int kind;               /* +0x0 */
    int oid;                /* +0x4 */
    LO *object;             /* +0x8 (PostRouteLoad) */
};
typedef char assert_ROUTESTATION_size[sizeof(ROUTESTATION) == 0xC ? 1 : -1];

/* 123 ROUTE (was CID123; 0x60): 256-point route (ROUTE, cid 0x7B: Carmelita's stations, carmelita.cpp); vtable 0x2CCF10, 48 slots */
struct ROUTE : LO
{
    int isReversed;         /* +0x50 1: distances along the route count the other way */
    int stationCount;       /* +0x54 */
    ROUTESTATION *stations; /* +0x58 (AddRoutePoint; at most 256) */
    SHAPE *shape;           /* +0x5C the path through the stations (PostRouteLoad) */
    virtual void PostLoad() __asm__("PostRouteLoad");   /* slot 15 */
    virtual CLASSINFO *GetClassInfo() __asm__("GetCid123ClassDesc");   /* slot 6 */
    virtual void LoadFromBrx(CBinaryInputStream *pbis) __asm__("LoadRouteFromBrx");   /* slot 14 */
};
typedef char assert_CID123_size[sizeof(ROUTE) == 0x60 ? 1 : -1];

/* 182 BHV182 (0x50): keep pace on route; vtable 0x2CCE90, 14 slots */
struct BHV182 : BHV
{
    int state;              /* +0x30 0 at a station, 1 walking, 2 leaping/recovering, 3 attacking, 4 hands-off */
    float stateTime;        /* +0x34 */
    ROUTE *route;          /* +0x38 [S] the mode's object (SetBhvguardMode): the route */
    int currentStation;     /* +0x3C the station the guard is at */
    int targetStation;      /* +0x40 the station to go to */
    CID120 *attack;         /* +0x44 */
    float lastFastHeroTime; /* +0x48 last time the hero moved fast */
    int unknown4C;
    virtual void Start() __asm__("StartBhvRoute");   /* slot 8 */
    virtual CLASSINFO *GetClassInfo() __asm__("GetBhv182ClassDesc");   /* slot 6 */
    virtual void OnPush() __asm__("InitBhvRoute");   /* slot 7 */
    virtual void Close() __asm__("CloseBhvRoute");   /* slot 9 */
    virtual void Update() __asm__("UpdateBhvRoute");   /* slot 10 */
};
typedef char assert_BHV182_size[sizeof(BHV182) == 0x50 ? 1 : -1];

/* 63 CARMELITA (0x1020); vtable 0x2CCBB8, 89 slots */
struct CARMELITA : STEPGUARD
{
    LO *marker;             /* +0x1010 the position marker (oid 0x62B) she targets while chasing on her route */
    int unknown1014[3];
    virtual void Init() __asm__("InitCarmelita");   /* slot 7 */
    virtual CLASSINFO *GetClassInfo() __asm__("GetCarmelitaClassDesc");   /* slot 6 */
    virtual void HandleMessage(MSGID msgid, void *pv) __asm__("HandleCarmelitaMessage");   /* slot 5 */
    virtual void PostLoad() __asm__("PostCarmelitaLoad");   /* slot 15 */
    virtual int IsZapLethal(ZPR *pzpr) __asm__("IsCarmelitaZapLethal");   /* slot 86 */
    virtual LO *ChooseTarget() __asm__("ChooseCarmelitaTarget");   /* slot 88 */
};
typedef char assert_CARMELITA_size[sizeof(CARMELITA) == 0x1020 ? 1 : -1];

/* 64 CONTESSA (0x1010); vtable 0x2CD0A0, 89 slots */
struct CONTESSA : STEPGUARD
{
    virtual void Init() __asm__("InitContessa");   /* slot 7 */
    virtual CLASSINFO *GetClassInfo() __asm__("GetContessaClassDesc");   /* slot 6 */
    virtual void Update() __asm__("UpdateContessa");   /* slot 16 */
};
typedef char assert_CONTESSA_size[sizeof(CONTESSA) == 0x1010 ? 1 : -1];

/* 68 DANCER (0x410): rhythm-game performer; vtable 0x2CD748, 87 slots */
/* A dancer's move (rhythm.cpp): its button, then three clips (oid, the clip once loaded, apply flags): 0 played on
   success, 1 on a failed press, 2 on a miss. */
struct DMR { int oid; void *clip; int applyFlags; };
struct DMOVE { int key; DMR clips[3]; };
struct DANCER : STEP
{
    void *idleSegment;          /* +0x2C0 the clip played between moves (UpdateDancer) */
    union { unsigned int numMovesU; int numMoves; };   /* +0x2C4 (AddDancerMove compares it unsigned) */
    DMOVE moves[8];             /* +0x2C8 */
    ASEGA *segmentAnim;         /* +0x408 the playing segment */
    int isPerforming;           /* +0x40C set/cleared by Splice messages 0x27/0x28 (inferred: a scripted move plays) */
    virtual void PostLoad() __asm__("PostDancerLoad");   /* slot 15 */
    virtual CLASSINFO *GetClassInfo() __asm__("GetDancerClassDesc");   /* slot 6 */
    virtual void HandleMessage(MSGID msgid, void *pv) __asm__("HandleDancerMessage");   /* slot 5 */
    virtual void RemoveHierarchy() __asm__("RemoveDancerHierarchy");   /* slot 12 */
    virtual void Update() __asm__("UpdateDancer");   /* slot 16 */
    virtual void GetCpdefi(CPDEFI *pcpdefi) __asm__("GetDancerCpdefi");   /* slot 50 */
};
typedef char assert_DANCER_size[sizeof(DANCER) == 0x410 ? 1 : -1];
typedef char assert_DANCER_moves[SLY2_OFFSETOF(DANCER, moves) == 0x2C8 ? 1 : -1];   /* rhythm.cpp */
typedef char assert_DANCER_segmentAnim[SLY2_OFFSETOF(DANCER, segmentAnim) == 0x408 ? 1 : -1];

/* The rhythm game's records (rhythm.cpp). RHYTHMNOTE: a note on the drawn track (0x30). RHYTHMCUE: a cue: the dancer,
   its button and the move that follows, when it is due (music time), and its demonstration / play state.
   RHYTHMGRADE: a crowd grade SMA driven by the score (object oid, grade oid, the bound SMA, current grade, the score
   thresholds). */
struct RHYTHMNOTE { float time; int buttonMask; char pad[8]; union { qword screenPos; VECTOR4 screenPosV; }; int isHit; char pad2[0x30 - 0x24]; };
struct RHYTHMCUE
{
    DANCER *dancer;
    int key;
    int nextKey;
    float time;
    float timeAdded;
    int hasPlayed;              /* demonstration state: the dancer showed the move */
    int hasPrompted;            /* play state: game event 0x34 sent */
    int wasPressed;             /* play state: the player pressed for it */
    int isExpired;              /* play state: the late window passed */
};
struct RHYTHMGRADE { int oid; int gradeOid; void *sma; int gradeIndex; float thresholds[3]; };

/* 121 RHYTHMMGR (0x10B0): rhythm-game manager (rhythm.cpp; was CID121); vtable 0x2CD5B8, 48 slots */
struct RHYTHMMGR : LO
{
    int state;                  /* +0x050 RHYTHM_STATE (rhythm.cpp; option 342 SetRhythmMgrState) */
    float stateTime;            /* +0x054 */
    float earlyWindow;          /* +0x058 seconds before a cue a press still counts (option 1159) */
    float lateWindow;           /* +0x05C seconds after (option 1160) */
    int numNotes;               /* +0x060 */
    int pad64[3];
    RHYTHMNOTE notes[6];        /* +0x070 */
    int hintState;              /* +0x190 0 waiting, 1 flying to the next note */
    int hintIndex;              /* +0x194 */
    int pad198[2];
    union { qword hintPos; VECTOR4 hintPosV; };  /* +0x1A0 the hint marker */
    qword hintVelocity;         /* +0x1B0 */
    int numCues;                /* +0x1C0 */
    RHYTHMCUE cues[3];          /* +0x1C4 */
    int eventButtons;           /* +0x230 buttons reported with game events 0x34 / 0x35 */
    LM scoreRange;              /* +0x234 (option 1162) */
    int numScores;              /* +0x23C */
    float scores[16];           /* +0x240 last scores */
    int averageWindow;          /* +0x280 scores averaged (option 1163) */
    float averageScore;         /* +0x284 (option 1169) */
    float smoothedScore;        /* +0x288 */
    union { char smoothing[0x0C]; SMP smoothingParams; };   /* +0x28C (option 1164; SmoothValue takes the bytes) */
    float earlyPressScore, wrongButtonScore, repeatPressScore, missScore;   /* +0x298 (options 1165..1168) */
    union { unsigned int numGradesU; int numGrades; };    /* +0x2A8 */
    RHYTHMGRADE grades[8];      /* +0x2AC */
    char pad38C[0x10B0 - 0x38C];
    virtual void PostLoad() __asm__("PostRhythmManagerLoad");   /* slot 15 */
    virtual CLASSINFO *GetClassInfo() __asm__("GetCid121ClassDesc");   /* slot 6 */
    virtual void Init() __asm__("InitRhythmMgr");   /* slot 7 */
    virtual void Update() __asm__("UpdateRhythmMgr");   /* slot 16 */
    virtual void RenderSelf() __asm__("DrawRhythmMgr");   /* slot 19 */
};
typedef char assert_RHYTHMMGR_size[sizeof(RHYTHMMGR) == 0x10B0 ? 1 : -1]; typedef char assert_RHYTHMMGR_grades[SLY2_OFFSETOF(RHYTHMMGR, grades) == 0x2AC ? 1 : -1];

/* 122 CID122 (0x60): ? (LoadFromBrx with FILEREC list); vtable 0x2CDA10, 48 slots */
struct CID122 : LO
{
    char pad50[0x60 - 0x50];
    virtual void LoadFromBrx(CBinaryInputStream *pbis) __asm__("LoadCid122FromBrx");   /* slot 14 */
    virtual CLASSINFO *GetClassInfo() __asm__("GetCid122ClassDesc");   /* slot 6 */
};
typedef char assert_CID122_size[sizeof(CID122) == 0x60 ? 1 : -1];

/* One of Dimitri's escape routes (0x24 bytes, dimitri.cpp): two ends with a volume around each, given as oids by
   the Splice option AddDimitriRoute and resolved by PostDimitriLoad. */
struct DIMITRIROUTE
{
    int oidFirst;           /* +0x00 */
    LO *first;              /* +0x04 */
    int oidFirstVolume;     /* +0x08 */
    VOL *firstVolume;       /* +0x0C the volume around the first end */
    int oidSecond;          /* +0x10 */
    LO *second;             /* +0x14 */
    int oidSecondVolume;    /* +0x18 */
    VOL *secondVolume;      /* +0x1C */
    int isUsed;             /* +0x20 */
};
typedef char assert_DIMITRIROUTE_size[sizeof(DIMITRIROUTE) == 0x24 ? 1 : -1];

/* 59 DIMITRI (0x1020); vtable 0x2CDE40, 89 slots */
struct DIMITRI : GUARD
{
    int routeCount;         /* +0x1010 escape routes (> 0: ResumeBhvguard flees from the hero) */
    DIMITRIROUTE *routes;   /* +0x1014 (LoadDimitriFromBrx allocates 32) */
    int hitPointsAtLastEscape; /* +0x1018 hit points when the last escape started */
    int unknown101C;
    virtual void PostLoad() __asm__("PostDimitriLoad");   /* slot 15 */
    virtual CLASSINFO *GetClassInfo() __asm__("GetDimitriClassDesc");   /* slot 6 */
    virtual void LoadFromBrx(CBinaryInputStream *pbis) __asm__("LoadDimitriFromBrx");   /* slot 14 */
};
typedef char assert_DIMITRI_size[sizeof(DIMITRI) == 0x1020 ? 1 : -1];

/* 180 BHV180 (0x40): DIMITRI flee; vtable 0x2CDDC0, 14 slots */
struct BHV180 : BHV
{
    int state;              /* +0x30 0 run the attack behaviour on the route, 1 fight */
    float stateTime;        /* +0x34 */
    LO *target;             /* +0x38 [S] what Dimitri escapes from (SetBhvguardMode: the mode's object) */
    DIMITRIROUTE *route;    /* +0x3C the chosen route */
    virtual void OnPush() __asm__("InitBhvDimitriEscape");   /* slot 7 */
    virtual CLASSINFO *GetClassInfo() __asm__("GetBhv180ClassDesc");   /* slot 6 */
    virtual void Start() __asm__("StartBhvDimitriEscape");   /* slot 8 */
    virtual void Close() __asm__("CloseBhvDimitriEscape");   /* slot 9 */
    virtual void Update() __asm__("UpdateBhvDimitriEscape");   /* slot 10 */
};
typedef char assert_BHV180_size[sizeof(BHV180) == 0x40 ? 1 : -1];

/* 181 BHV181 (0x60): DIMITRI flee; vtable 0x2CDD40, 14 slots */
struct BHV181 : BHV
{
    int state;              /* +0x30 0 turn, 1 face the target, 2 walk to the route end, 3 attack */
    float stateTime;        /* +0x34 */
    DIMITRIROUTE *route;    /* +0x38 */
    float unknown3C;
    LO *goal;               /* +0x40 the route end to go to (ChooseBhvDimitriAttackRouteEnd) */
    float lastAttackTime;   /* +0x44 */
    float arrivalTime;      /* +0x48 */
    LO *target;             /* +0x4C */
    int attackCount;        /* +0x50 */
    CID120 *attack;         /* +0x54 the chosen attack */
    int unknown58[2];
    virtual void Start() __asm__("StartBhvDimitriAttack");   /* slot 8 */
    virtual CLASSINFO *GetClassInfo() __asm__("GetBhv181ClassDesc");   /* slot 6 */
    virtual void OnPush() __asm__("InitBhvDimitriAttack");   /* slot 7 */
    virtual void Close() __asm__("CloseBhvDimitriAttack");   /* slot 9 */
    virtual void Update() __asm__("UpdateBhvDimitriAttack");   /* slot 10 */
};
typedef char assert_BHV181_size[sizeof(BHV181) == 0x60 ? 1 : -1];

/* 117 NOISETYPE (was CID117; 0x70): stimulus template (radius, height, ramp; the world's seven in SW::apstimk); vtable 0x2CE2F8, 48 slots */
struct NOISETYPE : LO
{
    int kind;               /* +0x50 0 audible, 1 visible (CanStepguardSenseStimulus; listeners hear only kind 0) */
    float range;            /* +0x54 (InitNoiseType 1500) */
    LM heightRange;         /* +0x58 (-200..500) */
    float duration;         /* +0x60 the noises' lifetime (< 0: they last; InitNoiseType 0.25 s) */
    int level;              /* +0x64 copied into each stimulus' NOISE::level */
    int unknown68[2];
    virtual void Init() __asm__("InitNoiseType");   /* slot 7 */
    virtual CLASSINFO *GetClassInfo() __asm__("GetCid117ClassDesc");   /* slot 6 */
};
typedef char assert_CID117_size[sizeof(NOISETYPE) == 0x70 ? 1 : -1];

/* 147 NOISE (was CID147; 0x60): stimulus (noise) instance; vtable 0x2CE2B8, 6 slots; defined between NOISETYPE and LISTENER: noise.cpp emits LISTENER, NOISE, NOISETYPE */
/* NOISE is the NOISE record (noise.cpp's view NOISE; STEPGUARD::heldNoise): a refcounted noise event emitted from a
   NOISETYPE noise type and linked on g_sw.dlStimulus. */
struct NOISE : BASIC
{
    int refCount;           /* +0x10 */
    struct NOISETYPE *type;    /* +0x14 the stimulus type it was emitted from */
    int unknown18[2];
    qword position;         /* +0x20 where it is (a held noise is moved with its maker: UpdateBhvSleep) */
    qword targetPosition;   /* +0x30 CreateNoise: the emission point; EmitStepguardStimulus: the alarm target's position */
    LO *source;             /* +0x40 what made it (NewNoise's third argument; a guard does not sense its own) */
    LO *subject;            /* +0x44 who it is about (level 4 alarms: only a tracked subject is sensed) */
    int serial;             /* +0x48 */
    float startTime;        /* +0x4C */
    int unknown50;
    int level;              /* +0x54 from NOISETYPE::level; compared with STEPGUARD::minStimulusLevel (3: a thrown guard yelling,
                               UpdateBhvThrown; 4: an alarm about subject, EmitStepguardStimulus) */
    NOISE *next;           /* +0x58 link of DL g_sw.dlStimulus (with previous: a DLE) */
    NOISE *previous;       /* +0x5C */
    virtual void AddRef() __asm__("RetainNoise");   /* slot 1; key function (noise.cpp) */
    virtual CLASSINFO *GetClassInfo() __asm__("GetCid147ClassDesc");   /* slot 6 */
    virtual void Release() __asm__("ReleaseNoise");   /* slot 2 */
};
typedef char assert_CID147_size[sizeof(NOISE) == 0x60 ? 1 : -1];
typedef char assert_CID147_position[SLY2_OFFSETOF(NOISE, position) == 0x20 ? 1 : -1];
typedef char assert_CID147_level[SLY2_OFFSETOF(NOISE, level) == 0x54 ? 1 : -1];

/* 47 LISTENER (was CID47; 0x110): stimulus listener (noise.cpp); vtable 0x2CE118, 50 slots */
struct LISTENER : XFM
{
    unsigned int noiseCount; /* +0xE0 the noises it heard and holds (at most 8) */
    NOISE *noises[8];      /* +0xE4 */
    DLE dleListener;        /* +0x104 link in g_sw.dlCid47 (AddListenerHierarchy; kept across CloneListener, as the grab
                               lists' links are; was LISTENERKEY key) */
    int unknown10C;
    virtual void AddHierarchy() __asm__("AddListenerHierarchy");   /* slot 11 */
    virtual CLASSINFO *GetClassInfo() __asm__("GetCid47ClassDesc");   /* slot 6 */
    virtual void Init() __asm__("InitListener");   /* slot 7 */
    virtual void RemoveHierarchy() __asm__("RemoveListenerHierarchy");   /* slot 12 */
    virtual void Update() __asm__("UpdateListener");   /* slot 16 */
    virtual void Clone(LO *ploBase) __asm__("CloneListener");   /* slot 41 */
};
typedef char assert_CID47_dleListener[SLY2_OFFSETOF(LISTENER, dleListener) == 0x104 ? 1 : -1];
typedef char assert_CID47_size[sizeof(LISTENER) == 0x110 ? 1 : -1];

/* 31 EXPLG (0xF0); vtable 0x2CE828, 51 slots */
struct EXPLG : EXPL
{
    int numChildren;            /* +0xE0 the explosions of the group (emitter.cpp LoadExplgFromBrx) */
    LO **children;              /* +0xE4 */
    int unkE8[2];
    virtual int GetChildren(LO ***paplo) __asm__("CploExplgChildren");   /* slot 28 */
    virtual CLASSINFO *GetClassInfo() __asm__("GetExplgClassDesc");   /* slot 6 */
    virtual void LoadFromBrx(CBinaryInputStream *pbis) __asm__("LoadExplgFromBrx");   /* slot 14 */
    virtual void Clone(LO *ploBase) __asm__("CloneExplg");   /* slot 41 */
    virtual void ExplodeExplso(EXPLSO *pexplso) __asm__("ExplodeExplgExplso");   /* slot 51 */
};
typedef char assert_EXPLG_size[sizeof(EXPLG) == 0xF0 ? 1 : -1];

/* 33 EXPLS (0x110); vtable 0x2CE4D0, 52 slots */
struct EXPLS : EXPLO
{
    struct SFX *sfx;            /* +0xF0 its sound effect (EnsureExplsSfx creates it; options 209, 618..620) */
    LM countRange;              /* +0xF4 particles per explosion (option 551) */
    int keepsGroups;            /* +0xFC emit into its own rip / blip groups and subscribe it (option 617) */
    void *pripg;                /* +0x100 its rip group (forgotten when that object goes away: HandleExplsMessage) */
    void *pblipg;               /* +0x104 its blip group */
    int unk108[2];
    virtual void HandleMessage(MSGID msgid, void *pv) __asm__("HandleExplsMessage");   /* slot 5 */
    virtual CLASSINFO *GetClassInfo() __asm__("GetExplsClassDesc");   /* slot 6 */
    virtual void ExplodeExplso(EXPLSO *pexplso) __asm__("ExplodeExplsExplso");   /* slot 51 */
    virtual void FireExplso(EXPLSO *pexplso) __asm__("FireExplsExplso");   /* slot 52 */
};
typedef char assert_EXPLS_size[sizeof(EXPLS) == 0x110 ? 1 : -1];

/* 131 WIDGET131 (0x140): WIDGET: episode menu; vtable 0x2CEEB8, 50 slots */
struct WIDGET131 : WIDGET
{
    /* the menu (episodemenu.cpp; g_sw.pepmenu points at it) */
    int episode;                 /* +0x110 selected episode */
    int nextEpisode;             /* +0x114 episode being scrolled to */
    int lastEpisode;             /* +0x118 last selectable episode */
    int menuState;               /* +0x11C EPMENU_* */
    int menuSub;                 /* +0x120 */
    float menuStateTime;         /* +0x124 real time of the last menu state change */
    int chapter;                 /* +0x128 */
    int promptState;             /* +0x12C */
    float promptTime;            /* +0x130 */
    char pad134[0x140 - 0x134];
    virtual void Init() __asm__("InitEpisodeMenu");   /* slot 7 */
    virtual CLASSINFO *GetClassInfo() __asm__("GetWidget131ClassDesc");   /* slot 6 */
    virtual void PostLoad() __asm__("PostEpisodeMenuLoad");   /* slot 15 */
    virtual void Update() __asm__("UpdateEpisodeMenu");   /* slot 16 */
};
typedef char assert_WIDGET131_size[sizeof(WIDGET131) == 0x140 ? 1 : -1];
typedef char assert_WIDGET131_promptTime[SLY2_OFFSETOF(WIDGET131, promptTime) == 0x130 ? 1 : -1];

/* 116 SCREENFADE (was CID116; 0x70): screen tint / fill (render kinds 14 and 1); vtable 0x2CF058, 48 slots */
struct SCREENFADE : LO
{
    /* fields named from screenfade.cpp's FADEV view and CreateSwScreenTint (sw.cpp) */
    RGBA rgbaFadeIn;     /* +0x50 colour the fade starts from */
    RGBA rgbaHold;       /* +0x54 colour held between the two blends */
    RGBA rgbaFadeOut;    /* +0x58 colour the fade ends at */
    float dtFadeIn;      /* +0x5C phase durations, seconds */
    float dtHold;
    float dtFadeOut;
    float tStart;        /* +0x68 clock time when the fade started */
    int renderKind;      /* +0x6C 0: render pass 1, 1: render pass 14 */
    virtual void Init() __asm__("InitScreenFade");   /* slot 7 */
    virtual CLASSINFO *GetClassInfo() __asm__("GetCid116ClassDesc");   /* slot 6 */
    virtual void Update() __asm__("UpdateScreenFade");   /* slot 16 */
    virtual void RenderSelf() __asm__("RenderScreenFade");   /* slot 19 */
};
typedef char assert_CID116_size[sizeof(SCREENFADE) == 0x70 ? 1 : -1];
typedef char assert_CID116_renderKind[SLY2_OFFSETOF(SCREENFADE, renderKind) == 0x6C ? 1 : -1];

/* 57 GOMER (0x1010): instances are swarmers; vtable 0x2CF520, 89 slots */
struct GOMER : STEPGUARD
{
    virtual void Init() __asm__("RenderGomerSelf__FP5GOMERP2CMP2RO");   /* slot 7 */
    virtual CLASSINFO *GetClassInfo() __asm__("GetGomerClassDesc");   /* slot 6 */
};
typedef char assert_GOMER_size[sizeof(GOMER) == 0x1010 ? 1 : -1];

/* 113 CID113 (0x60): guard script; vtable 0x2CF880, 48 slots */
struct CID113 : LO
{
    DL commands;            /* +0x50 [S] the SSGE command list (InitGuardScript: link at +0x18); PostStepguardLoad keeps the head as the guard's home record */
    char pad5C[0x60 - 0x5C];
    virtual void PostLoad() __asm__("PostGuardScriptLoad");   /* slot 15 */
    virtual CLASSINFO *GetClassInfo() __asm__("GetCid113ClassDesc");   /* slot 6 */
    virtual void Init() __asm__("InitGuardScript");   /* slot 7 */
};
typedef char assert_CID113_size[sizeof(CID113) == 0x60 ? 1 : -1];
typedef char assert_CID113_commands[SLY2_OFFSETOF(CID113, commands) == 0x50 ? 1 : -1];

/* 169 BHV169 (0x60): guard-script runner (flag 0x21); vtable 0x2CF7F8, 15 slots */
struct BHV169 : BHV
{
    CID113 *script;         /* +0x30 [S] the script being run */
    LO *context;            /* +0x34 [S] the object script oids are looked up relative to */
    SSGE *currentCommand;   /* +0x38 [S] */
    float commandStartTime; /* +0x3C [S] */
    int isCommandBusy;      /* +0x40 [S] a child behaviour runs the current command */
    SSGRESUME *resume;      /* +0x44 [S] the resume point SetBhvScriptScript starts from (cleared after) */
    float heading;          /* +0x48 [S] */
    int allowPartialRoute;  /* +0x4C [S] passed to the path/goto children; also blocks special states (CanSetStepguardSpecial) */
    ANIMBINDING animBinding; /* +0x50 [S] the binding command 14 plays */
    virtual void OnPush() __asm__("InitBhvScript");   /* slot 7 */
    virtual CLASSINFO *GetClassInfo() __asm__("GetBhv169ClassDesc");   /* slot 6 */
    virtual void Close() __asm__("CloseBhvScript");   /* slot 9 */
    virtual void Update() __asm__("UpdateBhvScript");   /* slot 10 */
    virtual void ExecuteScriptCommand(SSGE *pssge) __asm__("ExecuteBhvScriptCommand");   /* slot 15: starts a command (SetBhvScriptScript, UpdateBhvScript) */
};
typedef char assert_BHV169_animBinding[SLY2_OFFSETOF(BHV169, animBinding) == 0x50 ? 1 : -1];
typedef char assert_BHV169_size[sizeof(BHV169) == 0x60 ? 1 : -1];

/* 171 BHV171 (0xE0): suspicious/search mode (flag 0x1F); vtable 0x2CFA10, 14 slots */
/* One search point of BHV171: the candidate (pulled 200 out from the guard) and the original hiding place. */
struct BHV171POINT
{
    qword candidate;        /* +0x00 */
    qword original;         /* +0x10 */
};
struct BHV171 : BHV
{
    int state;              /* +0x30 -1 idle, 0..5 (UpdateBhvSearch) */
    float stateTime;        /* +0x34 */
    float lookEndTime;      /* +0x38 */
    float nextTime;         /* +0x3C give-up time in state 2, next search point time in state 3 */
    float unknown40;
    NOISE *watched;      /* +0x44 the stimulus being investigated (SetBhvSearchWatched; counted: slots 1/2) */
    int needsReplan;        /* +0x48 */
    int unknown4C;
    BHV171POINT searchPoints[4]; /* +0x50 (ChooseBhvSearchPoints) */
    unsigned int searchPointIndex; /* +0xD0 */
    int unknownD4[3];
    virtual void Start() __asm__("StartBhvSearch");   /* slot 8 */
    virtual CLASSINFO *GetClassInfo() __asm__("GetBhv171ClassDesc");   /* slot 6 */
    virtual void OnPush() __asm__("InitBhvSearch");   /* slot 7 */
    virtual void Close() __asm__("CloseBhvSearch");   /* slot 9 */
    virtual void Update() __asm__("UpdateBhvSearch");   /* slot 10 */
};
typedef char assert_BHV171_size[sizeof(BHV171) == 0xE0 ? 1 : -1];

/* A spawn selector of SPAWNER (0x1C bytes, spawner.cpp): kind 0 an LO spot, 1 a curve (walked with a one-command
   script), 2 a small LO spot. */
struct SPAWNSELECTOR
{
    int kind;               /* +0x00 */
    int oid;                /* +0x04 */
    LO *object;             /* +0x08 */
    CID113 *script;         /* +0x0C kind 1: the script made for the curve (cid 0x71) */
    SSGE *scriptCommand;    /* +0x10 kind 1: its command (the direction in its +0x8) */
    int isBusy;             /* +0x14 a spawned guard uses it */
    float busyUntil;        /* +0x18 */
};
typedef char assert_SPAWNSELECTOR_size[sizeof(SPAWNSELECTOR) == 0x1C ? 1 : -1];
/* A spawned-guard slot of SPAWNER (0x30 bytes): status 0 free (ready at readyTime), 1 waiting for the player to move
   away before re-arming. */
struct SPAWNENTRY
{
    STEPGUARD *guard;       /* +0x00 */
    float activeTime;       /* +0x04 */
    float readyTime;        /* +0x08 */
    SPAWNSELECTOR *selector; /* +0x0C */
    int status;             /* +0x10 */
    int unknown14[3];
    qword position;         /* +0x20 the focus position when the respawn delay was armed */
};
typedef char assert_SPAWNENTRY_size[sizeof(SPAWNENTRY) == 0x30 ? 1 : -1];

/* 119 SPAWNER (was CID119; 0xB0): off-screen guard spawner; vtable 0x2CFA90, 48 slots */
struct SPAWNER : LO
{
    int selectorCount;      /* +0x50 */
    SPAWNSELECTOR *selectors; /* +0x54 (at most 128) */
    int entryCount;         /* +0x58 slots in use */
    int entryCapacity;      /* +0x5C */
    SPAWNENTRY *entries;    /* +0x60 */
    int unknown64[3];
    qword lastSpawnPosition; /* +0x70 the focus position at the last spawn */
    float planarRange;      /* +0x80 5500: farther points do not count */
    int useEdgeTest;        /* +0x84 */
    int useRangeTest;       /* +0x88 */
    int useOcclusionTest;   /* +0x8C */
    int templateCount;      /* +0x90 guard templates (at most 8) */
    LO **templates;         /* +0x94 */
    int offsetCount;        /* +0x98 test offsets around a spawn point (at most 16) */
    qword *offsets;         /* +0x9C */
    float respawnDelay;     /* +0xA0 */
    float respawnDelay2;    /* +0xA4 the selector's */
    float nextSpawnTime;    /* +0xA8 */
    int mode;               /* +0xAC 0 never while the hero has an alert, 1 only shortly after it, 2 always */
    virtual void Init() __asm__("InitSpawner");   /* slot 7 */
    virtual CLASSINFO *GetClassInfo() __asm__("GetCid119ClassDesc");   /* slot 6 */
    virtual void RemoveHierarchy() __asm__("RemoveSpawnerHierarchy");   /* slot 12 */
    virtual void LoadFromBrx(CBinaryInputStream *pbis) __asm__("LoadSpawnerFromBrx");   /* slot 14 */
    virtual void PostLoad() __asm__("PostSpawnerLoad");   /* slot 15 */
    virtual void Update() __asm__("UpdateSpawner");   /* slot 16 */
};
typedef char assert_CID119_size[sizeof(SPAWNER) == 0xB0 ? 1 : -1];

/* 157 BHVANIM (0x50): animation/locomotion leaf (flag 9); vtable 0x2D0AB0, 14 slots */
struct BHVANIM : BHV
{
    int state;              /* +0x30 0 playing, 1 done (-1 before Start) */
    float stateTime;        /* +0x34 time of the last state change */
    LO *lookAt;             /* +0x38 object to face (UpdateBhvanimMotion) */
    int sgf;                /* +0x3C the guard flag it raises while active */
    ANIMBINDING *binding;   /* +0x40 the animation it plays (FindCurrentAnimBinding when not preset) */
    int unknown44[3];
    virtual void Close() __asm__("CloseBhvanim");   /* slot 9 */
    virtual CLASSINFO *GetClassInfo() __asm__("GetBhvanimClassDesc");   /* slot 6 */
    virtual void OnPush() __asm__("InitBhvanim");   /* slot 7 */
    virtual void Start() __asm__("StartBhvanim");   /* slot 8 */
    virtual void Update() __asm__("UpdateBhvanim");   /* slot 10 */
    virtual void UpdateMotion() __asm__("UpdateBhvanimMotion");   /* slot 11 */
    virtual int GetGroundProbeFlags() __asm__("GetBhvanimGroundProbeFlags");   /* slot 12 */
    virtual int HandleBehaviorMessage(int msgid, void *pv) __asm__("HandleBhvanimMessage");   /* slot 13 */
};
typedef char assert_BHVANIM_size[sizeof(BHVANIM) == 0x50 ? 1 : -1];

/* 158 BHVIDLE (0x50): idle leaf (flag 2); vtable 0x2D0A30, 14 slots */
struct BHVIDLE : BHVANIM
{
    virtual void OnPush() __asm__("InitBhvidle");   /* slot 7 */
    virtual CLASSINFO *GetClassInfo() __asm__("GetBhvidleClassDesc");   /* slot 6 */
    virtual void Start() __asm__("StartBhvidle");   /* slot 8 */
    virtual void Close() __asm__("CloseBhvidle");   /* slot 9 */
    virtual int HandleBehaviorMessage(int msgid, void *pv) __asm__("HandleBhvidleMessage");   /* slot 13 */
};
typedef char assert_BHVIDLE_size[sizeof(BHVIDLE) == 0x50 ? 1 : -1];

/* 160 BHVHIT (0xC0): hit reaction; vtable 0x2D09B0, 14 slots */
struct BHVHIT : BHVANIM
{
    union {
        ZPR zpr;            /* +0x50 the zap that caused the reaction (SetBhvguardMode copies the mode's zap) */
        struct {
            LO *attacker;   /* +0x50 the zap's source */
            int unknown54[2];
            int zapKind;    /* +0x5C */
            int unknown60[20];
        };
    };
    float cameraJoltTime;   /* +0xB0 */
    float zapModeTime;      /* +0xB4 */
    int unknownB8[2];
    virtual void Close() __asm__("CloseBhvhit");   /* slot 9 */
    virtual CLASSINFO *GetClassInfo() __asm__("GetBhvhitClassDesc");   /* slot 6 */
    virtual void Start() __asm__("StartBhvhit");   /* slot 8 */
    virtual void Update() __asm__("UpdateBhvhit");   /* slot 10 */
};
typedef char assert_BHVHIT_size[sizeof(BHVHIT) == 0xC0 ? 1 : -1];

/* 161 BHV161 (0xC0); vtable 0x2D0930, 14 slots */
struct BHV161 : BHVHIT
{
    virtual void OnPush() __asm__("InitBhvHurt");   /* slot 7 */
    virtual CLASSINFO *GetClassInfo() __asm__("GetBhv161ClassDesc");   /* slot 6 */
    virtual void Start() __asm__("StartBhvHurt");   /* slot 8 */
};
typedef char assert_BHV161_size[sizeof(BHV161) == 0xC0 ? 1 : -1];

/* 163 BHVKO (0xC0): KO / dead (flag 7); vtable 0x2D08B0, 14 slots */
struct BHVKO : BHVHIT
{
    virtual void Close() __asm__("CloseBhvko");   /* slot 9 */
    virtual CLASSINFO *GetClassInfo() __asm__("GetBhvkoClassDesc");   /* slot 6 */
    virtual void OnPush() __asm__("InitBhvko");   /* slot 7 */
    virtual void Start() __asm__("StartBhvko");   /* slot 8 */
};
typedef char assert_BHVKO_size[sizeof(BHVKO) == 0xC0 ? 1 : -1];

/* 162 BHV162 (0xC0); vtable 0x2D0830, 14 slots */
struct BHV162 : BHVHIT
{
    virtual void OnPush() __asm__("InitBhvKnockdown");   /* slot 7 */
    virtual CLASSINFO *GetClassInfo() __asm__("GetBhv162ClassDesc");   /* slot 6 */
    virtual void Start() __asm__("StartBhvKnockdown");   /* slot 8 */
    virtual void Close() __asm__("CloseBhvKnockdown");   /* slot 9 */
};
typedef char assert_BHV162_size[sizeof(BHV162) == 0xC0 ? 1 : -1];

/* 159 BHVSTRIKE (0x60): attack-animation leaf (flag 3); vtable 0x2D07B0, 14 slots */
struct BHVSTRIKE : BHVANIM
{
    int isLeapEnabled;      /* +0x50 */
    int unknown54[3];
    virtual void Start() __asm__("StartBhvstrike");   /* slot 8 */
    virtual CLASSINFO *GetClassInfo() __asm__("GetBhvstrikeClassDesc");   /* slot 6 */
    virtual void OnPush() __asm__("InitBhvstrike");   /* slot 7 */
    virtual void Close() __asm__("CloseBhvstrike");   /* slot 9 */
    virtual void Update() __asm__("UpdateBhvstrike");   /* slot 10 */
    virtual int HandleBehaviorMessage(int msgid, void *pv) __asm__("HandleBhvstrikeMessage");   /* slot 13 */
};
typedef char assert_BHVSTRIKE_size[sizeof(BHVSTRIKE) == 0x60 ? 1 : -1];

/* 166 BHVPATH (0xC0): path base; vtable 0x2D0728, 15 slots */
struct BHVPATH : BHV
{
    SHAPE *shape;           /* +0x30 the path shape followed */
    float pathParam;        /* +0x34 position along it */
    int isReversed;         /* +0x38 */
    int hasEnd;             /* +0x3C stops at endParam */
    int ignoresBlockers;    /* +0x40 */
    float endParam;         /* +0x44 */
    float startParamHint;   /* +0x48 */
    int state;              /* +0x4C (3 finished) */
    float stateTime;        /* +0x50 */
    struct OBSTACLE *blocker;  /* +0x54 the convex obstacle blocking the path (FindShapeFreePoint's result) */
    int isBlocked;          /* +0x58 */
    float blockerParam;     /* +0x5C */
    float blockerRange;     /* +0x60 */
    float arrivalRadius;    /* +0x64 */
    float nearRadius;       /* +0x68 */
    float farRadius;        /* +0x6C */
    float separation;       /* +0x70 */
    int unknown74[3];
    qword startPosition;    /* +0x80 */
    qword velocity;         /* +0x90 */
    int isInitialized;      /* +0xA0 */
    int unknownA4[3];
    qword endPoint;         /* +0xB0 */
    virtual void OnPush() __asm__("InitBhvpath");   /* slot 7 */
    virtual CLASSINFO *GetClassInfo() __asm__("GetBhvpathClassDesc");   /* slot 6 */
    virtual void Start() __asm__("StartBhvpath");   /* slot 8 */
    virtual void Update() __asm__("UpdateBhvpath");   /* slot 10 */
    virtual void SetPathState(int nState) __asm__("SetBhvpathState");   /* slot 15 */
};
typedef char assert_BHVPATH_size[sizeof(BHVPATH) == 0xC0 ? 1 : -1];

/* 168 BHVWALKGOAL (0xE0): player walk-to-goal (writes POCTL stick); vtable 0x2CC4C0, 15 slots */
struct BHVWALKGOAL : BHVPATH
{
    POCTL *mirrorPoctl;     /* +0xC0 controller whose buttons are mirrored (UpdateBhvwalkgoalPoctl) */
    int mirrorButtonMask;   /* +0xC4 */
    float mirrorDelay;      /* +0xC8 */
    int forcesButton8;      /* +0xCC */
    float speedScale;       /* +0xD0 */
    int unknownD4[3];
    virtual void OnPush() __asm__("InitBhvwalkgoal");   /* slot 7 */
    virtual CLASSINFO *GetClassInfo() __asm__("GetBhvwalkgoalClassDesc");   /* slot 6 */
    virtual void Start() __asm__("StartBhvwalkgoal");   /* slot 8 */
    virtual void Update() __asm__("UpdateBhvwalkgoal");   /* slot 10 */
    virtual void UpdateSteering(POCTL *ppoctl) __asm__("UpdateBhvwalkgoalPoctl");   /* slot 14 */
};
typedef char assert_BHVWALKGOAL_size[sizeof(BHVWALKGOAL) == 0xE0 ? 1 : -1];

/* 193 BHV193 (0x50): player scripted go-to; vtable 0x2CC440, 14 slots */
struct BHV193 : BHV
{
    int usesStraightPath;   /* +0x30 */
    float arrivalRadius;    /* +0x34 */
    int forcesButton8;      /* +0x38 */
    float speedScale;       /* +0x3C */
    qword goal;             /* +0x40 */
    virtual void OnPush() __asm__("InitBhvWalkTo");   /* slot 7 */
    virtual CLASSINFO *GetClassInfo() __asm__("GetBhv193ClassDesc");   /* slot 6 */
    virtual void Start() __asm__("StartBhvWalkTo");   /* slot 8 */
};
typedef char assert_BHV193_size[sizeof(BHV193) == 0x50 ? 1 : -1];

/* 194 BHVFOLLOW (0xA0): player follow leader (StartStepplayerFollow); vtable 0x2CC3C0, 14 slots */
struct BHVFOLLOW : BHV
{
    PO *leader;             /* +0x30 the followed player */
    int unknown34[3];
    qword followOffset;     /* +0x40 */
    qword followPoint;      /* +0x50 */
    int state;              /* +0x60 */
    float stateTime;        /* +0x64 */
    float velocityLeadTime; /* +0x68 */
    float distanceNear;     /* +0x6C */
    float distanceFar;      /* +0x70 */
    float speedNear;        /* +0x74 */
    float speedFar;         /* +0x78 */
    int unknown7C;
    qword landingPoint;     /* +0x80 */
    SO *threat;             /* +0x90 the object the interact behaviour is pushed against */
    int unknown94[3];
    virtual void Start() __asm__("StartBhvfollow");   /* slot 8 */
    virtual CLASSINFO *GetClassInfo() __asm__("GetBhvfollowClassDesc");   /* slot 6 */
    virtual void OnPush() __asm__("InitBhvfollow");   /* slot 7 */
    virtual void Close() __asm__("CloseBhvfollow");   /* slot 9 */
    virtual void Update() __asm__("UpdateBhvfollow");   /* slot 10 */
};
typedef char assert_BHVFOLLOW_size[sizeof(BHVFOLLOW) == 0xA0 ? 1 : -1];

/* 195 BHV195 (0x40); vtable 0x2CC340, 14 slots */
struct BHV195 : BHV
{
    int state;              /* +0x30 */
    float stateTime;        /* +0x34 */
    TARGET *target;         /* +0x38 the interaction target */
    int unknown3C;
    virtual void Close() __asm__("CloseBhvInteract");   /* slot 9 */
    virtual CLASSINFO *GetClassInfo() __asm__("GetBhv195ClassDesc");   /* slot 6 */
    virtual void OnPush() __asm__("InitBhvInteract");   /* slot 7 */
    virtual void Start() __asm__("StartBhvInteract");   /* slot 8 */
    virtual void Update() __asm__("UpdateBhvInteract");   /* slot 10 */
    virtual void UpdateSteering(POCTL *ppoctl) __asm__("SteerBhvInteract");   /* slot 14 */
};
typedef char assert_BHV195_size[sizeof(BHV195) == 0x40 ? 1 : -1];

/* 196 BHV196 (0x40); vtable 0x2CC2C0, 14 slots */
struct BHV196 : BHV
{
    int state;              /* +0x30 */
    float stateTime;        /* +0x34 */
    TARGET *target;         /* +0x38 the interaction target */
    int unknown3C;
    virtual void Close() __asm__("CloseBhvInteract2");   /* slot 9 */
    virtual CLASSINFO *GetClassInfo() __asm__("GetBhv196ClassDesc");   /* slot 6 */
    virtual void OnPush() __asm__("InitBhvInteract2");   /* slot 7 */
    virtual void Start() __asm__("StartBhvInteract2");   /* slot 8 */
    virtual void Update() __asm__("UpdateBhvInteract2");   /* slot 10 */
    virtual void UpdateSteering(POCTL *ppoctl) __asm__("SteerBhvInteract2");   /* slot 14 */
};
typedef char assert_BHV196_size[sizeof(BHV196) == 0x40 ? 1 : -1];

/* 197 BHV197 (0x40); vtable 0x2CC240, 14 slots */
struct BHV197 : BHV
{
    int state;              /* +0x30 */
    float stateTime;        /* +0x34 */
    TARGET *target;         /* +0x38 the interaction target */
    int unknown3C;
    virtual void Close() __asm__("CloseBhvInteract3");   /* slot 9 */
    virtual CLASSINFO *GetClassInfo() __asm__("GetBhv197ClassDesc");   /* slot 6 */
    virtual void OnPush() __asm__("InitBhvInteract3");   /* slot 7 */
    virtual void Start() __asm__("StartBhvInteract3");   /* slot 8 */
    virtual void Update() __asm__("UpdateBhvInteract3");   /* slot 10 */
    virtual void UpdateSteering(POCTL *ppoctl) __asm__("SteerBhvInteract3");   /* slot 14 */
};
typedef char assert_BHV197_size[sizeof(BHV197) == 0x40 ? 1 : -1];

/* 198 BHV198 (0x40): Murray behaviour; vtable 0x2CC1C0, 14 slots */
struct BHV198 : BHV
{
    float startHeading;     /* +0x30 */
    int unknown34[3];
    virtual void Start() __asm__("StartBhvMurray");   /* slot 8 */
    virtual CLASSINFO *GetClassInfo() __asm__("GetBhv198ClassDesc");   /* slot 6 */
    virtual void Close() __asm__("CloseBhvMurray");   /* slot 9 */
    virtual void Update() __asm__("UpdateBhvMurray");   /* slot 10 */
};
typedef char assert_BHV198_size[sizeof(BHV198) == 0x40 ? 1 : -1];

/* 167 BHVWALKPATH (0xF0): path/goal follower; vtable 0x2D06A0, 15 slots */
struct BHVWALKPATH : BHVPATH
{
    int animSgf;            /* +0xC0 the guard flag of the walk animation */
    float rate;             /* +0xC4 */
    LO *lookAtTarget;       /* +0xC8 */
    int unknownCC;
    qword stuckCheckPos;    /* +0xD0 */
    float stuckTimer;       /* +0xE0 */
    int unknownE4[3];
    virtual void OnPush() __asm__("InitBhvwalkpath");   /* slot 7 */
    virtual CLASSINFO *GetClassInfo() __asm__("GetBhvwalkpathClassDesc");   /* slot 6 */
    virtual void Update() __asm__("UpdateBhvwalkpath");   /* slot 10 */
    virtual void SetPathState(int nState) __asm__("SetBhvwalkpathState");   /* slot 15 */
};
typedef char assert_BHVWALKPATH_size[sizeof(BHVWALKPATH) == 0xF0 ? 1 : -1];

/* 164 BHVATTACK (0x50): attack execution (flag 0x18); vtable 0x2D0620, 14 slots */
struct BHVATTACK : BHV
{
    LO *victim;             /* +0x30 the attacked object */
    int state;              /* +0x34 */
    float stateTime;        /* +0x38 */
    CID120 *attack;         /* +0x3C the attack definition being run */
    int isHitConfirmed;     /* +0x40 set by message 0xB from the victim's guard */
    int unknown44[3];
    virtual void Start() __asm__("StartBhvattack");   /* slot 8 */
    virtual CLASSINFO *GetClassInfo() __asm__("GetBhvattackClassDesc");   /* slot 6 */
    virtual void OnPush() __asm__("InitBhvattack");   /* slot 7 */
    virtual void Close() __asm__("CloseBhvattack");   /* slot 9 */
    virtual void Update() __asm__("UpdateBhvattack");   /* slot 10 */
    virtual int HandleBehaviorMessage(int msgid, void *pv) __asm__("HandleBhvattackMessage");   /* slot 13 */
};
typedef char assert_BHVATTACK_size[sizeof(BHVATTACK) == 0x50 ? 1 : -1];

/* 165 BHVCOMBAT (0x60): alert/combat mode (flag 0x1D); vtable 0x2D05A0, 14 slots */
struct BHVCOMBAT : BHV
{
    LO *target;             /* +0x30 */
    int state;              /* +0x34 */
    float stateTime;        /* +0x38 */
    CID120 *attack;         /* +0x3C the chosen attack */
    int isHitConfirmed;     /* +0x40 */
    struct CID17 *pathzone; /* +0x44 the path zone of the target position the pursuit was last goaled at */
    int unknown48[2];
    qword cachedPosition;   /* +0x50 */
    virtual void Close() __asm__("CloseBhvcombat");   /* slot 9 */
    virtual CLASSINFO *GetClassInfo() __asm__("GetBhvcombatClassDesc");   /* slot 6 */
    virtual void OnPush() __asm__("InitBhvcombat");   /* slot 7 */
    virtual void Start() __asm__("StartBhvcombat");   /* slot 8 */
    virtual void Update() __asm__("UpdateBhvcombat");   /* slot 10 */
};
typedef char assert_BHVCOMBAT_size[sizeof(BHVCOMBAT) == 0x60 ? 1 : -1];

/* 170 BHVPATROL (0x50): calm/patrol mode (flag 0x1E); vtable 0x2D0520, 14 slots */
struct BHVPATROL : BHV
{
    float walkEndTime;      /* +0x30 */
    float pauseEndTime;     /* +0x34 */
    int state;              /* +0x38 */
    float stateTime;        /* +0x3C */
    CID113 *script;         /* +0x40 the patrol script being run */
    int unknown44[3];
    virtual void Close() __asm__("CloseBhvpatrol");   /* slot 9 */
    virtual CLASSINFO *GetClassInfo() __asm__("GetBhvpatrolClassDesc");   /* slot 6 */
    virtual void OnPush() __asm__("InitBhvpatrol");   /* slot 7 */
    virtual void Start() __asm__("StartBhvpatrol");   /* slot 8 */
    virtual void Update() __asm__("UpdateBhvpatrol");   /* slot 10 */
};
typedef char assert_BHVPATROL_size[sizeof(BHVPATROL) == 0x50 ? 1 : -1];

/* 156 BHVGUARD (0xB0): guard root: awareness mode +0x30; vtable 0x2D04A0, 14 slots */
struct BHVGUARD : BHV
{
    BHVGUARDREC mode;       /* +0x30 the current mode */
    float modeTime;         /* +0xA0 when the mode was entered */
    float watchEndTime;     /* +0xA4 */
    int unknownA8[2];
    virtual void OnPush() __asm__("InitBhvguard");   /* slot 7 */
    virtual CLASSINFO *GetClassInfo() __asm__("GetBhvguardClassDesc");   /* slot 6 */
    virtual void Start() __asm__("StartBhvguard");   /* slot 8 */
    virtual void Update() __asm__("UpdateBhvguard");   /* slot 10 */
    virtual int HandleBehaviorMessage(int msgid, void *pv) __asm__("HandleBhvguardMessage");   /* slot 13 */
};
typedef char assert_BHVGUARD_size[sizeof(BHVGUARD) == 0xB0 ? 1 : -1];

/* 176 BHV176 (0x40); vtable 0x2D0420, 14 slots */
struct BHV176 : BHV
{
    int state;              /* +0x30 */
    float stateTime;        /* +0x34 */
    float lastYellTime;     /* +0x38 */
    int thrower;            /* +0x3C id of the object that threw the guard (detector target id) */
    virtual void Close() __asm__("CloseBhvThrown");   /* slot 9 */
    virtual CLASSINFO *GetClassInfo() __asm__("GetBhv176ClassDesc");   /* slot 6 */
    virtual void OnPush() __asm__("InitBhvThrown");   /* slot 7 */
    virtual void Start() __asm__("StartBhvThrown");   /* slot 8 */
    virtual void Update() __asm__("UpdateBhvThrown");   /* slot 10 */
};
typedef char assert_BHV176_size[sizeof(BHV176) == 0x40 ? 1 : -1];

/* 177 BHVJUGGLED (0x70): juggled / carried (flag 0x17); vtable 0x2D03A0, 14 slots. A BHVANIM: its OnPush/Start/Close/
   Update call the BHVANIM ones and slot 11 is BHVANIM's UpdateBhvanimMotion. */
struct BHVJUGGLED : BHVANIM
{
    int juggleState;        /* +0x50 own sub-state (-1 before Start; the BHVANIM state is +0x30) */
    float stateTime;        /* +0x54 */
    int hitCount;           /* +0x58 */
    SO *holder;             /* +0x5C the object holding the guard (its physicsTree groups the intersections ShouldStepguardIgnoreIntersection skips) */
    int unknown60;
    void *holdAct;          /* +0x64 */
    int unknown68[2];
    virtual void Close() __asm__("CloseBhvjuggled");   /* slot 9 */
    virtual CLASSINFO *GetClassInfo() __asm__("GetBhvjuggledClassDesc");   /* slot 6 */
    virtual void OnPush() __asm__("InitBhvjuggled");   /* slot 7 */
    virtual void Start() __asm__("StartBhvjuggled");   /* slot 8 */
    virtual void Update() __asm__("UpdateBhvjuggled");   /* slot 10 */
    virtual int GetGroundProbeFlags() __asm__("GetBhvjuggledGroundProbeFlags");   /* slot 12 */
    virtual int HandleBehaviorMessage(int msgid, void *pv) __asm__("HandleBhvjuggledMessage");   /* slot 13 */
};
typedef char assert_BHVJUGGLED_size[sizeof(BHVJUGGLED) == 0x70 ? 1 : -1];

/* 172 BHVLEAP (0xA0): leap (flag 0xF); vtable 0x2D0320, 14 slots. A BHVANIM: OnPush/Start/Close/Update call the
   BHVANIM ones and slots 11-13 are BHVANIM's. The binding's payload (+0x8) is the leap driver. */
struct BHVLEAP : BHVANIM
{
    qword landingPoint;     /* +0x50 the point leapt to (snapped to the ground, or the attach point) */
    LO *target;             /* +0x60 the object leapt onto (shape/rail/pipe/attachable; NULL: the ground) */
    int isReversed;         /* +0x64 (rail/pipe direction) */
    int extra;              /* +0x68 passed to the leap driver */
    int hasAttachBasis;     /* +0x6C */
    MATRIX3 attachBasis;    /* +0x70 orientation on the target */
    virtual void Close() __asm__("CloseBhvleap");   /* slot 9 */
    virtual CLASSINFO *GetClassInfo() __asm__("GetBhvleapClassDesc");   /* slot 6 */
    virtual void OnPush() __asm__("InitBhvleap");   /* slot 7 */
    virtual void Start() __asm__("StartBhvleap");   /* slot 8 */
    virtual void Update() __asm__("UpdateBhvleap");   /* slot 10 */
};
typedef char assert_BHVLEAP_size[sizeof(BHVLEAP) == 0xA0 ? 1 : -1];

/* 175 BHV175 (0x60): sleep; vtable 0x2D02A0, 14 slots. A BHVANIM: OnPush/Start/Close/Update call the BHVANIM ones and
   slots 11-13 are BHVANIM's. */
struct BHV175 : BHVANIM
{
    int sleepState;         /* +0x50 own sub-state: 0 asleep, 1 waking (-1 before Start) */
    float sleepStateF;      /* +0x54 the sub-state again, as a float (SetBhvSleepState) */
    float wakeTime;         /* +0x58 */
    int unknown5C;
    virtual void OnPush() __asm__("InitBhvSleep");   /* slot 7 */
    virtual CLASSINFO *GetClassInfo() __asm__("GetBhv175ClassDesc");   /* slot 6 */
    virtual void Start() __asm__("StartBhvSleep");   /* slot 8 */
    virtual void Close() __asm__("CloseBhvSleep");   /* slot 9 */
    virtual void Update() __asm__("UpdateBhvSleep");   /* slot 10 */
};
typedef char assert_BHV175_size[sizeof(BHV175) == 0x60 ? 1 : -1];

/* 173 BHVSHOOT (0x40): SHOOT: ranged attack (BHVSHOOT); vtable 0x2D0220, 14 slots */
struct BHVSHOOT : BHV
{
    int state;              /* +0x30 */
    float stateTime;        /* +0x34 */
    LO *target;             /* +0x38 */
    int unknown3C;
    virtual void Start() __asm__("StartBhvshoot");   /* slot 8 */
    virtual CLASSINFO *GetClassInfo() __asm__("GetBhvshootClassDesc");   /* slot 6 */
    virtual void OnPush() __asm__("InitBhvshoot");   /* slot 7 */
    virtual void Close() __asm__("CloseBhvshoot");   /* slot 9 */
    virtual void Update() __asm__("UpdateBhvshoot");   /* slot 10 */
    virtual int HandleBehaviorMessage(int msgid, void *pv) __asm__("HandleBhvshootMessage");   /* slot 13 */
};
typedef char assert_BHVSHOOT_size[sizeof(BHVSHOOT) == 0x40 ? 1 : -1];

/* 178 BHV178 (0x50); vtable 0x2D01A0, 14 slots */
struct BHV178 : BHV
{
    int state;              /* +0x30 */
    float stateTime;        /* +0x34 */
    LO *preferredTarget;    /* +0x38 the cheapest designated target (ChooseBhvTargetsTarget) */
    LO *nearAvoidObject;    /* +0x3C an avoid object within 800 that wins over it */
    LO *chosenTarget;       /* +0x40 the one fought (BHVCOMBAT target) */
    float nextChooseTime;   /* +0x44 */
    int unknown48[2];
    virtual void Start() __asm__("StartBhvTargets");   /* slot 8 */
    virtual CLASSINFO *GetClassInfo() __asm__("GetBhv178ClassDesc");   /* slot 6 */
    virtual void OnPush() __asm__("InitBhvTargets");   /* slot 7 */
    virtual void Close() __asm__("CloseBhvTargets");   /* slot 9 */
    virtual void Update() __asm__("UpdateBhvTargets");   /* slot 10 */
};
typedef char assert_BHV178_size[sizeof(BHV178) == 0x50 ? 1 : -1];

/* 179 BHV179 (0x40); vtable 0x2D0120, 14 slots */
struct BHV179 : BHV
{
    int state;              /* +0x30 */
    int unknown34[3];
    virtual void Close() __asm__("CloseBhvCircle");   /* slot 9 */
    virtual CLASSINFO *GetClassInfo() __asm__("GetBhv179ClassDesc");   /* slot 6 */
    virtual void OnPush() __asm__("InitBhvCircle");   /* slot 7 */
    virtual void Start() __asm__("StartBhvCircle");   /* slot 8 */
    virtual void Update() __asm__("UpdateBhvCircle");   /* slot 10 */
};
typedef char assert_BHV179_size[sizeof(BHV179) == 0x40 ? 1 : -1];

/* 183 BHV183 (0x70); vtable 0x2D00A0, 14 slots */
struct BHV183 : BHV
{
    LO *escorted;           /* +0x30 the object escorted */
    int unknown34[3];
    qword localOffset;      /* +0x40 the escort position in the escorted object's frame */
    int state;              /* +0x50 */
    float stateTime;        /* +0x54 */
    int unknown58[2];
    qword lastGoalPosition; /* +0x60 */
    virtual void Start() __asm__("StartBhvEscort");   /* slot 8 */
    virtual CLASSINFO *GetClassInfo() __asm__("GetBhv183ClassDesc");   /* slot 6 */
    virtual void OnPush() __asm__("InitBhvEscort");   /* slot 7 */
    virtual void Close() __asm__("CloseBhvEscort");   /* slot 9 */
    virtual void Update() __asm__("UpdateBhvEscort");   /* slot 10 */
};
typedef char assert_BHV183_size[sizeof(BHV183) == 0x70 ? 1 : -1];

/* 184 BHVRAIL (0x40): rail traversal; vtable 0x2D0020, 14 slots */
struct BHVRAIL : BHV
{
    int state;              /* +0x30 */
    float stateTime;        /* +0x34 */
    float animSpeed;        /* +0x38 */
    float goalParam;        /* +0x3C position along the rail to reach */
    virtual void Start() __asm__("StartBhvrail");   /* slot 8 */
    virtual CLASSINFO *GetClassInfo() __asm__("GetBhvrailClassDesc");   /* slot 6 */
    virtual void OnPush() __asm__("InitBhvrail");   /* slot 7 */
    virtual void Close() __asm__("CloseBhvrail");   /* slot 9 */
    virtual void Update() __asm__("UpdateBhvrail");   /* slot 10 */
};
typedef char assert_BHVRAIL_size[sizeof(BHVRAIL) == 0x40 ? 1 : -1];

/* 185 BHVPIPE (0x40): pipe traversal; vtable 0x2CFFA0, 14 slots */
struct BHVPIPE : BHV
{
    int state;              /* +0x30 */
    float stateTime;        /* +0x34 */
    float goalParam;        /* +0x38 position along the pipe to reach */
    float smoothedSpeed;    /* +0x3C */
    virtual void Start() __asm__("StartBhvpipe");   /* slot 8 */
    virtual CLASSINFO *GetClassInfo() __asm__("GetBhvpipeClassDesc");   /* slot 6 */
    virtual void OnPush() __asm__("InitBhvpipe");   /* slot 7 */
    virtual void Close() __asm__("CloseBhvpipe");   /* slot 9 */
    virtual void Update() __asm__("UpdateBhvpipe");   /* slot 10 */
};
typedef char assert_BHVPIPE_size[sizeof(BHVPIPE) == 0x40 ? 1 : -1];

/* 187 BHV187 (0x30); vtable 0x2CFF20, 14 slots */
struct BHV187 : BHV
{
    virtual void OnPush() __asm__("InitBhvAttachCid19");   /* slot 7 */
    virtual CLASSINFO *GetClassInfo() __asm__("GetBhv187ClassDesc");   /* slot 6 */
    virtual void Start() __asm__("StartBhvAttachCid19");   /* slot 8 */
    virtual void Close() __asm__("CloseBhvAttachCid19");   /* slot 9 */
};
typedef char assert_BHV187_size[sizeof(BHV187) == 0x30 ? 1 : -1];

/* 188 BHV188 (0x30); vtable 0x2CFEA0, 14 slots */
struct BHV188 : BHV
{
    virtual void OnPush() __asm__("InitBhvAttachCid39");   /* slot 7 */
    virtual CLASSINFO *GetClassInfo() __asm__("GetBhv188ClassDesc");   /* slot 6 */
    virtual void Start() __asm__("StartBhvAttachCid39");   /* slot 8 */
    virtual void Close() __asm__("CloseBhvAttachCid39");   /* slot 9 */
};
typedef char assert_BHV188_size[sizeof(BHV188) == 0x30 ? 1 : -1];

/* 186 BHVGOTO (0xD0): go-to / look-at tracker (flag 0x1C); vtable 0x2CFE20, 14 slots */
struct BHVGOTO : BHV
{
    int state;              /* +0x30 (1 arrived) */
    float stateTime;        /* +0x34 */
    int sgf;                /* +0x38 the guard flag it holds (-1 none) */
    int unknown3C;
    PATHLINKEND goal;       /* +0x40 (SetBhvgotoGoal) */
    PATHLINKEND waypoint;   /* +0x60 the next waypoint: the goal, or the first route link's endpoint */
    PATHLINKEND relocateGoal; /* +0x80 the relocate goal (BHVRELOCATE) */
    float planTime;         /* +0xA0 time of the last route plan */
    int pathGraphVersion;   /* +0xA4 path graph version the route was planned on */
    LO *lookAtTarget;       /* +0xA8 */
    int unknownAC;
    float gotoParam;        /* +0xB0 [S] set from the script's goto command (SSGE::gotoParam; meaning unknown) */
    int numRouteLinks;      /* +0xB4 */
    union {
        PATHLINK *firstLink;    /* +0xB8 the route's (single) link */
        PATHLINK *route[1];     /* +0xB8 the same as the route array FindPathlinkRoute fills (FindBhvgotoRoute) */
    };
    int isDirect;           /* +0xBC goes straight to the goal (no route) */
    int allowPartialRoute;  /* +0xC0 */
    int unknownC4[3];
    virtual void Close() __asm__("CloseBhvgoto");   /* slot 9 */
    virtual CLASSINFO *GetClassInfo() __asm__("GetBhvgotoClassDesc");   /* slot 6 */
    virtual void OnPush() __asm__("InitBhvgoto");   /* slot 7 */
    virtual void Start() __asm__("StartBhvgoto");   /* slot 8 */
    virtual void Update() __asm__("UpdateBhvgoto");   /* slot 10 */
};
typedef char assert_BHVGOTO_size[sizeof(BHVGOTO) == 0xD0 ? 1 : -1];

/* 192 BHVPATROLPATH (0x60): patrol-route walker (flag 0x2E); vtable 0x2CFDA0, 14 slots */
struct BHVPATROLPATH : BHV
{
    SHAPE *shape;           /* +0x30 the patrol path */
    int isForward;          /* +0x34 */
    float pathParam;        /* +0x38 */
    int unknown3C;
    qword goal;             /* +0x40 */
    LO *lookAtTarget;       /* +0x50 */
    int unknown54;
    int isDirect;           /* +0x58 */
    int unknown5C;
    virtual void Close() __asm__("CloseBhvpatrolpath");   /* slot 9 */
    virtual CLASSINFO *GetClassInfo() __asm__("GetBhvpatrolpathClassDesc");   /* slot 6 */
    virtual void OnPush() __asm__("InitBhvpatrolpath");   /* slot 7 */
    virtual void Start() __asm__("StartBhvpatrolpath");   /* slot 8 */
    virtual void Update() __asm__("UpdateBhvpatrolpath");   /* slot 10 */
};
typedef char assert_BHVPATROLPATH_size[sizeof(BHVPATROLPATH) == 0x60 ? 1 : -1];

/* 189 BHV189 (0x50); vtable 0x2CFD20, 14 slots */
struct BHV189 : BHV
{
    int state;              /* +0x30 */
    float stateTime;        /* +0x34 */
    LO *target;             /* +0x38 */
    float lastRegoalTime;   /* +0x3C */
    float forcedDelay;      /* +0x40 */
    int unknown44[3];
    virtual void Start() __asm__("StartBhvStalk");   /* slot 8 */
    virtual CLASSINFO *GetClassInfo() __asm__("GetBhv189ClassDesc");   /* slot 6 */
    virtual void OnPush() __asm__("InitBhvStalk");   /* slot 7 */
    virtual void Close() __asm__("CloseBhvStalk");   /* slot 9 */
    virtual void Update() __asm__("UpdateBhvStalk");   /* slot 10 */
};
typedef char assert_BHV189_size[sizeof(BHV189) == 0x50 ? 1 : -1];

/* 190 BHVPATHLINK (0x50): pathlink traversal (flag 0x2C); vtable 0x2CFCA0, 14 slots */
struct BHVPATHLINK : BHV
{
    PATHLINK *pathlink;     /* +0x30 the path link being walked */
    int segmentIndex;       /* +0x34 */
    int state;              /* +0x38 */
    float stateTime;        /* +0x3C */
    float segmentTimeout;   /* +0x40 */
    int unknown44[3];
    virtual void Start() __asm__("StartBhvpathlink");   /* slot 8 */
    virtual CLASSINFO *GetClassInfo() __asm__("GetBhvpathlinkClassDesc");   /* slot 6 */
    virtual void OnPush() __asm__("InitBhvpathlink");   /* slot 7 */
    virtual void Close() __asm__("CloseBhvpathlink");   /* slot 9 */
    virtual void Update() __asm__("UpdateBhvpathlink");   /* slot 10 */
};
typedef char assert_BHVPATHLINK_size[sizeof(BHVPATHLINK) == 0x50 ? 1 : -1];

/* 191 BHV191 (0x50); vtable 0x2CFC20, 14 slots */
struct BHV191 : BHV
{
    PATHLINKEND end;
    virtual void Start() __asm__("StartBhvRelocate");   /* slot 8 */
    virtual CLASSINFO *GetClassInfo() __asm__("GetBhv191ClassDesc");   /* slot 6 */
    virtual void OnPush() __asm__("InitBhvRelocate");   /* slot 7 */
    virtual void Close() __asm__("CloseBhvRelocate");   /* slot 9 */
    virtual void Update() __asm__("UpdateBhvRelocate");   /* slot 10 */
};
typedef char assert_BHV191_size[sizeof(BHV191) == 0x50 ? 1 : -1];

/* 71..76 HACK, HACKSHIP, HACKBULLET, HACKENEMY, HACKLOCK, HACKWALL (key TU hack): sly2/hack.h */
#include "sly2/hack.h"

/* 12 WALLSPOT (was CID12; 0x1C0): wall grabspot (wall press: the *WallGrabspot* functions); vtable 0x2D2738, 50 slots */
struct WALLSPOT : SHAPE, GRABSPOT
{
    DLE dleGrab;            /* +0x1B4 link in the world's grab list g_sw.dlCid12 (sw.cpp InitDl offset; kept across Clone) */
    int unknown1BC;
    virtual void OnAdd() __asm__("OnWallGrabspotAdd");   /* slot 23 */
    virtual CLASSINFO *GetClassInfo() __asm__("GetCid12ClassDesc");   /* slot 6 */
    virtual void Init() __asm__("InitWallGrabspot");   /* slot 7 */
    virtual void PostLoad() __asm__("PostWallGrabspotLoad");   /* slot 15 */
    virtual void OnRemove() __asm__("OnWallGrabspotRemove");   /* slot 22 */
    virtual void Clone(LO *ploBase) __asm__("CloneWallGrabspot");   /* slot 41 */
    virtual int GetGrabRange(float *pasRange) __asm__("GetWallGrabspotWrapRange");   /* GRABSPOT slot 1 (delta -400) */
    virtual void GetClosestParam(STEPPLAYER *pstepplayer, float *psParam) __asm__("GetWallGrabspotClosestParam");   /* GRABSPOT slot 2 (delta -400) */
    virtual void GetGrabPos(float sParam, STEPPLAYER *pstepplayer, qword *ppos, float *pgHeading) __asm__("GetWallGrabspotGrabPosition");   /* GRABSPOT slot 3 (delta -400) */
    virtual void GetSparklePos(qword *ppos, qword *pdv, float sParam) __asm__("GetWallGrabspotSparklePosition");   /* GRABSPOT slot 5 (delta -400) */
    virtual LO *GetOwner() __asm__("GetCid12GrabspotOwner__FP8WALLSPOT");   /* GRABSPOT slot 6 (delta -400) */
};
typedef char assert_CID12_size[sizeof(WALLSPOT) == 0x1C0 ? 1 : -1];
typedef char assert_CID12_dleGrab[SLY2_OFFSETOF(WALLSPOT, dleGrab) == 0x1B4 ? 1 : -1];

/* 34 POLESPOT (was CID34; 0x110): pole grabspot (the *PoleGrabspot* functions); vtable 0x2D2558, 50 slots */
struct POLESPOT : XFM, GRABSPOT
{
    DLE dleGrab;            /* +0x104 link in g_sw.dlCid34 (kept across Clone) */
    float sRadius;          /* +0x10C pole radius */
    virtual void OnAdd() __asm__("OnPoleGrabspotAdd");   /* slot 23 */
    virtual CLASSINFO *GetClassInfo() __asm__("GetCid34ClassDesc");   /* slot 6 */
    virtual void Init() __asm__("InitPoleGrabspot");   /* slot 7 */
    virtual void PostLoad() __asm__("PostPoleGrabspotLoad");   /* slot 15 */
    virtual void OnRemove() __asm__("OnPoleGrabspotRemove");   /* slot 22 */
    virtual void Clone(LO *ploBase) __asm__("ClonePoleGrabspot");   /* slot 41 */
    virtual int GetGrabRange(float *pasRange) __asm__("GetPoleGrabspotWrapRange");   /* GRABSPOT slot 1 (delta -224) */
    virtual void GetClosestParam(STEPPLAYER *pstepplayer, float *psParam) __asm__("GetPoleGrabspotClosestParam");   /* GRABSPOT slot 2 (delta -224) */
    virtual void GetGrabPos(float sParam, STEPPLAYER *pstepplayer, qword *ppos, float *pgHeading) __asm__("GetPoleGrabspotGrabPosition");   /* GRABSPOT slot 3 (delta -224) */
    virtual void GetSparklePos(qword *ppos, qword *pdv, float sParam) __asm__("GetPoleGrabspotSparklePosition");   /* GRABSPOT slot 5 (delta -224) */
    virtual LO *GetOwner() __asm__("GetCid34GrabspotOwner__FP8POLESPOT");   /* GRABSPOT slot 6 (delta -224) */
};
typedef char assert_CID34_size[sizeof(POLESPOT) == 0x110 ? 1 : -1];
typedef char assert_CID34_sRadius[SLY2_OFFSETOF(POLESPOT, sRadius) == 0x10C ? 1 : -1];

/* 38 CID38 (0x110); vtable 0x2D2378, 50 slots */
struct CID38 : XFM, GRABSPOT
{
    DLE dleGrab;            /* +0x104 link in g_sw.dlCid38 (kept across Clone) */
    int nGrabState;         /* +0x10C the grab state the player takes (grab.cpp TryGrabJtNearestCandidate); nonzero: the
                               grab position is not pushed out by the player's radius (GetHoldGrabspotGrabPosition) */
    virtual void OnAdd() __asm__("OnHoldGrabspotAdd");   /* slot 23 */
    virtual CLASSINFO *GetClassInfo() __asm__("GetCid38ClassDesc");   /* slot 6 */
    virtual void Init() __asm__("InitHoldGrabspot");   /* slot 7 */
    virtual void PostLoad() __asm__("PostHoldGrabspotLoad");   /* slot 15 */
    virtual void OnRemove() __asm__("OnHoldGrabspotRemove");   /* slot 22 */
    virtual void Clone(LO *ploBase) __asm__("CloneHoldGrabspot");   /* slot 41 */
    virtual void GetGrabPos(float sParam, STEPPLAYER *pstepplayer, qword *ppos, float *pgHeading) __asm__("GetHoldGrabspotGrabPosition");   /* GRABSPOT slot 3 (delta -224) */
    virtual LO *GetOwner() __asm__("GetCid38GrabspotOwner__FP5CID38");   /* GRABSPOT slot 6 (delta -224) */
};
typedef char assert_CID38_size[sizeof(CID38) == 0x110 ? 1 : -1];

/* 88 BARREL (0x1A0): BARREL / carryable (REVIEW, medium); vtable 0x2D20D0, 75 slots */
struct BARREL : SO, GRABSPOT
{
    DLE dleGrab;            /* +0x174 link in g_sw.dlCid88 (kept across Clone) */
    int carryState;         /* +0x17C 0 free .. 3 (SetBarrelState) */
    float carryStateTime;   /* +0x180 */
    float grabRadius;       /* +0x184 the radius a held barrel's safe place goes round at (grab.cpp RecordJtGrabPlace, the
                               other grabbed objects of grab states 0 / 13) (inferred) */
    int unknown188;
    int isFree;             /* +0x18C physical when not carried */
    STEPPLAYER *carrier;    /* +0x190 the player carrying it */
    int unknown194[3];
    virtual void OnAdd() __asm__("OnBarrelAdd");   /* slot 23 */
    virtual CLASSINFO *GetClassInfo() __asm__("GetBarrelClassDesc");   /* slot 6 */
    virtual void Init() __asm__("InitBarrel");   /* slot 7 */
    virtual void OnRemove() __asm__("OnBarrelRemove");   /* slot 22 */
    virtual void OnProxyAcquire(LO *plo) __asm__("ResetBarrel");   /* slot 26 */
    virtual void Clone(LO *ploBase) __asm__("CloneBarrel");   /* slot 41 */
    virtual int ShouldIgnoreIntersection(SO *psoOther) __asm__("ShouldBarrelIgnoreIntersection");   /* slot 61 */
    virtual void PresetAccel() __asm__("PresetBarrelAccel");   /* slot 70 */
    virtual int TakeDamage(ZPR *pzpr) __asm__("HandleBarrelZap");   /* slot 75 */
    virtual LO *GetOwner() __asm__("GetBarrelGrabspotOwner__FP6BARREL");   /* GRABSPOT slot 6 (delta -336) */
};
typedef char assert_BARREL_size[sizeof(BARREL) == 0x1A0 ? 1 : -1];
typedef char assert_BARREL_carryState[SLY2_OFFSETOF(BARREL, carryState) == 0x17C ? 1 : -1];

/* 35 CID35 (0x180): GRABSPOT XFM; mission trigger spot (ApplyTaskStates); vtable 0x2D1EE8, 51 slots */
struct CID35 : XFM, GRABSPOT
{
    DLE dleGrab;            /* +0x104 link in g_sw.dlCid35 */
    LO *activator;          /* +0x10C who fired it last (FireTriggerGrabspot: a HERO from booty.cpp, an LO from grabspot.cpp) */
    float sRadius;          /* +0x110 biases the grab score (AdjustTriggerGrabspotScore) */
    void *relay;            /* +0x114 fired with the trigger */
    int crsmg;              /* +0x118 */
    OID arsmg[4][4];        /* +0x11C RSMG[4] (as HACKLOCK's: oidRoot, oidSM, oidTrigger, oidNoTrigger) */
    int cpair;              /* +0x15C */
    int apair[4][2];        /* +0x160 task state pairs (ApplyTaskStates) */
    virtual void OnAdd() __asm__("OnTriggerGrabspotAdd");   /* slot 23 */
    virtual CLASSINFO *GetClassInfo() __asm__("GetCid35ClassDesc");   /* slot 6 */
    virtual void Init() __asm__("InitTriggerGrabspot");   /* slot 7 */
    virtual void PostLoad() __asm__("PostTriggerGrabspotLoad");   /* slot 15 */
    virtual void OnRemove() __asm__("OnTriggerGrabspotRemove");   /* slot 22 */
    virtual void GetGrabPos(float sParam, STEPPLAYER *pstepplayer, qword *ppos, float *pgHeading) __asm__("GetTriggerGrabspotGrabPosition");   /* GRABSPOT slot 3 (delta -224) */
    virtual void AdjustScore(float *pgScore) __asm__("AdjustTriggerGrabspotScore");   /* GRABSPOT slot 4 (delta -224) */
    virtual LO *GetOwner() __asm__("GetCid35GrabspotOwner__FP5CID35");   /* GRABSPOT slot 6 (delta -224) */
    virtual void OnTriggered(LO *ploActivator) __asm__("FireTriggerGrabspot");   /* slot 51: the object that set it off */
};
typedef char assert_CID35_size[sizeof(CID35) == 0x180 ? 1 : -1];
typedef char assert_CID35_sRadius[SLY2_OFFSETOF(CID35, sRadius) == 0x110 ? 1 : -1];
typedef char assert_CID35_apair[SLY2_OFFSETOF(CID35, apair) == 0x160 ? 1 : -1];

/* 37 CID37 (0x1A0): booty container; vtable 0x2CB9F8, 51 slots */
struct CID37 : CID35
{
    /* The booty container's fields (booty.cpp's CID37V view names them; declared here so the class has its size on every
       target). */
    union { int id; unsigned short idShort; };   /* +0x180 */
    int kind;                   /* +0x184 */
    int acceptedKind;           /* +0x188 the booty kind it takes (inferred) */
    float timerSeconds;         /* +0x18C HUD timer started when booty goes in (inferred) */
    int isLocked;               /* +0x190 refuses booty while set (inferred) */
    struct BOOTY *pbooty;       /* +0x194 the booty it holds */
    struct NOISE *heldNoise;   /* +0x198 the noise it makes while holding booty (ReplaceHeldSwNoise) */
    char pad19C[0x1A0 - 0x19C];
    virtual void PostLoad() __asm__("PostBootyContainerLoad");   /* slot 15 Also known as: PostLoadBootyStand (upstream readable / Sly 1 name; PostLoadBootyStand__FP2SW). */
    virtual CLASSINFO *GetClassInfo() __asm__("GetCid37ClassDesc");   /* slot 6 */
    virtual void Init() __asm__("InitBootyContainer");   /* slot 7 */
    virtual void OnAdd() __asm__("OnBootyContainerAdd");   /* slot 23 */
    virtual void OnTriggered(LO *ploActivator) __asm__("OnBootyContainerTouched");   /* slot 51: the object that set it off */
};
typedef char assert_CID37_size[sizeof(CID37) == 0x1A0 ? 1 : -1];

/* 36 CARRYSPOT (was CID36; 0x1A0): carry grabspot, the object Murray lifts (child of CID35; *CarryGrabspot*); vtable 0x2D1D00, 51 slots */
struct CARRYSPOT : CID35
{
    LO *unknown180;         /* +0x180 a trigger target that is it or its descendant is skipped */
    OID oid1;               /* +0x184 three linked objects resolved at post-load */
    LO *plo1;               /* +0x188 */
    OID oid2;               /* +0x18C */
    LO *plo2;               /* +0x190 */
    OID oid3;               /* +0x194 */
    LO *plo3;               /* +0x198 */
    OID oidTarget;          /* +0x19C a target with another id at +0xF4 is skipped (inferred) */
    virtual void PostLoad() __asm__("PostCarryGrabspotLoad");   /* slot 15 */
    virtual CLASSINFO *GetClassInfo() __asm__("GetCid36ClassDesc");   /* slot 6 */
    virtual void Init() __asm__("InitCarryGrabspot");   /* slot 7 */
    virtual void OnTriggered(LO *ploActivator) __asm__("OnTriggerGrabspotGrabbed");   /* slot 51: the object that set it off */
};
typedef char assert_CID36_size[sizeof(CARRYSPOT) == 0x1A0 ? 1 : -1];
typedef char assert_CID36_oid1[SLY2_OFFSETOF(CARRYSPOT, oid1) == 0x184 ? 1 : -1];

/* 27 CID27 (0x160): table grabspot (VOL + GRABSPOT; hide under); vtable 0x2D1B60, 50 slots */
struct CID27 : VOL, GRABSPOT
{
    DLE dleGrab;            /* +0x154 link in g_sw.dlCid27 */
    LO *ploHold;            /* +0x15C child whose height sets the hold position */
    virtual void OnAdd() __asm__("OnTableGrabspotAdd");   /* slot 23 */
    virtual CLASSINFO *GetClassInfo() __asm__("GetCid27ClassDesc");   /* slot 6 */
    virtual void Init() __asm__("InitTableGrabspot");   /* slot 7 */
    virtual void PostLoad() __asm__("PostTableGrabspotLoad");   /* slot 15 */
    virtual void OnRemove() __asm__("OnTableGrabspotRemove");   /* slot 22 */
    virtual void GetGrabPos(float sParam, STEPPLAYER *pstepplayer, qword *ppos, float *pgHeading) __asm__("GetTableGrabspotGrabPosition");   /* GRABSPOT slot 3 (delta -304) */
    virtual void GetSparklePos(qword *ppos, qword *pdv, float sParam) __asm__("GetTableGrabspotSparklePosition");   /* GRABSPOT slot 5 (delta -304) */
    virtual LO *GetOwner() __asm__("GetCid27GrabspotOwner__FP5CID27");   /* GRABSPOT slot 6 (delta -304) */
};
typedef char assert_CID27_size[sizeof(CID27) == 0x160 ? 1 : -1];
typedef char assert_CID27_ploHold[SLY2_OFFSETOF(CID27, ploHold) == 0x15C ? 1 : -1];

/* 19 HND (0x140): hook; REVIEW proposes HND; vtable 0x2D2978, 50 slots */
struct HND : TARGET, GRABSPOT
{
    int nHangKind;          /* +0x124 hang kind (jt.cpp SetJtState, the hang entry) */
    int unknown128;
    int sfxidHangStart;     /* +0x12C sound at the hang start (jt.cpp SetJtState; HND::Init sets 8) */
    int sfxidHangEnd;       /* +0x130 (HND::Init sets 9) */
    float unknown134;
    LO *ploTarget;          /* +0x138 object the hang aims at (jt.cpp ChooseJtGroundState) */
    int unknown13C;
    virtual void Init() __asm__("InitHnd");   /* slot 7 */
    virtual CLASSINFO *GetClassInfo() __asm__("GetHndClassDesc");   /* slot 6 */
    virtual void GetGrabPos(float sParam, STEPPLAYER *pstepplayer, qword *ppos, float *pgHeading) __asm__("GetHndGrabPosition");   /* GRABSPOT slot 3 (delta -256) */
    virtual LO *GetOwner() __asm__("GetHndGrabspotOwner__FP3HND");   /* GRABSPOT slot 6 (delta -256) */
};
typedef char assert_HND_size[sizeof(HND) == 0x140 ? 1 : -1];
typedef char assert_HND_ploTarget[SLY2_OFFSETOF(HND, ploTarget) == 0x138 ? 1 : -1];

/* 8 CID8 (0x100): mesh surface; vtable 0x2D2CB8, 50 slots */
struct CID8 : XFM
{
    char padE0[0x100 - 0xE0];
    virtual void Init() __asm__("InitCid8");   /* slot 7 */
    virtual CLASSINFO *GetClassInfo() __asm__("GetCid8ClassDesc");   /* slot 6 */
    virtual void LoadFromBrx(CBinaryInputStream *pbis) __asm__("LoadCid8FromBrx");   /* slot 14 */
};
typedef char assert_CID8_size[sizeof(CID8) == 0x100 ? 1 : -1];

/* 9 CID9 (0x120): mesh surface (jump target list); vtable 0x2D2B18, 50 slots */
struct CID9 : CID8
{
    DLE dleList;                /* +0x100 link in SW::dlCid9 */
    float maxDropHeight;        /* +0x108 how far below the jumping player the surface may be to be a jump target
                                   (air.cpp ChooseJtLanding; GRABSPOT::sBusy plays the same role for the grab spots) */
    char pad10C[0x120 - 0x10C];
    virtual void Init() __asm__("InitCid9");   /* slot 7 */
    virtual CLASSINFO *GetClassInfo() __asm__("GetCid9ClassDesc");   /* slot 6 */
    virtual void AddHierarchy() __asm__("AddCid9Hierarchy");   /* slot 11 */
    virtual void RemoveHierarchy() __asm__("RemoveCid9Hierarchy");   /* slot 12 */
    virtual void PostLoad() __asm__("PostCid9Load");   /* slot 15 */
    virtual void Clone(LO *ploBase) __asm__("CloneCid9");   /* slot 41 */
};
typedef char assert_CID9_size[sizeof(CID9) == 0x120 ? 1 : -1];

/* 40 CID40 (0xF0): JMT (jump target; none in the Paris hub); vtable 0x2D3200, 50 slots */
struct CID40 : XFM
{
    DLE dleList;                /* +0xE0 link in SW::dlCid40 */
    char padE8[0xF0 - 0xE8];
    virtual void AddHierarchy() __asm__("AddJmtHierarchy");   /* slot 11 */
    virtual CLASSINFO *GetClassInfo() __asm__("GetCid40ClassDesc");   /* slot 6 */
    virtual void Init() __asm__("InitJmt");   /* slot 7 */
    virtual void RemoveHierarchy() __asm__("RemoveJmtHierarchy");   /* slot 12 */
    virtual void Clone(LO *ploBase) __asm__("CloneJmt");   /* slot 41 */
};
typedef char assert_CID40_size[sizeof(CID40) == 0xF0 ? 1 : -1];

/* 4 LIGHT (0x170): relight inputs: 3 qw from +0x120; vtable 0x2D33A0, 50 slots */
struct LIGHT : XFM
{
    int kind;               /* +0xE0 [S] 0 directional, 1 point (light.cpp LIGHTV) */
    int parentStamp;        /* +0xE4 [S] the parent transform stamp the light was last rebuilt for */
    int unknownE8[2];
    union {                 /* +0xF0 [S] the highlight colour (HSV; RebuildLight reads the value channel) */
        qword highlightHsv;
        float highlightHsvComponents[4];
    };
    qword position;         /* +0x100 [S] bounding sphere centre */
    float bias;             /* +0x110 [S] rank = bias + weight * distance */
    float weight;           /* +0x114 [S] */
    int slot;               /* +0x118 [S] world slot, < 0 until assigned */
    int unknown11C;
    union {                 /* +0x120 [S] the 48-byte shader payload (RebuildLight fills it; the packets copy it) */
        uint payload[12];
        struct {
            int kindPayload;    /* +0x120 */
            float duAngle;      /* +0x124 */
            float ruAngle;      /* +0x128 */
            float duFall;       /* +0x12C */
            float agRgba[4];    /* +0x130 */
            VECTOR vec;         /* +0x140 */
            float ruFall;       /* +0x14C */
        };
    };
    float midtoneStrength;  /* +0x150 [S] */
    float midtoneAngle;     /* +0x154 [S] degrees */
    int enabled;            /* +0x158 [S] */
    int isStatic;           /* +0x15C [S] exclusive with isDynamic (SetLightStatic / SetLightDynamic) */
    int isDynamic;          /* +0x160 [S] */
    union {                 /* +0x164 [S] the point-light falloff range (SetLightFalloff copies it whole: Sly 1's lmFallOff) */
        struct {
            float sNear;    /* +0x164 */
            float sFar;     /* +0x168 */
        };
        LM lmFallOff;
    };
    int unknown16C;
    virtual void Init() __asm__("InitLight");   /* slot 7 */
    virtual CLASSINFO *GetClassInfo() __asm__("GetLightClassDesc");   /* slot 6 */
    virtual void AddHierarchy() __asm__("AddLightHierarchy");   /* slot 11 */
    virtual void RemoveHierarchy() __asm__("RemoveLightHierarchy");   /* slot 12 */
    virtual void PostLoad() __asm__("PostLightLoad");   /* slot 15 */
    virtual void UpdateXfWorld() __asm__("ProjectLightTransform");   /* slot 17 */
    virtual void OnRemove() __asm__("OnLightRemove");   /* slot 22 */
    virtual void OnAdd() __asm__("OnLightAdd");   /* slot 23 */
    virtual void TranslateToPos(qword qwPos) __asm__("TranslateLightToPos");   /* slot 29 */
    virtual void RotateToMat(MATRIX3 *pmatWorld) __asm__("RotateLightToMat");   /* slot 30 */
};
typedef char assert_LIGHT_size[sizeof(LIGHT) == 0x170 ? 1 : -1];
typedef char assert_LIGHT_position[SLY2_OFFSETOF(LIGHT, position) == 0x100 ? 1 : -1];
typedef char assert_LIGHT_payload[SLY2_OFFSETOF(LIGHT, payload) == 0x120 ? 1 : -1];
typedef char assert_LIGHT_sNear[SLY2_OFFSETOF(LIGHT, sNear) == 0x164 ? 1 : -1];

/* 96 CID96 (0x250): mini pigeon/rat critter; vtable 0x2D3838, 75 slots */
struct CID96 : SO
{
    char pad150[0x250 - 0x150];
    virtual void RenderSelf() __asm__("RenderCritterSelf");   /* slot 19 */
    virtual CLASSINFO *GetClassInfo() __asm__("GetCid96ClassDesc");   /* slot 6 */
    virtual void HandleMessage(MSGID msgid, void *pv) __asm__("HandleCritterMessage");   /* slot 5 */
    virtual void Init() __asm__("InitCritter");   /* slot 7 */
    virtual void RemoveHierarchy() __asm__("RemoveCritterHierarchy");   /* slot 12 */
    virtual void LoadFromBrx(CBinaryInputStream *pbis) __asm__("LoadCritterFromBrx");   /* slot 14 */
    virtual void PostLoad() __asm__("PostCritterLoad");   /* slot 15 */
    virtual void Update() __asm__("UpdateCritter");   /* slot 16 */
    virtual void UpdateXfWorld() __asm__("UpdateCritterXfWorld");   /* slot 17 */
    virtual void OnProxyAcquire(LO *plo) __asm__("ResetCritterFromProxy");   /* slot 26 */
    virtual void PresetAccel() __asm__("PresetCritterAccel");   /* slot 70 */
    virtual int TakeDamage(ZPR *pzpr) __asm__("TakeCritterDamage");   /* slot 75 */
};
typedef char assert_CID96_size[sizeof(CID96) == 0x250 ? 1 : -1];

/* 80 MISSILE (0x270); vtable 0x2D3FA0, 78 slots */
struct MISSILE : BOMB
{
    /* missile.cpp's MISSILE names these (declared here so the class has its size on every target). */
    LO *targetObject;           /* +0x210 */
    int unknown214[3];
    qword targetPos;            /* +0x220 */
    ASEG *flightAnim;           /* +0x230 */
    ASEGA *flightAnimState;     /* +0x234 */
    int numIgnoredObjects;      /* +0x238 */
    OID ignoredObjects[4];
    int numIgnoredClasses;
    CID ignoredClasses[4];
    float launchSpeed;          /* +0x260 */
    int hasFlightAction;        /* +0x264 */
    ACT *flightAction;          /* +0x268 */
    float flightActionParam;    /* +0x26C */
    virtual CLASSINFO *GetClassInfo() __asm__("GetMissileClassDesc");   /* slot 6 */
    virtual void Init() __asm__("InitMissile__FP7MISSILE");   /* slot 7 */
    virtual void RemoveHierarchy() __asm__("OnMissileRemove__FP7MISSILE");   /* slot 12 */
    virtual void LoadFromBrx(CBinaryInputStream *pbis) __asm__("LoadMissileFromBrx__FP7MISSILEP18CBinaryInputStream");   /* slot 14 */
    virtual int ShouldIgnoreIntersection(SO *psoOther) __asm__("ShouldMissileIgnoreIntersection__FP7MISSILEP2SO");   /* slot 61 */
    virtual void ApplyThrow() __asm__("ApplyMissileThrow");   /* slot 77 */
    virtual void Fire(LO *ploTarget, VECTOR4 *pposTarget) __asm__("FireMissile");   /* slot 78: launch at a target object and a position in its space (0: in the world) */
};
typedef char assert_MISSILE_size[sizeof(MISSILE) == 0x270 ? 1 : -1];

/* 81 ACCMISS (0x280): ACCMISS = Bentley crossbow dart; vtable 0x2D3D20, 78 slots */
struct ACCMISS : MISSILE
{
    float cruiseSpeed;          /* +0x270 */
    float speedChangeRate;      /* +0x274 */
    int unknown278[2];
    virtual CLASSINFO *GetClassInfo() __asm__("GetAccmissClassDesc");   /* slot 6 */
    virtual void Init() __asm__("InitAccmiss__FP7ACCMISS");   /* slot 7 */
    virtual void PresetAccel() __asm__("PresetAccmissAccel");   /* slot 70 */
};
typedef char assert_ACCMISS_size[sizeof(ACCMISS) == 0x280 ? 1 : -1];

/* 82 CID82 (0x290); vtable 0x2D3AA0, 78 slots */
struct CID82 : MISSILE
{
    float flightTime;           /* +0x270 */
    float maxSpeed;             /* +0x274 */
    float arrivalTime;          /* +0x278 */
    int unknown27C;
    qword targetVelocity;       /* +0x280 */
    virtual CLASSINFO *GetClassInfo() __asm__("GetCid82ClassDesc");   /* slot 6 */
    virtual void Init() __asm__("InitSplmiss");   /* slot 7 */
    virtual void PresetAccel() __asm__("PresetSplmissAccel");   /* slot 70 */
    virtual void Fire(LO *ploTarget, VECTOR4 *pposTarget) __asm__("FireSplmiss");   /* slot 78 */
};
typedef char assert_CID82_size[sizeof(CID82) == 0x290 ? 1 : -1];

/* 49 MS (0x150); vtable 0x2D4260, 75 slots */
struct MS : SO
{
    virtual void Init() __asm__("InitMs");   /* slot 7 */
    virtual CLASSINFO *GetClassInfo() __asm__("GetMsClassDesc");   /* slot 6 */
    virtual void RenderSelf() __asm__("RenderMsGlobset");   /* slot 19 */
};
typedef char assert_MS_size[sizeof(MS) == 0x150 ? 1 : -1];

/* 174 BHV174 (0x40): NEELA watch/alarm; vtable 0x2D4AF8, 14 slots */
struct BHV174 : BHV
{
    int state;              /* +0x30 0 run the chase script (BHV169), 1 chase the player, 2 give up */
    float stateTime;        /* +0x34 */
    CID113 *chaseScript;    /* +0x38 [S] copied from NEELA::chaseScript (SetBhvguardMode) */
    int unknown3C;
    virtual void Start() __asm__("StartBhvNeelaChase");   /* slot 8 */
    virtual CLASSINFO *GetClassInfo() __asm__("GetBhv174ClassDesc");   /* slot 6 */
    virtual void OnPush() __asm__("InitBhvNeelaChase");   /* slot 7 */
    virtual void Close() __asm__("CloseBhvNeelaChase");   /* slot 9 */
    virtual void Update() __asm__("UpdateBhvNeelaChase");   /* slot 10 */
};
typedef char assert_BHV174_size[sizeof(BHV174) == 0x40 ? 1 : -1];

/* 60 NEELA (0x10A0); vtable 0x2D4820, 89 slots */
/* NEELA's chase tuning (+0x1070): the speed factor as a clamped quadratic of the player distance, and the give-up
   time of her chase behaviour. */
struct NEELACHASETUNE
{
    CLQ speedCurve;         /* +0x00 (InitNeela 1.35, -0.0003, 0) */
    LM speedRange;          /* +0x10 (0.9..1.2) */
    float giveUpTime;       /* +0x18 (7 s) */
};
struct NEELA : STEPGUARD
{
    VOL *moveVolume;        /* +0x1010 where she looks for a target while moving (flag 9) */
    VOL *idleVolume;        /* +0x1014 ... while idle (flag 2) */
    int state;              /* +0x1018 0 looking, 1 aiming her prop at the target */
    float stateTime;        /* +0x101C */
    ASEG *propAnimation;    /* +0x1020 [S] bound oid 0x5D8 (PostNeelaLoad) */
    ASEGA *propAsega;       /* +0x1024 its playing instance (state 1) */
    LO *target;             /* +0x1028 */
    struct ACT *aimAct;     /* +0x102C the act turning the prop part (its angle goal at +0x20) */
    void *unknown1030;      /* +0x1030 [S] bound oid 0x5D9 */
    void *propPart;         /* +0x1034 [S] bound oid 0x5DA: the part turned (position +0x30, its space +0xA0) */
    int unknown1038[2];
    qword propAxis;         /* +0x1040 the prop's effect offset at label 0x5DB (PostNeelaLoad) */
    LO *prop;               /* +0x1050 her attached prop (removed by ResetNeelaToHome) */
    int unknown1054[3];
    ANIMBINDING giveUpBinding; /* +0x1060 the animation of chase state 2 (PostNeelaLoad: kind 0, flags 1) */
    NEELACHASETUNE chaseTune; /* +0x1070 */
    CID113 *chaseScript;    /* +0x108C her option (SendRootBhvMode15): chase (BHV174) with this script instead of combat (ShouldBhvguardUseNeelaBehavior) */
    SSGRESUME resumeState;  /* +0x1090 where the chase script resumes */
    int unknown109C;
    virtual void Init() __asm__("InitNeela");   /* slot 7 */
    virtual CLASSINFO *GetClassInfo() __asm__("GetNeelaClassDesc");   /* slot 6 */
    virtual void PostLoad() __asm__("PostNeelaLoad");   /* slot 15 */
    virtual void Update() __asm__("UpdateNeela");   /* slot 16 */
    virtual void OnProxyAcquire(LO *plo) __asm__("ResetNeelaToHome");   /* slot 26 */
};
typedef char assert_NEELA_size[sizeof(NEELA) == 0x10A0 ? 1 : -1];
typedef char assert_NEELA_chaseScript[SLY2_OFFSETOF(NEELA, chaseScript) == 0x108C ? 1 : -1];

/* 98 CID98 (0x180): breakable props (chairs, barrels, crates, containers); vtable 0x2D4B78, 75 slots */
struct BRKACTION; struct BRKSTAGE;   /* breakable.cpp: a trigger class or action record (kind, value), a stage */
struct CID98 : SO
{
    /* Own fields from breakable.cpp (its TRIG view until v1.16). */
    int triggerClassCount;      /* +0x150 */
    BRKACTION *triggerClasses;  /* +0x154 the classes whose contact fires it (cid, armed) */
    int firingCount;            /* +0x158 firings of the current stage */
    int stage;                  /* +0x15C the current stage */
    int stageCount;             /* +0x160 */
    BRKSTAGE *stages;           /* +0x164 the stages (stages[-1]: the always-run stage) */
    int reactionMask;           /* +0x168 what a firing may do (camera 2, rumble 4, noise 8; 1 none); option 935 */
    LO *instigator;             /* +0x16C the object that fired it */
    int zapKind;                /* +0x170 the zap kind that fired it (0 for contacts, inferred) */
    DLE dleBreakable;           /* +0x174 link in g_sw.dlCid98 (OnBreakableAdd; carmelita.cpp / bhvsearch.cpp walk it) */
    int unknown17C;
    virtual void AdjustXps() __asm__("HandleBreakableContacts");   /* slot 58 */
    virtual CLASSINFO *GetClassInfo() __asm__("GetCid98ClassDesc");   /* slot 6 */
    virtual void Init() __asm__("InitBreakable");   /* slot 7 */
    virtual void LoadFromBrx(CBinaryInputStream *pbis) __asm__("LoadBreakableFromBrx");   /* slot 14 */
    virtual void OnRemove() __asm__("OnBreakableRemove");   /* slot 22 */
    virtual void OnAdd() __asm__("OnBreakableAdd");   /* slot 23 */
    virtual void OnProxyAcquire(LO *plo) __asm__("ResetBreakable");   /* slot 26 */
    virtual int TakeDamage(ZPR *pzpr) __asm__("AcceptBreakableZap");   /* slot 75 */
};
typedef char assert_CID98_dleBreakable[SLY2_OFFSETOF(CID98, dleBreakable) == 0x174 ? 1 : -1];
typedef char assert_CID98_size[sizeof(CID98) == 0x180 ? 1 : -1];

/* 14 OBSTACLE (was CID14; 0x1F0): convex polygon SHAPE / pathzone obstacle (cnvo.cpp: the TU sits where Sly 1 has
   cnvo.c, but Sly 1's CNVO is a conveyor; this class clips, merges and registers convex obstacles); vtable 0x2D4DE0, 50 slots */
struct OBSTACLE : SHAPE
{
    /* Own fields from cnvo.cpp (its CID14V view until v1.16). The outline is the shape's points (the last repeats
       the first); edgePlanes holds the numControlVertices - 1 side planes, valid while planeStamp equals the
       parent transform's worldStamp (BuildCid14EdgePlanes). */
    int winding;                /* +0x190 ClassifyCid14Polygon's result (cnvo.cpp WINDING_*) */
    int unknown194;
    void *unknown198;
    void *unknown19C;
    QWROW boundsMin;            /* +0x1A0 XY bounds of the outline */
    QWROW boundsMax;            /* +0x1B0 */
    int isMerged;               /* +0x1C0 a pooled merged polygon (NewMergedCid14Obstacle) */
    DL members;                 /* +0x1C4 the polygons merged into this one (linked through dleMerged) */
    DLE dleMerged;              /* +0x1D0 link in the owner's members list; also g_sw.dlCid14Free's link */
    OBSTACLE *owner;               /* +0x1D8 the merged polygon this one belongs to */
    struct PATHZONE *pathzone;  /* +0x1DC the zone it is registered in (AddPathzoneObstacle) */
    DLE dlePending;             /* +0x1E0 link in g_sw.dlCid14Pending (inferred) */
    int planeStamp;             /* +0x1E8 */
    qword *edgePlanes;          /* +0x1EC the side planes */
    virtual void Init() __asm__("InitCid14");   /* slot 7 */
    virtual CLASSINFO *GetClassInfo() __asm__("GetCid14ClassDesc");   /* slot 6 */
    virtual void AddHierarchy() __asm__("AddCid14Hierarchy");   /* slot 11 */
    virtual void RemoveHierarchy() __asm__("RemoveCid14Hierarchy");   /* slot 12 */
    virtual void LoadFromBrx(CBinaryInputStream *pbis) __asm__("LoadCid14FromBrx");   /* slot 14 */
    virtual void PostLoad() __asm__("PostCid14Load");   /* slot 15 */
    virtual void Clone(LO *ploBase) __asm__("CloneCid14");   /* slot 41 */
};
typedef char assert_CID14_dleMerged[SLY2_OFFSETOF(OBSTACLE, dleMerged) == 0x1D0 ? 1 : -1];
typedef char assert_CID14_dlePending[SLY2_OFFSETOF(OBSTACLE, dlePending) == 0x1E0 ? 1 : -1];
typedef char assert_CID14_size[sizeof(OBSTACLE) == 0x1F0 ? 1 : -1];

/* 45 CID45 (0x130): render-to-texture owner (item preview?); vtable 0x2D4F80, 50 slots */
struct CID45 : XFM
{
    /* The render-target camera's fields (rtcam.cpp's RTCAM view names them; declared here so the class has its size on
       every target). */
    int numHidden;              /* +0xE0 objects hidden while rendering, with their flags (1 shown again, 2 re-enabled) */
    LO *aploHidden[4];
    int agrfHidden[4];
    int numShown;               /* +0x104 objects shown only in the target */
    SO *apsoShown[4];
    CID45 *parent;              /* +0x118 */
    float fov, clipNear, clipFar;
    struct RENDERTGT *prt;      /* +0x128 the claimed target */
    int unknown12C;
    virtual void Init() __asm__("InitRtcam");   /* slot 7 */
    virtual CLASSINFO *GetClassInfo() __asm__("GetCid45ClassDesc");   /* slot 6 */
    virtual void LoadFromBrx(CBinaryInputStream *pbis) __asm__("LoadRtcamFromBrx");   /* slot 14 */
    virtual void RenderSelf() __asm__("RenderRtcam");   /* slot 19 */
};
typedef char assert_CID45_size[sizeof(CID45) == 0x130 ? 1 : -1];
typedef char assert_CID45_prt[SLY2_OFFSETOF(CID45, prt) == 0x128 ? 1 : -1];

/* A navigation graph (CG): vertices (CGV, 0x20 bytes each), triangles (CGT, three vertex pointers each), the 2D BSP
   nodes, and the zone's dynamic obstacles with their sorted x and z begin/end events (sweep and prune; the event
   arrays start with two sentinels, so numEvents is 2 + 2 per obstacle). path.cpp's 2D BSP walkers see it as GRAPH.
   Moved from src/game/engine/physics/path.cpp with PATHZONE. */
struct CGV;
/* CGE: an edge of a navigation vertex: the neighbour and the cost of walking to it (FindPathAStar adds it to the
   path cost; FindPathPoints stores the XY distance). */
struct CGE
{
    CGV *pcgv;              /* the neighbouring vertex */
    float cost;             /* +0x04 */
};
/* CGV: a navigation vertex (0x20 bytes on the EE): its position and its edges. */
struct CGV
{
    qword pos;              /* position */
    int numEdges;           /* +0x10 */
    CGE *edges;             /* +0x14 */
};
/* CGT: a walkable triangle: its three corners (12 bytes on the EE, three pointers natively). */
struct CGT
{
    CGV *vertices[3];
};
/* CBSP: a node of the graph's 2D BSP: the splitting line as a plane (x, y, d; the walkers dot it with (px, py, -1))
   and the two children (back, front). A child index with bit 15 set is a leaf: bits 0..13 index the triangles, and
   bit 14 (walk) / bit 15 (always) say whether the leaf is walkable for the query's flags. */
union CBSP
{
    qword plane;
    struct
    {
        float x, y, z;
        unsigned short child[2];
    } e;
};
struct CG
{
    int ccgv;               /* number of vertices */
    CGV *acgv;              /* +0x04 vertices */
    int numTriangles;       /* +0x08 */
    CGT *triangles;         /* +0x0C */
    int numBspNodes;        /* +0x10 */
    CBSP *bspNodes;         /* +0x14 */
    int numObstacles;       /* +0x18 */
    int obstacleCapacity;   /* +0x1C */
    int numEvents;          /* +0x20 */
    void *xEvents;          /* +0x24 */
    void *zEvents;          /* +0x28 */
    void **obstacles;       /* +0x2C */
};
typedef char assert_CG_size[sizeof(CG) == 0x30 ? 1 : -1];

/* 17 PATHZONE (0x130): *_pz; vtable 0x2D52B0, 50 slots */
struct PATHZONE : XFM
{
    CG cg;                      /* +0xE0 the zone's navigation graph */
    int unknown110;             /* +0x110 (Init: -1) */
    int version;                /* +0x114 navigation version it was last changed in (g_sw.pathzoneVersion) */
    DL dlRegent;                /* +0x118 the REGENT records of the objects in the zone (SetSoPhysRegion; link at +0x60) */
    int zoneIndex;              /* +0x124 its index in the world's zone BSP (BindSwPathzones) */
    int unk128[2];
    virtual void Init() __asm__("InitPathzone");   /* slot 7 */
    virtual CLASSINFO *GetClassInfo() __asm__("GetPathzoneClassDesc");   /* slot 6 */
    virtual void LoadFromBrx(CBinaryInputStream *pbis) __asm__("LoadPathzoneFromBrx");   /* slot 14 */
};
typedef char assert_PATHZONE_size[sizeof(PATHZONE) == 0x130 ? 1 : -1];
typedef char assert_PATHZONE_zoneIndex[SLY2_OFFSETOF(PATHZONE, zoneIndex) == 0x124 ? 1 : -1];

/* 124 PATHLINK (0x160): BHVPATHLINK walks it; REVIEW; vtable 0x2D5120, 48 slots */
struct PATHLINK : LO
{
    int numEnds;            /* +0x50 the number of link segments (BHVPATHLINK walks 0..numEnds-1) */
    int unknown54[3];
    PATHLINKEND aend[6];    /* +0x60 aend[0]/aend[1]: the link's start and end (PlanBhvgoto) */
    int kind;               /* +0x120 0: a walkable segment (PlanBhvgoto tests the guard's distance to it) */
    struct CID17 *startZone; /* +0x124 the path zone at its near end (PostPathlinkLoad; a PATHZONE in every snapshot) */
    struct CID17 *endZone;  /* +0x128 the path zone at its far end (FindBhvgotoRoute) */
    char pad12C[0x130 - 0x12C]; qword startPos; qword endPos; /* +0x130/+0x140 aend[0]'s / aend[numEnds-1]'s centre (PostPathlinkLoad) */
    int linkIndex;          /* +0x150 its index in the world's zone BSP link table (REGTREE::links; BindSwPathzones; Init: -1) */
    char pad154[0x160 - 0x154];
    virtual void Init() __asm__("InitPathlink");   /* slot 7 */
    virtual CLASSINFO *GetClassInfo() __asm__("GetPathlinkClassDesc");   /* slot 6 */
    virtual void AddHierarchy() __asm__("AddPathlinkHierarchy");   /* slot 11 */
    virtual void RemoveHierarchy() __asm__("RemovePathlinkHierarchy");   /* slot 12 */
    virtual void PostLoad() __asm__("PostPathlinkLoad");   /* slot 15 */
};
typedef char assert_PATHLINK_size[sizeof(PATHLINK) == 0x160 ? 1 : -1];
typedef char assert_PATHLINK_kind[SLY2_OFFSETOF(PATHLINK, kind) == 0x120 ? 1 : -1]; typedef char assert_PATHLINK_endPos[SLY2_OFFSETOF(PATHLINK, endPos) == 0x140 ? 1 : -1];

/* 99 CID99 (0x170): pickpocket dispenser / guard pouch; vtable 0x2D5450, 75 slots */
struct CID99 : SO
{
    /* lootpouch.cpp's SPRAYER view names +0x150..+0x168 (declared here so the class has its size on every target). */
    struct XF *launchFrame;     /* +0x150 the frame the item is drawn at and launched from (FindSoXf) */
    SO *itemProxy;              /* +0x154 the proxy copies come from (AcquireProxyObject) */
    struct DPRIZE *pdprize;     /* +0x158 the held item a pickpocket takes (pickpocket.cpp: 0 = nothing to pick; a DPRIZE in
                                   every snapshot; lootpouch.cpp's heldItem) (inferred) */
    LO *unknown15C[3];          /* +0x15C objects bound by oid (lootpouch.cpp's snip table D_002C0CB0) */
    SM *psm;                    /* +0x168 its state machine (snip table; ReplaceSma's first argument) */
    SMA *psma;                  /* +0x16C its state machine instance (pickpocket.cpp sets its goal state; an SMA in every snapshot) */
    virtual void PostLoad() __asm__("PostLootPouchLoad");   /* slot 15 */
    virtual CLASSINFO *GetClassInfo() __asm__("GetCid99ClassDesc");   /* slot 6 */
    virtual void RenderSelf() __asm__("RenderLootPouchSelf");   /* slot 19 */
};
typedef char assert_CID99_size[sizeof(CID99) == 0x170 ? 1 : -1]; typedef char assert_CID99_psma[SLY2_OFFSETOF(CID99, psma) == 0x16C ? 1 : -1];

/* 11 PIPE (0x1E0): lamp posts; JTPIPE; vtable 0x2D56F8, 50 slots */
struct PIPE : SHAPE, GRABSPOT
{
    DLE dleGrab;            /* +0x1B4 link in g_sw.dlCid11 (grab.cpp GatherJtGrabCandidates, grab mask 2) */
    int unknown1BC;         /* +0x1BC */
    qword axis;             /* +0x1C0 the reference axis the goal matrix is built around (pipe.cpp: D_002A2DA0 at InitPipe,
                               GetPipeGoalMatrix) */
    int attachEffectId;     /* +0x1D0 sound played when the player attaches (pipe_jt.cpp; option 0x1D0) */
    int releaseEffectId;    /* +0x1D4 sound played when the player lets go (pipe_jt.cpp; option 0x1D4) */
    char pad1D8[0x1E0 - 0x1D8];
    virtual void OnAdd() __asm__("OnPipeAdd");   /* slot 23 */
    virtual CLASSINFO *GetClassInfo() __asm__("GetPipeClassDesc");   /* slot 6 */
    virtual void Init() __asm__("InitPipe");   /* slot 7 */
    virtual void PostLoad() __asm__("PostPipeLoad");   /* slot 15 */
    virtual void OnRemove() __asm__("OnPipeRemove");   /* slot 22 */
    virtual void Clone(LO *ploBase) __asm__("ClonePipe");   /* slot 41 */
    virtual int GetGrabRange(float *pasRange) __asm__("GetPipeWrapRange");   /* GRABSPOT slot 1 (delta -400) */
    virtual void GetClosestParam(STEPPLAYER *pstepplayer, float *psParam) __asm__("GetPipeClosestParam");   /* GRABSPOT slot 2 (delta -400) */
    virtual void GetGrabPos(float sParam, STEPPLAYER *pstepplayer, qword *ppos, float *pgHeading) __asm__("GetPipeGrabPosition");   /* GRABSPOT slot 3 (delta -400) */
    virtual void GetSparklePos(qword *ppos, qword *pdv, float sParam) __asm__("GetPipeSparklePosition");   /* GRABSPOT slot 5 (delta -400) */
    virtual LO *GetOwner() __asm__("GetPipeGrabspotOwner__FP4PIPE");   /* GRABSPOT slot 6 (delta -400) */
};
typedef char assert_PIPE_size[sizeof(PIPE) == 0x1E0 ? 1 : -1];
typedef char assert_PIPE_dleGrab[SLY2_OFFSETOF(PIPE, dleGrab) == 0x1B4 ? 1 : -1]; typedef char assert_PIPE_axis[SLY2_OFFSETOF(PIPE, axis) == 0x1C0 ? 1 : -1];


/* 13 RAIL (0x1D0): wires; vtable 0x2D6FE8, 50 slots */
struct RAIL : SHAPE, GRABSPOT
{
    DLE dleGrab;            /* +0x1B4 link in g_sw.dlCid13 (grab.cpp GatherJtGrabCandidates, grab mask 0x200) */
    int fSlide;             /* +0x1BC the held player slides along it (grab.cpp PresetJtAccelHide); zero on an open rail:
                               the ends hold the player (UpdateJtInternalXpsHide) */
    float svSlide;          /* +0x1C0 slide speed limit (1000 at init) */
    float rSlide;           /* +0x1C4 slide gain on the gravity component along the rail (1 at init) */
    char pad1C8[0x1D0 - 0x1C8];
    virtual void OnAdd() __asm__("OnRailAdd");   /* slot 23 */
    virtual CLASSINFO *GetClassInfo() __asm__("GetRailClassDesc");   /* slot 6 */
    virtual void Init() __asm__("InitRail");   /* slot 7 */
    virtual void PostLoad() __asm__("PostRailLoad");   /* slot 15 */
    virtual void OnRemove() __asm__("OnRailRemove");   /* slot 22 */
    virtual void Clone(LO *ploBase) __asm__("CloneRail");   /* slot 41 */
    virtual int GetGrabRange(float *pasRange) __asm__("GetRailWrapRange");   /* GRABSPOT slot 1 (delta -400) */
    virtual void GetClosestParam(STEPPLAYER *pstepplayer, float *psParam) __asm__("GetRailClosestParam");   /* GRABSPOT slot 2 (delta -400) */
    virtual void GetGrabPos(float sParam, STEPPLAYER *pstepplayer, qword *ppos, float *pgHeading) __asm__("GetRailGrabPosition");   /* GRABSPOT slot 3 (delta -400) */
    virtual void GetSparklePos(qword *ppos, qword *pdv, float sParam) __asm__("GetRailSparklePosition");   /* GRABSPOT slot 5 (delta -400) */
    virtual LO *GetOwner() __asm__("GetRailGrabspotOwner__FP4RAIL");   /* GRABSPOT slot 6 (delta -400) */
};
typedef char assert_RAIL_size[sizeof(RAIL) == 0x1D0 ? 1 : -1];
typedef char assert_RAIL_dleGrab[SLY2_OFFSETOF(RAIL, dleGrab) == 0x1B4 ? 1 : -1];
typedef char assert_RAIL_rSlide[SLY2_OFFSETOF(RAIL, rSlide) == 0x1C4 ? 1 : -1];

/* 61 RAJAN (0x1020); vtable 0x2D7188, 90 slots */
struct RAJAN : STEPGUARD
{
    int hasCameraPolicy;        /* +0x1010 camera policy 6 is set (rajan.cpp SetRajanCameraFocus) */
    int unk1014[3];
    virtual void Init() __asm__("InitRajan");   /* slot 7 */
    virtual CLASSINFO *GetClassInfo() __asm__("GetRajanClassDesc");   /* slot 6 */
    virtual int CanSenseStimulus(struct NOISE *pstim) __asm__("CanRajanSenseStimulus");   /* slot 89 */
    virtual void SetCameraFocus() __asm__("SetRajanCameraFocus");   /* slot 90 */
};
typedef char assert_RAJAN_size[sizeof(RAJAN) == 0x1020 ? 1 : -1];

/* 87 RIPG (0x1B0); vtable 0x2D77D8, 75 slots */
struct RIPG : SO
{
    int groupKind;          /* +0x150 [S] NewRipGroup's kind: 0 the world's shared group, 1 one with its own tree
                               (InitRipg; CanRipgStayAsleep: never sleeps) */
    float boundsPadding;    /* +0x154 [S] ProjectRipgTransform grows the rips' bounds by it on every side */
    float emitScale;        /* +0x158 [S] */
    DL ripList;             /* +0x15C [S] the group's RIPs */
    int unknown168[2];
    qword boundsData[3];    /* +0x170 [S] the group's bounds: min and max corner, centre (ProjectRipgTransform; UpdateRipgBounds
                               copies them with the radius into its tree record) */
    float boundsRadius;     /* +0x1A0 [S] */
    int unknown1A4[3];
    virtual int IsFixed() __asm__("CanRipgStayAsleep");   /* slot 62 */
    virtual CLASSINFO *GetClassInfo() __asm__("GetRipgClassDesc");   /* slot 6 */
    virtual void Init() __asm__("InitRipg__FP4RIPG");   /* slot 7 */
    virtual void RemoveHierarchy() __asm__("OnRipgRemove__FP4RIPG");   /* slot 12 */
    virtual void Update() __asm__("UpdateRipg");   /* slot 16 */
    virtual void UpdateXfWorld() __asm__("ProjectRipgTransform");   /* slot 17 */
    virtual void RenderSelf() __asm__("RenderRipgSelf__FP4RIPGP2CMP2RO");   /* slot 19 */
    virtual void UpdateBounds() __asm__("UpdateRipgBounds");   /* slot 49 */
};
typedef char assert_RIPG_ripList[SLY2_OFFSETOF(RIPG, ripList) == 0x15C ? 1 : -1];
typedef char assert_RIPG_boundsRadius[SLY2_OFFSETOF(RIPG, boundsRadius) == 0x1A0 ? 1 : -1];
typedef char assert_RIPG_size[sizeof(RIPG) == 0x1B0 ? 1 : -1];

/* 77 ROBOTTANK and 108 CID108 (key TU robottank): sly2/tank.h */
#include "sly2/tank.h"

/* 112 LAUNCHER (was CID112; 0xC0): aim manager / launcher (darts, guard pebbles, sword missiles); vtable 0x2D7F40, 48 slots */
struct LAUNCHER : LO
{
    /* Own fields from launcher.cpp (its LAUNCH / S views until v1.16; options 503..550). */
    qword posTarget;            /* +0x50 the launch target, in psoSource's frame (option 504) */
    SO *psoSource;              /* +0x60 the frame the target is given in, 0: the world (option 503) */
    undefined4 unknown64[3];
    CLQ clqPredict;             /* +0x70 predicted flight time over the distance (option 545) */
    LM lmPredict;               /* +0x80 its limits (option 546) */
    undefined4 unknown88[2];
    CLQ clqOffset;              /* +0x90 the offset along the direction to the target, over the distance (option 547) */
    LM lmOffset;                /* +0xA0 its limits (option 548) */
    LM lmSpin;                  /* +0xA8 random spin range (option 549) */
    float pitch;                /* +0xB0 ballistic pitch blend (option 550; 0.5 at init) */
    undefined4 unknownB4[3];
    virtual void Init() __asm__("InitCid112");   /* slot 7 */
    virtual CLASSINFO *GetClassInfo() __asm__("GetCid112ClassDesc");   /* slot 6 */
};
typedef char assert_CID112_pitch[SLY2_OFFSETOF(LAUNCHER, pitch) == 0xB0 ? 1 : -1];
typedef char assert_CID112_size[sizeof(LAUNCHER) == 0xC0 ? 1 : -1];

/* 70 SAFEHOUSE (0x240): character-select PO; vtable 0x2D80D0, 81 slots */
/* One safehouse joystick binding: in state machine state `state`, button `buttonMask` fires relay `oid` (0x378: open
   the ThiefNet tab). */
struct SHENTRY { void *state; int buttonMask; int oid; };
struct SAFEHOUSE : PO
{
    /* safehouse.cpp's fields (declared here so the class has its size on every target). */
    int state;                  /* +0x180 0 idle, 1 selecting */
    SM *stateMachine;           /* +0x184 */
    SMA *stateMachineAnim;      /* +0x188 */
    unsigned int bindingCount;  /* +0x18C */
    SHENTRY bindings[12];       /* +0x190 */
    int choiceCount;            /* +0x220 allowed states */
    void *choices[6];           /* +0x224 the allowed states' oids (SetPoChoices: at most 6) */
    char pad23C[0x240 - 0x23C];
    virtual void Init() __asm__("InitSafehouse");   /* slot 7 */
    virtual CLASSINFO *GetClassInfo() __asm__("GetSafehouseClassDesc");   /* slot 6 */
    virtual void PostLoad() __asm__("PostSafehouseLoad");   /* slot 15 */
    virtual void Update() __asm__("UpdateSafehouse");   /* slot 16 */
    virtual void SetCtlState(int nState) __asm__("HandleSafehouseMessage");   /* slot 79 */
};
typedef char assert_SAFEHOUSE_size[sizeof(SAFEHOUSE) == 0x240 ? 1 : -1];

/* 85 LASEN (0x1D0): LASEN laser beams (zap 0x1000); vtable 0x2D8738, 75 slots */
/* LASENCAP: an end cap of a LASEN, drawn at one end of every beam: the object, the ASEG found on it, the ASEGA playing
   it and the curve of its scale channel (CHN::pac; slot 3 AC::Evaluate gives the cap's scale). From sensor.cpp. */
struct LASENCAP
{
    ALO *palo;
    struct ASEG *paseg;
    struct ASEGA *pasega;
    struct AC *pscaler;   /* the scale channel's curve */
};
struct LASEN : SENSOR
{
    ALO *lineObject;            /* +0x190 drawn along each beam */
    ALO *stretchObject;         /* +0x194 scaled to each beam's length */
    int numBeams;               /* +0x198 */
    struct BEAM *beams;         /* +0x19C the beams (sensor.cpp's BEAM, 0x150 each) */
    LASENCAP caps[2];           /* +0x1A0 */
    float endpointSpeed;        /* +0x1C0 time scale of the moving endpoints */
    char pad1C4[0x1D0 - 0x1C4];
    virtual void PostLoad() __asm__("PostLasenLoad");   /* slot 15 */
    virtual CLASSINFO *GetClassInfo() __asm__("GetLasenClassDesc");   /* slot 6 */
    virtual void Init() __asm__("InitLasen");   /* slot 7 */
    virtual void LoadFromBrx(CBinaryInputStream *pbis) __asm__("LoadLasenFromBrx");   /* slot 14 */
    virtual void Update() __asm__("UpdateLasen");   /* slot 16 */
    virtual void RenderSelf() __asm__("RenderLasenSelf");   /* slot 19 */
    virtual void Clone(LO *ploBase) __asm__("CloneLasen");   /* slot 41 */
    virtual SOTREE *NewTree() __asm__("NewLasenTree");   /* slot 53 */
    virtual void OnContactFreed(SO *pso) __asm__("RemoveLasenContactsWith");   /* slot 54 */
    virtual void AddCustomXps(SOPART *ppart, int cbsp, BSP *absp, BSP *pbsp, XP **ppxpFirst) __asm__("AddLasenBeamXps");   /* slot 60 */
};
typedef char assert_LASEN_size[sizeof(LASEN) == 0x1D0 ? 1 : -1];

/* 114 RELAY (0xB0): cutscene / sequence script; vtable 0x2D8C48, 48 slots. A list of RELAYCMD commands built by the
   AddRelay* Splice builtins (sly2/relay.h) and run by RELAYJSG scripts when the relay fires. Layout from
   src/game/gameplay/cutscene/relay.cpp (the former RELAYV view and the per-function views of Init/LoadFromBrx/Clone/
   Update/FireRelay/StopRelay/AddRelayCommand); Sly 1 has no relay class, names from use. */
struct RELAY : LO
{
    int crelaycmd;              /* +0x50 commands in arelaycmd */
    struct RELAYCMD *arelaycmd; /* +0x54 the command list (stack buffer while loading, then world heap) */
    int crelaycmdMax;           /* +0x58 the relay's own command limit, -1 = the default 0x200 (SetRelayCommandLimit) */
    DL dlJsg;                   /* +0x5C running scripts (RELAYJSG, link at +0xE4) */
    struct RELAYJSG *pjsgSignal; /* +0x68 the script waiting on a Signal command */
    LO *ploFirer;               /* +0x6C who fired the relay */
    float tFired;               /* +0x70 time since it fired (real time when the LO runs unpaused) */
    int fFiring;                /* +0x74 */
    int fBusy;                  /* +0x78 holds the LO busy mark (SetLoBusyKind) */
    int fAppendEnd;             /* +0x7C close the command list with a BlockEnd before firing */
    int fOwnCommands;           /* +0x80 crelaycmdMax was set: Clone copies the command array (inferred) */
    int calarm;                 /* +0x84 alarms raised by the relay's scripts */
    struct ALARM *apalarm[8];   /* +0x88 */
    int anUnkA8[2];             /* +0xA8 unused by relay.cpp */
    virtual void Init() __asm__("InitRelay");   /* slot 7 */
    virtual CLASSINFO *GetClassInfo() __asm__("GetRelayClassDesc");   /* slot 6 */
    virtual void LoadFromBrx(CBinaryInputStream *pbis) __asm__("LoadRelayFromBrx");   /* slot 14 */
    virtual void Update() __asm__("UpdateRelay");   /* slot 16 */
    virtual void Clone(LO *ploBase) __asm__("CloneRelay");   /* slot 41 */
};
typedef char assert_RELAY_size[sizeof(RELAY) == 0xB0 ? 1 : -1];
typedef char assert_RELAY_dlJsg[SLY2_OFFSETOF(RELAY, dlJsg) == 0x5C ? 1 : -1];
typedef char assert_RELAY_fOwnCommands[SLY2_OFFSETOF(RELAY, fOwnCommands) == 0x80 ? 1 : -1];
typedef char assert_RELAY_apalarm[SLY2_OFFSETOF(RELAY, apalarm) == 0x88 ? 1 : -1];

/* 106 CID106 (0x160): mesh deformed along a path; vtable 0x2D8DD8, 75 slots */
struct CID106 : SO
{
    char pad150[0x160 - 0x150];
    virtual void RenderSelf() __asm__("RenderCid106Self");   /* slot 19 */
    virtual CLASSINFO *GetClassInfo() __asm__("GetCid106ClassDesc");   /* slot 6 */
    virtual void PostLoad() __asm__("PostCid106Load");   /* slot 15 */
};
typedef char assert_CID106_size[sizeof(CID106) == 0x160 ? 1 : -1];

/* 139 LOOP (0x70); vtable 0x2D93C0, 10 slots */
struct LOOP : SAA
{
    LM loopTimeRange;       /* +0x50 [S] (shdanim.cpp) */
    LM pauseTimeRange;      /* +0x58 [S] */
    float frameRate;        /* +0x60 [S] */
    float frame;            /* +0x64 [S] */
    float pauseTime;        /* +0x68 [S] */
    float pauseRemaining;   /* +0x6C [S] */
    virtual void Init(SAAF *psaaf) __asm__("InitLoop");   /* slot 7 */
    virtual CLASSINFO *GetClassInfo() __asm__("GetLoopClassDesc");   /* slot 6 */
    virtual void Update() __asm__("UpdateLoop__FP4LOOPf");   /* slot 8 */
    virtual void GetCompletion() __asm__("GetLoopCompletion__FP4LOOP");   /* slot 9 */
};
typedef char assert_LOOP_size[sizeof(LOOP) == 0x70 ? 1 : -1];
typedef char assert_LOOP_pauseRemaining[SLY2_OFFSETOF(LOOP, pauseRemaining) == 0x6C ? 1 : -1];

/* 140 PINGPONG (0x70); vtable 0x2D9360, 10 slots */
struct PINGPONG : SAA
{
    LM loopTimeRange;       /* +0x50 [S] (shdanim.cpp) */
    LM pauseTimeRange;      /* +0x58 [S] */
    float frameRate;        /* +0x60 [S] */
    float frame;            /* +0x64 [S] */
    float pauseTime;        /* +0x68 [S] */
    float pauseRemaining;   /* +0x6C [S] */
    virtual void Init(SAAF *psaaf) __asm__("InitPingpong");   /* slot 7 */
    virtual CLASSINFO *GetClassInfo() __asm__("GetPingpongClassDesc");   /* slot 6 */
    virtual void Update() __asm__("UpdatePingpong__FP8PINGPONGf");   /* slot 8 */
    virtual void GetCompletion() __asm__("GetPingpongCompletion__FP8PINGPONG");   /* slot 9 */
};
typedef char assert_PINGPONG_size[sizeof(PINGPONG) == 0x70 ? 1 : -1];

/* 141 SHUFFLE (0x60); vtable 0x2D9300, 10 slots */
struct SHUFFLE : SAA
{
    float pauseMin;         /* +0x50 [S] (shdanim.cpp) */
    float pauseMax;         /* +0x54 [S] */
    float pauseTime;        /* +0x58 [S] */
    int unknown5C;
    virtual void Init(SAAF *psaaf) __asm__("InitShuffle__FP7SHUFFLEP4SAAF");   /* slot 7 */
    virtual CLASSINFO *GetClassInfo() __asm__("GetShuffleClassDesc");   /* slot 6 */
    virtual void Update() __asm__("UpdateShuffle__FP7SHUFFLEf");   /* slot 8 */
};
typedef char assert_SHUFFLE_size[sizeof(SHUFFLE) == 0x60 ? 1 : -1];

/* 142 EYES (0x80); vtable 0x2D92A0, 10 slots */
enum EYESS { EYESS_Nil = -1, EYESS_Open = 0, EYESS_Closing = 1, EYESS_Closed = 2, EYESS_Opening = 3 };
struct EYES : SAA
{
    float blinkTime;        /* +0x50 [S] (shdanim.cpp) */
    float openTimeMin;      /* +0x54 [S] */
    float openTimeMax;      /* +0x58 [S] */
    float doubleBlinkChance;    /* +0x5C [S] */
    int unknown60;          /* +0x60 [S] a second oid DoesEyesMatchOid accepts */
    EYESS eyess;            /* +0x64 [S] */
    float stateTime;        /* +0x68 [S] */
    float openTime;         /* +0x6C [S] */
    float frameRate;        /* +0x70 [S] */
    float frame;            /* +0x74 [S] */
    float openFraction;     /* +0x78 [S] */
    int unknown7C;
    virtual void Update() __asm__("UpdateEyes");   /* slot 8 */
    virtual CLASSINFO *GetClassInfo() __asm__("GetEyesClassDesc");   /* slot 6 */
    virtual void Init(SAAF *psaaf) __asm__("InitEyes");   /* slot 7 */
    virtual int DoesMatchOid(int *poid) __asm__("DoesEyesMatchOid");   /* slot 10 */
};
typedef char assert_EYES_size[sizeof(EYES) == 0x80 ? 1 : -1];
typedef char assert_EYES_openFraction[SLY2_OFFSETOF(EYES, openFraction) == 0x78 ? 1 : -1];

/* 143 SCROLLER (0x70); vtable 0x2D9240, 10 slots */
struct SCROLLER : SAA
{
    float uSpeed;           /* +0x50 [S] (shdanim.cpp) */
    float vSpeed;           /* +0x54 [S] */
    float uRange;           /* +0x58 [S] */
    float vRange;           /* +0x5C [S] */
    float uSpeedScale;      /* +0x60 [S] master speed multipliers (SetScrollerMasterSpeeds) */
    float vSpeedScale;      /* +0x64 [S] */
    int unknown68[2];
    virtual void Update() __asm__("UpdateScroller");   /* slot 8 */
    virtual CLASSINFO *GetClassInfo() __asm__("GetScrollerClassDesc");   /* slot 6 */
    virtual void Init(SAAF *psaaf) __asm__("InitScroller__FP8SCROLLERP4SAAF");   /* slot 7 */
    virtual void GetCompletion() __asm__("GetScrollerCompletion__FP8SCROLLER");   /* slot 9 */
};
typedef char assert_SCROLLER_size[sizeof(SCROLLER) == 0x70 ? 1 : -1];
typedef char assert_SCROLLER_vSpeedScale[SLY2_OFFSETOF(SCROLLER, vSpeedScale) == 0x64 ? 1 : -1];

/* 144 CIRCLER (0x60); vtable 0x2D91E0, 10 slots */
struct CIRCLER : SAA
{
    float angularSpeed;     /* +0x50 [S] (shdanim.cpp) */
    float radius;           /* +0x54 [S] */
    float uCenter;          /* +0x58 [S] */
    float vCenter;          /* +0x5C [S] */
    virtual void Init(SAAF *psaaf) __asm__("InitCircler");   /* slot 7 */
    virtual CLASSINFO *GetClassInfo() __asm__("GetCirclerClassDesc");   /* slot 6 */
    virtual void Update() __asm__("UpdateCircler__FP7CIRCLERf");   /* slot 8 */
    virtual void GetCompletion() __asm__("GetCirclerCompletion__FP7CIRCLER");   /* slot 9 */
};
typedef char assert_CIRCLER_size[sizeof(CIRCLER) == 0x60 ? 1 : -1];

/* 97 SKY (0x160): sky dome; render kind 1; vtable 0x2D9500, 75 slots */
struct SKY : SO
{
    int followCameraZ;          /* +0x150 nonzero: also follow the camera's height (sky.cpp) */
    int unk154[3];
    virtual void PostLoad() __asm__("PostSkyLoad");   /* slot 15 */
    virtual CLASSINFO *GetClassInfo() __asm__("GetSkyClassDesc");   /* slot 6 */
    virtual void Update() __asm__("UpdateSky");   /* slot 16 */
};
typedef char assert_SKY_size[sizeof(SKY) == 0x160 ? 1 : -1];

/* 39 CID39 (0x110): GRABSPOT XFM, list D_003E13D8; vtable 0x2D9DD0, 50 slots */
struct CID39 : XFM, GRABSPOT
{
    DLE dleGrab;            /* +0x104 link in g_sw.dlCid39 (grab.cpp GatherJtGrabCandidates, grab mask 0x400) */
    int unknown10C;
    virtual void Init() __asm__("InitGrabSpotXfm");   /* slot 7 */
    virtual CLASSINFO *GetClassInfo() __asm__("GetCid39ClassDesc");   /* slot 6 */
    virtual void PostLoad() __asm__("PostGrabSpotXfmLoad");   /* slot 15 */
    virtual void OnRemove() __asm__("OnGrabSpotXfmRemove");   /* slot 22 */
    virtual void OnAdd() __asm__("OnGrabSpotXfmAdd");   /* slot 23 */
    virtual void Clone(LO *ploBase) __asm__("CloneGrabSpotXfm");   /* slot 41 */
    virtual void GetGrabPos(float sParam, STEPPLAYER *pstepplayer, qword *ppos, float *pgHeading) __asm__("GetOffsetToObjectPosition");   /* GRABSPOT slot 3 (delta -224) */
    virtual LO *GetOwner() __asm__("GetCid39GrabspotOwner__FP5CID39");   /* GRABSPOT slot 6 (delta -224) */
};
typedef char assert_CID39_size[sizeof(CID39) == 0x110 ? 1 : -1];
typedef char assert_CID39_dleGrab[SLY2_OFFSETOF(CID39, dleGrab) == 0x104 ? 1 : -1];

/* 46 RECONTARGET (was CID46; 0x120): recon photo target; vtable 0x2D9F70, 50 slots */
/* RECONTARGET: a recon target (the photographs the recon prompt picks out; recon.cpp, RECONSEL). Member names from
   recon.cpp's view, in the modern style since v1.26 (were sRadius, sHead, fDone, rFade). */
struct RECONTARGET : XFM
{
    float selectionRadius;  /* +0xE0 selection radius (75) */
    float visibilityRadius; /* +0xE4 head offset / visibility radius (defaults to selectionRadius) */
    float threshold;        /* +0xE8 (0.25) */
    TASK *link;             /* +0xEC the task the photograph completes */
    int isDone;             /* +0xF0 done without a task */
    int id;                 /* +0xF4 its text */
    int idPending;          /* +0xF8 (defaults to id) */
    int key;                /* +0xFC dialog */
    LO *fadeObject;         /* +0x100 faded in / out by distance (rFade) */
    float fadeDistance;     /* +0x104 (5000) */
    int state;              /* +0x108 visibility: 1 visible, 2 hidden, 3 partly */
    int mode;               /* +0x10C */
    int choice;             /* +0x110 */
    undefined4 unknown114[3];
    virtual void PostLoad() __asm__("PostReconTargetLoad");   /* slot 15 */
    virtual CLASSINFO *GetClassInfo() __asm__("GetCid46ClassDesc");   /* slot 6 */
    virtual void HandleMessage(MSGID msgid, void *pv) __asm__("HandleReconTargetMessage");   /* slot 5 */
    virtual void Init() __asm__("InitReconTarget");   /* slot 7 */
    virtual void LoadFromBrx(CBinaryInputStream *pbis) __asm__("LoadReconTargetFromBrx");   /* slot 14 */
};
typedef char assert_CID46_size[sizeof(RECONTARGET) == 0x120 ? 1 : -1];

/* 125 GUARDMGR (was CID125; 0x60): guard manager (oid 0x656); vtable 0x2DA3D8, 48 slots */
struct GUARDMGR : LO
{
    int guardCount;         /* +0x50 [S] the guards in the world (PostGuardManagerLoad) */
    STEPGUARD **guards;     /* +0x54 [S] */
    char pad58[0x60 - 0x58];
    virtual void Init() __asm__("InitGuardManager");   /* slot 7 */
    virtual CLASSINFO *GetClassInfo() __asm__("GetCid125ClassDesc");   /* slot 6 */
    virtual void PostLoad() __asm__("PostGuardManagerLoad");   /* slot 15 */
    virtual void Update() __asm__("UpdateGuardManager");   /* slot 16 */
};
typedef char assert_CID125_size[sizeof(GUARDMGR) == 0x60 ? 1 : -1];
typedef char assert_CID125_guards[SLY2_OFFSETOF(GUARDMGR, guards) == 0x54 ? 1 : -1];

/* 129 WIDGET129 (0x220): WIDGET: objective counter (computed per frame); vtable 0x2DAD90, 50 slots */
struct ITEMCOUNTERENTRY { OID oid; TASK *task; };   /* an item counter entry: the task's oid, resolved by PostLoad */
struct WIDGET129 : WIDGET
{
    int numEntriesA;             /* +0x110 AddItemCounterOidA (at most 16) */
    ITEMCOUNTERENTRY entriesA[16];   /* +0x114 */
    int numEntriesB;             /* +0x194 AddItemCounterOidB; the counted done/total/remaining texts are for these */
    ITEMCOUNTERENTRY entriesB[16];   /* +0x198 */
    char pad218[0x220 - 0x218];
    virtual void PostLoad() __asm__("PostLoadItemCounterWidget");   /* slot 15 */
    virtual CLASSINFO *GetClassInfo() __asm__("GetWidget129ClassDesc");   /* slot 6 */
    virtual void Update() __asm__("UpdateItemCounterWidget");   /* slot 16 */
};
typedef char assert_WIDGET129_size[sizeof(WIDGET129) == 0x220 ? 1 : -1];
typedef char assert_WIDGET129_numEntriesB[SLY2_OFFSETOF(WIDGET129, numEntriesB) == 0x194 ? 1 : -1];

/* 127 WIDGET127 (0x120): WIDGET: modal hands-off box; vtable 0x2DABF0, 50 slots */
struct WIDGET127 : WIDGET
{
    int isOpen;                  /* +0x110 set while the box is up (ShowModalBoxWidget, UpdateModalBoxWidget) */
    int keepsGamePaused;         /* +0x114 0: dismissing the box restarts the current job */
    char pad118[0x120 - 0x118];
    virtual void Init() __asm__("InitModalBoxWidget");   /* slot 7 */
    virtual CLASSINFO *GetClassInfo() __asm__("GetWidget127ClassDesc");   /* slot 6 */
    virtual void Update() __asm__("UpdateModalBoxWidget");   /* slot 16 */
    virtual void Show(int idLabel) __asm__("ShowModalBoxWidget");   /* slot 49 */
};
typedef char assert_WIDGET127_size[sizeof(WIDGET127) == 0x120 ? 1 : -1];
typedef char assert_WIDGET127_keepsGamePaused[SLY2_OFFSETOF(WIDGET127, keepsGamePaused) == 0x114 ? 1 : -1];

/* 128 WIDGET128 (0x110): WIDGET: job completion banner (pauses clock slot 7); vtable 0x2DAA50, 50 slots */
struct WIDGET128 : WIDGET
{
    virtual void Init() __asm__("InitWidget128");   /* slot 7 */
    virtual CLASSINFO *GetClassInfo() __asm__("GetWidget128ClassDesc");   /* slot 6 */
};
typedef char assert_WIDGET128_size[sizeof(WIDGET128) == 0x110 ? 1 : -1];

/* 105 CID105 (0x1A0): motion trail; vtable 0x2DB0D0, 75 slots */
struct CID105 : SO
{
    char pad150[0x1A0 - 0x150];
    virtual void AddHierarchy() __asm__("AddTrailHierarchy");   /* slot 11 */
    virtual CLASSINFO *GetClassInfo() __asm__("GetCid105ClassDesc");   /* slot 6 */
    virtual void Init() __asm__("InitTrail");   /* slot 7 */
    virtual void PostLoad() __asm__("PostTrailLoad");   /* slot 15 */
    virtual void Update() __asm__("UpdateTrail");   /* slot 16 */
    virtual void UpdateXfWorld() __asm__("ProjectTrailTransform");   /* slot 17 */
    virtual void RenderSelf() __asm__("RenderTrail");   /* slot 19 */
};
typedef char assert_CID105_size[sizeof(CID105) == 0x1A0 ? 1 : -1];

/* 21 CID21 (0x100): camera-related XFM (task visibility lists); vtable 0x2DB378, 50 slots */
struct CID21 : XFM
{
    DLE listLink;               /* +0xE0 link in g_sw.dlCid21 (target.cpp) */
    float maxDistance;          /* +0xE8 ignored beyond this distance (inferred) */
    float rangeMin;             /* +0xEC LM limits of the second distance (height or lateral offset; inferred) */
    float rangeMax;             /* +0xF0 */
    int priority;               /* +0xF4 higher wins over nearer (inferred) */
    int unkF8[2];
    virtual void OnAdd() __asm__("OnLookTargetAdd");   /* slot 23 */
    virtual CLASSINFO *GetClassInfo() __asm__("GetCid21ClassDesc");   /* slot 6 */
    virtual void Init() __asm__("InitLookTarget");   /* slot 7 */
    virtual void OnRemove() __asm__("OnLookTargetRemove");   /* slot 22 */
    virtual void Clone(LO *ploBase) __asm__("CloneLookTarget");   /* slot 41 */
};
typedef char assert_CID21_size[sizeof(CID21) == 0x100 ? 1 : -1];

/* 100 CID100 (0x1B0): tossables (alarm clock tossable_distract, music box); vtable 0x2DBB10, 76 slots. Slot functions
   are named Tossable (InitTossable, UpdateTossable...); the gadgets (CID101/CID102) derive from it. Layout from
   src/game/gameplay/objects/gadget.cpp's CID100V view (names from there; also relay.cpp's/air.cpp's copies and
   throw.cpp's reads). */
struct CID100 : SO
{
    int state;                  /* +0x150 gadget.cpp's TOSSABLE_STATE */
    float time;                 /* +0x154 time of the last state change */
    int pendingContact;         /* +0x158 -1 none; 2 a surface to stick to was found; 4 touched while held (inferred) */
    int gadgetKind;             /* +0x15C GADGET_KIND or another gadget id (throw.cpp charges its gadget mp) */
    SO *nearest;                /* +0x160 homing target (gadget 0x29) */
    SM *stateMachine;           /* +0x164 */
    SMA *smApplication;         /* +0x168 its goal follows the state */
    LO *thrower;                /* +0x16C */
    float maxThrowerDistance;   /* +0x170 farther than this from the thrower it is fired (3000) */
    int blastZapKind;           /* +0x174 zap kind of the blast hits (inferred, as BOMB's) */
    int blastDamage;            /* +0x178 damage of the blast hits (-10) */
    ACT *rotationJob;           /* +0x17C */
    ACT *positionJob;           /* +0x180 */
    int unk184[3];
    qword stickNormal;          /* +0x190 contact normal of the surface to stick to */
    SO *stickTarget;            /* +0x1A0 */
    int unk1A4[3];
    virtual void RemoveHierarchy() __asm__("RemoveTossableHierarchy");   /* slot 12 */
    virtual CLASSINFO *GetClassInfo() __asm__("GetCid100ClassDesc");   /* slot 6 */
    virtual void HandleMessage(MSGID msgid, void *pv) __asm__("HandleTossableMessage");   /* slot 5 */
    virtual void Init() __asm__("InitTossable");   /* slot 7 */
    virtual void AddHierarchy() __asm__("AddTossableHierarchy");   /* slot 11 */
    virtual void LoadFromBrx(CBinaryInputStream *pbis) __asm__("LoadTossableFromBrx");   /* slot 14 */
    virtual void Update() __asm__("UpdateTossable");   /* slot 16 */
    virtual void AdjustNewXp(XP *pxp, int ixpd) __asm__("AdjustTossableNewXp");   /* slot 55 */
    virtual void AdjustXps() __asm__("UpdateTossableContacts");   /* slot 58 */
    virtual int ShouldIgnoreIntersection(SO *psoOther) __asm__("ShouldTossableIgnoreIntersection");   /* slot 61 */
    virtual void PresetAccel() __asm__("OrientTossableToTarget");   /* slot 70 */
    virtual int TakeDamage(ZPR *pzpr) __asm__("TakeTossableDamage");   /* slot 75 */
    virtual void SetState(int state) __asm__("SetTossableState");   /* slot 76 */
};
typedef char assert_CID100_size[sizeof(CID100) == 0x1B0 ? 1 : -1];
typedef char assert_CID100_stickNormal[SLY2_OFFSETOF(CID100, stickNormal) == 0x190 ? 1 : -1];

/* 101 CID101 (0x1B0): Bentley trigger bomb / mines (gadget object); vtable 0x2C9810, 76 slots */
struct CID101 : CID100
{
    virtual CLASSINFO *GetClassInfo() __asm__("GetCid101ClassDesc");   /* slot 6 */
};
typedef char assert_CID101_size[sizeof(CID101) == 0x1B0 ? 1 : -1];

/* 130 WIDGET130 (0x130): WIDGET: the recon selector prompt (recon); vtable 0x2C9670, 50 slots */
struct WIDGET130 : WIDGET
{
    /* the recon selector's prompt (oid 0x362, recon): the selector state to request next in each situation
       (target done, rising, falling, near, empty: names from recon's view; -1 from the constructor) */
    int actorChoice;             /* +0x110 */
    int risingChoice;            /* +0x114 */
    int fallingChoice;           /* +0x118 */
    int nearChoice;              /* +0x11C */
    int emptyChoice;             /* +0x120 */
    char pad124[0x130 - 0x124];
    WIDGET130() __asm__("ConstructWidget130");     /* out of line: engine/presentation/hud.cpp */
    virtual CLASSINFO *GetClassInfo() __asm__("GetWidget130ClassDesc");   /* slot 6 */
};
typedef char assert_WIDGET130_size[sizeof(WIDGET130) == 0x130 ? 1 : -1];

/* 118 CID118 (0x60); vtable 0x2C94E0, 48 slots */
struct CID118 : LO
{
    char pad50[0x60 - 0x50];
    virtual CLASSINFO *GetClassInfo() __asm__("GetCid118ClassDesc");   /* slot 6 */
};
typedef char assert_CID118_size[sizeof(CID118) == 0x60 ? 1 : -1];

/* 153 JSG (0xB0): relay interpreter; vtable 0x2C94A0, 6 slots */
struct JSG : BASIC
{
    char pad10[0xB0 - 0x10];
    virtual CLASSINFO *GetClassInfo() __asm__("GetJsgClassDesc");   /* slot 6 */
};
typedef char assert_JSG_size[sizeof(JSG) == 0xB0 ? 1 : -1];

/* 62 JEANBISON (0x1010); vtable 0x2C91C8, 89 slots */
struct JEANBISON : STEPGUARD
{
    virtual CLASSINFO *GetClassInfo() __asm__("GetJeanbisonClassDesc");   /* slot 6 */
};
typedef char assert_JEANBISON_size[sizeof(JEANBISON) == 0x1010 ? 1 : -1];

/* 65 ARPEGGIO (0x1010); vtable 0x2C8EF0, 89 slots */
struct ARPEGGIO : STEPGUARD
{
    virtual CLASSINFO *GetClassInfo() __asm__("GetArpeggioClassDesc");   /* slot 6 */
};
typedef char assert_ARPEGGIO_size[sizeof(ARPEGGIO) == 0x1010 ? 1 : -1];

/* 66 BEAR (0x1010); vtable 0x2C8C18, 89 slots */
struct BEAR : STEPGUARD
{
    virtual CLASSINFO *GetClassInfo() __asm__("GetBearClassDesc");   /* slot 6 */
};
typedef char assert_BEAR_size[sizeof(BEAR) == 0x1010 ? 1 : -1];

/* 67 BEARCUB (0x1010): single instance; vtable 0x2C8940, 89 slots */
struct BEARCUB : STEPGUARD
{
    virtual CLASSINFO *GetClassInfo() __asm__("GetBearcubClassDesc");   /* slot 6 */
};
typedef char assert_BEARCUB_size[sizeof(BEARCUB) == 0x1010 ? 1 : -1];

/* 102 CID102 (0x1D0): guard trap (drops a prize); vtable 0x2DB8A0, 76 slots */
struct CID102 : CID100
{
    char pad1B0[0x1D0 - 0x1B0];
    virtual void Init() __asm__("InitGuardTrap");   /* slot 7 */
    virtual CLASSINFO *GetClassInfo() __asm__("GetCid102ClassDesc");   /* slot 6 */
    virtual void Update() __asm__("UpdateGuardTrap");   /* slot 16 */
    virtual void AddExternalAccelerations(XA *pxa) __asm__("PushGuardTrapAway");   /* slot 71 */
    virtual void SetState(int state) __asm__("SelectGuardTrapTarget");   /* slot 76 */
};
typedef char assert_CID102_size[sizeof(CID102) == 0x1D0 ? 1 : -1];

/* 69 TURRET (0x1E0): grapple turret; vtable 0x2DBD80, 81 slots */
struct TURRET : PO
{
    /* turret.cpp's TURRETV view names these (declared here so the class has its size on every target); options 910..922
       and 325. */
    SO *target;                 /* +0x180 the aim target */
    int aim;                    /* +0x184 aim at the target */
    int follow;                 /* +0x188 zoom on the target */
    float pitch, yaw, vpitch, vyaw;   /* +0x18C */
    LM yawRange;                /* +0x19C */
    LM pitchRange;              /* +0x1A4 */
    LM range;                   /* +0x1AC zoom range */
    float blend, velocity, strength;  /* +0x1B4 */
    LO *projectileProxy;        /* +0x1C0 what it fires (a PROXY, AcquireProxyObject) */
    struct LAUNCHER *launcher;    /* +0x1C4 its launcher (option 915: a CID 0x70) */
    SO *lastLaunched;           /* +0x1C8 last object fired */
    int isActRunning;           /* +0x1CC the rotation act is running */
    LO *self;                   /* +0x1D0 */
    int isActive;               /* +0x1D4 */
    void *rotationAct;          /* +0x1D8 rotation act */
    char pad1DC[0x1E0 - 0x1DC];
    virtual void PostLoad() __asm__("PostTurretLoad");   /* slot 15 */
    virtual CLASSINFO *GetClassInfo() __asm__("GetTurretClassDesc");   /* slot 6 */
    virtual void Init() __asm__("InitTurret");   /* slot 7 */
    virtual void Update() __asm__("UpdateTurret");   /* slot 16 */
    virtual void Clone(LO *ploBase) __asm__("CloneTurret");   /* slot 41 */
    virtual int ShouldIgnoreIntersection(SO *psoOther) __asm__("FilterTurretProxySource");   /* slot 61 */
    virtual void OnActive(int fActive) __asm__("OnTurretActive");   /* slot 78 */
};
typedef char assert_TURRET_size[sizeof(TURRET) == 0x1E0 ? 1 : -1];

#include "sly2/vis.h"   /* 5 VISZONE, the vis zone (a header of its own so cm.cpp can read it without classes.h) */

/* 23 CID23 (0x140): VOL child; vtable 0x2DC210, 50 slots */
struct CID23 : VOL
{
    DLE dleList;                /* +0x130 link in SW::dlCid23 */
    char pad138[0x140 - 0x138];
    virtual void OnAdd() __asm__("OnCid23Add");   /* slot 23 */
    virtual CLASSINFO *GetClassInfo() __asm__("GetCid23ClassDesc");   /* slot 6 */
    virtual void OnRemove() __asm__("FreezeVol");   /* slot 22 */
};
typedef char assert_CID23_size[sizeof(CID23) == 0x140 ? 1 : -1];

/* 41 CID41 (0xE0): in-level teleport destination (WARP_* strings); vtable 0x2DC890, 50 slots */
struct CID41 : XFM
{
    virtual void LoadFromBrx(CBinaryInputStream *pbis) __asm__("LoadTeleportDestinationFromBrx");   /* slot 14 */
    virtual CLASSINFO *GetClassInfo() __asm__("GetCid41ClassDesc");   /* slot 6 */
};
typedef char assert_CID41_size[sizeof(CID41) == 0xE0 ? 1 : -1];

/* 26 EXIT (0x190): level exit volume; vtable 0x2DC6F0, 50 slots. Sends the player to another world: plays the
   queued movies, then wipes to the target world. Layout from src/game/level/exit_warp.cpp (the former EXITV view). */
struct EXIT : VOL
{
    int isDefault;              /* +0x130 the exit taken by default */
    int redirectsToDefault;     /* +0x134 taking it takes the default exit instead */
    int skipWipe;               /* +0x138 */
    int state;                  /* +0x13C 0 player inside, 1 left, 2 off, 3 armed, 4 warping */
    float stateTime;            /* +0x140 time of the last state change */
    void *targetWorld;          /* +0x144 target world blob */
    int movieCount;             /* +0x148 movies played before the wipe */
    void **movies;              /* +0x14C */
    int targetWarpOid;          /* +0x150 (0x189) */
    int wipeOid;                /* +0x154 (-1) */
    float wipeTime;             /* +0x158 */
    float rearmDelay;           /* +0x15C after leaving, re-armed once this has passed */
    float wipeDelay;            /* +0x160 delay from taking the exit to the wipe */
    int taskStateCount;         /* +0x164 */
    TASKSTATEREQ taskStates[4];    /* +0x168 */
    int unk188[2];              /* +0x188 unused by exit_warp.cpp */
    virtual void Init() __asm__("InitExit");   /* slot 7 */
    virtual CLASSINFO *GetClassInfo() __asm__("GetExitClassDesc");   /* slot 6 */
    virtual void LoadFromBrx(CBinaryInputStream *pbis) __asm__("LoadExitFromBrx");   /* slot 14 */
    virtual void Update() __asm__("UpdateExit");   /* slot 16 */
};
typedef char assert_EXIT_size[sizeof(EXIT) == 0x190 ? 1 : -1];
typedef char assert_EXIT_taskStates[SLY2_OFFSETOF(EXIT, taskStates) == 0x168 ? 1 : -1];

/* WARPPAIR: an object a warp moves on trigger, and the object it is moved to (MatchTransform). */
struct WARPPAIR { OID object; OID target; };

/* 42 WARP (0x180): destinations; entrance_* aliases; vtable 0x2DC550, 50 slots. A point the player is placed at on
   arrival: moves the paired objects to their targets, applies its rsmgs and task states and resets every exit. Layout
   from src/game/level/exit_warp.cpp (the former WARPV view; relay.cpp reads the aliases). */
struct WARP : XFM
{
    int rsmgCount;              /* +0xE0 */
    RSMG rsmgs[4];              /* +0xE4 */
    int taskStateCount;         /* +0x124 */
    TASKSTATEREQ taskStates[4];    /* +0x128 */
    unsigned int pairCount;     /* +0x148 objects moved on trigger */
    WARPPAIR pairs[4];          /* +0x14C */
    int aliasCount;             /* +0x16C other oids the warp answers to */
    OID aliases[4];             /* +0x170 */
    virtual void LoadFromBrx(CBinaryInputStream *pbis) __asm__("LoadWarpFromBrx");   /* slot 14 */
    virtual CLASSINFO *GetClassInfo() __asm__("GetWarpClassDesc");   /* slot 6 */
};
typedef char assert_WARP_size[sizeof(WARP) == 0x180 ? 1 : -1];
typedef char assert_WARP_aliases[SLY2_OFFSETOF(WARP, aliases) == 0x170 ? 1 : -1];

/* 89 WATER (0x190): zap kind 0x2000; vtable 0x2DCA30, 75 slots */
struct WATER : SO
{
    /* Its own fields (water.cpp uses the class itself since v1.18). */
    XA *pxaFirst;               /* +0x150 the objects in it: XA records (source the water, other the object), linked
                                   through XA::nextInSource (UpdateWater / UpdateSwXaList) */
    char pad154[0x160 - 0x154];
    VECTOR posCurrent;          /* +0x160 the current (CalculateWaterCurrent) */
    char pad16C[0x170 - 0x16C];
    struct EXPL *recipient;     /* +0x170 the splash effect: exploded (ExplodeExplso) when an object hits the surface fast
                                   (option 797: a CID 0x1E, EXPL) */
    struct EXPL *emitter;       /* +0x174 (option 798: an EXPL) */
    SO *splashObject;           /* +0x178 emitted at fast entries (WakeSoWater; an SO in the snapshots) */
    SO *rippleObject;           /* +0x17C emitted at wakes (WakeSoWater; an SO in the snapshots) */
    int hasWakeSound;           /* +0x180 objects moving through it play their wake sound (WakeSoWater) */
    int unknown184;             /* +0x184 */
    int zapKind;                /* +0x188 the zap it inflicts on submerged parts (CanWaterInflictZap) (inferred) */
    char pad18C[0x190 - 0x18C];
    virtual void PostLoad() __asm__("PostWaterLoad");   /* slot 15 */
    virtual CLASSINFO *GetClassInfo() __asm__("GetWaterClassDesc");   /* slot 6 */
    virtual void HandleMessage(MSGID msgid, void *pv) __asm__("HandleWaterMessage");   /* slot 5 */
    virtual void Init() __asm__("InitWater");   /* slot 7 */
    virtual void RemoveHierarchy() __asm__("RemoveWaterHierarchy");   /* slot 12 */
    virtual void Update() __asm__("UpdateWater");   /* slot 16 */
    virtual void UpdateBounds() __asm__("UpdateWaterBounds");   /* slot 49 */
    virtual void InitPhys() __asm__("InitWaterPhysics");   /* slot 64 */
    virtual void AddExternalAccelerations(XA *pxa) __asm__("AddWaterExternalAccelerations");   /* slot 71 */
    virtual int IsPosInside(qword pos) __asm__("IsPosInWater");   /* slot 73 */
};
typedef char assert_WATER_size[sizeof(WATER) == 0x190 ? 1 : -1]; typedef char assert_WATER_hasWakeSound[SLY2_OFFSETOF(WATER, hasWakeSound) == 0x180 ? 1 : -1];

/* 107 WAYPT (0x190): job marker mission_start_*; vtable 0x2DCC98, 75 slots */
struct WAYPT : SO
{
    int mode;                   /* +0x150 when it shows: 0 unless unknown3B0C, 1 only with unknown3B0C, 2 for a job */
    int triggerClass;           /* +0x154 class of hero that activates it */
    float inactiveRadius;       /* +0x158 activation radius while inactive */
    float activeRadius;         /* +0x15C ... and while active */
    struct TASK *attachment;    /* +0x160 the task it marks (the label is its job's name plate) */
    int isActive;               /* +0x164 */
    LO *source;                 /* +0x168 the marked object (the beam stands on it) */
    SO *marker;                 /* +0x16C the marker at the top */
    SO *beam;                   /* +0x170 */
    SO *base;                   /* +0x174 */
    int clipFrame;              /* +0x178 camera frame of the last height clip */
    int hasCurtainHit;          /* +0x17C */
    float height;               /* +0x180 beam height added to clear the scenery (CalculateWayptCurtainExtraHeight) */
    int requiredPowerup;        /* +0x184 hidden once this powerup is acquired (-1: none) */
    void *jobCondition;         /* +0x188 shown only while this job condition holds */
    int unk18C;
    virtual void PostLoad() __asm__("PostWayptLoad");   /* slot 15 */
    virtual CLASSINFO *GetClassInfo() __asm__("GetWayptClassDesc");   /* slot 6 */
    virtual void Init() __asm__("InitWaypt");   /* slot 7 */
    virtual void Update() __asm__("UpdateWaypt");   /* slot 16 */
    virtual void RenderSelf() __asm__("RenderWaypt");   /* slot 19 */
};
typedef char assert_WAYPT_size[sizeof(WAYPT) == 0x190 ? 1 : -1];

/* 115 WR (0x900); vtable 0x2DCF00, 48 slots: defined in sly2/wr.h (the anim header) with its fields. */
struct WR;

/* The OwnFields accessors declared in sly2/xfm.h and sly2/so.h (GetBarrelOwnFields): each returns the class's GRABSPOT
   base, which the option records address. A plain static_cast<GRABSPOT *>(p) adds a null test (movz) that the
   linkonce copies do not have, and so does a reference cast; the base offset is therefore taken from a static_cast
   of a non-null constant (folded to sizeof(first base) on the EE, the ABI's offset elsewhere) and added to p. */
#define SLY2_GRABSPOT_BASE(T, p) ((void *)((char *)(p) + ((char *)static_cast<GRABSPOT *>((T *)0x100) - (char *)0x100)))
inline void *GetCid12OwnFields(WALLSPOT *p) { return SLY2_GRABSPOT_BASE(WALLSPOT, p); }
inline void *GetCid34OwnFields(POLESPOT *p) { return SLY2_GRABSPOT_BASE(POLESPOT, p); }
inline void *GetCid38OwnFields(CID38 *p) { return SLY2_GRABSPOT_BASE(CID38, p); }
inline void *GetCid35OwnFields(CID35 *p) { return SLY2_GRABSPOT_BASE(CID35, p); }
inline void *GetCid27OwnFields(CID27 *p) { return SLY2_GRABSPOT_BASE(CID27, p); }
inline void *GetHndOwnFields(HND *phnd) { return SLY2_GRABSPOT_BASE(HND, phnd); }
inline void *GetPipeOwnFields(PIPE *ppipe) { return SLY2_GRABSPOT_BASE(PIPE, ppipe); }
inline void *GetRailOwnFields(RAIL *prail) { return SLY2_GRABSPOT_BASE(RAIL, prail); }
inline void *GetCid39OwnFields(CID39 *p) { return SLY2_GRABSPOT_BASE(CID39, p); }
inline void *GetBarrelOwnFields(BARREL *pbarrel) { return SLY2_GRABSPOT_BASE(BARREL, pbarrel); }

#endif /* SLY2_CLASSES_H */
