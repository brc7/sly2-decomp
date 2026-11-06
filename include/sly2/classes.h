/* include/sly2/classes.h -- the classes of the class registry (app/class_registry.cpp's factories and descriptor
 * getters) that no other sly2 header defines, at file scope under their real names, so that their constructors
 * reference the real vtables (_vt$<len><NAME>, named in config/symbol_addrs.txt) and no alias line is needed.
 *
 * Each class: its parent (class-tree.md), its size (the descriptor's cb, asserted), what construction shows (the
 * members that are constructed, out-of-line constructors under their current symbols), and its real virtual layout
 * from the ELF vtable: the overrides of inherited slots (Pclassinfo, slot 6, first: no TU defines it as a member, so
 * no TU becomes a vtable's key TU and emits it) and its new slots, each declared as a member whose asm label is the
 * function the vtable holds at that slot. Members inherit the names and signatures of the slot's first declaration
 * (sly2/lo.h, so.h, ...); slots no header names are VSlot<n>() (n = 8-byte slot index; slot 0 is null). The ten
 * grab-spot classes override GRABSPOT's slots as their secondary vtables show (delta = -offset of GRABSPOT).
 * The classes the other headers define (BASIC, LO, XFM, PROXY, SHAPE, TN, TARGET, VOL, DETECTOR, EMITTER, EXPL,
 * EXPLO, SO, CID148, CID150, PO, POCTL, STEP, LEAP, CID146, BHV, STEPPLAYER, JT, MURRAY, BENTLEY, STEPGUARD, ASEG,
 * ASEGBL, SM, ASEGA, SMA, JOB, TASK, CM) and SW (sly2/sw.h, types/views-3) carry the same in their headers.
 * Generated from the ELF and class_registry's construction views (types/views-2); see
 * docs/guides/tu-merging.md, "Views unified". */
#ifndef SLY2_CLASSES_H
#define SLY2_CLASSES_H
#include "sly2/players.h"
#include "sly2/guard.h"
#include "sly2/cm.h"
#include "sly2/emitter.h"
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
inline BASIC::BASIC() : pframe(refptrNull), psidebag(refptrNull) {}
/* PO (sly2/po.h): the one PO field construction sets. */
inline PO::PO() { pvUnk174 = 0; }

/* The grab-spot mix-in (second base of the grab-spot classes): GRABSPOTDATA (0x20 bytes, object/proxy.cpp) and
   its own vptr at +0x20 (vtable 0x2D28D8, 6 slots; hidden-types.md B2.12). Slot names from gameplay/player/grabspot.cpp;
   only FGetGrabRange's signature is modelled.
   A class with a secondary base must override at least one of its virtuals exactly when its secondary vtable in the
   ELF differs from the base's own: with -fno-rtti g++ reuses the base's vtable (_vt$8GRABSPOT) for a secondary base
   whose virtuals the class does not override, which changes the constructor's vptr stores. Every grab-spot class
   overrides slot 6 (an 8-byte function at 0x2599E8..); CID36/CID37 override nothing of GRABSPOT and reuse CID35's
   (0x2D1EA8 in the ELF for all three). */
struct GRABSPOT
{
    char abData[0x20];
    virtual int FGetGrabRange() __asm__("GRABSPOT_FGetGrabRange");   /* slot 1 */
    virtual void GetClosestParam() __asm__("func_00182CC8");         /* slot 2 */
    virtual void GetGrabPos() __asm__("func_00182CD0");              /* slot 3 */
    virtual void AdjustScore() __asm__("func_00182D70");             /* slot 4 */
    virtual void GetSparklePos() __asm__("func_00182D78");           /* slot 5 */
    virtual void VtF() __asm__("func_00182E78");                     /* slot 6 */
};

/* BLIPG's 32 records (0x1C bytes each) with an empty inline constructor: g++ still emits the 32-step loop. */
struct BLIP
{
    char ab[0x1C];
    BLIP() {}
};

/* A two-pointer record constructed out of line by func_002562A8 (guardbhv; zeroes its first two words). PATHLINK
   holds six, BHVGOTO three, BHV191 one. HYPOTHESIS: a path-link endpoint {pshape, plo, .., pos} (func_002562A8's
   annotation). */
struct PATHLINKEND
{
    void *pv0;
    void *pv1;
    char ab[0x18];
    PATHLINKEND() __asm__("func_002562A8");
};

/* BHVGUARD's 0x70-byte record at +0x30 (awareness mode at +0x30: BHVGUARD.md); its inline constructor clears it. */
struct BHVGUARDREC
{
    char ab[0x70];
    BHVGUARDREC() { memset(this, 0, sizeof(*this)); }
};

/* 84 SENSOR (0x190); vtable 0x2D89A0, 75 slots */
struct SENSOR : SO
{
    char pad150[0x190 - 0x150];
    virtual CLASSINFO *Pclassinfo() __asm__("GetSensorClassDesc");   /* slot 6 */
    virtual void Init() __asm__("InitSensor");   /* slot 7 */
    virtual int FIgnoreIntersection(SO *psoOther) __asm__("FIgnoreSensorIntersection");   /* slot 61 */
};
typedef char assert_SENSOR_size[sizeof(SENSOR) == 0x190 ? 1 : -1];

/* 90 DPRIZE (0x230): abstract; vtable 0x2D6B98, 76 slots */
struct DPRIZE : SO
{
    char pad150[0x230 - 0x150];
    virtual CLASSINFO *Pclassinfo() __asm__("GetDprizeClassDesc");   /* slot 6 */
    virtual void HandleMessage(MSGID msgid, void *pv) __asm__("HandleDprizeMessage");   /* slot 5 */
    virtual void Init() __asm__("InitDprize");   /* slot 7 */
    virtual void LoadFromBrx(CBinaryInputStream *pbis) __asm__("LoadDprizeFromBrx");   /* slot 14 */
    virtual void VUnk14() __asm__("PostDprizeLoad");   /* slot 15 */
    virtual void VUnk15() __asm__("UpdateDprize");   /* slot 16 */
    virtual void VUnk16() __asm__("ProjectDprizeTransform");   /* slot 17 */
    virtual void RenderSelf() __asm__("RenderDprizeSelf");   /* slot 19 */
    virtual int FTakeDamage(ZPR *pzpr) __asm__("func_001C5DF0");   /* slot 75 */
    virtual void VSlot76() __asm__("SetDprizeDprizes");   /* slot 76 */
};
typedef char assert_DPRIZE_size[sizeof(DPRIZE) == 0x230 ? 1 : -1];

/* 126 WIDGET (0x110); vtable 0x2DAF30, 50 slots */
struct WIDGET : LO
{
    char pad50[0x110 - 0x50];
    WIDGET() __asm__("WIDGET_ctor");          /* out of line: presentation/hud/WIDGET_ctor.c */
    virtual CLASSINFO *Pclassinfo() __asm__("GetWidgetClassDesc");   /* slot 6 */
    virtual void LoadFromBrx(CBinaryInputStream *pbis) __asm__("LoadWidgetFromBrx");   /* slot 14 */
    virtual void VUnk14() __asm__("PostWidgetLoad");   /* slot 15 */
    virtual void VUnk15() __asm__("UpdateWidget");   /* slot 16 */
    virtual void RenderSelf() __asm__("RenderWidget");   /* slot 19 */
    virtual void VSlot49() __asm__("ShowWidget");   /* slot 49 */
    virtual void VSlot50() __asm__("HideWidget");   /* slot 50 */
};
typedef char assert_WIDGET_size[sizeof(WIDGET) == 0x110 ? 1 : -1];

/* 130 WIDGET130 (0x130); vtable 0x2C9670, 50 slots */
struct WIDGET130 : WIDGET
{
    char pad110[0x130 - 0x110];
    WIDGET130() __asm__("func_00211C58");     /* out of line: presentation/hud/func_00211C58.cpp */
    virtual CLASSINFO *Pclassinfo() __asm__("GetWidget130ClassDesc");   /* slot 6 */
};
typedef char assert_WIDGET130_size[sizeof(WIDGET130) == 0x130 ? 1 : -1];

/* 138 SAA (0x50); vtable 0x2D9420, 10 slots */
struct SAA : BASIC
{
    char pad10[0x50 - 0x10];
    virtual CLASSINFO *Pclassinfo() __asm__("GetSaaClassDesc");   /* slot 6 */
    virtual void VSlot7() __asm__("func_001ED8E0");   /* slot 7 */
    virtual void VSlot8() __asm__("UpdateSaa");   /* slot 8 */
    virtual void VSlot9() __asm__("UCompleteSaa__FP3SAA");   /* slot 9 */
    virtual void VSlot10() __asm__("func_001EDB80");   /* slot 10 */
};
typedef char assert_SAA_size[sizeof(SAA) == 0x50 ? 1 : -1];

/* 147 CID147 (0x60): stimulus (noise) instance; vtable 0x2CE2B8, 6 slots */
struct CID147 : BASIC
{
    char pad10[0x60 - 0x10];
    virtual CLASSINFO *Pclassinfo() __asm__("GetCid147ClassDesc");   /* slot 6 */
    virtual void VUnk00() __asm__("func_001566B8");   /* slot 1 */
    virtual void VUnk01() __asm__("func_001566C8");   /* slot 2 */
};
typedef char assert_CID147_size[sizeof(CID147) == 0x60 ? 1 : -1];

/* 149 CID149 (0x30): Splice XF handle; vtable 0x2CA2C0, 6 slots */
struct CID149 : CID148
{
    char pad20[0x30 - 0x20];
    virtual CLASSINFO *Pclassinfo() __asm__("GetCid149ClassDesc");   /* slot 6 */
};
typedef char assert_CID149_size[sizeof(CID149) == 0x30 ? 1 : -1];

/* 151 CID151 (0x60): SND (sound instance); vtable not known */
struct CID151 : BASIC
{
    char pad10[0x60 - 0x10];
    virtual CLASSINFO *Pclassinfo() __asm__("GetCid151ClassDesc");   /* slot 6 (vtable not known) */
};
typedef char assert_CID151_size[sizeof(CID151) == 0x60 ? 1 : -1];

/* 152 CID152 (0x250): sound manager (find-sound-object~); vtable 0x2CA510, 6 slots */
struct CID152 : BASIC
{
    char pad10[0x250 - 0x10];
    virtual CLASSINFO *Pclassinfo() __asm__("GetCid152ClassDesc");   /* slot 6 */
};
typedef char assert_CID152_size[sizeof(CID152) == 0x250 ? 1 : -1];

/* 154 CID154 (0x30): ticker / hint box (find-ticker-object~); vtable 0x2CA490, 6 slots */
struct CID154 : BASIC
{
    char pad10[0x30 - 0x10];
    virtual CLASSINFO *Pclassinfo() __asm__("GetCid154ClassDesc");   /* slot 6 */
};
typedef char assert_CID154_size[sizeof(CID154) == 0x30 ? 1 : -1];

/* 58 GUARD (0x1010): plain guard; vtable 0x2C9DC0, 89 slots */
struct GUARD : STEPGUARD
{
    virtual CLASSINFO *Pclassinfo() __asm__("GetGuardClassDesc");   /* slot 6 */
};
typedef char assert_GUARD_size[sizeof(GUARD) == 0x1010 ? 1 : -1];

/* 44 CID44 (0x140): camera marker (CPCAMERA target); vtable 0x2CA9F0, 50 slots */
struct CID44 : XFM
{
    char padE0[0x140 - 0xE0];
    virtual void Init() __asm__("func_0012E600");   /* slot 7 */
    virtual CLASSINFO *Pclassinfo() __asm__("GetCid44ClassDesc");   /* slot 6 */
    virtual void AddHierarchy() __asm__("func_0012E6A0");   /* slot 11 */
    virtual void RemoveHierarchy() __asm__("func_0012E6E8");   /* slot 12 */
    virtual void VUnk14() __asm__("func_0012E648");   /* slot 15 */
    virtual void *VUnk42(int id) __asm__("func_0012E738");   /* slot 43 */
    virtual void VUnk43(int id) __asm__("func_0012E740");   /* slot 44 */
    virtual void VUnk44(int id) __asm__("func_0012E750");   /* slot 45 */
};
typedef char assert_CID44_size[sizeof(CID44) == 0x140 ? 1 : -1];

/* 43 CID43 (0x100): ambient-sound XFM (OnAdd/OnRemove start/stop); vtable 0x2CABD8, 50 slots */
struct CID43 : XFM
{
    char padE0[0x100 - 0xE0];
    virtual CLASSINFO *Pclassinfo() __asm__("GetCid43ClassDesc");   /* slot 6 */
    virtual void Init() __asm__("func_0012EB48");   /* slot 7 */
    virtual void LoadFromBrx(CBinaryInputStream *pbis) __asm__("func_0012EB78");   /* slot 14 */
    virtual void VUnk14() __asm__("func_0012EC50");   /* slot 15 */
    virtual void Freeze() __asm__("func_0012EDE8");   /* slot 22 */
    virtual void Unfreeze() __asm__("func_0012ED20");   /* slot 23 */
};
typedef char assert_CID43_size[sizeof(CID43) == 0x100 ? 1 : -1];

/* 86 BARRIER (0x180); vtable 0x2CAF88, 75 slots */
struct BARRIER : SO
{
    char pad150[0x180 - 0x150];
    virtual CLASSINFO *Pclassinfo() __asm__("GetBarrierClassDesc");   /* slot 6 */
    virtual void Init() __asm__("InitBarrier");   /* slot 7 */
    virtual void VUnk14() __asm__("PostBarrierLoad");   /* slot 15 */
    virtual void AdjustNewXp(XP *pxp, int ixpd) __asm__("NotifyBarrierImpact");   /* slot 55 */
    virtual int FIgnoreIntersection(SO *psoOther) __asm__("FIgnoreBarrierIntersection");   /* slot 61 */
};
typedef char assert_BARRIER_size[sizeof(BARRIER) == 0x180 ? 1 : -1];

/* 28 BLIPG (0x460); vtable 0x2CB5D8, 51 slots */
struct BLIPG : XFM
{
    BLIP ablip[32];
    virtual CLASSINFO *Pclassinfo() __asm__("GetBlipgClassDesc");   /* slot 6 */
    virtual void VUnk01() __asm__("func_00139318");   /* slot 2 */
    virtual void Init() __asm__("InitBlipg__FP5BLIPG");   /* slot 7 */
    virtual void RemoveHierarchy() __asm__("OnBlipgRemove__FP5BLIPG");   /* slot 12 */
    virtual void VUnk15() __asm__("UpdateBlipg");   /* slot 16 */
    virtual void VUnk16() __asm__("ProjectBlipgTransform");   /* slot 17 */
    virtual void RenderSelf() __asm__("RenderBlipgSelf");   /* slot 19 */
    virtual void VSlot51() __asm__("SubscribeBlipgObject");   /* slot 51 */
};
typedef char assert_BLIPG_size[sizeof(BLIPG) == 0x460 ? 1 : -1];

/* 79 BOMB (0x210): also guard rock ammo; vtable 0x2CB780, 77 slots */
struct BOMB : SO
{
    char pad150[0x210 - 0x150];
    virtual CLASSINFO *Pclassinfo() __asm__("GetBombClassDesc");   /* slot 6 */
    virtual void HandleMessage(MSGID msgid, void *pv) __asm__("HandleBombMessage");   /* slot 5 */
    virtual void Init() __asm__("InitBomb");   /* slot 7 */
    virtual void VUnk14() __asm__("PostBombLoad");   /* slot 15 */
    virtual void VUnk15() __asm__("UpdateBomb");   /* slot 16 */
    virtual void VUnk25(LO *plo) __asm__("ResetBomb");   /* slot 26 */
    virtual void VUnk57() __asm__("func_0013AEB0");   /* slot 58 */
    virtual void VUnk59(SOPART *ppart, int n2, int n3, void *pvGeom, XP **ppxpFirst) __asm__("AddBombCustomXps");   /* slot 60 */
    virtual int FIgnoreIntersection(SO *psoOther) __asm__("FIgnoreBombIntersection");   /* slot 61 */
    virtual int FTakeDamage(ZPR *pzpr) __asm__("FAbsorbBombWkr");   /* slot 75 */
    virtual void VSlot76() __asm__("func_0013AA70");   /* slot 76 */
    virtual void VSlot77() __asm__("ApplyBombThrow");   /* slot 77 */
};
typedef char assert_BOMB_size[sizeof(BOMB) == 0x210 ? 1 : -1];

/* 103 BOOTY (0x190): replaces cid_table ALARM; vtable 0x2CBBA0, 75 slots */
struct BOOTY : SO
{
    char pad150[0x190 - 0x150];
    virtual CLASSINFO *Pclassinfo() __asm__("GetBootyClassDesc");   /* slot 6 */
    virtual void HandleMessage(MSGID msgid, void *pv) __asm__("HandleBootyMessage");   /* slot 5 */
    virtual void Init() __asm__("InitBooty");   /* slot 7 */
    virtual void LoadFromBrx(CBinaryInputStream *pbis) __asm__("LoadBootyFromBrx");   /* slot 14 */
    virtual void VUnk14() __asm__("PostBootyLoad");   /* slot 15 */
    virtual void VUnk15() __asm__("UpdateBooty");   /* slot 16 */
    virtual void RenderSelf() __asm__("RenderBootySelf");   /* slot 19 */
    virtual void VUnk26(LO *plo) __asm__("func_0013BF30");   /* slot 27 */
    virtual void AdjustNewXp(XP *pxp, int ixpd) __asm__("AdjustBootyNewXp");   /* slot 55 */
    virtual int FIgnoreIntersection(SO *psoOther) __asm__("FIgnoreBootyIntersection");   /* slot 61 */
    virtual int FTakeDamage(ZPR *pzpr) __asm__("func_0013C368");   /* slot 75 */
};
typedef char assert_BOOTY_size[sizeof(BOOTY) == 0x190 ? 1 : -1];

/* 104 CID104 (0x180): buffer effects / frame-capture transition; vtable 0x2CC548, 75 slots */
struct CID104 : SO
{
    char pad150[0x180 - 0x150];
    virtual void VUnk14() __asm__("func_00147220");   /* slot 15 */
    virtual CLASSINFO *Pclassinfo() __asm__("GetCid104ClassDesc");   /* slot 6 */
    virtual void HandleMessage(MSGID msgid, void *pv) __asm__("func_00147338");   /* slot 5 */
    virtual void Init() __asm__("func_001471C0");   /* slot 7 */
    virtual void VUnk15() __asm__("func_00147278");   /* slot 16 */
};
typedef char assert_CID104_size[sizeof(CID104) == 0x180 ? 1 : -1];

/* 83 BUTTON (0x300): instances: bouncers, buoys; vtable 0x2CC950, 75 slots */
struct BUTTON : SO
{
    char pad150[0x300 - 0x150];
    virtual CLASSINFO *Pclassinfo() __asm__("GetButtonClassDesc");   /* slot 6 */
    virtual void Init() __asm__("InitButton");   /* slot 7 */
    virtual void RemoveHierarchy() __asm__("func_001492B0");   /* slot 12 */
    virtual void LoadFromBrx(CBinaryInputStream *pbis) __asm__("LoadButtonFromBrx");   /* slot 14 */
    virtual void VUnk14() __asm__("PostButtonLoad");   /* slot 15 */
    virtual void VUnk15() __asm__("UpdateButton");   /* slot 16 */
    virtual void Clone(LO *ploBase) __asm__("CloneButton");   /* slot 41 */
    virtual int FTakeDamage(ZPR *pzpr) __asm__("FAbsorbButtonWkr");   /* slot 75 */
};
typedef char assert_BUTTON_size[sizeof(BUTTON) == 0x300 ? 1 : -1];

/* 24 VOLBTN (0x2D0): REVIEW, high; vtable 0x2CC7B0, 50 slots */
struct VOLBTN : VOL
{
    char pad130[0x2D0 - 0x130];
    virtual CLASSINFO *Pclassinfo() __asm__("GetVolbtnClassDesc");   /* slot 6 */
    virtual void Init() __asm__("InitVolbtn");   /* slot 7 */
    virtual void RemoveHierarchy() __asm__("func_00149880");   /* slot 12 */
    virtual void LoadFromBrx(CBinaryInputStream *pbis) __asm__("LoadVolbtnFromBrx");   /* slot 14 */
    virtual void VUnk14() __asm__("PostVolbtnLoad");   /* slot 15 */
    virtual void VUnk15() __asm__("UpdateVolbtn");   /* slot 16 */
    virtual void Clone(LO *ploBase) __asm__("CloneVolbtn");   /* slot 41 */
};
typedef char assert_VOLBTN_size[sizeof(VOLBTN) == 0x2D0 ? 1 : -1];

/* 25 CID25 (0x2D0): VOLBTN variant; vtable 0x2C9C20, 50 slots */
struct CID25 : VOLBTN
{
    virtual CLASSINFO *Pclassinfo() __asm__("GetCid25ClassDesc");   /* slot 6 */
};
typedef char assert_CID25_size[sizeof(CID25) == 0x2D0 ? 1 : -1];

/* 20 CID20 (0xF0): hand-attach XFM (adjust_hand_jt / _bentley, 2 per level); vtable 0x2C9A80, 50 slots */
struct CID20 : XFM
{
    char padE0[0xF0 - 0xE0];
    virtual CLASSINFO *Pclassinfo() __asm__("GetCid20ClassDesc");   /* slot 6 */
};
typedef char assert_CID20_size[sizeof(CID20) == 0xF0 ? 1 : -1];

/* 123 CID123 (0x60): 256-point route; vtable 0x2CCF10, 48 slots */
struct CID123 : LO
{
    char pad50[0x60 - 0x50];
    virtual void VUnk14() __asm__("func_0014A0A8");   /* slot 15 */
    virtual CLASSINFO *Pclassinfo() __asm__("GetCid123ClassDesc");   /* slot 6 */
    virtual void LoadFromBrx(CBinaryInputStream *pbis) __asm__("LoadRouteFromBrx");   /* slot 14 */
};
typedef char assert_CID123_size[sizeof(CID123) == 0x60 ? 1 : -1];

/* 182 BHV182 (0x50): keep pace on route; vtable 0x2CCE90, 14 slots */
struct BHV182 : BHV
{
    char pad30[0x50 - 0x30];
    virtual void Start() __asm__("func_0014A200");   /* slot 8 */
    virtual CLASSINFO *Pclassinfo() __asm__("GetBhv182ClassDesc");   /* slot 6 */
    virtual void OnPush() __asm__("func_0014A1B0");   /* slot 7 */
    virtual void VSlot9() __asm__("func_0014A248");   /* slot 9 */
    virtual void VSlot10() __asm__("func_0014A2A0");   /* slot 10 */
};
typedef char assert_BHV182_size[sizeof(BHV182) == 0x50 ? 1 : -1];

/* 63 CARMELITA (0x1020); vtable 0x2CCBB8, 89 slots */
struct CARMELITA : STEPGUARD
{
    char pad1010[0x1020 - 0x1010];
    virtual void Init() __asm__("func_0014B1A0");   /* slot 7 */
    virtual CLASSINFO *Pclassinfo() __asm__("GetCarmelitaClassDesc");   /* slot 6 */
    virtual void HandleMessage(MSGID msgid, void *pv) __asm__("func_0014B218");   /* slot 5 */
    virtual void VUnk14() __asm__("func_0014B1D8");   /* slot 15 */
    virtual int VUnk85(ZPR *pzpr) __asm__("func_0014B460");   /* slot 86 */
    virtual void VSlot87() __asm__("func_0014B290");   /* slot 88 */
};
typedef char assert_CARMELITA_size[sizeof(CARMELITA) == 0x1020 ? 1 : -1];

/* 64 CONTESSA (0x1010); vtable 0x2CD0A0, 89 slots */
struct CONTESSA : STEPGUARD
{
    virtual void Init() __asm__("func_0014FAC8");   /* slot 7 */
    virtual CLASSINFO *Pclassinfo() __asm__("GetContessaClassDesc");   /* slot 6 */
    virtual void VUnk15() __asm__("func_0014FAF8");   /* slot 16 */
};
typedef char assert_CONTESSA_size[sizeof(CONTESSA) == 0x1010 ? 1 : -1];

/* 68 DANCER (0x410): rhythm-game performer; vtable 0x2CD748, 87 slots */
struct DANCER : STEP
{
    char pad2C0[0x410 - 0x2C0];
    virtual void VUnk14() __asm__("func_00152100");   /* slot 15 */
    virtual CLASSINFO *Pclassinfo() __asm__("GetDancerClassDesc");   /* slot 6 */
    virtual void HandleMessage(MSGID msgid, void *pv) __asm__("func_00152250");   /* slot 5 */
    virtual void RemoveHierarchy() __asm__("func_001520C8");   /* slot 12 */
    virtual void VUnk15() __asm__("func_00152148");   /* slot 16 */
    virtual void GetCpdefi(CPDEFI *pcpdefi) __asm__("GetDancerCpdefi");   /* slot 50 */
};
typedef char assert_DANCER_size[sizeof(DANCER) == 0x410 ? 1 : -1];

/* 121 CID121 (0x10B0): rhythm-game manager; vtable 0x2CD5B8, 48 slots */
struct CID121 : LO
{
    char pad50[0x10B0 - 0x50];
    virtual void VUnk14() __asm__("func_001527C8");   /* slot 15 */
    virtual CLASSINFO *Pclassinfo() __asm__("GetCid121ClassDesc");   /* slot 6 */
    virtual void Init() __asm__("InitRhythmMgr");   /* slot 7 */
    virtual void VUnk15() __asm__("UpdateRhythmMgr");   /* slot 16 */
    virtual void RenderSelf() __asm__("DrawRhythmMgr");   /* slot 19 */
};
typedef char assert_CID121_size[sizeof(CID121) == 0x10B0 ? 1 : -1];

/* 122 CID122 (0x60): ? (LoadFromBrx with FILEREC list); vtable 0x2CDA10, 48 slots */
struct CID122 : LO
{
    char pad50[0x60 - 0x50];
    virtual CLASSINFO *Pclassinfo() __asm__("GetCid122ClassDesc");   /* slot 6 */
    virtual void LoadFromBrx(CBinaryInputStream *pbis) __asm__("func_00153B08");   /* slot 14 */
};
typedef char assert_CID122_size[sizeof(CID122) == 0x60 ? 1 : -1];

/* 59 DIMITRI (0x1020); vtable 0x2CDE40, 89 slots */
struct DIMITRI : GUARD
{
    char pad1010[0x1020 - 0x1010];
    virtual CLASSINFO *Pclassinfo() __asm__("GetDimitriClassDesc");   /* slot 6 */
    virtual void LoadFromBrx(CBinaryInputStream *pbis) __asm__("LoadDimitriFromBrx");   /* slot 14 */
    virtual void VUnk14() __asm__("PostDimitriLoad");   /* slot 15 */
};
typedef char assert_DIMITRI_size[sizeof(DIMITRI) == 0x1020 ? 1 : -1];

/* 180 BHV180 (0x40): DIMITRI flee; vtable 0x2CDDC0, 14 slots */
struct BHV180 : BHV
{
    char pad30[0x40 - 0x30];
    virtual CLASSINFO *Pclassinfo() __asm__("GetBhv180ClassDesc");   /* slot 6 */
    virtual void OnPush() __asm__("func_00155560");   /* slot 7 */
    virtual void Start() __asm__("func_001555E0");   /* slot 8 */
    virtual void VSlot9() __asm__("func_00155598");   /* slot 9 */
    virtual void VSlot10() __asm__("func_00155660");   /* slot 10 */
};
typedef char assert_BHV180_size[sizeof(BHV180) == 0x40 ? 1 : -1];

/* 181 BHV181 (0x60): DIMITRI flee; vtable 0x2CDD40, 14 slots */
struct BHV181 : BHV
{
    char pad30[0x60 - 0x30];
    virtual CLASSINFO *Pclassinfo() __asm__("GetBhv181ClassDesc");   /* slot 6 */
    virtual void OnPush() __asm__("func_001559B0");   /* slot 7 */
    virtual void Start() __asm__("func_00155A38");   /* slot 8 */
    virtual void VSlot9() __asm__("func_001559F8");   /* slot 9 */
    virtual void VSlot10() __asm__("func_00155D90");   /* slot 10 */
};
typedef char assert_BHV181_size[sizeof(BHV181) == 0x60 ? 1 : -1];

/* 117 CID117 (0x70): stimulus template (radius, height, ramp); vtable 0x2CE2F8, 48 slots */
struct CID117 : LO
{
    char pad50[0x70 - 0x50];
    virtual CLASSINFO *Pclassinfo() __asm__("GetCid117ClassDesc");   /* slot 6 */
    virtual void Init() __asm__("func_001564E8");   /* slot 7 */
};
typedef char assert_CID117_size[sizeof(CID117) == 0x70 ? 1 : -1];

/* 47 CID47 (0x110): stimulus listener; vtable 0x2CE118, 50 slots */
struct CID47 : XFM
{
    char padE0[0x110 - 0xE0];
    virtual CLASSINFO *Pclassinfo() __asm__("GetCid47ClassDesc");   /* slot 6 */
    virtual void Init() __asm__("func_00156CC8");   /* slot 7 */
    virtual void AddHierarchy() __asm__("func_00156D20");   /* slot 11 */
    virtual void RemoveHierarchy() __asm__("func_00156D58");   /* slot 12 */
    virtual void VUnk15() __asm__("func_00156EB8");   /* slot 16 */
    virtual void Clone(LO *ploBase) __asm__("func_00156D98");   /* slot 41 */
};
typedef char assert_CID47_size[sizeof(CID47) == 0x110 ? 1 : -1];

/* 31 EXPLG (0xF0); vtable 0x2CE828, 51 slots */
struct EXPLG : EXPL
{
    char padE0[0xF0 - 0xE0];
    virtual CLASSINFO *Pclassinfo() __asm__("GetExplgClassDesc");   /* slot 6 */
    virtual void LoadFromBrx(CBinaryInputStream *pbis) __asm__("LoadExplgFromBrx");   /* slot 14 */
    virtual int CploChildren(LO ***paplo) __asm__("CploExplgChildren");   /* slot 28 */
    virtual void Clone(LO *ploBase) __asm__("CloneExplg");   /* slot 41 */
    virtual void VSlot51() __asm__("ExplodeExplgExplso");   /* slot 51 */
};
typedef char assert_EXPLG_size[sizeof(EXPLG) == 0xF0 ? 1 : -1];

/* 33 EXPLS (0x110); vtable 0x2CE4D0, 52 slots */
struct EXPLS : EXPLO
{
    char padF0[0x110 - 0xF0];
    virtual CLASSINFO *Pclassinfo() __asm__("GetExplsClassDesc");   /* slot 6 */
    virtual void HandleMessage(MSGID msgid, void *pv) __asm__("HandleExplsMessage");   /* slot 5 */
    virtual void VSlot51() __asm__("ExplodeExplsExplso");   /* slot 51 */
    virtual void VSlot52() __asm__("FireExplsExplso");   /* slot 52 */
};
typedef char assert_EXPLS_size[sizeof(EXPLS) == 0x110 ? 1 : -1];

/* 131 WIDGET131 (0x140): WIDGET: episode menu; vtable 0x2CEEB8, 50 slots */
struct WIDGET131 : WIDGET
{
    char pad110[0x140 - 0x110];
    virtual CLASSINFO *Pclassinfo() __asm__("GetWidget131ClassDesc");   /* slot 6 */
    virtual void Init() __asm__("func_0015CD68");   /* slot 7 */
    virtual void VUnk14() __asm__("func_0015CDA8");   /* slot 15 */
    virtual void VUnk15() __asm__("func_0015CF30");   /* slot 16 */
};
typedef char assert_WIDGET131_size[sizeof(WIDGET131) == 0x140 ? 1 : -1];

/* 116 CID116 (0x70): screen tint / fill (render kinds 14 and 1); vtable 0x2CF058, 48 slots */
struct CID116 : LO
{
    char pad50[0x70 - 0x50];
    virtual CLASSINFO *Pclassinfo() __asm__("GetCid116ClassDesc");   /* slot 6 */
    virtual void Init() __asm__("func_0015DC80");   /* slot 7 */
    virtual void VUnk15() __asm__("func_0015DCD8");   /* slot 16 */
    virtual void RenderSelf() __asm__("func_0015DD48");   /* slot 19 */
};
typedef char assert_CID116_size[sizeof(CID116) == 0x70 ? 1 : -1];

/* 57 GOMER (0x1010): instances are swarmers; vtable 0x2CF520, 89 slots */
struct GOMER : STEPGUARD
{
    virtual CLASSINFO *Pclassinfo() __asm__("GetGomerClassDesc");   /* slot 6 */
    virtual void Init() __asm__("RenderGomerSelf__FP5GOMERP2CMP2RO");   /* slot 7 */
};
typedef char assert_GOMER_size[sizeof(GOMER) == 0x1010 ? 1 : -1];

/* 113 CID113 (0x60): guard script; vtable 0x2CF880, 48 slots */
struct CID113 : LO
{
    char pad50[0x60 - 0x50];
    virtual CLASSINFO *Pclassinfo() __asm__("GetCid113ClassDesc");   /* slot 6 */
    virtual void Init() __asm__("func_001709F8");   /* slot 7 */
    virtual void VUnk14() __asm__("func_00170A28");   /* slot 15 */
};
typedef char assert_CID113_size[sizeof(CID113) == 0x60 ? 1 : -1];

/* 169 BHV169 (0x60): guard-script runner (flag 0x21); vtable 0x2CF7F8, 15 slots */
struct BHV169 : BHV
{
    char pad30[0x60 - 0x30];
    virtual CLASSINFO *Pclassinfo() __asm__("GetBhv169ClassDesc");   /* slot 6 */
    virtual void OnPush() __asm__("func_00171228");   /* slot 7 */
    virtual void VSlot9() __asm__("func_00171398");   /* slot 9 */
    virtual void VSlot10() __asm__("func_001713E8");   /* slot 10 */
    virtual void VSlot15() __asm__("func_001715F0");   /* slot 15 */
};
typedef char assert_BHV169_size[sizeof(BHV169) == 0x60 ? 1 : -1];

/* 171 BHV171 (0xE0): suspicious/search mode (flag 0x1F); vtable 0x2CFA10, 14 slots */
struct BHV171 : BHV
{
    char pad30[0xE0 - 0x30];
    virtual CLASSINFO *Pclassinfo() __asm__("GetBhv171ClassDesc");   /* slot 6 */
    virtual void OnPush() __asm__("func_00171F58");   /* slot 7 */
    virtual void Start() __asm__("func_00171FA0");   /* slot 8 */
    virtual void VSlot9() __asm__("func_00171FD0");   /* slot 9 */
    virtual void VSlot10() __asm__("func_00172018");   /* slot 10 */
};
typedef char assert_BHV171_size[sizeof(BHV171) == 0xE0 ? 1 : -1];

/* 119 CID119 (0xB0): off-screen guard spawner; vtable 0x2CFA90, 48 slots */
struct CID119 : LO
{
    char pad50[0xB0 - 0x50];
    virtual CLASSINFO *Pclassinfo() __asm__("GetCid119ClassDesc");   /* slot 6 */
    virtual void Init() __asm__("func_00173158");   /* slot 7 */
    virtual void RemoveHierarchy() __asm__("func_001746B8");   /* slot 12 */
    virtual void LoadFromBrx(CBinaryInputStream *pbis) __asm__("func_001731B8");   /* slot 14 */
    virtual void VUnk14() __asm__("func_001733F8");   /* slot 15 */
    virtual void VUnk15() __asm__("func_00173A68");   /* slot 16 */
};
typedef char assert_CID119_size[sizeof(CID119) == 0xB0 ? 1 : -1];

/* 157 BHVANIM (0x50): animation/locomotion leaf (flag 9); vtable 0x2D0AB0, 14 slots */
struct BHVANIM : BHV
{
    char pad30[0x50 - 0x30];
    virtual void VSlot9() __asm__("CloseBhvanim");   /* slot 9 */
    virtual CLASSINFO *Pclassinfo() __asm__("GetBhvanimClassDesc");   /* slot 6 */
    virtual void OnPush() __asm__("InitBhvanim");   /* slot 7 */
    virtual void Start() __asm__("StartBhvanim");   /* slot 8 */
    virtual void VSlot10() __asm__("UpdateBhvanim");   /* slot 10 */
    virtual void VSlot11() __asm__("UpdateBhvanimMotion");   /* slot 11 */
    virtual void VSlot12() __asm__("BHVANIM_GrfGroundProbe");   /* slot 12 */
    virtual void VSlot13() __asm__("BHVANIM_FHandleMessage");   /* slot 13 */
};
typedef char assert_BHVANIM_size[sizeof(BHVANIM) == 0x50 ? 1 : -1];

/* 158 BHVIDLE (0x50): idle leaf (flag 2); vtable 0x2D0A30, 14 slots */
struct BHVIDLE : BHVANIM
{
    virtual void OnPush() __asm__("InitBhvidle");   /* slot 7 */
    virtual CLASSINFO *Pclassinfo() __asm__("GetBhvidleClassDesc");   /* slot 6 */
    virtual void Start() __asm__("StartBhvidle");   /* slot 8 */
    virtual void VSlot9() __asm__("CloseBhvidle");   /* slot 9 */
    virtual void VSlot13() __asm__("BHVIDLE_FHandleMessage");   /* slot 13 */
};
typedef char assert_BHVIDLE_size[sizeof(BHVIDLE) == 0x50 ? 1 : -1];

/* 160 BHVHIT (0xC0): hit reaction; vtable 0x2D09B0, 14 slots */
struct BHVHIT : BHVANIM
{
    char pad50[0xC0 - 0x50];
    virtual void VSlot9() __asm__("CloseBhvhit");   /* slot 9 */
    virtual CLASSINFO *Pclassinfo() __asm__("GetBhvhitClassDesc");   /* slot 6 */
    virtual void Start() __asm__("StartBhvhit");   /* slot 8 */
    virtual void VSlot10() __asm__("UpdateBhvhit");   /* slot 10 */
};
typedef char assert_BHVHIT_size[sizeof(BHVHIT) == 0xC0 ? 1 : -1];

/* 161 BHV161 (0xC0); vtable 0x2D0930, 14 slots */
struct BHV161 : BHVHIT
{
    virtual void OnPush() __asm__("func_001796A0");   /* slot 7 */
    virtual CLASSINFO *Pclassinfo() __asm__("GetBhv161ClassDesc");   /* slot 6 */
    virtual void Start() __asm__("func_001796D0");   /* slot 8 */
};
typedef char assert_BHV161_size[sizeof(BHV161) == 0xC0 ? 1 : -1];

/* 163 BHVKO (0xC0): KO / dead (flag 7); vtable 0x2D08B0, 14 slots */
struct BHVKO : BHVHIT
{
    virtual void VSlot9() __asm__("CloseBhvko");   /* slot 9 */
    virtual CLASSINFO *Pclassinfo() __asm__("GetBhvkoClassDesc");   /* slot 6 */
    virtual void OnPush() __asm__("InitBhvko");   /* slot 7 */
    virtual void Start() __asm__("StartBhvko");   /* slot 8 */
};
typedef char assert_BHVKO_size[sizeof(BHVKO) == 0xC0 ? 1 : -1];

/* 162 BHV162 (0xC0); vtable 0x2D0830, 14 slots */
struct BHV162 : BHVHIT
{
    virtual void OnPush() __asm__("func_00179750");   /* slot 7 */
    virtual CLASSINFO *Pclassinfo() __asm__("GetBhv162ClassDesc");   /* slot 6 */
    virtual void Start() __asm__("func_00179780");   /* slot 8 */
    virtual void VSlot9() __asm__("func_00179820");   /* slot 9 */
};
typedef char assert_BHV162_size[sizeof(BHV162) == 0xC0 ? 1 : -1];

/* 159 BHVSTRIKE (0x60): attack-animation leaf (flag 3); vtable 0x2D07B0, 14 slots */
struct BHVSTRIKE : BHVANIM
{
    char pad50[0x60 - 0x50];
    virtual void Start() __asm__("StartBhvstrike");   /* slot 8 */
    virtual CLASSINFO *Pclassinfo() __asm__("GetBhvstrikeClassDesc");   /* slot 6 */
    virtual void OnPush() __asm__("InitBhvstrike");   /* slot 7 */
    virtual void VSlot9() __asm__("CloseBhvstrike");   /* slot 9 */
    virtual void VSlot10() __asm__("UpdateBhvstrike");   /* slot 10 */
    virtual void VSlot13() __asm__("BHVSTRIKE_FHandleMessage");   /* slot 13 */
};
typedef char assert_BHVSTRIKE_size[sizeof(BHVSTRIKE) == 0x60 ? 1 : -1];

/* 166 BHVPATH (0xC0): path base; vtable 0x2D0728, 15 slots */
struct BHVPATH : BHV
{
    char pad30[0xC0 - 0x30];
    virtual void OnPush() __asm__("InitBhvpath");   /* slot 7 */
    virtual CLASSINFO *Pclassinfo() __asm__("GetBhvpathClassDesc");   /* slot 6 */
    virtual void Start() __asm__("StartBhvpath");   /* slot 8 */
    virtual void VSlot10() __asm__("UpdateBhvpath");   /* slot 10 */
    virtual void VSlot15() __asm__("SetBhvpathState");   /* slot 15 */
};
typedef char assert_BHVPATH_size[sizeof(BHVPATH) == 0xC0 ? 1 : -1];

/* 168 BHVWALKGOAL (0xE0): player walk-to-goal (writes POCTL stick); vtable 0x2CC4C0, 15 slots */
struct BHVWALKGOAL : BHVPATH
{
    char padC0[0xE0 - 0xC0];
    virtual void OnPush() __asm__("InitBhvwalkgoal");   /* slot 7 */
    virtual CLASSINFO *Pclassinfo() __asm__("GetBhvwalkgoalClassDesc");   /* slot 6 */
    virtual void Start() __asm__("StartBhvwalkgoal");   /* slot 8 */
    virtual void VSlot10() __asm__("UpdateBhvwalkgoal");   /* slot 10 */
    virtual void VSlot14() __asm__("UpdateBhvwalkgoalPoctl");   /* slot 14 */
};
typedef char assert_BHVWALKGOAL_size[sizeof(BHVWALKGOAL) == 0xE0 ? 1 : -1];

/* 193 BHV193 (0x50): player scripted go-to; vtable 0x2CC440, 14 slots */
struct BHV193 : BHV
{
    char pad30[0x50 - 0x30];
    virtual void OnPush() __asm__("func_00144E20");   /* slot 7 */
    virtual CLASSINFO *Pclassinfo() __asm__("GetBhv193ClassDesc");   /* slot 6 */
    virtual void Start() __asm__("func_00144E68");   /* slot 8 */
};
typedef char assert_BHV193_size[sizeof(BHV193) == 0x50 ? 1 : -1];

/* 194 BHVFOLLOW (0xA0): player follow leader (StartStepplayerFollow); vtable 0x2CC3C0, 14 slots */
struct BHVFOLLOW : BHV
{
    char pad30[0xA0 - 0x30];
    virtual void Start() __asm__("StartBhvfollow");   /* slot 8 */
    virtual CLASSINFO *Pclassinfo() __asm__("GetBhvfollowClassDesc");   /* slot 6 */
    virtual void OnPush() __asm__("InitBhvfollow");   /* slot 7 */
    virtual void VSlot9() __asm__("CloseBhvfollow");   /* slot 9 */
    virtual void VSlot10() __asm__("UpdateBhvfollow");   /* slot 10 */
};
typedef char assert_BHVFOLLOW_size[sizeof(BHVFOLLOW) == 0xA0 ? 1 : -1];

/* 195 BHV195 (0x40); vtable 0x2CC340, 14 slots */
struct BHV195 : BHV
{
    char pad30[0x40 - 0x30];
    virtual void VSlot9() __asm__("func_00145D68");   /* slot 9 */
    virtual CLASSINFO *Pclassinfo() __asm__("GetBhv195ClassDesc");   /* slot 6 */
    virtual void OnPush() __asm__("func_00145CD0");   /* slot 7 */
    virtual void Start() __asm__("func_00145D00");   /* slot 8 */
    virtual void VSlot10() __asm__("func_00145DB0");   /* slot 10 */
    virtual void VSlot14() __asm__("func_00146068");   /* slot 14 */
};
typedef char assert_BHV195_size[sizeof(BHV195) == 0x40 ? 1 : -1];

/* 196 BHV196 (0x40); vtable 0x2CC2C0, 14 slots */
struct BHV196 : BHV
{
    char pad30[0x40 - 0x30];
    virtual void VSlot9() __asm__("func_00146260");   /* slot 9 */
    virtual CLASSINFO *Pclassinfo() __asm__("GetBhv196ClassDesc");   /* slot 6 */
    virtual void OnPush() __asm__("func_001461C8");   /* slot 7 */
    virtual void Start() __asm__("func_001461F8");   /* slot 8 */
    virtual void VSlot10() __asm__("func_001462A8");   /* slot 10 */
    virtual void VSlot14() __asm__("func_00146560");   /* slot 14 */
};
typedef char assert_BHV196_size[sizeof(BHV196) == 0x40 ? 1 : -1];

/* 197 BHV197 (0x40); vtable 0x2CC240, 14 slots */
struct BHV197 : BHV
{
    char pad30[0x40 - 0x30];
    virtual void VSlot9() __asm__("func_00146758");   /* slot 9 */
    virtual CLASSINFO *Pclassinfo() __asm__("GetBhv197ClassDesc");   /* slot 6 */
    virtual void OnPush() __asm__("func_001466C0");   /* slot 7 */
    virtual void Start() __asm__("func_001466F0");   /* slot 8 */
    virtual void VSlot10() __asm__("func_001467A0");   /* slot 10 */
    virtual void VSlot14() __asm__("func_00146B18");   /* slot 14 */
};
typedef char assert_BHV197_size[sizeof(BHV197) == 0x40 ? 1 : -1];

/* 198 BHV198 (0x40): Murray behaviour; vtable 0x2CC1C0, 14 slots */
struct BHV198 : BHV
{
    char pad30[0x40 - 0x30];
    virtual void Start() __asm__("func_00146E98");   /* slot 8 */
    virtual CLASSINFO *Pclassinfo() __asm__("GetBhv198ClassDesc");   /* slot 6 */
    virtual void VSlot9() __asm__("func_00146EE0");   /* slot 9 */
    virtual void VSlot10() __asm__("func_00146F18");   /* slot 10 */
};
typedef char assert_BHV198_size[sizeof(BHV198) == 0x40 ? 1 : -1];

/* 167 BHVWALKPATH (0xF0): path/goal follower; vtable 0x2D06A0, 15 slots */
struct BHVWALKPATH : BHVPATH
{
    char padC0[0xF0 - 0xC0];
    virtual void OnPush() __asm__("InitBhvwalkpath");   /* slot 7 */
    virtual CLASSINFO *Pclassinfo() __asm__("GetBhvwalkpathClassDesc");   /* slot 6 */
    virtual void VSlot10() __asm__("UpdateBhvwalkpath");   /* slot 10 */
    virtual void VSlot15() __asm__("SetBhvwalkpathState");   /* slot 15 */
};
typedef char assert_BHVWALKPATH_size[sizeof(BHVWALKPATH) == 0xF0 ? 1 : -1];

/* 164 BHVATTACK (0x50): attack execution (flag 0x18); vtable 0x2D0620, 14 slots */
struct BHVATTACK : BHV
{
    char pad30[0x50 - 0x30];
    virtual void Start() __asm__("StartBhvattack");   /* slot 8 */
    virtual CLASSINFO *Pclassinfo() __asm__("GetBhvattackClassDesc");   /* slot 6 */
    virtual void OnPush() __asm__("InitBhvattack");   /* slot 7 */
    virtual void VSlot9() __asm__("CloseBhvattack");   /* slot 9 */
    virtual void VSlot10() __asm__("UpdateBhvattack");   /* slot 10 */
    virtual void VSlot13() __asm__("BHVATTACK_FHandleMessage");   /* slot 13 */
};
typedef char assert_BHVATTACK_size[sizeof(BHVATTACK) == 0x50 ? 1 : -1];

/* 165 BHVCOMBAT (0x60): alert/combat mode (flag 0x1D); vtable 0x2D05A0, 14 slots */
struct BHVCOMBAT : BHV
{
    char pad30[0x60 - 0x30];
    virtual void VSlot9() __asm__("CloseBhvcombat");   /* slot 9 */
    virtual CLASSINFO *Pclassinfo() __asm__("GetBhvcombatClassDesc");   /* slot 6 */
    virtual void OnPush() __asm__("InitBhvcombat");   /* slot 7 */
    virtual void Start() __asm__("StartBhvcombat");   /* slot 8 */
    virtual void VSlot10() __asm__("UpdateBhvcombat");   /* slot 10 */
};
typedef char assert_BHVCOMBAT_size[sizeof(BHVCOMBAT) == 0x60 ? 1 : -1];

/* 170 BHVPATROL (0x50): calm/patrol mode (flag 0x1E); vtable 0x2D0520, 14 slots */
struct BHVPATROL : BHV
{
    char pad30[0x50 - 0x30];
    virtual void VSlot9() __asm__("CloseBhvpatrol");   /* slot 9 */
    virtual CLASSINFO *Pclassinfo() __asm__("GetBhvpatrolClassDesc");   /* slot 6 */
    virtual void OnPush() __asm__("InitBhvpatrol");   /* slot 7 */
    virtual void Start() __asm__("StartBhvpatrol");   /* slot 8 */
    virtual void VSlot10() __asm__("UpdateBhvpatrol");   /* slot 10 */
};
typedef char assert_BHVPATROL_size[sizeof(BHVPATROL) == 0x50 ? 1 : -1];

/* 156 BHVGUARD (0xB0): guard root: awareness mode +0x30; vtable 0x2D04A0, 14 slots */
struct BHVGUARD : BHV
{
    BHVGUARDREC rec;
    char padA0[0xB0 - 0xA0];
    virtual void OnPush() __asm__("InitBhvguard");   /* slot 7 */
    virtual CLASSINFO *Pclassinfo() __asm__("GetBhvguardClassDesc");   /* slot 6 */
    virtual void Start() __asm__("StartBhvguard");   /* slot 8 */
    virtual void VSlot10() __asm__("UpdateBhvguard");   /* slot 10 */
    virtual void VSlot13() __asm__("BHVGUARD_FHandleMessage");   /* slot 13 */
};
typedef char assert_BHVGUARD_size[sizeof(BHVGUARD) == 0xB0 ? 1 : -1];

/* 176 BHV176 (0x40); vtable 0x2D0420, 14 slots */
struct BHV176 : BHV
{
    char pad30[0x40 - 0x30];
    virtual void VSlot9() __asm__("func_0017A7B8");   /* slot 9 */
    virtual CLASSINFO *Pclassinfo() __asm__("GetBhv176ClassDesc");   /* slot 6 */
    virtual void OnPush() __asm__("func_0017A6F8");   /* slot 7 */
    virtual void Start() __asm__("func_0017A730");   /* slot 8 */
    virtual void VSlot10() __asm__("func_0017A7F8");   /* slot 10 */
};
typedef char assert_BHV176_size[sizeof(BHV176) == 0x40 ? 1 : -1];

/* 177 BHVJUGGLED (0x70): juggled / carried (flag 0x17); vtable 0x2D03A0, 14 slots */
struct BHVJUGGLED : BHV
{
    char pad30[0x70 - 0x30];
    virtual void VSlot9() __asm__("CloseBhvjuggled");   /* slot 9 */
    virtual CLASSINFO *Pclassinfo() __asm__("GetBhvjuggledClassDesc");   /* slot 6 */
    virtual void OnPush() __asm__("InitBhvjuggled");   /* slot 7 */
    virtual void Start() __asm__("StartBhvjuggled");   /* slot 8 */
    virtual void VSlot10() __asm__("UpdateBhvjuggled");   /* slot 10 */
    virtual void VSlot11() __asm__("UpdateBhvanimMotion");   /* slot 11 */
    virtual void VSlot12() __asm__("BHVJUGGLED_GrfGroundProbe");   /* slot 12 */
    virtual void VSlot13() __asm__("BHVJUGGLED_FHandleMessage");   /* slot 13 */
};
typedef char assert_BHVJUGGLED_size[sizeof(BHVJUGGLED) == 0x70 ? 1 : -1];

/* 172 BHVLEAP (0xA0): leap (flag 0xF); vtable 0x2D0320, 14 slots */
struct BHVLEAP : BHV
{
    char pad30[0xA0 - 0x30];
    virtual void VSlot9() __asm__("CloseBhvleap");   /* slot 9 */
    virtual CLASSINFO *Pclassinfo() __asm__("GetBhvleapClassDesc");   /* slot 6 */
    virtual void OnPush() __asm__("InitBhvleap");   /* slot 7 */
    virtual void Start() __asm__("StartBhvleap");   /* slot 8 */
    virtual void VSlot10() __asm__("UpdateBhvleap");   /* slot 10 */
    virtual void VSlot11() __asm__("UpdateBhvanimMotion");   /* slot 11 */
    virtual void VSlot12() __asm__("BHVANIM_GrfGroundProbe");   /* slot 12 */
    virtual void VSlot13() __asm__("BHVANIM_FHandleMessage");   /* slot 13 */
};
typedef char assert_BHVLEAP_size[sizeof(BHVLEAP) == 0xA0 ? 1 : -1];

/* 175 BHV175 (0x60); vtable 0x2D02A0, 14 slots */
struct BHV175 : BHV
{
    char pad30[0x60 - 0x30];
    virtual void OnPush() __asm__("func_0017A4B0");   /* slot 7 */
    virtual CLASSINFO *Pclassinfo() __asm__("GetBhv175ClassDesc");   /* slot 6 */
    virtual void Start() __asm__("func_0017A4E8");   /* slot 8 */
    virtual void VSlot9() __asm__("func_0017A558");   /* slot 9 */
    virtual void VSlot10() __asm__("func_0017A670");   /* slot 10 */
    virtual void VSlot11() __asm__("UpdateBhvanimMotion");   /* slot 11 */
    virtual void VSlot12() __asm__("BHVANIM_GrfGroundProbe");   /* slot 12 */
    virtual void VSlot13() __asm__("BHVANIM_FHandleMessage");   /* slot 13 */
};
typedef char assert_BHV175_size[sizeof(BHV175) == 0x60 ? 1 : -1];

/* 173 BHVSHOOT (0x40): SHOOT: ranged attack (BHVSHOOT); vtable 0x2D0220, 14 slots */
struct BHVSHOOT : BHV
{
    char pad30[0x40 - 0x30];
    virtual void Start() __asm__("StartBhvshoot");   /* slot 8 */
    virtual CLASSINFO *Pclassinfo() __asm__("GetBhvshootClassDesc");   /* slot 6 */
    virtual void OnPush() __asm__("InitBhvshoot");   /* slot 7 */
    virtual void VSlot9() __asm__("CloseBhvshoot");   /* slot 9 */
    virtual void VSlot10() __asm__("UpdateBhvshoot");   /* slot 10 */
    virtual void VSlot13() __asm__("BHVSHOOT_FHandleMessage");   /* slot 13 */
};
typedef char assert_BHVSHOOT_size[sizeof(BHVSHOOT) == 0x40 ? 1 : -1];

/* 178 BHV178 (0x50); vtable 0x2D01A0, 14 slots */
struct BHV178 : BHV
{
    char pad30[0x50 - 0x30];
    virtual void Start() __asm__("func_0017B840");   /* slot 8 */
    virtual CLASSINFO *Pclassinfo() __asm__("GetBhv178ClassDesc");   /* slot 6 */
    virtual void OnPush() __asm__("func_0017B808");   /* slot 7 */
    virtual void VSlot9() __asm__("func_0017B870");   /* slot 9 */
    virtual void VSlot10() __asm__("func_0017BD78");   /* slot 10 */
};
typedef char assert_BHV178_size[sizeof(BHV178) == 0x50 ? 1 : -1];

/* 179 BHV179 (0x40); vtable 0x2D0120, 14 slots */
struct BHV179 : BHV
{
    char pad30[0x40 - 0x30];
    virtual void VSlot9() __asm__("func_0017BFB8");   /* slot 9 */
    virtual CLASSINFO *Pclassinfo() __asm__("GetBhv179ClassDesc");   /* slot 6 */
    virtual void OnPush() __asm__("func_0017BED0");   /* slot 7 */
    virtual void Start() __asm__("func_0017BF08");   /* slot 8 */
    virtual void VSlot10() __asm__("func_0017C3E8");   /* slot 10 */
};
typedef char assert_BHV179_size[sizeof(BHV179) == 0x40 ? 1 : -1];

/* 183 BHV183 (0x70); vtable 0x2D00A0, 14 slots */
struct BHV183 : BHV
{
    char pad30[0x70 - 0x30];
    virtual void Start() __asm__("func_0017C6A0");   /* slot 8 */
    virtual CLASSINFO *Pclassinfo() __asm__("GetBhv183ClassDesc");   /* slot 6 */
    virtual void OnPush() __asm__("func_0017C638");   /* slot 7 */
    virtual void VSlot9() __asm__("func_0017C670");   /* slot 9 */
    virtual void VSlot10() __asm__("func_0017C6D0");   /* slot 10 */
};
typedef char assert_BHV183_size[sizeof(BHV183) == 0x70 ? 1 : -1];

/* 184 BHVRAIL (0x40): rail traversal; vtable 0x2D0020, 14 slots */
struct BHVRAIL : BHV
{
    char pad30[0x40 - 0x30];
    virtual void Start() __asm__("StartBhvrail");   /* slot 8 */
    virtual CLASSINFO *Pclassinfo() __asm__("GetBhvrailClassDesc");   /* slot 6 */
    virtual void OnPush() __asm__("InitBhvrail");   /* slot 7 */
    virtual void VSlot9() __asm__("CloseBhvrail");   /* slot 9 */
    virtual void VSlot10() __asm__("UpdateBhvrail");   /* slot 10 */
};
typedef char assert_BHVRAIL_size[sizeof(BHVRAIL) == 0x40 ? 1 : -1];

/* 185 BHVPIPE (0x40): pipe traversal; vtable 0x2CFFA0, 14 slots */
struct BHVPIPE : BHV
{
    char pad30[0x40 - 0x30];
    virtual void Start() __asm__("StartBhvpipe");   /* slot 8 */
    virtual CLASSINFO *Pclassinfo() __asm__("GetBhvpipeClassDesc");   /* slot 6 */
    virtual void OnPush() __asm__("InitBhvpipe");   /* slot 7 */
    virtual void VSlot9() __asm__("CloseBhvpipe");   /* slot 9 */
    virtual void VSlot10() __asm__("UpdateBhvpipe");   /* slot 10 */
};
typedef char assert_BHVPIPE_size[sizeof(BHVPIPE) == 0x40 ? 1 : -1];

/* 187 BHV187 (0x30); vtable 0x2CFF20, 14 slots */
struct BHV187 : BHV
{
    virtual void OnPush() __asm__("func_0017D1C0");   /* slot 7 */
    virtual CLASSINFO *Pclassinfo() __asm__("GetBhv187ClassDesc");   /* slot 6 */
    virtual void Start() __asm__("func_0017D1F0");   /* slot 8 */
    virtual void VSlot9() __asm__("func_0017D280");   /* slot 9 */
};
typedef char assert_BHV187_size[sizeof(BHV187) == 0x30 ? 1 : -1];

/* 188 BHV188 (0x30); vtable 0x2CFEA0, 14 slots */
struct BHV188 : BHV
{
    virtual void OnPush() __asm__("func_0017D2B0");   /* slot 7 */
    virtual CLASSINFO *Pclassinfo() __asm__("GetBhv188ClassDesc");   /* slot 6 */
    virtual void Start() __asm__("func_0017D2E0");   /* slot 8 */
    virtual void VSlot9() __asm__("func_0017D330");   /* slot 9 */
};
typedef char assert_BHV188_size[sizeof(BHV188) == 0x30 ? 1 : -1];

/* 186 BHVGOTO (0xD0): go-to / look-at tracker (flag 0x1C); vtable 0x2CFE20, 14 slots */
struct BHVGOTO : BHV
{
    char pad30[0x40 - 0x30];
    PATHLINKEND end0;
    PATHLINKEND end1;
    PATHLINKEND end2;
    char padA0[0xD0 - 0xA0];
    virtual void VSlot9() __asm__("CloseBhvgoto");   /* slot 9 */
    virtual CLASSINFO *Pclassinfo() __asm__("GetBhvgotoClassDesc");   /* slot 6 */
    virtual void OnPush() __asm__("InitBhvgoto");   /* slot 7 */
    virtual void Start() __asm__("StartBhvgoto");   /* slot 8 */
    virtual void VSlot10() __asm__("UpdateBhvgoto");   /* slot 10 */
};
typedef char assert_BHVGOTO_size[sizeof(BHVGOTO) == 0xD0 ? 1 : -1];

/* 192 BHVPATROLPATH (0x60): patrol-route walker (flag 0x2E); vtable 0x2CFDA0, 14 slots */
struct BHVPATROLPATH : BHV
{
    char pad30[0x60 - 0x30];
    virtual void VSlot9() __asm__("CloseBhvpatrolpath");   /* slot 9 */
    virtual CLASSINFO *Pclassinfo() __asm__("GetBhvpatrolpathClassDesc");   /* slot 6 */
    virtual void OnPush() __asm__("InitBhvpatrolpath");   /* slot 7 */
    virtual void Start() __asm__("StartBhvpatrolpath");   /* slot 8 */
    virtual void VSlot10() __asm__("UpdateBhvpatrolpath");   /* slot 10 */
};
typedef char assert_BHVPATROLPATH_size[sizeof(BHVPATROLPATH) == 0x60 ? 1 : -1];

/* 189 BHV189 (0x50); vtable 0x2CFD20, 14 slots */
struct BHV189 : BHV
{
    char pad30[0x50 - 0x30];
    virtual void Start() __asm__("func_0017DDA8");   /* slot 8 */
    virtual CLASSINFO *Pclassinfo() __asm__("GetBhv189ClassDesc");   /* slot 6 */
    virtual void OnPush() __asm__("func_0017DD70");   /* slot 7 */
    virtual void VSlot9() __asm__("func_0017DDE8");   /* slot 9 */
    virtual void VSlot10() __asm__("func_0017DE50");   /* slot 10 */
};
typedef char assert_BHV189_size[sizeof(BHV189) == 0x50 ? 1 : -1];

/* 190 BHVPATHLINK (0x50): pathlink traversal (flag 0x2C); vtable 0x2CFCA0, 14 slots */
struct BHVPATHLINK : BHV
{
    char pad30[0x50 - 0x30];
    virtual void Start() __asm__("StartBhvpathlink");   /* slot 8 */
    virtual CLASSINFO *Pclassinfo() __asm__("GetBhvpathlinkClassDesc");   /* slot 6 */
    virtual void OnPush() __asm__("InitBhvpathlink");   /* slot 7 */
    virtual void VSlot9() __asm__("CloseBhvpathlink");   /* slot 9 */
    virtual void VSlot10() __asm__("UpdateBhvpathlink");   /* slot 10 */
};
typedef char assert_BHVPATHLINK_size[sizeof(BHVPATHLINK) == 0x50 ? 1 : -1];

/* 191 BHV191 (0x50); vtable 0x2CFC20, 14 slots */
struct BHV191 : BHV
{
    PATHLINKEND end;
    virtual void Start() __asm__("func_0017E8B8");   /* slot 8 */
    virtual CLASSINFO *Pclassinfo() __asm__("GetBhv191ClassDesc");   /* slot 6 */
    virtual void OnPush() __asm__("func_0017E888");   /* slot 7 */
    virtual void VSlot9() __asm__("func_0017E8E8");   /* slot 9 */
    virtual void VSlot10() __asm__("func_0017E918");   /* slot 10 */
};
typedef char assert_BHV191_size[sizeof(BHV191) == 0x50 ? 1 : -1];

/* 71 HACK (0x1C0): hacking minigame base (damageable PO); vtable 0x2D1878, 83 slots */
struct HACK : PO
{
    char pad180[0x1C0 - 0x180];
    virtual CLASSINFO *Pclassinfo() __asm__("GetHackClassDesc");   /* slot 6 */
    virtual void Init() __asm__("func_0017F090");   /* slot 7 */
    virtual void RemoveHierarchy() __asm__("func_0017F110");   /* slot 12 */
    virtual void VUnk14() __asm__("func_0017F138");   /* slot 15 */
    virtual void VUnk25(LO *plo) __asm__("func_0017F170");   /* slot 26 */
    virtual void AdjustNewXp(XP *pxp, int ixpd) __asm__("func_0017F1A0");   /* slot 55 */
    virtual int FTakeDamage(ZPR *pzpr) __asm__("func_0017F1D0");   /* slot 75 */
    virtual void VSlot82() __asm__("func_0017F5F8");   /* slot 82 */
    virtual void VSlot83() __asm__("func_0017F6B0");   /* slot 83 */
};
typedef char assert_HACK_size[sizeof(HACK) == 0x1C0 ? 1 : -1];

/* 72 HACKSHIP (0x2B0); vtable 0x2D15D0, 83 slots */
struct HACKSHIP : HACK
{
    char pad1C0[0x2A4 - 0x1C0];
    int nUnk2a4;
    char pad2A8[0x2B0 - 0x2A8];
    HACKSHIP() { nUnk2a4 = 0; }
    virtual CLASSINFO *Pclassinfo() __asm__("GetHackshipClassDesc");   /* slot 6 */
    virtual void Init() __asm__("InitHackship");   /* slot 7 */
    virtual void RemoveHierarchy() __asm__("func_0017F740");   /* slot 12 */
    virtual void VUnk14() __asm__("func_0017F850");   /* slot 15 */
    virtual void VUnk15() __asm__("func_0017F930");   /* slot 16 */
    virtual void VUnk69() __asm__("func_0017FB50");   /* slot 70 */
    virtual int FTakeDamage(ZPR *pzpr) __asm__("HACKSHIP_TakeDamage");   /* slot 75 */
    virtual void OnActive(int fActive) __asm__("func_0017F8C8");   /* slot 78 */
    virtual void VSlot83() __asm__("func_0017FCB0");   /* slot 83 */
};
typedef char assert_HACKSHIP_size[sizeof(HACKSHIP) == 0x2B0 ? 1 : -1];

/* 74 HACKBULLET (0x1E0); vtable 0x2D1328, 83 slots */
struct HACKBULLET : HACK
{
    char pad1C0[0x1E0 - 0x1C0];
    virtual CLASSINFO *Pclassinfo() __asm__("GetHackbulletClassDesc");   /* slot 6 */
    virtual void Init() __asm__("func_00180188");   /* slot 7 */
    virtual void VUnk15() __asm__("func_001801B8");   /* slot 16 */
    virtual void AdjustNewXp(XP *pxp, int ixpd) __asm__("func_001803E0");   /* slot 55 */
    virtual int FIgnoreIntersection(SO *psoOther) __asm__("func_001803A8");   /* slot 61 */
};
typedef char assert_HACKBULLET_size[sizeof(HACKBULLET) == 0x1E0 ? 1 : -1];

/* 73 HACKENEMY (0x390); vtable 0x2D1080, 83 slots */
struct HACKENEMY : HACK
{
    char pad1C0[0x390 - 0x1C0];
    virtual CLASSINFO *Pclassinfo() __asm__("GetHackenemyClassDesc");   /* slot 6 */
    virtual void HandleMessage(MSGID msgid, void *pv) __asm__("func_00181060");   /* slot 5 */
    virtual void Init() __asm__("InitHackenemy");   /* slot 7 */
    virtual void RemoveHierarchy() __asm__("func_00180598");   /* slot 12 */
    virtual void VUnk14() __asm__("func_001805C8");   /* slot 15 */
    virtual void VUnk15() __asm__("func_00180668");   /* slot 16 */
    virtual void VUnk25(LO *plo) __asm__("func_00180638");   /* slot 26 */
    virtual void AdjustNewXp(XP *pxp, int ixpd) __asm__("func_00180FF0");   /* slot 55 */
    virtual void VUnk69() __asm__("func_00180EE8");   /* slot 70 */
    virtual int FTakeDamage(ZPR *pzpr) __asm__("func_001811B8");   /* slot 75 */
    virtual void VSlot83() __asm__("func_00181238");   /* slot 83 */
};
typedef char assert_HACKENEMY_size[sizeof(HACKENEMY) == 0x390 ? 1 : -1];

/* 75 HACKLOCK (0x250); vtable 0x2D0DD8, 83 slots */
struct HACKLOCK : HACK
{
    char pad1C0[0x250 - 0x1C0];
    virtual CLASSINFO *Pclassinfo() __asm__("GetHacklockClassDesc");   /* slot 6 */
    virtual void Init() __asm__("InitHacklock");   /* slot 7 */
    virtual void VUnk14() __asm__("func_00181440");   /* slot 15 */
    virtual void VUnk15() __asm__("func_00181478");   /* slot 16 */
    virtual void VSlot83() __asm__("func_00181598");   /* slot 83 */
};
typedef char assert_HACKLOCK_size[sizeof(HACKLOCK) == 0x250 ? 1 : -1];

/* 76 HACKWALL (0x2A0); vtable 0x2D0B30, 83 slots */
struct HACKWALL : HACK
{
    char pad1C0[0x2A0 - 0x1C0];
    virtual CLASSINFO *Pclassinfo() __asm__("GetHackwallClassDesc");   /* slot 6 */
    virtual void Init() __asm__("InitHackwall");   /* slot 7 */
    virtual void VUnk14() __asm__("func_00181840");   /* slot 15 */
    virtual void VUnk15() __asm__("func_00181F60");   /* slot 16 */
    virtual void RenderSelf() __asm__("func_001821C0");   /* slot 19 */
    virtual void VUnk48() __asm__("func_00182780");   /* slot 49 */
    virtual void AdjustNewXp(XP *pxp, int ixpd) __asm__("func_00182700");   /* slot 55 */
    virtual void VUnk59(SOPART *ppart, int n2, int n3, void *pvGeom, XP **ppxpFirst) __asm__("func_001823C8");   /* slot 60 */
    virtual int CpartExtra() __asm__("func_00182778");   /* slot 63 */
    virtual int FTakeDamage(ZPR *pzpr) __asm__("func_001828A8");   /* slot 75 */
    virtual void VSlot82() __asm__("func_001829D8");   /* slot 82 */
    virtual void VSlot83() __asm__("func_001819E0");   /* slot 83 */
};
typedef char assert_HACKWALL_size[sizeof(HACKWALL) == 0x2A0 ? 1 : -1];

/* 12 CID12 (0x1C0): wall grabspot (wall press); vtable 0x2D2738, 50 slots */
struct CID12 : SHAPE, GRABSPOT
{
    char pad1B4[0x1C0 - 0x1B4];
    virtual CLASSINFO *Pclassinfo() __asm__("GetCid12ClassDesc");   /* slot 6 */
    virtual void Init() __asm__("func_00182E80");   /* slot 7 */
    virtual void VUnk14() __asm__("func_00182F28");   /* slot 15 */
    virtual void Freeze() __asm__("func_00182EF0");   /* slot 22 */
    virtual void Unfreeze() __asm__("func_00182EB8");   /* slot 23 */
    virtual void Clone(LO *ploBase) __asm__("func_00182FA0");   /* slot 41 */
    virtual int FGetGrabRange() __asm__("func_00182FE8");   /* GRABSPOT slot 1 (delta -400) */
    virtual void GetClosestParam() __asm__("func_00183040");   /* GRABSPOT slot 2 (delta -400) */
    virtual void GetGrabPos() __asm__("func_001830D8");   /* GRABSPOT slot 3 (delta -400) */
    virtual void GetSparklePos() __asm__("func_00183218");   /* GRABSPOT slot 5 (delta -400) */
    virtual void VtF() __asm__("func_002599F0");   /* GRABSPOT slot 6 (delta -400) */
};
typedef char assert_CID12_size[sizeof(CID12) == 0x1C0 ? 1 : -1];

/* 34 CID34 (0x110): pole (GRABSPOT); vtable 0x2D2558, 50 slots */
struct CID34 : XFM, GRABSPOT
{
    char pad104[0x110 - 0x104];
    virtual CLASSINFO *Pclassinfo() __asm__("GetCid34ClassDesc");   /* slot 6 */
    virtual void Init() __asm__("func_00183340");   /* slot 7 */
    virtual void VUnk14() __asm__("func_001833E0");   /* slot 15 */
    virtual void Freeze() __asm__("func_001833A8");   /* slot 22 */
    virtual void Unfreeze() __asm__("func_00183370");   /* slot 23 */
    virtual void Clone(LO *ploBase) __asm__("func_00183450");   /* slot 41 */
    virtual int FGetGrabRange() __asm__("func_00183498");   /* GRABSPOT slot 1 (delta -224) */
    virtual void GetClosestParam() __asm__("func_001834D0");   /* GRABSPOT slot 2 (delta -224) */
    virtual void GetGrabPos() __asm__("func_00183540");   /* GRABSPOT slot 3 (delta -224) */
    virtual void GetSparklePos() __asm__("func_00183600");   /* GRABSPOT slot 5 (delta -224) */
    virtual void VtF() __asm__("func_00259A00");   /* GRABSPOT slot 6 (delta -224) */
};
typedef char assert_CID34_size[sizeof(CID34) == 0x110 ? 1 : -1];

/* 38 CID38 (0x110); vtable 0x2D2378, 50 slots */
struct CID38 : XFM, GRABSPOT
{
    char pad104[0x110 - 0x104];
    virtual CLASSINFO *Pclassinfo() __asm__("GetCid38ClassDesc");   /* slot 6 */
    virtual void Init() __asm__("func_001836C8");   /* slot 7 */
    virtual void VUnk14() __asm__("func_00183768");   /* slot 15 */
    virtual void Freeze() __asm__("func_00183730");   /* slot 22 */
    virtual void Unfreeze() __asm__("func_001836F8");   /* slot 23 */
    virtual void Clone(LO *ploBase) __asm__("func_001837D0");   /* slot 41 */
    virtual void GetGrabPos() __asm__("func_00183818");   /* GRABSPOT slot 3 (delta -224) */
    virtual void VtF() __asm__("func_00259A10");   /* GRABSPOT slot 6 (delta -224) */
};
typedef char assert_CID38_size[sizeof(CID38) == 0x110 ? 1 : -1];

/* 88 BARREL (0x1A0): BARREL / carryable (REVIEW, medium); vtable 0x2D20D0, 75 slots */
struct BARREL : SO, GRABSPOT
{
    char pad174[0x1A0 - 0x174];
    virtual CLASSINFO *Pclassinfo() __asm__("GetBarrelClassDesc");   /* slot 6 */
    virtual void Init() __asm__("InitBarrel");   /* slot 7 */
    virtual void Freeze() __asm__("OnBarrelRemove");   /* slot 22 */
    virtual void Unfreeze() __asm__("OnBarrelAdd");   /* slot 23 */
    virtual void VUnk25(LO *plo) __asm__("func_00183970");   /* slot 26 */
    virtual void Clone(LO *ploBase) __asm__("CloneBarrel");   /* slot 41 */
    virtual int FIgnoreIntersection(SO *psoOther) __asm__("func_00183A50");   /* slot 61 */
    virtual void VUnk69() __asm__("func_00183B00");   /* slot 70 */
    virtual int FTakeDamage(ZPR *pzpr) __asm__("func_00183E08");   /* slot 75 */
    virtual void VtF() __asm__("func_00259A20");   /* GRABSPOT slot 6 (delta -336) */
};
typedef char assert_BARREL_size[sizeof(BARREL) == 0x1A0 ? 1 : -1];

/* 35 CID35 (0x180): GRABSPOT XFM; mission trigger spot (ApplyTaskStates); vtable 0x2D1EE8, 51 slots */
struct CID35 : XFM, GRABSPOT
{
    char pad104[0x180 - 0x104];
    virtual CLASSINFO *Pclassinfo() __asm__("GetCid35ClassDesc");   /* slot 6 */
    virtual void Init() __asm__("func_00183F08");   /* slot 7 */
    virtual void VUnk14() __asm__("func_00183F50");   /* slot 15 */
    virtual void Freeze() __asm__("func_00183FF0");   /* slot 22 */
    virtual void Unfreeze() __asm__("func_00183FB8");   /* slot 23 */
    virtual void GetGrabPos() __asm__("func_00184208");   /* GRABSPOT slot 3 (delta -224) */
    virtual void AdjustScore() __asm__("func_00184238");   /* GRABSPOT slot 4 (delta -224) */
    virtual void VtF() __asm__("func_00259A30");   /* GRABSPOT slot 6 (delta -224) */
    virtual void VSlot51() __asm__("func_00184130");   /* slot 51 */
};
typedef char assert_CID35_size[sizeof(CID35) == 0x180 ? 1 : -1];

/* 37 CID37 (0x1A0): booty container; vtable 0x2CB9F8, 51 slots */
struct CID37 : CID35
{
    char pad180[0x1A0 - 0x180];
    virtual CLASSINFO *Pclassinfo() __asm__("GetCid37ClassDesc");   /* slot 6 */
    virtual void Init() __asm__("func_0013CEB8");   /* slot 7 */
    virtual void VUnk14() __asm__("func_0013CEF8");   /* slot 15 */
    virtual void Unfreeze() __asm__("func_0013CFD8");   /* slot 23 */
    virtual void VSlot51() __asm__("func_0013D020");   /* slot 51 */
};
typedef char assert_CID37_size[sizeof(CID37) == 0x1A0 ? 1 : -1];

/* 36 CID36 (0x1A0): carry grabspot (child of CID35); vtable 0x2D1D00, 51 slots */
struct CID36 : CID35
{
    char pad180[0x1A0 - 0x180];
    virtual CLASSINFO *Pclassinfo() __asm__("GetCid36ClassDesc");   /* slot 6 */
    virtual void Init() __asm__("func_00184268");   /* slot 7 */
    virtual void VUnk14() __asm__("func_001842B0");   /* slot 15 */
    virtual void VSlot51() __asm__("func_00184330");   /* slot 51 */
};
typedef char assert_CID36_size[sizeof(CID36) == 0x1A0 ? 1 : -1];

/* 27 CID27 (0x160): table grabspot (VOL + GRABSPOT; hide under); vtable 0x2D1B60, 50 slots */
struct CID27 : VOL, GRABSPOT
{
    char pad154[0x160 - 0x154];
    virtual CLASSINFO *Pclassinfo() __asm__("GetCid27ClassDesc");   /* slot 6 */
    virtual void Init() __asm__("func_001843C0");   /* slot 7 */
    virtual void VUnk14() __asm__("func_001843F0");   /* slot 15 */
    virtual void Freeze() __asm__("func_001844D8");   /* slot 22 */
    virtual void Unfreeze() __asm__("func_001844A0");   /* slot 23 */
    virtual void GetGrabPos() __asm__("func_00184510");   /* GRABSPOT slot 3 (delta -304) */
    virtual void GetSparklePos() __asm__("func_001845A0");   /* GRABSPOT slot 5 (delta -304) */
    virtual void VtF() __asm__("func_00259A40");   /* GRABSPOT slot 6 (delta -304) */
};
typedef char assert_CID27_size[sizeof(CID27) == 0x160 ? 1 : -1];

/* 19 HND (0x140): hook; REVIEW proposes HND; vtable 0x2D2978, 50 slots */
struct HND : TARGET, GRABSPOT
{
    char pad124[0x140 - 0x124];
    virtual CLASSINFO *Pclassinfo() __asm__("GetHndClassDesc");   /* slot 6 */
    virtual void Init() __asm__("InitHnd");   /* slot 7 */
    virtual void GetGrabPos() __asm__("func_00184F98");   /* GRABSPOT slot 3 (delta -256) */
    virtual void VtF() __asm__("func_00259A70");   /* GRABSPOT slot 6 (delta -256) */
};
typedef char assert_HND_size[sizeof(HND) == 0x140 ? 1 : -1];

/* 8 CID8 (0x100): mesh surface; vtable 0x2D2CB8, 50 slots */
struct CID8 : XFM
{
    char padE0[0x100 - 0xE0];
    virtual CLASSINFO *Pclassinfo() __asm__("GetCid8ClassDesc");   /* slot 6 */
    virtual void Init() __asm__("func_00185130");   /* slot 7 */
    virtual void LoadFromBrx(CBinaryInputStream *pbis) __asm__("func_00185150");   /* slot 14 */
};
typedef char assert_CID8_size[sizeof(CID8) == 0x100 ? 1 : -1];

/* 9 CID9 (0x120): mesh surface (jump target list); vtable 0x2D2B18, 50 slots */
struct CID9 : CID8
{
    char pad100[0x120 - 0x100];
    virtual CLASSINFO *Pclassinfo() __asm__("GetCid9ClassDesc");   /* slot 6 */
    virtual void Init() __asm__("func_00185BF8");   /* slot 7 */
    virtual void AddHierarchy() __asm__("func_00185C28");   /* slot 11 */
    virtual void RemoveHierarchy() __asm__("func_00185C60");   /* slot 12 */
    virtual void VUnk14() __asm__("func_00185CE0");   /* slot 15 */
    virtual void Clone(LO *ploBase) __asm__("func_00185C98");   /* slot 41 */
};
typedef char assert_CID9_size[sizeof(CID9) == 0x120 ? 1 : -1];

/* 40 CID40 (0xF0): JMT (jump target; none in the Paris hub); vtable 0x2D3200, 50 slots */
struct CID40 : XFM
{
    char padE0[0xF0 - 0xE0];
    virtual CLASSINFO *Pclassinfo() __asm__("GetCid40ClassDesc");   /* slot 6 */
    virtual void Init() __asm__("InitJmt");   /* slot 7 */
    virtual void AddHierarchy() __asm__("AddJmtHierarchy");   /* slot 11 */
    virtual void RemoveHierarchy() __asm__("RemoveJmtHierarchy");   /* slot 12 */
    virtual void Clone(LO *ploBase) __asm__("CloneJmt");   /* slot 41 */
};
typedef char assert_CID40_size[sizeof(CID40) == 0xF0 ? 1 : -1];

/* 4 LIGHT (0x170): relight inputs: 3 qw from +0x120; vtable 0x2D33A0, 50 slots */
struct LIGHT : XFM
{
    char padE0[0x170 - 0xE0];
    virtual CLASSINFO *Pclassinfo() __asm__("GetLightClassDesc");   /* slot 6 */
    virtual void Init() __asm__("InitLight");   /* slot 7 */
    virtual void AddHierarchy() __asm__("AddLightHierarchy");   /* slot 11 */
    virtual void RemoveHierarchy() __asm__("RemoveLightHierarchy");   /* slot 12 */
    virtual void VUnk14() __asm__("PostLightLoad");   /* slot 15 */
    virtual void VUnk16() __asm__("func_001A3460");   /* slot 17 */
    virtual void Freeze() __asm__("OnLightRemove");   /* slot 22 */
    virtual void Unfreeze() __asm__("OnLightAdd");   /* slot 23 */
    virtual void TranslateToPos(qword qwPos) __asm__("TranslateLightToPos");   /* slot 29 */
    virtual void RotateToMat(MATRIX4 *pmatWorld) __asm__("RotateLightToMat");   /* slot 30 */
};
typedef char assert_LIGHT_size[sizeof(LIGHT) == 0x170 ? 1 : -1];

/* 96 CID96 (0x250): mini pigeon/rat critter; vtable 0x2D3838, 75 slots */
struct CID96 : SO
{
    char pad150[0x250 - 0x150];
    virtual CLASSINFO *Pclassinfo() __asm__("GetCid96ClassDesc");   /* slot 6 */
    virtual void HandleMessage(MSGID msgid, void *pv) __asm__("func_001B10D0");   /* slot 5 */
    virtual void Init() __asm__("func_001B0B78");   /* slot 7 */
    virtual void RemoveHierarchy() __asm__("func_001B0C68");   /* slot 12 */
    virtual void LoadFromBrx(CBinaryInputStream *pbis) __asm__("func_001B0CA8");   /* slot 14 */
    virtual void VUnk14() __asm__("func_001B0D08");   /* slot 15 */
    virtual void VUnk15() __asm__("func_001B0E28");   /* slot 16 */
    virtual void VUnk16() __asm__("func_001B1030");   /* slot 17 */
    virtual void RenderSelf() __asm__("func_001B1140");   /* slot 19 */
    virtual void VUnk25(LO *plo) __asm__("func_001B0D60");   /* slot 26 */
    virtual void VUnk69() __asm__("func_001B10B0");   /* slot 70 */
    virtual int FTakeDamage(ZPR *pzpr) __asm__("func_001B0F88");   /* slot 75 */
};
typedef char assert_CID96_size[sizeof(CID96) == 0x250 ? 1 : -1];

/* 80 MISSILE (0x270); vtable 0x2D3FA0, 78 slots */
struct MISSILE : BOMB
{
    char pad210[0x270 - 0x210];
    virtual CLASSINFO *Pclassinfo() __asm__("GetMissileClassDesc");   /* slot 6 */
    virtual void Init() __asm__("InitMissile__FP7MISSILE");   /* slot 7 */
    virtual void RemoveHierarchy() __asm__("OnMissileRemove__FP7MISSILE");   /* slot 12 */
    virtual void LoadFromBrx(CBinaryInputStream *pbis) __asm__("LoadMissileFromBrx__FP7MISSILEP18CBinaryInputStream");   /* slot 14 */
    virtual int FIgnoreIntersection(SO *psoOther) __asm__("FIgnoreMissileIntersection__FP7MISSILEP2SO");   /* slot 61 */
    virtual void VSlot77() __asm__("func_001B1F10");   /* slot 77 */
    virtual void VSlot78() __asm__("FireMissile");   /* slot 78 */
};
typedef char assert_MISSILE_size[sizeof(MISSILE) == 0x270 ? 1 : -1];

/* 81 ACCMISS (0x280): ACCMISS = Bentley crossbow dart; vtable 0x2D3D20, 78 slots */
struct ACCMISS : MISSILE
{
    char pad270[0x280 - 0x270];
    virtual CLASSINFO *Pclassinfo() __asm__("GetAccmissClassDesc");   /* slot 6 */
    virtual void Init() __asm__("InitAccmiss__FP7ACCMISS");   /* slot 7 */
    virtual void VUnk69() __asm__("PresetAccmissAccel");   /* slot 70 */
};
typedef char assert_ACCMISS_size[sizeof(ACCMISS) == 0x280 ? 1 : -1];

/* 82 CID82 (0x290); vtable 0x2D3AA0, 78 slots */
struct CID82 : MISSILE
{
    char pad270[0x290 - 0x270];
    virtual CLASSINFO *Pclassinfo() __asm__("GetCid82ClassDesc");   /* slot 6 */
    virtual void Init() __asm__("InitSplmiss");   /* slot 7 */
    virtual void VUnk69() __asm__("func_001B2468");   /* slot 70 */
    virtual void VSlot78() __asm__("FireSplmiss");   /* slot 78 */
};
typedef char assert_CID82_size[sizeof(CID82) == 0x290 ? 1 : -1];

/* 49 MS (0x150); vtable 0x2D4260, 75 slots */
struct MS : SO
{
    virtual CLASSINFO *Pclassinfo() __asm__("GetMsClassDesc");   /* slot 6 */
    virtual void Init() __asm__("InitMs");   /* slot 7 */
    virtual void RenderSelf() __asm__("RenderMsGlobset");   /* slot 19 */
};
typedef char assert_MS_size[sizeof(MS) == 0x150 ? 1 : -1];

/* 174 BHV174 (0x40): NEELA watch/alarm; vtable 0x2D4AF8, 14 slots */
struct BHV174 : BHV
{
    char pad30[0x40 - 0x30];
    virtual CLASSINFO *Pclassinfo() __asm__("GetBhv174ClassDesc");   /* slot 6 */
    virtual void OnPush() __asm__("func_001BA560");   /* slot 7 */
    virtual void Start() __asm__("func_001BA598");   /* slot 8 */
    virtual void VSlot9() __asm__("func_001BA5C8");   /* slot 9 */
    virtual void VSlot10() __asm__("func_001BA608");   /* slot 10 */
};
typedef char assert_BHV174_size[sizeof(BHV174) == 0x40 ? 1 : -1];

/* 60 NEELA (0x10A0); vtable 0x2D4820, 89 slots */
struct NEELA : STEPGUARD
{
    char pad1010[0x10A0 - 0x1010];
    virtual CLASSINFO *Pclassinfo() __asm__("GetNeelaClassDesc");   /* slot 6 */
    virtual void Init() __asm__("InitNeela");   /* slot 7 */
    virtual void VUnk14() __asm__("PostNeelaLoad");   /* slot 15 */
    virtual void VUnk15() __asm__("UpdateNeela");   /* slot 16 */
    virtual void VUnk25(LO *plo) __asm__("func_001BA0B8");   /* slot 26 */
};
typedef char assert_NEELA_size[sizeof(NEELA) == 0x10A0 ? 1 : -1];

/* 98 CID98 (0x180): breakable props (chairs, barrels, crates, containers); vtable 0x2D4B78, 75 slots */
struct CID98 : SO
{
    char pad150[0x180 - 0x150];
    virtual CLASSINFO *Pclassinfo() __asm__("GetCid98ClassDesc");   /* slot 6 */
    virtual void Init() __asm__("func_001BAB50");   /* slot 7 */
    virtual void LoadFromBrx(CBinaryInputStream *pbis) __asm__("func_001BABA0");   /* slot 14 */
    virtual void Freeze() __asm__("func_001BAD80");   /* slot 22 */
    virtual void Unfreeze() __asm__("func_001BADB8");   /* slot 23 */
    virtual void VUnk25(LO *plo) __asm__("func_001BBEA8");   /* slot 26 */
    virtual void VUnk57() __asm__("func_001BB250");   /* slot 58 */
    virtual int FTakeDamage(ZPR *pzpr) __asm__("func_001BADF0");   /* slot 75 */
};
typedef char assert_CID98_size[sizeof(CID98) == 0x180 ? 1 : -1];

/* 14 CID14 (0x1F0): convex polygon SHAPE / pathzone obstacle; vtable 0x2D4DE0, 50 slots */
struct CID14 : SHAPE
{
    char pad190[0x1F0 - 0x190];
    virtual CLASSINFO *Pclassinfo() __asm__("GetCid14ClassDesc");   /* slot 6 */
    virtual void Init() __asm__("func_001BC050");   /* slot 7 */
    virtual void AddHierarchy() __asm__("func_001BC160");   /* slot 11 */
    virtual void RemoveHierarchy() __asm__("func_001BC198");   /* slot 12 */
    virtual void LoadFromBrx(CBinaryInputStream *pbis) __asm__("func_001BC088");   /* slot 14 */
    virtual void VUnk14() __asm__("func_001BC570");   /* slot 15 */
    virtual void Clone(LO *ploBase) __asm__("func_001BC0D0");   /* slot 41 */
};
typedef char assert_CID14_size[sizeof(CID14) == 0x1F0 ? 1 : -1];

/* 45 CID45 (0x130): render-to-texture owner (item preview?); vtable 0x2D4F80, 50 slots */
struct CID45 : XFM
{
    char padE0[0x130 - 0xE0];
    virtual CLASSINFO *Pclassinfo() __asm__("GetCid45ClassDesc");   /* slot 6 */
    virtual void Init() __asm__("func_001BEA40");   /* slot 7 */
    virtual void LoadFromBrx(CBinaryInputStream *pbis) __asm__("func_001BEA88");   /* slot 14 */
    virtual void RenderSelf() __asm__("func_001BEE60");   /* slot 19 */
};
typedef char assert_CID45_size[sizeof(CID45) == 0x130 ? 1 : -1];

/* 17 PATHZONE (0x130): *_pz; vtable 0x2D52B0, 50 slots */
struct PATHZONE : XFM
{
    char padE0[0x130 - 0xE0];
    virtual CLASSINFO *Pclassinfo() __asm__("GetPathzoneClassDesc");   /* slot 6 */
    virtual void Init() __asm__("InitPathzone");   /* slot 7 */
    virtual void LoadFromBrx(CBinaryInputStream *pbis) __asm__("LoadPathzoneFromBrx");   /* slot 14 */
};
typedef char assert_PATHZONE_size[sizeof(PATHZONE) == 0x130 ? 1 : -1];

/* 124 PATHLINK (0x160): BHVPATHLINK walks it; REVIEW; vtable 0x2D5120, 48 slots */
struct PATHLINK : LO
{
    char pad50[0x60 - 0x50];
    PATHLINKEND aend[6];
    char pad120[0x160 - 0x120];
    virtual CLASSINFO *Pclassinfo() __asm__("GetPathlinkClassDesc");   /* slot 6 */
    virtual void Init() __asm__("InitPathlink");   /* slot 7 */
    virtual void AddHierarchy() __asm__("AddPathlinkHierarchy");   /* slot 11 */
    virtual void RemoveHierarchy() __asm__("RemovePathlinkHierarchy");   /* slot 12 */
    virtual void VUnk14() __asm__("PostPathlinkLoad");   /* slot 15 */
};
typedef char assert_PATHLINK_size[sizeof(PATHLINK) == 0x160 ? 1 : -1];

/* 99 CID99 (0x170): pickpocket dispenser / guard pouch; vtable 0x2D5450, 75 slots */
struct CID99 : SO
{
    char pad150[0x170 - 0x150];
    virtual CLASSINFO *Pclassinfo() __asm__("GetCid99ClassDesc");   /* slot 6 */
    virtual void VUnk14() __asm__("func_001C2D20");   /* slot 15 */
    virtual void RenderSelf() __asm__("func_001C2D70");   /* slot 19 */
};
typedef char assert_CID99_size[sizeof(CID99) == 0x170 ? 1 : -1];

/* 11 PIPE (0x1E0): lamp posts; JTPIPE; vtable 0x2D56F8, 50 slots */
struct PIPE : SHAPE, GRABSPOT
{
    char pad1B4[0x1E0 - 0x1B4];
    virtual CLASSINFO *Pclassinfo() __asm__("GetPipeClassDesc");   /* slot 6 */
    virtual void Init() __asm__("InitPipe");   /* slot 7 */
    virtual void VUnk14() __asm__("PostPipeLoad");   /* slot 15 */
    virtual void Freeze() __asm__("OnPipeRemove");   /* slot 22 */
    virtual void Unfreeze() __asm__("OnPipeAdd");   /* slot 23 */
    virtual void Clone(LO *ploBase) __asm__("ClonePipe");   /* slot 41 */
    virtual int FGetGrabRange() __asm__("func_001C3BB0");   /* GRABSPOT slot 1 (delta -400) */
    virtual void GetClosestParam() __asm__("func_001C3958");   /* GRABSPOT slot 2 (delta -400) */
    virtual void GetGrabPos() __asm__("func_001C3A08");   /* GRABSPOT slot 3 (delta -400) */
    virtual void GetSparklePos() __asm__("func_001C3A88");   /* GRABSPOT slot 5 (delta -400) */
    virtual void VtF() __asm__("func_00259AF8");   /* GRABSPOT slot 6 (delta -400) */
};
typedef char assert_PIPE_size[sizeof(PIPE) == 0x1E0 ? 1 : -1];

/* 91 COIN (0x240): COIN pool; vtable 0x2D6928, 76 slots */
struct COIN : DPRIZE
{
    char pad230[0x240 - 0x230];
    virtual CLASSINFO *Pclassinfo() __asm__("GetCoinClassDesc");   /* slot 6 */
    virtual void Init() __asm__("func_001C6508");   /* slot 7 */
    virtual void VUnk15() __asm__("func_001C6588");   /* slot 16 */
    virtual void VSlot76() __asm__("func_001C65F8");   /* slot 76 */
};
typedef char assert_COIN_size[sizeof(COIN) == 0x240 ? 1 : -1];

/* 93 MEDICINE (0x250): health refill; vtable 0x2D66B8, 76 slots */
struct MEDICINE : DPRIZE
{
    char pad230[0x250 - 0x230];
    virtual CLASSINFO *Pclassinfo() __asm__("GetMedicineClassDesc");   /* slot 6 */
    virtual void Init() __asm__("func_001C67D8");   /* slot 7 */
    virtual void VUnk15() __asm__("func_001C6880");   /* slot 16 */
    virtual void VSlot76() __asm__("func_001C68F0");   /* slot 76 */
};
typedef char assert_MEDICINE_size[sizeof(MEDICINE) == 0x250 ? 1 : -1];

/* 92 KEY (0x240); vtable 0x2D6448, 76 slots */
struct KEY : DPRIZE
{
    char pad230[0x240 - 0x230];
    virtual CLASSINFO *Pclassinfo() __asm__("GetKeyClassDesc");   /* slot 6 */
    virtual void Init() __asm__("func_001C6AB8");   /* slot 7 */
    virtual void VSlot76() __asm__("func_001C6B28");   /* slot 76 */
};
typedef char assert_KEY_size[sizeof(KEY) == 0x240 ? 1 : -1];

/* 94 LOOT (0x250): pickpocket item; vtable 0x2D61D8, 76 slots */
struct LOOT : DPRIZE
{
    char pad230[0x250 - 0x230];
    virtual CLASSINFO *Pclassinfo() __asm__("GetLootClassDesc");   /* slot 6 */
    virtual void Init() __asm__("InitLoot");   /* slot 7 */
    virtual void LoadFromBrx(CBinaryInputStream *pbis) __asm__("LoadLootFromBrx");   /* slot 14 */
    virtual void VUnk15() __asm__("func_001C7778");   /* slot 16 */
    virtual void VSlot76() __asm__("SetLootDprizes");   /* slot 76 */
};
typedef char assert_LOOT_size[sizeof(LOOT) == 0x250 ? 1 : -1];

/* 95 CLUE (0x230): clue bottle; vtable 0x2D5F68, 76 slots */
struct CLUE : DPRIZE
{
    virtual CLASSINFO *Pclassinfo() __asm__("GetClueClassDesc");   /* slot 6 */
    virtual void Init() __asm__("InitClue");   /* slot 7 */
    virtual void VUnk25(LO *plo) __asm__("func_001C7E68");   /* slot 26 */
    virtual void VUnk26(LO *plo) __asm__("func_001C7EA8");   /* slot 27 */
    virtual void VSlot76() __asm__("SetClueDprizes");   /* slot 76 */
};
typedef char assert_CLUE_size[sizeof(CLUE) == 0x230 ? 1 : -1];

/* 7 CLUEPROXY (0x110): PROXY child; clue-bottle proxy (REVIEW, medium); vtable 0x2D5DC8, 50 slots */
struct CLUEPROXY : PROXY
{
    char pad100[0x110 - 0x100];
    virtual CLASSINFO *Pclassinfo() __asm__("GetClueproxyClassDesc");   /* slot 6 */
    virtual void VUnk14() __asm__("func_001C8030");   /* slot 15 */
};
typedef char assert_CLUEPROXY_size[sizeof(CLUEPROXY) == 0x110 ? 1 : -1];

/* 78 VAULT (0x230): clue-bottle safe; vtable 0x2D5B30, 81 slots */
struct VAULT : PO
{
    char pad180[0x230 - 0x180];
    virtual CLASSINFO *Pclassinfo() __asm__("GetVaultClassDesc");   /* slot 6 */
    virtual void Init() __asm__("InitVault");   /* slot 7 */
    virtual void VUnk14() __asm__("func_001C82F0");   /* slot 15 */
    virtual void VUnk15() __asm__("func_001C8350");   /* slot 16 */
};
typedef char assert_VAULT_size[sizeof(VAULT) == 0x230 ? 1 : -1];

/* 13 RAIL (0x1D0): wires; vtable 0x2D6FE8, 50 slots */
struct RAIL : SHAPE, GRABSPOT
{
    char pad1B4[0x1D0 - 0x1B4];
    virtual CLASSINFO *Pclassinfo() __asm__("GetRailClassDesc");   /* slot 6 */
    virtual void Init() __asm__("InitRail");   /* slot 7 */
    virtual void VUnk14() __asm__("PostRailLoad");   /* slot 15 */
    virtual void Freeze() __asm__("OnRailRemove");   /* slot 22 */
    virtual void Unfreeze() __asm__("OnRailAdd");   /* slot 23 */
    virtual void Clone(LO *ploBase) __asm__("CloneRail");   /* slot 41 */
    virtual int FGetGrabRange() __asm__("func_001CBA68");   /* GRABSPOT slot 1 (delta -400) */
    virtual void GetClosestParam() __asm__("func_001CBAC0");   /* GRABSPOT slot 2 (delta -400) */
    virtual void GetGrabPos() __asm__("func_001CBB58");   /* GRABSPOT slot 3 (delta -400) */
    virtual void GetSparklePos() __asm__("func_001CBBA8");   /* GRABSPOT slot 5 (delta -400) */
    virtual void VtF() __asm__("func_00259B08");   /* GRABSPOT slot 6 (delta -400) */
};
typedef char assert_RAIL_size[sizeof(RAIL) == 0x1D0 ? 1 : -1];

/* 61 RAJAN (0x1020); vtable 0x2D7188, 90 slots */
struct RAJAN : STEPGUARD
{
    char pad1010[0x1020 - 0x1010];
    virtual CLASSINFO *Pclassinfo() __asm__("GetRajanClassDesc");   /* slot 6 */
    virtual void Init() __asm__("InitRajan");   /* slot 7 */
    virtual void VSlot88() __asm__("func_001CBEE0");   /* slot 89 */
    virtual void VSlot90() __asm__("func_001CBF90");   /* slot 90 */
};
typedef char assert_RAJAN_size[sizeof(RAJAN) == 0x1020 ? 1 : -1];

/* 87 RIPG (0x1B0); vtable 0x2D77D8, 75 slots */
struct RIPG : SO
{
    char pad150[0x1B0 - 0x150];
    virtual CLASSINFO *Pclassinfo() __asm__("GetRipgClassDesc");   /* slot 6 */
    virtual void Init() __asm__("InitRipg__FP4RIPG");   /* slot 7 */
    virtual void RemoveHierarchy() __asm__("OnRipgRemove__FP4RIPG");   /* slot 12 */
    virtual void VUnk15() __asm__("UpdateRipg");   /* slot 16 */
    virtual void VUnk16() __asm__("ProjectRipgTransform");   /* slot 17 */
    virtual void RenderSelf() __asm__("RenderRipgSelf__FP4RIPGP2CMP2RO");   /* slot 19 */
    virtual void VUnk48() __asm__("UpdateRipgBounds");   /* slot 49 */
    virtual int VUnk61() __asm__("func_001CE6C0");   /* slot 62 */
};
typedef char assert_RIPG_size[sizeof(RIPG) == 0x1B0 ? 1 : -1];

/* 77 ROBOTTANK (0x5D0): single instance; vtable 0x2D7CA8, 81 slots */
struct ROBOTTANK : PO
{
    char pad180[0x598 - 0x180];
    int nUnk598;
    char pad59C[0x5D0 - 0x59C];
    ROBOTTANK() { nUnk598 = 0; }
    virtual CLASSINFO *Pclassinfo() __asm__("GetRobottankClassDesc");   /* slot 6 */
    virtual void Init() __asm__("InitRobottank");   /* slot 7 */
    virtual void VUnk14() __asm__("PostRobottankLoad");   /* slot 15 */
    virtual void VUnk15() __asm__("UpdateRobottank");   /* slot 16 */
    virtual void RenderSelf() __asm__("RenderRobottankSelf");   /* slot 19 */
    virtual void VUnk25(LO *plo) __asm__("func_001D1910");   /* slot 26 */
    virtual void Clone(LO *ploBase) __asm__("CloneRobottank");   /* slot 41 */
    virtual void VUnk48() __asm__("UpdateRobottankBounds");   /* slot 49 */
    virtual void VUnk53(SO *pso) __asm__("func_001D3F30");   /* slot 54 */
    virtual void AdjustNewXp(XP *pxp, int ixpd) __asm__("AdjustRobottankNewXp");   /* slot 55 */
    virtual void VUnk59(SOPART *ppart, int n2, int n3, void *pvGeom, XP **ppxpFirst) __asm__("AddRobottankCustomXps");   /* slot 60 */
    virtual void VUnk69() __asm__("PresetRobottankAccel");   /* slot 70 */
    virtual int FTakeDamage(ZPR *pzpr) __asm__("func_001D4568");   /* slot 75 */
    virtual void OnActive(int fActive) __asm__("OnRobottankActive");   /* slot 78 */
};
typedef char assert_ROBOTTANK_size[sizeof(ROBOTTANK) == 0x5D0 ? 1 : -1];

/* 108 CID108 (0x170): robot-tank ammo pickup; vtable 0x2D7A40, 75 slots */
struct CID108 : SO
{
    char pad150[0x170 - 0x150];
    virtual CLASSINFO *Pclassinfo() __asm__("GetCid108ClassDesc");   /* slot 6 */
    virtual void Init() __asm__("InitRcPickup");   /* slot 7 */
    virtual void VUnk14() __asm__("PostRcPickupLoad");   /* slot 15 */
    virtual void VUnk15() __asm__("UpdateRcPickup");   /* slot 16 */
    virtual void VUnk25(LO *plo) __asm__("func_001D4F10");   /* slot 26 */
};
typedef char assert_CID108_size[sizeof(CID108) == 0x170 ? 1 : -1];

/* 112 CID112 (0xC0): aim manager / launcher (darts, guard pebbles, sword missiles); vtable 0x2D7F40, 48 slots */
struct CID112 : LO
{
    char pad50[0xC0 - 0x50];
    virtual CLASSINFO *Pclassinfo() __asm__("GetCid112ClassDesc");   /* slot 6 */
    virtual void Init() __asm__("func_001D6A90");   /* slot 7 */
};
typedef char assert_CID112_size[sizeof(CID112) == 0xC0 ? 1 : -1];

/* 70 SAFEHOUSE (0x240): character-select PO; vtable 0x2D80D0, 81 slots */
struct SAFEHOUSE : PO
{
    char pad180[0x240 - 0x180];
    virtual CLASSINFO *Pclassinfo() __asm__("GetSafehouseClassDesc");   /* slot 6 */
    virtual void Init() __asm__("func_001D7238");   /* slot 7 */
    virtual void VUnk14() __asm__("func_001D7268");   /* slot 15 */
    virtual void VUnk15() __asm__("func_001D72A8");   /* slot 16 */
    virtual void SetCtlState(int nState) __asm__("func_001D7418");   /* slot 79 */
};
typedef char assert_SAFEHOUSE_size[sizeof(SAFEHOUSE) == 0x240 ? 1 : -1];

/* 85 LASEN (0x1D0): LASEN laser beams (zap 0x1000); vtable 0x2D8738, 75 slots */
struct LASEN : SENSOR
{
    char pad190[0x1D0 - 0x190];
    virtual CLASSINFO *Pclassinfo() __asm__("GetLasenClassDesc");   /* slot 6 */
    virtual void Init() __asm__("func_001DE400");   /* slot 7 */
    virtual void LoadFromBrx(CBinaryInputStream *pbis) __asm__("LoadLasenFromBrx");   /* slot 14 */
    virtual void VUnk14() __asm__("PostLasenLoad");   /* slot 15 */
    virtual void VUnk15() __asm__("UpdateLasen");   /* slot 16 */
    virtual void RenderSelf() __asm__("RenderLasenSelf");   /* slot 19 */
    virtual void Clone(LO *ploBase) __asm__("CloneLasen");   /* slot 41 */
    virtual SOTREE *VUnk52() __asm__("func_001DE5A8");   /* slot 53 */
    virtual void VUnk53(SO *pso) __asm__("func_001DEEA8");   /* slot 54 */
    virtual void VUnk59(SOPART *ppart, int n2, int n3, void *pvGeom, XP **ppxpFirst) __asm__("func_001DF0A0");   /* slot 60 */
};
typedef char assert_LASEN_size[sizeof(LASEN) == 0x1D0 ? 1 : -1];

/* 114 RELAY (0xB0): cutscene / sequence script; vtable 0x2D8C48, 48 slots */
struct RELAY : LO
{
    char pad50[0xB0 - 0x50];
    virtual CLASSINFO *Pclassinfo() __asm__("GetRelayClassDesc");   /* slot 6 */
    virtual void Init() __asm__("InitRelay");   /* slot 7 */
    virtual void LoadFromBrx(CBinaryInputStream *pbis) __asm__("LoadRelayFromBrx");   /* slot 14 */
    virtual void VUnk15() __asm__("UpdateRelay");   /* slot 16 */
    virtual void Clone(LO *ploBase) __asm__("CloneRelay");   /* slot 41 */
};
typedef char assert_RELAY_size[sizeof(RELAY) == 0xB0 ? 1 : -1];

/* 106 CID106 (0x160): mesh deformed along a path; vtable 0x2D8DD8, 75 slots */
struct CID106 : SO
{
    char pad150[0x160 - 0x150];
    virtual CLASSINFO *Pclassinfo() __asm__("GetCid106ClassDesc");   /* slot 6 */
    virtual void VUnk14() __asm__("func_001E9858");   /* slot 15 */
    virtual void RenderSelf() __asm__("func_001E9AC0");   /* slot 19 */
};
typedef char assert_CID106_size[sizeof(CID106) == 0x160 ? 1 : -1];

/* 139 LOOP (0x70); vtable 0x2D93C0, 10 slots */
struct LOOP : SAA
{
    char pad50[0x70 - 0x50];
    virtual CLASSINFO *Pclassinfo() __asm__("GetLoopClassDesc");   /* slot 6 */
    virtual void VSlot7() __asm__("InitLoop");   /* slot 7 */
    virtual void VSlot8() __asm__("UpdateLoop__FP4LOOPf");   /* slot 8 */
    virtual void VSlot9() __asm__("UCompleteLoop__FP4LOOP");   /* slot 9 */
};
typedef char assert_LOOP_size[sizeof(LOOP) == 0x70 ? 1 : -1];

/* 140 PINGPONG (0x70); vtable 0x2D9360, 10 slots */
struct PINGPONG : SAA
{
    char pad50[0x70 - 0x50];
    virtual CLASSINFO *Pclassinfo() __asm__("GetPingpongClassDesc");   /* slot 6 */
    virtual void VSlot7() __asm__("InitPingpong");   /* slot 7 */
    virtual void VSlot8() __asm__("UpdatePingpong__FP8PINGPONGf");   /* slot 8 */
    virtual void VSlot9() __asm__("UCompletePingpong__FP8PINGPONG");   /* slot 9 */
};
typedef char assert_PINGPONG_size[sizeof(PINGPONG) == 0x70 ? 1 : -1];

/* 141 SHUFFLE (0x60); vtable 0x2D9300, 10 slots */
struct SHUFFLE : SAA
{
    char pad50[0x60 - 0x50];
    virtual CLASSINFO *Pclassinfo() __asm__("GetShuffleClassDesc");   /* slot 6 */
    virtual void VSlot7() __asm__("InitShuffle__FP7SHUFFLEP4SAAF");   /* slot 7 */
    virtual void VSlot8() __asm__("UpdateShuffle__FP7SHUFFLEf");   /* slot 8 */
};
typedef char assert_SHUFFLE_size[sizeof(SHUFFLE) == 0x60 ? 1 : -1];

/* 142 EYES (0x80); vtable 0x2D92A0, 10 slots */
struct EYES : SAA
{
    char pad50[0x80 - 0x50];
    virtual CLASSINFO *Pclassinfo() __asm__("GetEyesClassDesc");   /* slot 6 */
    virtual void VSlot7() __asm__("InitEyes");   /* slot 7 */
    virtual void VSlot8() __asm__("UpdateEyes");   /* slot 8 */
    virtual void VSlot10() __asm__("func_001EE420");   /* slot 10 */
};
typedef char assert_EYES_size[sizeof(EYES) == 0x80 ? 1 : -1];

/* 143 SCROLLER (0x70); vtable 0x2D9240, 10 slots */
struct SCROLLER : SAA
{
    char pad50[0x70 - 0x50];
    virtual CLASSINFO *Pclassinfo() __asm__("GetScrollerClassDesc");   /* slot 6 */
    virtual void VSlot7() __asm__("InitScroller__FP8SCROLLERP4SAAF");   /* slot 7 */
    virtual void VSlot8() __asm__("UpdateScroller");   /* slot 8 */
    virtual void VSlot9() __asm__("UCompleteScroller__FP8SCROLLER");   /* slot 9 */
};
typedef char assert_SCROLLER_size[sizeof(SCROLLER) == 0x70 ? 1 : -1];

/* 144 CIRCLER (0x60); vtable 0x2D91E0, 10 slots */
struct CIRCLER : SAA
{
    char pad50[0x60 - 0x50];
    virtual CLASSINFO *Pclassinfo() __asm__("GetCirclerClassDesc");   /* slot 6 */
    virtual void VSlot7() __asm__("InitCircler");   /* slot 7 */
    virtual void VSlot8() __asm__("UpdateCircler__FP7CIRCLERf");   /* slot 8 */
    virtual void VSlot9() __asm__("UCompleteCircler__FP7CIRCLER");   /* slot 9 */
};
typedef char assert_CIRCLER_size[sizeof(CIRCLER) == 0x60 ? 1 : -1];

/* 97 SKY (0x160): sky dome; render kind 1; vtable 0x2D9500, 75 slots */
struct SKY : SO
{
    char pad150[0x160 - 0x150];
    virtual CLASSINFO *Pclassinfo() __asm__("GetSkyClassDesc");   /* slot 6 */
    virtual void VUnk14() __asm__("PostSkyLoad");   /* slot 15 */
    virtual void VUnk15() __asm__("UpdateSky");   /* slot 16 */
};
typedef char assert_SKY_size[sizeof(SKY) == 0x160 ? 1 : -1];

/* 39 CID39 (0x110): GRABSPOT XFM, list D_003E13D8; vtable 0x2D9DD0, 50 slots */
struct CID39 : XFM, GRABSPOT
{
    char pad104[0x110 - 0x104];
    virtual CLASSINFO *Pclassinfo() __asm__("GetCid39ClassDesc");   /* slot 6 */
    virtual void Init() __asm__("func_001FF3B8");   /* slot 7 */
    virtual void VUnk14() __asm__("func_001FF3E8");   /* slot 15 */
    virtual void Freeze() __asm__("func_001FF470");   /* slot 22 */
    virtual void Unfreeze() __asm__("func_001FF438");   /* slot 23 */
    virtual void Clone(LO *ploBase) __asm__("func_001FF4A8");   /* slot 41 */
    virtual void GetGrabPos() __asm__("func_001FF4F0");   /* GRABSPOT slot 3 (delta -224) */
    virtual void VtF() __asm__("func_00259C08");   /* GRABSPOT slot 6 (delta -224) */
};
typedef char assert_CID39_size[sizeof(CID39) == 0x110 ? 1 : -1];

/* 46 CID46 (0x120): recon photo target; vtable 0x2D9F70, 50 slots */
struct CID46 : XFM
{
    char padE0[0x120 - 0xE0];
    virtual CLASSINFO *Pclassinfo() __asm__("GetCid46ClassDesc");   /* slot 6 */
    virtual void HandleMessage(MSGID msgid, void *pv) __asm__("HandleReconTargetMessage");   /* slot 5 */
    virtual void Init() __asm__("InitReconTarget");   /* slot 7 */
    virtual void LoadFromBrx(CBinaryInputStream *pbis) __asm__("LoadReconTargetFromBrx");   /* slot 14 */
    virtual void VUnk14() __asm__("PostReconTargetLoad");   /* slot 15 */
};
typedef char assert_CID46_size[sizeof(CID46) == 0x120 ? 1 : -1];

/* 120 CID120 (0xD0): guard attack definition; vtable 0x2DA840, 48 slots */
struct CID120 : LO
{
    char pad50[0xD0 - 0x50];
    virtual CLASSINFO *Pclassinfo() __asm__("GetCid120ClassDesc");   /* slot 6 */
    virtual void Init() __asm__("func_0020AD38");   /* slot 7 */
    virtual void VUnk14() __asm__("func_0020AE18");   /* slot 15 */
    virtual void Clone(LO *ploBase) __asm__("func_0020ADE8");   /* slot 41 */
};
typedef char assert_CID120_size[sizeof(CID120) == 0xD0 ? 1 : -1];

/* 125 CID125 (0x60): guard manager (oid 0x656); vtable 0x2DA3D8, 48 slots */
struct CID125 : LO
{
    char pad50[0x60 - 0x50];
    virtual CLASSINFO *Pclassinfo() __asm__("GetCid125ClassDesc");   /* slot 6 */
    virtual void Init() __asm__("func_0020B4A8");   /* slot 7 */
    virtual void VUnk14() __asm__("func_0020B500");   /* slot 15 */
    virtual void VUnk15() __asm__("func_0020B590");   /* slot 16 */
};
typedef char assert_CID125_size[sizeof(CID125) == 0x60 ? 1 : -1];

/* 129 WIDGET129 (0x220): WIDGET: objective counter (computed per frame); vtable 0x2DAD90, 50 slots */
struct WIDGET129 : WIDGET
{
    char pad110[0x220 - 0x110];
    virtual CLASSINFO *Pclassinfo() __asm__("GetWidget129ClassDesc");   /* slot 6 */
    virtual void VUnk14() __asm__("func_00211EC0");   /* slot 15 */
    virtual void VUnk15() __asm__("func_00211F90");   /* slot 16 */
};
typedef char assert_WIDGET129_size[sizeof(WIDGET129) == 0x220 ? 1 : -1];

/* 127 WIDGET127 (0x120): WIDGET: modal hands-off box; vtable 0x2DABF0, 50 slots */
struct WIDGET127 : WIDGET
{
    char pad110[0x120 - 0x110];
    virtual CLASSINFO *Pclassinfo() __asm__("GetWidget127ClassDesc");   /* slot 6 */
    virtual void Init() __asm__("func_00211D08");   /* slot 7 */
    virtual void VUnk15() __asm__("func_00211D38");   /* slot 16 */
    virtual void VSlot49() __asm__("func_00211CA8");   /* slot 49 */
};
typedef char assert_WIDGET127_size[sizeof(WIDGET127) == 0x120 ? 1 : -1];

/* 128 WIDGET128 (0x110): WIDGET: job completion banner (pauses clock slot 7); vtable 0x2DAA50, 50 slots */
struct WIDGET128 : WIDGET
{
    virtual CLASSINFO *Pclassinfo() __asm__("GetWidget128ClassDesc");   /* slot 6 */
    virtual void Init() __asm__("func_00211E40");   /* slot 7 */
};
typedef char assert_WIDGET128_size[sizeof(WIDGET128) == 0x110 ? 1 : -1];

/* 105 CID105 (0x1A0): motion trail; vtable 0x2DB0D0, 75 slots */
struct CID105 : SO
{
    char pad150[0x1A0 - 0x150];
    virtual CLASSINFO *Pclassinfo() __asm__("GetCid105ClassDesc");   /* slot 6 */
    virtual void Init() __asm__("InitTrail");   /* slot 7 */
    virtual void AddHierarchy() __asm__("func_0021BBD0");   /* slot 11 */
    virtual void VUnk14() __asm__("PostTrailLoad");   /* slot 15 */
    virtual void VUnk15() __asm__("UpdateTrail");   /* slot 16 */
    virtual void VUnk16() __asm__("func_0021C1C8");   /* slot 17 */
    virtual void RenderSelf() __asm__("func_0021C340");   /* slot 19 */
};
typedef char assert_CID105_size[sizeof(CID105) == 0x1A0 ? 1 : -1];

/* 21 CID21 (0x100): camera-related XFM (task visibility lists); vtable 0x2DB378, 50 slots */
struct CID21 : XFM
{
    char padE0[0x100 - 0xE0];
    virtual CLASSINFO *Pclassinfo() __asm__("GetCid21ClassDesc");   /* slot 6 */
    virtual void Init() __asm__("func_0021D3D8");   /* slot 7 */
    virtual void Freeze() __asm__("func_0021D428");   /* slot 22 */
    virtual void Unfreeze() __asm__("func_0021D460");   /* slot 23 */
    virtual void Clone(LO *ploBase) __asm__("func_0021D498");   /* slot 41 */
};
typedef char assert_CID21_size[sizeof(CID21) == 0x100 ? 1 : -1];

/* 100 CID100 (0x1B0): tossables (alarm clock tossable_distract, music box); vtable 0x2DBB10, 76 slots */
struct CID100 : SO
{
    char pad150[0x1B0 - 0x150];
    virtual CLASSINFO *Pclassinfo() __asm__("GetCid100ClassDesc");   /* slot 6 */
    virtual void HandleMessage(MSGID msgid, void *pv) __asm__("func_002241D8");   /* slot 5 */
    virtual void Init() __asm__("func_00223F70");   /* slot 7 */
    virtual void AddHierarchy() __asm__("func_00224028");   /* slot 11 */
    virtual void RemoveHierarchy() __asm__("func_00224068");   /* slot 12 */
    virtual void LoadFromBrx(CBinaryInputStream *pbis) __asm__("func_00223FE0");   /* slot 14 */
    virtual void VUnk15() __asm__("func_002240B0");   /* slot 16 */
    virtual void AdjustNewXp(XP *pxp, int ixpd) __asm__("func_00224350");   /* slot 55 */
    virtual void VUnk57() __asm__("func_00224380");   /* slot 58 */
    virtual int FIgnoreIntersection(SO *psoOther) __asm__("func_002242E0");   /* slot 61 */
    virtual void VUnk69() __asm__("func_00224698");   /* slot 70 */
    virtual int FTakeDamage(ZPR *pzpr) __asm__("func_00224190");   /* slot 75 */
    virtual void VSlot76() __asm__("func_00224EC8");   /* slot 76 */
};
typedef char assert_CID100_size[sizeof(CID100) == 0x1B0 ? 1 : -1];

/* 101 CID101 (0x1B0): Bentley trigger bomb / mines (gadget object); vtable 0x2C9810, 76 slots */
struct CID101 : CID100
{
    virtual CLASSINFO *Pclassinfo() __asm__("GetCid101ClassDesc");   /* slot 6 */
};
typedef char assert_CID101_size[sizeof(CID101) == 0x1B0 ? 1 : -1];

/* 118 CID118 (0x60); vtable 0x2C94E0, 48 slots */
struct CID118 : LO
{
    char pad50[0x60 - 0x50];
    virtual CLASSINFO *Pclassinfo() __asm__("GetCid118ClassDesc");   /* slot 6 */
};
typedef char assert_CID118_size[sizeof(CID118) == 0x60 ? 1 : -1];

/* 153 JSG (0xB0): relay interpreter; vtable 0x2C94A0, 6 slots */
struct JSG : BASIC
{
    char pad10[0xB0 - 0x10];
    virtual CLASSINFO *Pclassinfo() __asm__("GetJsgClassDesc");   /* slot 6 */
};
typedef char assert_JSG_size[sizeof(JSG) == 0xB0 ? 1 : -1];

/* 62 JEANBISON (0x1010); vtable 0x2C91C8, 89 slots */
struct JEANBISON : STEPGUARD
{
    virtual CLASSINFO *Pclassinfo() __asm__("GetJeanbisonClassDesc");   /* slot 6 */
};
typedef char assert_JEANBISON_size[sizeof(JEANBISON) == 0x1010 ? 1 : -1];

/* 65 ARPEGGIO (0x1010); vtable 0x2C8EF0, 89 slots */
struct ARPEGGIO : STEPGUARD
{
    virtual CLASSINFO *Pclassinfo() __asm__("GetArpeggioClassDesc");   /* slot 6 */
};
typedef char assert_ARPEGGIO_size[sizeof(ARPEGGIO) == 0x1010 ? 1 : -1];

/* 66 BEAR (0x1010); vtable 0x2C8C18, 89 slots */
struct BEAR : STEPGUARD
{
    virtual CLASSINFO *Pclassinfo() __asm__("GetBearClassDesc");   /* slot 6 */
};
typedef char assert_BEAR_size[sizeof(BEAR) == 0x1010 ? 1 : -1];

/* 67 BEARCUB (0x1010): single instance; vtable 0x2C8940, 89 slots */
struct BEARCUB : STEPGUARD
{
    virtual CLASSINFO *Pclassinfo() __asm__("GetBearcubClassDesc");   /* slot 6 */
};
typedef char assert_BEARCUB_size[sizeof(BEARCUB) == 0x1010 ? 1 : -1];

/* 102 CID102 (0x1D0): guard trap (drops a prize); vtable 0x2DB8A0, 76 slots */
struct CID102 : CID100
{
    char pad1B0[0x1D0 - 0x1B0];
    virtual CLASSINFO *Pclassinfo() __asm__("GetCid102ClassDesc");   /* slot 6 */
    virtual void Init() __asm__("func_002252E0");   /* slot 7 */
    virtual void VUnk15() __asm__("func_00225300");   /* slot 16 */
    virtual void VUnk70() __asm__("func_002255C8");   /* slot 71 */
    virtual void VSlot76() __asm__("func_00225660");   /* slot 76 */
};
typedef char assert_CID102_size[sizeof(CID102) == 0x1D0 ? 1 : -1];

/* 69 TURRET (0x1E0): grapple turret; vtable 0x2DBD80, 81 slots */
struct TURRET : PO
{
    char pad180[0x1E0 - 0x180];
    virtual CLASSINFO *Pclassinfo() __asm__("GetTurretClassDesc");   /* slot 6 */
    virtual void Init() __asm__("InitTurret");   /* slot 7 */
    virtual void VUnk14() __asm__("PostTurretLoad");   /* slot 15 */
    virtual void VUnk15() __asm__("UpdateTurret");   /* slot 16 */
    virtual void Clone(LO *ploBase) __asm__("CloneTurret");   /* slot 41 */
    virtual int FIgnoreIntersection(SO *psoOther) __asm__("func_00226310");   /* slot 61 */
    virtual void OnActive(int fActive) __asm__("OnTurretActive");   /* slot 78 */
};
typedef char assert_TURRET_size[sizeof(TURRET) == 0x1E0 ? 1 : -1];

/* 5 CID5 (0x120): vis zone (all *_vz WEs); vtable 0x2DC070, 50 slots */
struct CID5 : XFM
{
    char padE0[0x120 - 0xE0];
    virtual CLASSINFO *Pclassinfo() __asm__("GetCid5ClassDesc");   /* slot 6 */
    virtual void Init() __asm__("func_00227350");   /* slot 7 */
    virtual void LoadFromBrx(CBinaryInputStream *pbis) __asm__("func_00227380");   /* slot 14 */
};
typedef char assert_CID5_size[sizeof(CID5) == 0x120 ? 1 : -1];

/* 23 CID23 (0x140): VOL child; vtable 0x2DC210, 50 slots */
struct CID23 : VOL
{
    char pad130[0x140 - 0x130];
    virtual CLASSINFO *Pclassinfo() __asm__("GetCid23ClassDesc");   /* slot 6 */
    virtual void Freeze() __asm__("func_00227E58");   /* slot 22 */
    virtual void Unfreeze() __asm__("func_00227E20");   /* slot 23 */
};
typedef char assert_CID23_size[sizeof(CID23) == 0x140 ? 1 : -1];

/* 41 CID41 (0xE0): in-level teleport destination (WARP_* strings); vtable 0x2DC890, 50 slots */
struct CID41 : XFM
{
    virtual CLASSINFO *Pclassinfo() __asm__("GetCid41ClassDesc");   /* slot 6 */
    virtual void LoadFromBrx(CBinaryInputStream *pbis) __asm__("func_00228140");   /* slot 14 */
};
typedef char assert_CID41_size[sizeof(CID41) == 0xE0 ? 1 : -1];

/* 26 EXIT (0x190): level exit volume; vtable 0x2DC6F0, 50 slots */
struct EXIT : VOL
{
    char pad130[0x190 - 0x130];
    virtual CLASSINFO *Pclassinfo() __asm__("GetExitClassDesc");   /* slot 6 */
    virtual void Init() __asm__("InitExit");   /* slot 7 */
    virtual void LoadFromBrx(CBinaryInputStream *pbis) __asm__("LoadExitFromBrx");   /* slot 14 */
    virtual void VUnk15() __asm__("UpdateExit");   /* slot 16 */
};
typedef char assert_EXIT_size[sizeof(EXIT) == 0x190 ? 1 : -1];

/* 42 WARP (0x180): destinations; entrance_* aliases; vtable 0x2DC550, 50 slots */
struct WARP : XFM
{
    char padE0[0x180 - 0xE0];
    virtual CLASSINFO *Pclassinfo() __asm__("GetWarpClassDesc");   /* slot 6 */
    virtual void LoadFromBrx(CBinaryInputStream *pbis) __asm__("LoadWarpFromBrx");   /* slot 14 */
};
typedef char assert_WARP_size[sizeof(WARP) == 0x180 ? 1 : -1];

/* 89 WATER (0x190): zap kind 0x2000; vtable 0x2DCA30, 75 slots */
struct WATER : SO
{
    char pad150[0x190 - 0x150];
    virtual CLASSINFO *Pclassinfo() __asm__("GetWaterClassDesc");   /* slot 6 */
    virtual void HandleMessage(MSGID msgid, void *pv) __asm__("func_00229370");   /* slot 5 */
    virtual void Init() __asm__("InitWater");   /* slot 7 */
    virtual void RemoveHierarchy() __asm__("func_00229308");   /* slot 12 */
    virtual void VUnk14() __asm__("PostWaterLoad");   /* slot 15 */
    virtual void VUnk15() __asm__("UpdateWater");   /* slot 16 */
    virtual void VUnk48() __asm__("func_00229680");   /* slot 49 */
    virtual void InitPhys() __asm__("func_00228BB0");   /* slot 64 */
    virtual void VUnk70() __asm__("func_002292D8");   /* slot 71 */
    virtual void VUnk72() __asm__("func_00229D78");   /* slot 73 */
};
typedef char assert_WATER_size[sizeof(WATER) == 0x190 ? 1 : -1];

/* 107 WAYPT (0x190): job marker mission_start_*; vtable 0x2DCC98, 75 slots */
struct WAYPT : SO
{
    char pad150[0x190 - 0x150];
    virtual CLASSINFO *Pclassinfo() __asm__("GetWayptClassDesc");   /* slot 6 */
    virtual void Init() __asm__("InitWaypt");   /* slot 7 */
    virtual void VUnk14() __asm__("PostWayptLoad");   /* slot 15 */
    virtual void VUnk15() __asm__("UpdateWaypt");   /* slot 16 */
    virtual void RenderSelf() __asm__("RenderWaypt");   /* slot 19 */
};
typedef char assert_WAYPT_size[sizeof(WAYPT) == 0x190 ? 1 : -1];

/* 115 WR (0x900); vtable 0x2DCF00, 48 slots */
struct WR : LO
{
    char pad50[0x900 - 0x50];
    virtual CLASSINFO *Pclassinfo() __asm__("GetWrClassDesc");   /* slot 6 */
    virtual void Init() __asm__("InitWr");   /* slot 7 */
    virtual void VUnk15() __asm__("UpdateWrMatrixes__FP2WR");   /* slot 16 */
};
typedef char assert_WR_size[sizeof(WR) == 0x900 ? 1 : -1];

#endif /* SLY2_CLASSES_H */
