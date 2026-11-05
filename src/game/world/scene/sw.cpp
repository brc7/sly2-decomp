// sw.cpp -- the world (SW, the global g_sw at D_003DD490): construction, load and delete, the per-frame update,
// hands-off / no-pause / letterbox counters, lighting, the object update list, level and world tables.
// TU 0x20D9D0..0x210EB0 (68 functions, address order).
//
// Types: sly2/sw.h's SW for the m2c bodies (one unnamed word, +0x3EA8, through a macro); level/world table
// records as small TU-private views. The hand-written per-function files keep their views in namespaces named
// after each function (as jt.cpp does), their C++-named definitions as C-linkage definitions of their literal
// symbols there. ResolveSwSaveVars (a Lilly port) is at file scope: it completes types basic.h only
// forward-declares (SVPATH, SAVEDATA, LOUPD...), which sw.h's SW refers to.
// g_sw is defined here with an inline constructor (SW_ctor) and destructor (func_00258348 with g++'s in-charge flag
// 2), and D_003E1C20 with an inline constructor: g++ generates the 96-instruction construct-and-destruct
// StaticInit_sw and the _GLOBAL_$D$ stub func_00210E90 next to the _GLOBAL_$I$ one (as cm.cpp does for g_cm).
// SLY2_ALIAS: __static_initialization_and_destruction_0=StaticInit_sw
// SLY2_ALIAS: _GLOBAL_$I$*=func_00210E70
// SLY2_ALIAS: _GLOBAL_$D$*=func_00210E90

#include "sly2/basic.h"
#include "sly2/lo.h"
#include "sly2/sw.h"
#include "sly2/slotheap.h"

/* g_sw: a view of the SW bytes whose inline constructor and destructor call the C-named SW_ctor and the linkonce
   destructor func_00258348 (cm.cpp's g_cm pattern). */
struct SWG;
extern "C" {
SW *SW_ctor(SW *psw);
void func_00258348(SWG *psw, int grfDelete);
extern char D_002C3EB8[], D_003E1C60[];
}
struct SWG
{
    char ab[sizeof(SW)];
    SWG() { SW_ctor((SW *)this); }
    ~SWG() { func_00258348(this, 2); }
} __attribute__((aligned(16)));
SWG D_003DD490;
/* D_003E1C20: four words built at startup (a table header {0, D_002C3EB8, 0, D_003E1C60}). */
struct O1C20
{
    int x0;
    void *x4;
    int x8;
    void *xC;
    O1C20() { x0 = 0; x4 = D_002C3EB8; x8 = 0; xC = D_003E1C60; }
};
O1C20 D_003E1C20;

// 0x210CF0 StaticInit_sw (g_sw and D_003E1C20, then the six slot pools; g_sw's destruction) and its stubs.
SLY2_INSTANTIATE_SLOT_POOLS()

/* An SW word sw.h leaves unnamed (+0x3EA8, in the padding before slPoctl): set by func_00210978. */
#define F3EA8(psw) (*(int *)((char *)(psw) + 0x3EA8))
/* A level-list entry (8 bytes) and a {key, value} table. */
struct LEVELLIST8 { int n0; int n4; };
struct KVE { int key; void *pv; };
struct KVT { int c; KVE *a; };
extern "C" {
extern char D_002E1E60[];   /* the letterbox object */
extern LEVELLIST8 *D_003E1C4C;  /* g_alevellist */
extern FILEREC *D_003E1C50;     /* g_aWorldEntryRecord */
extern int D_003E1110;          /* current level index */
extern char D_002A8A98[];       /* empty default for func_00210CA8 */
void ShowLetterbox(void *pletterbox);
void HideLetterbox(void *pletterbox);
void UpdateSwIllum(SW *psw);
int NWorldCur();
FILEREC *PrecWorldEntry(int nWorld);
}
void AddGrfusr(int grf);
void RemoveGrfusr(int grf);
LO *PloFindSwObject(SW *psw, int grffso, OID oid, LO *plo);

namespace M_SW_ctor {
struct DL {void *first,*last;int offset;};
struct POOL {int bytes,count,alignment;void *first;int extra[2];};
struct F2 {float x,y;};struct F3 {float x,y,z;};
struct SW {
    void * a;
    void * b;
    char unknown8[0x4];
    void * vt;
    DL list10;
    char unknown1C[0x4];
    DL list20;
    DL list2C;
    DL first[512];
    DL third[512];
    DL second[199];
    POOL pool398C;
    char unknown39A4[0x4C];
    DL list39F0;
    POOL pool39FC;
    char unknown3A14[0xEC];
    DL list3B00;
    char unknown3B0C[0xC4];
    DL list3BD0;
    DL list3BDC;
    char unknown3BE8[0x98];
    int n3C80;
    char unknown3C84[0x4];
    int n3C88;
    int n3C8C;
    POOL pool3C90;
    DL list3CA8;
    POOL pool3CB4;
    int n3CCC;
    char unknown3CD0[0x10];
    POOL pool3CE0;
    POOL pool3CF8;
    DL list3D10;
    DL list3D1C;
    DL list3D28;
    DL list3D34;
    POOL pool3D40;
    char unknown3D58[0x30];
    DL list3D88;
    POOL pool3D94;
    POOL pool3DAC;
    POOL pool3DC4;
    DL list3DDC;
    POOL pool3DE8;
    int n3E00;
    char unknown3E04[0xC];
    F3 triple;
    char unknown3E1C[0x4];
    float f3E20;
    int n3E24;
    char unknown3E28[0x1C];
    POOL pool3E44;
    char unknown3E5C[0x8];
    float f3E64;
    float f3E68;
    float f3E6C;
    char unknown3E70[0x3C];
    DL list3EAC;
    char unknown3EB8[0x58];
    int n3F10;
    char unknown3F14[0x10];
    DL list3F24;
    DL list3F30;
    DL list3F3C;
    DL list3F48;
    DL list3F54;
    DL list3F60;
    DL list3F6C;
    DL list3F78;
    DL list3F84;
    DL list3F90;
    DL list3F9C;
    DL list3FA8;
    DL list3FB4;
    DL list3FC0;
    DL list3FCC;
    DL list3FD8;
    DL list3FE4;
    DL list3FF0;
    DL list3FFC;
    POOL pool4008;
    char unknown4020[0x1C];
    POOL pool403C;
    POOL pool4054;
    char unknown406C[0x4];
    POOL pool4070;
    POOL pool4088;
    POOL pool40A0;
    POOL pool40B8;
    POOL pool40D0;
    POOL pool40E8;
    DL list4100;
    DL list410C;
    POOL pool4118;
    DL list4130;
    POOL pool413C;
    char unknown4154[0x400];
    int n4554;
    char unknown4558[0x38];
    char tags[0x20];
    DL list45B0;
    DL list45BC;
    char unknown45C8[0x8];
    F3 coefficients;
    char unknown45DC[0x4];
    F2 range;
    char unknown45E8[0x60];
    POOL pool4648;
    POOL pool4660;
    char unknown4678[0xC8];
    int n4740;
    char unknown4744[0xC];
    float f4750;
    float f4754;
};
extern "C" { extern char D_002DC018[]; }
extern "C" { extern float D_002C3E60,D_002C3E64,D_002C3E68,D_002C3E6C; }
extern "C" POOL *func_0011CCA8(POOL *,int,int,int);
extern "C" void InitDl__FP2DLi(DL *,int);
extern "C" void func_0011A050(DL *,int);
extern "C" int CbMaxRip(void);
extern "C" void *func_002333B4(void *,int,unsigned int);
/** @sly2-annot addr=0x20D9D0
 * Purpose: SW constructor: vt D_002DC018, ~30 CSplotheap pools, ~60 DLs, 512+199+512 hash buckets, illum HSV
 *   (180,255,255), fog/fov defaults
 * Info: proposed name SW_ctor (T1); class SW; TU sw
 * Notes: runtime5b: SW re-constructed in place on every level change (S56)
 * Confidence: mid (draft) [T1 0.75] - evidence: runtime (save-state RAM); sources: r7_1F8DD8+runtime
 * See: docs/engine/core-library.md#6-source-files, docs/engine/overview.md#3-world--level-lifecycle,
 *   docs/engine/world-sw.md#1-sw-the-world-singleton
 * @end-sly2-annot */
extern "C" SW *SW_ctor(SW *p)
{
    p->a=0;p->b=0;p->vt=D_002DC018;
    func_0011CCA8(&p->pool398C,0x14,0x100,0x10);
    func_0011CCA8(&p->pool39FC,0x14,0x100,0x10);
    func_0011CCA8(&p->pool3C90,0x14,0x100,0x10);
    func_0011CCA8(&p->pool3CB4,0xc,0x200,0x10);
    func_0011CCA8(&p->pool3CE0,0x80,0x300,0x10);
    func_0011CCA8(&p->pool3CF8,0x14,0x400,0x10);
    func_0011CCA8(&p->pool3D40,0xc0,0x100,0x10);
    func_0011CCA8(&p->pool3D94,0x60,0x100,0x10);
    func_0011CCA8(&p->pool3DAC,0x80,0x400,0x10);
    func_0011CCA8(&p->pool3DC4,0x14,0x8,0x10);
    func_0011CCA8(&p->pool3DE8,0x50,0x80,0x10);
    p->n3E00=0x80000000;
    func_0011CCA8(&p->pool3E44,0x30,0x40,0x10);
    func_0011CCA8(&p->pool4008,0x60,0x40,0x10);
    func_0011CCA8(&p->pool403C,0x1b0,0x10,0x10);
    int chosen=CbMaxRip();
    chosen=chosen?chosen:0xF0;
    func_0011CCA8(&p->pool4054,chosen,0x80,0x10);
    func_0011CCA8(&p->pool4070,0x460,0x28,0x10);
    func_0011CCA8(&p->pool4088,0x10c0,0x80,0x40);
    func_0011CCA8(&p->pool40A0,0x560,0x8,0x10);
    func_0011CCA8(&p->pool40B8,0x1c,0x200,0x10);
    func_0011CCA8(&p->pool40D0,0xf0,0x20,0x10);
    func_0011CCA8(&p->pool40E8,0x10,0x10,0x10);
    func_0011CCA8(&p->pool4118,0x60,0x20,0x10);
    func_0011CCA8(&p->pool413C,0x30,0x10,0x10);
    func_0011CCA8(&p->pool4648,0x40,0x10,0x10);
    func_0011CCA8(&p->pool4660,0xd0,0x10,0x10);
    InitDl__FP2DLi(&p->list10,0x24);
    InitDl__FP2DLi(&p->list20,0x4);
    InitDl__FP2DLi(&p->list2C,0xC);
    InitDl__FP2DLi(&p->list3D10,0x48);
    InitDl__FP2DLi(&p->list3D1C,0x40);
    InitDl__FP2DLi(&p->list3D28,0x50);
    InitDl__FP2DLi(&p->list3D34,0x58);
    InitDl__FP2DLi(&p->list3D88,0x24);
    func_0011A050(&p->list39F0,0x10);
    func_0011A050(&p->list3CA8,0x10);
    InitDl__FP2DLi(&p->list3F24,0xE0);
    InitDl__FP2DLi(&p->list3F30,0x1B4);
    InitDl__FP2DLi(&p->list3F3C,0x1B4);
    InitDl__FP2DLi(&p->list3F48,0x104);
    InitDl__FP2DLi(&p->list3F54,0x174);
    InitDl__FP2DLi(&p->list3F60,0x1B4);
    InitDl__FP2DLi(&p->list3F6C,0x104);
    InitDl__FP2DLi(&p->list3F78,0x104);
    InitDl__FP2DLi(&p->list3F84,0x104);
    InitDl__FP2DLi(&p->list3F90,0xE0);
    InitDl__FP2DLi(&p->list3FA8,0x154);
    InitDl__FP2DLi(&p->list3F9C,0x100);
    InitDl__FP2DLi(&p->list3DDC,0x1C);
    InitDl__FP2DLi(&p->list3FFC,0x58);
    InitDl__FP2DLi(&p->list3FB4,0xFE0);
    InitDl__FP2DLi(&p->list3FC0,0x174);
    func_0011A050(&p->list3EAC,0x84);
    func_0011A050(&p->list4100,0x18);
    func_0011A050(&p->list410C,0x40);
    InitDl__FP2DLi(&p->list4130,0x50);
    InitDl__FP2DLi(&p->list3FCC,0x20);
    InitDl__FP2DLi(&p->list3FD8,0x104);
    InitDl__FP2DLi(&p->list45B0,0x1D0);
    InitDl__FP2DLi(&p->list45BC,0x1E0);
    InitDl__FP2DLi(&p->list3B00,0x88);
    InitDl__FP2DLi(&p->list3FE4,0xE0);
    InitDl__FP2DLi(&p->list3FF0,0x130);
    InitDl__FP2DLi(&p->list3BD0,0xB4);
    func_0011A050(&p->list3BDC,0x4C);
    unsigned int i=0;
    for(;i<512;i++)func_0011A050(&p->first[i],0x1C);
    F3 *triple=&p->triple;
    F2 *range=&p->range;
    F3 *coefficients=&p->coefficients;
    char *tags=p->tags;
    DL *list=p->second;
    for(int n=198;n>=0;n--,list++)func_0011A050(list,0x20);
    i=0;
    for(;i<512;i++)func_0011A050(&p->third[i],12);
    float upper=D_002C3E6C;
    float lower=D_002C3E68;
    float end=D_002C3E64,start=D_002C3E60;
    p->triple.x=180.0f;
    triple->y=255.0f;
    triple->z=255.0f;
    p->n3C80=-1;
    p->f3E20=90.0f;
    p->f3E64=128.0f;
    p->f3E68=96.0f;
    p->f3E6C=160.0f;
    p->n3C88=393;
    p->n3C8C=-1;
    p->n3CCC=1;
    p->range.x=upper;
    p->n3F10=-2;
    p->n3E24=0;
    range->y=lower;
    float slope=(upper-lower)/(end-start);
    float intercept=(end*lower-start*upper)/(end-start);
    p->coefficients.x=intercept;
    coefficients->y=slope;
    coefficients->z=0.0f;
    p->n4740=-1;
    p->f4750=-60.0f;
    p->f4754=-60.0f;
    p->n4554=1;
    func_002333B4(tags,-1,0x20);
    return p;
}
} // namespace M_SW_ctor

namespace M_SetSwTransition { extern "C" {
typedef struct CHK { int a[0x4C / 4]; } CHK;
typedef struct CHKMGR { char pad[0x3BF0]; int fRestored; CHK chk; } CHKMGR;
void RestoreChkmgrFromCheckpoint(CHKMGR *pchkmgr, CHK *pchk) __asm__("SetSwTransition");
/** @sly2-annot addr=0x20DF80
 * Purpose: fRestored=1; copy CHK record
 * Info: proposed name SetSwTransition (EXISTING); TU sw
 * Notes: RENAME proposed -> SetSwTransition (existing_name_changes.csv) | runtime5b: CONFIRMED (code +
 *   runtime S56): posts the transition request SW+0x3BF0/+0x3BF4 consumed by ExecuteSwTransition; no
 *   checkpoint record
 * Confidence: high [EXISTING 0.95] - evidence: runtime (save-state RAM); arbitration; pre-existing symbol in
 *   symbol_addrs.txt; sources: r7_1F8DD8+arbitration+runtime
 * See: docs/engine/world-sw.md#world-sw-level-loading-transitions-hands-off-callbacks,
 *   docs/engine/world-sw.md#2-loading-a-level, docs/engine/world-sw.md#7-checkpoints-refuted
 * @end-sly2-annot */
void RestoreChkmgrFromCheckpoint(CHKMGR *pchkmgr, CHK *pchk)
{
    pchkmgr->fRestored = 1;
    pchkmgr->chk = *pchk;
}
} } // namespace M_SetSwTransition

namespace M_func_0020E040 { extern "C" {
extern int D_003E1080;
extern int D_00443740;
/** @sly2-annot addr=0x20E040
 * Purpose: True if D_003E1080!=0 or D_00443740==1 (checkpoint-manager state)
 * Info: proposed name FSwTransitionBusy (T2); TU sw
 * Confidence: low (draft) [T2 0.25] - evidence: static analysis; sources: r7_1F8DD8
 * See: docs/reference/functions/world-sw.md#sw-0x20d9d0-0x210eb0
 * @end-sly2-annot */
int func_0020E040(void) { if (D_003E1080 != 0) return 1; return D_00443740 == 1; }
} } // namespace M_func_0020E040

namespace M_DeleteSw { extern "C" {
struct S { char pad[0x3B18]; int h3B18; };
extern char D_002DEB30[], D_003D9E70[], D_002DDF40[], D_003DD040[], D_002E0CCC[];
extern int D_002E1E40, D_002F7900, D_002DD5BC, D_002DD648, D_003D4A54, D_002DE570, D_002DFB80, D_002DFBC4;
extern int D_003E1C40, D_003E1C44, D_003E1C48, D_003E1C4C, D_003E1C50, D_003DD48C;
void BroadcastGameEvent(void *p, int a, int b);
void AbortJob(int h);
void func_001D9408(void *p);
void func_001FF830(struct S *p);
void func_00161458(void *p);
void func_0014D650(void *p);
void ResetSndmgr(void *p);
void func_001EC7E8(void);
void SetRumbleRums__FP6RUMBLE4RUMS(void *p, int n);
void FreeSw__Fv(void);

/** @sly2-annot addr=0x20E068
 * Purpose: Tear down the current world before loading: game callback(1), free view/joy state, ResetSndmgr,
 *   clear ~15 global object pointers, FreeSw
 * Info: proposed name DeleteSw (T1); TU sw
 * Notes: names_v2 v1 proposed LoadChkpntFromBrx__FP6CHKPNTP18CBinaryInputStream (0.000)
 * Confidence: mid (draft) [T1 0.60] - evidence: static naming pass; sources: r7_1F8DD8
 * See: docs/engine/overview.md#3-world--level-lifecycle, docs/engine/world-sw.md#2-loading-a-level,
 *   docs/engine/world-sw.md#10-key-functions
 * @end-sly2-annot */
void DeleteSw(struct S *p)
{
    void *pv = D_002DEB30;
    BroadcastGameEvent(pv, 1, 0);
    if (p->h3B18) {
        AbortJob(p->h3B18);
        p->h3B18 = 0;
    }
    func_001D9408(D_003D9E70);
    func_001FF830(p);
    func_00161458(pv);
    func_0014D650(D_002DDF40);
    ResetSndmgr(D_003DD040);
    func_001EC7E8();
    SetRumbleRums__FP6RUMBLE4RUMS(D_002E0CCC, 3);
    D_002E1E40 = 0;
    D_002F7900 = 0;
    D_002DD5BC = 0;
    D_002DD648 = 0;
    D_003D4A54 = 0;
    D_002DE570 = 0;
    D_002DFB80 = 0;
    D_002DFBC4 = 0;
    D_003E1C40 = 0;
    D_003E1C44 = 0;
    D_003E1C48 = 0;
    D_003E1C4C = 0;
    D_003E1C50 = 0;
    D_003DD48C = 0;
    FreeSw__Fv();
}
} } // namespace M_DeleteSw

namespace M_func_0020E180 { extern "C" {
struct MSG { int cb; int nKind; };
struct H { int a; int b; int c; int d; int e; int f; char dl[0x10]; };
struct S {
    char pad0[0x3BF4]; int fl3BF4;
    char pad1[0x3CD4 - 0x3BF8]; void *a3CD4[3];
    char pad2[0x3E38 - 0x3CE0]; int n3E38;
    char pad3[0x3F0C - 0x3E3C]; struct H *ph; char pad4[0x3F20 - 0x3F10]; void *p3F20;
    char pad5[0x45E8 - 0x3F24]; char a45E8[2][0x28];
};
struct ZPR { char ab[0x630]; };
extern char D_002DEB30[];
void *func_001AE1D0(struct MSG *pmsg);
void *func_001AE2A0(struct MSG *pmsg);
void func_0011A050(void *p, int n);
void func_001703F8(struct ZPR *pz, int a, int b);
void func_001BE398(void *p, struct ZPR *pz);
void BroadcastGameEvent(void *p, int a, int b);

/** @sly2-annot addr=0x20E180
 * Purpose: Pre-load setup of the fresh SW: 3x0x4800 and 0x8000 work buffers, 0x28 header with DL, InitZpr x2,
 *   game callback(2)
 * Info: proposed name InitSwLoadBuffers (T2); TU sw
 * Notes: names_v2 v1 proposed BindChkpnt__FP6CHKPNT (0.004)
 * Confidence: low (draft) [T2 0.40] - evidence: static naming pass; sources: r7_1F8DD8
 * See: docs/reference/functions/world-sw.md#sw-0x20d9d0-0x210eb0
 * @end-sly2-annot */
void func_0020E180(struct S *p)
{
    struct MSG msg;
    struct ZPR zpr;
    int i;
    unsigned int j;
    for (i = 0; i < 3; i++) {
        msg.cb = 0x4800;
        msg.nKind = 0x10;
        p->a3CD4[i] = func_001AE1D0(&msg);
    }
    msg.cb = 0x8000;
    msg.nKind = 0x10;
    p->p3F20 = func_001AE2A0(&msg);
    msg.cb = 0x28;
    msg.nKind = 0x10;
    p->ph = (__typeof__(p->ph))func_001AE2A0(&msg);
    p->ph->a = -1;
    p->ph->c = -1;
    func_0011A050(p->ph->dl, 0x14);
    func_001703F8(&zpr, 0x1400, 0x1800);
    for (j = 0; j < 2; j++)
        func_001BE398(p->a45E8[j], &zpr);
    if (p->fl3BF4 & 0x20)
        p->n3E38 = 1;
    BroadcastGameEvent(D_002DEB30, 2, 0);
}
} } // namespace M_func_0020E180

namespace M_func_0020E2B0 { extern "C" {
struct O { char pad[0xB8]; int nB8; char p2[0xC]; int nC8; };
struct S { char pad[0x3E5C]; int n; };
int CploFindSwObjectsByClass(struct S *p, int a, int b, int c, int cMax, struct O **apo);
void func_001A6D28(struct O *o, int n);
/** @sly2-annot addr=0x20E2B0
 * Purpose: Find up to 16 objects of class MS (0x31); keep the one with largest +0xB8, store its +0xC8 and
 *   call 0x1A6D28(po,0xFF)
 * Info: proposed name BindSwMs (T2); TU sw
 * Confidence: low (draft) [T2 0.25] - evidence: static analysis; sources: r7_1F8DD8
 * See: docs/reference/functions/world-sw.md#sw-0x20d9d0-0x210eb0
 * @end-sly2-annot */
void func_0020E2B0(struct S *p)
{
    struct O *apo[16];
    int c = CploFindSwObjectsByClass(p, 5, 0x31, 0, 16, apo);
    int nMax = 0;
    struct O *poBest = 0;
    int i;
    for (i = 0; i < c; i++) {
        if (apo[i]->nB8 > nMax) {
            nMax = apo[i]->nB8;
            poBest = apo[i];
        }
    }
    if (poBest) {
        p->n = poBest->nC8;
        func_001A6D28(poBest, 0xFF);
    }
}
} } // namespace M_func_0020E2B0

namespace M_func_0020E338 {
class OBase { public: int a, b, c; virtual void V1(); virtual void V2(); virtual void V3(); virtual void V4(); virtual void V5();
 virtual void V6(); virtual void V7(); virtual void V8(); virtual void V9(); };
class O : public OBase { public: char pad[0x54 - 0x10]; unsigned int x54; char p2[8]; float g60; float g64; int p68; int n6C; };
struct S { char pad[0x3E60]; O *o; };
extern "C" O *PloNew__F3CIDP2SO3OIDi(int a, void *p, int b, int c);
/** @sly2-annot addr=0x20E338
 * Purpose: PloNew(cid 0x74) with params 0.1/0.25, flags 0x80200000, then vt9 (add); stored at SW+0x3E60
 * Info: proposed name CreateSwDefaultObject74 (T2); TU sw
 * Confidence: low (draft) [T2 0.25] - evidence: static analysis; sources: r7_1F8DD8
 * See: docs/reference/functions/world-sw.md#sw-0x20d9d0-0x210eb0
 * @end-sly2-annot */
extern "C" void func_0020E338(S *p)
{
    p->o = PloNew__F3CIDP2SO3OIDi(0x74, 0, -1, -1);
    p->o->g60 = 0.1f;
    p->o->g64 = 0.25f;
    p->o->x54 = 0x80200000;
    p->o->n6C = 1;
    p->o->V9();
}
} // namespace M_func_0020E338

// A Lilly port at file scope: its types complete basic.h forward declarations (used by the m2c bodies below too).
// ResolveSwSaveVars: ported from Lilly's personal Sly 2 decomp (src/P2/sw.c), reduced to the declarations it needs.
// Every global is named by its address; the function keeps its readable name and gets the team symbol through __asm__.
// Globals: D_002DEB30 (g_game), D_002E1E54 (g_nLang), D_003D4A60 (g_savedata), D_002A39E8 (g_apchzWorld)
// 0x0020E3C0 ResolveSwSaveVars__FP2SW  (T1037)
// Once the level file has named its save variables (LoadSaveVarsFromBrx), looks up the paths the game itself reads and writes by name (SW 0x3B1C..:
// the language and sound settings in "flags", the job and world of this level, each world's "dt" and "percent_complete") and stores the current
// language, job and world; "episodeBest" is raised to the current world when that is a new best. Then tells the game (event 3).
#include "sly2/basic.h"
#include "sly2/lo.h"
#include "sly2/sw.h"
struct SLOT
{
      SLOT *pslotNext;
};
extern GAME D_002DEB30;
struct GRABSPOTDATA
{
      TN *ptn;
      undefined4 unk_0x04;
      float sUnk08;
      undefined4 unk_0x0c;
      int nPriority;
      int nUnk14;
      int nGroup;
      uint grfUnk1c;
};
struct GRABSPOT : public GRABSPOTDATA
{
};
struct CELL
{
      qword plane;
      CELL *pcellNeg;
      CELL *pcellPos;
      int ileafNeg;
      int ileafPos;
};
struct LANGSTR
{
      int id;
      char *pchz;
};
struct SVN
{
      int nHash;
      char *pchzName;
      int id;
      SVN *psvnNext;
};
struct SVPATH
{
      int cid;
      short *aid;
};
struct SAVEDATA
{
      byte ab[0x5400];
      uint nVersion;
      int nSum;
};
struct LOUPD
{
      int grf;
      LO *plo;
};
int IlevelCur();
extern "C" void BroadcastGameEvent(GAME *pgame, int nEvent, LO *plo);
union TAG
{
    int m_n;
    float m_g;
    int m_bool;
    uint m_symid;
    BIFK m_bifk;
    const VECTOR4* m_pvec4;
    CPair* m_ppair;
    CProc* m_pproc;
    VECTOR* m_pvector;
    MATRIX4* m_pmatrix;
    CLQ* m_pclq;
    LM* m_plm;
    SMP* m_psmp;
    BASIC* m_pbasic;
    CMethod* m_pmethod;
};
class CRef
{
public:
    TAGK m_tagk;
    TAG m_tag;
    CRef() { m_tagk = TAGK_Nil; }
    CRef(TAGK tagk) { SetTag(tagk); }
    CRef(TAGK tagk, int n) { m_tagk = tagk; m_tag.m_n = n; }
    CRef(TAGK tagk, float g) { m_tagk = tagk; m_tag.m_g = g; }
    CRef(const CRef &ref);
    ~CRef() { Decref(); }
    CRef &operator=(const CRef &ref);
    bool operator==(const CRef &ref);
    void Decref();
    void SetTag(TAGK tagk);
};
extern int D_002E1E54;
extern SAVEDATA D_003D4A60;
extern const char *D_002A39E8[9];
extern "C" SVPATH *PkeyNewSaveKey(const char *pchz, ...);
extern "C" CRef RefSetGlobalSaveVar(SVPATH *psvp, const CRef &ref, SAVEDATA *psave);
inline void SetSaveVarRef(SVPATH *psvp, CRef ref) { RefSetGlobalSaveVar(psvp, ref, &D_003D4A60); }
inline void SetSaveVarInt(SVPATH *psvp, int n)
{
    CRef ref((TAGK)0x15, n);
    SetSaveVarRef(psvp, ref);
}
extern "C" CRef RefSetSaveVarV(const CRef &ref, const char *pchz, ...);
extern "C" int NWorldCur();
extern "C" int NEpisodeBest();
void ResolveSwSaveVars(SW *psw) __asm__("ResolveSwSaveVars__FP2SW");
/** @sly2-annot addr=0x20E3C0
 * Purpose: Look up save-variable paths the game reads (language, sound flags, job/world, per-world
 *   dt/percent_complete), store current language/job/world, raise episodeBest, notify game (event 3)
 * Info: proposed name ResolveSwSaveVars (T1); TU sw
 * Notes: names_v2 C proposed LoadSwFromBrx__FP2SWP18CBinaryInputStream (0.438)
 * Confidence: high [T1 0.85] - evidence: static naming pass; sources: r7_1F8DD8+verdicts_tu_r7r8
 * See: docs/engine/save-memcard.md#1-the-save-blob-g_gs-savecpp-0x1d7b88-0x1d93d8,
 *   docs/engine/world-sw.md#loadswfrombrx-0x13da18-matched-hand-file,
 *   docs/engine/world-sw.md#10-key-functions
 * @end-sly2-annot */
void ResolveSwSaveVars(SW *psw)
{
    psw->psvpDt = PkeyNewSaveKey("dt", 0);
    psw->psvpWidCurrent = PkeyNewSaveKey("widCurrent", 0);
    psw->psvpEpisodeCurrent = PkeyNewSaveKey("episodeCurrent", 0);
    psw->psvpCcoin = PkeyNewSaveKey("loot", "ccoin", 0);
    psw->psvpLang = PkeyNewSaveKey("flags", "lang", 0);
    psw->psvpMixerMode = PkeyNewSaveKey("flags", "nMixerMode", 0);
    psw->psvpMusicVolume = PkeyNewSaveKey("flags", "uMusicVolume", 0);
    psw->psvpEffectsVolume = PkeyNewSaveKey("flags", "uEffectsVolume", 0);
    psw->psvpUseHeadset = PkeyNewSaveKey("flags", "fUseHeadset", 0);
    psw->psvpScreenCenterX = PkeyNewSaveKey("flags", "dxScreenCenter", 0);
    psw->psvpScreenCenterY = PkeyNewSaveKey("flags", "dyScreenCenter", 0);
    psw->psvpNoVibration = PkeyNewSaveKey("flags", "fNoVibration", 0);
    psw->psvpReverseLookAround = PkeyNewSaveKey("flags", "fReverseLookAround", 0);
    psw->psvpReverseAim = PkeyNewSaveKey("flags", "fReverseAim", 0);
    psw->psvpAutosaveWarned = PkeyNewSaveKey("flags", "fAutosaveWarned", 0);
    for (int iworld = 0; iworld < 9; iworld++)
    {
        psw->apsvpEpisodeDt[iworld] = PkeyNewSaveKey("episode", D_002A39E8[iworld], "dt", 0);
        psw->apsvpEpisodePercent[iworld] = PkeyNewSaveKey("episode", D_002A39E8[iworld], "percent_complete", 0);
    }
    SetSaveVarInt(psw->psvpLang, D_002E1E54);
    SetSaveVarInt(psw->psvpWidCurrent, IlevelCur());
    SetSaveVarInt(psw->psvpEpisodeCurrent, NWorldCur());
    if (NWorldCur() > NEpisodeBest())
        RefSetSaveVarV(CRef((TAGK)0x15, NWorldCur()), "episodeBest", 0);
    BroadcastGameEvent(&D_002DEB30, 3, 0);
}

namespace M_PostSwLoad { extern "C" {
struct VE { short d; short pad; void (*pf)(void *); };
struct O { char pad[0xC]; struct VE *vt; };
struct S {
    char pad0[0x39D0]; int n39D0; int pad39D4; int n39D8; int pad39DC; struct O **ppoFirst; struct O **ppoLast;
    char pad1[0x403C - 0x39E8]; char a403C[0x18]; char a4054[0x1C]; char a4070[0x18]; char a4088[4];
    char pad2[0x45CC - 0x408C]; void *p45CC;
};
extern char D_002DDF40[], D_002DEB30[], D_003D9E70[];
void func_0011CDD8(void *p);
void func_0020E338(struct S *p);
void func_0020E2B0(struct S *p);
void func_001FF828(struct S *p);
void InitSwProxySpares(struct S *p);
void func_0020F710(struct S *p);
void func_001569B8(struct S *p);
void EnsureSwDefaultLight(struct S *p);
void PostLoadSwPathzones(struct S *p);
void func_00210940(struct S *p);
void func_001957C0(struct S *p);
void func_001D9520(void *p);
void ProcessSwCallbacks(struct S *p);
void func_001C1260(void *p);
void SetupCm__FP2CM(void *p);
void BroadcastGameEvent(void *p, int a, int b);

/** @sly2-annot addr=0x20E740
 * Purpose: After the BRX is read: reset transient pools, bind HUD/aux objects, run post-load callbacks (vt15)
 *   until queue empty, StartupCm-like camera reset, game callback(4)
 * Info: proposed name PostSwLoad (T1); TU sw
 * Notes: names_v2 v1 proposed UpdateChkpnt__FP6CHKPNTf (0.000)
 * Confidence: mid (draft) [T1 0.55] - evidence: static naming pass; sources: r7_1F8DD8
 * See: docs/engine/overview.md#2-the-frame-loop-exact-order,
 *   docs/engine/overview.md#3-world--level-lifecycle, docs/engine/world-sw.md#2-loading-a-level
 * @end-sly2-annot */
void PostSwLoad(struct S *p)
{
    struct O **ppo;
    func_0011CDD8(p->a403C);
    func_0011CDD8(p->a4054);
    func_0011CDD8(p->a4070);
    func_0011CDD8(p->a4088);
    func_0020E338(p);
    func_0020E2B0(p);
    func_001FF828(p);
    InitSwProxySpares(p);
    func_0020F710(p);
    func_001569B8(p);
    EnsureSwDefaultLight(p);
    PostLoadSwPathzones(p);
    func_00210940(p);
    func_001957C0(p);
    func_001D9520(D_003D9E70);
    while (ProcessSwCallbacks(p), p->ppoLast != p->ppoFirst) {
        for (ppo = p->ppoFirst; ppo < p->ppoLast; ppo++)
            (*ppo)->vt[15].pf((char *)*ppo + (*ppo)->vt[15].d);
        p->ppoLast = p->ppoFirst;
    }
    p->ppoLast = 0;
    p->ppoFirst = 0;
    p->n39D8 = 0;
    p->n39D0 = 0;
    func_001C1260(p->p45CC);
    SetupCm__FP2CM(D_002DDF40);
    BroadcastGameEvent(D_002DEB30, 4, 0);
}
} } // namespace M_PostSwLoad

namespace M_ExecuteSwTransition {
// Load the pending world and restore actor and view state.
typedef unsigned int uint;
struct HEADER {int words[16];};
struct REQUEST {int flags;HEADER header;int objectId,objectKey;};
struct KEY {int name,tag,kind;char data[0x104];};
struct VT {short delta,unused;void *function;};
struct WIDGET {char pad0[0x58];int shown;};
struct ACTOR {char pad0[0xC];VT *vtable;char pad10[8];int tag;char pad1C[0xF10];WIDGET *widget;};
struct CONTEXT {char pad0[0x148];int count;struct {int tag,value;} objects[1];};
struct SW {char pad0[0x3BF0];int pending;REQUEST request;HEADER current;int unused3C80,unused3C84,objectId,objectKey;char pad3C90[0x228];int actorCount;ACTOR *actors[1];char pad3EC0[0x8AC];float resumeTime;char pad4770[0x20];};
struct FRAME;
struct STREAM;
struct REF;
extern "C" { extern char D_002DDED0[],D_002DDF40[],D_002DEB30[],D_002F7980[],D_002F66B0[],D_00443740[],D_002E1E88[]; }
extern "C" { extern char D_002A8A88[]; }
extern "C" { extern ACTOR *D_002E1E40,*D_002DD5BC,*D_002F7900; }
extern "C" { extern SW D_003DD490; }
extern "C" {
void FlushFrames__Fi(int);
FRAME *STACKSCOPE_ctor(FRAME *);
void STACKSCOPE_ctor_1AE4A0(FRAME *,int);
void SetPhase__F5PHASE(int);
void func_00258248(KEY *);
STREAM *func_001848A8(STREAM *,int,void *,int);
int func_00184D60(STREAM *,KEY *);
void func_00184950(STREAM *,int);
void SetDecompress__18CBinaryInputStreamiPUc(STREAM *,int,void *);
void CStreamBis_Refill(STREAM *);
void DeleteSw(SW *);
void ResetClock(void *);
void AddGrfusr__Fi(int);
void *func_002333B4(void *,int,uint);
SW *SW_ctor(SW *);
int CMpeg_ExecuteOids(void *);
void InitCm__FP2CM(void *);
void func_0020E180(SW *);
void LoadSwFromBrx__FP2SWP18CBinaryInputStream(SW *,STREAM *);
void CStreamBis_Close(STREAM *);
void PostSwLoad(SW *);
void func_001618F8(SW *,float);
void ClearCheatItems(void);
CONTEXT *PwarpFromOid(SW *,int,int);
void TriggerWarp(CONTEXT *);
ACTOR *PpoHero(SW *);
ACTOR *PpoCur(SW *);
void Decref__4CRef(REF *);
ACTOR *PloFindSwObject__FP2SWi3OIDP2LO(SW *,int,int,void *);
int FIsBasicDerivedFrom__FP5BASIC3CID(ACTOR *,int);
int GetPoCtlState(ACTOR *);
int NWorldCur(void);
void SetWidgetState(WIDGET *,int);
void UpdateCmLast__FP2CM(void *);
void CutCm__FP2CM(void *);
void BroadcastGameEvent(void *,int,void *);
void ClearPhase(int);
void func_0022B420(void *);
void func_001A4E80(void *);
}
struct FRAME {FRAME(){}~FRAME(){}};
struct STREAM {int flags;char pad4[0x2C];void *vtable;int spoolBytes;char *spool;int fileHandle,position,writePosition,capacity,input,secondaryHandle,state54,state58,state5C,state60;};
struct REF {int tag,value;REF(const REF &);~REF(){Decref__4CRef(this);}};
extern "C" REF RefGetSaveVarV(const char *,...);
static inline float Time(){return *(float *)D_002DDED0;}
static inline int Same(const HEADER *a,const HEADER *b){if(a==b)return true;if(!b)return false;return (a->words[2]^a->words[14])==(b->words[2]^b->words[14]);}
static inline int SetKey(KEY *key,const HEADER *header){if(!header)return false;int c=header->words[0],a=header->words[13],d=header->words[15],b=header->words[1];key->kind=2;key->tag=b^d;key->name=(c^a)+0xCA0;return key->tag!=0;}
static inline void Activate(ACTOR *actor){VT *entry=&actor->vtable[8];((void (*)(void *))entry->function)((char *)actor+entry->delta);}
static inline void Enable(ACTOR *actor,int mode){VT *entry=&actor->vtable[79];((void (*)(void *,int))entry->function)((char *)actor+entry->delta,mode);}
static inline void Attach(ACTOR *actor,CONTEXT *context){VT *entry=&actor->vtable[38];((void (*)(void *,CONTEXT *))entry->function)((char *)actor+entry->delta,context);}
static inline void Hide(ACTOR *actor){VT *entry=&actor->vtable[9];((void (*)(void *))entry->function)((char *)actor+entry->delta);}
static inline int NonZero(int v){int r; __asm__("sltu %0,$zero,%1":"=r"(r):"r"(v));return r;}
static inline STREAM *StreamHome(STREAM *home){register STREAM *result __asm__("$4");__asm__("la %0,%a1":"=r"(result):"p"(home));return result;}
/** @sly2-annot addr=0x20E890
 * Purpose: Level transition: if a request is pending, open level file, DeleteSw, memset+construct g_sw,
 *   InitSwLoadBuffers, LoadSwFromBrx, PostSwLoad, restore checkpoint/actor (oidPoCurrent), camera, game
 *   callback(5)
 * Info: proposed name ExecuteSwTransition (T1); TU sw
 * Notes: runtime5b: SW memset + SW::SW before LoadSwFromBrx (S56); step order corrected in docs
 * Confidence: mid (draft) [T1 0.60] - evidence: runtime (save-state RAM); sources: r7_1F8DD8+runtime
 * See: docs/engine/overview.md#2-the-frame-loop-exact-order,
 *   docs/engine/overview.md#3-world--level-lifecycle, docs/engine/overview.md#6-subsystem-map
 * @end-sly2-annot */
extern "C" void ExecuteSwTransition(SW *p)
{
    if(!p->pending)return;
    FlushFrames__Fi(1);
    REQUEST request=p->request;
    REQUEST *destination=&p->request;
    p->pending=0;
    float resume;
    if(request.flags&0xCD)resume=0.0f;
    else{float remaining=p->resumeTime-Time();resume=remaining>0.0f?remaining:0.0f;}
    FRAME frame;
    FRAME *scope=&frame;
    STACKSCOPE_ctor(scope);
    SetPhase__F5PHASE(2);
    KEY key;
    KEY *keyPointer=&key;
    keyPointer->kind=2;
    func_00258248(keyPointer);
    int hasFile=NonZero(request.header.words[1]^request.header.words[15]);
    if(hasFile&&!SetKey(&key,&request.header)){STACKSCOPE_ctor_1AE4A0(scope,2);return;}
    HEADER *current=&p->current; const HEADER *requested=&request.header; int changed=!Same(requested,current);
    STREAM stream;
    register STREAM *argument __asm__("$4")=StreamHome(&stream);
    register STREAM *reader __asm__("$21");
    __asm__("daddu %0,%1,$zero":"=r"(reader):"r"(argument));
    func_001848A8(argument,0xD8000,D_002F7980,0);
    if(hasFile&&!func_00184D60(reader,&key)){func_00184950(reader,2);STACKSCOPE_ctor_1AE4A0(scope,2);return;}
    SetDecompress__18CBinaryInputStreamiPUc(reader,0x4000,(void *)0x70000000);
    CStreamBis_Refill(reader);
    DeleteSw(p);
    ResetClock(D_002DDED0);
    AddGrfusr__Fi(8);
    func_002333B4(&D_003DD490,0,0x4790);
    func_002333B4(&D_003DD490,0,0x4790);
    SW_ctor(&D_003DD490);
    if(CMpeg_ExecuteOids(D_002F66B0))changed=1;
    InitCm__FP2CM(D_002DDF40);
    p->current=request.header;
    *destination=request;
    func_0020E180(p);
    if(hasFile){LoadSwFromBrx__FP2SWP18CBinaryInputStream(p,reader);CStreamBis_Close(reader);}
    PostSwLoad(p);
    if(resume>0.0f)func_001618F8(p,resume);
    if(changed||(p->request.flags&0x40))ClearCheatItems();
    CONTEXT *context=PwarpFromOid(p,p->request.objectId,p->request.objectKey);
    if(context){TriggerWarp(context);p->objectId=p->request.objectId;p->objectKey=p->request.objectKey;}
    if(!PpoHero(p)&&!PpoCur(p)&&p->actorCount>0){
        ACTOR *selected=0;
        int actorId;
        {REF value=RefGetSaveVarV(D_002A8A88,0);actorId=value.value;}
        if(actorId!=-1)selected=PloFindSwObject__FP2SWi3OIDP2LO(p,5,actorId,0);
        if(!selected)selected=D_002E1E40;
        if(!selected)selected=D_002DD5BC;
        if(!selected)selected=D_002F7900;
        if(selected){
            Activate(selected);
            Enable(selected,1);
            if(context){
                int i;
                for(i=0;i<context->count;i++)if(context->objects[i].tag==selected->tag)break;
                if(i>=context->count)Attach(selected,context);
            }
            for(int i=0;i<p->actorCount;i++){
                ACTOR *actor=p->actors[i];
                if(actor!=selected&&FIsBasicDerivedFrom__FP5BASIC3CID(actor,0x34)&&!GetPoCtlState(actor))Hide(actor);
            }
        }
    }
    if(!NWorldCur()){
        ACTOR *actor=PpoHero(p);
        if(actor&&FIsBasicDerivedFrom__FP5BASIC3CID(actor,0x34)&&actor->widget&&!actor->widget->shown)SetWidgetState(actor->widget,1);
    }
    UpdateCmLast__FP2CM(D_002DDF40);
    CutCm__FP2CM(D_002DDF40);
    BroadcastGameEvent(D_002DEB30,5,0);
    ClearPhase(2);
    func_0022B420(D_00443740);
    if(!hasFile)func_001A4E80(D_002E1E88);
    func_00184950(reader,2);
    STACKSCOPE_ctor_1AE4A0(scope,2);
}
} // namespace M_ExecuteSwTransition

namespace M_IncrementSwHandsOff { extern "C" {
struct S { char pad[0x3D78]; int c; };
void AddGrfusr__Fi(int n);
void AcquireSwLetterbox(struct S *p);
void func_0020F198(struct S *p);
/** @sly2-annot addr=0x20EFC0
 * Purpose: ++SW+0x3D78 (first -> AddGrfusr(2)); grf&1 AcquireSwLetterbox, grf&2 ++count 0x3D84
 * Info: proposed name IncrementSwHandsOff (T1); TU sw
 * Notes: runtime5b/runtime6: hands-off counter SW+0x3D78 = 1 in pause, cutscene and job banner
 * Confidence: high [T1 0.90] - evidence: runtime (save-state RAM); sources: r7_1F8DD8+verify_side+runtime
 * See: docs/engine/world-sw.md#5-hands-off-and-locks, docs/engine/world-sw.md#10-key-functions,
 *   docs/reference/functions/cutscenes.md#relay-0x1dfad8-0x1e5d80
 * @end-sly2-annot */
void IncrementSwHandsOff(struct S *p, int fl)
{
    if (++p->c == 1) AddGrfusr__Fi(2);
    if (fl & 1) AcquireSwLetterbox(p);
    if (fl & 2) func_0020F198(p);
}
} } // namespace M_IncrementSwHandsOff

namespace M_DecrementSwHandsOff { extern "C" {
struct S { char pad[0x3D78]; int n; };
struct G;
extern struct G D_002DFC00;
void RemoveGrfusr__Fi(int n);
void func_00187E08(struct G *pg, int n);
void ReleaseSwLetterbox(struct S *p);
void func_0020F1A8(struct S *p);
/** @sly2-annot addr=0x20F030
 * Purpose: --SW+0x3D78 (last -> RemoveGrfusr(2), SetJoyBtnHandled); grf&1 ReleaseSwLetterbox, grf&2 --0x3D84
 * Info: proposed name DecrementSwHandsOff (T1); TU sw
 * Notes: names_v2 v1 proposed FUN_001417f0 (0.000) | runtime6: lock counters return to 0 after HandsOff(0,3)
 * Confidence: high [T1 0.85] - evidence: runtime (save-state RAM); static naming pass; sources:
 *   r7_1F8DD8+runtime
 * See: docs/engine/world-sw.md#5-hands-off-and-locks, docs/engine/world-sw.md#10-key-functions,
 *   docs/reference/functions/cutscenes.md#relay-0x1dfad8-0x1e5d80
 * @end-sly2-annot */
void DecrementSwHandsOff(struct S *p, int grf)
{
    if (--p->n == 0) {
        RemoveGrfusr__Fi(2);
        func_00187E08(&D_002DFC00, 8);
    }
    if (grf & 1) ReleaseSwLetterbox(p);
    if (grf & 2) func_0020F1A8(p);
}
} } // namespace M_DecrementSwHandsOff

/** @sly2-annot addr=0x20F0B0
 * Purpose: SW+0x3D78 > 0
 * Info: proposed name FSwHandsOff (T1); TU sw
 * Notes: names_v2 C proposed GetSwPlayerSuck__FP2SWPf (0.200)
 * Confidence: high [T1 0.85] - evidence: static naming pass; sources: r7_1F8DD8+verdicts_tu_r7r8
 * See: docs/engine/world-sw.md#5-hands-off-and-locks, docs/engine/world-sw.md#10-key-functions,
 *   docs/reference/functions/world-sw.md#sw-0x20d9d0-0x210eb0
 * @end-sly2-annot */
extern "C" int FSwHandsOff(SW *psw)
{
    return psw->cHandsOff > 0;
}

/** @sly2-annot addr=0x20F0C0
 * Purpose: ++SW+0x3D7C no-pause counter (first -> AddGrfusr(4)); used by SetJsgHandsOff and SetJtModeState
 *   kind 1 (recon)
 * Info: proposed name IncrementSwNoPause (T1); TU sw
 * Notes: v3: superseded IncrementSwUsrLock | runtime6 N1: Lilly jtmode.c calls it IncrementSwNoPause; S57
 *   0x3D7C=1 in recon mode without hands-off; v2 IncrementSwUsrLock
 * Confidence: mid (draft) [T1 0.70] - evidence: runtime (save-state RAM); Sly 1 / Lilly reference; sources:
 *   r7_1F8DD8+verdicts_B+runtime6
 * See: docs/engine/cutscenes.md#1-relay-and-jsg-the-sequence-engine-tu-0x1dfad8-0x1e5d80-file-name-unknown-lilly-jsgc,
 *   docs/engine/world-sw.md#5-hands-off-and-locks, docs/engine/world-sw.md#10-key-functions
 * @end-sly2-annot */
extern "C" void func_0020F0C0(SW *psw)
{
    if (++psw->cNoPause == 1) {
        AddGrfusr(4);
    }
}

/** @sly2-annot addr=0x20F0F0
 * Purpose: --SW+0x3D7C no-pause counter (last -> RemoveGrfusr(4))
 * Info: proposed name DecrementSwNoPause (T1); TU sw
 * Notes: names_v2 B proposed DecrementSwHandsOff__FP2SW (0.846) | v3: superseded DecrementSwUsrLock |
 *   runtime6 N1 pair; v2 DecrementSwUsrLock
 * Confidence: mid (draft) [T1 0.70] - evidence: runtime (save-state RAM); static naming pass; sources:
 *   r7_1F8DD8+runtime6
 * See: docs/engine/world-sw.md#5-hands-off-and-locks,
 *   docs/guides/naming-workflow.md#1-where-things-stand-master_names_v2,
 *   docs/reference/functions/cutscenes.md#relay-0x1dfad8-0x1e5d80
 * @end-sly2-annot */
extern "C" void func_0020F0F0(SW *psw)
{
    if (--psw->cNoPause == 0) {
        RemoveGrfusr(4);
    }
}

/** @sly2-annot addr=0x20F128
 * Purpose: ++SW+0x3D80; first -> show letterbox object (0x1A3078(&D_002E1E60): hides subtitles, vt49)
 * Info: proposed name AcquireSwLetterbox (T1); TU sw
 * Confidence: mid (draft) [T1 0.50] - evidence: static analysis; sources: r7_1F8DD8
 * See: docs/engine/world-sw.md#5-hands-off-and-locks,
 *   docs/reference/functions/world-sw.md#sw-0x20d9d0-0x210eb0
 * @end-sly2-annot */
extern "C" void AcquireSwLetterbox(SW *psw)
{
    if (++psw->cUnk3d80 == 1) {
        ShowLetterbox(D_002E1E60);
    }
}

/** @sly2-annot addr=0x20F160
 * Purpose: --SW+0x3D80; last -> 0x1A30D0 (vt50 hide)
 * Info: proposed name ReleaseSwLetterbox (T1); TU sw
 * Notes: names_v2 C proposed IsSwVagPlaying__FP2SWPi (0.438)
 * Confidence: high [T1 0.85] - evidence: static naming pass; sources: r7_1F8DD8+verdicts_tu_r7r8
 * See: docs/engine/world-sw.md#10-key-functions, docs/reference/functions/world-sw.md#sw-0x20d9d0-0x210eb0
 * @end-sly2-annot */
extern "C" void ReleaseSwLetterbox(SW *psw)
{
    if (--psw->cUnk3d80 == 0) {
        HideLetterbox(D_002E1E60);
    }
}

/** @sly2-annot addr=0x20F198
 * Purpose: ++SW+0x3D84
 * Info: proposed name IncrementSwInputLock (T2); TU sw
 * Confidence: low (draft) [T2 0.30] - evidence: static analysis; sources: r7_1F8DD8
 * See: docs/reference/functions/world-sw.md#sw-0x20d9d0-0x210eb0
 * @end-sly2-annot */
extern "C" void func_0020F198(SW *psw)
{
    psw->cUnk3d84++;
}

/** @sly2-annot addr=0x20F1A8
 * Purpose: --SW+0x3D84
 * Info: proposed name DecrementSwInputLock (T2); TU sw
 * Confidence: low (draft) [T2 0.30] - evidence: static analysis; sources: r7_1F8DD8
 * See: docs/reference/functions/world-sw.md#sw-0x20d9d0-0x210eb0
 * @end-sly2-annot */
extern "C" void func_0020F1A8(SW *psw)
{
    psw->cUnk3d84--;
}

/** @sly2-annot addr=0x20F1B8
 * Purpose: SW+0x3D84 > 0
 * Info: proposed name FSwInputLock (T2); TU sw
 * Confidence: low (draft) [T2 0.30] - evidence: static analysis; sources: r7_1F8DD8+verdicts_tu_r7r8
 * See: docs/engine/world-sw.md#10-key-functions, docs/reference/functions/world-sw.md#sw-0x20d9d0-0x210eb0
 * @end-sly2-annot */
extern "C" int func_0020F1B8(SW *psw)
{
    return psw->cUnk3d84 > 0;
}

namespace M_UpdateSwIllum { extern "C" {
typedef int qword __attribute__((mode(TI)));
typedef union { qword q; float f; } U;
typedef struct { float x, y, z; } V3;
typedef union { qword q; V3 v; } UV;
extern char D_002BBB60[];
struct S { char pad[0x3D5C]; int n3D5C; char pad2[0x3E10 - 0x3D60]; V3 hsv; int pad3; float f3E20; float f3E24; V3 rgb; float f3E34; int f3E38; };
void ConvertUserRgbToUserHsv(V3 *pin, UV *pout);
void func_001ED2D8(UV *pv);
float GLimitLm__FP2LMf(void *p, float g);
/** @sly2-annot addr=0x20F1C8
 * Purpose: Recompute ambient light colour from HSV (+0x3E10) scaled by midtone (+0x3E20/255) and shadow level
 *   (+0x3E24/255); bump illum change counter
 * Info: proposed name UpdateSwIllum (T1); TU sw
 * Confidence: mid (draft) [T1 0.50] - evidence: static analysis; sources: r7_1F8DD8
 * See: docs/engine/rendering.md#3-drawsw-sort-prologue-and-pass-dispatch-srchandfunc_00157fa0c-cstatic,
 *   docs/reference/functions/world-sw.md#sw-0x20d9d0-0x210eb0
 * @end-sly2-annot */
void UpdateSwIllum(struct S *p)
{
    float k = 1.0f / 255.0f;
    V3 c;
    UV out;
    U u;
    qword r;
    c.x = p->hsv.x;
    c.y = p->hsv.y * k;
    c.z = p->hsv.z * k;
    ConvertUserRgbToUserHsv(&c, &out);
    {
        qword qwuqOut;
        __asm__("qmtc2.ni %1, %0" : "=j"(qwuqOut) : "r"(p->f3E20 * k));
        u.q = qwuqOut;
    }
    __asm__("vmulx.xyzw %0, %1, %2x" : "=j"(r) : "j"(out.q), "j"(u.q));
    out.q = r;
    func_001ED2D8(&out);
    p->rgb = out.v;
    p->f3E34 = p->f3E24 * k;
    if (p->f3E38)
        p->f3E34 = GLimitLm__FP2LMf(D_002BBB60, p->f3E34 + 0.3f);
    p->n3D5C++;
}
} } // namespace M_UpdateSwIllum

/** @sly2-annot addr=0x20F2B8
 * Purpose: Store HSV qword (+0x3E10) and UpdateSwIllum
 * Info: proposed name SetSwIllumHsv (T2); TU sw
 * Confidence: low (draft) [T2 0.45] - evidence: static analysis; sources: r7_1F8DD8
 * See: docs/reference/functions/world-sw.md#sw-0x20d9d0-0x210eb0
 * @end-sly2-annot */
extern "C" void func_0020F2B8(SW *psw, qword hsv)
{
    *(qword *)&psw->hsvLight = hsv;
    UpdateSwIllum(psw);
}

/** @sly2-annot addr=0x20F2D8
 * Purpose: Store +0x3E24 and UpdateSwIllum
 * Info: proposed name SetSwIllumShadow (T1); TU sw
 * Notes: names_v2 v1 proposed FAddAshAseg__FP3ASHP4ASEG (0.009)
 * Confidence: mid (draft) [T1 0.50] - evidence: static naming pass; sources: r7_1F8DD8
 * See: docs/reference/functions/world-sw.md#sw-0x20d9d0-0x210eb0
 * @end-sly2-annot */
extern "C" void SetSwIllumShadow(SW *psw, float g)
{
    psw->gMidtone = g;
    UpdateSwIllum(psw);
}

/** @sly2-annot addr=0x20F2F8
 * Purpose: Store midtone +0x3E20 (ctor default 90) and UpdateSwIllum
 * Info: proposed name SetSwIllum (T1); TU sw
 * Notes: names_v2 v1 proposed FAddAshOid__FP3ASH3OID (0.009)
 * Confidence: high [T1 0.85] - evidence: static naming pass; sources: r7_1F8DD8+verdicts_tu_r7r8+verify_side
 * See: docs/engine/world-sw.md#10-key-functions, docs/reference/functions/world-sw.md#sw-0x20d9d0-0x210eb0
 * @end-sly2-annot */
extern "C" void SetSwIllum(SW *psw, float g)
{
    psw->gLightScale = g;
    UpdateSwIllum(psw);
}

namespace M_LoadLevelTableFromBrx {
// LoadLevelTableFromBrx: ported from Lilly's personal Sly 2 decomp (src/P2/sw.c), reduced to the declarations it needs.
// Every global is named by its address; the function keeps its readable name and gets the team symbol through __asm__.
// Globals: D_003E1C4C (g_alevellist), D_003E1C48 (g_anEpisodeOfLevel), D_003E1C44 (g_anWorldOfLevel), D_003E1C3C (g_nUnk3E1C3C), D_003E1C38 (g_cLevelTable), D_003E1C40 (g_aLevelRecord), D_003E1C50 (g_aWorldEntryRecord)
// 0x0020F318 LoadLevelTableFromBrx  (T1038)
// u32 clevel, u32 (unused); clevel x { file record (0x40 bytes); u8 world; u8 episode; u8 fEntry (the record is also the entry of its world:
// copied to g_aWorldEntryRecord[world]); u8 n + n u16 level numbers }.
#include "sly2/sw.h"
class CBinaryStreamBase
{
public:
    int m_grfbis;
    byte *m_pb;
    int m_cb;
    int m_ibStream;
    byte *m_pbRaw;
    int m_cbRaw;
    int m_grfDecomp;
    int m_cbSpillOver;
    int m_unk20;
    byte *m_abWindow;
    int m_unk28;
    int m_unk2c;
};
class CBinaryInputStream : public CBinaryStreamBase
{
public:
    int m_cbSpool;
    byte *m_abSpool;
    int m_fd;
    int m_cbDone;
    int m_cbRequest;
    int m_cbRemaining;
    int m_cbRetry;
    int m_isector;
    int m_cbuf;
    int m_cbufFill;
    int m_icbufRead;
    int m_icbufWrite;
    uchar U8Read() __asm__("U8Read__18CBinaryInputStream");
    ushort U16Read() __asm__("U16Read__18CBinaryInputStream");
    uint U32Read() __asm__("U32Read__18CBinaryInputStream");
    void ReadFileRecord(void *pv) __asm__("CBinaryInputStream_ReadCb64");
};
struct SWALLOC
{
    int cb;
    int cbAlign;
};
void *PvAllocSwImpl(SWALLOC *pswalloc) __asm__("func_001AE1D0");
void *PvAllocSwClearImpl(SWALLOC *pswalloc) __asm__("func_001AE2A0");
inline void *PvAllocSwLoad(int cb, int cbAlign)
{
    SWALLOC swalloc;
    swalloc.cb = cb;
    SWALLOC *pswalloc = &swalloc;
    pswalloc->cbAlign = cbAlign;
    return PvAllocSwImpl(pswalloc);
}
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
extern "C" { extern int D_003E1C38; }
extern "C" { extern int D_003E1C3C; }
extern "C" { extern FILEREC *D_003E1C40; }
extern "C" { extern int *D_003E1C44; }
extern "C" { extern int *D_003E1C48; }
extern "C" { extern LEVELLIST *D_003E1C4C; }
extern "C" { extern FILEREC *D_003E1C50; }
extern "C" void LoadLevelTableFromBrx(SW *psw, CBinaryInputStream *pbis) __asm__("LoadLevelTableFromBrx");
/** @sly2-annot addr=0x20F318
 * Purpose: Read level table: clevel x {file record, world, episode, fEntry, n level numbers}; fills
 *   g_alevellist/g_anWorldOfLevel/g_anEpisodeOfLevel/world entry records
 * Info: proposed name LoadLevelTableFromBrx (T1); TU sw
 * Confidence: high [T1 0.85] - evidence: static analysis; sources: r7_1F8DD8
 * See: docs/engine/world-sw.md#loadswfrombrx-0x13da18-matched-hand-file,
 *   docs/engine/world-sw.md#3-level-table-and-world-identity, docs/engine/world-sw.md#10-key-functions
 * @end-sly2-annot */
extern "C" void LoadLevelTableFromBrx(SW *psw, CBinaryInputStream *pbis)
{
    D_003E1C38 = pbis->U32Read();
    D_003E1C3C = pbis->U32Read();
    D_003E1C40 = (FILEREC *)PvAllocSwLoad(D_003E1C38 * sizeof(FILEREC), 0x10);
    D_003E1C44 = (int *)PvAllocSwLoad(D_003E1C38 * sizeof(int), 0x10);
    D_003E1C48 = (int *)PvAllocSwLoad(D_003E1C38 * sizeof(int), 0x10);
    D_003E1C4C = (LEVELLIST *)PvAllocSwLoad(D_003E1C38 * sizeof(LEVELLIST), 0x10);
    D_003E1C50 = (FILEREC *)PvAllocSwClearLoad(9 * sizeof(FILEREC), 0x10);
    for (int ilevel = 0; ilevel < D_003E1C38; ilevel++)
    {
        pbis->ReadFileRecord(&D_003E1C40[ilevel]);
        D_003E1C44[ilevel] = pbis->U8Read();
        D_003E1C48[ilevel] = pbis->U8Read();
        if (pbis->U8Read())
            D_003E1C50[D_003E1C44[ilevel]] = D_003E1C40[ilevel];
        LEVELLIST *plevellist = &D_003E1C4C[ilevel];
        plevellist->c = pbis->U8Read();
        plevellist->a = (short *)PvAllocSwLoad(plevellist->c * sizeof(short), 0x10);
        for (int i = 0; i < plevellist->c; i++)
            plevellist->a[i] = pbis->U16Read();
    }
}
} // namespace M_LoadLevelTableFromBrx

namespace M_LoadSwSharedHeaderFromBrx { extern "C" {
extern int D_003E1C30;
extern int D_003E1C54;
extern int D_003E1C58;
int S32Read__18CBinaryInputStream(void *pbis);
/** @sly2-annot addr=0x20F5D0
 * Purpose: Read (cb, id) of a cross-level shared data block; flag reload if id changed or forced
 * Info: proposed name LoadSwSharedHeaderFromBrx (T1); TU sw
 * Notes: level_parser CONFIRMED-parse: payload layout followed by the parser, 100%% of bytes in 4 levels
 * Confidence: high [T1 0.90] - evidence: static analysis; sources: r7_1F8DD8+verdicts_tu_r7r8+parse-verified
 * See: docs/engine/world-sw.md#10-key-functions,
 *   docs/formats/level-objects.md#2-per-world-entry-header-loop-body-of-loadswfrombrx,
 *   docs/reference/functions/world-sw.md#sw-0x20d9d0-0x210eb0
 * @end-sly2-annot */
void LoadSwSharedHeaderFromBrx(int a, void *pbis)
{
    int x = S32Read__18CBinaryInputStream(pbis);
    int y = S32Read__18CBinaryInputStream(pbis);
    D_003E1C30 = x;
    if (y != D_003E1C58) a = 1;
    D_003E1C54 = a;
    D_003E1C58 = y;
}
} } // namespace M_LoadSwSharedHeaderFromBrx

namespace M_LoadSwSharedDataFromBrx { extern "C" {
extern int D_003E1C54;
extern int D_003E1C30;
struct L { int a[4]; };
struct M { int a; int b; int c[2]; };
void Align__18CBinaryInputStreami(void *q, int n);
void STACKSCOPE_ctor(struct L *pl);
void *func_001AE318(struct M *pm);
void Read__18CBinaryInputStreamiPv(void *q, int a, void *x);
void RelocateSharedColorTables(void *x);
void UploadBitmaps__FP3GSB(void *p);
void STACKSCOPE_ctor_1AE4A0(struct L *pl, int n);
void Skip__18CBinaryInputStreami(void *q, int a);
void func_00170828(void *p);
/** @sly2-annot addr=0x20F640
 * Purpose: If shared block changed: read cb bytes into slot alloc and rebuild (0x1EA568, 0x170730); else skip
 *   it and reuse (0x170828)
 * Info: proposed name LoadSwSharedDataFromBrx (T1); TU sw
 * Notes: level_parser CONFIRMED-parse: payload layout followed by the parser, 100%% of bytes in 4 levels
 * Confidence: mid (draft) [T1 0.75] - evidence: static analysis; sources: r7_1F8DD8+parse-verified
 * See: docs/engine/world-sw.md#10-key-functions,
 *   docs/formats/level-objects.md#2-per-world-entry-header-loop-body-of-loadswfrombrx,
 *   docs/formats/textures.md#1-layout-inside-a-world-entry-cstatic--cparse
 * @end-sly2-annot */
void LoadSwSharedDataFromBrx(void *p, void *q)
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
        x = func_001AE318(pm); }
        Read__18CBinaryInputStreamiPv(q, D_003E1C30, x);
        RelocateSharedColorTables(x);
        UploadBitmaps__FP3GSB(p);
        STACKSCOPE_ctor_1AE4A0(&l, 2);
    } else {
        Skip__18CBinaryInputStreami(q, D_003E1C30);
        func_00170828(p);
    }
}
} } // namespace M_LoadSwSharedDataFromBrx

namespace M_func_0020F710 { extern "C" {
extern char D_003DD490[];
extern int D_002C3E70[4];
struct L { float a; int b; int c; float d; int e[4]; };
struct E { int x; int pad[2]; };
struct S { char pad[0x3BA0]; struct E ae[4]; char pad2[0x4764 - 0x3BD0]; int n4764; int n4768; };
void *func_002333B4(void *p, int c, int n);
int PloFindSwObject__FP2SWi3OIDP2LO(void *p, int a, int b, int c);
void SetWidgetXf(int x, struct L *pl);
int CClueCollected(void);
int func_001C7D00(void);
/** @sly2-annot addr=0x20F710
 * Purpose: Find 4 HUD objects (kind 0x405, OIDs 0x371/0x36D/0x36E/0x36F), init them, record two counters
 * Info: proposed name BindSwTimerHud (T2); TU sw
 * Notes: names_v2 v1 proposed LoadBtn__FP3BTNP3ALO (0.003)
 * Confidence: low (draft) [T2 0.35] - evidence: static naming pass; sources: r7_1F8DD8
 * See: docs/reference/functions/world-sw.md#sw-0x20d9d0-0x210eb0
 * @end-sly2-annot */
void func_0020F710(struct S *p)
{
    struct L l;
    int i;
    int *pn = D_002C3E70;
    func_002333B4(&l, 0, sizeof(l));
    l.d = 1.0f;
    l.a = 1.0f;
    for (i = 0; i < 4; i++) {
        int x = PloFindSwObject__FP2SWi3OIDP2LO(D_003DD490, 0x405, D_002C3E70[i], 0);
        if (x) {
            SetWidgetXf(x, &l);
            p->ae[i].x = x;
        }
    }
    p->n4764 = CClueCollected();
    p->n4768 = func_001C7D00();
}
} } // namespace M_func_0020F710

namespace M_func_0020F7E0 { extern "C" {
struct F8 { char pad[0x14]; float v; };
struct O { char pad[0xF8]; struct F8 *pf8; };
struct E { struct O *po; float v; int f; };
struct S { char pad[0x3BA0]; struct E ae[4]; char pad2[0x476C - 0x3BD0]; float t476C; int n4770; };
extern struct { float t; float dt; } D_002DDED0;
extern char D_002C3E88[];
extern float D_002C3E80;
int FWidgetVisible(struct O *po);
float GSmoothA__FffffP4SMPAPf(float a, float b, float c, void *p, float *pv, float dt);
void *PsndStartSound(int n);
void SetSndSourceLo(void *p, void *source);

/** @sly2-annot addr=0x20F7E0
 * Purpose: Per-frame: smooth the 4 HUD elements; while a countdown (SW+0x476C - t) runs play tick/tock sfx
 *   0x30/0x31 each second
 * Info: proposed name UpdateSwTimerHud (T2); TU sw
 * Confidence: low (draft) [T2 0.40] - evidence: static analysis; sources: r7_1F8DD8
 * See: docs/engine/hud-ui.md#1-widget-hud-objects-hud-tu-0x210eb0-0x21bb88-file-name-unknown,
 *   docs/engine/overview.md#5-per-frame-update-and-render,
 *   docs/reference/functions/world-sw.md#sw-0x20d9d0-0x210eb0
 * @end-sly2-annot */
void func_0020F7E0(struct S *p)
{
    float z;
    float t = 0.0f;
    int i;
    z = t;
    for (i = 0; i < 4; i++) {
        struct E *e = &p->ae[i];
        struct O *o = e->po;
        if (!o || !FWidgetVisible(o)) {
            e->f = 1;
            continue;
        }
        if (e->f) {
            o->pf8->v = t;
            e->v = 0.0f;
            e->f = 0;
        } else {
            o->pf8->v = GSmoothA__FffffP4SMPAPf(o->pf8->v, e->v, t, D_002C3E88, &e->v, D_002DDED0.dt);
        }
        t += D_002C3E80;
        if (i == 3) {
            float d = -D_002DDED0.t + p->t476C;
            int n;
            d = (0.0f > d) ? 0.0f : d;
            n = (int)d;
            if (n != p->n4770) {
                SetSndSourceLo(PsndStartSound((n & 1) ? 0x30 : 0x31), 0);
                p->n4770 = n;
            }
        }
    }
}
} } // namespace M_func_0020F7E0

namespace M_FFilterSoPair { extern "C" {
typedef struct { short d; short pad; void *pfn; } VTE;
struct O { char pad[0xC]; VTE *vt; };
struct S { char pad[0x60]; struct O *po; char pad2[0xC]; long fl; };
struct B { char pad[0xEC]; struct S *ps; };
struct E { int pad; struct B *pb; int pad2[2]; long fl; };
int func_00134310(int a, int b);
#define VC61(o, a) ((int (*)(void *, struct O *))(o)->vt[61].pfn)((char *)(o) + (o)->vt[61].d, a)
/** @sly2-annot addr=0x20F948
 * Purpose: Intersection filter: reject self/flagged; accept unless relationship 0x134310(a,b) is 1 or 3
 * Info: proposed name FFilterSoPair (T1); TU sw
 * Notes: names_v2 v1 proposed PostBtnLoad__FP3BTN (0.007)
 * Confidence: high [T1 0.85] - evidence: static naming pass; sources: r7_1F8DD8+verdicts_tu_r7r8
 * See: docs/engine/world-sw.md#10-key-functions, docs/reference/functions/player.md#throw-0x21f2e0-0x220d40,
 *   docs/reference/functions/world-sw.md#sw-0x20d9d0-0x210eb0
 * @end-sly2-annot */
int FFilterSoPair(struct S *p, struct E *e)
{
    struct S *ps = e->pb->ps;
    int a, b, r;
    if (!(e->fl & (1L << 48)) && (ps->fl & 4))
        return 0;
    if ((e->fl & (1L << 48)) || ps == p)
        return 0;
    a = VC61(p->po, ps->po);
    b = VC61(ps->po, p->po);
    r = func_00134310(a, b);
    return r != 1 && r != 3;
}
} } // namespace M_FFilterSoPair
#undef VC61

namespace M_func_0020FA10 { extern "C" {
struct O { char pad[0x70]; long fl; };
struct P { char pad[0xEC]; struct O *po; };
struct S { int a; struct P *pp; char pad[8]; long fl; };
/** @sly2-annot addr=0x20FA10
 * Purpose: Filter: entry object is not ctx and not flag 4
 * Info: proposed name FFilterSoNotSelf (T2); TU sw
 * Confidence: low (draft) [T2 0.30] - evidence: static analysis; sources: r7_1F8DD8
 * See: docs/reference/functions/world-sw.md#sw-0x20d9d0-0x210eb0
 * @end-sly2-annot */
int func_0020FA10(struct O *po, struct S *p) {
    int r = 0;
    if (!(p->fl & 0x1000000000000L)) {
        struct O *o = p->pp->po;
        if (!(o->fl & 4)) r = o != po;
    }
    return r;
}
} } // namespace M_func_0020FA10

namespace M_func_0020FA50 { extern "C" {
struct VTE { short d; short pad; int (*pfn)(void *); };
struct O { int a, b, c; struct VTE *vt; };
struct S { int a; struct O *po; };
int func_0020FA10(void *pv, struct S *p);
/** @sly2-annot addr=0x20FA50
 * Purpose: FFilterSoNotSelf && object vt62 predicate
 * Info: proposed name FFilterSoNotSelfVt62 (T2); TU sw
 * Confidence: low (draft) [T2 0.25] - evidence: static analysis; sources: r7_1F8DD8
 * See: docs/reference/functions/world-sw.md#sw-0x20d9d0-0x210eb0
 * @end-sly2-annot */
int func_0020FA50(void *pv, struct S *p) { struct O *o; struct VTE *e; if (!func_0020FA10(pv, p)) return 0; o = p->po; e = &o->vt[62]; return e->pfn((char *)o + e->d); }
} } // namespace M_func_0020FA50

namespace M_IntersectSwBoundingBox { extern "C" {
typedef int qword __attribute__((mode(TI)));
typedef union { qword q; float f[4]; } U;
struct MSG { int cb; int nKind; };
struct O { char pad[0x60]; struct O *next; };
struct G { float mn[4]; float mx[3]; char pad[0x34 - 0x1C]; struct O *first; char pad2[0x40 - 0x38]; struct G *next; };
struct S { char pad[0x3D1C]; struct G *first; };
void *func_001AE318(struct MSG *pmsg);
void FlushSwBoundsUpdates(struct S *p);
void UpdateSwAaox(struct S *p);

/** @sly2-annot addr=0x20FAA0
 * Purpose: Collect up to 256 SOs whose groups overlap the AABB(min/max of two points), optional filter;
 *   result in slot-allocated array
 * Info: proposed name IntersectSwBoundingBox (T1); TU sw
 * Confidence: mid (draft) [T1 0.75] - evidence: static analysis; sources: r7_1F8DD8
 * See: docs/engine/game-progression.md#8-key-functions, docs/engine/world-sw.md#10-key-functions,
 *   docs/reference/functions/game-progression.md#prize-0x1c4e50-0x1c8cc0
 * @end-sly2-annot */
void IntersectSwBoundingBox(struct S *p, qword a, qword b, int (*pfn)(void *, struct O *), void *ctx, int *pc, struct O ***pap)
{
    U mn, mx;
    struct MSG msg;
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
        struct MSG *pm = &msg;
        pm->cb = 0x400;
        pm->nKind = 0x10;
    }
    c = 0;
    ap = (__typeof__(ap))func_001AE318(&msg);
    FlushSwBoundsUpdates(p);
    UpdateSwAaox(p);
    for (g = p->first; g; g = g->next) {
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
} } // namespace M_IntersectSwBoundingBox

namespace M_IntersectSwBoundingSphere { extern "C" {
typedef int qword __attribute__((mode(TI)));
struct MSG { int cb; int nKind; };
struct O { char pad[0x60]; struct O *next; };
struct G { char pad[0x20]; qword pos; float r; struct O *first; char pad2[0x40 - 0x38]; struct G *next; };
struct S { char pad[0x3D1C]; struct G *first; };
void *func_001AE318(struct MSG *pmsg);
void FlushSwBoundsUpdates(struct S *p);
void UpdateSwAaox(struct S *p);

/** @sly2-annot addr=0x20FC60
 * Purpose: Same for a sphere (center, radius)
 * Info: proposed name IntersectSwBoundingSphere (T1); TU sw
 * Confidence: high [T1 0.85] - evidence: static analysis; sources: r7_1F8DD8+verdicts_tu_r7r8
 * See: docs/engine/world-sw.md#10-key-functions,
 *   docs/reference/functions/splice-scripting.md#bif-0x11f440-0x1258f0,
 *   docs/reference/functions/world-sw.md#sw-0x20d9d0-0x210eb0
 * @end-sly2-annot */
void IntersectSwBoundingSphere(struct S *p, qword pos, float r, int (*pfn)(void *, struct O *), void *ctx, int *pc, struct O ***pap)
{
    struct MSG msg;
    struct O **ap;
    struct G *g;
    int c;
    msg.cb = 0x400;
    msg.nKind = 0x10;
    ap = (__typeof__(ap))func_001AE318(&msg);
    c = 0;
    FlushSwBoundsUpdates(p);
    UpdateSwAaox(p);
    for (g = p->first; g; g = g->next) {
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
} } // namespace M_IntersectSwBoundingSphere

namespace M_func_0020FDC8 { extern "C" {
struct O { char pad[0x148]; long fl; };
struct A { int pad; struct O *po; };
struct M { int cb; int align; int pad[2]; };
void *func_001AE318(struct M *pm);
/** @sly2-annot addr=0x20FDC8
 * Purpose: Deduplicate the SOs referenced by an array (mark 0x40000000), output count + slot array
 * Info: proposed name UniqueSwSoList (T2); TU sw
 * Confidence: low (draft) [T2 0.35] - evidence: static analysis; sources: r7_1F8DD8
 * See: docs/reference/functions/splice-scripting.md#bif-0x11f440-0x1258f0,
 *   docs/reference/functions/world-sw.md#sw-0x20d9d0-0x210eb0
 * @end-sly2-annot */
void func_0020FDC8(int n, struct A **apa, int *pc, struct O ***pppo)
{
    struct M m;
    struct O **apo;
    int c = 0;
    int i;
    m.cb = n * 4;
    m.align = 0x10;
    apo = (struct O **)func_001AE318(&m);
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
} } // namespace M_func_0020FDC8

namespace M_FAddRsmg__FP2SWP4RSMGiPii3OIDN25 { extern "C" {
typedef int OID;
typedef struct RSMG { OID oidRoot; OID oidSM; OID oidTrigger; OID oidNoTrigger; } RSMG;
int FAddRsmg(void *pv, RSMG *arsmg, int crsmgMax, int *pcrsmg, int fTrigger, OID oidRoot, OID oidSM, OID oid)
    __asm__("FAddRsmg__FP2SWP4RSMGiPii3OIDN25");
/** @sly2-annot addr=0x20FEC0
 * Purpose: Add/update a (root,SM)->trigger/untrigger goal record
 * Info: proposed name FAddRsmg__FP2SWP4RSMGiPii3OIDN25 (EXISTING); TU sw
 * Notes: RENAME proposed -> FAddRsmg__FP2SWP4RSMGiPii3OIDN25 (existing_name_changes.csv)
 * Confidence: high [EXISTING 0.80] - evidence: arbitration; pre-existing symbol in symbol_addrs.txt; sources:
 *   r7_1F8DD8+verdicts_tu_r7r8+arbitration
 * See: docs/engine/world-sw.md#10-key-functions, docs/reference/functions/world-sw.md#sw-0x20d9d0-0x210eb0
 * @end-sly2-annot */
int FAddRsmg(void *pv, RSMG *arsmg, int crsmgMax, int *pcrsmg, int fTrigger, OID oidRoot, OID oidSM, OID oid)
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
} } // namespace M_FAddRsmg__FP2SWP4RSMGiPii3OIDN25

namespace M_TriggerRsmg { extern "C" {
typedef struct QW { int a[4]; } QW;
void func_0020FFF0(void *p, QW *pq, int a, int b, int c, int d);
/** @sly2-annot addr=0x20FF70
 * Purpose: Apply every RSMG record in an array (calls 0x20FFF0 each)
 * Info: proposed name TriggerRsmg (T1); TU sw
 * Confidence: mid (draft) [T1 0.60] - evidence: static analysis; sources: r7_1F8DD8
 * See: docs/reference/functions/world-sw.md#sw-0x20d9d0-0x210eb0
 * @end-sly2-annot */
void TriggerRsmg(void *p, int c, QW *pq, int a, int b)
{
    int i;
    for (i = 0; i < c; i++) {
        func_0020FFF0(p, &pq[i], a, b, 0, 0);

    }
}
} } // namespace M_TriggerRsmg

namespace M_func_0020FFF0 { extern "C" {
typedef struct { short d; short pad; void *pfn; } VTE;
struct Y { char pad[0xC]; VTE *vt; };
struct D { int a; int b; int c; int d; };
int CploFindSwObjects_15E650(void *p, int k, int a, int b, int c, void **ppo);
int FIsBasicDerivedFrom__FP5BASIC3CID(void *o, int n);
struct Y *func_001F67E0(void *o, int b);
void SubscribeLoObject(struct Y *y, int x);
void SeekSma(struct Y *y, int id);
void SetSmaGoal(struct Y *y, int id);
/** @sly2-annot addr=0x20FFF0
 * Purpose: Find root object, its SM by oid, then SeekSma or SetSmaGoal to trigger/untrigger goal; optional
 *   returned SMA ref
 * Info: proposed name TriggerRsmgOne (T2); TU sw
 * Notes: names_v2 v1 proposed TriggerRsmg__FP2SWiP4RSMGP2LOi (0.012)
 * Confidence: low (draft) [T2 0.45] - evidence: static naming pass; sources: r7_1F8DD8
 * See: docs/reference/functions/world-sw.md#sw-0x20d9d0-0x210eb0
 * @end-sly2-annot */
void func_0020FFF0(void *p, struct D *pd, int b, int fl, int x, struct Y **ppy)
{
    void *o;
    struct Y *y;
    int id;
    if (ppy)
        *ppy = 0;
    if (CploFindSwObjects_15E650(p, 0x204, pd->a, b, 1, &o) != 1)
        return;
    if (!FIsBasicDerivedFrom__FP5BASIC3CID(o, 0x30))
        return;
    y = func_001F67E0(o, pd->b);
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
        ((void (*)(void *))y->vt[1].pfn)((char *)y + y->vt[1].d);
        SubscribeLoObject(y, x);
    }
    if (fl & 2)
        SeekSma(y, id);
    else
        SetSmaGoal(y, id);
}
} } // namespace M_func_0020FFF0

/** @sly2-annot addr=0x210100
 * Purpose: Return &array(+0x39D0)[count(+0x39D4)++] (12-byte records)
 * Info: proposed name PAllocSwLoQueue (T2); TU sw
 * Confidence: low (draft) [T2 0.30] - evidence: static analysis; sources: r7_1F8DD8
 * See: docs/reference/functions/world-sw.md#sw-0x20d9d0-0x210eb0
 * @end-sly2-annot */
/* The next 12-byte record of the array at +0x39D0. */
extern "C" void *func_00210100(SW *psw)
{
    int i = psw->cUnk39d4;
    psw->cUnk39d4 = i + 1;
    return (char *)psw->aUnk39d0 + i * 0xC;
}

/** @sly2-annot addr=0x210120
 * Purpose: Return &array(+0x39D8)[count++] (12-byte records); drained by PostSwLoad
 * Info: proposed name PAllocSwPostLoadQueue (T2); TU sw
 * Confidence: low (draft) [T2 0.30] - evidence: static analysis; sources: r7_1F8DD8
 * See: docs/reference/functions/world-sw.md#sw-0x20d9d0-0x210eb0
 * @end-sly2-annot */
/* The next 12-byte record of the array at +0x39D8. */
extern "C" void *func_00210120(SW *psw)
{
    int i = psw->cUnk39dc;
    psw->cUnk39dc = i + 1;
    return (char *)psw->aUnk39d8 + i * 0xC;
}

namespace M_func_00210140 { extern "C" {
struct E;
struct O { char pad[0x40]; struct E *e; };
struct E { unsigned int grf; struct O *o; };
struct G { char pad[0x3F14]; int count; int dead; int *index; struct E *array; };
/** @sly2-annot addr=0x210140
 * Purpose: Append LO to SW update array (back-pointer at LO+0x40), compacting when >=4096 or >10% dead
 * Info: proposed name AddSwUpdateLo (T2); TU sw
 * Notes: names_v2 v1 proposed TriggerBtn__FP3BTNii (0.001)
 * Confidence: low (draft) [T2 0.45] - evidence: static naming pass; sources: r7_1F8DD8
 * See: docs/engine/object-model.md#9-key-functions,
 *   docs/reference/functions/object-model.md#lo-0x1a5270-0x1a7518,
 *   docs/reference/functions/world-sw.md#sw-0x20d9d0-0x210eb0
 * @end-sly2-annot */
void func_00210140(struct G *g, struct O *o)
{
    struct E *e;
    if ((unsigned int)o->e & 0x40)
        return;
    if (g->count >= 0x1000 || (float)g->dead / (float)g->count > 0.1f) {
        struct E *end = g->array + g->count;
        struct E *tracked;
        struct E *write;
        struct E *read;
        if (g->index)
            tracked = g->array + *g->index;
        else
            tracked = 0;
        write = g->array;
        for (read = g->array; read < end; read++) {
            if (read == tracked)
                tracked = write;
            if (read->o) {
                read->o->e = write;
                *write++ = *read;
            }
        }
        g->dead = 0;
        g->count = write - g->array;
        if (g->index)
            *g->index = tracked - g->array;
    }
    e = &g->array[g->count++];
    e->o = o;
    e->grf = (unsigned int)o->e;
    o->e = e;
}
} } // namespace M_func_00210140

/** @sly2-annot addr=0x210268
 * Purpose: Clear LO entry in update array, restore LO+0x40 flags, ++dead count (+0x3F18)
 * Info: proposed name RemoveSwUpdateLo (T2); TU sw
 * Notes: names_v2 v1 proposed LoadButtonFromBrx__FP6BUTTONP18CBinaryInputStream (0.001)
 * Confidence: low (draft) [T2 0.45] - evidence: static naming pass; sources: r7_1F8DD8
 * See: docs/engine/object-model.md#9-key-functions,
 *   docs/reference/functions/object-model.md#lo-0x1a5270-0x1a7518,
 *   docs/reference/functions/world-sw.md#sw-0x20d9d0-0x210eb0
 * @end-sly2-annot */
/* Takes an object off the update list: restores its +0x40 word, clears the entry, counts the hole. */
extern "C" void func_00210268(SW *psw, LO *plo)
{
    int n = plo->grfUnk40;
    if (((n ^ 1) & 1) != 0) {
        LOUPD *ploupd = (LOUPD *)n;
        plo->grfUnk40 = ploupd->grf;
        ploupd->grf = 0;
        ploupd->plo = 0;
        psw->cloupdHole++;
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
struct B { X *first; int a, b; };
struct S { char pad[0x3038]; B ab[199]; };
extern "C" void func_001C4A60(S *p, int k);
/** @sly2-annot addr=0x2102A0
 * Purpose: 0x1C4A60(p,0) then vt24 on every LO in the 199 class buckets (+0x3038)
 * Info: proposed name RefreshSwObjects (T2); TU sw
 * Notes: names_v2 v1 proposed InsertButtonPos__FP6BUTTONP3PNTP6VECTOR (0.008) | runtime: per-class list
 *   stride at SW+0x3038 is 12 bytes
 * Confidence: low (draft) [T2 0.25] - evidence: runtime (save-state RAM); static naming pass; sources:
 *   r7_1F8DD8+runtime-corrected
 * See: docs/engine/world-sw.md#10-key-functions, docs/reference/functions/world-sw.md#sw-0x20d9d0-0x210eb0
 * @end-sly2-annot */
extern "C" void func_002102A0(S *p)
{
    int i;
    func_001C4A60(p, 0);
    for (i = 0; i < 199; i++) {
        X *x = ((struct B *)((char *)p + 0x3038))[i].first;
        for (; x; x = x->next)
            x->v24();
    }
}
} // namespace M_func_002102A0

/** @sly2-annot addr=0x210330
 * Purpose: Empty (jr ra), no callers
 * Info: proposed name junk_00210330 (T1); class LO slot24; TU sw
 * Notes: junk/padding (not a function)
 * Confidence: mid (draft) [T1 0.30] - evidence: static analysis; sources: r7_1F8DD8
 * @end-sly2-annot */
extern "C" void func_00210330()
{
}

namespace M_ShowSwAsegObject {
struct X { char pad[0xC];
  virtual void v1(); virtual void v2(); virtual void v3(); virtual void v4();
  virtual void v5(); virtual void v6(); virtual void v7(); virtual void v8(); virtual void v9(); };
extern "C" X *PloFindSwObject__FP2SWi3OIDP2LO(void *pv, int k, int id);
extern "C" int FIsBasicDerivedFrom__FP5BASIC3CID(void *p, int cid);
extern "C" void FadeSoIn(X *p, float f);
extern "C" void FadeSoOut(X *p, float f);
/** @sly2-annot addr=0x210338
 * Purpose: ASEG event 7/8: find object; fade in/out over dt if SO-derived and dt!=0, else vt8 Add / vt9
 *   Remove
 * Info: proposed name ShowSwAsegObject (T1); TU sw
 * Notes: names_v2 v1 proposed InsertButtonMat__FP6BUTTONP3XFMP7MATRIX3 (0.012)
 * Confidence: mid (draft) [T1 0.50] - evidence: static naming pass; sources: r7_1F8DD8
 * See: docs/reference/functions/world-sw.md#sw-0x20d9d0-0x210eb0
 * @end-sly2-annot */
extern "C" X *ShowSwAsegObject(void *pv, int fOn, int id, float g)
{
    X *p = PloFindSwObject__FP2SWi3OIDP2LO(pv, 4, id);
    if (p) {
        if (g != 0.0f && FIsBasicDerivedFrom__FP5BASIC3CID(p, 0x30)) {
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

namespace M_func_00210408 { extern "C" {
void *PloFindSwObject__FP2SWi3OIDP2LO(void *pv, int k, int id, void *ctx);
int FIsBasicDerivedFrom__FP5BASIC3CID(void *p, int cid);
void ShowSoGlobsById(void *p, void *q);
void HideSoGlobsById(void *p, void *q);
/** @sly2-annot addr=0x210408
 * Purpose: ASEG event 13/14: on SO target call 0x1F6978 or 0x1F69F8 (enable/disable pair)
 * Info: proposed name HandleSwAsegEvent13 (T2); TU sw
 * Notes: names_v2 v1 proposed IposFindButtonClosest__FP6BUTTONPf (0.068)
 * Confidence: low (draft) [T2 0.25] - evidence: static naming pass; sources: r7_1F8DD8
 * See: docs/reference/functions/world-sw.md#sw-0x20d9d0-0x210eb0
 * @end-sly2-annot */
void func_00210408(void *pv, int f, int id, void *q, void *pdef, void *ctx)
{
    void *p;
    if (id == -1)
        p = pdef;
    else
        p = PloFindSwObject__FP2SWi3OIDP2LO(pv, 4, id, ctx);
    if (p && FIsBasicDerivedFrom__FP5BASIC3CID(p, 0x30)) {
        if (f)
            ShowSoGlobsById(p, q);
        else
            HideSoGlobsById(p, q);
    }
}
} } // namespace M_func_00210408

namespace M_func_002104A0 { extern "C" {
void *PloFindSwObject__FP2SWi3OIDP2LO(void *pv, int k, int id, void *ctx);
int FIsBasicDerivedFrom__FP5BASIC3CID(void *p, int cid);
void ShowSoPartsById(void *p, void *q);
void func_001F6B48(void *p, void *q);
/** @sly2-annot addr=0x2104A0
 * Purpose: ASEG event 15/16: on SO call 0x1F6AC8 or 0x1F6B48
 * Info: proposed name HandleSwAsegEvent15 (T2); TU sw
 * Confidence: low (draft) [T2 0.25] - evidence: static analysis; sources: r7_1F8DD8
 * See: docs/reference/functions/world-sw.md#sw-0x20d9d0-0x210eb0
 * @end-sly2-annot */
void func_002104A0(void *pv, int f, int id, void *q, void *pdef, void *ctx)
{
    void *p;
    if (id == -1)
        p = pdef;
    else
        p = PloFindSwObject__FP2SWi3OIDP2LO(pv, 4, id, ctx);
    if (p && FIsBasicDerivedFrom__FP5BASIC3CID(p, 0x30)) {
        if (f)
            ShowSoPartsById(p, q);
        else
            func_001F6B48(p, q);
    }
}
} } // namespace M_func_002104A0

namespace M_func_00210538 { extern "C" {
struct VT { char pad[0x198]; short d; short pd; void (*pf)(void *, void *); };
struct X { char pad[0xC]; struct VT *pvt; };
struct MSG { unsigned int grf; struct X *p4; float g8; char padc[0x24]; struct X *p30; char pad34[0xC]; };
struct X *PloFindSwObject__FP2SWi3OIDP2LO(void *pg, int k, int oid, void *ctx);
/** @sly2-annot addr=0x210538
 * Purpose: ASEG event 17: find object and two optional others, build a message (grf bits 1/0x20/0x18) and
 *   send via vt
 * Info: proposed name SendSwAsegMessage (T2); TU sw
 * Confidence: low (draft) [T2 0.30] - evidence: static analysis; sources: r7_1F8DD8
 * See: docs/reference/functions/world-sw.md#sw-0x20d9d0-0x210eb0
 * @end-sly2-annot */
void func_00210538(void *pg, int oid, int oid1, int oid2, float g, void *ctx)
{
    struct X *x = PloFindSwObject__FP2SWi3OIDP2LO(pg, 4, oid, ctx);
    struct MSG msg;
    if (!x)
        return;
    msg.grf = 0;
    if (oid1 != -1) {
        struct X *y = PloFindSwObject__FP2SWi3OIDP2LO(pg, 4, oid1, ctx);
        if (y) {
            msg.p4 = y;
            msg.grf |= 1;
        }
    }
    {
        unsigned int v = msg.grf;
        if ((v ^ 1) & 1) {
            msg.p4 = x;
            msg.grf = v | 1;
        }
    }
    if (oid2 != -1) {
        struct X *z = PloFindSwObject__FP2SWi3OIDP2LO(pg, 4, oid2, ctx);
        if (z) {
            msg.p30 = z;
            msg.grf |= 0x20;
        }
    }
    if (g != 1.0f) {
        msg.g8 = g;
        msg.grf |= 0x18;
    }
    x->pvt->pf((char *)x + x->pvt->d, &msg);
}
} } // namespace M_func_00210538

namespace M_func_00210678 { extern "C" {
void *PloFindSwObject__FP2SWi3OIDP2LO(void *pv, int k, int id);
void *func_001EC918(int id);
int FIsBasicDerivedFrom__FP5BASIC3CID(void *p, int cid);
void func_001F80D0(void *p, float f);
void SetEyesClosed(void *p, float f);
/** @sly2-annot addr=0x210678
 * Purpose: ASEG event 20: SO -> 0x1F80D0(f); EYES (0x8E) found by 0x1EC918 -> 0x1EE3B0(f)
 * Info: proposed name HandleSwAsegEvent20 (T2); TU sw
 * Notes: names_v2 v1 proposed PresetButtonAccel__FP6BUTTONf (0.010)
 * Confidence: low (draft) [T2 0.25] - evidence: static naming pass; sources: r7_1F8DD8
 * See: docs/reference/functions/world-sw.md#sw-0x20d9d0-0x210eb0
 * @end-sly2-annot */
void func_00210678(void *pv, int id1, int id2, float f)
{
    void *p1 = 0;
    void *p2 = 0;
    if (id1 != -1) {
        void *q = PloFindSwObject__FP2SWi3OIDP2LO(pv, 4, id1);
        if (q && FIsBasicDerivedFrom__FP5BASIC3CID(q, 0x30))
            p1 = q;
    }
    if (id2 != -1) {
        p2 = func_001EC918(id2);
        if (p2 && !FIsBasicDerivedFrom__FP5BASIC3CID(p2, 0x8E))
            p2 = 0;
    }
    if (p1)
        func_001F80D0(p1, f);
    if (p2)
        SetEyesClosed(p2, f);
}
} } // namespace M_func_00210678

namespace M_StartSwAsegSound { extern "C" {
// Complete WIP; true source-object lookup and sound-reference contracts.
static inline void Clamp(float *f,float *b) {float lo=b[0],hi=b[1]; if(*f<lo)*f=lo; else if(hi<*f)*f=hi;}
struct VT { char pad[0x40]; short d; short pd; int (*kind)(void *); };
struct M { char pad[0xC]; struct VT *vt; };
struct C { char pad[0x20]; float value; };
struct O { char pad[0xFC4]; struct C *curve; };
struct G { char pad[0x475C]; float offset; };
extern float D_002C3EA0[3];
extern float D_002C3EB0[2];
struct O *PloFindSwObject__FP2SWi3OIDP2LO(struct G *, int, int, void *);
int FIsBasicDerivedFrom__FP5BASIC3CID(struct O *, int);
struct M *PsndStartSound(int);
void SetSndSourceLo(struct M *, struct O *);
void SetSndRange(struct M *, float, float);
void func_001FB1F8(struct M *, float);
void SetSndPitch(struct M *, float);
void func_001FB520(struct M *, float);
void RcPtr_Assign(struct M **, struct M *);
static inline struct O *SelectedValue(struct O *p){struct O *value;__asm__("daddu %0,%1,$zero":"=r"(value):"r"(p));return value;}
/** @sly2-annot addr=0x210740
 * Purpose: ASEG event 0: start sfx at source object (optionally by oid) with range/vol/pitch; continuous ones
 *   stored in the ASEGA ref
 * Info: proposed name StartSwAsegSound (T1); TU sw
 * Confidence: mid (draft) [T1 0.55] - evidence: static analysis; sources: r7_1F8DD8
 * See: docs/reference/functions/sound.md#sound-0x1faf70-0x1ff3b8,
 *   docs/reference/functions/world-sw.md#sw-0x20d9d0-0x210eb0
 * @end-sly2-annot */
void StartSwAsegSound(float offset, float fade, float scale, struct G *p, int id, int oid, int x, int y, struct O *fallback, void *owner, void *unused, struct M **out)
{
    struct O *o;
    struct M *m;
    if (id == -1)
        return;
    __asm__ volatile("" : : : "$4");
    if(oid!=-1)fallback=PloFindSwObject__FP2SWi3OIDP2LO(p,4,oid,owner);
    o=SelectedValue(fallback);
    if (o && FIsBasicDerivedFrom__FP5BASIC3CID(o, 0x38) && o->curve) {
        float v = o->curve->value;
        float f = D_002C3EA0[0] + v * (D_002C3EA0[1] + v * D_002C3EA0[2]);
        Clamp(&f, D_002C3EB0);
        offset += f;
    }
    offset += p->offset;
    m = PsndStartSound(id);
    SetSndSourceLo(m, o);
    SetSndRange(m, (float)x, (float)y);
    func_001FB1F8(m, scale);
    SetSndPitch(m, offset);
    func_001FB520(m, fade);
    if (m->vt->kind((char *)m + m->vt->d) == 2)
        RcPtr_Assign(out, m);
}
} } // namespace M_StartSwAsegSound

namespace M_StopSwAsegSound { extern "C" {
struct O { char pad[0x60]; int id; };
void StopSnd(struct O *p, float f);
void RcPtr_Assign(struct O **pp, int a);
/** @sly2-annot addr=0x2108E8
 * Purpose: ASEG event 1: StopSnd stored continuous sound (optionally matching id) and release ref
 * Info: proposed name StopSwAsegSound (T1); TU sw
 * Notes: names_v2 v1 proposed InitVolbtn__FP6VOLBTN (0.000)
 * Confidence: high [T1 0.85] - evidence: static naming pass; sources: r7_1F8DD8+verdicts_tu_r7r8+verify_side
 * See: docs/engine/world-sw.md#10-key-functions, docs/reference/functions/world-sw.md#sw-0x20d9d0-0x210eb0
 * @end-sly2-annot */
void StopSwAsegSound(void *a, int id, void *b, struct O **pp)
{
    struct O *p = *pp;
    if (!p)
        return;
    if (id != -1 && p->id != id)
        return;
    StopSnd(p, 0.0f);
    RcPtr_Assign(pp, 0);
}
} } // namespace M_StopSwAsegSound

/** @sly2-annot addr=0x210940
 * Purpose: SW+0x4638 = PloFindSwObject(psw,5,oid 0x620,0)
 * Info: proposed name BindSwObject620 (T2); TU sw
 * Confidence: low (draft) [T2 0.30] - evidence: static analysis; sources: r7_1F8DD8
 * See: docs/reference/functions/world-sw.md#sw-0x20d9d0-0x210eb0
 * @end-sly2-annot */
extern "C" void func_00210940(SW *psw)
{
    psw->ploUnk4638 = PloFindSwObject(psw, 5, (OID)0x620, 0);
}

/** @sly2-annot addr=0x210978
 * Purpose: If n==1 store at SW+0x3EA8; return psw
 * Info: proposed name SetSwField3EA8 (T2); TU sw
 * Confidence: low (draft) [T2 0.20] - evidence: static analysis; sources: r7_1F8DD8
 * See: docs/reference/functions/world-sw.md#sw-0x20d9d0-0x210eb0
 * @end-sly2-annot */
extern "C" SW *func_00210978(SW *psw, int n)
{
    if (n == 1) {
        F3EA8(psw) = n;
    }
    return psw;
}

namespace M_func_00210990 { extern "C" {
struct E { char pad[0x40]; };
extern struct E *D_003E1C40;
/** @sly2-annot addr=0x210990
 * Purpose: &g_arecLevel[ilevel] (NULL for -1)
 * Info: proposed name PrecLevelFile (T2); TU sw
 * Confidence: low (draft) [T2 0.40] - evidence: static analysis; sources: r7_1F8DD8
 * See: docs/reference/functions/world-sw.md#sw-0x20d9d0-0x210eb0
 * @end-sly2-annot */
struct E *func_00210990(int i)
{
    if (i == -1)
        return 0;
    return &D_003E1C40[i];
}
} } // namespace M_func_00210990

/** @sly2-annot addr=0x2109B8
 * Purpose: g_alevellist + ilevel*8
 * Info: proposed name PlevelFromIlevel (T2); TU sw
 * Confidence: low (draft) [T2 0.40] - evidence: static analysis; sources: r7_1F8DD8
 * See: docs/reference/functions/world-sw.md#sw-0x20d9d0-0x210eb0
 * @end-sly2-annot */
extern "C" LEVELLIST8 *func_002109B8(int ilevel)
{
    return &D_003E1C4C[ilevel];
}

namespace M_func_002109D0 { extern "C" {
struct X { char pad[8]; int x8; char padc[0x2C]; int x38; };
extern int D_003E1C38;
struct X *func_00210990(int i);
static inline int FEqual(struct X *a, struct X *b)
{
    if (a == b)
        return 1;
    if (!a || !b)
        return 0;
    return (a->x8 ^ a->x38) == (b->x8 ^ b->x38);
}
/** @sly2-annot addr=0x2109D0
 * Purpose: Index of level whose file record equals arg, else -1
 * Info: proposed name IlevelFromRec (T2); TU sw
 * Notes: names_v2 v1 proposed PostVolbtnLoad__FP6VOLBTN (0.004)
 * Confidence: low (draft) [T2 0.40] - evidence: static naming pass; sources: r7_1F8DD8
 * See: docs/reference/functions/world-sw.md#sw-0x20d9d0-0x210eb0
 * @end-sly2-annot */
int func_002109D0(struct X *px)
{
    int i;
    for (i = 0; i < D_003E1C38; i++) {
        if (FEqual(px, func_00210990(i)))
            return i;
    }
    return -1;
}
} } // namespace M_func_002109D0

/** @sly2-annot addr=0x210A88
 * Purpose: Return the current level index D_003E1110
 * Info: proposed name IlevelCur (T1); TU sw
 * Notes: top50_round (0.80): Body. Siblings PlevelFromIlevel/IlevelFromRec,
 *   NEpisodeCur=g_anEpisodeOfLevel[IlevelCur()], NWorldCur=g_anWorldOfLevel[IlevelCur()]. Splice builtin 97
 *   returns it and scripts switch on it (case (load-filter~) ...) with le
 * Confidence: high [T1 0.80] - evidence: semantic review; sources: r7_1F8DD8+top50_round
 * See: docs/engine/world-sw.md#10-key-functions,
 *   docs/reference/functions/splice-scripting.md#bif-0x11f440-0x1258f0,
 *   docs/reference/functions/world-sw.md#sw-0x20d9d0-0x210eb0
 * @end-sly2-annot */
int IlevelCur()
{
    return D_003E1110;
}

namespace M_NEpisodeCur { extern "C" {
extern int *D_003E1C48;
int IlevelCur__Fv(void);
/** @sly2-annot addr=0x210A98
 * Purpose: g_anEpisodeOfLevel[IlevelCur()] or -1
 * Info: proposed name NEpisodeCur (T1); TU sw
 * Confidence: mid (draft) [T1 0.50] - evidence: static analysis; sources: r7_1F8DD8
 * See: docs/reference/functions/splice-scripting.md#bif-0x11f440-0x1258f0,
 *   docs/reference/functions/world-sw.md#sw-0x20d9d0-0x210eb0
 * @end-sly2-annot */
int NEpisodeCur(void)
{
    int i = IlevelCur__Fv();
    if (i == -1)
        return -1;
    return D_003E1C48[i];
}
} } // namespace M_NEpisodeCur

namespace M_NWorldCur { extern "C" {
extern int *D_003E1C44;
int IlevelCur__Fv(void);
/** @sly2-annot addr=0x210AE0
 * Purpose: g_anWorldOfLevel[IlevelCur()] or -1
 * Info: proposed name NWorldCur (T1); TU sw
 * Confidence: mid (draft) [T1 0.50] - evidence: static analysis; sources: r7_1F8DD8
 * See: docs/reference/functions/player.md#stepplayer-0x13ff70-0x144638,
 *   docs/reference/functions/splice-scripting.md#bif-0x11f440-0x1258f0,
 *   docs/reference/functions/world-sw.md#sw-0x20d9d0-0x210eb0
 * @end-sly2-annot */
int NWorldCur(void)
{
    int i = IlevelCur__Fv();
    if (i == -1)
        return -1;
    return D_003E1C44[i];
}
} } // namespace M_NWorldCur

namespace M_NEpisodeBest { extern "C" {
struct CRef { int tagk; int tag; };
void RefGetSaveVarV(struct CRef *r, char *s, int a);
void Decref__4CRef(struct CRef *r);
extern char D_002A8A78[];
/** @sly2-annot addr=0x210B28
 * Purpose: Read save variable "episodeBest"
 * Info: proposed name NEpisodeBest (T1); TU sw
 * Notes: names_v2 v1 proposed CloneVolbtn__FP6VOLBTNT0 (0.000)
 * Confidence: mid (draft) [T1 0.55] - evidence: static naming pass; sources: r7_1F8DD8
 * See: docs/reference/functions/hud-ui.md#screen-0x1d93d8-0x1de2a0,
 *   docs/reference/functions/world-sw.md#sw-0x20d9d0-0x210eb0
 * @end-sly2-annot */
int NEpisodeBest(void)
{
    struct CRef r;
    int v;
    RefGetSaveVarV(&r, D_002A8A78, 0);
    v = r.tag;
    Decref__4CRef(&r);
    return v;
}
} } // namespace M_NEpisodeBest

/** @sly2-annot addr=0x210B68
 * Purpose: return 0
 * Info: proposed name FSwStub0 (T2); TU sw
 * Confidence: low (draft) [T2 0.20] - evidence: static analysis; sources: r7_1F8DD8
 * See: docs/reference/functions/splice-scripting.md#bif-0x11f440-0x1258f0,
 *   docs/reference/functions/world-sw.md#sw-0x20d9d0-0x210eb0
 * @end-sly2-annot */
extern "C" int func_00210B68()
{
    return 0;
}

/** @sly2-annot addr=0x210B70
 * Purpose: PrecWorldEntry(NWorldCur())
 * Info: proposed name PrecWorldEntryCur (T2); TU sw
 * Notes: names_v2 v1 proposed SetVolbtnButtons__FP6VOLBTN7BUTTONS (0.000)
 * Confidence: low (draft) [T2 0.40] - evidence: static naming pass; sources: r7_1F8DD8
 * See: docs/reference/functions/world-sw.md#sw-0x20d9d0-0x210eb0
 * @end-sly2-annot */
extern "C" void func_00210B70()
{
    PrecWorldEntry(NWorldCur());
}

/** @sly2-annot addr=0x210B98
 * Purpose: &g_arecWorldEntry[iworld] (64 bytes) if valid, else NULL
 * Info: proposed name PrecWorldEntry (T1); TU sw
 * Confidence: high [T1 0.85] - evidence: static analysis; sources: r7_1F8DD8+verdicts_tu_r7r8
 * See: docs/engine/world-sw.md#10-key-functions,
 *   docs/reference/functions/main-loop-startup.md#cheat-0x14bea8-0x14c548,
 *   docs/reference/functions/world-sw.md#sw-0x20d9d0-0x210eb0
 * @end-sly2-annot */
/* The world's entry record, or 0 when there is none (or it is empty: its words 1 and 15 equal). */
extern "C" FILEREC *PrecWorldEntry(int nWorld)
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

namespace M_func_00210BE0 { extern "C" {
typedef struct { char *pch; char *pchOut; } ENT;
typedef struct { int c; ENT *a; int x8; int xC; } TBL;
typedef struct { int a[4]; } ALLOC;
extern struct G { char pad[0x4640]; int f; } D_003DD490;
extern ALLOC *__11CLinearHeapiPUc(ALLOC *, int, int);
extern char *PchzFindSwLangstr__FP2SWi(struct G *, char *);
extern int func_0021ED28(char *);
extern char *PchzAllocCopy__FP4HEAPPci(ALLOC *, char *, int);

/** @sly2-annot addr=0x210BE0
 * Purpose: For each entry look up localized string via g_sw (0x13E368) and copy into a linear heap
 * Info: proposed name LocalizeSwStringTable (T2); TU sw
 * Confidence: low (draft) [T2 0.35] - evidence: static analysis; sources: r7_1F8DD8
 * See: docs/reference/functions/world-sw.md#sw-0x20d9d0-0x210eb0
 * @end-sly2-annot */
void func_00210BE0(TBL *p)
{
    ALLOC al;
    int i;
    if (D_003DD490.f == 0)
        return;
    __11CLinearHeapiPUc(&al, p->x8, p->xC);
    for (i = 0; i < p->c; i++) {
        ENT *e = &p->a[i];
        e->pchOut = PchzFindSwLangstr__FP2SWi(&D_003DD490, e->pch);
        if (func_0021ED28(e->pchOut))
            e->pchOut = PchzAllocCopy__FP4HEAPPci(&al, e->pchOut, 4);
        else
            e->pchOut = 0;
    }
}
} } // namespace M_func_00210BE0

/** @sly2-annot addr=0x210CA8
 * Purpose: Search {key,ptr} table; return ptr or default empty D_002A8A98
 * Info: proposed name PvFindSwKeyedTable (T2); TU sw
 * Confidence: low (draft) [T2 0.30] - evidence: static analysis; sources: r7_1F8DD8
 * See: docs/reference/functions/world-sw.md#sw-0x20d9d0-0x210eb0
 * @end-sly2-annot */
/* The value stored under key in a {key, value} table, or the empty default. */
extern "C" void *func_00210CA8(KVT *pkvt, int key)
{
    int i;
    for (i = 0; i < pkvt->c; i++) {
        if (key == pkvt->a[i].key && pkvt->a[i].pv != 0) {
            return pkvt->a[i].pv;
        }
    }
    return D_002A8A98;
}

