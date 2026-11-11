// sw.cpp -- the world (SW, the global g_sw at g_sw): construction, load and delete, the per-frame update,
// hands-off / no-pause / letterbox counters, lighting, the object update list, level and world tables.
// TU 0x20D9D0..0x210EB0 (68 functions, address order).
//
// Types: sly2/sw.h's SW for the m2c bodies (one unnamed word, +0x3EA8, through a macro); level/world table
// records as small TU-private views. The hand-written per-function files keep their views in namespaces named
// after each function (as jt.cpp does), their C++-named definitions as C-linkage definitions of their literal
// symbols there. ResolveSwSaveVars (a Lilly port) is at file scope: it completes types basic.h only
// forward-declares (SVPATH, SAVEDATA, LOUPD...), which sw.h's SW refers to.
// g_sw is defined here as a plain SW (constructor SW_ctor, implicit destructor calling ~BASIC with g++'s in-charge
// flag 2), and D_003E1C20 with an inline constructor: g++ generates the 96-instruction construct-and-destruct
// StaticInit_sw and the _GLOBAL_$D$ stub GlobalDestructors_sw next to the _GLOBAL_$I$ one (as cm.cpp does for g_cm).
// Entry points: SW_ctor (g_sw), ExecuteSwTransition (per frame: level loads), PostSwLoad, DeleteSw,
// the hands-off / letterbox counters, the level and world table queries (GetCurrentWorld, GetWorldEntry...).
// SLY2_ALIAS: __static_initialization_and_destruction_0=StaticInit_sw
// SLY2_ALIAS: _GLOBAL_$I$*=GlobalConstructors_sw
// SLY2_ALIAS: _GLOBAL_$D$*=GlobalDestructors_sw

#include "sly2/basic.h"
#include "sly2/lo.h"
#include "sly2/so.h"
#include "sly2/po.h"
#include "sly2/sw.h"
#include "sly2/cm.h"
#include "sly2/save.h"
#include "sly2/slotheap.h"
#include "sly2/joy.h"
#include "sly2/scrap.h"
#include "sly2/cref.h"
#include "sly2/hud.h"
#include "sly2/clock.h"
#include "sly2/game.h"
#include "sly2/sndmgr.h"
#include "sly2/vtables.h"
#include "sly2/classes.h"
#include "sly2/streambis.h"
#include "sly2/hw.h"
#include "sly2/light.h"
#include "heap.h"

struct LEVELLIST8;
extern "C" {
/* sw's plain .bss words, first declared in address order: g++ 2.95 lays the .bss objects without a constructor out
   in the order of their first declaration (after g_sw and D_003E1C20, which it emits at their definitions). */
extern int D_003E1C30, D_003E1C34, D_003E1C38, D_003E1C3C;
extern FILEREC *D_003E1C40;
extern int *D_003E1C44, *D_003E1C48;
extern LEVELLIST8 *D_003E1C4C;
extern FILEREC *D_003E1C50;
extern int D_003E1C54, D_003E1C58;
SW *SW_ctor(SW *world);
extern char D_002C3EB8[], D_003E1C60[];
}
/* g_sw (sly2/sw.h): its constructor is SW_ctor; its implicit destructor runs ~BASIC (_$_5BASIC). */
SW g_sw __attribute__((aligned(16)));
/* D_003E1C20: four words built at startup (a table header {0, D_002C3EB8, 0, D_003E1C60}). */
struct O1C20
{
    int count0;   /* zeroed by the constructor (a count for table0, inferred) */
    void *table0;
    int count8;   /* likewise for table1 (inferred) */
    void *table1;
    // Initialise the record with its two tables and zero counters.
    O1C20() { count0 = 0; table0 = D_002C3EB8; count8 = 0; table1 = D_003E1C60; }
};
O1C20 D_003E1C20;

// 0x210CF0 StaticInit_sw (g_sw and D_003E1C20, then the six slot pools; g_sw's destruction) and its stubs.
SLY2_INSTANTIATE_SLOT_POOLS()

/* An SW word sw.h leaves unnamed (+0x3EA8, in the padding before slPoctl): set by SetSwUnknown3EA8. */
#define F3EA8(world) (*(int *)((char *)(world) + 0x3EA8))
/* A level-list entry (8 bytes) and a {key, value} table. */
struct LEVELLIST8 { int n0; int n4; };
struct KVE { int key; void *pv; };
struct KVT { int c; KVE *a; };
extern "C" {
extern char D_002E1E60[];   /* the letterbox object */
extern LEVELLIST8 *D_003E1C4C;  /* g_alevellist */
extern FILEREC *D_003E1C50;     /* g_aWorldEntryRecord */
void ShowLetterbox(void *pletterbox);
void HideLetterbox(void *pletterbox);
void UpdateSwIllum(SW *world);
int GetCurrentWorld();
FILEREC *GetWorldEntry(int nWorld);
}
void AddGrfusr(int grf);
void RemoveGrfusr(int grf);
LO *FindSwObject(SW *world, int grffso, OID oid, LO *plo);

namespace M_SW_ctor {
extern float D_002C3E60 __asm__("D_002C3E60"),D_002C3E64 __asm__("D_002C3E64"),D_002C3E68 __asm__("D_002C3E68"),D_002C3E6C __asm__("D_002C3E6C");
extern void InitDl__FP2DLi(DL *,int) __asm__("InitDl__FP2DLi");
extern void InitSl(SL *,int) __asm__("InitSl");
extern int GetMaxRipSize(void) __asm__("GetMaxRipSize");
extern void *func_002333B4(void *,int,unsigned int) __asm__("memset");
/* The world names (g_apchzWorld, the save-variable key of each world's progress), defined oidBuckets: the strings are
   the oidBuckets of sw's .rodata (a definition's literals are emitted with it) and the table lies in the oidBuckets part of
   .rodata, with the objects' named constants in TU order (.rodata.early, tu-merging.md "Phase 6"). Asm-labelled:
   the declaration below (non-const pointers) is what the functions see. */
namespace SW_WORLDS { extern const char *const r_D_002A39E8[9] __asm__("D_002A39E8") __attribute__((section(".rodata.early"))) =
    {"intro", "nightclub", "palace", "temple", "prison", "castle", "train", "sawmill", "blimp"}; }
// SW constructor (the global g_sw): vtable, about 30 slot-heap pools, about 60 lists, the oid/class/proxy hash buckets,
// the default illumination (HSV 180/255/255), fog and field-of-view defaults.
// Runs again in place on every level change.
extern "C" SW *SW_ctor(SW *world)
{
    world->scriptFrame.m_p=0;world->scriptVariables.m_p=0;SLY2_SET_VPTR(world, SW);   /* BASIC's construction as plain stores */
    InitSlotheap(&world->slotheapLoroot,sizeof(LOROOT),0x100,0x10);
    InitSlotheap(&world->slotheapEvent,0x14,0x100,0x10);
    InitSlotheap(&world->slotheapCallback,0x14,0x100,0x10);
    InitSlotheap(&world->slotheapRefnode,0xc,0x200,0x10);
    InitSlotheap(&world->slotheapSotree,sizeof(SOTREE),0x300,0x10);
    InitSlotheap(&world->slotheapXplist,0x14,0x400,0x10);
    InitSlotheap(&world->slotheapXp,0xc0,0x100,0x10);
    InitSlotheap(&world->slotheapAsega,sizeof(ASEGA),0x100,0x10);
    InitSlotheap(&world->slotheapAct,0x80,0x400,0x10);
    InitSlotheap(&world->slotheapOvra,sizeof(OVRA),0x8,0x10);
    InitSlotheap(&world->slotheapSma,sizeof(SMA),0x80,0x10);
    *(int *)&world->rgbaClear=0x80000000;
    InitSlotheap(&world->slotheapSobounds,sizeof(SOBOUNDS),0x40,0x10);
    InitSlotheap(&world->slotheapStimulus,sizeof(STIMULUS),0x40,0x10);
    InitSlotheap(&world->slotheapRipg,sizeof(RIPG),0x10,0x10);
    int chosen=GetMaxRipSize();
    chosen=chosen?chosen:0xF0;
    InitSlotheap(&world->slotheapRip,chosen,0x80,0x10);
    InitSlotheap(&world->slotheapBlipg,sizeof(BLIPG),0x28,0x10);
    InitSlotheap(&world->slotheapBlip,0x10c0,0x80,0x40);
    InitSlotheap(&world->slotheapBlipSpline,0x560,0x8,0x10);
    InitSlotheap(&world->slotheapSsge,0x1c,0x200,0x10);
    InitSlotheap(&world->slotheapJsg,0xf0,0x20,0x10);
    InitSlotheap(&world->slotheapXa,sizeof(XA),0x10,0x10);
    InitSlotheap(&world->slotheapZprHit,sizeof(ZPR),0x20,0x10);
    InitSlotheap(&world->slotheapXfh,0x30,0x10,0x10);
    InitSlotheap(&world->slotheapLeap,0x40,0x10,0x10);
    InitSlotheap(&world->slotheapDiract,0xd0,0x10,0x10);
    InitDl__FP2DLi(&world->dlChild,SLY2_OFFSETOF(LO, siblingLink));
    InitDl__FP2DLi(&world->roots,SLY2_OFFSETOF(LOROOT, rootLink));
    InitDl__FP2DLi(&world->busyRoots,SLY2_OFFSETOF(LOROOT, busyLink));
    InitDl__FP2DLi(&world->dlSoTreeAll,SLY2_OFFSETOF(SOTREE, dleAll));
    InitDl__FP2DLi(&world->dlSoTree,SLY2_OFFSETOF(SOTREE, dleAwake));
    InitDl__FP2DLi(&world->dlSoTreeQueue1,SLY2_OFFSETOF(SOTREE, dleQueue1));
    InitDl__FP2DLi(&world->dlSoTreeQueue2,SLY2_OFFSETOF(SOTREE, dleQueue2));
    InitDl__FP2DLi(&world->dlAsega,SLY2_OFFSETOF(ASEGA, dleSw));
    InitSl(&world->slEvent,0x10);
    InitSl(&world->slCallback,0x10);
    InitDl__FP2DLi(&world->dlTarget,SLY2_OFFSETOF(TARGET, targetListLink));
    InitDl__FP2DLi(&world->dlCid11,0x1B4);
    InitDl__FP2DLi(&world->dlCid13,0x1B4);
    InitDl__FP2DLi(&world->dlCid39,SLY2_OFFSETOF(CID39, dleGrab));
    InitDl__FP2DLi(&world->dlCid88,0x174);
    InitDl__FP2DLi(&world->dlCid12,SLY2_OFFSETOF(CID12, dleGrab));
    InitDl__FP2DLi(&world->dlCid34,SLY2_OFFSETOF(CID34, dleGrab));
    InitDl__FP2DLi(&world->dlCid38,SLY2_OFFSETOF(CID38, dleGrab));
    InitDl__FP2DLi(&world->dlCid35,SLY2_OFFSETOF(CID35, dleGrab));
    InitDl__FP2DLi(&world->dlCid40,0xE0);
    InitDl__FP2DLi(&world->dlCid27,SLY2_OFFSETOF(CID27, dleGrab));
    InitDl__FP2DLi(&world->dlCid9,0x100);
    InitDl__FP2DLi(&world->dlSma,SLY2_OFFSETOF(SMA, dleSw));
    InitDl__FP2DLi(&world->dlStimulus,0x58);
    InitDl__FP2DLi(&world->dlStepguard,SLY2_OFFSETOF(STEPGUARD, dleGuard));
    InitDl__FP2DLi(&world->dlCid98,0x174);
    InitSl(&world->slPoctl,0x84);
    InitSl(&world->slShd,0x18);
    InitSl(&world->slSaa,0x40);
    InitDl__FP2DLi(&world->dlZprHit,SLY2_OFFSETOF(ZPR, dle));
    InitDl__FP2DLi(&world->dlTargetrec,0x20);
    InitDl__FP2DLi(&world->dlCid47,0x104);
    InitDl__FP2DLi(&world->dlCid14Free,0x1D0);
    InitDl__FP2DLi(&world->dlCid14Pending,0x1E0);
    InitDl__FP2DLi(&world->dlTaskActive,SLY2_OFFSETOF(TASK, dleActive));
    InitDl__FP2DLi(&world->dlCid21,SLY2_OFFSETOF(CID21, listLink));
    InitDl__FP2DLi(&world->dlCid23,0x130);
    InitDl__FP2DLi(&world->dlWidgetExclusive,SLY2_OFFSETOF(WIDGET, exclusiveLink));
    InitSl(&world->slWidgetDeferred,0x4C);
    unsigned int i=0;
    for(;i<512;i++)InitSl(&world->objectsByOid[i],SLY2_OFFSETOF(LO, oidHashLink));
    VECTOR4 *hsv=&world->hsvLight;
    LM *range=&world->lmGuardScale;
    CLQ *coefficients=&world->clqGuardScale;
    char *unknownBytes=(char *)&world->lightmaskFree;
    SL *list=world->objectsByClass;
    for(int n=198;n>=0;n--,list++)InitSl(list,SLY2_OFFSETOF(LO, classListLink));
    i=0;
    for(;i<512;i++)InitSl(&world->aslPxrOid[i],SLY2_OFFSETOF(PXR, oidHashLink));
    float upper=D_002C3E6C;
    float lower=D_002C3E68;
    float end=D_002C3E64,start=D_002C3E60;
    world->hsvLight.x=180.0f;
    hsv->y=255.0f;
    hsv->z=255.0f;
    world->loadFilter=-1;
    world->midtone=90.0f;
    world->agShdLight[0]=128.0f;
    world->agShdLight[1]=96.0f;
    world->agShdLight[2]=160.0f;
    world->returnWarpOid=393;
    world->returnWarpContext=-1;
    world->fAaoxUnsorted=1;
    world->lmGuardScale.minValue=upper;
    world->isrcLoading=-2;
    world->shadow=0;
    range->maxValue=lower;
    float slope=(upper-lower)/(end-start);
    float intercept=(end*lower-start*upper)/(end-start);
    world->clqGuardScale.g0=intercept;
    coefficients->g1=slope;
    coefficients->g2=0.0f;
    world->idHintGrab=-1;
    world->tLookLast=-60.0f;
    world->unknown4754=-60.0f;
    world->fLightTreeDirty=1;
    func_002333B4(unknownBytes,-1,0x20);
    return world;
}
} // namespace M_SW_ctor

// Posts a level transition: sets isTransitionPending and copies the request to SW::transition for ExecuteSwTransition.
// Also known as: RestoreChkmgrFromCheckpoint (upstream readable / Sly 1 name; RestoreChkmgrFromCheckpoint__FP6CHKMGR).
extern "C" void SetSwTransition(SW *world, TRANS *request)
{
    world->isTransitionPending = 1;
    world->transition = *request;
}

namespace M_func_0020E040 {
extern int D_00443740 __asm__("D_00443740");
// True while a level transition is pending or the wipe is wiping out (D_00443740 state 1).
extern "C" int IsSwTransitionBusy(void) { if (g_sw.isTransitionPending != 0) return 1; return D_00443740 == 1; }
} // namespace M_func_0020E040

namespace M_DeleteSw {
extern char D_003D9E70[] __asm__("D_003D9E70");

extern int D_002E1E40 __asm__("D_002E1E40"), D_002F7900 __asm__("D_002F7900"), D_002DD5BC __asm__("D_002DD5BC"), D_002DD648 __asm__("D_002DD648"), D_003D4A54 __asm__("D_003D4A54"), D_002DE570 __asm__("D_002DE570"), D_002DFB80 __asm__("D_002DFB80"), g_grfusr __asm__("g_grfusr");
extern int D_003E1C40 __asm__("D_003E1C40"), D_003E1C44 __asm__("D_003E1C44"), D_003E1C48 __asm__("D_003E1C48"), D_003E1C4C __asm__("D_003E1C4C"), D_003E1C50 __asm__("D_003E1C50"), D_003DD48C __asm__("D_003DD48C");
void BroadcastGameEvent(void *p, int a, int b) __asm__("BroadcastGameEvent");
void AbortJob(LO *pjob) __asm__("AbortJob");
void ResetScreens(void *p) __asm__("ResetScreens");
void DeleteSwSplice(SW *psw) __asm__("DeleteSwSplice");
void ClearGameDifficulty(void *p) __asm__("ClearGameDifficulty");
void ResetCameraPolicies(void *p) __asm__("ResetCameraPolicies");
void ResetSndmgr(void *p) __asm__("ResetSndmgr");
void ClearShaderGsState(void) __asm__("ClearShaderGsState");
void SetRumbleRums__FP6RUMBLE4RUMS(RUMBLE *prumble, int rums) __asm__("SetRumbleRums__FP6RUMBLE4RUMS");
void FreeSw__Fv(void) __asm__("FreeSw__Fv");

// Tears down the current world before a load: game callback 1, frees the view and joystick state, resets the sound
// manager, clears about 15 global object pointers and frees the world's memory.
// Also known as: UnloadWorld (upstream readable / Sly 1 name; UnloadWorld__FPv).
extern "C" void DeleteSw(SW *world)
{
    void *pv = &g_game;
    BroadcastGameEvent(pv, 1, 0);
    if (world->ploJobFailed) {
        AbortJob(world->ploJobFailed);
        world->ploJobFailed = 0;
    }
    ResetScreens(D_003D9E70);
    DeleteSwSplice(world);
    ClearGameDifficulty(pv);
    ResetCameraPolicies(&g_cm);
    ResetSndmgr(&g_sndmgr);
    ClearShaderGsState();
    SetRumbleRums__FP6RUMBLE4RUMS(&g_joy.rumble, 3);
    D_002E1E40 = 0;
    D_002F7900 = 0;
    D_002DD5BC = 0;
    D_002DD648 = 0;
    D_003D4A54 = 0;
    D_002DE570 = 0;
    D_002DFB80 = 0;
    g_grfusr = 0;
    D_003E1C40 = 0;
    D_003E1C44 = 0;
    D_003E1C48 = 0;
    D_003E1C4C = 0;
    D_003E1C50 = 0;
    D_003DD48C = 0;
    FreeSw__Fv();
}
} // namespace M_DeleteSw

namespace M_func_0020E180 {
struct ZPR { char ab[0x630]; };
void *AllocFromWorldHeap(struct SWALLOC *pmsg) __asm__("AllocFromWorldHeap");
void *AllocClearedFromWorldHeap(struct SWALLOC *pmsg) __asm__("AllocClearedFromWorldHeap");
void InitSl(void *p, int n) __asm__("InitSl");
void func_001703F8(struct ZPR *pz, int a, int b) __asm__("InitZpr__FP3ZPR3ZPKP2LO");
void InitRenderTargets(void *p, struct ZPR *pz) __asm__("InitRenderTargets");
void BroadcastGameEvent(void *p, int a, int b) __asm__("BroadcastGameEvent");

// Sets up a fresh world for loading: the three AAOX buffers (0x4800 bytes each), the update list (0x8000 bytes), the root
// proxy record, the two render targets (one ZPR init),
// game callback 2.
extern "C" void InitSwLoadBuffers(SW *world)
{
    struct SWALLOC msg;
    struct ZPR zpr;
    int i;
    unsigned int j;
    for (i = 0; i < 3; i++) {
        msg.cb = 0x4800;
        msg.cbAlign = 0x10;
        world->aaaox[i] = (struct AAOX *)AllocFromWorldHeap(&msg);
    }
    msg.cb = 0x8000;
    msg.cbAlign = 0x10;
    world->aloupd = (LOUPD *)AllocClearedFromWorldHeap(&msg);
    msg.cb = 0x28;
    msg.cbAlign = 0x10;
    world->ppxrRoot = (PXR *)AllocClearedFromWorldHeap(&msg);
    world->ppxrRoot->proxyId = -1;
    world->ppxrRoot->proxyOid = -1;
    InitSl(&world->ppxrRoot->children, SLY2_OFFSETOF(PXR, siblingLink));
    func_001703F8(&zpr, 0x1400, 0x1800);
    for (j = 0; j < 2; j++)
        InitRenderTargets(&world->artgt[j], &zpr);
    if (world->transition.flags & 0x20)
        world->unknown3E38 = 1;
    BroadcastGameEvent(&g_game, 2, 0);
}
} // namespace M_func_0020E180

namespace M_func_0020E2B0 {
int FindSwObjectsByClass(SW *p, int a, int b, int c, int cMax, MS **apo) __asm__("FindSwObjectsByClass");
void SetLoVisMask(MS *o, int n) __asm__("SetLoVisMask");
// Picks the main MS object (class 0x31): of up to 16, the one with the most model pieces (GLOBSET::numPieces,
// +0xB8); stores its GLOBSET word +0x48 as SW::pblockRender and makes it visible to every camera mask. (inferred)
// The header names that word GLOBSET::aposeset (POSESET *): LoadPoseSetsFromBrx builds those records, and rendertree.cpp's
// CullSceneRenderList walks the same array as RENDERBLOCKs (pblockRoot), so the two names cover one record type.
extern "C" void SelectSwMainMs(SW *world)
{
    MS *found[16];
    int numFound = FindSwObjectsByClass(world, 5, 0x31, 0, 16, found);
    int maxPriority = 0;
    MS *best = 0;
    int i;
    for (i = 0; i < numFound; i++) {
        if (found[i]->model.numPieces > maxPriority) {
            maxPriority = found[i]->model.numPieces;
            best = found[i];
        }
    }
    if (best) {
        world->pblockRender = (RENDERBLOCK *)best->model.aposeset;
        SetLoVisMask(best, 0xFF);
    }
}
} // namespace M_func_0020E2B0

namespace M_func_0020E338 {
extern CID116 *CreateLo__F3CIDP2SO3OIDi(int a, void *p, int b, int c) __asm__("CreateLo__F3CIDP2SO3OIDi");
// Creates the world's screen tint/fill object (class 0x74, CID116 in docs/engine.md section 7; parameters 0.1 and
// 0.25, hold colour 0x80200000), stores it at SW+0x3E60 and takes it out of the world (Remove). (inferred)
extern "C" void CreateSwScreenTint(SW *world)
{
    world->pcid116Fade = CreateLo__F3CIDP2SO3OIDi(0x74, 0, -1, -1);
    world->pcid116Fade->dtHold = 0.1f;
    world->pcid116Fade->dtFadeOut = 0.25f;
    *(uint *)&world->pcid116Fade->rgbaHold = 0x80200000;   /* the packed colour */
    world->pcid116Fade->renderKind = 1;
    world->pcid116Fade->Remove();   /* slot 9: idle until a fade starts */
}
} // namespace M_func_0020E338

// A Lilly port at file scope: its types complete basic.h forward declarations (used by the m2c bodies below too).
// ResolveSwSaveVars: ported from Lilly's personal Sly 2 decomp (src/P2/sw.c), reduced to the declarations it needs.
// Every global is named by its address; the function keeps its readable name and gets the team symbol through __asm__.
// Globals: g_game, D_002E1E54 (g_nLang), D_003D4A60 (g_savedata), D_002A39E8 (g_apchzWorld)
// 0x0020E3C0 ResolveSwSaveVars__FP2SW  (T1037)
// Once the level file has named its save variables (LoadSaveVarsFromBrx), looks up the paths the game itself reads and writes by name (SW 0x3B1C..:
// the language and sound settings in "flags", the job and world of this level, each world's "dt" and "percent_complete") and stores the current
// language, job and world; "episodeBest" is raised to the current world when that is a new best. Then tells the game (event 3).
#include "sly2/basic.h"
#include "sly2/lo.h"
#include "sly2/sw.h"
/* SVN: sly2/sw.h */
/* SAVEDATA: sly2/save.h */
/* LOUPD: sly2/sw.h */
int GetCurrentLevelIndex();
extern "C" void BroadcastGameEvent(GAME *pgame, int nEvent, LO *plo);
extern int D_002E1E54;
extern const char *D_002A39E8[9];
extern "C" SVPATH *NewSaveKey(const char *pchz, ...);
extern "C" CRef SetGlobalSaveVar(SVPATH *psvp, const CRef &ref, SAVEDATA *saveManager);
// Store a script reference into a global save variable.
inline void SetSaveVarRef(SVPATH *psvp, CRef ref) { SetGlobalSaveVar(psvp, ref, &g_gs); }
// Store an int into a global save variable.
inline void SetSaveVarInt(SVPATH *psvp, int n)
{
    CRef ref(n, 0x15);
    SetSaveVarRef(psvp, ref);
}
extern "C" CRef SetSaveVarByName(const CRef &ref, const char *pchz, ...);
extern "C" int GetCurrentWorld();
extern "C" int GetBestEpisode();
void ResolveSwSaveVars(SW *world) __asm__("ResolveSwSaveVars__FP2SW");
// Looks up the save variables the game reads (language, sound flags, job and world, per-world play time and percent
// complete), stores the current language, job and world, raises episodeBest and notifies the game (event 3).
void ResolveSwSaveVars(SW *world)
{
    world->psvpDt = NewSaveKey("dt", 0);
    world->psvpWidCurrent = NewSaveKey("widCurrent", 0);
    world->psvpEpisodeCurrent = NewSaveKey("episodeCurrent", 0);
    world->psvpCcoin = NewSaveKey("loot", "ccoin", 0);
    world->psvpLang = NewSaveKey("flags", "lang", 0);
    world->psvpMixerMode = NewSaveKey("flags", "nMixerMode", 0);
    world->psvpMusicVolume = NewSaveKey("flags", "uMusicVolume", 0);
    world->psvpEffectsVolume = NewSaveKey("flags", "uEffectsVolume", 0);
    world->psvpUseHeadset = NewSaveKey("flags", "fUseHeadset", 0);
    world->psvpScreenCenterX = NewSaveKey("flags", "dxScreenCenter", 0);
    world->psvpScreenCenterY = NewSaveKey("flags", "dyScreenCenter", 0);
    world->psvpNoVibration = NewSaveKey("flags", "fNoVibration", 0);
    world->psvpReverseLookAround = NewSaveKey("flags", "fReverseLookAround", 0);
    world->psvpReverseAim = NewSaveKey("flags", "fReverseAim", 0);
    world->psvpAutosaveWarned = NewSaveKey("flags", "fAutosaveWarned", 0);
    for (int iworld = 0; iworld < 9; iworld++)
    {
        world->apsvpEpisodeDt[iworld] = NewSaveKey("episode", D_002A39E8[iworld], "dt", 0);
        world->apsvpEpisodePercent[iworld] = NewSaveKey("episode", D_002A39E8[iworld], "percent_complete", 0);
    }
    SetSaveVarInt(world->psvpLang, D_002E1E54);
    SetSaveVarInt(world->psvpWidCurrent, GetCurrentLevelIndex());
    SetSaveVarInt(world->psvpEpisodeCurrent, GetCurrentWorld());
    if (GetCurrentWorld() > GetBestEpisode())
        SetSaveVarByName(CRef(GetCurrentWorld(), 0x15), "episodeBest", 0);
    BroadcastGameEvent(&g_game, 3, 0);
}

namespace M_PostSwLoad {
extern char D_003D9E70[] __asm__("D_003D9E70");
void PrimeSlotheap(void *p) __asm__("PrimeSlotheap");
void CreateSwScreenTint(SW *psw) __asm__("CreateSwScreenTint");
void SelectSwMainMs(SW *psw) __asm__("SelectSwMainMs");
void ClearSwSpliceTable(SW *psw) __asm__("ClearSwSpliceTable");
void InitSwProxySpares(SW *psw) __asm__("InitSwProxySpares");
void BindSwTimerHud(SW *psw) __asm__("BindSwTimerHud");
void CreateSwNoiseTypes(SW *psw) __asm__("CreateSwNoiseTypes");
void EnsureSwDefaultLight(SW *psw) __asm__("EnsureSwDefaultLight");
void PostLoadSwPathzones(SW *psw) __asm__("PostLoadSwPathzones");
void BindSwBootyProxy(SW *psw) __asm__("BindSwBootyProxy");
void PostLoadGrabMarker(SW *psw) __asm__("PostLoadGrabMarker");
void PostLoadScreens(void *p) __asm__("PostLoadScreens");
void ProcessSwCallbacks(SW *psw) __asm__("ProcessSwCallbacks");
void BuildPathzoneLinkLists(void *p) __asm__("BuildPathzoneLinkLists");
void SetupCm__FP2CM(void *p) __asm__("SetupCm__FP2CM");

// After the BRX is read: resets the transient pools, binds the HUD objects, runs the post-load callbacks (slot 15)
// until the queue is empty, resets the camera and runs game callback 4.
// Also known as: StartSw (upstream readable / Sly 1 name; StartSw__FP2SW).
extern "C" void PostSwLoad(SW *world)
{
    LO **ppo;
    PrimeSlotheap(&world->slotheapRipg);
    PrimeSlotheap(&world->slotheapRip);
    PrimeSlotheap(&world->slotheapBlipg);
    PrimeSlotheap(&world->slotheapBlip);
    CreateSwScreenTint(world);
    SelectSwMainMs(world);
    ClearSwSpliceTable(world);
    InitSwProxySpares(world);
    BindSwTimerHud(world);
    CreateSwNoiseTypes(world);
    EnsureSwDefaultLight(world);
    PostLoadSwPathzones(world);
    BindSwBootyProxy(world);
    PostLoadGrabMarker(world);
    PostLoadScreens(D_003D9E70);
    while (ProcessSwCallbacks(world), world->pploNew != world->aploNew) {
        for (ppo = world->aploNew; ppo < world->pploNew; ppo++)
            (*ppo)->PostLoad();
        world->pploNew = world->aploNew;
    }
    world->pploNew = 0;
    world->aploNew = 0;
    world->pvPostLoadPool = 0;
    world->pvLorunPool = 0;
    BuildPathzoneLinkLists(world->pregtree);
    SetupCm__FP2CM(&g_cm);
    BroadcastGameEvent(&g_game, 4, 0);
}
} // namespace M_PostSwLoad

namespace M_ExecuteSwTransition {
// Load the pending world and restore actor and view state.
typedef unsigned int uint;
struct FILEKEY {int name,tag,kind;char filename[0x110 - 0xC];};   /* a file key (FILEKEY: memcard.cpp InitFileKey, sound.cpp K): two key words, the kind, then the file name; no pointers (the earlier pointer slots came from the unrelated FILEKEY class) */
typedef STEPPLAYER ACTOR;   /* the hero (stepplayer.h); jtmode.hud.pwidgetHud (+0xF2C) its HUD widget */
typedef WARP CONTEXT;   /* the destination warp (classes.h WARP) */
struct FRAME;
typedef CStreamBis STREAM;   /* the level file's stream (sly2/streambis.h) */
extern char D_002F7980[] __asm__("D_002F7980"), D_002F66B0[] __asm__("D_002F66B0"), D_00443740[] __asm__("D_00443740"), D_002E1E88[] __asm__("D_002E1E88");
extern ACTOR *D_002E1E40 __asm__("D_002E1E40"),*D_002DD5BC __asm__("D_002DD5BC"),*D_002F7900 __asm__("D_002F7900");

void FlushFrames__Fi(int) __asm__("FlushFrames__Fi");
FRAME *STACKSCOPE_ctor(FRAME *) __asm__("STACKSCOPE_ctor");
void STACKSCOPE_ctor_1AE4A0(FRAME *,int) __asm__("_$_10STACKSCOPE");
void SetPhase__F5PHASE(int) __asm__("SetPhase__F5PHASE");
void InitFileKey(FILEKEY *) __asm__("InitFileKey");
int OpenStreamFromFileKey(STREAM *,FILEKEY *) __asm__("OpenStreamFromFileKey");
void SetDecompress__18CBinaryInputStreamiPUc(STREAM *,int,void *) __asm__("SetDecompress__18CBinaryInputStreamiPUc");
void DeleteSw(SW *) __asm__("DeleteSw");
void ResetClock(void *) __asm__("ResetClock");
void AddGrfusr__Fi(int) __asm__("AddGrfusr__Fi");
void *func_002333B4(void *,int,uint) __asm__("memset");
int CMpeg_ExecuteOids(void *) __asm__("CMpeg_ExecuteOids");
void InitCm__FP2CM(void *) __asm__("InitCm__FP2CM");
void InitSwLoadBuffers(SW *) __asm__("InitSwLoadBuffers");
void LoadSwFromBrx__FP2SWP18CBinaryInputStream(SW *,STREAM *) __asm__("LoadSwFromBrx__FP2SWP18CBinaryInputStream");
void PostSwLoad(SW *) __asm__("PostSwLoad");
void ShowCountdownHud(SW *,float) __asm__("ShowCountdownHud");
void ClearCheatItems(void) __asm__("ClearCheatItems");
CONTEXT *FindWarpByOid(SW *,int,int) __asm__("FindWarpByOid");
void TriggerWarp(CONTEXT *) __asm__("TriggerWarp");
ACTOR *GetHeroPo(SW *) __asm__("GetHeroPo");
ACTOR *GetCurrentPo(SW *) __asm__("GetCurrentPo");
ACTOR *FindSwObject__FP2SWi3OIDP2LO(SW *,int,int,void *) __asm__("FindSwObject__FP2SWi3OIDP2LO");
int IsBasicDerivedFrom__FP5BASIC3CID(ACTOR *,int) __asm__("IsBasicDerivedFrom__FP5BASIC3CID");
int GetPoCtlState(ACTOR *) __asm__("GetPoCtlState");
int GetCurrentWorld(void) __asm__("GetCurrentWorld");
void SetWidgetState(WIDGET *,int) __asm__("SetWidgetState");
void UpdateCmLast__FP2CM(void *) __asm__("UpdateCmLast__FP2CM");
void CutCm__FP2CM(void *) __asm__("CutCm__FP2CM");
void BroadcastGameEvent(void *,int,void *) __asm__("BroadcastGameEvent");
void ClearPhase(int) __asm__("ClearPhase");
void ResumeWipeAfterLoad(void *) __asm__("ResumeWipeAfterLoad");
void RunNextStartupStep(void *) __asm__("RunNextStartupStep");

// Empty scope object (its constructor and destructor only shape the generated code).
struct FRAME {FRAME(){}~FRAME(){}};
extern CRef GetSaveVarByName(const char *,...) __asm__("GetSaveVarByName");
// The frame clock's current time.
static inline float Time(){return g_clock.time;}
// True when two file headers name the same file (same pointer, or equal key words 2 ^ 14).
static inline int Same(const FILEREC *a,const FILEREC *b){if(a==b)return true;if(!b)return false;return (a->aw[2]^a->aw[14])==(b->aw[2]^b->aw[14]);}
// Build the file decryption key from its header (kind 2, name = (w0 ^ w13) + 0xCA0, tag = w1 ^ w15); returns whether
// it is usable (tag != 0).
static inline int SetKey(FILEKEY *key,const FILEREC *header){if(!header)return false;int c=header->aw[0],a=header->aw[13],d=header->aw[15],b=header->aw[1];key->kind=2;key->tag=b^d;key->name=(c^a)+0xCA0;return key->tag!=0;}
// Call the actor's activate virtual (slot 8).
static inline void Activate(ACTOR *actor){actor->Add();}
// Call the actor's virtual slot 79 with mode.
static inline void Enable(ACTOR *actor,int mode){actor->SetCtlState(mode);}
// Attach the actor to the context (virtual slot 38).
static inline void Attach(ACTOR *actor,CONTEXT *context){actor->MatchTransform((LO *)context);}
// Call the actor's deactivate virtual (slot 9).
static inline void Hide(ACTOR *actor){actor->Remove();}
// v != 0 (sltu); the stream record's address, in $4. The asm only pins the instructions for the match.
#ifdef SLY2_EE_ASM
static inline int NonZero(int v){int r; __asm__("sltu %0,$zero,%1":"=r"(r):"r"(v));return r;}
static inline STREAM *StreamHome(STREAM *home){register STREAM *result __asm__("$4");__asm__("la %0,%a1":"=r"(result):"p"(home));return result;}
#else
static inline int NonZero(int v){return v!=0;}
static inline STREAM *StreamHome(STREAM *home){return home;}
#endif
// Runs a pending level transition: opens the level file, deletes the old world, constructs a new g_sw, loads it from
// the BRX (LoadSwFromBrx, PostSwLoad), restores the checkpoint and the current actor, sets up the camera and runs game
// callback 5. Called once per frame by the main loop.
// Also known as: PswUpdateTransition (upstream readable / Sly 1 name; PswUpdateTransition__FP2SW).
extern "C" void ExecuteSwTransition(SW *world)
{
    if(!world->isTransitionPending)return;
    FlushFrames__Fi(1);
    TRANS request=world->transition;
    TRANS *destination=&world->transition;
    world->isTransitionPending=0;
    float resume;
    if(request.flags&0xCD)resume=0.0f;
    else{float remaining=world->hudTimerEndTime-Time();resume=remaining>0.0f?remaining:0.0f;}
    FRAME frame;
    FRAME *scope=&frame;
    STACKSCOPE_ctor(scope);
    SetPhase__F5PHASE(2);
    FILEKEY key;
    FILEKEY *keyPointer=&key;
    keyPointer->kind=2;
    InitFileKey(keyPointer);
    int hasFile=NonZero(request.destinationLevel.aw[1]^request.destinationLevel.aw[15]);
    if(hasFile&&!SetKey(&key,&request.destinationLevel)){STACKSCOPE_ctor_1AE4A0(scope,2);return;}
    FILEREC *current=&world->currentLevel; const FILEREC *requested=&request.destinationLevel; int changed=!Same(requested,current);
    CStreamBisStorage stream;   /* constructed and destroyed through the C entry points */
    register STREAM *argument __asm__("$4")=StreamHome((STREAM *)&stream);
    register STREAM *reader __asm__("$21");
#ifdef SLY2_EE_ASM
    __asm__("daddu %0,%1,$zero":"=r"(reader):"r"(argument));   /* a copy the compiler cannot fold */
#else
    reader=argument;
#endif
    CStreamBis_ctor(argument,0xD8000,D_002F7980,0);
    if(hasFile&&!OpenStreamFromFileKey(reader,&key)){DestroyCStreamBis(reader,2);STACKSCOPE_ctor_1AE4A0(scope,2);return;}
    SetDecompress__18CBinaryInputStreamiPUc(reader,0x4000,(void *)SLY2_HW_SCRATCHPAD);
    CStreamBis_Refill(reader);
    DeleteSw(world);
    ResetClock(&g_clock);
    AddGrfusr__Fi(8);
    func_002333B4(&g_sw,0,sizeof(SW));
    func_002333B4(&g_sw,0,sizeof(SW));
    SW_ctor(&g_sw);
    if(CMpeg_ExecuteOids(D_002F66B0))changed=1;
    InitCm__FP2CM(&g_cm);
    world->currentLevel=request.destinationLevel;
    *destination=request;
    InitSwLoadBuffers(world);
    if(hasFile){LoadSwFromBrx__FP2SWP18CBinaryInputStream(world,reader);CStreamBis_Close(reader);}
    PostSwLoad(world);
    if(resume>0.0f)ShowCountdownHud(world,resume);
    if(changed||(world->transition.flags&0x40))ClearCheatItems();
    CONTEXT *context=FindWarpByOid(world,world->transition.oidWarpDest,world->transition.oidWarpContext);
    if(context){TriggerWarp(context);world->returnWarpOid=world->transition.oidWarpDest;world->returnWarpContext=world->transition.oidWarpContext;}
    if(!GetHeroPo(world)&&!GetCurrentPo(world)&&(int)world->cpo>0){
        ACTOR *selected=0;
        int actorId;
        {CRef value=GetSaveVarByName("oidPoCurrent",0);actorId=value.u.n;}
        if(actorId!=-1)selected=FindSwObject__FP2SWi3OIDP2LO(world,5,actorId,0);
        if(!selected)selected=D_002E1E40;
        if(!selected)selected=D_002DD5BC;
        if(!selected)selected=D_002F7900;
        if(selected){
            Activate(selected);
            Enable(selected,1);
            if(context){
                int i;
                for(i=0;i<(int)context->pairCount;i++)if(context->pairs[i].object==selected->oid)break;
                if(i>=(int)context->pairCount)Attach(selected,context);
            }
            for(int i=0;i<(int)world->cpo;i++){
                ACTOR *actor=(ACTOR *)world->playerObjects[i];
                if(actor!=selected&&IsBasicDerivedFrom__FP5BASIC3CID(actor,0x34)&&!GetPoCtlState(actor))Hide(actor);
            }
        }
    }
    if(!GetCurrentWorld()){
        ACTOR *actor=GetHeroPo(world);
        if(actor&&IsBasicDerivedFrom__FP5BASIC3CID(actor,0x34)&&actor->jtmode.hud.pwidgetHud&&!actor->jtmode.hud.pwidgetHud->state)SetWidgetState(actor->jtmode.hud.pwidgetHud,1);
    }
    UpdateCmLast__FP2CM(&g_cm);
    CutCm__FP2CM(&g_cm);
    BroadcastGameEvent(&g_game,5,0);
    ClearPhase(2);
    ResumeWipeAfterLoad(D_00443740);
    if(!hasFile)RunNextStartupStep(D_002E1E88);
    DestroyCStreamBis(reader,2);
    STACKSCOPE_ctor_1AE4A0(scope,2);
}
} // namespace M_ExecuteSwTransition

namespace M_IncrementSwHandsOff {
void AddGrfusr__Fi(int n) __asm__("AddGrfusr__Fi");
void AcquireSwLetterbox(SW *p) __asm__("AcquireSwLetterbox");
void IncrementSwInputLock(SW *p) __asm__("IncrementSwInputLock");
// Takes control away from the player (counted): the first call disables input; flag 1 also shows the letterbox,
// flag 2 also locks input.
// Also known as: PauseGame (upstream readable / Sly 1 name; PauseGame__FP9GAMEPAUSEi).
extern "C" void IncrementSwHandsOff(SW *world, int fl)
{
    if (++world->handsOffCount == 1) AddGrfusr__Fi(2);
    if (fl & 1) AcquireSwLetterbox(world);
    if (fl & 2) IncrementSwInputLock(world);
}
} // namespace M_IncrementSwHandsOff

namespace M_DecrementSwHandsOff {
void RemoveGrfusr__Fi(int n) __asm__("RemoveGrfusr__Fi");
void ClearJoyButtons(JOY *pjoy, int grfbtn) __asm__("ClearJoyButtons");
void ReleaseSwLetterbox(SW *p) __asm__("ReleaseSwLetterbox");
void DecrementSwInputLock(SW *p) __asm__("DecrementSwInputLock");
// Gives control back (counted): the last call re-enables input; flags as for IncrementSwHandsOff.
// Also known as: ResumeGame (upstream readable / Sly 1 name; ResumeGame__FP9GAMEPAUSEi).
extern "C" void DecrementSwHandsOff(SW *world, int grf)
{
    if (--world->handsOffCount == 0) {
        RemoveGrfusr__Fi(2);
        ClearJoyButtons(&g_joy, 8);
    }
    if (grf & 1) ReleaseSwLetterbox(world);
    if (grf & 2) DecrementSwInputLock(world);
}
} // namespace M_DecrementSwHandsOff

// True while the player has no control (hands-off count > 0).
// Also known as: IsSwHandsOff (upstream readable / Sly 1 name; IsSwHandsOff__FP2SW).
extern "C" int IsSwHandsOff(SW *world)
{
    return world->handsOffCount > 0;
}

// Disables pausing (counted; used by cutscene hands-off and the recon mode).
// Lilly's jtmode.c port calls it IncrementSwNoPause too.
// Also known as: PauseTalk (upstream readable / Sly 1 name; PauseTalk__FP2SW).
extern "C" void IncrementSwNoPause(SW *world)
{
    if (++world->noPauseCount == 1) {
        AddGrfusr(4);
    }
}

// Re-enables pausing (counted).
// Also known as: ResumeTalk (upstream readable / Sly 1 name; ResumeTalk__FP2SW).
extern "C" void DecrementSwNoPause(SW *world)
{
    if (--world->noPauseCount == 0) {
        RemoveGrfusr(4);
    }
}

// 0x20F120: dead-code scrap (sly2/scrap.h)
SLY2_SCRAP("slt $2, $zero, $2");

// Shows the letterbox bars (counted; the first call hides the subtitles and shows the letterbox object).
extern "C" void AcquireSwLetterbox(SW *world)
{
    if (++world->unknownCount3D80 == 1) {
        ShowLetterbox(D_002E1E60);
    }
}

// Hides the letterbox bars when the last user releases them.
extern "C" void ReleaseSwLetterbox(SW *world)
{
    if (--world->unknownCount3D80 == 0) {
        HideLetterbox(D_002E1E60);
    }
}

// 0x20F190: dead-code scrap (sly2/scrap.h)
SLY2_SCRAP("slt $2, $zero, $2");

// Locks player input (counted). (inferred)
extern "C" void IncrementSwInputLock(SW *world)
{
    world->unknown3D84++;
}

// Unlocks player input (counted). (inferred)
extern "C" void DecrementSwInputLock(SW *world)
{
    world->unknown3D84--;
}

// True while player input is locked. (inferred)
extern "C" int IsSwInputLocked(SW *world)
{
    return world->unknown3D84 > 0;
}

namespace M_UpdateSwIllum {
typedef int qword __attribute__((mode(TI)));
typedef union { qword q; float f; } U;
typedef union { qword q; VECTOR v; } UV;
extern char g_lmZeroOne[] __asm__("g_lmZeroOne");
void ConvertUserRgbToUserHsv(VECTOR *pin, UV *pout) __asm__("ConvertUserRgbToUserHsv");
void ColorCorrectVector(UV *pv) __asm__("ColorCorrectVector");
float ClampToLimits__FP2LMf(void *p, float g) __asm__("ClampToLimits__FP2LMf");
// Recomputes the ambient light colour from the world's HSV colour scaled by the midtone and shadow levels and bumps
// the illumination change counter.
extern "C" void UpdateSwIllum(SW *world)
{
    float k = 1.0f / 255.0f;
    VECTOR c;
    UV out;
    U u;
    qword r;
    c.x = world->hsvLight.x;
    c.y = world->hsvLight.y * k;
    c.z = world->hsvLight.z * k;
    ConvertUserRgbToUserHsv(&c, &out);
    {
        qword qwuqOut;
        __asm__("qmtc2.ni %1, %0" : "=j"(qwuqOut) : "r"(world->midtone * k));
        u.q = qwuqOut;
    }
    __asm__("vmulx.xyzw %0, %1, %2x" : "=j"(r) : "j"(out.q), "j"(u.q));
    out.q = r;
    ColorCorrectVector(&out);
    world->vecLightRgb = out.v;
    world->shadowScale = world->shadow * k;
    if (world->unknown3E38)
        world->shadowScale = ClampToLimits__FP2LMf(g_lmZeroOne, world->shadowScale + 0.3f);
    world->relightStamp++;
}
} // namespace M_UpdateSwIllum

// Sets the world's illumination HSV colour and recomputes the ambient light.
extern "C" void SetSwIllumHsv(SW *world, qword hsv)
{
    *(qword *)&world->hsvLight = hsv;
    UpdateSwIllum(world);
}

// Sets the illumination shadow level and recomputes the ambient light.
extern "C" void SetSwIllumShadow(SW *world, float g)
{
    world->shadow = g;
    UpdateSwIllum(world);
}

// Sets the illumination midtone (default 90) and recomputes the ambient light.
extern "C" void SetSwIllum(SW *world, float g)
{
    world->midtone = g;
    UpdateSwIllum(world);
}

namespace M_LoadLevelTableFromBrx {
// LoadLevelTableFromBrx: ported from Lilly's personal Sly 2 decomp (src/P2/sw.c), reduced to the declarations it needs.
// Every global is named by its address; the function keeps its readable name and gets the team symbol through __asm__.
// Globals: D_003E1C4C (g_alevellist), D_003E1C48 (g_anEpisodeOfLevel), D_003E1C44 (g_anWorldOfLevel), D_003E1C3C (g_nUnk3E1C3C), D_003E1C38 (g_cLevelTable), D_003E1C40 (g_aLevelRecord), D_003E1C50 (g_aWorldEntryRecord)
// 0x0020F318 LoadLevelTableFromBrx  (T1038)
// u32 clevel, u32 (unused); clevel x { file record (0x40 bytes); u8 world; u8 episode; u8 fEntry (the record is also the entry of its world:
// copied to g_aWorldEntryRecord[world]); u8 n + n u16 level numbers }.
#include "sly2/sw.h"
void *PvAllocSwImpl(SWALLOC *pswalloc) __asm__("AllocFromWorldHeap");
void *PvAllocSwClearImpl(SWALLOC *pswalloc) __asm__("AllocClearedFromWorldHeap");
// Allocate cb bytes with alignment cbAlign from the world heap.
inline void *PvAllocSwLoad(int cb, int cbAlign)
{
    SWALLOC swalloc;
    swalloc.cb = cb;
    SWALLOC *pswalloc = &swalloc;
    pswalloc->cbAlign = cbAlign;
    return PvAllocSwImpl(pswalloc);
}
// Allocate cb zeroed bytes with alignment cbAlign from the world heap.
inline void *PvAllocSwClearLoad(int cb, int cbAlign)
{
    SWALLOC swalloc;
    swalloc.cb = cb;
    SWALLOC *pswalloc = &swalloc;
    pswalloc->cbAlign = cbAlign;
    return PvAllocSwClearImpl(pswalloc);
}
struct LEVELLIST
{
      int c;
      short *a;
};
extern int D_003E1C38 __asm__("D_003E1C38");
extern int D_003E1C3C __asm__("D_003E1C3C");
extern FILEREC *D_003E1C40 __asm__("D_003E1C40");
extern int *D_003E1C44 __asm__("D_003E1C44");
extern int *D_003E1C48 __asm__("D_003E1C48");
extern LEVELLIST *D_003E1C4C __asm__("D_003E1C4C");
extern FILEREC *D_003E1C50 __asm__("D_003E1C50");
extern "C" void LoadLevelTableFromBrx(SW *world, CBinaryInputStream *stream) __asm__("LoadLevelTableFromBrx");
// Reads the level table: per level its file record, world, episode, entry flag and level numbers (fills the level
// list, the world and episode of each level and the world entry records).
// Also known as: LoadWorldRecordsFromBrx (upstream readable / Sly 1 name;
//   LoadWorldRecordsFromBrx__FP2SWP18CBinaryInputStream).
extern "C" void LoadLevelTableFromBrx(SW *world, CBinaryInputStream *stream)
{
    D_003E1C38 = stream->U32Read();
    D_003E1C3C = stream->U32Read();
    D_003E1C40 = (FILEREC *)PvAllocSwLoad(D_003E1C38 * sizeof(FILEREC), 0x10);
    D_003E1C44 = (int *)PvAllocSwLoad(D_003E1C38 * sizeof(int), 0x10);
    D_003E1C48 = (int *)PvAllocSwLoad(D_003E1C38 * sizeof(int), 0x10);
    D_003E1C4C = (LEVELLIST *)PvAllocSwLoad(D_003E1C38 * sizeof(LEVELLIST), 0x10);
    D_003E1C50 = (FILEREC *)PvAllocSwClearLoad(9 * sizeof(FILEREC), 0x10);
    for (int ilevel = 0; ilevel < D_003E1C38; ilevel++)
    {
        CBinaryInputStream_ReadCb64(stream, &D_003E1C40[ilevel]);
        D_003E1C44[ilevel] = stream->U8Read();
        D_003E1C48[ilevel] = stream->U8Read();
        if (stream->U8Read())
            D_003E1C50[D_003E1C44[ilevel]] = D_003E1C40[ilevel];
        LEVELLIST *plevellist = &D_003E1C4C[ilevel];
        plevellist->c = stream->U8Read();
        plevellist->a = (short *)PvAllocSwLoad(plevellist->c * sizeof(short), 0x10);
        for (int i = 0; i < plevellist->c; i++)
            plevellist->a[i] = stream->U16Read();
    }
}
} // namespace M_LoadLevelTableFromBrx

namespace M_LoadSwSharedHeaderFromBrx {
extern int D_003E1C30 __asm__("D_003E1C30");
extern int D_003E1C54 __asm__("D_003E1C54");
extern int D_003E1C58 __asm__("D_003E1C58");
int S32Read__18CBinaryInputStream(void *stream) __asm__("S32Read__18CBinaryInputStream");
// Reads the header (size, id) of the data block shared across levels and flags a reload when the id changed or when
// forced. Layout parse-verified: a parser following it reads every byte of 4 levels.
// Also known as: LoadBulkHeaderFromBrx (upstream readable / Sly 1 name; LoadBulkHeaderFromBrx__FiPv).
extern "C" void LoadSwSharedHeaderFromBrx(int a, void *stream)
{
    int x = S32Read__18CBinaryInputStream(stream);
    int y = S32Read__18CBinaryInputStream(stream);
    D_003E1C30 = x;
    if (y != D_003E1C58) a = 1;
    D_003E1C54 = a;
    D_003E1C58 = y;
}
} // namespace M_LoadSwSharedHeaderFromBrx

namespace M_LoadSwSharedDataFromBrx {
extern int D_003E1C54 __asm__("D_003E1C54");
extern int D_003E1C30 __asm__("D_003E1C30");
struct L { int a[4]; };
struct M { int a; int b; int c[2]; };
void Align__18CBinaryInputStreami(void *q, int n) __asm__("Align__18CBinaryInputStreami");
void STACKSCOPE_ctor(struct L *pl) __asm__("STACKSCOPE_ctor");
void *AllocFromStack(struct M *pm) __asm__("AllocFromStack");
void Read__18CBinaryInputStreamiPv(void *q, int a, void *x) __asm__("Read__18CBinaryInputStreamiPv");
void RelocateSharedColorTables(void *x) __asm__("RelocateSharedColorTables");
void UploadBitmaps__FP3GSB(void *p) __asm__("UploadBitmaps__FP3GSB");
void STACKSCOPE_ctor_1AE4A0(struct L *pl, int n) __asm__("_$_10STACKSCOPE");
void Skip__18CBinaryInputStreami(void *q, int a) __asm__("Skip__18CBinaryInputStreami");
void AllocGsbBitmaps(void *p) __asm__("AllocGsbBitmaps");
// Reads the shared data block when it changed (and rebuilds from it), otherwise skips it and reuses the loaded copy.
// Layout parse-verified: a parser following it reads every byte of 4 levels.
// Also known as: LoadBulkFromBrx (upstream readable / Sly 1 name; LoadBulkFromBrx__FP4SHDSP18CBinaryInputStream).
extern "C" void LoadSwSharedDataFromBrx(void *world, void *q)
{
    Align__18CBinaryInputStreami(q, 0x10);
    if (D_003E1C54) {
        struct L l;
        struct M m;
        void *x;
        STACKSCOPE_ctor(&l);
        { struct M *pm = &m;
        pm->a = D_003E1C30;
        pm->b = 0x10;
        x = AllocFromStack(pm); }
        Read__18CBinaryInputStreamiPv(q, D_003E1C30, x);
        RelocateSharedColorTables(x);
        UploadBitmaps__FP3GSB(world);
        STACKSCOPE_ctor_1AE4A0(&l, 2);
    } else {
        Skip__18CBinaryInputStreami(q, D_003E1C30);
        AllocGsbBitmaps(world);
    }
}
} // namespace M_LoadSwSharedDataFromBrx

namespace M_func_0020F710 {
extern int D_002C3E70[4] __asm__("D_002C3E70");
void *func_002333B4(void *p, int c, int n) __asm__("memset");
WIDGET *FindSwObject__FP2SWi3OIDP2LO(SW *psw, int grffso, int oid, int context) __asm__("FindSwObject__FP2SWi3OIDP2LO");
void SetWidgetXf(WIDGET *widget, SPRITEXF *pxf) __asm__("SetWidgetXf");
int CountCollectedClues(void) __asm__("CountCollectedClues");
int GetMaxClueCount(void) __asm__("GetMaxClueCount");
// Finds the four timer HUD objects (oids 0x371, 0x36D, 0x36E, 0x36F), initializes them and records two counters.
// (inferred)
extern "C" void BindSwTimerHud(SW *world)
{
    SPRITEXF l;
    int i;
    int *pn = D_002C3E70;
    func_002333B4(&l, 0, sizeof(l));
    l.a[3] = 1.0f;   /* identity */
    l.a[0] = 1.0f;
    for (i = 0; i < 4; i++) {
        WIDGET *widget = FindSwObject__FP2SWi3OIDP2LO(&g_sw, 0x405, D_002C3E70[i], 0);
        if (widget) {
            SetWidgetXf(widget, &l);
            world->ahudstack[i].widget = widget;
        }
    }
    world->numCluesCollected = CountCollectedClues();
    world->numClues = GetMaxClueCount();
}
} // namespace M_func_0020F710

namespace M_func_0020F7E0 {
extern char D_002C3E88[] __asm__("D_002C3E88");
extern float D_002C3E80 __asm__("D_002C3E80");
int IsWidgetVisible(WIDGET *widget) __asm__("IsWidgetVisible");
float SmoothValueWithAccel__FffffP4SMPAPf(float a, float b, float c, void *p, float *pv, float dt) __asm__("SmoothValueWithAccel__FffffP4SMPAPf");
void *CreateSfxSound(int n) __asm__("CreateSfxSound");
void SetSndSourceLo(void *p, void *source) __asm__("SetSndSourceLo");

// Per-frame timer HUD update: smooths the four elements and, while a countdown runs, plays the tick/tock sounds
// (0x30/0x31) once a second. (inferred)
// Also known as: StackCounters (upstream readable / Sly 1 name; StackCounters__FP2SW).
extern "C" void UpdateSwTimerHud(SW *world)
{
    float z;
    float t = 0.0f;
    int i;
    z = t;
    for (i = 0; i < 4; i++) {
        HUDSTACK *e = &world->ahudstack[i];
        WIDGET *o = e->widget;
        if (!o || !IsWidgetVisible(o)) {
            e->shouldSnap = 1;
            continue;
        }
        if (e->shouldSnap) {
            o->xform->a[5] = t;
            e->dyVel = 0.0f;
            e->shouldSnap = 0;
        } else {
            o->xform->a[5] = SmoothValueWithAccel__FffffP4SMPAPf(o->xform->a[5], e->dyVel, t, D_002C3E88, &e->dyVel, g_clock.deltaTime);
        }
        t += D_002C3E80;
        if (i == 3) {
            float remaining = -g_clock.time + world->hudTimerEndTime;
            int n;
            remaining = (0.0f > remaining) ? 0.0f : remaining;
            n = (int)remaining;
            if (n != world->csecHudTimerShown) {
                SetSndSourceLo(CreateSfxSound((n & 1) ? 0x30 : 0x31), 0);
                world->csecHudTimerShown = n;
            }
        }
    }
}
} // namespace M_func_0020F7E0

namespace M_FFilterSoPair {
int MaxInt(int a, int b) __asm__("MaxInt");
// Intersection filter: rejects the object itself and flagged objects, and pairs whose relationship (0x134310) is 1
// or 3.
// Also known as: NOverlapKind (upstream readable / Sly 1 name; NOverlapKind__FP3ALOT0).
extern "C" int ShouldKeepSoPair(SOTREE *object, SOPART *e)
{
    SOTREE *ps = e->owner->physicsTree;
    int a, b, r;
    if (!e->unknownFlag10A && ps->skipsUpdate)
        return 0;
    if (e->unknownFlag10A || ps == object)
        return 0;
    a = object->root->ShouldIgnoreIntersection(ps->root);
    b = ps->root->ShouldIgnoreIntersection(object->root);
    r = MaxInt(a, b);
    return r != 1 && r != 3;
}
} // namespace M_FFilterSoPair

namespace M_func_0020FA10 {
struct O { char pad[0x70]; long fl; };
struct P : LO { char pad[0xEC - 0x50]; struct O *po; };
struct S { int a; struct P *pp; char pad[8]; long fl; };
// Intersection filter: keeps objects that are not the context object and do not have flag 4.
extern "C" int ShouldKeepSoNotSelf(struct O *object, struct S *p) {
    int r = 0;
    if (!(p->fl & 0x1000000000000L)) {
        struct O *o = p->pp->po;
        if (!(o->fl & 4)) r = o != object;
    }
    return r;
}
} // namespace M_func_0020FA10

namespace M_func_0020FA50 {
int ShouldKeepSoNotSelf(void *pv, SOPART *p) __asm__("ShouldKeepSoNotSelf");
// Intersection filter: ShouldKeepSoNotSelf and the owner's IsFixed (slot 62).
extern "C" int ShouldKeepSoNotSelfSlot62(void *pv, SOPART *p) { if (!ShouldKeepSoNotSelf(pv, p)) return 0; return p->owner->IsFixed(); }
} // namespace M_func_0020FA50

namespace M_IntersectSwBoundingBox {
typedef int qword __attribute__((mode(TI)));
struct O { char pad[0x60]; struct O *next; };
struct G { float mn[4]; float mx[3]; char pad[0x34 - 0x1C]; struct O *first; char pad2[0x40 - 0x38]; struct G *next; };
void *AllocFromStack(struct SWALLOC *pmsg) __asm__("AllocFromStack");
void FlushSwBoundsUpdates(SW *p) __asm__("FlushSwBoundsUpdates");
void UpdateSwAaox(SW *p) __asm__("UpdateSwAaox");

// Collects up to 256 SOs whose bounding groups overlap the box spanned by two points, with an optional filter; the
// result is a slot-allocated array.
extern "C" void IntersectSwBoundingBox(SW *p, qword a, qword b, int (*pfn)(void *, struct O *), void *ctx, int *pc, struct O ***pap)
{
    QWROW mn, mx;
    struct SWALLOC msg;
    struct O **ap;
    struct G *g;
    int c;
    {
        qword qwmnqOut;
        __asm__("vmini.xyz %0, %1, %2" : "=j"(qwmnqOut) : "j"(a), "j"(b));
        mn.q = qwmnqOut;
    } {
        qword qwmxqOut;
        __asm__("vmax.xyz %0, %1, %2" : "=j"(qwmxqOut) : "j"(a), "j"(b));
        mx.q = qwmxqOut;
    }
    {
        struct SWALLOC *pm = &msg;
        pm->cb = 0x400;
        pm->cbAlign = 0x10;
    }
    c = 0;
    ap = (__typeof__(ap))AllocFromStack(&msg);
    FlushSwBoundsUpdates(p);
    UpdateSwAaox(p);
    for (g = (struct G *)p->dlSoTree.head; g; g = g->next) {
        struct O *o;
        if (g->mn[0] > mx.f[0] || g->mx[0] < mn.f[0] ||
            g->mn[1] > mx.f[1] || g->mx[1] < mn.f[1] ||
            g->mn[2] > mx.f[2] || g->mx[2] < mn.f[2])
            continue;
        for (o = g->first; o; o = o->next) {
            if (pfn && !pfn(ctx, o))
                continue;
            if (c < 0x100)
                ap[c++] = o;
        }
    }
    *pap = ap;
    *pc = c;
}
} // namespace M_IntersectSwBoundingBox

namespace M_IntersectSwBoundingSphere {
typedef int qword __attribute__((mode(TI)));
struct O { char pad[0x60]; struct O *next; };
struct G { char pad[0x20]; qword pos; float r; struct O *first; char pad2[0x40 - 0x38]; struct G *next; };
void *AllocFromStack(struct SWALLOC *pmsg) __asm__("AllocFromStack");
void FlushSwBoundsUpdates(SW *p) __asm__("FlushSwBoundsUpdates");
void UpdateSwAaox(SW *p) __asm__("UpdateSwAaox");

// Collects up to 256 SOs whose bounding groups overlap a sphere, with an optional filter.
extern "C" void IntersectSwBoundingSphere(SW *p, qword pos, float r, int (*pfn)(void *, struct O *), void *ctx, int *pc, struct O ***pap)
{
    struct SWALLOC msg;
    struct O **ap;
    struct G *g;
    int c;
    msg.cb = 0x400;
    msg.cbAlign = 0x10;
    ap = (__typeof__(ap))AllocFromStack(&msg);
    c = 0;
    FlushSwBoundsUpdates(p);
    UpdateSwAaox(p);
    for (g = (struct G *)p->dlSoTree.head; g; g = g->next) {
        float rr;
        qword d, e, tmp;
        float f;
        struct O *o;
        __asm__("vsub.xyzw %0, %1, %2" : "=j"(d) : "j"(pos), "j"(g->pos));
        __asm__("vmul.xyz %0, %0, %0\n\tvaddw.x %1, $vf0, $vf0w\n\tvadday.x $ACC, %0, %0y\n\tvmaddz.x %0, %1, %0z" : "=j"(e), "=&j"(tmp) : "0"(d));
        rr = r + g->r;
        rr *= rr;
        __asm__("mtc1 %1, %0" : "=f"(f) : "r"(e));
        if (rr < f)
            continue;
        for (o = g->first; o; o = o->next) {
            if (pfn && !pfn(ctx, o))
                continue;
            if (c < 0x100)
                ap[c++] = o;
        }
    }
    *pap = ap;
    *pc = c;
}
} // namespace M_IntersectSwBoundingSphere

namespace M_func_0020FDC8 {
struct O : LO { char pad[0x148 - 0x50]; long fl; };
struct A { int pad; struct O *po; };
struct M { int cb; int align; int pad[2]; };
void *AllocFromStack(struct M *pm) __asm__("AllocFromStack");
// Builds the list of distinct SOs referenced by an array of entries (marking them while collecting).
extern "C" void BuildUniqueSoList(int n, struct A **apa, int *pc, struct O ***pppo)
{
    struct M m;
    struct O **apo;
    int c = 0;
    int i;
    m.cb = n * 4;
    m.align = 0x10;
    apo = (struct O **)AllocFromStack(&m);
    for (i = 0; i < n; i++) {
        struct O *po = apa[i]->po;
        if (!(po->fl & 0x40000000)) {
            po->fl |= 0x40000000;
            apo[c++] = po;
        }
    }
    for (i = 0; i < c; i++)
        apo[i]->fl &= ~0x40000000L;
    *pc = c;
    *pppo = apo;
}
} // namespace M_func_0020FDC8

namespace M_FAddRsmg__FP2SWP4RSMGiPii3OIDN25 {
typedef int OID;
typedef struct RSMG { OID oidRoot; OID oidSM; OID oidTrigger; OID oidNoTrigger; } RSMG;
extern "C" { int AddOrUpdateRsmg(void *pv, RSMG *arsmg, int crsmgMax, int *pcrsmg, int fTrigger, OID oidRoot, OID oidSM, OID oid)
    __asm__("AddOrUpdateRsmg__FP2SWP4RSMGiPii3OIDN25"); }
// Adds or updates a rsmg record ((root, state machine) -> goal on trigger or untrigger) in a bounded array.
// (Sly 1-derived name: FAddRsmg.)
// Also known as: FAddRsmg (upstream readable / Sly 1 name; FAddRsmg__FP4RSMGiPii3OIDN24).
extern "C" int AddOrUpdateRsmg(void *pv, RSMG *arsmg, int crsmgMax, int *pcrsmg, int fTrigger, OID oidRoot, OID oidSM, OID oid)
{
    int i;
    for (i = 0; i < *pcrsmg; i++) {
        RSMG *prsmg = &arsmg[i];
        if (prsmg->oidRoot == oidRoot && (prsmg->oidSM == -1 || prsmg->oidSM == oidSM)) {
            if (fTrigger)
                prsmg->oidTrigger = oid;
            else
                prsmg->oidNoTrigger = oid;
            return 1;
        }
    }
    if (*pcrsmg + 1 >= crsmgMax)
        return 0;
    i = (*pcrsmg)++;
    arsmg[i].oidRoot = oidRoot;
    arsmg[i].oidSM = oidSM;
    if (fTrigger) {
        arsmg[i].oidTrigger = oid;
        arsmg[i].oidNoTrigger = -1;
    } else {
        arsmg[i].oidNoTrigger = oid;
        arsmg[i].oidTrigger = -1;
    }
    return 1;
}
} // namespace M_FAddRsmg__FP2SWP4RSMGiPii3OIDN25

namespace M_TriggerRsmg {
typedef struct QW { int a[4]; } QW;
void TriggerOneRsmg(void *p, QW *pq, int a, int b, int c, int d) __asm__("TriggerOneRsmg");
// Applies every rsmg record of an array (TriggerOneRsmg on each).
// Also known as: ApplyRsmg (upstream readable / Sly 1 name; ApplyRsmg__FP2LOiP4RSMGii).
extern "C" void TriggerRsmg(void *p, int c, QW *pq, int a, int b)
{
    int i;
    for (i = 0; i < c; i++) {
        TriggerOneRsmg(p, &pq[i], a, b, 0, 0);

    }
}
} // namespace M_TriggerRsmg

namespace M_func_0020FFF0 {
struct D { int a; int b; int c; int d; };
int FindSwObjects(void *p, int k, int a, int b, int c, void **ppo) __asm__("FindSwObjects");
int IsBasicDerivedFrom__FP5BASIC3CID(void *o, int n) __asm__("IsBasicDerivedFrom__FP5BASIC3CID");
BASIC *FindLiveSma(void *o, int b) __asm__("FindLiveSma");
void SubscribeLoObject(BASIC *y, int x) __asm__("SubscribeLoObject");
void SeekSma(BASIC *y, int id) __asm__("SeekSma");
void SetSmaGoal(BASIC *y, int id) __asm__("SetSmaGoal");
// Applies one rsmg record: finds the root object and its state machine by oid, then seeks or sets the trigger or
// untrigger goal; optionally returns the state machine.
extern "C" void TriggerOneRsmg(void *p, struct D *pd, int b, int fl, int x, BASIC **ppy)
{
    void *o;
    BASIC *y;
    int id;
    if (ppy)
        *ppy = 0;
    if (FindSwObjects(p, 0x204, pd->a, b, 1, &o) != 1)
        return;
    if (!IsBasicDerivedFrom__FP5BASIC3CID(o, 0x30))
        return;
    y = FindLiveSma(o, pd->b);
    if (!y)
        return;
    if (fl & 1)
        id = pd->c;
    else
        id = pd->d;
    if (id == -1)
        return;
    if (x) {
        *ppy = y;
        y->AddRef();
        SubscribeLoObject(y, x);
    }
    if (fl & 2)
        SeekSma(y, id);
    else
        SetSmaGoal(y, id);
}
} // namespace M_func_0020FFF0

// Returns the next free 12-byte record of the world's deferred-callback array (AddLoDeferredCallback uses it).
/* The next 12-byte record of the array at +0x39D0. */
extern "C" void *AllocSwLoCallbackRecord(SW *world)
{
    int i = world->clorunPool;
    world->clorunPool = i + 1;
    return (char *)world->pvLorunPool + i * sizeof(LORUN);
}

// Returns the next free 12-byte record of the world's post-load queue (drained by PostSwLoad).
/* The next 12-byte record of the array at +0x39D8. */
extern "C" void *AllocSwPostLoadRecord(SW *world)
{
    int i = world->cpostLoadPool;
    world->cpostLoadPool = i + 1;
    return (char *)world->pvPostLoadPool + i * 0xC;
}

namespace M_func_00210140 {
// Adds an object to the world's per-object update array (back-pointer kept in its +0x40 word), compacting the array
// when it is full or more than 10% holes.
// Also known as: AddLoToUpdateTable (upstream readable / Sly 1 name; AddLoToUpdateTable__FP2SWP2LO).
extern "C" void AddLoToSwUpdateList(SW *g, LO *o)
{
    LOUPD *e;
    if (o->unknown40 & 0x40)
        return;
    if (g->cloupd >= 0x1000 || (float)g->cloupdHole / (float)g->cloupd > 0.1f) {
        LOUPD *end = g->aloupd + g->cloupd;
        LOUPD *tracked;
        LOUPD *write;
        LOUPD *read;
        if (g->piloupdCur)
            tracked = g->aloupd + *((int *)g->piloupdCur);
        else
            tracked = 0;
        write = g->aloupd;
        for (read = g->aloupd; read < end; read++) {
            if (read == tracked)
                tracked = write;
            if (read->plo) {
                read->plo->ploupd = write;
                *write++ = *read;
            }
        }
        g->cloupdHole = 0;
        g->cloupd = write - g->aloupd;
        if (g->piloupdCur)
            *((int *)g->piloupdCur) = tracked - g->aloupd;
    }
    e = &g->aloupd[g->cloupd++];
    e->plo = o;
    e->grf = o->unknown40;
    o->ploupd = e;
}
} // namespace M_func_00210140

// Takes an object off the update array: restores its +0x40 word, clears the entry and counts the hole.
/* Takes an object off the update list: restores its +0x40 word, clears the entry, counts the hole. */
// Also known as: RemoveLoFromUpdateTable (upstream readable / Sly 1 name; RemoveLoFromUpdateTable__FP2SWP2LO).
extern "C" void RemoveLoFromSwUpdateList(SW *world, LO *plo)
{
    int n = plo->unknown40;
    if (((n ^ 1) & 1) != 0) {
        LOUPD *ploupd = plo->ploupd;
        plo->unknown40 = ploupd->grf;
        ploupd->grf = 0;
        ploupd->plo = 0;
        world->cloupdHole++;
    }
}

namespace M_func_002102A0 {
struct XB { char pad[0xC];
  virtual void v1(); virtual void v2(); virtual void v3(); virtual void v4(); virtual void v5();
  virtual void v6(); virtual void v7(); virtual void v8(); virtual void v9(); virtual void v10();
  virtual void v11(); virtual void v12(); virtual void v13(); virtual void v14(); virtual void v15();
  virtual void v16(); virtual void v17(); virtual void v18(); virtual void v19(); virtual void v20();
  virtual void v21(); virtual void v22(); virtual void v23(); virtual void v24();
};
struct X : XB { char pad10[0x10]; X *next; };
extern void SetSwPoOverride(SW *p, int k) __asm__("SetSwPoOverride");
// Calls vtable slot 24 on every object of the 199 class lists, after 0x1C4A60(world, 0). (inferred: the slot's
// meaning is not known; PROXY uses it to release its spares)
extern "C" void CallSlot24OnSwObjects(SW *world)
{
    int i;
    SetSwPoOverride(world, 0);
    for (i = 0; i < 199; i++) {
        X *x = (X *)((SL *)((char *)world + SLY2_OFFSETOF(SW, objectsByClass)))[i].head;   /* the list base computed once, as the code does */
        for (; x; x = x->next)
            x->v24();
    }
}
} // namespace M_func_002102A0

// LO's default for vtable slot 24: empty.
extern "C" void LO_Slot24Default()
{
}

namespace M_ShowSwAsegObject {
struct X { char pad[0xC];
  virtual void v1(); virtual void v2(); virtual void v3(); virtual void v4();
  virtual void v5(); virtual void v6(); virtual void v7(); virtual void v8(); virtual void v9(); };
extern X *FindSwObject__FP2SWi3OIDP2LO(void *pv, int k, int id) __asm__("FindSwObject__FP2SWi3OIDP2LO");
extern int IsBasicDerivedFrom__FP5BASIC3CID(void *p, int cid) __asm__("IsBasicDerivedFrom__FP5BASIC3CID");
extern void FadeSoIn(X *p, float f) __asm__("FadeSoIn");
extern void FadeSoOut(X *p, float f) __asm__("FadeSoOut");
// Animation event 7/8: shows or hides an object, fading SOs in or out over dt when dt is non-zero, else adding or
// removing it.
// Also known as: AddRemoveByOid (upstream readable / Sly 1 name; AddRemoveByOid__FPv3OIDif).
extern "C" X *ShowSwAsegObject(void *pv, int fOn, int id, float g)
{
    X *p = FindSwObject__FP2SWi3OIDP2LO(pv, 4, id);
    if (p) {
        if (g != 0.0f && IsBasicDerivedFrom__FP5BASIC3CID(p, 0x30)) {
            if (fOn)
                FadeSoIn(p, g);
            else
                FadeSoOut(p, g);
        } else {
            if (fOn)
                p->v8();
            else
                p->v9();
        }
    }
    return p;
}
} // namespace M_ShowSwAsegObject

namespace M_func_00210408 {
void *FindSwObject__FP2SWi3OIDP2LO(void *pv, int k, int id, void *ctx) __asm__("FindSwObject__FP2SWi3OIDP2LO");
int IsBasicDerivedFrom__FP5BASIC3CID(void *p, int cid) __asm__("IsBasicDerivedFrom__FP5BASIC3CID");
void ShowSoGlobsById(void *p, void *q) __asm__("ShowSoGlobsById");
void HideSoGlobsById(void *p, void *q) __asm__("HideSoGlobsById");
// Animation event 13/14: shows or hides the target SO's globs with the given id (ShowSoGlobsById / HideSoGlobsById).
extern "C" void ShowSwAsegGlobs(void *pv, int f, int id, void *q, void *pdef, void *ctx)
{
    void *p;
    if (id == -1)
        p = pdef;
    else
        p = FindSwObject__FP2SWi3OIDP2LO(pv, 4, id, ctx);
    if (p && IsBasicDerivedFrom__FP5BASIC3CID(p, 0x30)) {
        if (f)
            ShowSoGlobsById(p, q);
        else
            HideSoGlobsById(p, q);
    }
}
} // namespace M_func_00210408

namespace M_func_002104A0 {
void *FindSwObject__FP2SWi3OIDP2LO(void *pv, int k, int id, void *ctx) __asm__("FindSwObject__FP2SWi3OIDP2LO");
int IsBasicDerivedFrom__FP5BASIC3CID(void *p, int cid) __asm__("IsBasicDerivedFrom__FP5BASIC3CID");
void ShowSoPartsById(void *p, void *q) __asm__("ShowSoPartsById");
void HideSoPartsById(void *p, void *q) __asm__("HideSoPartsById");
// Animation event 15/16: shows or hides the target SO's parts with the given id (ShowSoPartsById / 0x1F6B48).
extern "C" void ShowSwAsegParts(void *pv, int f, int id, void *q, void *pdef, void *ctx)
{
    void *p;
    if (id == -1)
        p = pdef;
    else
        p = FindSwObject__FP2SWi3OIDP2LO(pv, 4, id, ctx);
    if (p && IsBasicDerivedFrom__FP5BASIC3CID(p, 0x30)) {
        if (f)
            ShowSoPartsById(p, q);
        else
            HideSoPartsById(p, q);
    }
}
} // namespace M_func_002104A0

namespace M_func_00210538 {
struct X { char pad[0xC]; VTE *vtable; };
typedef EXPLSO MSG;   /* what an explosion explodes with (emitter.h EXPLSO; EXPL slot 51) */
struct X *FindSwObject__FP2SWi3OIDP2LO(void *pg, int k, int oid, void *ctx) __asm__("FindSwObject__FP2SWi3OIDP2LO");
// Animation event 17: finds the object and up to two others and sends them a message.
extern "C" void SendSwAsegMessage(void *world, int oid, int oid1, int oid2, float g, void *ctx)
{
    struct X *x = FindSwObject__FP2SWi3OIDP2LO(world, 4, oid, ctx);
    MSG msg;
    if (!x)
        return;
    msg.grf = 0;
    if (oid1 != -1) {
        struct X *y = FindSwObject__FP2SWi3OIDP2LO(world, 4, oid1, ctx);
        if (y) {
            msg.ploReference = (LO *)y;
            msg.grf |= 1;
        }
    }
    {
        unsigned int v = msg.grf;
        if ((v ^ 1) & 1) {
            msg.ploReference = (LO *)x;
            msg.grf = v | 1;
        }
    }
    if (oid2 != -1) {
        struct X *z = FindSwObject__FP2SWi3OIDP2LO(world, 4, oid2, ctx);
        if (z) {
            msg.ploTouch = (LO *)z;
            msg.grf |= 0x20;
        }
    }
    if (g != 1.0f) {
        msg.scale = g;
        msg.grf |= 0x18;
    }
    ((void (*)(void *, void *))x->vtable[51].pfn)((char *)x + x->vtable[51].d, &msg);
}
} // namespace M_func_00210538

namespace M_func_00210678 {
void *FindSwObject__FP2SWi3OIDP2LO(void *pv, int k, int id) __asm__("FindSwObject__FP2SWi3OIDP2LO");
void *FindShaderAnimById(int id) __asm__("FindShaderAnimById");
int IsBasicDerivedFrom__FP5BASIC3CID(void *p, int cid) __asm__("IsBasicDerivedFrom__FP5BASIC3CID");
void SetSoEyesClosed(void *p, float f) __asm__("SetSoEyesClosed");
void SetEyesClosed(void *p, float f) __asm__("SetEyesClosed");
// Animation event 20: sets how closed the eyes are, on the target SO's EYES children (SetSoEyesClosed) or on an EYES
// object (SetEyesClosed).
// Also known as: SetEyesClosedOf (upstream readable / Sly 1 name; SetEyesClosedOf__FP2LOiif).
extern "C" void SetSwAsegEyesClosed(void *pv, int id1, int id2, float f)
{
    void *p1 = 0;
    void *p2 = 0;
    if (id1 != -1) {
        void *q = FindSwObject__FP2SWi3OIDP2LO(pv, 4, id1);
        if (q && IsBasicDerivedFrom__FP5BASIC3CID(q, 0x30))
            p1 = q;
    }
    if (id2 != -1) {
        p2 = FindShaderAnimById(id2);
        if (p2 && !IsBasicDerivedFrom__FP5BASIC3CID(p2, 0x8E))
            p2 = 0;
    }
    if (p1)
        SetSoEyesClosed(p1, f);
    if (p2)
        SetEyesClosed(p2, f);
}
} // namespace M_func_00210678

namespace M_StartSwAsegSound {
// Complete WIP; true source-object lookup and sound-reference contracts.
extern "C" { static inline void Clamp(float *f,float *b) {float lo=b[0],hi=b[1]; if(*f<lo)*f=lo; else if(hi<*f)*f=hi;} }
struct C { char pad[0x20]; float value; };
typedef STEPGUARD O;   /* a STEPGUARD (cid 0x38): +0xFC4 guard.h scale, a curve whose value scales the sound */
extern float D_002C3EA0[3] __asm__("D_002C3EA0");
extern float D_002C3EB0[2] __asm__("D_002C3EB0");
O *FindSwObject__FP2SWi3OIDP2LO(SW *, int, int, void *) __asm__("FindSwObject__FP2SWi3OIDP2LO");
int IsBasicDerivedFrom__FP5BASIC3CID(O *, int) __asm__("IsBasicDerivedFrom__FP5BASIC3CID");
CID151 *CreateSfxSound(int) __asm__("CreateSfxSound");
void SetSndSourceLo(CID151 *, O *) __asm__("SetSndSourceLo");
void SetSndRange(CID151 *, float, float) __asm__("SetSndRange");
void SetSndVolume(CID151 *, float) __asm__("SetSndVolume");
void SetSndPitch(CID151 *, float) __asm__("SetSndPitch");
void SetSndDoppler(CID151 *, float) __asm__("SetSndDoppler");
void RcPtr_Assign(CID151 **, CID151 *) __asm__("RcPtr_Assign");
// Copy the object pointer through a register move.
#ifdef SLY2_EE_ASM
extern "C" { static inline O *SelectedValue(O *p){O *value;__asm__("daddu %0,%1,$zero":"=r"(value):"r"(p));return value;} }
#else
extern "C" { static inline O *SelectedValue(O *p){return p;} }
#endif
// Animation event 0: starts a sound at the source object (optionally found by oid) with range, volume and pitch;
// looping sounds are kept in the animation's reference so StopSwAsegSound can stop them.
// Also known as: PlayAsegaSound (upstream readable / Sly 1 name; PlayAsegaSound__FfffPviiiiN33PPv).
extern "C" void StartSwAsegSound(float offset, float fade, float scale, SW *p, int id, int oid, int x, int y, O *fallback, void *owner, void *unused, CID151 **out)
{
    O *o;
    CID151 *m;
    if (id == -1)
        return;
    __asm__ volatile("" : : : "$4");
    if(oid!=-1)fallback=FindSwObject__FP2SWi3OIDP2LO(p,4,oid,owner);
    o=SelectedValue(fallback);
    if (o && IsBasicDerivedFrom__FP5BASIC3CID(o, 0x38) && ((struct C *)o->scale)) {
        float v = ((struct C *)o->scale)->value;
        float f = D_002C3EA0[0] + v * (D_002C3EA0[1] + v * D_002C3EA0[2]);
        Clamp(&f, D_002C3EB0);
        offset += f;
    }
    offset += p->unknown475C;
    m = CreateSfxSound(id);
    SetSndSourceLo(m, o);
    SetSndRange(m, (float)x, (float)y);
    SetSndVolume(m, scale);
    SetSndPitch(m, offset);
    SetSndDoppler(m, fade);
    if (m->GetKind() == 2)
        RcPtr_Assign(out, m);
}
} // namespace M_StartSwAsegSound

namespace M_StopSwAsegSound {
typedef BANKSND O;   /* the playing sound (snd.h BANKSND: its sfx id at +0x60) */
void StopSnd(O *p, float f) __asm__("StopSnd");
void RcPtr_Assign(O **pp, int a) __asm__("RcPtr_Assign");
// Animation event 1: stops the stored looping sound (optionally only with a matching id) and releases it.
// Also known as: StopAsegaSound (upstream readable / Sly 1 name; StopAsegaSound__FP5ASEGAiPvT2).
extern "C" void StopSwAsegSound(void *a, int id, void *b, O **pp)
{
    O *p = *pp;
    if (!p)
        return;
    if (id != -1 && p->sfxId != id)
        return;
    StopSnd(p, 0.0f);
    RcPtr_Assign(pp, 0);
}
} // namespace M_StopSwAsegSound

// Finds the proxy object with oid 0x620 and stores it at SW+0x4638: the proxy source booties are acquired from and
// locked on (booty.cpp).
extern "C" void BindSwBootyProxy(SW *world)
{
    world->bootyProxySource = FindSwObject(world, 5, (OID)0x620, 0);
}

// Stores n in the world's unnamed word +0x3EA8 when n is 1; returns the world. (inferred)
extern "C" SW *SetSwUnknown3EA8(SW *world, int n)
{
    if (n == 1) {
        F3EA8(world) = n;
    }
    return world;
}

namespace M_func_00210990 {
struct E { char pad[0x40]; };
extern struct E *D_003E1C40 __asm__("D_003E1C40");
// Returns the file record of level index levelIndex, or null for -1.
extern "C" struct E *GetLevelFileRecord(int i)
{
    if (i == -1)
        return 0;
    return &D_003E1C40[i];
}
} // namespace M_func_00210990

// Returns the level-list entry of level index levelIndex.
extern "C" LEVELLIST8 *GetLevelListEntry(int ilevel)
{
    return &D_003E1C4C[ilevel];
}

namespace M_func_002109D0 {
struct X { char pad[8]; int sectorKey; char padc[0x2C]; int sectorXor; };   /* a file record: obfuscated sector (cf. GetDiscFileEntrySize) */
extern int D_003E1C38 __asm__("D_003E1C38");
struct X *GetLevelFileRecord(int i) __asm__("GetLevelFileRecord");
// True when two file records name the same file (same pointer, or equal key words).
extern "C" { static inline int FEqual(struct X *a, struct X *b)
{
    if (a == b)
        return 1;
    if (!a || !b)
        return 0;
    return (a->sectorKey ^ a->sectorXor) == (b->sectorKey ^ b->sectorXor);
} }
// Returns the index of the level whose file record equals the given one, or -1.
extern "C" int FindLevelIndexByFileRecord(struct X *px)
{
    int i;
    for (i = 0; i < D_003E1C38; i++) {
        if (FEqual(px, GetLevelFileRecord(i)))
            return i;
    }
    return -1;
}
} // namespace M_func_002109D0

// Returns the current level index.
int GetCurrentLevelIndex()
{
    return g_sw.loadFilter;
}

namespace M_NEpisodeCur {
extern int *D_003E1C48 __asm__("D_003E1C48");
int GetCurrentLevelIndex__Fv(void) __asm__("GetCurrentLevelIndex__Fv");
// Returns the episode of the current level, or -1.
extern "C" int GetCurrentEpisode(void)
{
    int i = GetCurrentLevelIndex__Fv();
    if (i == -1)
        return -1;
    return D_003E1C48[i];
}
} // namespace M_NEpisodeCur

namespace M_NWorldCur {
extern int *D_003E1C44 __asm__("D_003E1C44");
int GetCurrentLevelIndex__Fv(void) __asm__("GetCurrentLevelIndex__Fv");
// Returns the world of the current level, or -1.
extern "C" int GetCurrentWorld(void)
{
    int i = GetCurrentLevelIndex__Fv();
    if (i == -1)
        return -1;
    return D_003E1C44[i];
}
} // namespace M_NWorldCur

namespace M_NEpisodeBest {
void GetSaveVarByName(CRefData *r, char *s, int a) __asm__("GetSaveVarByName");
void Decref__4CRef(CRefData *r) __asm__("Decref__4CRef");
// Returns the best episode reached (save variable "episodeBest").
extern "C" int GetBestEpisode(void)
{
    CRefData r;
    int v;
    GetSaveVarByName(&r, "episodeBest", 0);
    v = r.u.n;
    Decref__4CRef(&r);
    return v;
}
} // namespace M_NEpisodeBest

// Always returns 0 (a stub). (inferred)
extern "C" int SwStubFalse()
{
    return 0;
}

// Returns the entry record of the current world (GetWorldEntry(GetCurrentWorld())).
extern "C" void GetCurrentWorldEntry()
{
    GetWorldEntry(GetCurrentWorld());
}

// Returns a world's entry record, or null when there is none (or it is empty).
/* The world's entry record, or 0 when there is none (or it is empty: its words 1 and 15 equal). */
extern "C" FILEREC *GetWorldEntry(int nWorld)
{
    FILEREC *prec;
    int f;
    if (nWorld == -1) {
        return 0;
    }
    prec = &D_003E1C50[nWorld];
    f = 0;
    if (prec != 0) {
        f = prec->aw[1] != prec->aw[15];
    }
    return (f == 0) ? 0 : prec;
}

namespace M_func_00210BE0 {
typedef struct { char *pch; char *pchOut; } ENT;
typedef struct { int c; ENT *a; int heapSize; byte *heapBuffer; } TBL;
extern char *FindSwLocalizedString__FP2SWi(SW *, char *) __asm__("FindSwLocalizedString__FP2SWi");
extern int strlen_(char *) __asm__("strlen");
extern char *PchzAllocCopy__FP4HEAPPci(HEAP *, char *, int) __asm__("PchzAllocCopy__FP4HEAPPci");

// Replaces each entry of a string table by its localized string (FindSwLangstr), copied into a linear heap.
extern "C" void LocalizeSwStringTable(TBL *table)
{
    int i;
    if (g_sw.clangstr == 0)
        return;
    CLinearHeap heap(table->heapSize, table->heapBuffer);
    for (i = 0; i < table->c; i++) {
        ENT *e = &table->a[i];
        e->pchOut = FindSwLocalizedString__FP2SWi(&g_sw, e->pch);
        if (strlen_(e->pchOut))
            e->pchOut = PchzAllocCopy__FP4HEAPPci(&heap, e->pchOut, 4);
        else
            e->pchOut = 0;
    }
}
} // namespace M_func_00210BE0

// Returns the value stored under key in a {key, value} table, or "" when absent.
/* The value stored under key in a {key, value} table, or the empty default. */
extern "C" void *FindKeyedTableValue(KVT *pkvt, int key)
{
    int i;
    for (i = 0; i < pkvt->c; i++) {
        if (key == pkvt->a[i].key && pkvt->a[i].pv != 0) {
            return pkvt->a[i].pv;
        }
    }
    return (char *)"";
}


// ---- Data owned by this TU, in address order (docs/guides/tu-merging.md "Data"). Defined after the functions
// so no function sees a definition (keeps the code as matched); the namespaced declarations above refer to these.
// The strings at 0x2A8A78 ("episodeBest"), 0x2A8A88 ("oidPoCurrent") and 0x2A8A98 ("") are the functions' own
// literals, pooled per TU (spelled as literals where they are used: tu-merging.md "Phase 6").









   /* zero-filled .data, the address goes into D_003E1C20 */
int D_003E1C30;
int D_003E1C34;   /* read by brx_sw.cpp's level loader and serialize.cpp */
int D_003E1C38;
int D_003E1C3C;
FILEREC *D_003E1C40;   /* g_arecLevel */
int *D_003E1C44;
int *D_003E1C48;
LEVELLIST8 *D_003E1C4C;   /* g_alevellist */
FILEREC *D_003E1C50;   /* g_aWorldEntryRecord */
int D_003E1C54;
int D_003E1C58;
char D_003E1C60[0x10];   /* its address goes into D_003E1C20 */

// ---- sw's .data in address order (tu-merging.md "Phase 6: one link mode"): one block after the functions (definition order is emission order); the declarations above are what the functions see.
float D_002C3E60 = 4000.0f;
float D_002C3E64 = 1500.0f;
float D_002C3E68 = 1.5f;
float D_002C3E6C = 1.0f;
int D_002C3E70[4] = {881, 877, 878, 879};
float D_002C3E80 = 65.0f;
SMPA D_002C3E88 = {500.0f, 0.0f, 0.25f, 2500.0f};
float D_002C3EA0[3] __attribute__((aligned(16))) = {2.0f, -2.0f, 0.0f};
float D_002C3EB0[2] = {0.0f, 1.0f};
char D_002C3EB8[8] = {0};
