// framecapture.cpp -- CID104, the frame-capture transition: an SO that, when started, copies the current frame buffer
// into a texture, takes the player's hands off, slows the game clock and plays a sound, then draws the captured frame
// as two textured quads placed by two child objects (a split/flip screen transition) until its animation ends.
// D_002DD644 points at the running one; the main loop calls DrawFrameCaptureScreen. Entry points: the CID104 vtable
// (InitFrameCapture, PostLoad, UpdateFrameCapture, HandleFrameCaptureMessage), StartFrameCapture (Splice) and
// DrawFrameCaptureScreen / IsFrameCaptureInactive (main loop). VU0 asm (one helper).
// TU 0x1471C0..0x147E20 (11 functions, address order).
// SLY2_ALIAS: __static_initialization_and_destruction_0=StaticInit_framecapture
// SLY2_ALIAS: _GLOBAL_$I$*=GlobalConstructors_framecapture
#include "sly2/slotheap.h"
#include "sly2/classes.h"
#include "sly2/sw.h"
#include "gifs.h"
#include "sly2/clock.h"
#include "sly2/sndmgr.h"
extern "C" char base_D_002DEA28[] __asm__("D_002DEA28");   /* base of the address names below: see docs/guides/tu-merging.md "Data" */

// 0x147CC8 StaticInit_framecapture and GlobalConstructors_framecapture: g++-generated for the six engine slot pools.
SLY2_INSTANTIATE_SLOT_POOLS()

typedef int qwordV __attribute__((mode(TI)));
typedef unsigned long ulong;

/* g++ 2.95 vtable entry: this-adjust delta and the function. */
struct ASEGA;
/* CID104's own fields after its SO base (LO::unknown40 is the tagged flag word: a pointer to flags when bit 0 is
   clear, else the flags themselves; SO::rootTransform is passed with each placer's matrix to GetRelativeXfMatrix). */
struct CAPTUREV : SO
{
    int state;          /* +0x150 CAPTURE_STATE */
    float stateTime;    /* +0x154 */
    char *placers[2];   /* +0x158 the two objects whose matrices (+0x40) place the quads */
    ASEGA *segmentAnim; /* +0x160 the transition animation */
    int drawQuad0;      /* +0x164 per-quad draw flags, toggled by animation events */
    int drawQuad1;      /* +0x168 */
    int drawQuad2;      /* +0x16C */
    int soundCue;       /* +0x170 sound-manager cue the closing phase waits for (-1: none; inferred) */
};
typedef char assert_CAPTURE_soundCue[(unsigned int)&((CAPTUREV *)0)->soundCue == 0x170 ? 1 : -1];
/* Capture states (inferred from the update, draw and message code). */
enum CAPTURE_STATE
{
    CAPTURE_IDLE = -1,
    CAPTURE_STARTED = 0,    /* StartFrameCapture ran; the next update requests the grab */
    CAPTURE_GRAB = 1,       /* the next draw copies the frame buffer */
    CAPTURE_SHOWING = 2,    /* the animation plays the transition */
    CAPTURE_HOLDING = 3     /* animation paused until the sound cue is ready */
};
struct MSG { void *source; int unknown4; int eventKind; };   /* kind 9: an animation event from source */
enum { MESSAGE_SEGMENT_EVENT = 9 };
struct VECTOR4F { float x, y, z, w; };
struct M4 { qwordV r[4]; };
struct TEX { ulong unknown0; ulong pad; ulong unknown10; };   /* GS words of the capture texture (buffer base/width bits) */
struct X { char pad[0x24]; void *unknown24; char pad2[0x30 - 0x28]; int unknown30; };   /* the open frame (g_pfrmOpen) */

/* framecapture's .data: defined after the functions, in address order (the UV offsets D_002BD398/D_002BD39C are one
 * float[6] read through two names; D_002BD398 defined). */
struct SNIP { int grfsnip; int oid; int ib; };   /* Sly 1's SnipLoObjects record */
extern SNIP D_002BD380[2];
extern CAPTUREV *D_002DD644;
extern "C" {
extern X *D_002DEA4C __asm__("g_pfrmOpen");
#undef D_002DEA34
#define D_002DEA34 (*(void * *)((char *)base_D_002DEA28 + 0xC))   /* frm's VIFS+0xC */
extern GIFS D_002DEA00;
extern TEX D_002DECD0;
extern QW D_002DF4B0[];
extern QW D_002DFB00[];
extern QW D_002DFB20[];
extern float D_002BD398[];
extern float D_002BD39C[] __asm__("D_002BD398+4");   /* D_002BD398[1..]: a separate symbol to g++ (one base changes DrawFrameCaptureScreen) */
extern VECTOR4F D_002BD3B0[4];   /* quad corners */
extern float D_002BD3F0[8];   /* corner UVs */
union M4F { float af[16]; M4 m; };   /* floats first, so the matrix can be initialized */
extern M4F D_002BD410;
void InitSo(CAPTUREV *p);
void SetLoBusyKind(CAPTUREV *p, int n);
void PostSoLoad(CAPTUREV *p);
void BindLoOidTable(CAPTUREV *p, int n, void *q);
void DropAsegaRef(ASEGA **ppasega);
int IsAsegaRetracted(ASEGA *pasega);
int IsDialogPlaying(void *p, int n);
void HandleBasicMessage(CAPTUREV *p, int kind, MSG *message);
void SetSoundManagerFlags(void *p, int a, int b);
void SetClockRate(CLOCK *p, int a, float g);
void IncrementSwHandsOff(void *p, int a);
void DecrementSwHandsOff(void *p, int a);
int IsLoInWorld(CAPTUREV *p);
void AppendDlEntry__FP2DLPv(void *p, LOROOT *px);
void SetAsegaMasterSpeed(ASEGA *p, float g);
void *FindSwObject__FP2SWi3OIDP2LO(void *p, int k, int oid, void *q);
void ReplaceCurrentAseg__FP4ASEGP3ALOffiPP5ASEGA(void *aseg, void *alo, float t, float s, int n, ASEGA **pp);
void CaptureFramebufferToTexture(CAPTUREV *p);
void SetFrameCaptureState(CAPTUREV *p, int n);
void GetRelativeXfMatrix(M4 *pm, int n, void *p, void *q);
qwordV __ml__FRC7MATRIX4G10VU_VECTOR4(M4 *pm, qwordV v);
void Force__FPv(void *pv);
}

// Sets bits of an LO flag word that is stored inline or (low bit clear) behind a pointer.
static inline void SetTaggedFlags(int *taggedFlags, int bits)
{
    unsigned int flags = *taggedFlags;
    if ((flags ^ 1) & 1) *(int *)flags |= bits;
    else *taggedFlags = flags | bits;
}

// CID104 vtable slot 7 (Init): SO defaults, idle state, busy, LO flags 0x102.
extern "C" void InitFrameCapture(CAPTUREV *capture)
{
    InitSo(capture);
    capture->state = CAPTURE_IDLE;
    SetLoBusyKind(capture, 1);
    SetTaggedFlags(&capture->unknown40, 0x102);
}

// CID104 vtable slot 15 (PostLoad): finishes the SO load, binds its snip table (the two placing objects), goes idle
// and removes itself from the world until started.
// Also known as: PostLoadAnimatedOverlay (upstream readable / Sly 1 name; PostLoadAnimatedOverlay__FP2LO).
void CID104::PostLoad()
{
    CAPTUREV *capture = (CAPTUREV *)this;
    PostSoLoad(capture);
    BindLoOidTable(capture, 2, D_002BD380);
    capture->state = CAPTURE_IDLE;
    capture->Remove();
}

// CID104 vtable slot 16 (Update): a started capture asks for the grab; a holding one resumes once the sound cue is
// ready (IsDialogPlaying answers 0); a showing one goes idle when its animation has finished. (inferred)
// Also known as: UpdateOverlay (upstream readable / Sly 1 name; UpdateOverlay__FP2LO).
extern "C" void UpdateFrameCapture(CAPTUREV *capture)
{
    switch (capture->state) {
    case CAPTURE_STARTED:
        SetFrameCaptureState(capture, CAPTURE_GRAB);
        return;
    case CAPTURE_HOLDING:
        if (IsDialogPlaying(&g_sndmgr, capture->soundCue) == 0) {
            SetFrameCaptureState(capture, CAPTURE_SHOWING);
            return;
        }
        return;
    case CAPTURE_SHOWING:
        ASEGA *anim = capture->segmentAnim;
        if (anim == 0 || IsAsegaRetracted(anim) != 0) {
            DropAsegaRef(&capture->segmentAnim);
            SetFrameCaptureState(capture, CAPTURE_IDLE);
        }
        break;
    }
}

/* Animation events of the transition (their oids). */
enum
{
    EVENT_SHOW_QUAD0 = 0x5FB, EVENT_HIDE_QUAD0 = 0x5FC,
    EVENT_SHOW_QUAD1 = 0x5FD, EVENT_HIDE_QUAD1 = 0x5FE,
    EVENT_CLEAR_QUAD2 = 0x5FF, EVENT_SET_QUAD2 = 0x600,
    EVENT_WAIT_FOR_SOUND = 0x601
};
// CID104 vtable slot 5 (HandleMessage): events of its own animation switch the per-quad draw flags or, when a sound
// cue is set, pause the animation until the cue is ready (CAPTURE_HOLDING). (inferred)
// Also known as: OnOverlayMessage (upstream readable / Sly 1 name; OnOverlayMessage__FP2LO5MSGIDPv).
extern "C" void HandleFrameCaptureMessage(CAPTUREV *capture, int kind, MSG *message)
{
    HandleBasicMessage(capture, kind, message);
    if (kind != MESSAGE_SEGMENT_EVENT) return;
    if (message->source != capture->segmentAnim) return;
    switch (message->eventKind) {
    case EVENT_SHOW_QUAD0: capture->drawQuad0 = 1; break;
    case EVENT_HIDE_QUAD0: capture->drawQuad0 = 0; break;
    case EVENT_SHOW_QUAD1: capture->drawQuad1 = 1; break;
    case EVENT_HIDE_QUAD1: capture->drawQuad1 = 0; break;
    case EVENT_CLEAR_QUAD2: capture->drawQuad2 = 0; break;
    case EVENT_SET_QUAD2: capture->drawQuad2 = 1; break;
    case EVENT_WAIT_FOR_SOUND:
        if (capture->soundCue != -1) SetFrameCaptureState(capture, CAPTURE_HOLDING);
        break;
    }
}

// State change. Leaving idle makes this the running capture (D_002DD644), pauses sound group 2 (inferred), stops the
// game clock (rate 0 on clock channel 8), takes the player's hands off and adds the object to the world (and its root
// to the world's display list, vtable slot 23, if not yet there); returning to idle undoes all of it. Holding pauses
// the animation (master speed 0) and leaving it resumes it.
extern "C" void SetFrameCaptureState(CAPTUREV *capture, int state)
{
    if (state == capture->state) return;
    switch (capture->state) {
    case CAPTURE_IDLE:
        D_002DD644 = capture;
        SetSoundManagerFlags(&g_sndmgr, 2, 0);
        SetClockRate(&g_clock, 8, 0.0f);
        IncrementSwHandsOff(&g_sw, 0);
        capture->Add();
        if (!IsLoInWorld(capture)) {
            LO *rootObject;
            AppendDlEntry__FP2DLPv(&g_sw.busyRoots, capture->root);
            rootObject = capture->root->topObject;
            rootObject->OnAdd();
        }
        break;
    case CAPTURE_HOLDING:
        if (capture->segmentAnim) SetAsegaMasterSpeed(capture->segmentAnim, 1.0f);
        break;
    }
    capture->state = state;
    capture->stateTime = g_clock.realTime;
    switch (state) {
    case CAPTURE_IDLE:
        D_002DD644 = 0;
        SetClockRate(&g_clock, 8, 1.0f);
        SetSoundManagerFlags(&g_sndmgr, 2, 1);
        DecrementSwHandsOff(&g_sw, 0);
        capture->Remove();
        break;
    case CAPTURE_HOLDING:
        if (capture->segmentAnim) SetAsegaMasterSpeed(capture->segmentAnim, 0.0f);
        break;
    }
}

// Builds and sends the GIF/DMA packet that copies the frame buffer into the capture texture (D_002DECD0).
// Also known as: CaptureOverlayFrame (upstream readable / Sly 1 name; CaptureOverlayFrame__Fv).
extern "C" void CaptureFramebufferToTexture(CAPTUREV *capture)
{
    GIFS *g = &D_002DEA00;
    TEX *texture = &D_002DECD0;
    g->AddPrimPack(0, 1, 0xE);
    g->PackAD(0x4D, (texture->unknown10 & 0x1FF) | (texture->unknown0 & 0x3F0000));
    g->PackAD(0x48, 0x31001);
    g->EndDmaCnt();
    g->AddDmaRefs(2, D_002DFB00);
    g->AddDmaCnt();
    g->AddPrimPack(0x316, 7, 0x53531EE);
    g->PackAD(0x15, 0x61);
    g->PackAD(0x3F, 0);
    g->PackRGBA(0x80808080);
    g->PackUV(8, 8);
    g->PackXYZ(0x7000, 0x7900, 0);
    g->PackUV(0x2008, 0xE08);
    g->PackXYZ(0x8000, 0x8700, 0);
    g->EndDmaCnt();
    g->AddDmaRefs(2, D_002DFB20);
    g->AddDmaCnt();
    g->AddPrimPack(0x316, 7, 0x53531EE);
    g->PackAD(0x15, 0x61);
    g->PackAD(0x3F, 0);
    g->PackRGBA(0x80808080);
    g->PackUV(8, 8);
    g->PackXYZ(0x8000, 0x7900, 0);
    g->PackUV(0x2008, 0xE08);
    g->PackXYZ(0x9000, 0x8700, 0);
}

// True unless the capture is in its closing states (2 or 3); the main loop uses it to decide whether to draw
// normally. (inferred)
// Also known as: FAnimatedOverlayDrawNormal (upstream readable / Sly 1 name; FAnimatedOverlayDrawNormal__FPc).
extern "C" int IsFrameCaptureInactive(CAPTUREV *capture)
{
    switch (capture->state) {
    case CAPTURE_SHOWING:
    case CAPTURE_HOLDING:
        return 0;
    }
    return 1;
}

union VECTOR4V { qwordV q; float f[4]; };
union VECTOR4V;
// VU vector wrapper.
struct VU_VECTOR { qwordV q; VU_VECTOR() {} VU_VECTOR(qwordV a) { q = a; } VU_VECTOR(const VECTOR4V &v); };
// VU vector wrapper constructible from a 4-float vector.
struct VU_VECTOR4 { qwordV q; VU_VECTOR4() {} VU_VECTOR4(qwordV a) { q = a; } VU_VECTOR4(const VECTOR4F &v) { q = *(qwordV *)&v; } };
// Float broadcast into a VU0 register on construction.
struct VU_FLOAT { qwordV q; VU_FLOAT(float f) { __asm__("qmtc2.ni %1, %0" : "=j"(q) : "r"(f)); } };
// Loads a vector from its union view.
inline VU_VECTOR::VU_VECTOR(const VECTOR4V &v) { q = v.q; }
VU_VECTOR4 operator*(const M4 &m, VU_VECTOR4 v) __asm__("__ml__FRC7MATRIX4G10VU_VECTOR4");
// Return v scaled by f.
static inline qwordV operator*(VU_FLOAT f, VU_VECTOR v)
{ qwordV r; __asm__("vmulx.xyzw %0, %2, %1x" : "=j"(r) : "j"(f.q), "j"(v.q)); return r; }
// GIF packing wrappers taking references (as the original's inline members did), the 12.4 fixed-point screen
// coordinate with its offset, and the clip-to-screen scale ((g + 0.5) * size + 0.5).
static inline void SetStq(GIFS *pgifs, const float &s, const float &t, const float &q) { pgifs->PackSTQ(s, t, q); }
// Pack an RGBA register value into the GIF stream.
static inline void SetRgba(GIFS *pgifs, const int &rgba) { pgifs->PackRGBA(rgba); }
// Pack an XYZ register value into the GIF stream.
static inline void SetXyz(GIFS *pgifs, const int &x, const int &y, const int &z) { pgifs->PackXYZ(x, y, z); }
// Convert g plus a pixel offset to 12.4 fixed point.
static inline int IFix(const float &g, const int &off) { return ((int)g + off) << 4; }
// Map g to screen space: (g + 0.5) * s + 0.5.
static inline float Screen(const float &g, const float &s) { return (g + 0.5f) * s + 0.5f; }

// Called from the main loop: draws the captured frame as textured quads (one per enabled draw flag), each placed by
// the world matrix of one of the two child objects (the split/flip transition). (inferred)
// The three quads: 0 (placer 0) needs drawQuad0 and drawQuad2, 1 (placer 1) drawQuad1, 2 (placer 0, other UVs)
// drawQuad0 without drawQuad2.
// Matching notes: unsigned loop counters (the target uses sltiu); the loop-1 counter is reused for the placer (i = quad & 1);
// the `GIFS *pgifs = &D_002DEA00;` local at the top of the j-loop body orders the address set-ups like the target.
// Also known as: DrawAnimatedOverlayFrame (upstream readable / Sly 1 name; DrawAnimatedOverlayFrame__FPv).
extern "C" void DrawFrameCaptureScreen(CAPTUREV *capture)
{
    M4 placerMatrices[2];
    float invW;
    unsigned int i;
    int quad, nextQuad;
    switch (capture->state) {
    case CAPTURE_GRAB:
        CaptureFramebufferToTexture(capture);
        SetFrameCaptureState(capture, CAPTURE_SHOWING);
        break;
    case CAPTURE_SHOWING:
    case CAPTURE_HOLDING:
        break;
    default:
        return;
    }
    D_002DEA4C->unknown30 &= ~1;
    if (!D_002DEA4C->unknown24)
        D_002DEA4C->unknown24 = D_002DEA34;
    TEX *texture = &D_002DECD0;
    D_002DEA00.EndDmaCnt();
    D_002DEA00.AddDmaRefs(0x25, D_002DF4B0);
    D_002DEA00.AddDmaCnt();
    D_002DEA00.AddPrimPack(0, 1, 0xE);
    D_002DEA00.PackAD(0x47, 0x31001);
    D_002DEA00.PackAD(6, ((texture->unknown10 & 0x1FF) << 5) | (((texture->unknown0 >> 16) & 0x3F) << 14) | 0x6A8000000UL);
    D_002DEA00.PackAD(0x14, 0x61);
    D_002DEA00.PackAD(0x3F, 0);
    D_002DEA00.PackAD(8, 0x3800080000AUL);
    for (i = 0; i < 2; i++) {
        M4 matrix;
        GetRelativeXfMatrix(&matrix, 0, capture->rootTransform, capture->placers[i] + 0x40);
        placerMatrices[i] = matrix;
    }
    for (quad = 0; quad < 3; quad = nextQuad) {
        switch (quad) {
        case 0:
            nextQuad = 1;
            if (!capture->drawQuad0 || !capture->drawQuad2)
                continue;
            break;
        case 1:
            nextQuad = 2;
            if (!capture->drawQuad1)
                continue;
            break;
        case 2:
            nextQuad = 3;
            if (!capture->drawQuad0 || capture->drawQuad2)
                continue;
            break;
        }
        {
            i = quad & 1;
            unsigned int j;
            D_002DEA00.AddPrimPack(0x14, 3, 0x512);
            nextQuad = quad + 1;
            for (j = 0; j < 4; j++) {
                VECTOR4V vertex;
                GIFS *pgifs = &D_002DEA00; // one GIF packet pointer for the three per-vertex calls
                VU_VECTOR4 pos = placerMatrices[i] * VU_VECTOR4(D_002BD3B0[j]);
                VU_VECTOR4 posClip = D_002BD410.m * pos;
                vertex.q = posClip.q;
                Force__FPv(&vertex);
                invW = 1.0f / vertex.f[3];
                vertex.q = VU_FLOAT(invW) * VU_VECTOR(vertex);
                Force__FPv(&invW);
                Force__FPv(&vertex);
                vertex.f[0] = Screen(vertex.f[0], 512.0f);
                vertex.f[1] = Screen(vertex.f[1], 224.0f);
                Force__FPv(&vertex);
                SetStq(pgifs, invW * (D_002BD3F0[j * 2] + D_002BD398[i * 2]),
                       invW * (D_002BD3F0[j * 2 + 1] + D_002BD39C[i * 2]), invW);
                SetRgba(pgifs, 0x80808080);
                SetXyz(pgifs, IFix(vertex.f[0], 0x700), IFix(vertex.f[1], 0x790), 0);
            }
        }
    }
}

// Splice entry: starts the transition with the animation segment oid (found from the capture; it runs on real time,
// LO flag 0x100) and the sound cue to wait for (-1: none); all quads drawn. Nothing happens if the segment is missing.
// Also known as: StartAnimatedOverlay (upstream readable / Sly 1 name; StartAnimatedOverlay__FP2LOii).
extern "C" void StartFrameCapture(CAPTUREV *capture, int oid, int soundCue)
{
    CAPTUREV *segment;
    capture->soundCue = soundCue;
    capture->drawQuad2 = 1;
    capture->drawQuad1 = 1;
    capture->drawQuad0 = 1;
    segment = (CAPTUREV *)FindSwObject__FP2SWi3OIDP2LO(&g_sw, 1, oid, capture);
    if (segment) {
        SetTaggedFlags(&segment->unknown40, 0x100);
        ReplaceCurrentAseg__FP4ASEGP3ALOffiPP5ASEGA(segment, capture, 0.0f, 1.0f, 4, &capture->segmentAnim);
        SetFrameCaptureState(capture, CAPTURE_STARTED);
    }
}

// ---- Data owned by this TU, in address order (docs/guides/tu-merging.md "Data"). Defined after the functions
// so no function sees a definition (keeps the code as matched); the namespaced declarations above refer to these.
CAPTUREV *D_002DD644;

// ---- framecapture's .data in address order (tu-merging.md "Phase 6: one link mode"): one block after the functions (definition order is emission order); the declarations above are what the functions see.
SNIP D_002BD380[2] = {{0x10, 0x5F9, 0x158}, {0x10, 0x5FA, 0x15C}};
extern "C" {
/* The UV offsets: one float[6], read through D_002BD398 and D_002BD39C (its second word, an address name). */
float D_002BD398[6] = {0.0f, 0.0f, 0.25f, 0.0f, 0.0f, 0.0f};
VECTOR4F D_002BD3B0[4] = {{-0.5f, 0.5f, 0.0f, 1.0f}, {-0.5f, -0.5f, 0.0f, 1.0f}, {0.5f, 0.5f, 0.0f, 1.0f}, {0.5f, -0.5f, 0.0f, 1.0f}};
float D_002BD3F0[8] = {0.0f, 0.0f, 0.0f, 0.21875f, 0.25f, 0.0f, 0.25f, 0.21875f};
M4F D_002BD410 = {{1.0f, 0.0f, 0.0f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f}};
}
