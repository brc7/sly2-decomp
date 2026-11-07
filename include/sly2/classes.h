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
   0x2C8940..0x2CA550's order: CID152, CM, CID154, ASEGBL, CID149 and CID150 (so.h), POCTL (po.h), EXPLO
   (emitter.h), then GUARD and the rest below. */
#include "sly2/basic.h"
#include "sly2/lo.h"

/* 152 CID152 (0x250): sound manager (find-sound-object~); vtable 0x2CA510, 6 slots */
struct CID152 : BASIC
{
    char pad10[0x250 - 0x10];
    virtual CLASSINFO *GetClassInfo() __asm__("GetCid152ClassDesc");   /* slot 6 */
};
typedef char assert_CID152_size[sizeof(CID152) == 0x250 ? 1 : -1];

#include "sly2/cm.h"

/* 154 CID154 (0x30): ticker / hint box (find-ticker-object~); vtable 0x2CA490, 6 slots */
struct CID154 : BASIC
{
    char pad10[0x30 - 0x10];
    virtual CLASSINFO *GetClassInfo() __asm__("GetCid154ClassDesc");   /* slot 6 */
};
typedef char assert_CID154_size[sizeof(CID154) == 0x30 ? 1 : -1];

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
inline PO::PO() { unknown174 = 0; }

/* The grab-spot mix-in (second base of the grab-spot classes): GRABSPOTDATA (0x20 bytes, object/proxy.cpp) and
   its own vptr at +0x20 (vtable 0x2D28D8, 6 slots; hidden-types.md B2.12). Slot names from gameplay/player/grabspot.cpp;
   only GetGrabRange's signature is modelled.
   A class with a secondary base must override at least one of its virtuals exactly when its secondary vtable in the
   ELF differs from the base's own: with -fno-rtti g++ reuses the base's vtable (_vt$8GRABSPOT) for a secondary base
   whose virtuals the class does not override, which changes the constructor's vptr stores. Every grab-spot class
   overrides slot 6 (an 8-byte function at 0x2599E8..); CID36/CID37 override nothing of GRABSPOT and reuse CID35's
   (0x2D1EA8 in the ELF for all three). Those slot-6 functions are free inline functions of xfm.h/so.h (the owner
   getters, link/linkonce-misc); the GetOwner labels below name them exactly, so the key TU that emits a class's vtable
   emits its getter too (.gnu.linkonce.t.<symbol>), the only object that does. */
struct GRABSPOT
{
    char abData[0x20];
    virtual int GetGrabRange() __asm__("GetGrabspotWrapRange");     /* slot 1; key function (grabspot.cpp) */
    virtual void GetClosestParam() __asm__("GetGrabspotClosestParam");         /* slot 2 */
    virtual void GetGrabPos() __asm__("GetGrabspotGrabPosition");              /* slot 3 */
    virtual void AdjustScore() __asm__("AdjustGrabspotScore");             /* slot 4 */
    virtual void GetSparklePos() __asm__("GetGrabspotSparklePosition");           /* slot 5 */
    virtual void GetOwner() __asm__("GetGrabspotOwner");                     /* slot 6 */
};

/* BLIPG's 32 records (0x1C bytes each) with an empty inline constructor: g++ still emits the 32-step loop. */
struct BLIP
{
    char ab[0x1C];
    // Uninitialised blip.
    BLIP() {}
};

struct PATHLINK;
/* A path-link endpoint / go-to goal (0x20), constructed out of line by ClearPointerPair (guardbhv; it zeroes the
   first two words): a shape, an object, or (both NULL) the point at +0x10. PATHLINK holds six, BHVGOTO three (goal,
   waypoint, relocate goal), BHV191 one; tu_1709A8/tu_171F58 build goals on the stack for SetBhvgotoGoal. */
struct PATHLINKEND
{
    SHAPE *shape;           /* +0x00 the goal shape (GetPathlinkEndpointPos projects onto it) */
    LO *object;             /* +0x04 the goal object */
    int unknown8[2];
    qword pos;              /* +0x10 the goal point */
    PATHLINKEND() __asm__("ClearPointerPair");
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
    char pad150[0x190 - 0x150];
    virtual void Init() __asm__("InitSensor");   /* slot 7; key function (sensor.cpp) */
    virtual CLASSINFO *GetClassInfo() __asm__("GetSensorClassDesc");   /* slot 6 */
    virtual int ShouldIgnoreIntersection(SO *psoOther) __asm__("ShouldSensorIgnoreIntersection");   /* slot 61 */
};
typedef char assert_SENSOR_size[sizeof(SENSOR) == 0x190 ? 1 : -1];

/* 90 DPRIZE (0x230): abstract; vtable 0x2D6B98, 76 slots */
struct DPRIZE : SO
{
    char pad150[0x230 - 0x150];
    virtual int TakeDamage(ZPR *pzpr) __asm__("TakeDprizeDamage");   /* slot 75; key function (prize.cpp) */
    virtual CLASSINFO *GetClassInfo() __asm__("GetDprizeClassDesc");   /* slot 6 */
    virtual void HandleMessage(MSGID msgid, void *pv) __asm__("HandleDprizeMessage");   /* slot 5 */
    virtual void Init() __asm__("InitDprize");   /* slot 7 */
    virtual void LoadFromBrx(CBinaryInputStream *pbis) __asm__("LoadDprizeFromBrx");   /* slot 14 */
    virtual void PostLoad() __asm__("PostDprizeLoad");   /* slot 15 */
    virtual void Update() __asm__("UpdateDprize");   /* slot 16 */
    virtual void UpdateXfWorld() __asm__("ProjectDprizeTransform");   /* slot 17 */
    virtual void RenderSelf() __asm__("RenderDprizeSelf");   /* slot 19 */
    virtual void SetState() __asm__("SetDprizeDprizes");   /* slot 76 */
};
typedef char assert_DPRIZE_size[sizeof(DPRIZE) == 0x230 ? 1 : -1];

/* 126 WIDGET (0x110); vtable 0x2DAF30, 50 slots */
struct WIDGET : LO
{
    char pad50[0x110 - 0x50];
    WIDGET() __asm__("WIDGET_ctor");          /* out of line: presentation/hud/WIDGET_ctor.c */
    virtual void RenderSelf() __asm__("RenderWidget");   /* slot 19; key function (hud.cpp) */
    virtual CLASSINFO *GetClassInfo() __asm__("GetWidgetClassDesc");   /* slot 6 */
    virtual void LoadFromBrx(CBinaryInputStream *pbis) __asm__("LoadWidgetFromBrx");   /* slot 14 */
    virtual void PostLoad() __asm__("PostWidgetLoad");   /* slot 15 */
    virtual void Update() __asm__("UpdateWidget");   /* slot 16 */
    virtual void Show(int idLabel) __asm__("ShowWidget");   /* slot 49: shows the widget with an animation label (-1 default; game.cpp ShowCountdownHud, letterbox, relay AddRelayWidgetShow) */
    virtual void Hide() __asm__("HideWidget");   /* slot 50 */
};
typedef char assert_WIDGET_size[sizeof(WIDGET) == 0x110 ? 1 : -1];

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
    void *owner;            /* +0x30 [S] */
    SL instances;           /* +0x34 [S] the instances (shdanim.cpp SAIE) */
    SAA *next;              /* +0x40 [S] next in g_sw.slSaa */
    int cqwMain;            /* +0x44 [S] (glob.cpp SAAFIELDS) */
    void *pvMain;           /* +0x48 [S] */
    int unknown4C;
    virtual void Init(SAAF *psaaf) __asm__("InitSaa");   /* slot 7 (its first new virtual); key function (shdanim.cpp) */
    virtual CLASSINFO *GetClassInfo() __asm__("GetSaaClassDesc");   /* slot 6 */
    virtual void Update() __asm__("UpdateSaa");   /* slot 8 */
    virtual void GetCompletion() __asm__("GetSaaCompletion__FP3SAA");   /* slot 9 */
    virtual void DoesMatchOid() __asm__("DoesSaaMatchOid");   /* slot 10 */
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
    char padE0[0x140 - 0xE0];
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

/* 43 CID43 (0x100): ambient-sound XFM (OnAdd/OnRemove start/stop); vtable 0x2CABD8, 50 slots */
struct CID43 : XFM
{
    char padE0[0x100 - 0xE0];
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
    char pad150[0x180 - 0x150];
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
    BLIP ablip[32];
    virtual void Release() __asm__("ReleaseBlipg");   /* slot 2 */
    virtual CLASSINFO *GetClassInfo() __asm__("GetBlipgClassDesc");   /* slot 6 */
    virtual void Init() __asm__("InitBlipg__FP5BLIPG");   /* slot 7 */
    virtual void RemoveHierarchy() __asm__("OnBlipgRemove__FP5BLIPG");   /* slot 12 */
    virtual void Update() __asm__("UpdateBlipg");   /* slot 16 */
    virtual void UpdateXfWorld() __asm__("ProjectBlipgTransform");   /* slot 17 */
    virtual void RenderSelf() __asm__("RenderBlipgSelf");   /* slot 19 */
    virtual void SubscribeObject() __asm__("SubscribeBlipgObject");   /* slot 51 */
};
typedef char assert_BLIPG_size[sizeof(BLIPG) == 0x460 ? 1 : -1];

/* 79 BOMB (0x210): also guard rock ammo; vtable 0x2CB780, 77 slots */
struct BOMB : SO
{
    char pad150[0x200 - 0x150];
    LO *thrower;            /* +0x200 [S] the object that threw the bomb (a guard ignores its own team's: TakeStepguardDamage) */
    char pad204[0x210 - 0x204];
    virtual void HandleMessage(MSGID msgid, void *pv) __asm__("HandleBombMessage");   /* slot 5 */
    virtual CLASSINFO *GetClassInfo() __asm__("GetBombClassDesc");   /* slot 6 */
    virtual void Init() __asm__("InitBomb");   /* slot 7 */
    virtual void PostLoad() __asm__("PostBombLoad");   /* slot 15 */
    virtual void Update() __asm__("UpdateBomb");   /* slot 16 */
    virtual void OnProxyAcquire(LO *plo) __asm__("ResetBomb");   /* slot 26 */
    virtual void AdjustXps() __asm__("HandleBombContact");   /* slot 58 */
    virtual void AddCustomXps(SOPART *ppart, int n2, int n3, void *pvGeom, XP **ppxpFirst) __asm__("AddBombCustomXps");   /* slot 60 */
    virtual int ShouldIgnoreIntersection(SO *psoOther) __asm__("ShouldBombIgnoreIntersection");   /* slot 61 */
    virtual int TakeDamage(ZPR *pzpr) __asm__("TakeBombDamage");   /* slot 75 */
    virtual void CopyFromSource() __asm__("CloneBomb");   /* slot 76 */
    virtual void ApplyThrow() __asm__("ApplyBombThrow");   /* slot 77 */
};
typedef char assert_BOMB_size[sizeof(BOMB) == 0x210 ? 1 : -1];

/* 103 BOOTY (0x190): replaces cid_table ALARM; vtable 0x2CBBA0, 75 slots */
struct BOOTY : SO
{
    char pad150[0x190 - 0x150];
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

/* 83 BUTTON (0x300): instances: bouncers, buoys; vtable 0x2CC950, 75 slots */
struct BUTTON : SO
{
    char pad150[0x300 - 0x150];
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

/* 24 VOLBTN (0x2D0): REVIEW, high; vtable 0x2CC7B0, 50 slots */
struct VOLBTN : VOL
{
    char pad130[0x2D0 - 0x130];
    virtual void Clone(LO *ploBase) __asm__("CloneVolbtn");   /* slot 41 */
    virtual CLASSINFO *GetClassInfo() __asm__("GetVolbtnClassDesc");   /* slot 6 */
    virtual void Init() __asm__("InitVolbtn");   /* slot 7 */
    virtual void RemoveHierarchy() __asm__("RemoveVolbtnHierarchy");   /* slot 12 */
    virtual void LoadFromBrx(CBinaryInputStream *pbis) __asm__("LoadVolbtnFromBrx");   /* slot 14 */
    virtual void PostLoad() __asm__("PostVolbtnLoad");   /* slot 15 */
    virtual void Update() __asm__("UpdateVolbtn");   /* slot 16 */
};
typedef char assert_VOLBTN_size[sizeof(VOLBTN) == 0x2D0 ? 1 : -1];

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

/* 123 CID123 (0x60): 256-point route; vtable 0x2CCF10, 48 slots */
struct CID123 : LO
{
    char pad50[0x60 - 0x50];
    virtual void PostLoad() __asm__("PostRouteLoad");   /* slot 15 */
    virtual CLASSINFO *GetClassInfo() __asm__("GetCid123ClassDesc");   /* slot 6 */
    virtual void LoadFromBrx(CBinaryInputStream *pbis) __asm__("LoadRouteFromBrx");   /* slot 14 */
};
typedef char assert_CID123_size[sizeof(CID123) == 0x60 ? 1 : -1];

/* 182 BHV182 (0x50): keep pace on route; vtable 0x2CCE90, 14 slots */
struct BHV182 : BHV
{
    char pad30[0x38 - 0x30];
    LO *target;             /* +0x38 [S] the mode's object (SetBhvguardMode) */
    char pad3C[0x50 - 0x3C];
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
    char pad1010[0x1020 - 0x1010];
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
struct DANCER : STEP
{
    char pad2C0[0x410 - 0x2C0];
    virtual void PostLoad() __asm__("PostDancerLoad");   /* slot 15 */
    virtual CLASSINFO *GetClassInfo() __asm__("GetDancerClassDesc");   /* slot 6 */
    virtual void HandleMessage(MSGID msgid, void *pv) __asm__("HandleDancerMessage");   /* slot 5 */
    virtual void RemoveHierarchy() __asm__("RemoveDancerHierarchy");   /* slot 12 */
    virtual void Update() __asm__("UpdateDancer");   /* slot 16 */
    virtual void GetCpdefi(CPDEFI *pcpdefi) __asm__("GetDancerCpdefi");   /* slot 50 */
};
typedef char assert_DANCER_size[sizeof(DANCER) == 0x410 ? 1 : -1];

/* 121 CID121 (0x10B0): rhythm-game manager; vtable 0x2CD5B8, 48 slots */
struct CID121 : LO
{
    char pad50[0x10B0 - 0x50];
    virtual void PostLoad() __asm__("PostRhythmManagerLoad");   /* slot 15 */
    virtual CLASSINFO *GetClassInfo() __asm__("GetCid121ClassDesc");   /* slot 6 */
    virtual void Init() __asm__("InitRhythmMgr");   /* slot 7 */
    virtual void Update() __asm__("UpdateRhythmMgr");   /* slot 16 */
    virtual void RenderSelf() __asm__("DrawRhythmMgr");   /* slot 19 */
};
typedef char assert_CID121_size[sizeof(CID121) == 0x10B0 ? 1 : -1];

/* 122 CID122 (0x60): ? (LoadFromBrx with FILEREC list); vtable 0x2CDA10, 48 slots */
struct CID122 : LO
{
    char pad50[0x60 - 0x50];
    virtual void LoadFromBrx(CBinaryInputStream *pbis) __asm__("LoadCid122FromBrx");   /* slot 14 */
    virtual CLASSINFO *GetClassInfo() __asm__("GetCid122ClassDesc");   /* slot 6 */
};
typedef char assert_CID122_size[sizeof(CID122) == 0x60 ? 1 : -1];

/* 59 DIMITRI (0x1020); vtable 0x2CDE40, 89 slots */
struct DIMITRI : GUARD
{
    int fleeCount;          /* +0x1010 > 0: ResumeBhvguard flees from the hero (inferred) */
    int unknown1014[3];
    virtual void PostLoad() __asm__("PostDimitriLoad");   /* slot 15 */
    virtual CLASSINFO *GetClassInfo() __asm__("GetDimitriClassDesc");   /* slot 6 */
    virtual void LoadFromBrx(CBinaryInputStream *pbis) __asm__("LoadDimitriFromBrx");   /* slot 14 */
};
typedef char assert_DIMITRI_size[sizeof(DIMITRI) == 0x1020 ? 1 : -1];

/* 180 BHV180 (0x40): DIMITRI flee; vtable 0x2CDDC0, 14 slots */
struct BHV180 : BHV
{
    char pad30[0x38 - 0x30];
    LO *target;             /* +0x38 [S] what Dimitri escapes from (SetBhvguardMode: the mode's object) */
    char pad3C[0x40 - 0x3C];
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
    char pad30[0x60 - 0x30];
    virtual void Start() __asm__("StartBhvDimitriAttack");   /* slot 8 */
    virtual CLASSINFO *GetClassInfo() __asm__("GetBhv181ClassDesc");   /* slot 6 */
    virtual void OnPush() __asm__("InitBhvDimitriAttack");   /* slot 7 */
    virtual void Close() __asm__("CloseBhvDimitriAttack");   /* slot 9 */
    virtual void Update() __asm__("UpdateBhvDimitriAttack");   /* slot 10 */
};
typedef char assert_BHV181_size[sizeof(BHV181) == 0x60 ? 1 : -1];

/* 117 CID117 (0x70): stimulus template (radius, height, ramp); vtable 0x2CE2F8, 48 slots */
struct CID117 : LO
{
    char pad50[0x70 - 0x50];
    virtual void Init() __asm__("InitNoiseType");   /* slot 7 */
    virtual CLASSINFO *GetClassInfo() __asm__("GetCid117ClassDesc");   /* slot 6 */
};
typedef char assert_CID117_size[sizeof(CID117) == 0x70 ? 1 : -1];

/* 147 CID147 (0x60): stimulus (noise) instance; vtable 0x2CE2B8, 6 slots; defined between CID117 and CID47: noise.cpp emits CID47, CID147, CID117 */
/* CID147 is the NOISE record (noise.cpp's view NOISE; STEPGUARD::heldNoise): a refcounted noise event emitted from a
   CID117 noise type and linked on g_sw.dlStimulus. */
struct CID147 : BASIC
{
    int refCount;           /* +0x10 */
    struct CID117 *type;    /* +0x14 the noise type it was emitted from */
    int unknown18[2];
    qword position;         /* +0x20 where it is (a held noise is moved with its maker: UpdateBhvSleep) */
    qword startPosition;    /* +0x30 */
    int loudness;           /* +0x40 */
    int unknown44;
    int serial;             /* +0x48 */
    float startTime;        /* +0x4C */
    int unknown50;
    int noiseKind;          /* +0x54 what made it (3: a thrown guard yelling, UpdateBhvThrown) */
    CID147 *next;           /* +0x58 link of DL g_sw.dlStimulus */
    int unknown5C;
    virtual void AddRef() __asm__("RetainNoise");   /* slot 1; key function (noise.cpp) */
    virtual CLASSINFO *GetClassInfo() __asm__("GetCid147ClassDesc");   /* slot 6 */
    virtual void Release() __asm__("ReleaseNoise");   /* slot 2 */
};
typedef char assert_CID147_size[sizeof(CID147) == 0x60 ? 1 : -1];
typedef char assert_CID147_position[SLY2_OFFSETOF(CID147, position) == 0x20 ? 1 : -1];
typedef char assert_CID147_noiseKind[SLY2_OFFSETOF(CID147, noiseKind) == 0x54 ? 1 : -1];

/* 47 CID47 (0x110): stimulus listener; vtable 0x2CE118, 50 slots */
struct CID47 : XFM
{
    char padE0[0x110 - 0xE0];
    virtual void AddHierarchy() __asm__("AddListenerHierarchy");   /* slot 11 */
    virtual CLASSINFO *GetClassInfo() __asm__("GetCid47ClassDesc");   /* slot 6 */
    virtual void Init() __asm__("InitListener");   /* slot 7 */
    virtual void RemoveHierarchy() __asm__("RemoveListenerHierarchy");   /* slot 12 */
    virtual void Update() __asm__("UpdateListener");   /* slot 16 */
    virtual void Clone(LO *ploBase) __asm__("CloneListener");   /* slot 41 */
};
typedef char assert_CID47_size[sizeof(CID47) == 0x110 ? 1 : -1];

/* 31 EXPLG (0xF0); vtable 0x2CE828, 51 slots */
struct EXPLG : EXPL
{
    char padE0[0xF0 - 0xE0];
    virtual int GetChildren(LO ***paplo) __asm__("CploExplgChildren");   /* slot 28 */
    virtual CLASSINFO *GetClassInfo() __asm__("GetExplgClassDesc");   /* slot 6 */
    virtual void LoadFromBrx(CBinaryInputStream *pbis) __asm__("LoadExplgFromBrx");   /* slot 14 */
    virtual void Clone(LO *ploBase) __asm__("CloneExplg");   /* slot 41 */
    virtual void ExplodeExplso() __asm__("ExplodeExplgExplso");   /* slot 51 */
};
typedef char assert_EXPLG_size[sizeof(EXPLG) == 0xF0 ? 1 : -1];

/* 33 EXPLS (0x110); vtable 0x2CE4D0, 52 slots */
struct EXPLS : EXPLO
{
    char padF0[0x110 - 0xF0];
    virtual void HandleMessage(MSGID msgid, void *pv) __asm__("HandleExplsMessage");   /* slot 5 */
    virtual CLASSINFO *GetClassInfo() __asm__("GetExplsClassDesc");   /* slot 6 */
    virtual void ExplodeExplso() __asm__("ExplodeExplsExplso");   /* slot 51 */
    virtual void FireExplso() __asm__("FireExplsExplso");   /* slot 52 */
};
typedef char assert_EXPLS_size[sizeof(EXPLS) == 0x110 ? 1 : -1];

/* 131 WIDGET131 (0x140): WIDGET: episode menu; vtable 0x2CEEB8, 50 slots */
struct WIDGET131 : WIDGET
{
    char pad110[0x140 - 0x110];
    virtual void Init() __asm__("InitEpisodeMenu");   /* slot 7 */
    virtual CLASSINFO *GetClassInfo() __asm__("GetWidget131ClassDesc");   /* slot 6 */
    virtual void PostLoad() __asm__("PostEpisodeMenuLoad");   /* slot 15 */
    virtual void Update() __asm__("UpdateEpisodeMenu");   /* slot 16 */
};
typedef char assert_WIDGET131_size[sizeof(WIDGET131) == 0x140 ? 1 : -1];

/* 116 CID116 (0x70): screen tint / fill (render kinds 14 and 1); vtable 0x2CF058, 48 slots */
struct CID116 : LO
{
    char pad50[0x70 - 0x50];
    virtual void Init() __asm__("InitScreenFade");   /* slot 7 */
    virtual CLASSINFO *GetClassInfo() __asm__("GetCid116ClassDesc");   /* slot 6 */
    virtual void Update() __asm__("UpdateScreenFade");   /* slot 16 */
    virtual void RenderSelf() __asm__("RenderScreenFade");   /* slot 19 */
};
typedef char assert_CID116_size[sizeof(CID116) == 0x70 ? 1 : -1];

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
    STIMULUS *watched;      /* +0x44 the stimulus being investigated (SetBhvSearchWatched; counted: slots 1/2) */
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

/* 119 CID119 (0xB0): off-screen guard spawner; vtable 0x2CFA90, 48 slots */
struct CID119 : LO
{
    char pad50[0xB0 - 0x50];
    virtual void Init() __asm__("InitSpawner");   /* slot 7 */
    virtual CLASSINFO *GetClassInfo() __asm__("GetCid119ClassDesc");   /* slot 6 */
    virtual void RemoveHierarchy() __asm__("RemoveSpawnerHierarchy");   /* slot 12 */
    virtual void LoadFromBrx(CBinaryInputStream *pbis) __asm__("LoadSpawnerFromBrx");   /* slot 14 */
    virtual void PostLoad() __asm__("PostSpawnerLoad");   /* slot 15 */
    virtual void Update() __asm__("UpdateSpawner");   /* slot 16 */
};
typedef char assert_CID119_size[sizeof(CID119) == 0xB0 ? 1 : -1];

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
    virtual void GetGroundProbeFlags() __asm__("GetBhvanimGroundProbeFlags");   /* slot 12 */
    virtual void HandleBehaviorMessage() __asm__("HandleBhvanimMessage");   /* slot 13 */
};
typedef char assert_BHVANIM_size[sizeof(BHVANIM) == 0x50 ? 1 : -1];

/* 158 BHVIDLE (0x50): idle leaf (flag 2); vtable 0x2D0A30, 14 slots */
struct BHVIDLE : BHVANIM
{
    virtual void OnPush() __asm__("InitBhvidle");   /* slot 7 */
    virtual CLASSINFO *GetClassInfo() __asm__("GetBhvidleClassDesc");   /* slot 6 */
    virtual void Start() __asm__("StartBhvidle");   /* slot 8 */
    virtual void Close() __asm__("CloseBhvidle");   /* slot 9 */
    virtual void HandleBehaviorMessage() __asm__("HandleBhvidleMessage");   /* slot 13 */
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
    virtual void HandleBehaviorMessage() __asm__("HandleBhvstrikeMessage");   /* slot 13 */
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
    LO *blocker;            /* +0x54 the object blocking the path */
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
    virtual void UpdateSteering() __asm__("UpdateBhvwalkgoalPoctl");   /* slot 14 */
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
    virtual void UpdateSteering() __asm__("SteerBhvInteract");   /* slot 14 */
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
    virtual void UpdateSteering() __asm__("SteerBhvInteract2");   /* slot 14 */
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
    virtual void UpdateSteering() __asm__("SteerBhvInteract3");   /* slot 14 */
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
    virtual void HandleBehaviorMessage() __asm__("HandleBhvattackMessage");   /* slot 13 */
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
    virtual void HandleBehaviorMessage() __asm__("HandleBhvguardMessage");   /* slot 13 */
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
    virtual void GetGroundProbeFlags() __asm__("GetBhvjuggledGroundProbeFlags");   /* slot 12 */
    virtual void HandleBehaviorMessage() __asm__("HandleBhvjuggledMessage");   /* slot 13 */
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
    virtual void HandleBehaviorMessage() __asm__("HandleBhvshootMessage");   /* slot 13 */
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
    PATHLINK *firstLink;    /* +0xB8 the route's (single) link (FindPathlinkRoute) */
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
    int unknown34;
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

/* 71 HACK (0x1C0): hacking minigame base (damageable PO); vtable 0x2D1878, 83 slots */
struct HACK : PO
{
    char pad180[0x1C0 - 0x180];
    virtual void RemoveHierarchy() __asm__("RemoveHackHierarchy");   /* slot 12 */
    virtual CLASSINFO *GetClassInfo() __asm__("GetHackClassDesc");   /* slot 6 */
    virtual void Init() __asm__("InitHack");   /* slot 7 */
    virtual void PostLoad() __asm__("PostHackLoad");   /* slot 15 */
    virtual void OnProxyAcquire(LO *plo) __asm__("ResetHackHealth");   /* slot 26 */
    virtual void AdjustNewXp(XP *pxp, int ixpd) __asm__("AdjustHackNewXp");   /* slot 55 */
    virtual int TakeDamage(ZPR *pzpr) __asm__("TakeHackDamage");   /* slot 75 */
    virtual void Kill() __asm__("KillHack");   /* slot 82 */
    virtual void Reset() __asm__("ResetHack");   /* slot 83 */
};
typedef char assert_HACK_size[sizeof(HACK) == 0x1C0 ? 1 : -1];

/* 72 HACKSHIP (0x2B0); vtable 0x2D15D0, 83 slots */
struct HACKSHIP : HACK
{
    char pad1C0[0x2A4 - 0x1C0];
    int nUnk2a4;
    char pad2A8[0x2B0 - 0x2A8];
    HACKSHIP();
    virtual void RemoveHierarchy() __asm__("RemoveHackshipHierarchy");   /* slot 12 */
    virtual CLASSINFO *GetClassInfo() __asm__("GetHackshipClassDesc");   /* slot 6 */
    virtual void Init() __asm__("InitHackship");   /* slot 7 */
    virtual void PostLoad() __asm__("PostHackshipLoad");   /* slot 15 */
    virtual void Update() __asm__("UpdateHackship");   /* slot 16 */
    virtual void PresetAccel() __asm__("PresetHackshipAccel");   /* slot 70 */
    virtual int TakeDamage(ZPR *pzpr) __asm__("TakeHackshipDamage");   /* slot 75 */
    virtual void OnActive(int fActive) __asm__("OnHackshipActive");   /* slot 78 */
    virtual void Reset() __asm__("ResetHackship");   /* slot 83 */
};
typedef char assert_HACKSHIP_size[sizeof(HACKSHIP) == 0x2B0 ? 1 : -1];
/* The constructor class_registry inlines into its factory. g++ 2.95 emits a class's inline member functions in
   its key TU (as global functions the ELF does not have), so the key TU defines SLY2_KEY_TU_HACKSHIP and sees
   only the declaration. */
#ifndef SLY2_KEY_TU_HACKSHIP
// HACKSHIP constructor: clears nUnk2a4.
inline HACKSHIP::HACKSHIP() { nUnk2a4 = 0; }
#endif

/* 74 HACKBULLET (0x1E0); vtable 0x2D1328, 83 slots */
struct HACKBULLET : HACK
{
    char pad1C0[0x1E0 - 0x1C0];
    virtual void Init() __asm__("InitHackbullet");   /* slot 7 */
    virtual CLASSINFO *GetClassInfo() __asm__("GetHackbulletClassDesc");   /* slot 6 */
    virtual void Update() __asm__("UpdateHackbullet");   /* slot 16 */
    virtual void AdjustNewXp(XP *pxp, int ixpd) __asm__("AdjustHackbulletNewXp");   /* slot 55 */
    virtual int ShouldIgnoreIntersection(SO *psoOther) __asm__("ShouldHackbulletIgnoreIntersection");   /* slot 61 */
};
typedef char assert_HACKBULLET_size[sizeof(HACKBULLET) == 0x1E0 ? 1 : -1];

/* 73 HACKENEMY (0x390); vtable 0x2D1080, 83 slots */
struct HACKENEMY : HACK
{
    char pad1C0[0x390 - 0x1C0];
    virtual void Reset() __asm__("ResetHackenemy");   /* slot 83 */
    virtual CLASSINFO *GetClassInfo() __asm__("GetHackenemyClassDesc");   /* slot 6 */
    virtual void HandleMessage(MSGID msgid, void *pv) __asm__("HandleHackenemyMessage");   /* slot 5 */
    virtual void Init() __asm__("InitHackenemy");   /* slot 7 */
    virtual void RemoveHierarchy() __asm__("RemoveHackenemyHierarchy");   /* slot 12 */
    virtual void PostLoad() __asm__("PostHackenemyLoad");   /* slot 15 */
    virtual void Update() __asm__("UpdateHackenemy");   /* slot 16 */
    virtual void OnProxyAcquire(LO *plo) __asm__("ResetHackenemyState");   /* slot 26 */
    virtual void AdjustNewXp(XP *pxp, int ixpd) __asm__("AdjustHackenemyNewXp");   /* slot 55 */
    virtual void PresetAccel() __asm__("PresetHackenemyAccel");   /* slot 70 */
    virtual int TakeDamage(ZPR *pzpr) __asm__("TakeHackenemyDamage");   /* slot 75 */
};
typedef char assert_HACKENEMY_size[sizeof(HACKENEMY) == 0x390 ? 1 : -1];

/* 75 HACKLOCK (0x250); vtable 0x2D0DD8, 83 slots */
struct HACKLOCK : HACK
{
    char pad1C0[0x250 - 0x1C0];
    virtual void Init() __asm__("InitHacklock");   /* slot 7 */
    virtual CLASSINFO *GetClassInfo() __asm__("GetHacklockClassDesc");   /* slot 6 */
    virtual void PostLoad() __asm__("PostHacklockLoad");   /* slot 15 */
    virtual void Update() __asm__("UpdateHacklock");   /* slot 16 */
    virtual void Reset() __asm__("ResetHacklock");   /* slot 83 */
};
typedef char assert_HACKLOCK_size[sizeof(HACKLOCK) == 0x250 ? 1 : -1];

/* 76 HACKWALL (0x2A0); vtable 0x2D0B30, 83 slots */
struct HACKWALL : HACK
{
    char pad1C0[0x2A0 - 0x1C0];
    virtual void Init() __asm__("InitHackwall");   /* slot 7 */
    virtual CLASSINFO *GetClassInfo() __asm__("GetHackwallClassDesc");   /* slot 6 */
    virtual void PostLoad() __asm__("PostHackwallLoad");   /* slot 15 */
    virtual void Update() __asm__("UpdateHackwall");   /* slot 16 */
    virtual void RenderSelf() __asm__("RenderHackwallSelf");   /* slot 19 */
    virtual void UpdateBounds() __asm__("UpdateHackwallTreeBounds");   /* slot 49 */
    virtual void AdjustNewXp(XP *pxp, int ixpd) __asm__("AdjustHackwallNewXp");   /* slot 55 */
    virtual void AddCustomXps(SOPART *ppart, int n2, int n3, void *pvGeom, XP **ppxpFirst) __asm__("AddHackwallXps");   /* slot 60 */
    virtual int GetExtraPartCount() __asm__("GetHackwallExtraPartCount");   /* slot 63 */
    virtual int TakeDamage(ZPR *pzpr) __asm__("TakeHackwallDamage");   /* slot 75 */
    virtual void Kill() __asm__("KillHackwall");   /* slot 82 */
    virtual void Reset() __asm__("ResetHackwall");   /* slot 83 */
};
typedef char assert_HACKWALL_size[sizeof(HACKWALL) == 0x2A0 ? 1 : -1];

/* 12 CID12 (0x1C0): wall grabspot (wall press); vtable 0x2D2738, 50 slots */
struct CID12 : SHAPE, GRABSPOT
{
    char pad1B4[0x1C0 - 0x1B4];
    virtual void OnAdd() __asm__("OnWallGrabspotAdd");   /* slot 23 */
    virtual CLASSINFO *GetClassInfo() __asm__("GetCid12ClassDesc");   /* slot 6 */
    virtual void Init() __asm__("InitWallGrabspot");   /* slot 7 */
    virtual void PostLoad() __asm__("PostWallGrabspotLoad");   /* slot 15 */
    virtual void OnRemove() __asm__("OnWallGrabspotRemove");   /* slot 22 */
    virtual void Clone(LO *ploBase) __asm__("CloneWallGrabspot");   /* slot 41 */
    virtual int GetGrabRange() __asm__("GetWallGrabspotWrapRange");   /* GRABSPOT slot 1 (delta -400) */
    virtual void GetClosestParam() __asm__("GetWallGrabspotClosestParam");   /* GRABSPOT slot 2 (delta -400) */
    virtual void GetGrabPos() __asm__("GetWallGrabspotGrabPosition");   /* GRABSPOT slot 3 (delta -400) */
    virtual void GetSparklePos() __asm__("GetWallGrabspotSparklePosition");   /* GRABSPOT slot 5 (delta -400) */
    virtual void GetOwner() __asm__("func_002599F0__FP5CID12");   /* GRABSPOT slot 6 (delta -400) */
};
typedef char assert_CID12_size[sizeof(CID12) == 0x1C0 ? 1 : -1];

/* 34 CID34 (0x110): pole (GRABSPOT); vtable 0x2D2558, 50 slots */
struct CID34 : XFM, GRABSPOT
{
    char pad104[0x110 - 0x104];
    virtual void OnAdd() __asm__("OnPoleGrabspotAdd");   /* slot 23 */
    virtual CLASSINFO *GetClassInfo() __asm__("GetCid34ClassDesc");   /* slot 6 */
    virtual void Init() __asm__("InitPoleGrabspot");   /* slot 7 */
    virtual void PostLoad() __asm__("PostPoleGrabspotLoad");   /* slot 15 */
    virtual void OnRemove() __asm__("OnPoleGrabspotRemove");   /* slot 22 */
    virtual void Clone(LO *ploBase) __asm__("ClonePoleGrabspot");   /* slot 41 */
    virtual int GetGrabRange() __asm__("GetPoleGrabspotWrapRange");   /* GRABSPOT slot 1 (delta -224) */
    virtual void GetClosestParam() __asm__("GetPoleGrabspotClosestParam");   /* GRABSPOT slot 2 (delta -224) */
    virtual void GetGrabPos() __asm__("GetPoleGrabspotGrabPosition");   /* GRABSPOT slot 3 (delta -224) */
    virtual void GetSparklePos() __asm__("GetPoleGrabspotSparklePosition");   /* GRABSPOT slot 5 (delta -224) */
    virtual void GetOwner() __asm__("func_00259A00__FP5CID34");   /* GRABSPOT slot 6 (delta -224) */
};
typedef char assert_CID34_size[sizeof(CID34) == 0x110 ? 1 : -1];

/* 38 CID38 (0x110); vtable 0x2D2378, 50 slots */
struct CID38 : XFM, GRABSPOT
{
    char pad104[0x110 - 0x104];
    virtual void OnAdd() __asm__("OnHoldGrabspotAdd");   /* slot 23 */
    virtual CLASSINFO *GetClassInfo() __asm__("GetCid38ClassDesc");   /* slot 6 */
    virtual void Init() __asm__("InitHoldGrabspot");   /* slot 7 */
    virtual void PostLoad() __asm__("PostHoldGrabspotLoad");   /* slot 15 */
    virtual void OnRemove() __asm__("OnHoldGrabspotRemove");   /* slot 22 */
    virtual void Clone(LO *ploBase) __asm__("CloneHoldGrabspot");   /* slot 41 */
    virtual void GetGrabPos() __asm__("GetHoldGrabspotGrabPosition");   /* GRABSPOT slot 3 (delta -224) */
    virtual void GetOwner() __asm__("func_00259A10__FP5CID38");   /* GRABSPOT slot 6 (delta -224) */
};
typedef char assert_CID38_size[sizeof(CID38) == 0x110 ? 1 : -1];

/* 88 BARREL (0x1A0): BARREL / carryable (REVIEW, medium); vtable 0x2D20D0, 75 slots */
struct BARREL : SO, GRABSPOT
{
    char pad174[0x1A0 - 0x174];
    virtual void OnAdd() __asm__("OnBarrelAdd");   /* slot 23 */
    virtual CLASSINFO *GetClassInfo() __asm__("GetBarrelClassDesc");   /* slot 6 */
    virtual void Init() __asm__("InitBarrel");   /* slot 7 */
    virtual void OnRemove() __asm__("OnBarrelRemove");   /* slot 22 */
    virtual void OnProxyAcquire(LO *plo) __asm__("ResetBarrel");   /* slot 26 */
    virtual void Clone(LO *ploBase) __asm__("CloneBarrel");   /* slot 41 */
    virtual int ShouldIgnoreIntersection(SO *psoOther) __asm__("ShouldBarrelIgnoreIntersection");   /* slot 61 */
    virtual void PresetAccel() __asm__("PresetBarrelAccel");   /* slot 70 */
    virtual int TakeDamage(ZPR *pzpr) __asm__("HandleBarrelZap");   /* slot 75 */
    virtual void GetOwner() __asm__("func_00259A20__FP6BARREL");   /* GRABSPOT slot 6 (delta -336) */
};
typedef char assert_BARREL_size[sizeof(BARREL) == 0x1A0 ? 1 : -1];

/* 35 CID35 (0x180): GRABSPOT XFM; mission trigger spot (ApplyTaskStates); vtable 0x2D1EE8, 51 slots */
struct CID35 : XFM, GRABSPOT
{
    char pad104[0x180 - 0x104];
    virtual void OnAdd() __asm__("OnTriggerGrabspotAdd");   /* slot 23 */
    virtual CLASSINFO *GetClassInfo() __asm__("GetCid35ClassDesc");   /* slot 6 */
    virtual void Init() __asm__("InitTriggerGrabspot");   /* slot 7 */
    virtual void PostLoad() __asm__("PostTriggerGrabspotLoad");   /* slot 15 */
    virtual void OnRemove() __asm__("OnTriggerGrabspotRemove");   /* slot 22 */
    virtual void GetGrabPos() __asm__("GetTriggerGrabspotGrabPosition");   /* GRABSPOT slot 3 (delta -224) */
    virtual void AdjustScore() __asm__("AdjustTriggerGrabspotScore");   /* GRABSPOT slot 4 (delta -224) */
    virtual void GetOwner() __asm__("func_00259A30__FP5CID35");   /* GRABSPOT slot 6 (delta -224) */
    virtual void OnTriggered() __asm__("FireTriggerGrabspot");   /* slot 51 */
};
typedef char assert_CID35_size[sizeof(CID35) == 0x180 ? 1 : -1];

/* 37 CID37 (0x1A0): booty container; vtable 0x2CB9F8, 51 slots */
struct CID37 : CID35
{
    char pad180[0x1A0 - 0x180];
    virtual void PostLoad() __asm__("PostBootyContainerLoad");   /* slot 15 */
    virtual CLASSINFO *GetClassInfo() __asm__("GetCid37ClassDesc");   /* slot 6 */
    virtual void Init() __asm__("InitBootyContainer");   /* slot 7 */
    virtual void OnAdd() __asm__("OnBootyContainerAdd");   /* slot 23 */
    virtual void OnTriggered() __asm__("OnBootyContainerTouched");   /* slot 51 */
};
typedef char assert_CID37_size[sizeof(CID37) == 0x1A0 ? 1 : -1];

/* 36 CID36 (0x1A0): carry grabspot (child of CID35); vtable 0x2D1D00, 51 slots */
struct CID36 : CID35
{
    char pad180[0x1A0 - 0x180];
    virtual void PostLoad() __asm__("PostCarryGrabspotLoad");   /* slot 15 */
    virtual CLASSINFO *GetClassInfo() __asm__("GetCid36ClassDesc");   /* slot 6 */
    virtual void Init() __asm__("InitCarryGrabspot");   /* slot 7 */
    virtual void OnTriggered() __asm__("OnTriggerGrabspotGrabbed");   /* slot 51 */
};
typedef char assert_CID36_size[sizeof(CID36) == 0x1A0 ? 1 : -1];

/* 27 CID27 (0x160): table grabspot (VOL + GRABSPOT; hide under); vtable 0x2D1B60, 50 slots */
struct CID27 : VOL, GRABSPOT
{
    char pad154[0x160 - 0x154];
    virtual void OnAdd() __asm__("OnTableGrabspotAdd");   /* slot 23 */
    virtual CLASSINFO *GetClassInfo() __asm__("GetCid27ClassDesc");   /* slot 6 */
    virtual void Init() __asm__("InitTableGrabspot");   /* slot 7 */
    virtual void PostLoad() __asm__("PostTableGrabspotLoad");   /* slot 15 */
    virtual void OnRemove() __asm__("OnTableGrabspotRemove");   /* slot 22 */
    virtual void GetGrabPos() __asm__("GetTableGrabspotGrabPosition");   /* GRABSPOT slot 3 (delta -304) */
    virtual void GetSparklePos() __asm__("GetTableGrabspotSparklePosition");   /* GRABSPOT slot 5 (delta -304) */
    virtual void GetOwner() __asm__("func_00259A40__FP5CID27");   /* GRABSPOT slot 6 (delta -304) */
};
typedef char assert_CID27_size[sizeof(CID27) == 0x160 ? 1 : -1];

/* 19 HND (0x140): hook; REVIEW proposes HND; vtable 0x2D2978, 50 slots */
struct HND : TARGET, GRABSPOT
{
    char pad124[0x140 - 0x124];
    virtual void Init() __asm__("InitHnd");   /* slot 7 */
    virtual CLASSINFO *GetClassInfo() __asm__("GetHndClassDesc");   /* slot 6 */
    virtual void GetGrabPos() __asm__("GetHndGrabPosition");   /* GRABSPOT slot 3 (delta -256) */
    virtual void GetOwner() __asm__("func_00259A70__FP3HND");   /* GRABSPOT slot 6 (delta -256) */
};
typedef char assert_HND_size[sizeof(HND) == 0x140 ? 1 : -1];

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
    char pad100[0x120 - 0x100];
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
    char padE0[0xF0 - 0xE0];
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
    qword highlightHsv;     /* +0xF0 [S] */
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
    float sNear;            /* +0x164 [S] (light.cpp also copies the pair as two ints) */
    float sFar;             /* +0x168 [S] */
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
    virtual void RotateToMat(MATRIX4 *pmatWorld) __asm__("RotateLightToMat");   /* slot 30 */
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
    char pad210[0x270 - 0x210];
    virtual CLASSINFO *GetClassInfo() __asm__("GetMissileClassDesc");   /* slot 6 */
    virtual void Init() __asm__("InitMissile__FP7MISSILE");   /* slot 7 */
    virtual void RemoveHierarchy() __asm__("OnMissileRemove__FP7MISSILE");   /* slot 12 */
    virtual void LoadFromBrx(CBinaryInputStream *pbis) __asm__("LoadMissileFromBrx__FP7MISSILEP18CBinaryInputStream");   /* slot 14 */
    virtual int ShouldIgnoreIntersection(SO *psoOther) __asm__("ShouldMissileIgnoreIntersection__FP7MISSILEP2SO");   /* slot 61 */
    virtual void ApplyThrow() __asm__("ApplyMissileThrow");   /* slot 77 */
    virtual void Fire() __asm__("FireMissile");   /* slot 78 */
};
typedef char assert_MISSILE_size[sizeof(MISSILE) == 0x270 ? 1 : -1];

/* 81 ACCMISS (0x280): ACCMISS = Bentley crossbow dart; vtable 0x2D3D20, 78 slots */
struct ACCMISS : MISSILE
{
    char pad270[0x280 - 0x270];
    virtual CLASSINFO *GetClassInfo() __asm__("GetAccmissClassDesc");   /* slot 6 */
    virtual void Init() __asm__("InitAccmiss__FP7ACCMISS");   /* slot 7 */
    virtual void PresetAccel() __asm__("PresetAccmissAccel");   /* slot 70 */
};
typedef char assert_ACCMISS_size[sizeof(ACCMISS) == 0x280 ? 1 : -1];

/* 82 CID82 (0x290); vtable 0x2D3AA0, 78 slots */
struct CID82 : MISSILE
{
    char pad270[0x290 - 0x270];
    virtual CLASSINFO *GetClassInfo() __asm__("GetCid82ClassDesc");   /* slot 6 */
    virtual void Init() __asm__("InitSplmiss");   /* slot 7 */
    virtual void PresetAccel() __asm__("PresetSplmissAccel");   /* slot 70 */
    virtual void Fire() __asm__("FireSplmiss");   /* slot 78 */
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
    char pad30[0x38 - 0x30];
    int useChaseBehavior;   /* +0x38 [S] copied from NEELA::useChaseBehavior (SetBhvguardMode) */
    char pad3C[0x40 - 0x3C];
    virtual void Start() __asm__("StartBhvNeelaChase");   /* slot 8 */
    virtual CLASSINFO *GetClassInfo() __asm__("GetBhv174ClassDesc");   /* slot 6 */
    virtual void OnPush() __asm__("InitBhvNeelaChase");   /* slot 7 */
    virtual void Close() __asm__("CloseBhvNeelaChase");   /* slot 9 */
    virtual void Update() __asm__("UpdateBhvNeelaChase");   /* slot 10 */
};
typedef char assert_BHV174_size[sizeof(BHV174) == 0x40 ? 1 : -1];

/* 60 NEELA (0x10A0); vtable 0x2D4820, 89 slots */
struct NEELA : STEPGUARD
{
    char pad1010[0x108C - 0x1010];
    int useChaseBehavior;   /* +0x108C her option: chase (BHV174) instead of combat (ShouldBhvguardUseNeelaBehavior) */
    char pad1090[0x10A0 - 0x1090];
    virtual void Init() __asm__("InitNeela");   /* slot 7 */
    virtual CLASSINFO *GetClassInfo() __asm__("GetNeelaClassDesc");   /* slot 6 */
    virtual void PostLoad() __asm__("PostNeelaLoad");   /* slot 15 */
    virtual void Update() __asm__("UpdateNeela");   /* slot 16 */
    virtual void OnProxyAcquire(LO *plo) __asm__("ResetNeelaToHome");   /* slot 26 */
};
typedef char assert_NEELA_size[sizeof(NEELA) == 0x10A0 ? 1 : -1];

/* 98 CID98 (0x180): breakable props (chairs, barrels, crates, containers); vtable 0x2D4B78, 75 slots */
struct CID98 : SO
{
    char pad150[0x180 - 0x150];
    virtual void AdjustXps() __asm__("HandleBreakableContacts");   /* slot 58 */
    virtual CLASSINFO *GetClassInfo() __asm__("GetCid98ClassDesc");   /* slot 6 */
    virtual void Init() __asm__("InitBreakable");   /* slot 7 */
    virtual void LoadFromBrx(CBinaryInputStream *pbis) __asm__("LoadBreakableFromBrx");   /* slot 14 */
    virtual void OnRemove() __asm__("OnBreakableRemove");   /* slot 22 */
    virtual void OnAdd() __asm__("OnBreakableAdd");   /* slot 23 */
    virtual void OnProxyAcquire(LO *plo) __asm__("ResetBreakable");   /* slot 26 */
    virtual int TakeDamage(ZPR *pzpr) __asm__("AcceptBreakableZap");   /* slot 75 */
};
typedef char assert_CID98_size[sizeof(CID98) == 0x180 ? 1 : -1];

/* 14 CID14 (0x1F0): convex polygon SHAPE / pathzone obstacle; vtable 0x2D4DE0, 50 slots */
struct CID14 : SHAPE
{
    char pad190[0x1F0 - 0x190];
    virtual void Init() __asm__("InitCid14");   /* slot 7 */
    virtual CLASSINFO *GetClassInfo() __asm__("GetCid14ClassDesc");   /* slot 6 */
    virtual void AddHierarchy() __asm__("AddCid14Hierarchy");   /* slot 11 */
    virtual void RemoveHierarchy() __asm__("RemoveCid14Hierarchy");   /* slot 12 */
    virtual void LoadFromBrx(CBinaryInputStream *pbis) __asm__("LoadCid14FromBrx");   /* slot 14 */
    virtual void PostLoad() __asm__("PostCid14Load");   /* slot 15 */
    virtual void Clone(LO *ploBase) __asm__("CloneCid14");   /* slot 41 */
};
typedef char assert_CID14_size[sizeof(CID14) == 0x1F0 ? 1 : -1];

/* 45 CID45 (0x130): render-to-texture owner (item preview?); vtable 0x2D4F80, 50 slots */
struct CID45 : XFM
{
    char padE0[0x130 - 0xE0];
    virtual void Init() __asm__("InitRtcam");   /* slot 7 */
    virtual CLASSINFO *GetClassInfo() __asm__("GetCid45ClassDesc");   /* slot 6 */
    virtual void LoadFromBrx(CBinaryInputStream *pbis) __asm__("LoadRtcamFromBrx");   /* slot 14 */
    virtual void RenderSelf() __asm__("RenderRtcam");   /* slot 19 */
};
typedef char assert_CID45_size[sizeof(CID45) == 0x130 ? 1 : -1];

/* 17 PATHZONE (0x130): *_pz; vtable 0x2D52B0, 50 slots */
struct PATHZONE : XFM
{
    char padE0[0x130 - 0xE0];
    virtual void Init() __asm__("InitPathzone");   /* slot 7 */
    virtual CLASSINFO *GetClassInfo() __asm__("GetPathzoneClassDesc");   /* slot 6 */
    virtual void LoadFromBrx(CBinaryInputStream *pbis) __asm__("LoadPathzoneFromBrx");   /* slot 14 */
};
typedef char assert_PATHZONE_size[sizeof(PATHZONE) == 0x130 ? 1 : -1];

/* 124 PATHLINK (0x160): BHVPATHLINK walks it; REVIEW; vtable 0x2D5120, 48 slots */
struct PATHLINK : LO
{
    int numEnds;            /* +0x50 the number of link segments (BHVPATHLINK walks 0..numEnds-1) */
    int unknown54[3];
    PATHLINKEND aend[6];
    char pad120[0x160 - 0x120];
    virtual void Init() __asm__("InitPathlink");   /* slot 7 */
    virtual CLASSINFO *GetClassInfo() __asm__("GetPathlinkClassDesc");   /* slot 6 */
    virtual void AddHierarchy() __asm__("AddPathlinkHierarchy");   /* slot 11 */
    virtual void RemoveHierarchy() __asm__("RemovePathlinkHierarchy");   /* slot 12 */
    virtual void PostLoad() __asm__("PostPathlinkLoad");   /* slot 15 */
};
typedef char assert_PATHLINK_size[sizeof(PATHLINK) == 0x160 ? 1 : -1];

/* 99 CID99 (0x170): pickpocket dispenser / guard pouch; vtable 0x2D5450, 75 slots */
struct CID99 : SO
{
    char pad150[0x170 - 0x150];
    virtual void PostLoad() __asm__("PostLootPouchLoad");   /* slot 15 */
    virtual CLASSINFO *GetClassInfo() __asm__("GetCid99ClassDesc");   /* slot 6 */
    virtual void RenderSelf() __asm__("RenderLootPouchSelf");   /* slot 19 */
};
typedef char assert_CID99_size[sizeof(CID99) == 0x170 ? 1 : -1];

/* 11 PIPE (0x1E0): lamp posts; JTPIPE; vtable 0x2D56F8, 50 slots */
struct PIPE : SHAPE, GRABSPOT
{
    char pad1B4[0x1E0 - 0x1B4];
    virtual void OnAdd() __asm__("OnPipeAdd");   /* slot 23 */
    virtual CLASSINFO *GetClassInfo() __asm__("GetPipeClassDesc");   /* slot 6 */
    virtual void Init() __asm__("InitPipe");   /* slot 7 */
    virtual void PostLoad() __asm__("PostPipeLoad");   /* slot 15 */
    virtual void OnRemove() __asm__("OnPipeRemove");   /* slot 22 */
    virtual void Clone(LO *ploBase) __asm__("ClonePipe");   /* slot 41 */
    virtual int GetGrabRange() __asm__("GetPipeWrapRange");   /* GRABSPOT slot 1 (delta -400) */
    virtual void GetClosestParam() __asm__("GetPipeClosestParam");   /* GRABSPOT slot 2 (delta -400) */
    virtual void GetGrabPos() __asm__("GetPipeGrabPosition");   /* GRABSPOT slot 3 (delta -400) */
    virtual void GetSparklePos() __asm__("GetPipeSparklePosition");   /* GRABSPOT slot 5 (delta -400) */
    virtual void GetOwner() __asm__("func_00259AF8__FP4PIPE");   /* GRABSPOT slot 6 (delta -400) */
};
typedef char assert_PIPE_size[sizeof(PIPE) == 0x1E0 ? 1 : -1];

/* 91 COIN (0x240): COIN pool; vtable 0x2D6928, 76 slots */
struct COIN : DPRIZE
{
    char pad230[0x240 - 0x230];
    virtual void Update() __asm__("UpdateCoin");   /* slot 16 */
    virtual CLASSINFO *GetClassInfo() __asm__("GetCoinClassDesc");   /* slot 6 */
    virtual void Init() __asm__("InitCoin");   /* slot 7 */
    virtual void SetState() __asm__("SetCoinDprizes");   /* slot 76 */
};
typedef char assert_COIN_size[sizeof(COIN) == 0x240 ? 1 : -1];

/* 93 MEDICINE (0x250): health refill; vtable 0x2D66B8, 76 slots */
struct MEDICINE : DPRIZE
{
    char pad230[0x250 - 0x230];
    virtual void Update() __asm__("UpdateMedicine");   /* slot 16 */
    virtual CLASSINFO *GetClassInfo() __asm__("GetMedicineClassDesc");   /* slot 6 */
    virtual void Init() __asm__("InitMedicine");   /* slot 7 */
    virtual void SetState() __asm__("SetMedicineDprizes");   /* slot 76 */
};
typedef char assert_MEDICINE_size[sizeof(MEDICINE) == 0x250 ? 1 : -1];

/* 92 KEY (0x240); vtable 0x2D6448, 76 slots */
struct KEY : DPRIZE
{
    char pad230[0x240 - 0x230];
    virtual void Init() __asm__("InitKey");   /* slot 7 */
    virtual CLASSINFO *GetClassInfo() __asm__("GetKeyClassDesc");   /* slot 6 */
    virtual void SetState() __asm__("SetKeyDprizes");   /* slot 76 */
};
typedef char assert_KEY_size[sizeof(KEY) == 0x240 ? 1 : -1];

/* 94 LOOT (0x250): pickpocket item; vtable 0x2D61D8, 76 slots */
struct LOOT : DPRIZE
{
    char pad230[0x250 - 0x230];
    virtual void Update() __asm__("UpdateLoot");   /* slot 16 */
    virtual CLASSINFO *GetClassInfo() __asm__("GetLootClassDesc");   /* slot 6 */
    virtual void Init() __asm__("InitLoot");   /* slot 7 */
    virtual void LoadFromBrx(CBinaryInputStream *pbis) __asm__("LoadLootFromBrx");   /* slot 14 */
    virtual void SetState() __asm__("SetLootDprizes");   /* slot 76 */
};
typedef char assert_LOOT_size[sizeof(LOOT) == 0x250 ? 1 : -1];

/* 95 CLUE (0x230): clue bottle; vtable 0x2D5F68, 76 slots */
struct CLUE : DPRIZE
{
    virtual void Init() __asm__("InitClue");   /* slot 7 */
    virtual CLASSINFO *GetClassInfo() __asm__("GetClueClassDesc");   /* slot 6 */
    virtual void OnProxyAcquire(LO *plo) __asm__("ResetClue");   /* slot 26 */
    virtual void OnProxyRelease(LO *plo) __asm__("ClearClueState");   /* slot 27 */
    virtual void SetState() __asm__("SetClueDprizes");   /* slot 76 */
};
typedef char assert_CLUE_size[sizeof(CLUE) == 0x230 ? 1 : -1];

/* 7 CLUEPROXY (0x110): PROXY child; clue-bottle proxy (REVIEW, medium); vtable 0x2D5DC8, 50 slots */
struct CLUEPROXY : PROXY
{
    char pad100[0x110 - 0x100];
    virtual void PostLoad() __asm__("PostClueproxyLoad");   /* slot 15 */
    virtual CLASSINFO *GetClassInfo() __asm__("GetClueproxyClassDesc");   /* slot 6 */
};
typedef char assert_CLUEPROXY_size[sizeof(CLUEPROXY) == 0x110 ? 1 : -1];

/* 78 VAULT (0x230): clue-bottle safe; vtable 0x2D5B30, 81 slots */
struct VAULT : PO
{
    char pad180[0x230 - 0x180];
    virtual void PostLoad() __asm__("PostVaultLoad");   /* slot 15 */
    virtual CLASSINFO *GetClassInfo() __asm__("GetVaultClassDesc");   /* slot 6 */
    virtual void Init() __asm__("InitVault");   /* slot 7 */
    virtual void Update() __asm__("UpdateVault");   /* slot 16 */
};
typedef char assert_VAULT_size[sizeof(VAULT) == 0x230 ? 1 : -1];

/* 13 RAIL (0x1D0): wires; vtable 0x2D6FE8, 50 slots */
struct RAIL : SHAPE, GRABSPOT
{
    char pad1B4[0x1D0 - 0x1B4];
    virtual void OnAdd() __asm__("OnRailAdd");   /* slot 23 */
    virtual CLASSINFO *GetClassInfo() __asm__("GetRailClassDesc");   /* slot 6 */
    virtual void Init() __asm__("InitRail");   /* slot 7 */
    virtual void PostLoad() __asm__("PostRailLoad");   /* slot 15 */
    virtual void OnRemove() __asm__("OnRailRemove");   /* slot 22 */
    virtual void Clone(LO *ploBase) __asm__("CloneRail");   /* slot 41 */
    virtual int GetGrabRange() __asm__("GetRailWrapRange");   /* GRABSPOT slot 1 (delta -400) */
    virtual void GetClosestParam() __asm__("GetRailClosestParam");   /* GRABSPOT slot 2 (delta -400) */
    virtual void GetGrabPos() __asm__("GetRailGrabPosition");   /* GRABSPOT slot 3 (delta -400) */
    virtual void GetSparklePos() __asm__("GetRailSparklePosition");   /* GRABSPOT slot 5 (delta -400) */
    virtual void GetOwner() __asm__("func_00259B08__FP4RAIL");   /* GRABSPOT slot 6 (delta -400) */
};
typedef char assert_RAIL_size[sizeof(RAIL) == 0x1D0 ? 1 : -1];

/* 61 RAJAN (0x1020); vtable 0x2D7188, 90 slots */
struct RAJAN : STEPGUARD
{
    char pad1010[0x1020 - 0x1010];
    virtual void Init() __asm__("InitRajan");   /* slot 7 */
    virtual CLASSINFO *GetClassInfo() __asm__("GetRajanClassDesc");   /* slot 6 */
    virtual int CanSenseStimulus(struct STIMULUS *pstim) __asm__("CanRajanSenseStimulus");   /* slot 89 */
    virtual void SetCameraFocus() __asm__("SetRajanCameraFocus");   /* slot 90 */
};
typedef char assert_RAJAN_size[sizeof(RAJAN) == 0x1020 ? 1 : -1];

/* 87 RIPG (0x1B0); vtable 0x2D77D8, 75 slots */
struct RIPG : SO
{
    int ripgState;          /* +0x150 [S] (rip.cpp RIPGV) */
    int unknown154;
    float emitScale;        /* +0x158 [S] */
    DL ripList;             /* +0x15C [S] the group's RIPs */
    int unknown168[2];
    qword boundsData[3];    /* +0x170 [S] the group's bounds (UpdateRipgBounds) */
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

/* 77 ROBOTTANK (0x5D0): single instance; vtable 0x2D7CA8, 81 slots */
struct ROBOTTANK : PO
{
    char pad180[0x598 - 0x180];
    int nUnk598;
    char pad59C[0x5D0 - 0x59C];
    ROBOTTANK();
    virtual void OnActive(int fActive) __asm__("OnRobottankActive");   /* slot 78 */
    virtual CLASSINFO *GetClassInfo() __asm__("GetRobottankClassDesc");   /* slot 6 */
    virtual void Init() __asm__("InitRobottank");   /* slot 7 */
    virtual void PostLoad() __asm__("PostRobottankLoad");   /* slot 15 */
    virtual void Update() __asm__("UpdateRobottank");   /* slot 16 */
    virtual void RenderSelf() __asm__("RenderRobottankSelf");   /* slot 19 */
    virtual void OnProxyAcquire(LO *plo) __asm__("ResetRobottank");   /* slot 26 */
    virtual void Clone(LO *ploBase) __asm__("CloneRobottank");   /* slot 41 */
    virtual void UpdateBounds() __asm__("UpdateRobottankBounds");   /* slot 49 */
    virtual void OnContactFreed(SO *pso) __asm__("RemoveRobottankContactsWith");   /* slot 54 */
    virtual void AdjustNewXp(XP *pxp, int ixpd) __asm__("AdjustRobottankNewXp");   /* slot 55 */
    virtual void AddCustomXps(SOPART *ppart, int n2, int n3, void *pvGeom, XP **ppxpFirst) __asm__("AddRobottankCustomXps");   /* slot 60 */
    virtual void PresetAccel() __asm__("PresetRobottankAccel");   /* slot 70 */
    virtual int TakeDamage(ZPR *pzpr) __asm__("TakeRobottankDamage");   /* slot 75 */
};
typedef char assert_ROBOTTANK_size[sizeof(ROBOTTANK) == 0x5D0 ? 1 : -1];
/* The constructor class_registry inlines into its factory. g++ 2.95 emits a class's inline member functions in
   its key TU (as global functions the ELF does not have), so the key TU defines SLY2_KEY_TU_ROBOTTANK and sees
   only the declaration. */
#ifndef SLY2_KEY_TU_ROBOTTANK
// ROBOTTANK constructor: clears nUnk598.
inline ROBOTTANK::ROBOTTANK() { nUnk598 = 0; }
#endif

/* 108 CID108 (0x170): robot-tank ammo pickup; vtable 0x2D7A40, 75 slots */
struct CID108 : SO
{
    char pad150[0x170 - 0x150];
    virtual void Update() __asm__("UpdateRcPickup");   /* slot 16 */
    virtual CLASSINFO *GetClassInfo() __asm__("GetCid108ClassDesc");   /* slot 6 */
    virtual void Init() __asm__("InitRcPickup");   /* slot 7 */
    virtual void PostLoad() __asm__("PostRcPickupLoad");   /* slot 15 */
    virtual void OnProxyAcquire(LO *plo) __asm__("ResetRobottankPickup");   /* slot 26 */
};
typedef char assert_CID108_size[sizeof(CID108) == 0x170 ? 1 : -1];

/* 112 CID112 (0xC0): aim manager / launcher (darts, guard pebbles, sword missiles); vtable 0x2D7F40, 48 slots */
struct CID112 : LO
{
    char pad50[0xC0 - 0x50];
    virtual void Init() __asm__("InitCid112");   /* slot 7 */
    virtual CLASSINFO *GetClassInfo() __asm__("GetCid112ClassDesc");   /* slot 6 */
};
typedef char assert_CID112_size[sizeof(CID112) == 0xC0 ? 1 : -1];

/* 70 SAFEHOUSE (0x240): character-select PO; vtable 0x2D80D0, 81 slots */
struct SAFEHOUSE : PO
{
    char pad180[0x240 - 0x180];
    virtual void Init() __asm__("InitSafehouse");   /* slot 7 */
    virtual CLASSINFO *GetClassInfo() __asm__("GetSafehouseClassDesc");   /* slot 6 */
    virtual void PostLoad() __asm__("PostSafehouseLoad");   /* slot 15 */
    virtual void Update() __asm__("UpdateSafehouse");   /* slot 16 */
    virtual void SetCtlState(int nState) __asm__("HandleSafehouseMessage");   /* slot 79 */
};
typedef char assert_SAFEHOUSE_size[sizeof(SAFEHOUSE) == 0x240 ? 1 : -1];

/* 85 LASEN (0x1D0): LASEN laser beams (zap 0x1000); vtable 0x2D8738, 75 slots */
struct LASEN : SENSOR
{
    char pad190[0x1D0 - 0x190];
    virtual void PostLoad() __asm__("PostLasenLoad");   /* slot 15 */
    virtual CLASSINFO *GetClassInfo() __asm__("GetLasenClassDesc");   /* slot 6 */
    virtual void Init() __asm__("InitLasen");   /* slot 7 */
    virtual void LoadFromBrx(CBinaryInputStream *pbis) __asm__("LoadLasenFromBrx");   /* slot 14 */
    virtual void Update() __asm__("UpdateLasen");   /* slot 16 */
    virtual void RenderSelf() __asm__("RenderLasenSelf");   /* slot 19 */
    virtual void Clone(LO *ploBase) __asm__("CloneLasen");   /* slot 41 */
    virtual SOTREE *NewTree() __asm__("NewLasenTree");   /* slot 53 */
    virtual void OnContactFreed(SO *pso) __asm__("RemoveLasenContactsWith");   /* slot 54 */
    virtual void AddCustomXps(SOPART *ppart, int n2, int n3, void *pvGeom, XP **ppxpFirst) __asm__("AddLasenBeamXps");   /* slot 60 */
};
typedef char assert_LASEN_size[sizeof(LASEN) == 0x1D0 ? 1 : -1];

/* 114 RELAY (0xB0): cutscene / sequence script; vtable 0x2D8C48, 48 slots */
struct RELAY : LO
{
    char pad50[0xB0 - 0x50];
    virtual void Init() __asm__("InitRelay");   /* slot 7 */
    virtual CLASSINFO *GetClassInfo() __asm__("GetRelayClassDesc");   /* slot 6 */
    virtual void LoadFromBrx(CBinaryInputStream *pbis) __asm__("LoadRelayFromBrx");   /* slot 14 */
    virtual void Update() __asm__("UpdateRelay");   /* slot 16 */
    virtual void Clone(LO *ploBase) __asm__("CloneRelay");   /* slot 41 */
};
typedef char assert_RELAY_size[sizeof(RELAY) == 0xB0 ? 1 : -1];

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
    char pad50[0x70 - 0x50];
    virtual void Init(SAAF *psaaf) __asm__("InitLoop");   /* slot 7 */
    virtual CLASSINFO *GetClassInfo() __asm__("GetLoopClassDesc");   /* slot 6 */
    virtual void Update() __asm__("UpdateLoop__FP4LOOPf");   /* slot 8 */
    virtual void GetCompletion() __asm__("GetLoopCompletion__FP4LOOP");   /* slot 9 */
};
typedef char assert_LOOP_size[sizeof(LOOP) == 0x70 ? 1 : -1];

/* 140 PINGPONG (0x70); vtable 0x2D9360, 10 slots */
struct PINGPONG : SAA
{
    char pad50[0x70 - 0x50];
    virtual void Init(SAAF *psaaf) __asm__("InitPingpong");   /* slot 7 */
    virtual CLASSINFO *GetClassInfo() __asm__("GetPingpongClassDesc");   /* slot 6 */
    virtual void Update() __asm__("UpdatePingpong__FP8PINGPONGf");   /* slot 8 */
    virtual void GetCompletion() __asm__("GetPingpongCompletion__FP8PINGPONG");   /* slot 9 */
};
typedef char assert_PINGPONG_size[sizeof(PINGPONG) == 0x70 ? 1 : -1];

/* 141 SHUFFLE (0x60); vtable 0x2D9300, 10 slots */
struct SHUFFLE : SAA
{
    char pad50[0x60 - 0x50];
    virtual void Init(SAAF *psaaf) __asm__("InitShuffle__FP7SHUFFLEP4SAAF");   /* slot 7 */
    virtual CLASSINFO *GetClassInfo() __asm__("GetShuffleClassDesc");   /* slot 6 */
    virtual void Update() __asm__("UpdateShuffle__FP7SHUFFLEf");   /* slot 8 */
};
typedef char assert_SHUFFLE_size[sizeof(SHUFFLE) == 0x60 ? 1 : -1];

/* 142 EYES (0x80); vtable 0x2D92A0, 10 slots */
struct EYES : SAA
{
    char pad50[0x80 - 0x50];
    virtual void Update() __asm__("UpdateEyes");   /* slot 8 */
    virtual CLASSINFO *GetClassInfo() __asm__("GetEyesClassDesc");   /* slot 6 */
    virtual void Init(SAAF *psaaf) __asm__("InitEyes");   /* slot 7 */
    virtual void DoesMatchOid() __asm__("DoesEyesMatchOid");   /* slot 10 */
};
typedef char assert_EYES_size[sizeof(EYES) == 0x80 ? 1 : -1];

/* 143 SCROLLER (0x70); vtable 0x2D9240, 10 slots */
struct SCROLLER : SAA
{
    char pad50[0x70 - 0x50];
    virtual void Update() __asm__("UpdateScroller");   /* slot 8 */
    virtual CLASSINFO *GetClassInfo() __asm__("GetScrollerClassDesc");   /* slot 6 */
    virtual void Init(SAAF *psaaf) __asm__("InitScroller__FP8SCROLLERP4SAAF");   /* slot 7 */
    virtual void GetCompletion() __asm__("GetScrollerCompletion__FP8SCROLLER");   /* slot 9 */
};
typedef char assert_SCROLLER_size[sizeof(SCROLLER) == 0x70 ? 1 : -1];

/* 144 CIRCLER (0x60); vtable 0x2D91E0, 10 slots */
struct CIRCLER : SAA
{
    char pad50[0x60 - 0x50];
    virtual void Init(SAAF *psaaf) __asm__("InitCircler");   /* slot 7 */
    virtual CLASSINFO *GetClassInfo() __asm__("GetCirclerClassDesc");   /* slot 6 */
    virtual void Update() __asm__("UpdateCircler__FP7CIRCLERf");   /* slot 8 */
    virtual void GetCompletion() __asm__("GetCirclerCompletion__FP7CIRCLER");   /* slot 9 */
};
typedef char assert_CIRCLER_size[sizeof(CIRCLER) == 0x60 ? 1 : -1];

/* 97 SKY (0x160): sky dome; render kind 1; vtable 0x2D9500, 75 slots */
struct SKY : SO
{
    char pad150[0x160 - 0x150];
    virtual void PostLoad() __asm__("PostSkyLoad");   /* slot 15 */
    virtual CLASSINFO *GetClassInfo() __asm__("GetSkyClassDesc");   /* slot 6 */
    virtual void Update() __asm__("UpdateSky");   /* slot 16 */
};
typedef char assert_SKY_size[sizeof(SKY) == 0x160 ? 1 : -1];

/* 39 CID39 (0x110): GRABSPOT XFM, list D_003E13D8; vtable 0x2D9DD0, 50 slots */
struct CID39 : XFM, GRABSPOT
{
    char pad104[0x110 - 0x104];
    virtual void Init() __asm__("InitGrabSpotXfm");   /* slot 7 */
    virtual CLASSINFO *GetClassInfo() __asm__("GetCid39ClassDesc");   /* slot 6 */
    virtual void PostLoad() __asm__("PostGrabSpotXfmLoad");   /* slot 15 */
    virtual void OnRemove() __asm__("OnGrabSpotXfmRemove");   /* slot 22 */
    virtual void OnAdd() __asm__("OnGrabSpotXfmAdd");   /* slot 23 */
    virtual void Clone(LO *ploBase) __asm__("CloneGrabSpotXfm");   /* slot 41 */
    virtual void GetGrabPos() __asm__("GetOffsetToObjectPosition");   /* GRABSPOT slot 3 (delta -224) */
    virtual void GetOwner() __asm__("func_00259C08__FP5CID39");   /* GRABSPOT slot 6 (delta -224) */
};
typedef char assert_CID39_size[sizeof(CID39) == 0x110 ? 1 : -1];

/* 46 CID46 (0x120): recon photo target; vtable 0x2D9F70, 50 slots */
struct CID46 : XFM
{
    char padE0[0x120 - 0xE0];
    virtual void PostLoad() __asm__("PostReconTargetLoad");   /* slot 15 */
    virtual CLASSINFO *GetClassInfo() __asm__("GetCid46ClassDesc");   /* slot 6 */
    virtual void HandleMessage(MSGID msgid, void *pv) __asm__("HandleReconTargetMessage");   /* slot 5 */
    virtual void Init() __asm__("InitReconTarget");   /* slot 7 */
    virtual void LoadFromBrx(CBinaryInputStream *pbis) __asm__("LoadReconTargetFromBrx");   /* slot 14 */
};
typedef char assert_CID46_size[sizeof(CID46) == 0x120 ? 1 : -1];

/* 125 CID125 (0x60): guard manager (oid 0x656); vtable 0x2DA3D8, 48 slots */
struct CID125 : LO
{
    int guardCount;         /* +0x50 [S] the guards in the world (PostGuardManagerLoad) */
    STEPGUARD **guards;     /* +0x54 [S] */
    char pad58[0x60 - 0x58];
    virtual void Init() __asm__("InitGuardManager");   /* slot 7 */
    virtual CLASSINFO *GetClassInfo() __asm__("GetCid125ClassDesc");   /* slot 6 */
    virtual void PostLoad() __asm__("PostGuardManagerLoad");   /* slot 15 */
    virtual void Update() __asm__("UpdateGuardManager");   /* slot 16 */
};
typedef char assert_CID125_size[sizeof(CID125) == 0x60 ? 1 : -1];
typedef char assert_CID125_guards[SLY2_OFFSETOF(CID125, guards) == 0x54 ? 1 : -1];

/* 129 WIDGET129 (0x220): WIDGET: objective counter (computed per frame); vtable 0x2DAD90, 50 slots */
struct WIDGET129 : WIDGET
{
    char pad110[0x220 - 0x110];
    virtual void PostLoad() __asm__("PostLoadItemCounterWidget");   /* slot 15 */
    virtual CLASSINFO *GetClassInfo() __asm__("GetWidget129ClassDesc");   /* slot 6 */
    virtual void Update() __asm__("UpdateItemCounterWidget");   /* slot 16 */
};
typedef char assert_WIDGET129_size[sizeof(WIDGET129) == 0x220 ? 1 : -1];

/* 127 WIDGET127 (0x120): WIDGET: modal hands-off box; vtable 0x2DABF0, 50 slots */
struct WIDGET127 : WIDGET
{
    char pad110[0x120 - 0x110];
    virtual void Init() __asm__("InitModalBoxWidget");   /* slot 7 */
    virtual CLASSINFO *GetClassInfo() __asm__("GetWidget127ClassDesc");   /* slot 6 */
    virtual void Update() __asm__("UpdateModalBoxWidget");   /* slot 16 */
    virtual void Show(int idLabel) __asm__("ShowModalBoxWidget");   /* slot 49 */
};
typedef char assert_WIDGET127_size[sizeof(WIDGET127) == 0x120 ? 1 : -1];

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
    char padE0[0x100 - 0xE0];
    virtual void OnAdd() __asm__("OnLookTargetAdd");   /* slot 23 */
    virtual CLASSINFO *GetClassInfo() __asm__("GetCid21ClassDesc");   /* slot 6 */
    virtual void Init() __asm__("InitLookTarget");   /* slot 7 */
    virtual void OnRemove() __asm__("OnLookTargetRemove");   /* slot 22 */
    virtual void Clone(LO *ploBase) __asm__("CloneLookTarget");   /* slot 41 */
};
typedef char assert_CID21_size[sizeof(CID21) == 0x100 ? 1 : -1];

/* 100 CID100 (0x1B0): tossables (alarm clock tossable_distract, music box); vtable 0x2DBB10, 76 slots */
struct CID100 : SO
{
    char pad150[0x1B0 - 0x150];
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
    virtual void SetState() __asm__("SetTossableState");   /* slot 76 */
};
typedef char assert_CID100_size[sizeof(CID100) == 0x1B0 ? 1 : -1];

/* 101 CID101 (0x1B0): Bentley trigger bomb / mines (gadget object); vtable 0x2C9810, 76 slots */
struct CID101 : CID100
{
    virtual CLASSINFO *GetClassInfo() __asm__("GetCid101ClassDesc");   /* slot 6 */
};
typedef char assert_CID101_size[sizeof(CID101) == 0x1B0 ? 1 : -1];

/* 130 WIDGET130 (0x130); vtable 0x2C9670, 50 slots */
struct WIDGET130 : WIDGET
{
    int anUnk110[4];                          /* +0x110 set to -1 by the constructor */
    int nUnk120;                              /* +0x120 set to -1 by the constructor */
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
    virtual void AddExternalAccelerations() __asm__("PushGuardTrapAway");   /* slot 71 */
    virtual void SetState() __asm__("SelectGuardTrapTarget");   /* slot 76 */
};
typedef char assert_CID102_size[sizeof(CID102) == 0x1D0 ? 1 : -1];

/* 69 TURRET (0x1E0): grapple turret; vtable 0x2DBD80, 81 slots */
struct TURRET : PO
{
    char pad180[0x1E0 - 0x180];
    virtual void PostLoad() __asm__("PostTurretLoad");   /* slot 15 */
    virtual CLASSINFO *GetClassInfo() __asm__("GetTurretClassDesc");   /* slot 6 */
    virtual void Init() __asm__("InitTurret");   /* slot 7 */
    virtual void Update() __asm__("UpdateTurret");   /* slot 16 */
    virtual void Clone(LO *ploBase) __asm__("CloneTurret");   /* slot 41 */
    virtual int ShouldIgnoreIntersection(SO *psoOther) __asm__("FilterTurretProxySource");   /* slot 61 */
    virtual void OnActive(int fActive) __asm__("OnTurretActive");   /* slot 78 */
};
typedef char assert_TURRET_size[sizeof(TURRET) == 0x1E0 ? 1 : -1];

/* 5 CID5 (0x120): vis zone (all *_vz WEs); vtable 0x2DC070, 50 slots */
struct CID5 : XFM
{
    char padE0[0x120 - 0xE0];
    virtual void Init() __asm__("InitVis");   /* slot 7 */
    virtual CLASSINFO *GetClassInfo() __asm__("GetCid5ClassDesc");   /* slot 6 */
    virtual void LoadFromBrx(CBinaryInputStream *pbis) __asm__("LoadVisFromBrx");   /* slot 14 */
};
typedef char assert_CID5_size[sizeof(CID5) == 0x120 ? 1 : -1];

/* 23 CID23 (0x140): VOL child; vtable 0x2DC210, 50 slots */
struct CID23 : VOL
{
    char pad130[0x140 - 0x130];
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

/* 26 EXIT (0x190): level exit volume; vtable 0x2DC6F0, 50 slots */
struct EXIT : VOL
{
    char pad130[0x190 - 0x130];
    virtual void Init() __asm__("InitExit");   /* slot 7 */
    virtual CLASSINFO *GetClassInfo() __asm__("GetExitClassDesc");   /* slot 6 */
    virtual void LoadFromBrx(CBinaryInputStream *pbis) __asm__("LoadExitFromBrx");   /* slot 14 */
    virtual void Update() __asm__("UpdateExit");   /* slot 16 */
};
typedef char assert_EXIT_size[sizeof(EXIT) == 0x190 ? 1 : -1];

/* 42 WARP (0x180): destinations; entrance_* aliases; vtable 0x2DC550, 50 slots */
struct WARP : XFM
{
    char padE0[0x180 - 0xE0];
    virtual void LoadFromBrx(CBinaryInputStream *pbis) __asm__("LoadWarpFromBrx");   /* slot 14 */
    virtual CLASSINFO *GetClassInfo() __asm__("GetWarpClassDesc");   /* slot 6 */
};
typedef char assert_WARP_size[sizeof(WARP) == 0x180 ? 1 : -1];

/* 89 WATER (0x190): zap kind 0x2000; vtable 0x2DCA30, 75 slots */
struct WATER : SO
{
    char pad150[0x190 - 0x150];
    virtual void PostLoad() __asm__("PostWaterLoad");   /* slot 15 */
    virtual CLASSINFO *GetClassInfo() __asm__("GetWaterClassDesc");   /* slot 6 */
    virtual void HandleMessage(MSGID msgid, void *pv) __asm__("HandleWaterMessage");   /* slot 5 */
    virtual void Init() __asm__("InitWater");   /* slot 7 */
    virtual void RemoveHierarchy() __asm__("RemoveWaterHierarchy");   /* slot 12 */
    virtual void Update() __asm__("UpdateWater");   /* slot 16 */
    virtual void UpdateBounds() __asm__("UpdateWaterBounds");   /* slot 49 */
    virtual void InitPhys() __asm__("InitWaterPhysics");   /* slot 64 */
    virtual void AddExternalAccelerations() __asm__("AddWaterExternalAccelerations");   /* slot 71 */
    virtual void IsPosInside() __asm__("IsPosInWater");   /* slot 73 */
};
typedef char assert_WATER_size[sizeof(WATER) == 0x190 ? 1 : -1];

/* 107 WAYPT (0x190): job marker mission_start_*; vtable 0x2DCC98, 75 slots */
struct WAYPT : SO
{
    char pad150[0x190 - 0x150];
    virtual void PostLoad() __asm__("PostWayptLoad");   /* slot 15 */
    virtual CLASSINFO *GetClassInfo() __asm__("GetWayptClassDesc");   /* slot 6 */
    virtual void Init() __asm__("InitWaypt");   /* slot 7 */
    virtual void Update() __asm__("UpdateWaypt");   /* slot 16 */
    virtual void RenderSelf() __asm__("RenderWaypt");   /* slot 19 */
};
typedef char assert_WAYPT_size[sizeof(WAYPT) == 0x190 ? 1 : -1];

/* 115 WR (0x900); vtable 0x2DCF00, 48 slots */
struct WR : LO
{
    char pad50[0x900 - 0x50];
    virtual void Init() __asm__("InitWr");   /* slot 7 */
    virtual CLASSINFO *GetClassInfo() __asm__("GetWrClassDesc");   /* slot 6 */
    virtual void Update() __asm__("UpdateWrMatrixes__FP2WR");   /* slot 16 */
};
typedef char assert_WR_size[sizeof(WR) == 0x900 ? 1 : -1];

#endif /* SLY2_CLASSES_H */
