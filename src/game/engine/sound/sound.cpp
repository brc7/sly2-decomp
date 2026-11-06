// sound.cpp -- sounds (SND, and BANKSND / STREAMSND) and the sound manager (SNDMGR, the global g_sndmgr at
// D_003DD040): sound flags, pan / pitch / volume, the dirty list, bank and stream sounds (start, pause, refcount,
// pools), stream RPCs, the manager's playback mode and counters. TU 0x1FAF70..0x1FF3B8 (103 functions with the
// linkonce copies at 0x259xxx, address order).
//
// Types: TU-private SND / BANKSND / SNDMGR views for the m2c bodies (sly2/aseg.h's SND is a 3-word stub).
// g_sndmgr is defined here: SNDMGR_ctor constructs it, and its destructor releases the counted sound pointers at
// +0x200, +0x208[8] and +0x228 (slot 2 of each object's vtable), then runs ~BASIC (func_00258348, in-charge flag 2).
// Spelled as a base with that destructor and the pointers as members with an inline release, g++ generates the
// 123-instruction construct-and-destruct StaticInit_sound and the _GLOBAL_$D$ stub func_001FF398.
// The hand-written per-function files keep their views in namespaces named after each function (as jt.cpp does).
// SLY2_ALIAS: __static_initialization_and_destruction_0=StaticInit_sound
// SLY2_ALIAS: _GLOBAL_$I$*=func_001FF378
// SLY2_ALIAS: _GLOBAL_$D$*=func_001FF398

#include "sly2/basic.h"
#include "sly2/slotheap.h"
#include "sly2/cm.h"
#include "sly2/snd.h"

/* g_sndmgr's static construction and destruction. */
struct VTESND { short d; short pad; void (*pfn)(void *); };
struct SNDOBJ { char pad0[0xC]; VTESND *pvt; };
/* A counted pointer to a sound object: released through its vtable slot 2. */
struct SNDREF
{
    SNDOBJ *p;
    ~SNDREF() { if (p) p->pvt[2].pfn((char *)p + p->pvt[2].d); }
};
struct SNDMGRG;
extern "C" {
void SNDMGR_ctor(SNDMGRG *psndmgr);
void func_00258348(void *pv, int grfDelete);   /* ~BASIC */
}
struct SNDMGRBASE
{
    char ab[0x4C];
    DL dlSndDirty;          /* +0x4C [S] draft name: the sounds RemoveSndFromDirtyList takes out (was the address name D_003DD08C) */
    char ab58[0x200 - 0x58];
    ~SNDMGRBASE() { func_00258348(this, 2); }
};
struct SNDMGRG : public SNDMGRBASE
{
    SNDREF ref200;
    int n204;
    SNDREF aref208[8];
    SNDREF ref228;
    SNDMGRG() { SNDMGR_ctor(this); }
    ~SNDMGRG() {}
};
SNDMGRG D_003DD040;

// 0x1FF188 StaticInit_sound (g_sndmgr, then the six slot pools; g_sndmgr's destruction) and its stubs.
SLY2_INSTANTIATE_SLOT_POOLS()

/* A sound as the m2c bodies read it: vtable +0xC, pan +0x14, pitch +0x18, voice handle +0x40, refcount +0x4C,
   fade time +0x48; a bank sound's sfx id at +0x60. */
struct SNDV
{
    char pad0[0xC];
    VTESND *pvt;            /* +0x0C */
    char pad10[4];
    int nPan;               /* +0x14 */
    float rPitch;           /* +0x18 */
    char pad1C[0x40 - 0x1C];
    int hvoice;             /* +0x40 */
    char pad44[0x48 - 0x44];
    float dtFade;           /* +0x48 */
    ushort cref;            /* +0x4C */
    char pad4E[0x60 - 0x4E];
    int sfxid;              /* +0x60 (BANKSND) */
};
/* The sound manager as the m2c bodies read it. */
struct SNDMGRV
{
    char pad0[0x1C];
    SLOTHEAP slotheapBanksnd;   /* +0x1C */
    SLOTHEAP slotheapStreamsnd; /* +0x34 */
    DL dlDirty;             /* +0x4C */
    char pad58[0x5C - 0x58];
    int c5C;                /* +0x5C */
    int c60;                /* +0x60 */
    char pad64[0x68 - 0x64];
    int nState;             /* +0x68 (3: muted) */
    char pad6C[0xFC - 0x6C];
    SNDV **apsnd;           /* +0xFC */
    char pad100[0x104 - 0x100];
    int nPlaybackMode;      /* +0x104 */
};
#define G_SNDMGR (*(SNDMGRV *)&D_003DD040)
/* A stream record (func_001FCFA0) with a float parameter at +0x50. */
struct STREAMREC { char pad0[0x50]; float g50; };

extern "C" {
extern void *D_002D9C70[] __asm__("_vt$7BANKSND");  /* BANKSND vtable */
void SetSndFlags(SNDV *psnd, int grfMask, int grf);
SNDV *SND_ctor(SNDV *psnd);
SNDV *BANKSND_ctor(SNDV *psnd, int sfxid);
void *PvAllocBanksnd(int cb);
int *PsndFromSfxid(SNDMGRV *psndmgr, int sfxid);
void snd_PauseSound(int hvoice);
void snd_ContinueSound(int hvoice);
void *PvAllocSlotheapClear(SLOTHEAP *pslotheap);
void func_0011CD98(SLOTHEAP *pslotheap, void *pv);
void func_00233088(int nRpc, int nCmd, int n, int n3, int n4, int n5, int n6);   /* the IOP sound RPC */
STREAMREC *func_001FCFA0(int id);
void snd_SetPlaybackMode(int nMode);
}
int FFindDlEntry(DL *pdl, void *pv);
void AppendDlEntry(DL *pdl, void *pv);

namespace M_NewSfx { extern "C" {
typedef struct MSG { int cb; int nKind; } MSG;
void *func_001AE2A0(MSG *pmsg);
struct E { int n; float a; float b; float c; int d; };
/** @sly2-annot addr=0x1FAF70
 * Purpose: Allocate an SFX descriptor from SW heap: sfxid=-1, sFull=300, sStart=3000, uVol=1
 * Info: proposed name NewSfx (T1); TU sound
 * Confidence: mid (draft) [T1 0.70] - evidence: static analysis; sources: r7_1F8DD8
 * See: docs/engine/physics-collision.md#6-source-files, docs/engine/sound.md#5-source-files,
 *   docs/reference/functions/sound.md#sound-0x1faf70-0x1ff3b8
 * @end-sly2-annot */
struct E *NewSfx(void)
{
    MSG msg;
    struct E *pe;
    msg.cb = sizeof(struct E);
    msg.nKind = 0x10;
    pe = (__typeof__(pe))func_001AE2A0(&msg);
    pe->n = -1;
    pe->a = 300.0f;
    pe->b = 3000.0f;
    pe->c = 1.0f;
    return pe;
}
} } // namespace M_NewSfx

namespace M_SND_ctor { extern "C" {
extern char D_002D9D00[] __asm__("_vt$6CID151");
struct S { int a; int b; int c; void *vt; float g10; char pad[0x1C]; float g30; float g34; char pad2[0xC]; float g44; };
/** @sly2-annot addr=0x1FAFD0
 * Purpose: Base sound-instance ctor (vt 0x2D9D00, CID 0x97): vol 1, pitch-ish 1, range 300..3000
 * Info: proposed name SND_ctor (T1); class SND; TU sound
 * Confidence: mid (draft) [T1 0.60] - evidence: static analysis; sources: r7_1F8DD8
 * See: docs/reference/functions/sound.md#sound-0x1faf70-0x1ff3b8
 * @end-sly2-annot */
struct S *SND_ctor(struct S *p)
{
    p->vt = D_002D9D00;
    p->a = 0; //P
    p->b = 0; //P
    p->g10 = 1.0f; //P
    p->g44 = 1.0f; //P
    p->g30 = 300.0f; //P
    p->g34 = 3000.0f; //P
    return p;
}
} } // namespace M_SND_ctor

namespace M_PauseSnd { extern "C" {
struct S { char pad[0x4E]; short c; };
void SetSndFlags(struct S *, int, int);
/** @sly2-annot addr=0x1FB018
 * Purpose: Refcounted pause: ++cPause==1 -> SetSndFlags(6,2) (request pause)
 * Info: proposed name PauseSnd (T1); TU sound
 * Confidence: mid (draft) [T1 0.70] - evidence: static analysis; sources: r7_1F8DD8
 * See: docs/reference/functions/sound.md#sound-0x1faf70-0x1ff3b8
 * @end-sly2-annot */
void PauseSnd(struct S *p) { if (++p->c == 1) SetSndFlags(p, 6, 2); }
} } // namespace M_PauseSnd

namespace M_ResumeSnd { extern "C" {
struct S { char pad[0x4E]; short c; };
void SetSndFlags(struct S *, int, int);
/** @sly2-annot addr=0x1FB060
 * Purpose: --cPause==0 -> SetSndFlags(6,4) (request continue)
 * Info: proposed name ResumeSnd (T1); TU sound
 * Confidence: mid (draft) [T1 0.70] - evidence: static analysis; sources: r7_1F8DD8
 * See: docs/reference/functions/sound.md#sound-0x1faf70-0x1ff3b8
 * @end-sly2-annot */
void ResumeSnd(struct S *p) { if (--p->c == 0) SetSndFlags(p, 6, 4); }
} } // namespace M_ResumeSnd

/** @sly2-annot addr=0x1FB098
 * Purpose: Request stop (flag 8) with fade time arg
 * Info: proposed name StopSnd (T1); TU sound
 * Confidence: high [T1 0.85] - evidence: static analysis; sources: r7_1F8DD8+verdicts_tu_r7r8
 * See: docs/engine/sound.md#6-key-functions, docs/reference/functions/sound.md#sound-0x1faf70-0x1ff3b8
 * @end-sly2-annot */
extern "C" void StopSnd(SNDV *psnd, float dtFade)
{
    psnd->dtFade = dtFade;
    SetSndFlags(psnd, 0x1FFF, 8);
}

namespace M_StartSnd { extern "C" {
typedef int qword __attribute__((mode(TI)));
struct P { char pad[0x1C]; void *p1C; qword v20; char pad2[0x40 - 0x30]; int n40; char pad3[0x4E - 0x44]; short n4E; unsigned short f50; unsigned short n52; };
extern qword D_002A2DD0;
int FFloatsNear__Ffff(float, float, float);
void SetSndFlags(struct P *, int, int);
static inline float X(qword q) { float f; __asm__("mtc1 %1, %0" : "=f"(f) : "r"(q)); return f; }
static inline float Y(qword q) { float f; __asm__("prot3w %0, %1" : "=r"(f) : "r"(q)); return f; }
static inline float Z(qword q) { float f; __asm__("pextuw %0, $0, %1" : "=r"(f) : "r"(q)); return f; }
static inline int FNear(qword a, qword b)
{
    return FFloatsNear__Ffff(X(a), X(b), 0.0001f) && FFloatsNear__Ffff(Y(a), Y(b), 0.0001f) && FFloatsNear__Ffff(Z(a), Z(b), 0.0001f);
}
/** @sly2-annot addr=0x1FB0C0
 * Purpose: vt7: if not already playing/starting, request start + all dirty bits (0x1F1, +pause if paused,
 *   +0x200 if positional)
 * Info: proposed name StartSnd (T1); class SND; TU sound
 * Confidence: mid (draft) [T1 0.60] - evidence: static analysis; sources: r7_1F8DD8
 * See: docs/reference/functions/sound.md#sound-0x1faf70-0x1ff3b8
 * @end-sly2-annot */
} } // namespace M_StartSnd
void CID151::Start()   /* key function: CID151's vtable comes out here (the namespace's views) */
{
    using namespace M_StartSnd;
    struct P *p = (struct P *)this;
    int f = 0;
    int k;
    if (!p->n40) {
        f = p->n52 || (p->f50 & 1);
        f ^= 1;
    }
    if (!f) return;
    { int fA = p->n4E > 0; k = 0x1F1; if (fA) k = 0x1F3; }
    if (p->p1C || !FNear(D_002A2DD0, p->v20))
        k |= 0x200;
    SetSndFlags(p, k, k);
}

namespace M_func_001FB1F8 { extern "C" {
struct P { char pad[0x10]; float g; };
struct S;
extern struct S D_002BBB60;
float GLimitLm__FP2LMf(struct S *, float);
void SetSndFlags(struct P *, int, int);
/** @sly2-annot addr=0x1FB1F8
 * Purpose: Clamp 0..1 (GLimitLm g_lm01) and set vol, dirty 0x10 if changed at 1/1024 resolution
 * Info: proposed name SetSndVolume (T1); TU sound
 * Confidence: mid (draft) [T1 0.70] - evidence: static analysis; sources: r7_1F8DD8
 * See: docs/reference/functions/sound.md#sound-0x1faf70-0x1ff3b8
 * @end-sly2-annot */
void func_001FB1F8(struct P *p, float x)
{
    float g = GLimitLm__FP2LMf(&D_002BBB60, x);
    if ((int)(g * 1024.0f) != (int)(p->g * 1024.0f)) {
        p->g = g;
        SetSndFlags(p, 0x10, 0x10);
    }
}
} } // namespace M_func_001FB1F8

/** @sly2-annot addr=0x1FB268
 * Purpose: Normalize degrees mod 360; set pan, dirty 0x20
 * Info: proposed name SetSndPan (T1); TU sound
 * Confidence: mid (draft) [T1 0.55] - evidence: static analysis; sources: r7_1F8DD8
 * See: docs/reference/functions/sound.md#sound-0x1faf70-0x1ff3b8
 * @end-sly2-annot */
extern "C" void SetSndPan(SNDV *psnd, int deg)
{
    int nPan = deg % 360;
    nPan = (nPan > -1) ? nPan : nPan + 360;
    if (nPan != psnd->nPan) {
        psnd->nPan = nPan;
        SetSndFlags(psnd, 0x20, 0x20);
    }
}

/** @sly2-annot addr=0x1FB2C0
 * Purpose: Set pitch (+0x18), dirty 0x40
 * Info: proposed name SetSndPitch (T1); TU sound
 * Confidence: mid (draft) [T1 0.55] - evidence: static analysis; sources: r7_1F8DD8
 * See: docs/reference/functions/sound.md#sound-0x1faf70-0x1ff3b8
 * @end-sly2-annot */
extern "C" void SetSndPitch(SNDV *psnd, int n1, int n2, float rPitch)
{
    if (rPitch != psnd->rPitch) {
        psnd->rPitch = rPitch;
        SetSndFlags(psnd, 0x40, 0x40);
    }
}

namespace M_SetSndSourceLo { extern "C" {
typedef int qword __attribute__((mode(TI)));
struct SOURCE;
struct P { char pad[0x1C]; struct SOURCE *source; qword v; };
extern qword D_002A2DD0;
int FFloatsNear__Ffff(float, float, float);
void SetSndFlags(struct P *, unsigned short, unsigned short);
static inline float X(qword q) { float f; __asm__("mtc1 %1, %0" : "=f"(f) : "r"(q)); return f; }
static inline float Y(qword q) { float f; __asm__("prot3w %0, %1" : "=r"(f) : "r"(q)); return f; }
static inline float Z(qword q) { float f; __asm__("pextuw %0, $0, %1" : "=r"(f) : "r"(q)); return f; }
static inline int FNear(qword a, qword b)
{
    return FFloatsNear__Ffff(X(a), X(b), 0.0001f) && FFloatsNear__Ffff(Y(a), Y(b), 0.0001f) && FFloatsNear__Ffff(Z(a), Z(b), 0.0001f);
}
/** @sly2-annot addr=0x1FB2F8
 * Purpose: Attach sound to an ALO (positional), flag 0x200
 * Info: proposed name SetSndSourceLo (T1); TU sound
 * Notes: verifier CORRECT: SetSndSourceAlo -> SetSndSourceLo
 * Confidence: high [T1 0.85] - evidence: static analysis; sources: r7_1F8DD8+verdicts_tu_r7r8
 * See: docs/engine/animation.md#7-key-functions, docs/engine/main-loop-startup.md#8-key-functions,
 *   docs/engine/sound.md#2-sound-instances
 * @end-sly2-annot */
void SetSndSourceLo(struct P *p, struct SOURCE *source)
{
    p->source = source;
    if (source) SetSndFlags(p, 0x200, 0x200);
    else if (FNear(D_002A2DD0, p->v)) SetSndFlags(p, 0x200, 0);
}
} } // namespace M_SetSndSourceLo

namespace M_SetSndSourcePos { extern "C" {
typedef int qword __attribute__((mode(TI)));
struct P { char pad[0x1C]; int n1C; qword v; };
extern qword D_002A2DD0;
int FFloatsNear__Ffff(float, float, float);
void SetSndFlags(struct P *, int, int);
static inline float X(qword q) { float f; __asm__("mtc1 %1, %0" : "=f"(f) : "r"(q)); return f; }
static inline float Y(qword q) { float f; __asm__("prot3w %0, %1" : "=r"(f) : "r"(q)); return f; }
static inline float Z(qword q) { float f; __asm__("pextuw %0, $0, %1" : "=r"(f) : "r"(q)); return f; }
static inline int FNear(qword a, qword b)
{
    return FFloatsNear__Ffff(X(a), X(b), 0.0001f) && FFloatsNear__Ffff(Y(a), Y(b), 0.0001f) && FFloatsNear__Ffff(Z(a), Z(b), 0.0001f);
}
/** @sly2-annot addr=0x1FB3F0
 * Purpose: Set world position (positional unless zero vector), flag 0x200
 * Info: proposed name SetSndSourcePos (T1); TU sound
 * Confidence: mid (draft) [T1 0.55] - evidence: static analysis; sources: r7_1F8DD8
 * See: docs/reference/functions/sound.md#sound-0x1faf70-0x1ff3b8
 * @end-sly2-annot */
void SetSndSourcePos(struct P *p, qword v)
{
    p->v = v;
    if (!FNear(D_002A2DD0, v)) SetSndFlags(p, 0x200, 0x200);
    else if (!p->n1C) SetSndFlags(p, 0x200, 0);
}
} } // namespace M_SetSndSourcePos

namespace M_SetSndRange { extern "C" {
struct S { char pad[0x30]; float a; float b; };
/** @sly2-annot addr=0x1FB4F0
 * Purpose: Set full/stop distance (stop forced > full+1)
 * Info: proposed name SetSndRange (T1); TU sound
 * Confidence: mid (draft) [T1 0.50] - evidence: static analysis; sources: r7_1F8DD8
 * See: docs/reference/functions/sound.md#sound-0x1faf70-0x1ff3b8
 * @end-sly2-annot */
void SetSndRange(struct S *p, float a, float b) { if (b <= a) { b = a + 1.0f; p->b = b; } else p->b = b; p->a = a; }
} } // namespace M_SetSndRange

/** @sly2-annot addr=0x1FB520
 * Purpose: Empty stub (jr ra) taking a float; called with the fade/doppler arg in 0x210740
 * Info: proposed name SetSndDoppler (T2); TU sound
 * Confidence: low (draft) [T2 0.20] - evidence: static analysis; sources: r7_1F8DD8
 * See: docs/reference/functions/sound.md#sound-0x1faf70-0x1ff3b8
 * @end-sly2-annot */
/* SetSndDoppler: empty */
extern "C" void func_001FB520()
{
}

namespace M_UpdateSndPositional { extern "C" {
typedef int qword __attribute__((mode(TI)));
typedef unsigned long ulong;
struct VTE { short d; short pad; void (*pfn)(void *, int); };
struct O { char pad[0x28]; ulong fl; };
struct P { char pad[0xC]; struct VTE *pvt; float f10; char pad1[0x1C - 0x14]; struct O *po; qword v20; char x30[0x14]; float f44; };
void StopSnd(struct P *, float);
qword ConvertAloPos(struct O *, int, qword);
void CalculateSndVolPan(struct P *, qword, void *, float *, int *);
void SetSndFlags(struct P *, int, int);
void SetSndPan(struct P *, int);
/** @sly2-annot addr=0x1FB528
 * Purpose: vt16: from source ALO/pos compute distance volume and pan angle (CalculateSndVolPan); vt15 on
 *   audible<->silent transitions
 * Info: proposed name UpdateSndPositional (T1); class SND; TU sound
 * Confidence: mid (draft) [T1 0.55] - evidence: static analysis; sources: r7_1F8DD8
 * See: docs/reference/functions/sound.md#sound-0x1faf70-0x1ff3b8
 * @end-sly2-annot */
void UpdateSndPositional(struct P *p)
{
    float f;
    int n;
    float r;
    if (p->po && (((p->po->fl >> 60) & 1) == 0)) {
        StopSnd(p, 0.0f);
        return;
    }
    CalculateSndVolPan(p, ConvertAloPos(p->po, 0, p->v20), p->x30, &f, &n);
    r = p->f10 * f;
    if ((int)(r * 1024.0f) != (int)(p->f44 * 1024.0f)) {
        if (p->f44 == 0.0f) p->pvt[15].pfn((char *)p + p->pvt[15].d, 0);
        else if (r == 0.0f) p->pvt[15].pfn((char *)p + p->pvt[15].d, 1);
        p->f44 = r;
        SetSndFlags(p, 0x10, 0x10);
    }
    SetSndPan(p, n);
}
} } // namespace M_UpdateSndPositional

namespace M_SetSndFlags { extern "C" {
struct S { char pad[0x50]; unsigned short fl; };
void AddSndToDirtyList(struct S *);
void RemoveSndFromDirtyList(struct S *);
/** @sly2-annot addr=0x1FB658
 * Purpose: fl = (fl&~mask)|(val&mask); nonzero -> link into SNDMGR dirty list, zero -> unlink
 * Info: proposed name SetSndFlags (T1); TU sound
 * Confidence: mid (draft) [T1 0.75] - evidence: static analysis; sources: r7_1F8DD8
 * See: docs/engine/sound.md#2-sound-instances, docs/engine/sound.md#6-key-functions,
 *   docs/reference/functions/sound.md#sound-0x1faf70-0x1ff3b8
 * @end-sly2-annot */
void SetSndFlags(struct S *p, unsigned short m, unsigned short v)
{
    p->fl = (p->fl & ~m) | (v & m);
    if (p->fl) AddSndToDirtyList(p); else RemoveSndFromDirtyList(p);
}
} } // namespace M_SetSndFlags

/** @sly2-annot addr=0x1FB6A8
 * Purpose: If not in SNDMGR pending DL, append and Incref
 * Info: proposed name AddSndToDirtyList (T1); TU sound
 * Confidence: mid (draft) [T1 0.60] - evidence: static analysis; sources: r7_1F8DD8
 * See: docs/reference/functions/sound.md#sound-0x1faf70-0x1ff3b8
 * @end-sly2-annot */
extern "C" void AddSndToDirtyList(SNDV *psnd)
{
    if (FFindDlEntry(&G_SNDMGR.dlDirty, psnd) == 0) {
        VTESND *pvt = psnd->pvt;
        pvt[1].pfn((char *)psnd + pvt[1].d);   /* Incref */
        AppendDlEntry(&G_SNDMGR.dlDirty, psnd);
    }
}

namespace M_RemoveSndFromDirtyList { extern "C" {
struct VT { int pad[4]; short d; short p; void (*pfn)(void *); };
struct X { char pad[0xC]; struct VT *pvt; };
struct DL;
int FFindDlEntry__FP2DLPv(struct DL *, void *);
void RemoveDlEntry__FP2DLPv(struct DL *, void *);
/** @sly2-annot addr=0x1FB710
 * Purpose: If in pending DL, remove and Decref (vt2)
 * Info: proposed name RemoveSndFromDirtyList (T1); TU sound
 * Confidence: mid (draft) [T1 0.60] - evidence: static analysis; sources: r7_1F8DD8
 * See: docs/reference/functions/sound.md#sound-0x1faf70-0x1ff3b8
 * @end-sly2-annot */
void RemoveSndFromDirtyList(struct X *px)
{
    if (FFindDlEntry__FP2DLPv((struct DL *)&D_003DD040.dlSndDirty, px)) {
        RemoveDlEntry__FP2DLPv((struct DL *)&D_003DD040.dlSndDirty, px);
        px->pvt->pfn((char *)px + px->pvt->d);
    }
}
} } // namespace M_RemoveSndFromDirtyList

namespace M_CalculateSndVolPan { extern "C" {
typedef int qword __attribute__((mode(TI)));
typedef union {qword q;float f[4];int i[4];} V;
struct RANGE {float low,high;};
struct M3 {qword r[3];};
struct CAMERA {char pad[0x90];qword position;};
extern char D_003DD490[],D_002BBB60[];
extern struct CAMERA D_002DDF40;
void *PpoCur(void*);
qword GetLoWorldPos(void*);
qword ConvertMatVec(const struct M3*,const struct M3*,qword);
float GLimitLm__FP2LMf(void*,float);
float atan2f(float,float);
static inline qword Sub(qword a,qword b){register qword r __asm__("vf3");__asm__("vsub.xyzw %0,%1,%2":"=&j"(r):"j"(a),"j"(b));return r;}
static inline float Length(qword a,V*save){qword d,one;float g;
__asm__("vmul.xyz %0,%2,%2\n\tvaddw.x %1,$vf0,$vf0w\n\tvadday.x $ACC,%0,%0y\n\tvmaddz.x %0,%1,%0z":"=&j"(d),"=&j"(one):"j"(a));
__asm__("vsqrt $Q,%1x\n\tvwaitq\n\tvaddq.x %0,$vf0,$Q":"=j"(d):"j"(d));
save->q=a;
__asm__("qmfc2.ni %0,%1":"=r"(g):"j"(d));return g;}
static inline float X(qword q){float f;__asm__("mtc1 %1,%0":"=f"(f):"r"(q));return f;}
static inline float Y(qword q){int bits;float f;__asm__("prot3w %0,%1":"=r"(bits):"r"(q));__asm__("mtc1 %1,%0":"=f"(f):"r"(bits));return f;}
static inline qword Bits(qword v){qword q;__asm__("qmfc2.ni %0,%1":"=r"(q):"j"(v));return q;}
static inline void Rounded(int*out,float f){float bits;__asm__("cvt.w.s %0,%1":"=&f"(bits):"f"(f));__asm__("swc1 %1,%0":"=m"(*out):"f"(bits));}
/** @sly2-annot addr=0x1FB778
 * Purpose: Distance from ear (player PosLoWorld or camera) -> volume via (sStop-d)/(sStop-sFull) clamped;
 *   atan2 -> pan degrees
 * Info: proposed name CalculateSndVolPan (T1); TU sound
 * Confidence: high [T1 0.85] - evidence: static analysis; sources: r7_1F8DD8+verdicts_tu_r7r8
 * See: docs/engine/sound.md#6-key-functions, docs/reference/functions/sound.md#sound-0x1faf70-0x1ff3b8
 * @end-sly2-annot */
void CalculateSndVolPan(void *unused,qword point,const struct RANGE*limits,float*outValue,int*outAngle)
{
 V source,delta;
 void *player;
 register qword reference __asm__("vf1");
 float high,low,distance,value;
 register qword difference __asm__("vf3");
 source.q=point;
 player=PpoCur(D_003DD490);
 if(player)reference=GetLoWorldPos(player);
 else reference=D_002DDF40.position;
 high=limits->high;low=limits->low;
 {register qword input __asm__("vf2")=source.q;
  distance=Length(Sub(input,reference),&delta);}
 value=GLimitLm__FP2LMf(D_002BBB60,(high-distance)/(high-low));
 difference=delta.q;
 if(outValue)*outValue=value;
 if(outAngle){
   if(value>0.0f){
     qword direction;
     float angle;
     direction=ConvertMatVec(0,&(*(struct M3 *)&g_cm.mat),Bits(difference));
     angle=atan2f(-Y(direction),X(direction));
     if(limits->high<limits->low)angle+=3.1415927f;
     Rounded(outAngle,angle*57.295776f);return;
   }else *outAngle=0;
 }
}
} } // namespace M_CalculateSndVolPan

namespace M_UpdateSnd {
struct VTE { short d; short p; void (*pfn)(void *); };
struct P { char pad[0xC]; VTE *pvt; float f10; char pad2[0x40 - 0x14]; int n40; float f44; char pad3[0x50 - 0x48]; unsigned short f50; unsigned short n52; };
extern "C" void SetSndFlags(P *, int, int);
#define VCALL(p, n) ((p)->pvt[n].pfn((char *)(p) + (p)->pvt[n].d))
#define ICALL(p, n) (((int (*)(void *))(p)->pvt[n].pfn)((char *)(p) + (p)->pvt[n].d))
struct Lock {
    P *p;
    Lock(P *pp) { p = pp; VCALL(p, 1); }
    ~Lock() { VCALL(p, 2); }
};
/** @sly2-annot addr=0x1FB8E0
 * Purpose: Per-frame flush of a dirty sound: positional update;
 *   start(vt9)/stop(vt10)/status(vt14)/params(vt13)/pause(vt11)/continue(vt12) by flag bits
 * Info: proposed name UpdateSnd (T1); TU sound
 * Confidence: mid (draft) [T1 0.70] - evidence: static analysis; sources: r7_1F8DD8
 * See: docs/engine/sound.md#2-sound-instances, docs/reference/functions/sound.md#sound-0x1faf70-0x1ff3b8
 * @end-sly2-annot */
extern "C" void UpdateSnd(P *p)
{
    Lock l(p);
    unsigned short fl;
    if (p->f50 & 0x200) VCALL(p, 16);
    else p->f44 = p->f10;
    fl = p->f50;
    if (fl & 1) {
        VCALL(p, 9);
        if (ICALL(p, 8) == 1) SetSndFlags(p, 0x400, 0x400);
    } else if (p->n40) {
        if (fl & 8) {
            VCALL(p, 10);
            p->n40 = 0;
            SetSndFlags(p, 0x1DFF, 0);
        } else {
            if (fl & 0x1C00) VCALL(p, 14);
            if (p->n52) return;
            if (p->f50 & 0x1F0) {
                VCALL(p, 13);
                SetSndFlags(p, 0x1F0, 0);
            }
            fl = p->f50;
            if (fl & 2) {
                VCALL(p, 11);
                SetSndFlags(p, 2, 0);
            } else if (fl & 4) {
                VCALL(p, 12);
                SetSndFlags(p, 4, 0);
            }
        }
    } else if (!p->n52 && !(fl & 0x200)) {
        SetSndFlags(p, 0x1FFF, 0);
    }
}
} // namespace M_UpdateSnd
#undef VCALL
#undef ICALL

namespace M_func_001FBAF8 { extern "C" {
struct VTE { short d; short pad; void (*pfn)(void *); };
struct S { int a[3]; struct VTE *vt; char pad[0x30]; int x40; char pad2[0xE]; short x52; };
/** @sly2-annot addr=0x1FBAF8
 * Purpose: 989snd async callback: store voice handle (+0x40), clear busy (+0x52), Decref
 * Info: proposed name OnBanksndPlayStarted (T2); TU sound
 * Confidence: low (draft) [T2 0.45] - evidence: static analysis; sources: r7_1F8DD8
 * See: docs/reference/functions/sound.md#sound-0x1faf70-0x1ff3b8
 * @end-sly2-annot */
void func_001FBAF8(int x, long l) { struct S *p = (struct S *)(int)l; p->x40 = x; p->x52 = 0; p->vt[2].pfn((char *)p + p->vt[2].d); }
} } // namespace M_func_001FBAF8

namespace M_func_001FBB30 { extern "C" {
struct VTE { short d; short pad; void (*pfn)(void *); };
struct A { char pad[0xC]; struct VTE *vt; };
void StopSnd(struct A *p, float g);
void SetSndFlags(struct A *p, int a, int b);
/** @sly2-annot addr=0x1FBB30
 * Purpose: IsStillPlaying callback: not playing -> StopSnd; else re-arm status query (0x400); Decref
 * Info: proposed name OnBanksndStatus (T2); TU sound
 * Confidence: low (draft) [T2 0.45] - evidence: static analysis; sources: r7_1F8DD8
 * See: docs/reference/functions/sound.md#sound-0x1faf70-0x1ff3b8
 * @end-sly2-annot */
void func_001FBB30(int f, long l)
{
    struct A *p = (struct A *)l;
    if (!f)
        StopSnd(p, 0.0f);
    else
        SetSndFlags(p, 0x400, 0x400);
    p->vt[2].pfn((char *)p + p->vt[2].d);
}
} } // namespace M_func_001FBB30

/** @sly2-annot addr=0x1FBB90
 * Purpose: Allocate a BANKSND (0x70) from SNDMGR pool and construct it with sfxid; it starts on next
 *   UpdateSnd
 * Info: proposed name PsndStartSound (T1); TU sound
 * Notes: names_v2 B proposed SetPambVol__FP3AMBf (0.856)
 * Confidence: mid (draft) [T1 0.60] - evidence: static naming pass; sources: r7_1F8DD8
 * See: docs/engine/animation.md#7-key-functions, docs/engine/main-loop-startup.md#8-key-functions,
 *   docs/engine/overview.md#6-subsystem-map
 * @end-sly2-annot */
extern "C" void PsndStartSound(int sfxid)
{
    BANKSND_ctor((SNDV *)PvAllocBanksnd(0x70), sfxid);
}

/** @sly2-annot addr=0x1FBBC8
 * Purpose: BANKSND ctor: SND ctor, set start flag (vt D_002D9C70 store dropped by m2c)
 * Info: proposed name BANKSND_ctor (T1); class BANKSND; TU sound
 * Confidence: mid (draft) [T1 0.50] - evidence: static analysis; sources: r7_1F8DD8
 * See: docs/reference/functions/sound.md#sound-0x1faf70-0x1ff3b8
 * @end-sly2-annot */
extern "C" SNDV *BANKSND_ctor(SNDV *psnd, int sfxid)
{
    SND_ctor(psnd);
    psnd->sfxid = sfxid;
    psnd->pvt = (VTESND *)D_002D9C70;
    SetSndFlags(psnd, 1, 1);
    return psnd;
}

/** @sly2-annot addr=0x1FBC20
 * Purpose: vt8: kind of this sfx from SNDMGR table (2 = looping) or -1
 * Info: proposed name GetBanksndKind (T1); class BANKSND; TU sound
 * Confidence: mid (draft) [T1 0.50] - evidence: static analysis; sources: r7_1F8DD8
 * See: docs/reference/functions/sound.md#sound-0x1faf70-0x1ff3b8
 * @end-sly2-annot */
extern "C" int GetBanksndKind(SNDV *psnd)
{
    int *pn = PsndFromSfxid(&G_SNDMGR, psnd->sfxid);
    int n = -1;
    if (pn != 0) {
        n = *pn;
    }
    return n;
}

namespace M_PlayBanksnd { extern "C" {
struct VTE { short d; short p; void (*pfn)(void *); };
struct P { char pad[0xC]; struct VTE *pvt; char pad1[0x14 - 0x10]; int n14; float f18; char pad2[0x44 - 0x1C]; float f44; char pad3[0x50 - 0x48]; unsigned short f50; unsigned short n52; char pad4[0x60 - 0x54]; int n60; };
struct G { char pad[0xE8]; int nE8; };
struct S { int n0; };
struct M { int n0; int pad4; int n8; int nC; short h10; short pad12; int pad14[3]; short h20; short pad22; int pad24; int n28; int pad2C; };
extern struct G D_003DD040;
extern char D_003DD490[];
extern int D_002DD5BC, D_002F7900, D_002E1E40;
struct S *PsndFromSfxid(struct G *, int);
short IsndFromPsnd(struct G *, struct S *);
void SetSndFlags(struct P *, int, int);
void RemoveSndFromDirtyList(struct P *);
void *func_002333B4(void *, int, int) __asm__("memset");
void func_001FBAF8(void);
void func_00232320(struct M *, void (*)(void), struct P *);
void func_002322F0(struct M *);
int PpoHero(void *);
void func_001FEED0(struct G *);
/** @sly2-annot addr=0x1FBC58
 * Purpose: vt9: build 989 play params (bank, sound index, vol/pan/pitch); loopers play async with handle
 *   callback, one-shots fire-and-forget; per-sfx special cases
 * Info: proposed name PlayBanksnd (T1); class BANKSND; TU sound
 * Confidence: mid (draft) [T1 0.60] - evidence: static analysis; sources: r7_1F8DD8
 * See: docs/reference/functions/sound.md#sound-0x1faf70-0x1ff3b8
 * @end-sly2-annot */
void PlayBanksnd(struct P *p)
{
    struct G *pg = &D_003DD040;
    struct S *ps = PsndFromSfxid(pg, p->n60);
    struct M m;
    int id;
    if (!ps || !pg->nE8) {
        SetSndFlags(p, 0x1FFF, 0);
        return;
    }
    if (p->f50 & 2) {
        RemoveSndFromDirtyList(p);
        return;
    }
    func_002333B4(&m, 0, 0x30);
    m.n28 = 3;
    m.n0 = pg->nE8;
    m.h10 = IsndFromPsnd(pg, ps);
    if (p->f50 & 0x10) {
        m.n28 |= 4;
        m.n8 = (int)(p->f44 * 1024.0f);
    }
    if (p->f50 & 0x20) {
        m.n28 |= 8;
        m.nC = p->n14;
    }
    if (p->f50 & 0x40) {
        m.n28 |= 0x10;
        m.h20 = (int)(p->f18 * 1524.0f);
    }
    if (ps->n0 == 2) {
        p->n52 = 1;
        p->pvt[1].pfn((char *)p + p->pvt[1].d);
        func_00232320(&m, func_001FBAF8, p);
        SetSndFlags(p, 0x7F, 0);
        return;
    }
    id = 0;
    func_002322F0(&m);
    SetSndFlags(p, 0x1FFF, 0);
    switch (p->n60) {
    case 0x6F: id = D_002E1E40; break;
    case 0x73: id = D_002DD5BC; break;
    case 0x15: id = D_002F7900; break;
    }
    if (id == PpoHero(D_003DD490)) func_001FEED0(&D_003DD040);
}
} } // namespace M_PlayBanksnd

namespace M_StopBanksnd { extern "C" {
struct A { char pad[0x40]; int h; char pad2[4]; float g; };
void snd_AutoVol(int h, int a, int b, int c);
void snd_StopSound(int h);
/** @sly2-annot addr=0x1FBE80
 * Purpose: vt10: fade-out via snd_AutoVol(h,-4,dt*240,2) if dt>0 else snd_StopSound(h)
 * Info: proposed name StopBanksnd (T1); class BANKSND; TU sound
 * Confidence: mid (draft) [T1 0.60] - evidence: static analysis; sources: r7_1F8DD8
 * See: docs/reference/functions/sound.md#sound-0x1faf70-0x1ff3b8
 * @end-sly2-annot */
void StopBanksnd(struct A *p)
{
    if (p->g > 0.0f)
        snd_AutoVol(p->h, -4, (int)(p->g * 240.0f), 2);
    else
        snd_StopSound(p->h);
}
} } // namespace M_StopBanksnd

/** @sly2-annot addr=0x1FBEE0
 * Purpose: vt11: snd_PauseSound(handle)
 * Info: proposed name PauseBanksnd (T1); class BANKSND; TU sound
 * Confidence: mid (draft) [T1 0.65] - evidence: static analysis; sources: r7_1F8DD8
 * See: docs/reference/functions/sound.md#sound-0x1faf70-0x1ff3b8
 * @end-sly2-annot */
extern "C" void PauseBanksnd(SNDV *psnd)
{
    snd_PauseSound(psnd->hvoice);
}

/** @sly2-annot addr=0x1FBF00
 * Purpose: vt12: snd_ContinueSound(handle)
 * Info: proposed name ContinueBanksnd (T1); class BANKSND; TU sound
 * Confidence: mid (draft) [T1 0.65] - evidence: static analysis; sources: r7_1F8DD8
 * See: docs/reference/functions/sound.md#sound-0x1faf70-0x1ff3b8
 * @end-sly2-annot */
extern "C" void ContinueBanksnd(SNDV *psnd)
{
    snd_ContinueSound(psnd->hvoice);
}

namespace M_ApplyBanksndParams { extern "C" {
struct X { char pad[0x14]; int n14; float g18; char pad2[0x40 - 0x1C]; void *p40; float g44; char pad3[0x50 - 0x48]; unsigned short fl; };
void func_00232500(void *, int, int, int, int, int);
/** @sly2-annot addr=0x1FBF20
 * Purpose: vt13: send vol(*1024)/pan/pitch(*12) for dirty bits 0x10/0x20/0x40
 * Info: proposed name ApplyBanksndParams (T1); class BANKSND; TU sound
 * Confidence: mid (draft) [T1 0.55] - evidence: static analysis; sources: r7_1F8DD8
 * See: docs/reference/functions/sound.md#sound-0x1faf70-0x1ff3b8
 * @end-sly2-annot */
void ApplyBanksndParams(struct X *px)
{
    int fl = px->fl;
    if (fl & 0x70) {
        int m = 0;
        int a, b;
        if (fl & 0x10) { m = 1; a = (int)(px->g44 * 1024.0f); }
        if (fl & 0x20) m |= 4;
        if (fl & 0x40) { m |= 8; b = (int)(px->g18 * 12.0f); }
        func_00232500(px->p40, m, a, px->n14, b, 0);
    }
}
} } // namespace M_ApplyBanksndParams

namespace M_BANKSND_QueryStatus { extern "C" {
struct VT { int pad[2]; short d; short p; void (*pfn)(void *); };
struct X { char pad[0xC]; struct VT *pvt; char pad2[0x40 - 0x10]; void *p40; char pad3[0x50 - 0x44]; unsigned short fl; };
void func_001FBB30(void);
void snd_SoundIsStillPlaying_CB(void *, void (*)(void), void *);
void SetSndFlags(struct X *, int, int);
/** @sly2-annot addr=0x1FBFB0
 * Purpose: vt14: if 0x400 pending, Incref and ask IOP IsStillPlaying with callback OnBanksndStatus
 * Info: proposed name BANKSND_QueryStatus (T1); class BANKSND; TU sound
 * Confidence: mid (draft) [T1 0.50] - evidence: static analysis; sources: r7_1F8DD8
 * See: docs/reference/functions/sound.md#sound-0x1faf70-0x1ff3b8
 * @end-sly2-annot */
void BANKSND_QueryStatus(struct X *px)
{
    if (px->fl & 0x400) {
        px->pvt->pfn((char *)px + px->pvt->d);
        snd_SoundIsStillPlaying_CB(px->p40, func_001FBB30, px);
        SetSndFlags(px, 0x400, 0);
    }
}
} } // namespace M_BANKSND_QueryStatus

namespace M_func_001FC018 { extern "C" {
struct VTE { short d; short p; void *pfn; };
struct VT { struct VTE a[9]; };
struct P { char pad[0xC]; struct VT *pvt; };
void SetSndFlags(struct P *, int, int);
/** @sly2-annot addr=0x1FC018
 * Purpose: vt15: on becoming audible restart (vt7); on silence request stop (loopers keep params)
 * Info: proposed name SetBanksndCulled (T2); class BANKSND; TU sound
 * Confidence: low (draft) [T2 0.40] - evidence: static analysis; sources: r7_1F8DD8
 * See: docs/reference/functions/sound.md#sound-0x1faf70-0x1ff3b8
 * @end-sly2-annot */
void func_001FC018(struct P *p, int f)
{
    if (!f) ((void (*)(void *))p->pvt->a[7].pfn)((char *)p + p->pvt->a[7].d);
    else {
        int n = 0x1FF;
        SetSndFlags(p, ((int (*)(void *))p->pvt->a[8].pfn)((char *)p + p->pvt->a[8].d) == 2 ? n : 0x3FF, 8);
    }
}
} } // namespace M_func_001FC018

/** @sly2-annot addr=0x1FC090
 * Purpose: vt1: ++cref (u16 +0x4C)
 * Info: proposed name IncrefBanksnd (T1); class BANKSND; TU sound
 * Confidence: mid (draft) [T1 0.70] - evidence: static analysis; sources: r7_1F8DD8
 * See: docs/reference/functions/sound.md#sound-0x1faf70-0x1ff3b8
 * @end-sly2-annot */
void BANKSND::VUnk00()   /* key function: BANKSND's vtable comes out here */
{
    SNDV *psnd = (SNDV *)this;
    psnd->cref++;
}

namespace M_DecrefBanksnd { extern "C" {
struct S { char pad[0x4C]; short c; };
void func_00258348(struct S *, int);
void FreeBanksnd(struct S *);
/** @sly2-annot addr=0x1FC0A0
 * Purpose: vt2: --cref==0 -> dtor 0x258348 + free to pool
 * Info: proposed name DecrefBanksnd (T1); class BANKSND; TU sound
 * Confidence: mid (draft) [T1 0.70] - evidence: static analysis; sources: r7_1F8DD8
 * See: docs/reference/functions/sound.md#sound-0x1faf70-0x1ff3b8
 * @end-sly2-annot */
void DecrefBanksnd(struct S *p) { if (--p->c == 0 && p) { func_00258348(p, 0); FreeBanksnd(p); } }
} } // namespace M_DecrefBanksnd

/** @sly2-annot addr=0x1FC0F0
 * Purpose: Allocate from SNDMGR 0x70-byte pool (+0x1C)
 * Info: proposed name PvAllocBanksnd (T1); TU sound
 * Confidence: mid (draft) [T1 0.55] - evidence: static analysis; sources: r7_1F8DD8
 * See: docs/reference/functions/sound.md#sound-0x1faf70-0x1ff3b8
 * @end-sly2-annot */
extern "C" void *PvAllocBanksnd(int cb)
{
    return PvAllocSlotheapClear(&G_SNDMGR.slotheapBanksnd);
}

/** @sly2-annot addr=0x1FC110
 * Purpose: Free to SNDMGR 0x70-byte pool
 * Info: proposed name FreeBanksnd (T1); TU sound
 * Confidence: mid (draft) [T1 0.55] - evidence: static analysis; sources: r7_1F8DD8
 * See: docs/reference/functions/sound.md#sound-0x1faf70-0x1ff3b8
 * @end-sly2-annot */
extern "C" void FreeBanksnd(void *pv)
{
    func_0011CD98(&G_SNDMGR.slotheapBanksnd, pv);
}

namespace M_PstreamsndNew {
struct A; struct B;
struct X {
    static void *operator new(unsigned int cb) __asm__("PvAllocStreamsnd");
    X(A *pa, B *pb) __asm__("STREAMSND_ctor");
    char pad[0x3C0];
};
/** @sly2-annot addr=0x1FC138
 * Purpose: new STREAMSND(SNDSTREAM*, kind) (0x3C0 bytes) or NULL
 * Info: proposed name PstreamsndNew (T1); TU sound
 * Confidence: mid (draft) [T1 0.55] - evidence: static analysis; sources: r7_1F8DD8
 * See: docs/reference/functions/sound.md#sound-0x1faf70-0x1ff3b8
 * @end-sly2-annot */
extern "C" X *PstreamsndNew(A *pa, B *pb)
{
    X *px;
    if (pa)
        px = new X(pa, pb);
    else
        px = 0;
    return px;
}
} // namespace M_PstreamsndNew

namespace M_STREAMSND_ctor { extern "C" {
struct P { char pad[0xC]; void *pvt; char pad2[0x60 - 0x10]; int n60; char pad3[0x70 - 0x64]; float a[6]; char pad4[0x3B8 - 0x88]; int n3B8; };
struct G { char pad[0x58]; int n58; char pad2[0x15C - 0x5C]; float a[1]; };
extern struct G D_003DD040;
extern char D_002D9BE0[] __asm__("_vt$9STREAMSND");
void SND_ctor(struct P *);
void SetSndFlags(struct P *, int, int);
int func_001FD080(struct G *, int);
void PauseSnd(struct P *);
/** @sly2-annot addr=0x1FC190
 * Purpose: STREAMSND ctor: SND ctor, vt D_002D9BE0, stream record + kind, 6 group volumes=1; auto-pause if
 *   its channel volume is 0 or SNDMGR paused
 * Info: proposed name STREAMSND_ctor (T1); class STREAMSND; TU sound
 * Confidence: mid (draft) [T1 0.55] - evidence: static analysis; sources: r7_1F8DD8
 * See: docs/reference/functions/sound.md#sound-0x1faf70-0x1ff3b8
 * @end-sly2-annot */
struct P *STREAMSND_ctor(struct P *p, int n, int m)
{
    int i;
    struct G *pg;
    SND_ctor(p);
    p->n60 = n;
    p->pvt = D_002D9BE0;
    p->n3B8 = m;
    for (i = 0; i < 6; i++) p->a[i] = 1.0f;
    pg = &D_003DD040;
    SetSndFlags(p, 0x801, 0x801);
    if (*(pg->a + func_001FD080(pg, p->n60)) == 0.0f) PauseSnd(p);
    if (pg->n58) PauseSnd(p);
    return p;
}
} } // namespace M_STREAMSND_ctor

namespace M_StartStreamsnd { extern "C" {
struct P { char pad[0x40]; int n40; char pad2[0x50 - 0x44]; unsigned short fl; unsigned short c52; char pad3[0x68 - 0x54]; long l68; char pad4[0x3BC - 0x70]; int n3BC; };
void StartSnd(struct P *);
/** @sly2-annot addr=0x1FC270
 * Purpose: vt7: base Start then reset play time and status mask 0xF
 * Info: proposed name StartStreamsnd (T1); class STREAMSND; TU sound
 * Confidence: mid (draft) [T1 0.50] - evidence: static analysis; sources: r7_1F8DD8
 * See: docs/reference/functions/sound.md#sound-0x1faf70-0x1ff3b8
 * @end-sly2-annot */
void StartStreamsnd(struct P *p)
{
    int f = 0; if (!p->n40) { f = p->c52 || (p->fl & 1); f ^= 1; } if (f) {
        StartSnd(p);
        p->l68 = 0;
        p->n3BC = 0xF;
    }
}
} } // namespace M_StartStreamsnd

namespace M_SetStreamsndGroupVolume { extern "C" {
struct P { char pad[0x70]; float a[6]; char pad2[0x3BC - 0x88]; int fl; };
struct S;
extern struct S D_002BBB60;
float GLimitLm__FP2LMf(struct S *, float);
void SetSndFlags(struct P *, int, int);
/** @sly2-annot addr=0x1FC2E0
 * Purpose: Set volume of sub-group k<6 (clamped), dirty 0x80
 * Info: proposed name SetStreamsndGroupVolume (T1); TU sound
 * Confidence: mid (draft) [T1 0.50] - evidence: static analysis; sources: r7_1F8DD8
 * See: docs/reference/functions/sound.md#sound-0x1faf70-0x1ff3b8
 * @end-sly2-annot */
void SetStreamsndGroupVolume(struct P *p, unsigned int k, float g)
{
    if (k < 6) {
        float f = GLimitLm__FP2LMf(&D_002BBB60, g);
        if ((int)(f * 1024.0f) != (int)(p->a[k] * 1024.0f)) {
            p->fl |= 1 << k;
            p->a[k] = f;
            SetSndFlags(p, 0x80, 0x80);
        }
    }
}
} } // namespace M_SetStreamsndGroupVolume

namespace M_func_001FC380 { extern "C" {
struct P { char pad[0x88]; int a[6]; char pad2[0x3BC - 0xA0]; int fl; };
void SetSndFlags(struct P *, int, int);
static inline int Norm(int deg) { int n = deg % 360; return n > -1 ? n : n + 360; }
/** @sly2-annot addr=0x1FC380
 * Purpose: Set pan of sub-group k<6, dirty 0x100
 * Info: proposed name SetStreamsndGroupPan (T2); TU sound
 * Confidence: low (draft) [T2 0.45] - evidence: static analysis; sources: r7_1F8DD8
 * See: docs/reference/functions/sound.md#sound-0x1faf70-0x1ff3b8
 * @end-sly2-annot */
void func_001FC380(struct P *p, unsigned int k, int deg)
{
    if (k < 6) {
        int n = Norm(deg);
        if (n != p->a[k]) {
            p->fl |= 1 << k;
            p->a[k] = n;
            SetSndFlags(p, 0x100, 0x100);
        }
    }
}
} } // namespace M_func_001FC380

namespace M_func_001FC408 { extern "C" {
struct R { float lo, hi; };
struct E { int n; struct R r; float g; };
struct P { char pad[0xA0]; int c[6]; struct E ae[6][8]; };
void SetSndFlags(struct P *, int, int);
/** @sly2-annot addr=0x1FC408
 * Purpose: Append (object, volume, range) to sub-group k (max 8 per group); positional flag 0x200
 * Info: proposed name AddStreamsndGroupSource (T2); TU sound
 * Confidence: low (draft) [T2 0.45] - evidence: static analysis; sources: r7_1F8DD8
 * See: docs/engine/animation.md#7-key-functions, docs/reference/functions/animation.md#amb-0x12eb48-0x12f0f0,
 *   docs/reference/functions/sound.md#sound-0x1faf70-0x1ff3b8
 * @end-sly2-annot */
void func_001FC408(struct P *p, unsigned int k, int n, struct R *pr, float g)
{
    if (k < 6 && p->c[k] < 8) {
        struct E *pe;
        SetSndFlags(p, 0x200, 0x200);
        pe = &p->ae[k][p->c[k]++];
        pe->n = n;
        pe->g = g;
        pe->r = *pr;
        if (pe->r.lo == pe->r.hi) pe->r.hi += 1.0f;
    }
}
} } // namespace M_func_001FC408

namespace M_func_001FC4F8 { extern "C" {
struct S { char pad[0x3B8]; int fl; };
/** @sly2-annot addr=0x1FC4F8
 * Purpose: vt8: 2 if looping flag 4 else 1
 * Info: proposed name GetStreamsndKind (T2); class STREAMSND; TU sound
 * Confidence: low (draft) [T2 0.45] - evidence: static analysis; sources: r7_1F8DD8+verdicts_tu_r7r8
 * See: docs/engine/sound.md#6-key-functions, docs/reference/functions/sound.md#sound-0x1faf70-0x1ff3b8
 * @end-sly2-annot */
int func_001FC4F8(struct S *p) { return (p->fl & 4) ? 2 : 1; }
} } // namespace M_func_001FC4F8

namespace M_PlayStreamsnd { extern "C" {
struct K { int n0; int n4; int n8; char bC; char pad[0x110 - 0xD]; };
struct X { int n0; int n4; char pad[0x34 - 8]; int n34; int n38; int n3C; char pad2[0x4C - 0x40]; int n4C; };
struct VTE { short d; short p; void (*pfn)(void *); };
struct P { char pad[0xC]; struct VTE *pvt; char pad1[0x14 - 0x10]; int n14; char pad2[0x44 - 0x18]; float f44; char pad3[0x52 - 0x48]; short h52; char pad4[0x60 - 0x54]; struct X *p60; char pad5[0x3B8 - 0x64]; int n3B8; };
struct M { int n0; int n4; int n8; int nC; int n10; int n14; };
struct G { char pad[0x10]; int n10; };
extern struct G D_003DD040;
void InitFileKey(struct K *);
int func_001FD080(struct G *, struct X *);
void PauseSnd(struct P *);
void SetSndFlags(struct P *, int, int);
void func_001FCBA8(void);
void func_00233130(int, int, int, struct M *, void (*)(void), struct P *);
static inline int FSet(struct K *pk, struct X *px)
{
    if (!px) return 0;
    {
        int a = px->n34;
        int c = px->n0;
        int d = px->n3C;
        int b = px->n4;
        pk->n8 = 2;
        pk->n4 = b ^ d;
        pk->n0 = (c ^ a) + 0xCA0;
    }
    return pk->n4 != 0;
}
/** @sly2-annot addr=0x1FC510
 * Purpose: vt9: resolve stream file key, send start to SUKR IOP module with vol/pan
 * Info: proposed name PlayStreamsnd (T1); class STREAMSND; TU sound
 * Confidence: high [T1 0.85] - evidence: static analysis; sources: r7_1F8DD8+verdicts_tu_r7r8
 * See: docs/engine/sound.md#6-key-functions, docs/guides/port-plan.md#64-sound--coreaudio-k12,
 *   docs/reference/functions/sound.md#sound-0x1faf70-0x1ff3b8
 * @end-sly2-annot */
void PlayStreamsnd(struct P *p)
{
    struct K k;
    struct M m;
    int n;
    k.n8 = 2;
    InitFileKey(&k);
    n = func_001FD080(&D_003DD040, p->p60);
    if (FSet(&k, p->p60)) {
        if (k.n8 != 2) {
            SetSndFlags(p, 0x1FFF, 0);
            return;
        }
        m.n0 = k.n0;
        PauseSnd(p);
        m.n4 = (int)(p->f44 * 1024.0f);
        m.n8 = p->n14;
        m.nC = n;
        m.n10 = p->n3B8 | 1;
        {
            int fl = 0;
            if (D_003DD040.n10 && n == 2) fl = p->p60->n4C == 0;
            m.n14 = fl;
        }
        p->h52 = 1;
        p->pvt[1].pfn((char *)p + p->pvt[1].d);
        func_00233130(0x524B5553, 1, 0x18, &m, func_001FCBA8, p);
        SetSndFlags(p, 0x3F, 0);
    } else {
        SetSndFlags(p, 0x1FFF, 0);
    }
}
} } // namespace M_PlayStreamsnd

/** @sly2-annot addr=0x1FC698
 * Purpose: vt10: RPC 'SUKR'(0x524B5553) cmd 2 for stream id
 * Info: proposed name StopStreamsnd (T2); class STREAMSND; TU sound
 * Confidence: low (draft) [T2 0.40] - evidence: static analysis; sources: r7_1F8DD8
 * See: docs/reference/functions/sound.md#sound-0x1faf70-0x1ff3b8
 * @end-sly2-annot */
/* STREAMSND: RPC 'SUKR' (0x524B5553) command 2 (stop) for the stream. */
extern "C" void func_001FC698(SNDV *psnd)
{
    func_00233088(0x524B5553, 2, psnd->hvoice, 0, 0, 0, 0);
}

/** @sly2-annot addr=0x1FC6D0
 * Purpose: vt11: SUKR cmd 3
 * Info: proposed name PauseStreamsnd (T2); class STREAMSND; TU sound
 * Confidence: low (draft) [T2 0.40] - evidence: static analysis; sources: r7_1F8DD8
 * See: docs/reference/functions/sound.md#sound-0x1faf70-0x1ff3b8
 * @end-sly2-annot */
/* STREAMSND: RPC 'SUKR' (0x524B5553) command 3 (pause) for the stream. */
extern "C" void func_001FC6D0(SNDV *psnd)
{
    func_00233088(0x524B5553, 3, psnd->hvoice, 0, 0, 0, 0);
}

/** @sly2-annot addr=0x1FC708
 * Purpose: vt12: SUKR cmd 4
 * Info: proposed name ContinueStreamsnd (T2); class STREAMSND; TU sound
 * Confidence: low (draft) [T2 0.40] - evidence: static analysis; sources: r7_1F8DD8
 * See: docs/reference/functions/sound.md#sound-0x1faf70-0x1ff3b8
 * @end-sly2-annot */
/* STREAMSND: RPC 'SUKR' (0x524B5553) command 4 (continue) for the stream. */
extern "C" void func_001FC708(SNDV *psnd)
{
    func_00233088(0x524B5553, 4, psnd->hvoice, 0, 0, 0, 0);
}

namespace M_ApplyStreamsndParams { extern "C" {
struct P { char pad[0x14]; int n14; float f18; char pad2[0x40 - 0x1C]; int n40; float f44; char pad3[0x50 - 0x48]; unsigned short f50;
    char pad4[0x70 - 0x52]; float af70[6]; int a88[6]; char pad5[0x3BC - 0xA0]; int n3BC; };
void func_00233088(int, int, int, int, int, int, int);
/** @sly2-annot addr=0x1FC740
 * Purpose: vt13: SUKR cmd 5 vol/pan/pitch, then per-group params
 * Info: proposed name ApplyStreamsndParams (T1); class STREAMSND; TU sound
 * Confidence: mid (draft) [T1 0.50] - evidence: static analysis; sources: r7_1F8DD8
 * See: docs/reference/functions/sound.md#sound-0x1faf70-0x1ff3b8
 * @end-sly2-annot */
void ApplyStreamsndParams(struct P *p)
{
    int i;
    if (p->f50 & 0x70) {
        int fl = 0;
        int vol, pitch;
        if (p->f50 & 0x10) {
            fl = 1;
            vol = (int)(p->f44 * 1024.0f);
        }
        if (p->f50 & 0x20) fl |= 4;
        if (p->f50 & 0x40) {
            fl |= 8;
            pitch = (int)(p->f18 * 12.0f);
        }
        func_00233088(0x524B5553, 5, p->n40, fl, vol, p->n14, pitch);
    }
    if (p->f50 & 0x180) {
        for (i = 0; i < 6; i++) {
            int m = 1 << i;
            if (p->n3BC & m)
                func_00233088(0x524B5553, 6, p->n40, i, (int)(p->af70[i] * 1024.0f), p->a88[i], 0);
        }
        p->n3BC = 0;
    }
}
} } // namespace M_ApplyStreamsndParams

namespace M_func_001FC878 { extern "C" {
struct VTE { short d; short pad; void (*pfn)(void *); };
struct P { char pad[0xC]; struct VTE *pvt; char pad2[0x40 - 0x10]; int n40; char pad3[0x4E - 0x44]; short n4E; unsigned short f50; unsigned short n52; char pad4[0x68 - 0x54]; long l68; };
struct G { char pad[0x1F0]; long l1F0; long l1F8; };
extern struct G D_003DD040;
void OnStreamsndStatus(void);
void func_001FBB30(void);
void func_002330D0(int, int, int, int, int, int, int, void (*)(void), long);
void SetSndFlags(struct P *, int, int);
/** @sly2-annot addr=0x1FC878
 * Purpose: vt14: async SUKR cmd 7/8 status queries with callbacks 0x1FCC30/0x1FBB30; accumulate paused time
 * Info: proposed name STREAMSND_QueryStatus (T2); class STREAMSND; TU sound
 * Confidence: low (draft) [T2 0.45] - evidence: static analysis; sources: r7_1F8DD8
 * See: docs/reference/functions/sound.md#sound-0x1faf70-0x1ff3b8
 * @end-sly2-annot */
void func_001FC878(struct P *p)
{
    if (p->f50 & 0x1000) {
        p->pvt[1].pfn((char *)p + p->pvt[1].d);
        func_002330D0(0x524B5553, 7, p->n40, 0, 0, 0, 0, OnStreamsndStatus, (long)p);
        SetSndFlags(p, 0x1000, 0);
    }
    if (p->n52) return;
    if (p->f50 & 0x400) {
        p->pvt[1].pfn((char *)p + p->pvt[1].d);
        func_002330D0(0x524B5553, 8, p->n40, 0, 0, 0, 0, func_001FBB30, (long)p);
        SetSndFlags(p, 0x400, 0);
    }
    if ((p->f50 & 0x800) && p->n4E <= 0)
        p->l68 += D_003DD040.l1F0 - D_003DD040.l1F8;
}
} } // namespace M_func_001FC878

/** @sly2-annot addr=0x1FC9A8
 * Purpose: vt15: no-op (streams are not culled)
 * Info: proposed name SetStreamsndCulled (T2); class STREAMSND; TU sound
 * Confidence: low (draft) [T2 0.40] - evidence: static analysis; sources: r7_1F8DD8
 * See: docs/reference/functions/sound.md#sound-0x1faf70-0x1ff3b8
 * @end-sly2-annot */
/* STREAMSND slot 15: streams are not culled */
extern "C" void func_001FC9A8()
{
}

namespace M_UpdateStreamsndPositional { extern "C" {
typedef int qword __attribute__((mode(TI)));
struct LO {char pad28[0x28];unsigned long flags;};
struct RANGE {float low,high;};
struct E {struct LO *object;struct RANGE limits;float gain;};
struct P {char pad1C[0x1C];struct LO *object;qword position;char pad30[0x70];int counts[6];struct E entries[6][8];};
extern qword D_002A2DD0;
int FFloatsNear__Ffff(float,float,float);
void UpdateSndPositional(struct P *);
qword GetLoWorldPos(struct LO *);
void CalculateSndVolPan(struct P *,qword,const struct RANGE *,float *,int *);
void func_001FC380(struct P *,int,int);
void SetStreamsndGroupVolume(struct P *,int,float);
static inline float X(qword q) { float f; __asm__("mtc1 %1, %0" : "=f"(f) : "r"(q)); return f; }
static inline float Y(qword q) { float f; __asm__("prot3w %0, %1" : "=r"(f) : "r"(q)); return f; }
static inline float Z(qword q) { float f; __asm__("pextuw %0, $0, %1" : "=r"(f) : "r"(q)); return f; }
static inline int FNear(qword a, qword b)
{
    return FFloatsNear__Ffff(X(a), X(b), 0.0001f) && FFloatsNear__Ffff(Y(a), Y(b), 0.0001f) && FFloatsNear__Ffff(Z(a), Z(b), 0.0001f);
}
/** @sly2-annot addr=0x1FC9B0
 * Purpose: vt16: base positional update, then per sub-group: loudest visible source sets group vol/pan
 * Info: proposed name UpdateStreamsndPositional (T1); class STREAMSND; TU sound
 * Confidence: high [T1 0.85] - evidence: static analysis; sources: r7_1F8DD8+verdicts_tu_r7r8
 * See: docs/engine/sound.md#6-key-functions, docs/reference/functions/sound.md#sound-0x1faf70-0x1ff3b8
 * @end-sly2-annot */
void UpdateStreamsndPositional(struct P *p)
{
    int group;
    if (p->object || !FNear(D_002A2DD0, p->position))
        UpdateSndPositional(p);
    for (group = 0; group < 6; group++) {
        int count = p->counts[group];
        if (count) {
            float maximum = 0.0f;
            int flags = 0, active = 0, i;
            int *pflags = &flags;
            float fade;
            for (i = 0; i < count; i++) {
                struct E *entry = &p->entries[group][i];
                struct LO *object = entry->object;
                if (object && ((object->flags >> 60) & 1) == 1) {
                    active++;
                    CalculateSndVolPan(p, GetLoWorldPos(object), &entry->limits, &fade, pflags);
                    pflags = 0;
                    { float gain = fade * entry->gain; maximum = maximum > gain ? maximum : gain; }
                }
            }
            func_001FC380(p, group, active == 1 ? flags : 0);
            SetStreamsndGroupVolume(p, group, maximum);
        }
    }
}
} } // namespace M_UpdateStreamsndPositional

namespace M_func_001FCBA8 { extern "C" {
struct VTE { short d; short p; void (*pfn)(void *); };
struct VT { struct VTE a[8]; };
struct X { char pad[0xC]; struct VT *pvt; char pad2[0x40 - 0x10]; int f40; char pad3[0x50 - 0x44]; unsigned short fl; short s52; };
void ResumeSnd(struct X *);
void SetSndFlags(struct X *, int, int);
#define VF(q, n) (q)->pvt->a[n].pfn((char *)(q) + (q)->pvt->a[n].d)
/** @sly2-annot addr=0x1FCBA8
 * Purpose: SUKR start callback: store stream handle or stop on failure; Decref
 * Info: proposed name OnStreamsndStarted (T2); TU sound
 * Confidence: low (draft) [T2 0.40] - evidence: static analysis; sources: r7_1F8DD8
 * See: docs/reference/functions/sound.md#sound-0x1faf70-0x1ff3b8
 * @end-sly2-annot */
void func_001FCBA8(int f, long l)
{
    struct X *px = (struct X *)l;
    px->f40 = f;
    if (f) SetSndFlags(px, 0x1000, 0x1000);
    else {
        ResumeSnd(px);
        px->s52 = 0;
        if (!(px->fl & 8)) VF(px, 7);
    }
    VF(px, 2);
}
} } // namespace M_func_001FCBA8
#undef VF

namespace M_OnStreamsndStatus { extern "C" {
struct VT { int pad[4]; short d; short p; void (*pfn)(void *); };
struct X { char pad[0xC]; struct VT *pvt; char pad2[0x52 - 0x10]; short s52; };
void ResumeSnd(struct X *);
void SetSndFlags(struct X *, int, int);
/** @sly2-annot addr=0x1FCC30
 * Purpose: SUKR status callback (cmd 7)
 * Info: proposed name OnStreamsndStatus (T1); TU sound
 * Confidence: high [T1 0.85] - evidence: static analysis; sources: r7_1F8DD8+verdicts_tu_r7r8
 * See: docs/engine/sound.md#6-key-functions, docs/guides/port-plan.md#64-sound--coreaudio-k12,
 *   docs/reference/functions/sound.md#sound-0x1faf70-0x1ff3b8
 * @end-sly2-annot */
void OnStreamsndStatus(int f, long l)
{
    struct X *px = (struct X *)l;
    if (f) { px->s52 = 0; ResumeSnd(px); }
    else SetSndFlags(px, 0x1000, 0x1000);
    px->pvt->pfn((char *)px + px->pvt->d);
}
} } // namespace M_OnStreamsndStatus

/** @sly2-annot addr=0x1FCC90
 * Purpose: vt1: ++cref (u16 +0x4C)
 * Info: proposed name IncrefStreamsnd (T1); class STREAMSND; TU sound
 * Confidence: mid (draft) [T1 0.70] - evidence: static analysis; sources: r7_1F8DD8
 * See: docs/reference/functions/sound.md#sound-0x1faf70-0x1ff3b8
 * @end-sly2-annot */
void STREAMSND::VUnk00()   /* key function: STREAMSND's vtable comes out here */
{
    SNDV *psnd = (SNDV *)this;
    psnd->cref++;
}

namespace M_DecrefStreamsnd { extern "C" {
struct S { char pad[0x4C]; short c; };
void func_00258348(struct S *, int);
void FreeStreamsnd(struct S *);
/** @sly2-annot addr=0x1FCCA0
 * Purpose: vt2: --cref==0 -> dtor + free to 0x3C0 pool
 * Info: proposed name DecrefStreamsnd (T1); class STREAMSND; TU sound
 * Confidence: mid (draft) [T1 0.70] - evidence: static analysis; sources: r7_1F8DD8
 * See: docs/reference/functions/sound.md#sound-0x1faf70-0x1ff3b8
 * @end-sly2-annot */
void DecrefStreamsnd(struct S *p) { if (--p->c == 0 && p) { func_00258348(p, 0); FreeStreamsnd(p); } }
} } // namespace M_DecrefStreamsnd

/** @sly2-annot addr=0x1FCCF0
 * Purpose: Allocate from SNDMGR 0x3C0 pool (+0x34)
 * Info: proposed name PvAllocStreamsnd (T1); TU sound
 * Confidence: high [T1 0.85] - evidence: static analysis; sources: r7_1F8DD8+verdicts_tu_r7r8
 * See: docs/engine/sound.md#6-key-functions, docs/reference/functions/sound.md#sound-0x1faf70-0x1ff3b8
 * @end-sly2-annot */
extern "C" void *PvAllocStreamsnd(int cb)
{
    return PvAllocSlotheapClear(&G_SNDMGR.slotheapStreamsnd);
}

/** @sly2-annot addr=0x1FCD10
 * Purpose: Free to SNDMGR 0x3C0 pool
 * Info: proposed name FreeStreamsnd (T1); TU sound
 * Confidence: mid (draft) [T1 0.50] - evidence: static analysis; sources: r7_1F8DD8
 * See: docs/reference/functions/sound.md#sound-0x1faf70-0x1ff3b8
 * @end-sly2-annot */
extern "C" void FreeStreamsnd(void *pv)
{
    func_0011CD98(&G_SNDMGR.slotheapStreamsnd, pv);
}

namespace M_SNDMGR_ctor { extern "C" {
struct T { float a; float b; float c; };
struct V2 { float x, y; };
struct P {
    int n0; int n4; int pad8; void *pvt; int n10; char pad14[0x1C - 0x14]; char x1C[0x18]; char x34[0x18]; char x4C[0xC];
    int n58; char pad5C[0x68 - 0x5C]; int n68; char pad6C[0x80 - 0x6C]; char x80[0x10]; char x90[0x10];
    char padA0[0x104 - 0xA0]; int n104; char pad108[0x114 - 0x108]; struct T t114; struct T t120; struct T t12C;
    char pad138[0x200 - 0x138]; int n200; int pad204; int a208[8]; int n228;
};
extern char D_002CA510[] __asm__("_vt$6CID152");
extern const struct V2 D_002A83B8;
extern const struct V2 D_002A83C0;
void func_0011CCA8(void *, int, int, int);
void InitDl__FP2DLi(void *, int);
void FitClq__FffffP3CLQ(float, float, float, void *, float);
static inline void InitT(struct T *pt, float a, float c)
{
    pt->a = a;
    pt->b = 0.0f;
    pt->c = c;
}
/** @sly2-annot addr=0x1FCD38
 * Purpose: Sound-manager ctor (global g_sndmgr D_003DD040): pools 0x70x128 and 0x3C0x16, pending DL, music
 *   stack, volume curve fit
 * Info: proposed name SNDMGR_ctor (T1); class SNDMGR; TU sound
 * Confidence: mid (draft) [T1 0.60] - evidence: static analysis; sources: r7_1F8DD8
 * See: docs/engine/sound.md#1-sound-manager-soundcpp-0x1faf70-0x1ff398,
 *   docs/reference/functions/sound.md#sound-0x1faf70-0x1ff3b8
 * @end-sly2-annot */
struct P *SNDMGR_ctor(struct P *p)
{
    int n;
    int *q;
    p->n0 = 0;
    p->n4 = 0;
    p->pvt = D_002CA510;
    func_0011CCA8(p->x1C, 0x70, 0x80, 0x10);
    func_0011CCA8(p->x34, 0x3C0, 0x10, 0x10);
    p->n200 = 0;
    q = p->a208;
    for (n = 7; n != -1; n--) *q++ = 0;
    p->n228 = 0;
    InitDl__FP2DLi(p->x4C, 0x38);
    p->n68 = 3;
    p->n10 = 0;
    p->n104 = 0;
    InitT(&p->t12C, 1.0f, 0.25f);
    InitT(&p->t120, 1.0f, 0.25f);
    {
        struct T *pt = &p->t114;
        float b;
        float h = 0.5f;
        InitT(pt, 3.0f, 0.25f);
        b = pt->b;
        FitClq__FffffP3CLQ(b, D_002A83B8.y, h, p->x80, D_002A83B8.x);
        FitClq__FffffP3CLQ(b, D_002A83C0.y, h, p->x90, D_002A83C0.x);
    }
    p->n58 = 0;
    return p;
}
} } // namespace M_SNDMGR_ctor

namespace M_StartupSound { extern "C" {
struct G { char pad[0x13C]; float a[8][4]; float g1BC; float b[4]; char pad2[0x1F0 - 0x1D0]; long l1F0; long l1F8; };
extern struct G D_003DD040;
struct CLOCK;
extern struct CLOCK D_002DDED0;
void snd_StartSoundSystem(int);
int func_00233048(int, int, int, int, int, int, int);
long TickNow(struct CLOCK *);
/** @sly2-annot addr=0x1FCEA0
 * Purpose: snd_StartSoundSystem, SUKR init (cmd 0), all channel/category volumes = 1, record start clock
 * Info: proposed name StartupSound (T1); TU sound
 * Confidence: mid (draft) [T1 0.70] - evidence: static analysis; sources: r7_1F8DD8
 * See: docs/engine/main-loop-startup.md#open-questions, docs/engine/overview.md#1-boot,
 *   docs/reference/functions/sound.md#sound-0x1faf70-0x1ff3b8
 * @end-sly2-annot */
void StartupSound(void)
{
    int i, j;
    long t;
    snd_StartSoundSystem(2);
    (void)func_00233048(0x524B5553, 0, 0, 0, 0, 0, 0);
    D_003DD040.g1BC = 1.0f;
    for (i = 0; i < 4; i++) {
        D_003DD040.b[i] = 1.0f;
        for (j = 0; j < 8; j++) D_003DD040.a[j][i] = 1.0f;
    }
    t = TickNow(&D_002DDED0);
    D_003DD040.l1F0 = t;
    D_003DD040.l1F8 = t;
}
} } // namespace M_StartupSound

namespace M_func_001FCF70 { extern "C" {
struct S { char pad[0x6C]; int a; int b; };
/** @sly2-annot addr=0x1FCF70
 * Purpose: Async callback: store result (+b), clear busy
 * Info: proposed name OnSndmgrQueryB (T2); TU sound
 * Confidence: low (draft) [T2 0.30] - evidence: static analysis; sources: r7_1F8DD8
 * See: docs/reference/functions/sound.md#sound-0x1faf70-0x1ff3b8
 * @end-sly2-annot */
void func_001FCF70(int x, long l) { struct S *p = (struct S *)(int)l; p->b = x; p->a = 0; }
} } // namespace M_func_001FCF70

namespace M_func_001FCF88 { extern "C" {
struct S { char pad[0xA0]; int a; int b; };
/** @sly2-annot addr=0x1FCF88
 * Purpose: Async callback for SUKR cmd 0xC (0x1FDBA8)
 * Info: proposed name OnSndmgrQueryC (T2); TU sound
 * Confidence: low (draft) [T2 0.30] - evidence: static analysis; sources: r7_1F8DD8
 * See: docs/reference/functions/sound.md#sound-0x1faf70-0x1ff3b8
 * @end-sly2-annot */
void func_001FCF88(int x, long l) { struct S *p = (struct S *)(int)l; p->b = x; p->a = 0; }
} } // namespace M_func_001FCF88

namespace M_func_001FCFA0 { extern "C" {
struct T { char pad[0x40]; int n; struct T *pnext; };
struct S { char pad[0xEC]; struct T *pt; };
/** @sly2-annot addr=0x1FCFA0
 * Purpose: Find SNDSTREAM by id in one stream list
 * Info: proposed name PsndstreamFindDialog (T2); TU sound
 * Confidence: low (draft) [T2 0.40] - evidence: static analysis; sources: r7_1F8DD8
 * See: docs/reference/functions/sound.md#sound-0x1faf70-0x1ff3b8
 * @end-sly2-annot */
struct T *func_001FCFA0(struct S *p, int n) { struct T *pt; for (pt = p->pt; pt; pt = pt->pnext) if (pt->n == n) return pt; return 0; }
} } // namespace M_func_001FCFA0

/** @sly2-annot addr=0x1FCFD0
 * Purpose: Float field +0x50 of a stream record found by id, else 0
 * Info: proposed name GDialogStreamParam (T2); TU sound
 * Confidence: low (draft) [T2 0.30] - evidence: static analysis; sources: r7_1F8DD8
 * See: docs/reference/functions/sound.md#sound-0x1faf70-0x1ff3b8
 * @end-sly2-annot */
extern "C" float func_001FCFD0(int id)
{
    STREAMREC *prec = func_001FCFA0(id);
    float g = 0.0f;
    if (prec != 0) {
        g = prec->g50;
    }
    return g;
}

namespace M_func_001FCFF8 { extern "C" {
struct T { char pad[0x40]; int n; struct T *pnext; };
struct S { char pad[0xF0]; struct T *pt; };
/** @sly2-annot addr=0x1FCFF8
 * Purpose: Find SNDSTREAM by id (music list)
 * Info: proposed name PsndstreamFindMusic (T2); TU sound
 * Confidence: low (draft) [T2 0.40] - evidence: static analysis; sources: r7_1F8DD8
 * See: docs/reference/functions/sound.md#sound-0x1faf70-0x1ff3b8
 * @end-sly2-annot */
struct T *func_001FCFF8(struct S *p, int n) { struct T *pt; for (pt = p->pt; pt; pt = pt->pnext) if (pt->n == n) return pt; return 0; }
} } // namespace M_func_001FCFF8

namespace M_PsndFromSfxid { extern "C" {
struct S { char pad[0x18]; int c; char pad2[0xE0]; int *a; int *ai; };
/** @sly2-annot addr=0x1FD028
 * Purpose: sfxid -> SNDMGR sound entry via aisnd index table (NULL if unmapped)
 * Info: proposed name PsndFromSfxid (T1); TU sound
 * Confidence: mid (draft) [T1 0.50] - evidence: static analysis; sources: r7_1F8DD8
 * See: docs/reference/functions/sound.md#sound-0x1faf70-0x1ff3b8
 * @end-sly2-annot */
int *PsndFromSfxid(struct S *p, int n)
{
    int i;
    if (n < 0 || n >= p->c) return 0;
    i = p->ai[n];
    if (i < 0) return 0;
    return &p->a[i];
}
} } // namespace M_PsndFromSfxid

/** @sly2-annot addr=0x1FD070
 * Purpose: Index of a sound entry (ptr - base)/4
 * Info: proposed name IsndFromPsnd (T1); TU sound
 * Confidence: mid (draft) [T1 0.50] - evidence: static analysis; sources: r7_1F8DD8
 * See: docs/reference/functions/sound.md#sound-0x1faf70-0x1ff3b8
 * @end-sly2-annot */
extern "C" int IsndFromPsnd(SNDMGRV *psndmgr, SNDV **ppsnd)
{
    return (int)((char *)ppsnd - (char *)psndmgr->apsnd) >> 2;
}

namespace M_func_001FD080 { extern "C" {
struct T { char pad[0x44]; struct T *pnext; };
struct S { char pad[0xF0]; struct T *pt; struct T *pt2; };
/** @sly2-annot addr=0x1FD080
 * Purpose: 1 if record is in list A, 0 if it is the ambience record, else 2: picks volume channel
 * Info: proposed name ChannelFromPsndstream (T2); TU sound
 * Confidence: low (draft) [T2 0.30] - evidence: static analysis; sources: r7_1F8DD8
 * See: docs/reference/functions/sound.md#sound-0x1faf70-0x1ff3b8
 * @end-sly2-annot */
int func_001FD080(struct S *p, struct T *x) { struct T *pt; for (pt = p->pt; pt; pt = pt->pnext) if (pt == x) return 1; return p->pt2 != x ? 2 : 0; }
} } // namespace M_func_001FD080

namespace M_func_001FD0C0 { extern "C" {
extern int D_002C3C40[];
/** @sly2-annot addr=0x1FD0C0
 * Purpose: Table D_002C3C40[n] for n<6 (else -1): SM oid for dialog speaker role n
 * Info: proposed name OidDialogSmRole (T2); TU sound
 * Confidence: low (draft) [T2 0.30] - evidence: static analysis; sources: r7_1F8DD8+verdicts_tu_r7r8
 * See: docs/engine/sound.md#6-key-functions, docs/reference/functions/guards-ai.md#step-0x203948-0x205e70,
 *   docs/reference/functions/sound.md#sound-0x1faf70-0x1ff3b8
 * @end-sly2-annot */
int func_001FD0C0(int a, unsigned int n) { if (n >= 6) return -1; return D_002C3C40[n]; }
} } // namespace M_func_001FD0C0

namespace M_LoadSoundSectionFromBrx {
// LoadSoundSectionFromBrx: LoadSoundSectionFromBrx (0x1FD0E8), written from the sound section's layout (our names; Lilly's decomp has it as unmatched).
// Reads the level's sound section: the sound bank's file record (the loaded bank is kept when the record's file id is unchanged), per-sound kinds (S8),
// the effect id -> sound index table, the ambience stream record, six ids, then the dialogue/music stream records (one file record per language;
// the record of the language in g_nLang = D_002E1E54 is kept) and the loose file records. The allocation requests are {cb, cbAlign} records built by
// inline helpers; every helper call has its own SWALLOC local, which the compiler gives the same stack slot (sp+0x40).
// Matched without pins or asm: the loops use `int *pn = &anKind[i]` (pointer formed before the read), a separate counter j for the index table,
// memset declared as returning void * (a void return moved a v0/v1 choice), and the ambience stream through a temp (no s-register held for &ambience).
typedef unsigned char    uchar;
typedef unsigned short   ushort;
typedef unsigned int     uint;
typedef uchar byte;
typedef unsigned long ulong;
struct SNDMGR;
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
    void Read(int cb, void *pv) __asm__("Read__18CBinaryInputStreamiPv");
    uchar U8Read() __asm__("U8Read__18CBinaryInputStream");
    char S8Read() __asm__("S8Read__18CBinaryInputStream");
    short S16Read() __asm__("S16Read__18CBinaryInputStream");
    int S32Read() __asm__("S32Read__18CBinaryInputStream");
    float F32Read() __asm__("F32Read__18CBinaryInputStream");
    void ReadFileRecord(void *pv) __asm__("CBinaryInputStream_ReadCb64");
};
struct FILEREC
{
    uint aw[16];
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
struct STREAMSND;
struct SNDSTREAM
{
    FILEREC rec;
    int id;
    SNDSTREAM *psndstreamNext;
    int nUnk48;
    int nUnk4C;
    float gUnk50;
    int cpair;
    ulong *apair;
};
struct SNDFILE
{
    FILEREC rec;
    int id;
    SNDFILE *psndfileNext;
};
struct SNDMGR
{
    byte ab[0x18];
    int csfx;
    byte abUnk1c[0x8c];
    FILEREC rec;
    void *pbank;
    SNDSTREAM *psndstreamFirst;
    SNDFILE *psndfileFirst;
    FILEREC *precAmbience;
    int csnd;
    int *anKind;
    int *aisnd;
    byte abUnk104[0xCC];
    int cUnk1D0;
    int anUnk1D4[6];
    byte abUnk1EC[0x3C];
    STREAMSND *psndAmbience;
};
extern "C" { extern int D_002E1E54; }
extern "C" void snd_UnloadBank(void *);
extern "C" int snd_FlushSoundCommands(void);
extern "C" void *func_002333B4(void *, int, uint) __asm__("memset");
extern "C" STREAMSND *PstreamsndNew(FILEREC *, int);
extern "C" void func_00259B48(void *, STREAMSND *);
inline int FSameFileId(FILEREC *prec, FILEREC *precOther)
{
    if (prec == precOther)
        return 1;
    if (prec == 0 || precOther == 0)
        return 0;
    return (prec->aw[2] ^ prec->aw[14]) == (precOther->aw[2] ^ precOther->aw[14]);
}
extern "C" void LoadSoundSectionFromBrx(SNDMGR *psndmgr, CBinaryInputStream *pbis) __asm__("LoadSoundSectionFromBrx");
/** @sly2-annot addr=0x1FD0E8
 * Purpose: Read level sound section: bank file record, per-sound kinds, sfxid->index table, ambience stream,
 *   dialogue/music stream records per language, loose file records
 * Info: proposed name LoadSoundSectionFromBrx (T1); TU sound
 * Notes: audio format doc cites this function by this name (CONFIRMED(code) in the doc)
 * Confidence: high [T1 0.90] - evidence: static analysis; sources:
 *   r7_1F8DD8+verdicts_tu_r7r8+format-doc:audio
 * See: docs/engine/sound.md#1-sound-manager-soundcpp-0x1faf70-0x1ff398, docs/engine/sound.md#6-key-functions,
 *   docs/engine/world-sw.md#loadswfrombrx-0x13da18-matched-hand-file
 * @end-sly2-annot */
extern "C" void LoadSoundSectionFromBrx(SNDMGR *psndmgr, CBinaryInputStream *pbis)
{
    FILEREC rec;
    pbis->ReadFileRecord(&rec);
    if (!FSameFileId(&rec, &psndmgr->rec))
    {
        psndmgr->rec = rec;
        if (psndmgr->pbank)
        {
            snd_UnloadBank(psndmgr->pbank);
            while (snd_FlushSoundCommands())
                ;
            psndmgr->pbank = 0;
        }
    }
    int i;
    psndmgr->csnd = pbis->S16Read();
    psndmgr->anKind = (int *)PvAllocSwClearLoad(psndmgr->csnd * sizeof(int), 0x10);
    for (i = 0; i < psndmgr->csnd; i++)
    {
        int *pn = &psndmgr->anKind[i];
        *pn = pbis->S8Read();
    }
    psndmgr->csfx = pbis->S16Read();
    psndmgr->aisnd = (int *)PvAllocSwLoad(psndmgr->csfx * sizeof(int), 0x10);
    func_002333B4(psndmgr->aisnd, -1, psndmgr->csfx * sizeof(int));
    int j;
    for (j = 0; j < psndmgr->csnd; j++)
        psndmgr->aisnd[pbis->S16Read()] = j;
    psndmgr->cUnk1D0 = pbis->S16Read();
    if (psndmgr->cUnk1D0 > 0)
    {
        psndmgr->precAmbience = (FILEREC *)PvAllocSwClearLoad(sizeof(FILEREC), 0x10);
        pbis->ReadFileRecord(psndmgr->precAmbience);
        STREAMSND *psnd = PstreamsndNew(psndmgr->precAmbience, 4);
        func_00259B48(&psndmgr->psndAmbience, psnd);
    }
    func_002333B4(psndmgr->anUnk1D4, -1, sizeof(psndmgr->anUnk1D4));
    for (i = 0; i < psndmgr->cUnk1D0; i++)
        psndmgr->anUnk1D4[i] = pbis->S16Read();
    int clang = pbis->U8Read();
    int ilangMatch = 0;
    for (int ilang = 0; ilang < clang; ilang++)
    {
        if (pbis->U8Read() == D_002E1E54)
            ilangMatch = ilang;
    }
    int cstream = pbis->S32Read();
    SNDSTREAM *astream = (SNDSTREAM *)PvAllocSwClearLoad(cstream * sizeof(SNDSTREAM), 0x10);
    for (i = 0; i < cstream; i++)
    {
        SNDSTREAM *pstream = &astream[i];
        pstream->id = pbis->S16Read();
        pstream->nUnk48 = pbis->S16Read();
        for (int ilang = 0; ilang < clang; ilang++)
        {
            FILEREC recLang;
            pbis->ReadFileRecord(&recLang);
            int nUnk4C = pbis->S8Read();
            float gUnk50 = pbis->F32Read();
            if (ilang == ilangMatch)
            {
                pstream->rec = recLang;
                pstream->nUnk4C = nUnk4C;
                pstream->gUnk50 = gUnk50;
            }
        }
        pstream->cpair = pbis->S32Read();
        pstream->apair = (ulong *)PvAllocSwLoad(pstream->cpair * 8, 0x10);
        pbis->Read(pstream->cpair * 8, pstream->apair);
        pstream->psndstreamNext = psndmgr->psndstreamFirst;
        psndmgr->psndstreamFirst = pstream;
    }
    int cfile = pbis->S32Read();
    SNDFILE *afile = (SNDFILE *)PvAllocSwClearLoad(cfile * sizeof(SNDFILE), 0x10);
    for (i = 0; i < cfile; i++)
    {
        SNDFILE *pfile = &afile[i];
        pfile->id = pbis->S16Read();
        pbis->ReadFileRecord(pfile);
        pfile->psndfileNext = psndmgr->psndfileFirst;
        psndmgr->psndfileFirst = pfile;
    }
}
} // namespace M_LoadSoundSectionFromBrx

namespace M_PostSoundLoad { extern "C" {
struct K { int n0; int n4; int n8; char bC; char pad[0x110 - 0xD]; };
struct X { int n0; int n4; char pad[0x34 - 8]; int n34; int n38; int n3C; };
struct P { char pad[0xA8]; struct X xA8; char pad2[0xE8 - 0xA8 - sizeof(struct X)]; int pE8; char pad3[0xF8 - 0xEC]; int nF8; int *aFC; };
void InitFileKey(struct K *);
int snd_BankLoadByLoc(int, int);
void snd_ResolveBankXREFS(void);
int snd_IsSoundALooper(int, int);
static inline int FSet(struct K *pk, struct X *px)
{
    if (!px) return 0;
    {
        int a = px->n34;
        int c = px->n0;
        int d = px->n3C;
        int b = px->n4;
        pk->n8 = 2;
        pk->n4 = b ^ d;
        pk->n0 = (c ^ a) + 0xCA0;
    }
    return pk->n4 != 0;
}
/** @sly2-annot addr=0x1FD5E8
 * Purpose: Load bank if needed, snd_ResolveBankXREFS, mark looping sounds (kind 2) via snd_IsSoundALooper
 * Info: proposed name PostSoundLoad (T1); TU sound
 * Confidence: mid (draft) [T1 0.50] - evidence: static analysis; sources: r7_1F8DD8
 * See: docs/open-questions.md#modding-live-test-2026-11-01,
 *   docs/reference/functions/sound.md#sound-0x1faf70-0x1ff3b8
 * @end-sly2-annot */
void PostSoundLoad(struct P *p)
{
    int i;
    if (!p->pE8) {
        struct K k;
        k.n8 = 2;
        InitFileKey(&k);
        if (FSet(&k, &p->xA8) && k.n8 == 2) p->pE8 = snd_BankLoadByLoc(k.n0, 0);
        snd_ResolveBankXREFS();
    }
    for (i = 0; i < p->nF8; i++) {
        if (p->aFC[i] == 2) continue;
        if (snd_IsSoundALooper(p->pE8, i)) p->aFC[i] = 2;
    }
}
} } // namespace M_PostSoundLoad

namespace M_ResetSndmgr { extern "C" {
typedef int qword __attribute__((mode(TI)));
struct P {
    char pad0[0x14]; int n14; int n18; char s1C[0x18]; char s34[0x18]; char s4C[0x10]; int n5C; int n60; int pad64; int n68;
    int pad6C[2]; char s74[0xEC - 0x74]; int aEC[6]; int pad104; char s108[0xC];
    char pad114[0x138 - 0x114]; int n138; char pad13C[0x1D0 - 0x13C]; int n1D0; char pad1D4[0x200 - 0x1D4];
    int r200; int n204; char s208[0x20]; int r228; char s22C[0x18];
};
extern qword D_002A2DD0;
extern char D_004435F0[];
void snd_StopAllStreams(void);
void func_00233048(int, int, int, int, int, int, int);
int snd_FlushSoundCommands(void);
void func_00156BE0(int, qword, int, void *);
void *func_002333B4(void *, int, int) __asm__("memset");
void func_00227788(void *);
void func_00259B48(void *, void *);
void ResetSlotheap(void *);
void ClearDl__FP2DL(void *);
void ResetSndmgrVolumes(struct P *);
/** @sly2-annot addr=0x1FD700
 * Purpose: Stop all sounds/streams (989 cmd 0x18, SUKR cmd 10), flush, clear state, free dirty list, reset
 *   volumes
 * Info: proposed name ResetSndmgr (T1); TU sound
 * Confidence: mid (draft) [T1 0.55] - evidence: static analysis; sources: r7_1F8DD8
 * See: docs/reference/functions/sound.md#sound-0x1faf70-0x1ff3b8
 * @end-sly2-annot */
void ResetSndmgr(struct P *p)
{
    snd_StopAllStreams();
    func_00233048(0x524B5553, 10, 0, 0, 0, 0, 0);
    while (snd_FlushSoundCommands())
        ;
    p->n14 = 0;
    p->n18 = -1;
    p->n1D0 = 0;
    p->n5C = 0;
    p->n60 = 0;
    p->n138 = 0;
    p->aEC[0] = 0;
    p->aEC[1] = 0;
    p->aEC[2] = 0;
    p->aEC[3] = 0;
    p->aEC[4] = 0;
    p->aEC[5] = 0;
    func_00156BE0(0, D_002A2DD0, 0, p->s74);
    func_002333B4(p->s108, 0, 0xC);
    func_00227788(D_004435F0);
    func_00259B48(&p->r200, 0);
    p->n204 = 0;
    func_002333B4(p->s208, 0, 0x20);
    func_00259B48(&p->r228, 0);
    func_002333B4(p->s22C, 0, 0x18);
    p->n68 = 3;
    ResetSlotheap(p->s1C);
    ResetSlotheap(p->s34);
    ClearDl__FP2DL(p->s4C);
    ResetSndmgrVolumes(p);
}
} } // namespace M_ResetSndmgr

namespace M_UpdateSndmgr { extern "C" {
struct X;
struct Q { char pad[0x38]; struct Q *pNext; };
struct K { int a, b, c; };
struct P { char pad[0x4C]; struct Q *pq; char pad1[0x58 - 0x50]; int n58; char pad2[0x15C - 0x5C]; float f15C; char pad3[0x1F0 - 0x160];
    long l1F0; long l1F8; struct X *r200; int c; struct X *a[8]; struct X *r228; };
struct CLOCK { float t; float dt; char pad[0x40 - 8]; float f40; };
extern struct CLOCK D_002DDED0;
extern int D_002BD4C0;
extern struct K D_002A8288;
long TickNow(struct CLOCK *);
void func_001FDA48(struct P *);
void func_001FDBA8(struct P *);
void ResumeSnd(struct X *);
void PauseSnd(struct X *);
void func_001FDC08(struct P *);
void func_001FDEC0(struct P *);
void func_001FDEC8(struct P *);
int func_001FE5E0(struct P *);
float GSmooth__FfffP3SMPPf(float, float, float, struct K *, int);
void SetSndmgrChannelVolume(struct P *, int, float, int);
void UpdateSnd(struct Q *);
int snd_FlushSoundCommands(void);
/** @sly2-annot addr=0x1FD830
 * Purpose: Per-frame: clock, IOP status polls, global pause on game state 3, music mix, dialog lip-sync,
 *   master fades, UpdateSnd on every pending sound, flush 989 commands
 * Info: proposed name UpdateSndmgr (T1); TU sound
 * Confidence: mid (draft) [T1 0.75] - evidence: static analysis; sources: r7_1F8DD8
 * See: docs/engine/overview.md#5-per-frame-update-and-render,
 *   docs/engine/sound.md#1-sound-manager-soundcpp-0x1faf70-0x1ff398, docs/engine/sound.md#6-key-functions
 * @end-sly2-annot */
void UpdateSndmgr(struct P *p)
{
    struct X *top;
    float f, g;
    struct K k;
    struct Q *q;
    p->l1F8 = p->l1F0;
    p->l1F0 = TickNow(&D_002DDED0);
    func_001FDA48(p);
    func_001FDBA8(p);
    top = p->c > 0 ? *(p->a + p->c - 1) : 0;
    if (D_002BD4C0 == 3) {
        if (p->n58) {
            if (top) ResumeSnd(top);
            if (p->r228) ResumeSnd(p->r228);
            if (p->r200) ResumeSnd(p->r200);
            p->n58 = 0;
        }
    } else if (!p->n58) {
        if (top) PauseSnd(top);
        if (p->r228) PauseSnd(p->r228);
        if (p->r200) PauseSnd(p->r200);
        p->n58 = 1;
    }
    g = 1.0f;
    func_001FDC08(p);
    func_001FDEC0(p);
    func_001FDEC8(p);
    k = D_002A8288;
    if (func_001FE5E0(p) && D_002DDED0.dt == 0.0f) g = 0.0f;
    f = GSmooth__FfffP3SMPPf(p->f15C, g, D_002DDED0.f40, &k, 0);
    SetSndmgrChannelVolume(p, 0, f, 2);
    SetSndmgrChannelVolume(p, 1, f, 2);
    SetSndmgrChannelVolume(p, 2, g, 2);
    for (q = p->pq; q;) {
        struct Q *n = q->pNext;
        UpdateSnd(q);
        q = n;
    }
    snd_FlushSoundCommands();
}
} } // namespace M_UpdateSndmgr

namespace M_func_001FDA48 { extern "C" {
typedef int qword __attribute__((mode(TI)));
struct V3 { float x, y, z; float w; };
struct O { char pad[0x54]; float f54; float f58; float f5C; };
struct P { char pad[0x10]; int n10; char pad2[0x6C - 0x14]; int n6C; float f70; char x74[0xC]; struct V3 v80; struct V3 v90; };
struct SW { char pad[0x4034]; struct O *p4034; };
extern struct SW D_003DD490;
void func_001FCF70(void);
void func_002330D0(int, int, int, int, int, int, int, void (*)(void), long);
void *PpoHero(struct SW *);
int FIsBasicDerivedFrom__FP5BASIC3CID(void *, int);
qword GetLoWorldPos(void *);
void func_00156BE0(struct O *, qword, void *, void *);
static inline float Eval(struct V3 *pv, float t)
{
    return pv->x + t * (pv->y + t * pv->z);
}
/** @sly2-annot addr=0x1FDA48
 * Purpose: Poll SUKR cmd 0xB; when player (STEPPLAYER 0x34) exists and +0x70>0.5, track a listener object
 * Info: proposed name UpdateSndmgrListener (T2); TU sound
 * Confidence: low (draft) [T2 0.30] - evidence: static analysis; sources: r7_1F8DD8
 * See: docs/reference/functions/sound.md#sound-0x1faf70-0x1ff3b8
 * @end-sly2-annot */
void func_001FDA48(struct P *p)
{
    struct O *po;
    qword q;
    void *pv;
    if (!p->n10) {
        p->f70 = 0.0f;
    } else if (!p->n6C) {
        p->n6C = 1;
        func_002330D0(0x524B5553, 0xB, 0, 0, 0, 0, 0, func_001FCF70, (long)p);
    }
    po = 0;
    pv = PpoHero(&D_003DD490);
    if (pv && FIsBasicDerivedFrom__FP5BASIC3CID(pv, 0x34) && 0.5f < p->f70) {
        float y;
        po = D_003DD490.p4034;
        q = GetLoWorldPos(pv);
        po->f54 = Eval(&p->v80, p->f70);
        y = Eval(&p->v90, p->f70);
        po->f5C = y;
        po->f58 = -y;
    }
    func_00156BE0(po, q, pv, p->x74);
}
} } // namespace M_func_001FDA48

namespace M_func_001FDBA8 { extern "C" {
struct A { char pad[0xA0]; int f; };
void func_001FCF88(void);
void func_002330D0(int id, int n, int a, int b, int c, int d, int e, void (*pfn)(void), long p);
/** @sly2-annot addr=0x1FDBA8
 * Purpose: Issue SUKR cmd 0xC once (busy flag) with callback
 * Info: proposed name PollSndmgrStatus (T2); TU sound
 * Confidence: low (draft) [T2 0.30] - evidence: static analysis; sources: r7_1F8DD8
 * See: docs/reference/functions/sound.md#sound-0x1faf70-0x1ff3b8
 * @end-sly2-annot */
void func_001FDBA8(struct A *p)
{
    if (!p->f) {
        p->f = 1;
        func_002330D0(0x524B5553, 0xC, 0, 0, 0, 0, 0, func_001FCF88, (long)p);
    }
}
} } // namespace M_func_001FDBA8

namespace M_func_001FDC08 {
// Select and normalize three animation blend weights.
extern "C" {
struct REF {char pad50[0x50];unsigned short flags,state;};
struct COEF {float a,b,c;};
struct PLAYER {char pad4C8[0x4C8];int action;char pad4CC[0xBFC];int mode;char pad10CC[0x20];int state;};
struct P {char pad5C[0x5C];int modeA,modeB,selected;char pad68[0xA0];float weights[3];struct COEF coefficients[3];char pad138[0xCC];int count;struct REF *refs[8];};
extern char D_003DD490[];
struct PLAYER *PpoCur(void *);
int FIsBasicDerivedFrom__FP5BASIC3CID(void *,int);
float GSmooth__FfffP3SMPPf(float,float,float,struct COEF *,float *);
void SetStreamsndGroupVolume(struct REF *,int,float);
static __inline__ float Sqrt(float f){float r;__asm__ __volatile__("sqrt.s %0,%1":"=f"(r):"f"(f));return r;}
/** @sly2-annot addr=0x1FDC08
 * Purpose: Pick music layer (0 calm/1/2 action) from player state and smooth 3 group weights into the music
 *   STREAMSND
 * Info: proposed name UpdateSndmgrMusicMix (T2); TU sound
 * Confidence: low (draft) [T2 0.45] - evidence: static analysis; sources: r7_1F8DD8
 * See: docs/reference/functions/sound.md#sound-0x1faf70-0x1ff3b8
 * @end-sly2-annot */
void func_001FDC08(struct P *input)
{
    struct P *p=input;
    struct REF *ref;
    float *weights;
    int i;
    float sum,selectedSquare;
    p->selected=1;
    if(p->modeA)p->selected=2;
    else if(p->modeB)p->selected=0;
    else if(PpoCur(D_003DD490) && FIsBasicDerivedFrom__FP5BASIC3CID(PpoCur(D_003DD490),0x34)){
        struct PLAYER *player=PpoCur(D_003DD490);
        if(player->state==3)p->selected=0;
        else if(player->state==4)p->selected=2;
        else if(player->mode!=-1)p->selected=0;
        else if(player->action>=0){if(player->action<3 || player->action==13)p->selected=0;}
    }
    if(p->count>0)ref=((struct REF **)&p->count)[p->count];else ref=0;
    if(ref){
        int locked=0;
        if(ref->state || (ref->flags&1))locked=1;
        if(!locked)goto smooth;
    }
    p->weights[p->selected]=1.0f;
    goto norm;
smooth:
    p->weights[p->selected]=GSmooth__FfffP3SMPPf(p->weights[p->selected],1.0f,g_clock.dtReal,&p->coefficients[p->selected],0);
norm:
    sum=0.0f;selectedSquare=p->weights[p->selected]*p->weights[p->selected];
    for(i=0;i<3;i++)if(i!=p->selected)sum+=p->weights[i]*p->weights[i];
    if(sum<0.0001f){
        for(i=0;i<3;i++){
            p->weights[i]=(i==p->selected)?1.0f:0.0f;
        }
    }else{
        float gain=Sqrt((1.0f-selectedSquare)/sum);
        for(i=0;i<3;i++)if(i!=p->selected)p->weights[i]*=gain;
    }
    if(ref){SetStreamsndGroupVolume(ref,0,p->weights[0]);SetStreamsndGroupVolume(ref,1,p->weights[1]);SetStreamsndGroupVolume(ref,2,p->weights[2]);}
}

}
} // namespace M_func_001FDC08

/** @sly2-annot addr=0x1FDEC0
 * Purpose: Empty per-frame hook (jr ra)
 * Info: proposed name UpdateSndmgrStub (T2); TU sound
 * Confidence: low (draft) [T2 0.20] - evidence: static analysis; sources: r7_1F8DD8
 * See: docs/reference/functions/sound.md#sound-0x1faf70-0x1ff3b8
 * @end-sly2-annot */
extern "C" void func_001FDEC0()
{
}

namespace M_func_001FDEC8 { extern "C" {
typedef unsigned long ulong;
struct X { char pad[0x40]; int n40; char pad2[0x4E - 0x44]; short n4E; unsigned short f50; unsigned short n52; char pad3[0x60 - 0x54]; struct S *ps; char pad4[0x68 - 0x64]; ulong ul68; };
struct Ent { short h0; short pad; int n4; };
struct S { char pad[0x50]; float f50; int n54; struct Ent *a58; };
struct P { char pad[0x200]; struct X *r200; char pad2[0x244 - 0x204]; int n244; float f248; float f24C; };
void StopDialog(struct P *);
void func_001FE0E8(struct P *, struct Ent *);
/** @sly2-annot addr=0x1FDEC8
 * Purpose: Advance current dialog: by elapsed stream time fire lip-sync/anim events (0x1FE0E8); end ->
 *   StopDialog
 * Info: proposed name UpdateDialog (T2); TU sound
 * Confidence: low (draft) [T2 0.45] - evidence: static analysis; sources: r7_1F8DD8
 * See: docs/reference/functions/sound.md#sound-0x1faf70-0x1ff3b8
 * @end-sly2-annot */
void func_001FDEC8(struct P *p)
{
    struct X *x = p->r200;
    struct S *ps;
    int f;
    if (!x) return;
    {
        int g = x->n52 || (x->f50 & 1);
        if (g) return;
    }
    x = p->r200;
    if (x->n4E > 0) return;
    ps = x->ps;
    p->f24C += g_clock.dtReal;
    f = 0;
    if (!x->n40) {
        f = x->n52 || (x->f50 & 1);
        f ^= 1;
    }
    if (f) {
        if (0.0f < (float)p->r200->ul68 / 294912000.0f || ps->f50 <= p->f24C) StopDialog(p);
        return;
    }
    while (p->n244 < ps->n54) {
        struct Ent *pe = &ps->a58[p->n244];
        float d = (float)pe->h0 * 0.016666668f;
        if ((float)p->r200->ul68 / 294912000.0f < p->f248 + d) return;
        func_001FE0E8(p, pe);
        p->f248 += d;
        p->n244++;
    }
}
} } // namespace M_func_001FDEC8

namespace M_func_001FE0E8 { extern "C" {
struct E { short s0; short k; union { int n; float g; } u; };
struct P { char pad[0x22C]; void *ap[3]; void *p238; };
void func_001F80D0(void *, float);
void SetSmaGoal(void *, int);
/** @sly2-annot addr=0x1FE0E8
 * Purpose: Event k: role 3 -> 0x1F80D0(gesture,g); else SetSmaGoal(role SM, state)
 * Info: proposed name ApplyDialogEvent (T2); TU sound
 * Confidence: low (draft) [T2 0.45] - evidence: static analysis; sources: r7_1F8DD8
 * See: docs/reference/functions/sound.md#sound-0x1faf70-0x1ff3b8
 * @end-sly2-annot */
void func_001FE0E8(struct P *p, struct E *pe)
{
    int k = pe->k;
    if (k != -1 && p->ap[k]) {
        if (k == 3) func_001F80D0(p->p238, pe->u.g);
        else if (pe->u.n != -1) SetSmaGoal(p->ap[k], pe->u.n);
    }
}
} } // namespace M_func_001FE0E8

namespace M_PushMusic {
struct VTE { short d; short p; void (*pfn)(void *); };
struct VT { VTE a[8]; };
struct X { char pad[0xC]; VT *pvt; };
struct Ref;
extern "C" void func_00259BB0(Ref *, X *);
extern "C" void func_00259B48(Ref *, X *);
struct Ref {
    X *px;
    Ref(X *p) { func_00259BB0(this, p); }
    Ref(const Ref &r) { func_00259BB0(this, r.px); }
    Ref &operator=(const Ref &r) { func_00259B48(this, r.px); return *this; }
    Ref &operator=(X *p) { func_00259B48(this, p); return *this; }
    ~Ref() { if (px) px->pvt->a[2].pfn((char *)px + px->pvt->a[2].d); }
};
struct S { int c; Ref a[8]; };
struct P { char pad[0x204]; S s; };
extern "C" { extern int D_002DD5BC; }
extern "C" { extern int D_002F7900; }
extern "C" { extern int D_002E1E40; }
extern "C" {
Ref func_001FE478(P *, int);
void StopSnd(X *, float);
X *func_001FCFF8(P *, int);
X *PstreamsndNew(X *, int);
void func_001FC408(X *, int, int, float, float *);
}
struct V2 { float x, y; };
static inline void Set(V2 *pv, float x, float y) { pv->x = x; pv->y = y; }
/** @sly2-annot addr=0x1FE150
 * Purpose: Start music id: stop current top, reuse or create STREAMSND kind 4 with player-object sources,
 *   push on 8-deep music stack
 * Info: proposed name PushMusic (T1); TU sound
 * Confidence: mid (draft) [T1 0.50] - evidence: static analysis; sources: r7_1F8DD8
 * See: docs/reference/functions/sound.md#sound-0x1faf70-0x1ff3b8
 * @end-sly2-annot */
extern "C" X *PushMusic(P *p, int n)
{
    Ref r = func_001FE478(p, n);
    X *top = p->s.c > 0 ? (p->s.a + p->s.c - 1)->px : 0;
    if (top) StopSnd(top, 0.0f);
    if (r.px) {
        r.px->pvt->a[7].pfn((char *)r.px + r.px->pvt->a[7].d);
    } else {
        X *v = func_001FCFF8(p, n);
        if (!v || p->s.c == 8) return 0;
        float f = 1.0f;
        r = PstreamsndNew(v, 4);
        V2 af;
        Set(&af, 2000.0f, 5000.0f);
        func_001FC408(r.px, 3, D_002DD5BC, f, &af.x);
        func_001FC408(r.px, 3, D_002F7900, f, &af.x);
        func_001FC408(r.px, 4, D_002E1E40, f, &af.x);
    }
    return (p->s.a[p->s.c++] = r).px;
}
} // namespace M_PushMusic

namespace M_func_001FE310 {
struct VTE { short d; short p; void (*pfn)(void *); };
struct VT { VTE a[8]; };
struct X { char pad[0xC]; VT *pvt; char pad2[0x40 - 0x10]; int n40; char pad3[0x50 - 0x44]; unsigned short fl; unsigned short c52; };
struct S { int c; void *a[8]; };
struct P { char pad[0x204]; S s; };
struct Ref {
    X *px;
    ~Ref() { if (px) px->pvt->a[2].pfn((char *)px + px->pvt->a[2].d); }
};
extern "C" void func_001FE478(Ref *, P *, int);
extern "C" void StopSnd(X *, float);
extern "C" void func_00259B48(Ref *, void *);
/** @sly2-annot addr=0x1FE310
 * Purpose: Stop music id if playing (0x1FE478 lookup), StopSnd
 * Info: proposed name StopMusic (T2); TU sound
 * Confidence: low (draft) [T2 0.40] - evidence: static analysis; sources: r7_1F8DD8
 * See: docs/reference/functions/sound.md#sound-0x1faf70-0x1ff3b8
 * @end-sly2-annot */
extern "C" void func_001FE310(P *p, int n)
{
    Ref r;
    func_001FE478(&r, p, n);
    if (r.px) {
        int f = 0;
        if (!r.px->n40) { f = r.px->c52 || (r.px->fl & 1); f ^= 1; }
        if (!f) {
            StopSnd(r.px, 0.0f);
            func_00259B48(&r, p->s.c > 0 ? *(p->s.a + p->s.c - 1) : 0);
            if (r.px) r.px->pvt->a[7].pfn((char *)r.px + r.px->pvt->a[7].d);
        }
    }
}
} // namespace M_func_001FE310

namespace M_TMusicCur { extern "C" {
typedef unsigned long ulong;
struct Q { char pad[0x68]; ulong t; };
struct S { int c; struct Q *a[8]; };
struct P { char pad[0x204]; struct S s; };
/** @sly2-annot addr=0x1FE3F8
 * Purpose: Play time of top music stream in seconds (ticks/294912000)
 * Info: proposed name TMusicCur (T1); TU sound
 * Confidence: mid (draft) [T1 0.50] - evidence: static analysis; sources: r7_1F8DD8
 * See: docs/engine/minigames.md#7-dance-dancer-0x44--rhythm-manager-cid121-0x79-rhythm-tu-0x1520c8-0x1539a8-ccode,
 *   docs/engine/sound.md#2-sound-instances, docs/guides/port-plan.md#64-sound--coreaudio-k12
 * @end-sly2-annot */
float TMusicCur(struct P *p)
{
    struct Q *q;
    if (p->s.c > 0) { q = *(p->s.a + p->s.c - 1); }
    else q = 0;
    if (!q) return 0.0f;
    return (float)q->t * (1.0f / 294912000.0f);
}
} } // namespace M_TMusicCur

namespace M_func_001FE478 {
struct VTE { short d; short p; void (*pfn)(void *); };
struct VT { VTE a[8]; };
struct X { char pad[0xC]; VT *pvt; char pad2[0x60 - 0x10]; X *p60; };
struct Ref;
extern "C" void func_00259BB0(Ref *, X *);
extern "C" void func_00259B48(Ref *, X *);
struct Ref {
    X *px;
    Ref(X *p) { func_00259BB0(this, p); }
    Ref(const Ref &r) { func_00259BB0(this, r.px); }
    Ref &operator=(const Ref &r) { func_00259B48(this, r.px); return *this; }
    ~Ref() { if (px) px->pvt->a[2].pfn((char *)px + px->pvt->a[2].d); }
};
struct S { int c; Ref a[8]; };
struct P { char pad[0x204]; S s; };
extern "C" X *func_001FCFF8(P *, int);
/** @sly2-annot addr=0x1FE478
 * Purpose: Ref to a music STREAMSND on the stack matching id
 * Info: proposed name FindMusic (T2); TU sound
 * Confidence: low (draft) [T2 0.40] - evidence: static analysis; sources: r7_1F8DD8
 * See: docs/reference/functions/sound.md#sound-0x1faf70-0x1ff3b8
 * @end-sly2-annot */
extern "C" Ref func_001FE478(P *p, int n)
{
    X *v = func_001FCFF8(p, n);
    int i;
    for (i = 0; i < p->s.c; i++)
        if (p->s.a[i].px->p60 == v) break;
    if (i == p->s.c) return Ref(0);
    Ref r(p->s.a[i]);
    p->s.c--;
    for (; i < p->s.c; i++) p->s.a[i] = p->s.a[i + 1];
    return r;
}
} // namespace M_func_001FE478

namespace M_func_001FE5B8 { extern "C" {
struct S { char pad[0x68]; int fl; };
/** @sly2-annot addr=0x1FE5B8
 * Purpose: Set/clear bits in SNDMGR flags
 * Info: proposed name SetSndmgrFlag (T2); TU sound
 * Confidence: low (draft) [T2 0.40] - evidence: static analysis; sources: r7_1F8DD8
 * See: docs/reference/functions/cutscenes.md#tu_1471c0-0x1471c0-0x147e20,
 *   docs/reference/functions/sound.md#sound-0x1faf70-0x1ff3b8
 * @end-sly2-annot */
void func_001FE5B8(struct S *p, int f, int fSet) { if (fSet) p->fl |= f; else p->fl &= ~f; }
} } // namespace M_func_001FE5B8

/** @sly2-annot addr=0x1FE5E0
 * Purpose: +0x68 == 3
 * Info: proposed name FSndmgrMuted (T2); TU sound
 * Confidence: low (draft) [T2 0.30] - evidence: static analysis; sources: r7_1F8DD8
 * See: docs/reference/functions/sound.md#sound-0x1faf70-0x1ff3b8
 * @end-sly2-annot */
extern "C" int func_001FE5E0(SNDMGRV *psndmgr)
{
    return psndmgr->nState == 3;
}

namespace M_StartDialog {
struct VTE { short d; short p; void (*pfn)(void *); };
struct X { char pad[0xC]; VTE *pvt; };
struct Ref;
extern "C" void func_00259B48(Ref *, X *);
struct Ref {
    X *px;
    Ref &operator=(X *p) { func_00259B48(this, p); return *this; }
    operator X *() { return px; }
};
struct D { char pad[0x48]; int n48; char pad2[0x54 - 0x4C]; int n54; };
struct P { char pad[0x200]; Ref r200; char pad2[0x22C - 0x204]; X *a[6]; int n244; int n248; int n24C; };
struct G;
extern "C" { extern G D_003DD490; }
extern "C" { extern char D_004435F0[]; }
extern "C" {
void StopDialog(P *, int);
D *func_001FCFA0(P *, int);
X *PstreamsndNew(D *, int);
void func_00227728(void *, int, int);
X *PloFindSwObjectByClass__FP2SWi3CIDP2LO(G *, int, int, int);
int func_001FD0C0(P *, int);
X *func_001F67E0(X *, int);
}
/** @sly2-annot addr=0x1FE5F0
 * Purpose: Stop current dialog, create dialog STREAMSND kind 0, notify subtitles (0x227728), bind the speaker
 *   object SMs for 6 lip-sync roles
 * Info: proposed name StartDialog (T1); TU sound
 * Confidence: mid (draft) [T1 0.55] - evidence: static analysis; sources: r7_1F8DD8
 * See: docs/reference/functions/sound.md#sound-0x1faf70-0x1ff3b8
 * @end-sly2-annot */
extern "C" X *StartDialog(P *p, int k, int f)
{
    D *pd;
    StopDialog(p, k);
    pd = func_001FCFA0(p, k);
    if (!pd) return 0;
    p->r200 = PstreamsndNew(pd, 0);
    p->n244 = 0;
    p->n248 = 0;
    p->n24C = 0;
    if (!f) func_00227728(D_004435F0, k, pd->n48);
    if (pd->n48 != -1 && pd->n54) {
        register X *po __asm__("$19")=PloFindSwObjectByClass__FP2SWi3CIDP2LO(&D_003DD490, 5, pd->n48, 0);
        if (po) {
            register int i __asm__("$18")=0;
            register int primaryRole __asm__("$20")=3;
            register X **slot __asm__("$16")=p->a;
            for(;i<6;i++,slot++) {
                if(i==primaryRole)p->a[3]=po;
                else *slot=func_001F67E0(po,func_001FD0C0(p,i));
                { X *current=*slot;
                  if(current) { VTE &entry=current->pvt[1];
                    entry.pfn((char *)current+entry.d); } }
            }
        }
    }
    return p->r200;
}
} // namespace M_StartDialog

namespace M_StopDialog { extern "C" {
struct VTE { short d; short p; void (*pfn)(void *); };
struct VT { struct VTE a[3]; };
struct X { char pad[0xC]; struct VT *pvt; };
struct A { int a, b; };
struct E { char pad[0x54]; int c; struct A *ae; };
struct Y { char pad[0x60]; struct E *pe; };
struct Ref { struct Y *px; };
struct P { char pad[0x200]; struct Ref r; char pad2[0x22C - 0x204]; struct X *ap[6]; int n244; int n248; float g24C; };
void func_001FE0E8(struct P *, struct A *);
void SetSmaGoal(void *, int);
void StopSnd(struct Y *, float);
void func_00259B48(struct Ref *, void *);
void func_002333B4(void *, int, int) __asm__("memset");
static inline void Stop(struct Ref *pr)
{
    if (pr && pr->px) {
        StopSnd(pr->px, 0.0f);
        func_00259B48(pr, 0);
    }
}
/** @sly2-annot addr=0x1FE738
 * Purpose: Flush remaining dialog events, set role-5 SM to 0x611, stop stream, reset dialog state
 * Info: proposed name StopDialog (T1); TU sound
 * Confidence: high [T1 0.85] - evidence: static analysis; sources: r7_1F8DD8+verdicts_tu_r7r8
 * See: docs/engine/sound.md#6-key-functions, docs/reference/functions/sound.md#sound-0x1faf70-0x1ff3b8
 * @end-sly2-annot */
void StopDialog(struct P *p)
{
    int i;
    struct E *pe;
    if (!p->r.px) return;
    pe = p->r.px->pe;
    while (p->n244 < pe->c) {
        func_001FE0E8(p, &pe->ae[p->n244]);
        p->n244++;
    }
    if (p->ap[5]) SetSmaGoal(p->ap[5], 0x611);
    Stop(&p->r);
    p->n244 = 0;
    p->n248 = 0;
    p->g24C = 0.0f;
    for (i = 0; i < 6; i++) {
        struct X *px = p->ap[i];
        if (px) px->pvt->a[2].pfn((char *)px + px->pvt->a[2].d);
    }
    func_002333B4(p->ap, 0, 0x18);
}
} } // namespace M_StopDialog

namespace M_func_001FE848 { extern "C" {
struct U { char pad[0x40]; int n; };
struct T { char pad[0x60]; struct U *pu; };
struct S { char pad[0x200]; struct T *pt; };
static inline int PresentValue(void){register int r __asm__("$4");__asm__("addiu %0,$zero,%1":"=r"(r):"i"(1));return r;}
/** @sly2-annot addr=0x1FE848
 * Purpose: True if dialog (optionally of given id) is playing
 * Info: proposed name FDialogPlaying (T2); TU sound
 * Confidence: low (draft) [T2 0.45] - evidence: static analysis; sources: r7_1F8DD8
 * See: docs/reference/functions/cutscenes.md#tu_1471c0-0x1471c0-0x147e20,
 *   docs/reference/functions/sound.md#sound-0x1faf70-0x1ff3b8
 * @end-sly2-annot */
int func_001FE848(struct S *p, int n)
{
    struct U *pu; int f;
    if (p->pt) {
        pu = p->pt->pu;
        f = 0;
        if (n == -1 || pu->n == n) f = PresentValue();
        return f;
    }
    return 0;
}

typedef char guard_int_size[(sizeof(int)==4)?1:-1];
typedef char guard_pointer_size[(sizeof(void*)==4)?1:-1];
typedef char guard_selector[((unsigned)&((struct U*)0)->n==0x40)?1:-1];
typedef char guard_subobject[((unsigned)&((struct T*)0)->pu==0x60)?1:-1];
typedef char guard_owner[((unsigned)&((struct S*)0)->pt==0x200)?1:-1];
typedef char guard_selector_size[(sizeof(struct U)==0x44)?1:-1];
typedef char guard_subobject_size[(sizeof(struct T)==0x64)?1:-1];
typedef char guard_owner_size[(sizeof(struct S)==0x204)?1:-1];
} } // namespace M_func_001FE848

namespace M_func_001FE888 { extern "C" {
typedef unsigned long ulong;
struct X { char pad[0x40]; int n40; char pad2[0x50 - 0x44]; unsigned short fl; unsigned short c52; char pad3[0x68 - 0x54]; ulong t; };
struct P { char pad[0x200]; struct X *px; char pad2[0x24C - 0x204]; float g24C; };
static inline float T(struct X *px) { return (float)px->t * (1.0f / 294912000.0f); }
/** @sly2-annot addr=0x1FE888
 * Purpose: Current dialog play time (or cached time while starting)
 * Info: proposed name TDialogCur (T2); TU sound
 * Confidence: low (draft) [T2 0.40] - evidence: static analysis; sources: r7_1F8DD8
 * See: docs/reference/functions/sound.md#sound-0x1faf70-0x1ff3b8
 * @end-sly2-annot */
float func_001FE888(struct P *p)
{
    if (!p->px) return 0.0f;
    if (T(p->px) == 0.0f) {
        struct X *px = p->px;
        int f = 0;
        if (!px->n40) { f = px->c52 || (px->fl & 1); f ^= 1; }
        if (f) return p->g24C;
    }
    return T(p->px);
}
} } // namespace M_func_001FE888

namespace M_func_001FE990 { extern "C" {
struct S { char pad[0x1D0]; int c; int a[1]; };
/** @sly2-annot addr=0x1FE990
 * Purpose: Index of id in a small int table (-1)
 * Info: proposed name IambFind (T2); TU sound
 * Confidence: low (draft) [T2 0.30] - evidence: static analysis; sources: r7_1F8DD8
 * See: docs/engine/animation.md#7-key-functions, docs/reference/functions/animation.md#amb-0x12eb48-0x12f0f0,
 *   docs/reference/functions/sound.md#sound-0x1faf70-0x1ff3b8
 * @end-sly2-annot */
int func_001FE990(struct S *p, int n) { int i; for (i = 0; i < p->c; i++) if (p->a[i] == n) return i; return -1; }
} } // namespace M_func_001FE990

/** @sly2-annot addr=0x1FE9C8
 * Purpose: snd_SetPlaybackMode(mode) if changed (mono/stereo option)
 * Info: proposed name SetSndmgrPlaybackMode (T1); TU sound
 * Confidence: mid (draft) [T1 0.50] - evidence: static analysis; sources: r7_1F8DD8
 * See: docs/reference/functions/sound.md#sound-0x1faf70-0x1ff3b8
 * @end-sly2-annot */
extern "C" void SetSndmgrPlaybackMode(SNDMGRV *psndmgr, int nMode)
{
    if (psndmgr->nPlaybackMode != nMode) {
        psndmgr->nPlaybackMode = nMode;
        snd_SetPlaybackMode(nMode);
    }
}

namespace M_SetSndmgrMasterVolume { extern "C" {
struct P { char pad[0x1BC]; float g; };
struct S;
extern struct S D_002BBB60;
float GLimitLm__FP2LMf(struct S *, float);
void snd_SetMasterVolume(int, int);
/** @sly2-annot addr=0x1FE9F8
 * Purpose: Clamp and send master volume (snd_SetMasterVolume(0x10,..))
 * Info: proposed name SetSndmgrMasterVolume (T1); TU sound
 * Confidence: mid (draft) [T1 0.50] - evidence: static analysis; sources: r7_1F8DD8
 * See: docs/reference/functions/main-loop-startup.md#cheat-0x14bea8-0x14c548,
 *   docs/reference/functions/sound.md#sound-0x1faf70-0x1ff3b8
 * @end-sly2-annot */
void SetSndmgrMasterVolume(struct P *p, float x)
{
    float g = GLimitLm__FP2LMf(&D_002BBB60, x);
    int n = (int)(g * 1024.0f);
    if ((int)(p->g * 1024.0f) != n) {
        p->g = g;
        snd_SetMasterVolume(0x10, n);
    }
}
} } // namespace M_SetSndmgrMasterVolume

namespace M_SetSndmgrChannelVolume {
class X;
typedef void (X::*PMF)();
struct P {char pad0[0x13C];float weights[8][4];int pad1BC;float outputs[4];char pad1D0[0x30];X *object;int count;X *refs[8];X *component;};
struct LM {float low,high;};
extern "C" { extern LM D_002BBB60; }
extern "C" { extern PMF D_002A8298,D_002A82A0; }
extern "C" float GLimitLm__FP2LMf(LM *,float);
extern "C" void *func_002333B4(void *,int,unsigned int) __asm__("memset");
extern "C" void snd_SetMasterVolume(int,int);
static inline X *ReadReceiver(X **pp)
{
    register X *receiver __asm__("$4");
    __asm__("lw %0,%1":"=r"(receiver):"m"(*pp));
    return receiver;
}
/** @sly2-annot addr=0x1FEA60
 * Purpose: Set weight[category<8][channel<4], recompute channel output and send snd_SetMasterVolume; update
 *   live streams
 * Info: proposed name SetSndmgrChannelVolume (T1); TU sound
 * Confidence: mid (draft) [T1 0.50] - evidence: static analysis; sources: r7_1F8DD8
 * See: docs/reference/functions/sound.md#sound-0x1faf70-0x1ff3b8
 * @end-sly2-annot */
extern "C" void SetSndmgrChannelVolume(P *p,int channel,int category,float input)
{
    if((unsigned int)category>=8)return;
    if(channel<0)return;
    if(channel>=4)return;
    input=GLimitLm__FP2LMf(&D_002BBB60,input);
    float (*w)[4]=p->weights;
    if(p->weights[category][channel]==input)return;
    if(category==2){
        PMF action;
        func_002333B4(&action,0,8);
        if(input==0.0f)action=D_002A8298;
        else if(w[2][channel]==0.0f)action=D_002A82A0;
        if(action!=0){
            switch(channel){
            case 0:if(p->component)(p->component->*action)();break;
            case 1:for(int i=0;i<p->count;i++){X **pp=&p->refs[i];X *o=ReadReceiver(pp);if(o){union {X *p;unsigned w;} r;r.p=o;(((X *)r.w)->*action)();}}break;
            case 2:if(p->object)(p->object->*action)();break;
            }
        }
    }
    p->weights[category][channel]=input;
    input=1.0f;
    for(int i=0;i<8;i++)input*=p->weights[i][channel];
    int next=(int)(input*1024.0f);
    if((int)(p->outputs[channel]*1024.0f)!=next){
        p->outputs[channel]=input;
        snd_SetMasterVolume(channel,next);
    }
}

typedef char layout_pointer[(sizeof(void*)==4)?1:-1];
typedef char layout_int[(sizeof(int)==4)?1:-1];
typedef char layout_float[(sizeof(float)==4)?1:-1];
typedef char layout_member_pointer[(sizeof(PMF)==8)?1:-1];
typedef char layout_limit[(sizeof(LM)==8)?1:-1];
typedef char layout_limit_low[((unsigned)&((LM*)0)->low==0)?1:-1];
typedef char layout_limit_high[((unsigned)&((LM*)0)->high==4)?1:-1];
typedef char layout_weights[((unsigned)&((P*)0)->weights==0x13C)?1:-1];
typedef char layout_weight_stride[(sizeof(((P*)0)->weights[0])==0x10)?1:-1];
typedef char layout_outputs[((unsigned)&((P*)0)->outputs==0x1C0)?1:-1];
typedef char layout_object[((unsigned)&((P*)0)->object==0x200)?1:-1];
typedef char layout_count[((unsigned)&((P*)0)->count==0x204)?1:-1];
typedef char layout_references[((unsigned)&((P*)0)->refs==0x208)?1:-1];
typedef char layout_component[((unsigned)&((P*)0)->component==0x228)?1:-1];
typedef char layout_size[(sizeof(P)==0x22C)?1:-1];
} // namespace M_SetSndmgrChannelVolume

namespace M_ResetSndmgrVolumes { extern "C" {
void SetSndmgrMasterVolume(void *, float);
void SetSndmgrChannelVolume(void *, int, int, float);
/** @sly2-annot addr=0x1FEDF0
 * Purpose: Master=1 and all weights=1 (except category 1)
 * Info: proposed name ResetSndmgrVolumes (T1); TU sound
 * Confidence: mid (draft) [T1 0.50] - evidence: static analysis; sources: r7_1F8DD8
 * See: docs/reference/functions/sound.md#sound-0x1faf70-0x1ff3b8
 * @end-sly2-annot */
void ResetSndmgrVolumes(void *p)
{
    int i, j;
    SetSndmgrMasterVolume(p, 1.0f);
    for (i = 0; i < 8; i++) {
        if (i == 1) continue;
        for (j = 0; j < 4; j++) SetSndmgrChannelVolume(p, j, i, 1.0f);
    }
}
} } // namespace M_ResetSndmgrVolumes

/** @sly2-annot addr=0x1FEE90
 * Purpose: ++SNDMGR+0x5C
 * Info: proposed name IncrementSndmgrCount5C (T2); TU sound
 * Confidence: low (draft) [T2 0.25] - evidence: static analysis; sources: r7_1F8DD8
 * See: docs/reference/functions/guards-ai.md#tu_14a008-0x14a008-0x14b660,
 *   docs/reference/functions/sound.md#sound-0x1faf70-0x1ff3b8
 * @end-sly2-annot */
extern "C" void func_001FEE90(SNDMGRV *psndmgr)
{
    psndmgr->c5C++;
}

/** @sly2-annot addr=0x1FEEA0
 * Purpose: --SNDMGR+0x5C
 * Info: proposed name DecrementSndmgrCount5C (T2); TU sound
 * Confidence: low (draft) [T2 0.25] - evidence: static analysis; sources: r7_1F8DD8
 * See: docs/reference/functions/sound.md#sound-0x1faf70-0x1ff3b8
 * @end-sly2-annot */
extern "C" void func_001FEEA0(SNDMGRV *psndmgr)
{
    psndmgr->c5C--;
}

/** @sly2-annot addr=0x1FEEB0
 * Purpose: ++SNDMGR+0x60
 * Info: proposed name IncrementSndmgrCount60 (T2); TU sound
 * Confidence: low (draft) [T2 0.25] - evidence: static analysis; sources: r7_1F8DD8
 * See: docs/reference/functions/sound.md#sound-0x1faf70-0x1ff3b8
 * @end-sly2-annot */
extern "C" void func_001FEEB0(SNDMGRV *psndmgr)
{
    psndmgr->c60++;
}

/** @sly2-annot addr=0x1FEEC0
 * Purpose: --SNDMGR+0x60
 * Info: proposed name DecrementSndmgrCount60 (T2); TU sound
 * Confidence: low (draft) [T2 0.25] - evidence: static analysis; sources: r7_1F8DD8
 * See: docs/reference/functions/sound.md#sound-0x1faf70-0x1ff3b8
 * @end-sly2-annot */
extern "C" void func_001FEEC0(SNDMGRV *psndmgr)
{
    psndmgr->c60--;
}

// .rodata order (docs/guides/tu-merging.md "Phase 5: data order"): a named constant is emitted where it is
// defined, a function's literals and jump tables with the function, so each constant is defined where the ELF
// has it among the functions, under an asm label (no function sees a definition under the name it reads).
namespace RO_ORDER { extern const SMP r_D_002A8288 __asm__("D_002A8288") = {2.0f, 0.0f, 0.2f}; }   /* copied whole as three words */
namespace RO_ORDER {   /* pointers through asm-labelled declarations */
void p_PauseSnd() __asm__("PauseSnd");
extern const unsigned int r_D_002A8298[2] __asm__("D_002A8298") = {0xFFFF0000, (unsigned int)p_PauseSnd};
}
namespace RO_ORDER {   /* pointers through asm-labelled declarations */
void p_ResumeSnd() __asm__("ResumeSnd");
extern const unsigned int r_D_002A82A0[2] __asm__("D_002A82A0") = {0xFFFF0000, (unsigned int)p_ResumeSnd};
}
namespace M_func_001FEED0 { extern "C" {
struct M { int n0; int pad4; int n8; int nC; short h10; short pad12; int pad14[3]; short h20; short pad22; int pad24; int n28; int pad2C; };
struct P { char pad[0x64]; int n64; char pad2[0xE8 - 0x68]; int nE8; char pad3[0x138 - 0xEC]; int n138; };
int NRandInRange__Fii(int, int);
void *func_002333B4(void *, int, int) __asm__("memset");
void *PsndFromSfxid(struct P *, int);
short IsndFromPsnd(struct P *, void *);
void func_002322F0(struct M *);
/** @sly2-annot addr=0x1FEED0
 * Purpose: Special sfx handling after play: table-driven (7x6, 25) level selection; may stop voice
 * Info: proposed name PlayBanksndVariant (T2); TU sound
 * Notes: verifier: purpose partly wrong: SNDMGR helper (caller 0x1FBC58, when the sound belongs to the
 *   current player): random step from tables a[7][6]/b[25] walks +0x138 within -12..12, then sends a 4
 * Confidence: low (draft) [T2 0.25] - evidence: static analysis; sources: r7_1F8DD8+verdicts_tu_r7r8
 * See: docs/engine/sound.md#6-key-functions, docs/reference/functions/sound.md#sound-0x1faf70-0x1ff3b8
 * @end-sly2-annot */
void func_001FEED0(struct P *p)
{
    int r;
    if (p->n64) return;
    {
    int a[7][6] = {
        { -5, -5, -5, -5, -5, -5 },
        { -5, -5, -5, -5, -5, 1 },
        { -5, -5, -5, -5, 1, 1 },
        { -5, -5, -5, 1, 1, 1 },
        { -5, -5, 1, 1, 1, 1 },
        { -5, 1, 1, 1, 1, 1 },
        { 1, 1, 1, 1, 1, 1 },
    };
    int b[25] = { 6, 6, 6, 6, 6, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 4, 3, 2, 1, 0 };
    struct M m;
    r = NRandInRange__Fii(0, 5);
    p->n138 += a[b[p->n138 + 12]][r];
    func_002333B4(&m, 0, 0x30);
    m.n28 = 0x13;
    m.n0 = p->nE8;
    m.h10 = IsndFromPsnd(p, PsndFromSfxid(p, 0x28));
    m.h20 = p->n138 * 127;
    func_002322F0(&m);
    }
}
} } // namespace M_func_001FEED0

namespace M_func_001FF108 { extern "C" {
struct G { char pad[0x228]; void *p228; };
extern struct G D_003DD040;
int func_001FE990(struct G *);
void SetStreamsndGroupVolume(void *, int, float);
void func_001FC380(void *, int, int);
/** @sly2-annot addr=0x1FF108
 * Purpose: Splice: set volume/pan of ambience sub-group selected by id
 * Info: proposed name SetAmbienceGroup (T2); TU sound
 * Confidence: low (draft) [T2 0.40] - evidence: static analysis; sources: r7_1F8DD8
 * See: docs/reference/functions/sound.md#sound-0x1faf70-0x1ff3b8
 * @end-sly2-annot */
void func_001FF108(int a, int b, int c, float g)
{
    int r = func_001FE990(&D_003DD040);
    void *p = D_003DD040.p228;
    SetStreamsndGroupVolume(p, r, g);
    func_001FC380(p, r, c);
}
} } // namespace M_func_001FF108


namespace M_func_00259B48 { extern "C" {
typedef struct { int pad[2]; short d1; short p1; void (*f1)(void *); short d2; short p2; void (*f2)(void *); } VT;
typedef struct { int pad[3]; VT *vt; } O;
inline void func_00259B48(O **pp, O *o)
{
    if (o) o->vt->f1((char *)o + o->vt->d1);
    if (*pp) (*pp)->vt->f2((char *)*pp + (*pp)->vt->d2);
    *pp = o;
}
} } // namespace M_func_00259B48

namespace M_func_00259BB0 { extern "C" {
typedef struct { int pad[2]; short d; short p; void (*f)(void *); } VT;
typedef struct { int pad[3]; VT *vt; } O;
inline void func_00259BB0(O **pp, O *o) { if (o) o->vt->f((char *)o + o->vt->d); *pp = o; }
} } // namespace M_func_00259BB0

// ---- Data owned by this TU, in address order (docs/guides/tu-merging.md "Data"). Defined after the functions
// so no function sees a definition (keeps the code as matched); the namespaced declarations above refer to these.
extern const LM D_002A83B8 = {500.0f, 3000.0f};
extern const LM D_002A83C0 = {500.0f, 1000.0f};
int D_002C3C40[6] = {1548, 1549, 1550, -1, 1551, 1552};
