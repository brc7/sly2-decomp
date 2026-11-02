// ~CBinaryAsyncStream() is the out-of-line g++ dtor _$_18CBinaryAsyncStream (0x1325B8); calling it (rather than an inline wrapper) keeps `this` in $s6.
typedef unsigned int uint;
struct CTX;
extern "C" CTX *STACKSCOPE_ctor(CTX *);
extern "C" void STACKSCOPE_ctor_1AE4A0(CTX *, int);
struct CTX { CTX() __asm__("STACKSCOPE_ctor"); ~CTX() { STACKSCOPE_ctor_1AE4A0(this, 2); } };
struct AREQ { int cb; int align; AREQ(int c, int a) { cb = c; align = a; } };
struct K;
struct X {
    uint x0; uint x4; int x8; char opaqueC[0x110 - 0xC];
    X() { x8 = 2; Init(); }
    void Init() __asm__("func_00258248");
    bool FSet(K *k);
};
struct K { uint x0, x4, x8; char pad[0x34 - 0xC]; uint x34, x38, x3C; };
struct CBinaryAsyncStream;
extern "C" CBinaryAsyncStream *func_00132570(CBinaryAsyncStream *, int, void *);
extern "C" void func_001325B8(CBinaryAsyncStream *, int);
struct CBinaryAsyncStream { char pad[0x30]; CBinaryAsyncStream(int cb, void *pv) __asm__("func_00132570"); ~CBinaryAsyncStream(); };
struct SLOT;
struct G { int x0, x4, x8, xC; float x10, x14; char pad[0xFD50 - 0x18]; SLOT *xFD50; };
struct CLOCK;
struct JOY;
struct MPEG { char opaque[0x40]; int *end; };
struct P { char pad[0x108]; int x108; int pad10C; void *x110; int pad114; MPEG x118; };
extern "C" {
extern K D_002A3968;
extern K D_002A39A8;
extern int D_002F68F8;
extern int D_002C0268;
extern int D_002BD4C0;
extern G D_002E65E0;
extern CLOCK D_002DDED0;
extern JOY D_002DFC00;
void *func_001AE378(const AREQ &r);
int func_00132600(CBinaryAsyncStream *ps, X *px);
void CMpeg_Start(P *p, CBinaryAsyncStream *ps);
int func_0024B610(MPEG *p);
void MarkClockTick(CLOCK *p, int k);
void func_001871C0(JOY *p);
void UpdateSave__FP4SAVE(G *g);
uint GrfHandleJoyBtn(JOY *p, uint k);
void func_0014BA38(void);
void Update__5CMpeg(P *p);
void Finish__5CMpeg(P *p);
void func_00132688(CBinaryAsyncStream *ps);
}

inline bool X::FSet(K *k)
{
    if (!k) return 0;
    uint offset = k->x0 ^ k->x34;
    uint length = k->x4 ^ k->x3C;
    x8 = 2;
    x0 = offset + 0xCA0;
    x4 = length;
    return length;
}

static inline int FMatch(K *a, K *b)
{
    if (a == b) return 1;
    if (!a || !b) return 0;
    return ((a->x8 ^ a->x38) ^ (b->x8 ^ b->x38)) == 0;
}

#define FIELD_OFFSET(t,m) ((unsigned int)&(((t*)0x10000)->m)-0x10000)
typedef char AREQ_cb_Offset[FIELD_OFFSET(AREQ,cb)==0?1:-1];
typedef char AREQ_align_Offset[FIELD_OFFSET(AREQ,align)==4?1:-1];
typedef char X_x0_Offset[FIELD_OFFSET(X,x0)==0?1:-1];
typedef char X_x4_Offset[FIELD_OFFSET(X,x4)==4?1:-1];
typedef char X_x8_Offset[FIELD_OFFSET(X,x8)==8?1:-1];
typedef char X_opaqueC_Offset[FIELD_OFFSET(X,opaqueC)==12?1:-1];
typedef char K_x0_Offset[FIELD_OFFSET(K,x0)==0?1:-1];
typedef char K_x4_Offset[FIELD_OFFSET(K,x4)==4?1:-1];
typedef char K_x8_Offset[FIELD_OFFSET(K,x8)==8?1:-1];
typedef char K_x34_Offset[FIELD_OFFSET(K,x34)==52?1:-1];
typedef char K_x38_Offset[FIELD_OFFSET(K,x38)==56?1:-1];
typedef char K_x3C_Offset[FIELD_OFFSET(K,x3C)==60?1:-1];
typedef char MPEG_end_Offset[FIELD_OFFSET(MPEG,end)==64?1:-1];
typedef char P_x108_Offset[FIELD_OFFSET(P,x108)==264?1:-1];
typedef char P_x110_Offset[FIELD_OFFSET(P,x110)==272?1:-1];
typedef char P_x118_Offset[FIELD_OFFSET(P,x118)==280?1:-1];
typedef char G_x8_Offset[FIELD_OFFSET(G,x8)==8?1:-1];
typedef char G_xC_Offset[FIELD_OFFSET(G,xC)==12?1:-1];
typedef char G_x10_Offset[FIELD_OFFSET(G,x10)==16?1:-1];
typedef char G_x14_Offset[FIELD_OFFSET(G,x14)==20?1:-1];
typedef char G_xFD50_Offset[FIELD_OFFSET(G,xFD50)==64848?1:-1];
typedef char CTX_PrefixSize[sizeof(CTX)==1?1:-1];
typedef char AREQ_PrefixSize[sizeof(AREQ)==8?1:-1];
typedef char X_PrefixSize[sizeof(X)==272?1:-1];
typedef char K_PrefixSize[sizeof(K)==64?1:-1];
typedef char STRM_PrefixSize[sizeof(CBinaryAsyncStream)==48?1:-1];
typedef char MPEG_PrefixSize[sizeof(MPEG)==68?1:-1];
typedef char P_PrefixSize[sizeof(P)==348?1:-1];
typedef char G_PrefixSize[sizeof(G)==64852?1:-1];
typedef char uint_PrefixSize[sizeof(uint)==4?1:-1];
typedef char int_PrefixSize[sizeof(int)==4?1:-1];
/** @sly2-annot addr=0x1B3BD0
 * Purpose: Movie loop: open async stream, Start, per frame MarkClockTick(paused)/UpdateJoy/UpdateCd, decode,
 *   Finish
 * Info: proposed name CMpeg_Execute (T1); class CMpeg; TU mpeg
 * Confidence: high [T1 0.85] - evidence: static naming pass; sources:
 *   r5_1AE130+verdicts_tu_r3r5+verdicts_A2+names_v2-signature(verified)
 * See: docs/engine/cutscenes.md#2-fmv-playback-mpegcpp-0x1b2820-0x1b4438,
 *   docs/engine/cutscenes.md#5-key-functions, docs/formats/disc-records.md#3-static-records-in-the-elf
 * @end-sly2-annot */
extern "C" void Execute__5CMpegPv(P *p, K *k)
{
    CTX ctx;
    p->x110 = func_001AE378(AREQ(0x41FC00, 0x40));
    CBinaryAsyncStream strm(0x80000, p->x110);
    X x;
    bool ok = x.FSet(k);
    if (ok && func_00132600(&strm, &x)) {
        CMpeg_Start(p, &strm);
        while (!func_0024B610(&p->x118)) {
            int f;
            MarkClockTick(&D_002DDED0, 1);
            func_001871C0(&D_002DFC00);
            if (!D_002E65E0.xFD50) UpdateSave__FP4SAVE(&D_002E65E0);
            f = 0;
            if (FMatch(k, &D_002A3968) || FMatch(k, &D_002A39A8)) f = D_002F68F8 != 0;
            if (D_002C0268) f = 1;
            if (f) {
                int fQuit = 0;
                if (GrfHandleJoyBtn(&D_002DFC00, 0x91F)) fQuit = 1;
                if (fQuit) {
                    p->x108 = 0;
                    break;
                }
            }
            if (x.x8 == 2) {
                func_0014BA38();
                if (D_002BD4C0 != 3) break;
            }
            Update__5CMpeg(p);
        }
        if (FMatch(k, &D_002A39A8)) D_002F68F8 = 1;
        Finish__5CMpeg(p);
        func_00132688(&strm);
    }
    p->x110 = 0;
}
