// mpeg.cpp -- the movie player CMpeg (one global, D_002F66B0): the queue of movies to play, Execute (open the movie's file,
// demux and decode it with libmpeg until it ends or the player skips it), the display thread that sends the decoded frames to
// the GS (with a subtitle line), the video byte queue drained to the IPU and the audio (CMpegAudio: an ADPCM / PCM stream fed to
// the IOP through CQueueOutputIop). Entry points: StartupMpeg (boot), CMpeg_QueueMovie and CMpeg_ExecuteOids (the level
// transition plays the queued movies). TU 0x1B2820..0x1B4438 (34 functions, address order).
// SLY2_ALIAS: __static_initialization_and_destruction_0=StaticInit_mpeg
// SLY2_ALIAS: _GLOBAL_$I$*=func_001B4418
#include "sly2/slotheap.h"
#include "sly2/stackscope.h"
#include "sly2/clock.h"
#include "sly2/save.h"
#include "gifs.h"
#include "sly2/joy.h"
#include "sly2/sndmgr.h"
#include "sly2/vtables.h"

#define FIELD_OFFSET(t,m) ((unsigned int)&(((t*)0x10000)->m)-0x10000)
/* A movie (0x40): its file key words (xor-scrambled offset / length / id). */
struct K { uint offsetScrambled, lengthScrambled, idScrambled; char pad[0x34 - 0xC]; uint offsetKey, idKey, lengthKey; };   /* each value = scrambled ^ key */
/* The subtitle shown over a movie: its text id and the time window. */
struct SUBT { int unknown0; int idText; LM window; };
/* A byte queue (0x20); +0x10 is how many bytes it can take. */
struct CByteQueue { int a[4]; union { unsigned int ucb; int cb; }; int cb14; int a18[2]; };
typedef char CByteQueue_Size[sizeof(CByteQueue)==0x20?1:-1];
/* The byte queues' output (no vtable of its own in the ELF: nothing defines its virtuals). */
class CQueueOutput {
public:
    virtual int WriteBytes(int cb, char *pb);   /* slot 1 */
    virtual int IsAsyncDrain();               /* slot 2 */
};
/* The audio output to the IOP (0x30; the vptr at +0). Defined before CQueueOutputIpu, whose vtable comes out first. */
class CQueueOutputIop : public CQueueOutput {
public:
    virtual int IsAsyncDrain();   /* key function: CQueueOutputIop's vtable comes out here */
    virtual int WriteBytes(int cb, char *pb) __asm__("CQueueOutputIop_WriteBytes");
    int format;                 /* 0x10: ADPCM (interleaved per channel), else PCM */
    int sampleRate;             /* (inferred: passed on to snd_StartMovieSound) */
    int channelCount;
    int interleave;             /* bytes per channel block */
    int iopBuffer;              /* IOP buffer */
    int bufferSize;             /* its size */
    int writeOffset, readOffset;    /* ring cursors in the IOP buffer */
    int freeBytes, queuedBytes;     /* how many bytes it can take / holds */
    int totalSent;
    void Reset();
};
typedef char CQueueOutputIop_Size[sizeof(CQueueOutputIop)==0x30?1:-1];
/* The video output to the IPU (FeedMpegVideoDecoder's local, built by hand). */
class CQueueOutputIpu : public CQueueOutput {
public:
    virtual int IsAsyncDrain();   /* key function: CQueueOutputIpu's vtable comes out here */
    virtual int WriteBytes(int cb, char *pb) __asm__("CQueueOutputIpu_WriteBytes");
};
/* The audio stream header record. */
struct MPEGAUDIOREC { int a[2]; int format, sampleRate, channelCount, interleave; int pad[4]; };
struct CMpegAudio {
    int state;                  /* 0 off, 1 reading the header, 2 filling, 3 playing */
    int id;
    MPEGAUDIOREC rec;           /* 0x08 */
    CByteQueue qHeader;         /* 0x30 */
    CByteQueue qData;           /* 0x50 */
    CQueueOutputIop out;        /* 0x70 */
    int savedVolume2, savedVolume10;    /* the saved master volumes (channels 2 and 0x10) */
};
typedef char CMpegAudio_Size[sizeof(CMpegAudio)==0xA8?1:-1];
/* libmpeg's decoder state (its first words are the picture size and frame count). */
struct MPEG { int width; int height; int frameCount; char opaque[0x40 - 0xC]; int *end; };
struct CBinaryAsyncStream;
/* The player (D_002F66B0). */
struct CMPEG {
    union { unsigned int un; int n; };  /* queued movies */
    K ae[4];                    /* 0x004 */
    SUBT *psubt;                /* 0x104 */
    int fRepeat;                /* 0x108 */
    union { float t10C; int n10C; };    /* 0x10C: when the frame was built */
    char *pb;                   /* 0x110: the 0x41FC00-byte work buffer */
    CBinaryAsyncStream *pstrm;  /* 0x114 */
    MPEG mpeg;                  /* 0x118 */
    char pad15C[0x160 - 0x15C];
    CByteQueue qVideo;          /* 0x160 */
    CMpegAudio audio;           /* 0x180 */
    char pad228[0x23C - 0x228];
    int sema;                   /* 0x23C */
    volatile int stage;         /* 0x240: display thread stage */
    int pad244;                 /* 0x244: the ELF's next object is at +0x248 */
    CMPEG();
};
typedef char CMPEG_x104[FIELD_OFFSET(CMPEG,psubt)==0x104?1:-1];
typedef char CMPEG_x118[FIELD_OFFSET(CMPEG,mpeg)==0x118?1:-1];
typedef char CMPEG_x240[FIELD_OFFSET(CMPEG,stage)==0x240?1:-1];
typedef char CMPEG_size[sizeof(CMPEG)==0x248?1:-1];

/* 0x002F66B0: the player. Its constructor (inlined into StaticInit_mpeg) only sets the audio output's vtable (the
   member's implicit constructor; the IPU output's vtable is stored by hand in FeedMpegVideoDecoder). */
inline CMPEG::CMPEG() { }
CMPEG D_002F66B0;

// 0x1B42D0 StaticInit_mpeg and its _GLOBAL_$I$ stub func_001B4418: generated by g++ for the D_002F66B0 constructor
// (inlined) and the six engine slot pools.
SLY2_INSTANTIATE_SLOT_POOLS()


// Allocation request: byte count and alignment.
struct AREQ { int cb; int align; AREQ(int c, int a) { cb = c; align = a; } };
struct DMA;
struct ThreadParam { int status; void (*entry)(void *); void *stack; int stackSize; void *gpReg; int initPriority; int currentPriority; unsigned attr; unsigned option; int waitType; int waitId; int wakeupCount; };
struct JOY;

extern "C" {
extern DMA *D_002DE598;
int D_002F68F8;            /* first declared before D_002F6900: g++ 2.95 lays plain .bss out in declaration order */
char D_002F6900[0x1000];   /* the player thread's stack (to murray's .bss at 0x2F7900) */
extern char D_002E5270[] __asm__("_gp");   /* _gp (linker-defined), not an object */
extern int D_004425DC;
extern char D_004435F0[];
extern int D_002E1E54, D_002DEDBC;
/* Two movie file references (16 hashed words, sly2/sw.h FILEREC layout; the first is the intro movie, the same record
 * as startup's D_002A3868), read as K. */
extern const unsigned int D_002A3968[16] = {0xD77E9E24, 0xEF8D7128, 0x90A6F254, 0xF139A465, 0x0183958F, 0x6D4D86AC, 0xC5C41E37,
    0xE5DD4733, 0x9BD55409, 0xDEFE997A, 0x3CCDE869, 0xE79189E6, 0xC3EC6045, 0xD77006A2, 0x90A6FA5C, 0xEFA0B12C};
extern const unsigned int D_002A39A8[16] = {0x71544345, 0x891CC168, 0x26D34E9B, 0x30120C3B, 0x285ED44F, 0xF6109257, 0xAE965CAD,
    0xC4B8B404, 0x45003150, 0xE477A414, 0xF7052120, 0x48EE9B13, 0xCCED18FA, 0x7141672D, 0x26D34691, 0x8943016C};
int D_002C0268 = 0;   /* mpeg's .data */
extern int D_002BD4C0;

int func_00234A60(int n) __asm__("WaitSema");
void SendDmaSyncGsFinish__FP10sceDmaChanP2QW(DMA *, QW *);
void RestoreGsBuffers__Fv(void);
int IsWithinLimits__FP2LMf(const LM *, float);
void SwapGsBuffers__Fv(void);
int SemaCreate__Fii(int a, int b);
void *func_002333B4(void *pv, int c, int cb) __asm__("memset");
int CreateThread(ThreadParam *ptp);
int func_002355E0(int id, void *arg) __asm__("StartThread_2355E0");
int snd_InitMovieSound(int a, int b, int c, int d, int e, int f);
char *AllocFromStack(const AREQ &r);
void *AllocClearedFromStack(const AREQ &r);
void CopyAb__FPvT0Ui(void *dst, void *src, int cb);
void func_00234CA0(int n) __asm__("FlushCache");
int sceSifSetDma(void *r, int n);
int sceSifDmaStat(int h);
void func_00232FE8(int b, int x);
int func_00233018(void);
unsigned int snd_GetTransStatus(void);
void snd_PauseAllSoundsInGroup(unsigned int m);
void StopDialog(void *p);
void ClearSubtitle(void *p);
void snd_StopAllStreams(void);
int snd_FlushSoundCommands(void);
void snd_StreamSafeCheckCDIdle(int n);
void snd_StreamSafeCdSync(int n);
int snd_GetMasterVolume(int n);
void snd_SetMasterVolume(int n, int m);
void Init__10CByteQueueiPv(CByteQueue *p, int cb, void *pv);
void snd_ResetMovieSound(void);
void Reset__10CByteQueue(CByteQueue *p);
void snd_CloseMovieSound(void);
void snd_ContinueAllSoundsInGroup(unsigned int m);
int FillFromMemory__10CByteQueueiPUc(CByteQueue *p, int cb, void *pb);
int DrainToOutput__10CByteQueueiP12CQueueOutput(CByteQueue *p, int cb, void *pout);
void FreeDrain__10CByteQueuei(CByteQueue *p, int n);
void snd_StartMovieSound(int a, int b, int c, int d, int e);
void PushScaling__5CFontff(void *pf, float sx, float sy);
void PopScaling__5CFont(void *pf);
unsigned int GetFreeMemorySize(int n);
int OpenBasFromFileKey(CBinaryAsyncStream *ps, void *px);
int sceMpegIsEnd(MPEG *p);
void MarkClockTick(CLOCK *p, int k);
void UpdateJoy__FP3JOY(JOY *p);
void UpdateSave__FP4SAVE(SAVE *g);
uint HandleJoyButtons(JOY *p, uint k);
void UpdateCd__Fv(void);
void Close__18CBinaryAsyncStream(CBinaryAsyncStream *ps);
int sceMpegInit(void);
void sceMpegCreate(MPEG *p, void *pb, int cb);
void sceMpegAddStrCallback(MPEG *p, int a, int b, void *fn, CMPEG *s);
void sceMpegAddCallback(MPEG *p, int a, void *fn, CMPEG *s);
int sceMpegGetPicture(MPEG *p, void *pb, int c);
int func_00234A40(int n) __asm__("SignalSema");
int func_0024B590(MPEG *p) __asm__("sceMpegReset");
int sceMpegDelete(MPEG *p);
void Spool__18CBinaryAsyncStream(CBinaryAsyncStream *t);
int sceMpegDemuxPss(MPEG *p, int pb, int n);
void Skip__18CBinaryAsyncStreami(CBinaryAsyncStream *t, int r);

void MpegDisplayThread(void);
int CQueueOutputIop_Init(CQueueOutputIop *p, MPEGAUDIOREC *pa);
void StageInterleavedIopAudio(CQueueOutputIop *p, int cb, char *pb);
int CQueueOutputIop_SendBytes(CQueueOutputIop *p, int b, char *c);
void CQueueOutputIop_Update(CQueueOutputIop *p);
void Init__10CMpegAudio(CMpegAudio *p);
void Reset__10CMpegAudio(CMpegAudio *p);
void Close__10CMpegAudio(CMpegAudio *p);
int AcceptPacket__10CMpegAudioiPUc(CMpegAudio *p, int cb, unsigned char *pb);
void Update__10CMpegAudio(CMpegAudio *p);
int AcceptMpegVideoData__FP7sceMpegP16sceMpegCbDataStrP5CMpeg(void *a, struct MCB *pm, CMPEG *p);
void AcceptMpegAudioData__FP7sceMpegP16sceMpegCbDataStrP5CMpeg(void *a, struct MCB *pm, CMPEG *p);
int FeedMpegVideoDecoder__FP7sceMpegP13sceMpegCbDataP5CMpeg(int a, int b, CMPEG *p);
int OnMpegDecoderIdle__FP7sceMpegP13sceMpegCbDataP5CMpeg(void *a, void *b, CMPEG *p);
int OnMpegDecoderError__FP7sceMpegP13sceMpegCbDataP5CMpeg(void);
void BuildMpegGifs(CMPEG *p);
void DrawMpegSubtitles(CMPEG *p);
SUBT *FindMpegSubtitles(K *pe);
void Execute__5CMpegPv(CMPEG *p, K *k);
void CMpeg_Start(CMPEG *p, CBinaryAsyncStream *a);
void Update__5CMpeg(CMPEG *p);
void Finish__5CMpeg(CMPEG *p);
int DemuxStream__5CMpegi(CMPEG *p, int wait);
}

// Subtitle track lookup for a movie entry; returns none in the retail build (the caller stores the result in
// CMpeg+0x104 and DrawMpegSubtitles skips a null track). (inferred)
SUBT *FindMpegSubtitles(K *pe) {
    return 0;
}

// Sends the decoded frame (and the optional subtitle packet x) to the GS and swaps the display buffers.
static __inline__ void Draw(QW *x, CMPEG *draw)
{
    SendDmaSyncGsFinish__FP10sceDmaChanP2QW(D_002DE598, (QW *)((unsigned)draw->pb + 0x41BC00));
    if (x) SendDmaSyncGsFinish__FP10sceDmaChanP2QW(D_002DE598, x);
    SwapGsBuffers__Fv();
}
// Movie display thread: wait sema, DMA the decoded frame to GS (SendDmaSyncGsFinish), RestoreGsBuffers, time window
// check. (inferred)
void MpegDisplayThread(void)
{
    QW *x;
    for (;;) {
        D_002F66B0.stage = 0;
        func_00234A60(D_002F66B0.sema);
        D_002F66B0.stage = 1;
        SendDmaSyncGsFinish__FP10sceDmaChanP2QW(D_002DE598, (QW *)((unsigned)D_002F66B0.pb + 0x3F8E80));
        D_002F66B0.stage = 2;
        RestoreGsBuffers__Fv();
        x = 0;
        if (D_002F66B0.psubt && IsWithinLimits__FP2LMf(&D_002F66B0.psubt->window, g_clock.realTime - D_002F66B0.t10C))
            x = (QW *)((unsigned)D_002F66B0.pb + 0x41DC00);
        Draw(x, &D_002F66B0);
        Draw(x, &D_002F66B0);
    }
}

// Create mpeg semaphore and start the display thread.
void StartupMpeg()
{
    ThreadParam tp;
    D_002F66B0.sema = SemaCreate__Fii(0, 1);
    D_002F66B0.pstrm = 0;
    func_002333B4(&tp, 0, sizeof(tp));
    tp.entry = (void (*)(void *))MpegDisplayThread;
    tp.stack = D_002F6900;
    tp.gpReg = D_002E5270;
    tp.stackSize = 0x1000;
    tp.initPriority = 2;
    D_004425DC = CreateThread(&tp);
    func_002355E0(D_004425DC, 0);
}

// Init IOP output queue from audio params (0x10 = streaming mode), allocate IOP buffer (0x232EE8), Reset. (inferred)
int CQueueOutputIop_Init(CQueueOutputIop *queue, MPEGAUDIOREC *pa)
{
    queue->format = pa->format;
    queue->sampleRate = pa->sampleRate;
    queue->channelCount = pa->channelCount;
    queue->interleave = pa->interleave;
    if (queue->format == 0x10) {
        queue->bufferSize = queue->channelCount << 12;
        queue->iopBuffer = snd_InitMovieSound(0x400, 0x1000, 0x400, 0, 2, 3);
    } else {
        queue->bufferSize = 0x6000;
        queue->iopBuffer = snd_InitMovieSound(0x6000, 0, 0x400, 0, 2, 2);
    }
    if (queue->iopBuffer < 0)
        return 0;
    queue->Reset();
    return 1;
}

// Empties the IOP ring buffer: read/write cursors and counters back to the start.
void CQueueOutputIop::Reset()
{
    freeBytes = bufferSize;
    queuedBytes = 0;
    readOffset = 0;
    writeOffset = 0;
    totalSent = 0;
}

// Allocates cb bytes (alignment align) from the slot heap.
static inline char *Alloc(int cb, int align) { return AllocFromStack(AREQ(cb, align)); }
// Sly2 helper: for stereo-interleaved mode copy block into stack scratch (PvAllocStackImpl+CopyAb) before send.
// (inferred)
void StageInterleavedIopAudio(CQueueOutputIop *queue, int cb, char *pb)
{
    int ch = queue->format;
    if (ch != 0x10) return;
    if (queue->channelCount == 1) return;
    STACKSCOPE ctx;
    char *pbCur = pb;
    int chunk = queue->channelCount << 10;
    char *tmp = Alloc(chunk, 0x10);
    int cbCur = cb;
    while (cbCur) {
        int i = 0, k = 0, off;
        for (off = 0; off < chunk; off += queue->interleave) {
            CopyAb__FPvT0Ui(tmp + ((i << 10) + k * queue->interleave), pbCur + off, queue->interleave);
            i++;
            if (i >= queue->channelCount) {
                k++;
                i = 0;
            }
        }
        CopyAb__FPvT0Ui(pbCur, tmp, chunk);
        pbCur += chunk;
        cbCur -= chunk;
    }
}

// Write cb bytes into the IOP ring buffer in chunks via CQueueOutputIop_SendBytes; returns bytes written. (inferred)
extern "C" int CQueueOutputIop_WriteBytes(CQueueOutputIop *queue, int cb, char *pb)
{
    int c = cb;
    int total = 0;
    int rd;
    int n, r, avail;
    char *dst;
    if (c <= 0 || queue->freeBytes < c) c = queue->freeBytes;
    if (queue->format == 0x10) {
        c &= -(queue->channelCount << 10);
        StageInterleavedIopAudio(queue, c, pb);
    } else {
        c &= -0x400;
    }
    if (c) do {
        avail = queue->bufferSize - queue->writeOffset;
        if (c < avail) n = c; else n = avail;
        dst = pb + total;
        if (queue->format == 0x10) {
            n = 0x400;
            r = queue->totalSent / 1024 / queue->channelCount % 4;
            if (r == 0) {
                dst[1] = 6;
                dst[0x11] = 2;
            } else if (r == 3) {
                dst[0x3F1] = r;
            }
        }
        rd = CQueueOutputIop_SendBytes(queue, n, dst);
        c -= rd;
        if (!rd) break;
        total += rd;
        queue->writeOffset += rd;
        queue->writeOffset %= queue->bufferSize;
        queue->queuedBytes += rd;
        queue->freeBytes -= rd;
        queue->totalSent += rd;
        if (cb <= 0) break;
    } while (c);
    return total;
}

struct REQ { int a; int b; int c; int d; };
// FlushCache + sceSifSetDma to IOP buffer at write offset; returns count. (inferred)
int CQueueOutputIop_SendBytes(CQueueOutputIop *queue, int b, char *c)
{
    REQ req;
    int off, h, n;
    off = (queue->format == 0x10) ? 0 : queue->writeOffset;
    req.a = (int)c;
    req.b = queue->iopBuffer + off;
    req.c = b;
    req.d = 0;
    func_00234CA0(0);
    h = sceSifSetDma(&req, 1);
    while (sceSifDmaStat(h) >= 0) ;
    if (queue->format == 0x10) {
        int col, row;
        n = queue->totalSent / 1024;
        col = n % queue->channelCount;
        row = (n / queue->channelCount) % 4;
        func_00232FE8(b, (row << 10) + (col << 12));
    }
    return b;
}

// Read IOP playback position and update read/write cursors. (inferred)
void CQueueOutputIop_Update(CQueueOutputIop *queue)
{
    if (queue->format == 0x10) {
        int pos = func_00233018() * queue->channelCount;
        if (pos >= 0 && pos <= queue->bufferSize) {
            queue->readOffset = pos % queue->bufferSize;
            if (queue->readOffset == queue->writeOffset) queue->freeBytes = queue->bufferSize;
            else {
                queue->freeBytes = queue->readOffset - queue->writeOffset + queue->bufferSize;
                queue->freeBytes %= queue->bufferSize;
            }
        }
        queue->queuedBytes = queue->bufferSize - queue->freeBytes;
    } else {
        queue->freeBytes = (snd_GetTransStatus() & 0xFFFFFF) - queue->iopBuffer - queue->writeOffset + (queue->bufferSize - 0x400);
        queue->freeBytes %= queue->bufferSize;
        queue->queuedBytes = queue->bufferSize - queue->freeBytes;
        queue->readOffset = queue->writeOffset - queue->queuedBytes + queue->bufferSize;
        queue->readOffset %= queue->bufferSize;
    }
}

// CQueueOutputIop: no asynchronous drain (returns 0).
int CQueueOutputIop::IsAsyncDrain() {   /* key function */
    return 0;
}

// Kick IPU_TO DMA (0x1000B400/10/20) with up to 0x1000 bytes.
extern "C" int CQueueOutputIpu_WriteBytes(void *p, unsigned int cb, unsigned int pv)
{
    int n = cb & 0xFFFFFFF0;
    if (n > 0x1000) n = 0x1000;
    *(volatile unsigned int *)0x1000B420 = (unsigned int)n >> 4;
    *(volatile unsigned int *)0x1000B410 = pv & 0x0FFFFFFF;
    *(volatile unsigned int *)0x1000B400 = 0x101;
    __asm__ __volatile__("sync");
    return n;
}

// CQueueOutputIpu: drains asynchronously (returns 1).
int CQueueOutputIpu::IsAsyncDrain() {   /* key function */
    return 1;
}

// Stop game sound/music, allocate IOP voice resources for movie audio.
void Init__10CMpegAudio(CMpegAudio *audio)
{
    snd_PauseAllSoundsInGroup(0xFFFFFFFF);
    StopDialog(&g_sndmgr);
    ClearSubtitle(D_004435F0);
    snd_StopAllStreams();
    while (snd_FlushSoundCommands()) ;
    snd_StreamSafeCheckCDIdle(1);
    snd_StreamSafeCdSync(0);
    audio->savedVolume2 = snd_GetMasterVolume(2);
    audio->savedVolume10 = snd_GetMasterVolume(0x10);
    snd_SetMasterVolume(2, 0x400);
    snd_SetMasterVolume(0x10, 0x400);
    audio->state = 1;
    audio->id = -1;
    Init__10CByteQueueiPv(&audio->qHeader, 0x28, &audio->rec);
    Init__10CByteQueueiPv(&audio->qData, 0x18000, D_002F66B0.pb + 0x80000);
}

// reset byte queues and IOP queue.
void Reset__10CMpegAudio(CMpegAudio *audio)
{
    if (audio->state) {
        snd_ResetMovieSound();
        audio->state = 1;
        audio->id = -1;
        Reset__10CByteQueue(&audio->qHeader);
        Reset__10CByteQueue(&audio->qData);
        audio->out.Reset();
    }
}

// release IOP resources.
void Close__10CMpegAudio(CMpegAudio *audio)
{
    audio->state = 0;
    snd_CloseMovieSound();
    snd_SetMasterVolume(2, audio->savedVolume2);
    snd_SetMasterVolume(0x10, audio->savedVolume10);
    snd_ContinueAllSoundsInGroup(0xFFFFFFFF);
}

// accept demuxed audio packet: pick stream id, init queue from header, FillFromMemory.
int AcceptPacket__10CMpegAudioiPUc(CMpegAudio *audio, int cb, unsigned char *pb)
{
    int n;
    int id;
    if (audio->state == 0) return 0;
    id = pb[3];
    if (audio->id == -1) audio->id = id;
    if (audio->id != id) return 1;
    cb -= 4;
    pb += 4;
    if (audio->state == 1) {
        n = FillFromMemory__10CByteQueueiPUc(&audio->qHeader, cb, pb);
        cb -= n;
        pb += n;
        if (audio->qHeader.cb > 0) return 1;
        if (CQueueOutputIop_Init(&audio->out, &audio->rec)) audio->state = 2;
        else audio->state = 0;
    }
    if (audio->qData.cb >= cb) {
        FillFromMemory__10CByteQueueiPUc(&audio->qData, cb, pb);
        return 1;
    }
    return 0;
}

// drain byte queue to IOP per state.
void Update__10CMpegAudio(CMpegAudio *audio)
{
    switch (audio->state) {
    case 2:
        DrainToOutput__10CByteQueueiP12CQueueOutput(&audio->qData, audio->out.freeBytes, &audio->out);
        if (audio->out.freeBytes == 0) {
            if (audio->out.format == 0x10) snd_StartMovieSound(audio->out.iopBuffer, 0x1000, 0, audio->out.sampleRate, audio->out.channelCount);
            else snd_StartMovieSound(audio->out.iopBuffer, audio->out.bufferSize, audio->out.iopBuffer, 0, 0);
            audio->state = 3;
        }
        break;
    case 3:
        CQueueOutputIop_Update(&audio->out);
        DrainToOutput__10CByteQueueiP12CQueueOutput(&audio->qData, audio->out.freeBytes, &audio->out);
        break;
    }
}

/* libmpeg's stream callback data: the bytes at n8, nC of them. */
struct MCB { int a, b; int n8; unsigned int nC; };
// sceMpeg video stream callback: FillFromMemory video queue.
int AcceptMpegVideoData__FP7sceMpegP16sceMpegCbDataStrP5CMpeg(void *a, MCB *pm, CMPEG *player)
{
    if (player->qVideo.ucb < pm->nC) return 0;
    FillFromMemory__10CByteQueueiPUc(&player->qVideo, pm->nC, (void *)pm->n8);
    return 1;
}

// audio stream callback -> CMpegAudio::AcceptPacket(pmpeg+0x180).
void AcceptMpegAudioData__FP7sceMpegP16sceMpegCbDataStrP5CMpeg(void *a, MCB *pm, CMPEG *player) {
    AcceptPacket__10CMpegAudioiPUc(&player->audio, pm->nC, (unsigned char *)pm->n8);
}

// decode callback: feed IPU from video queue.
int FeedMpegVideoDecoder__FP7sceMpegP13sceMpegCbDataP5CMpeg(int a, int b, CMPEG *player)
{
    void *buf[16];
    while (player->qVideo.cb14 < 0x1000) {
        int v = DemuxStream__5CMpegi(player, 1);
        if (v == 2) {
            func_002333B4(buf, 0, 0x40);
            FillFromMemory__10CByteQueueiPUc(&player->qVideo, 0x40 - player->qVideo.cb14 % 32, buf);
            break;
        }
        if (v == 1) Update__10CMpegAudio(&player->audio);
    }
    buf[0] = SLY2_VTBL(CQueueOutputIpu);
    FreeDrain__10CByteQueuei(&player->qVideo, 0);
    DrainToOutput__10CByteQueueiP12CQueueOutput(&player->qVideo, 0, buf);
    return 1;
}

// idle callback: spool stream + audio update. (inferred)
int OnMpegDecoderIdle__FP7sceMpegP13sceMpegCbDataP5CMpeg(void *a, void *b, CMPEG *player)
{
    DemuxStream__5CMpegi(player, 0);
    Update__10CMpegAudio(&player->audio);
    return 1;
}

// sceMpeg error callback: always continues (returns 1).
int OnMpegDecoderError__FP7sceMpegP13sceMpegCbDataP5CMpeg(void) {
    return 1;
}

// build GIF packets to upload decoded RGB32 picture. (inferred)
void BuildMpegGifs(CMPEG *player)
{
    char *pb = player->pb + 0x285A80;
    int cx = player->mpeg.width / 16;
    int cy = player->mpeg.height / 16;
    int i, j;
    GIFS gifs;
    gifs.AllocStatic(0x22D6, (QW *)(player->pb + 0x3F8E80));
    gifs.AddDmaCnt();
    gifs.AddPrimPack(0, 1, 0xE);
    gifs.PackAD(0x50, 0xC180L << 36);
    gifs.PackAD(0x52, (0x8000L << 21) | 0x10);
    for (i = 0; i < cx; i++) {
        for (j = 0; j < cy; j++) {
            gifs.AddPrimPack(0, 1, 0xE);
            gifs.PackAD(0x51, ((ulong)i << 36) | ((ulong)(j << 4) << 48));
            gifs.PackAD(0x53, 0);
            gifs.AddImage(0x40);
            gifs.AddDmaRefs(0x40, (QW *)((uint)pb & 0x0FFFFFFF));
            pb += 0x400;
        }
    }
    gifs.AddPrimPack(0, 1, 0xE);
    gifs.PackAD(0x3F, 0);
    gifs.PackAD(0x61, 0);
    gifs.AddPrimEnd();
    gifs.AddDmaEnd();
    gifs.Detach(0, 0);
}

struct RGBA_RAW { uchar r, g, b, a; };
struct RGBAV {
    union { RGBA_RAW bytes; uint word; };
    // Uninitialised color.
    RGBAV() {}
    // Color from a packed word.
    RGBAV(uint value) { word = value; }
    // Copy a color.
    RGBAV(const RGBAV &c) { bytes = c.bytes; }
};
struct CFont { char opaque[0x140]; } __attribute__((aligned(16)));
struct CRichText {
    char *text, *cursor;
    CFont *current, *base;
    CFont font;
    int dirty;
    RGBAV color, setColor, baseColor, fontColor;
    CRichText(char *str, CFont *pf);
};
struct TB {
    float x, y, dx, dy;
    RGBAV color;
    int jh;
    int jv;
    void SetPos(float x, float y) __asm__("CTextBox_SetPos");
    void SetSize(float dx, float dy) __asm__("CTextBox_SetSize");
    void SetColor(RGBAV *prgba) __asm__("CTextBox_SetTextColor");
    void SetHorizontalJust(int j) __asm__("CTextBox_SetHorizontalJust");
    void SetVerticalJust(int j) __asm__("CTextBox_SetVerticalJust");
    // Text box at (x, y) of size (dx, dy) with color word `word` (default light grey).
    TB(float x, float y, float dx, float dy, uint word = 0xFFC0C0C0) {
        SetPos(x, y);
        SetSize(dx, dy);
        RGBAV rgba(word);
        SetColor(&rgba);
        SetHorizontalJust(0);
        SetVerticalJust(0);
    }
};
struct TEXT_ENTRY { int id; char *text; };
struct TEXT_TABLE { int count; TEXT_ENTRY *entries; };
extern "C" {
extern CFont *D_002DE760;
extern TEXT_TABLE D_003E1C20;
char *FindKeyedTableValue(TEXT_TABLE *p, int k);
float GetRichTextWrappedHeight(CRichText *pfont, float dx) __asm__("GetRichTextWrappedHeight");
float GetMaxLineWidth__9CRichText(CRichText *pfont);
void FillScreenRect(RGBAV rgba, GIFS *pgifs, float x0, float y0, float x1, float y1);
void Draw__9CRichTextP8CTextBoxT1P4GIFS(CRichText *pfont, TB *ptb, TB *clip, GIFS *pgifs);
}
typedef char CRichText_Size[sizeof(CRichText)==368?1:-1];
typedef char TB_Size[sizeof(TB)==28?1:-1];
typedef char GIFS_Size[sizeof(GIFS)==40?1:-1];
// Draw movie subtitle/caption text (CRichText/CFont scaling). (inferred)
void DrawMpegSubtitles(CMPEG *player)
{
    GIFS gifs;
    gifs.AllocStatic(0x200, (QW *)(player->pb + 0x41BC00));
    gifs.AddDmaCnt();
    gifs.AddPrimPack(6, 1, 0xE);
    gifs.PackAD(6, 0xAA8031800L);
    gifs.PackAD(0x14, 0x61);
    gifs.PackAD(8, 5);
    gifs.PackAD(0x42, 0x44);
    gifs.PackAD(0x47, 0x31001);
    gifs.AddPrimPack(0x116, 4, 0x4343);
    gifs.PackUV(8, 8);
    gifs.PackXYZF(0x7000, 0x7900, 0, 0);
    gifs.PackUV((player->mpeg.width << 4) | 8, (player->mpeg.height << 4) | 8);
    gifs.PackXYZF(0x9000, 0x8700, 0, 0);
    gifs.AddPrimPack(0, 1, 0xE);
    gifs.PackAD(0x61, 0);
    gifs.AddPrimEnd();
    gifs.AddDmaEnd();
    gifs.Detach(0, 0);
    if (player->psubt) {
        gifs.AllocStatic(0x200, (QW *)(player->pb + 0x41DC00));
        gifs.AddDmaCnt();
        PushScaling__5CFontff(D_002DE760, 0.8f, 0.8f);
        CRichText font(FindKeyedTableValue(&D_003E1C20, player->psubt->idText), D_002DE760);
        float dy = GetRichTextWrappedHeight(&font, 320.0f);
        float dx = GetMaxLineWidth__9CRichText(&font);
        TB tb(320.0f - dx * 0.5f, 476.8000183f - dy, dx, dy);
        RGBAV rgba;
        TB *ptb = &tb;
        ptb->jh = 1;
        rgba.word = 0x80000000;
        FillScreenRect(rgba, &gifs, tb.x, tb.y, tb.x + tb.dx, tb.y + tb.dy);
        Draw__9CRichTextP8CTextBoxT1P4GIFS(&font, ptb, 0, &gifs);
        PopScaling__5CFont(D_002DE760);
        gifs.AddPrimPack(0, 1, 0xE);
        gifs.PackAD(0x61, 0);
        gifs.AddPrimEnd();
        gifs.AddDmaEnd();
        gifs.Detach(0, 0);
    }
}

// Append a movie request (0x40-byte entry) to CMpeg queue (max 4) if valid. (inferred)
extern "C" void CMpeg_QueueMovie(CMPEG *player, K *pe)
{
    if (pe && pe->lengthScrambled != pe->lengthKey && player->un < 4) {
        player->ae[player->un++] = *pe;
    }
}

// Play every queued movie (Execute until done), unless GetFreeMemorySize<=0x41FC3F; called during level transition.
// (inferred)
extern "C" int CMpeg_ExecuteOids(CMPEG *player)
{
    int i;
    if (player->n <= 0) return 0;
    if (GetFreeMemorySize(1) <= 0x41FC3F) { player->n = 0; return 0; }
    for (i = 0; i < player->n; i++) {
        K *pe = &player->ae[i];
        player->psubt = FindMpegSubtitles(pe);
        do { Execute__5CMpegPv(player, pe); } while (player->fRepeat);
        player->psubt = 0;
    }
    player->n = 0;
    return 1;
}

// ~CBinaryAsyncStream() is the out-of-line g++ dtor _$_18CBinaryAsyncStream (0x1325B8); calling it (rather than an inline wrapper) keeps `this` in $s6.
struct X {
    uint sector; uint length; int mode; char opaqueC[0x110 - 0xC];   /* a disc file (sceCdlFILE-like, inferred) */
    // Construct in mode 2 and initialise.
    X() { mode = 2; Init(); }
    void Init() __asm__("InitFileKey");
    bool FSet(K *k);
};
struct CBinaryAsyncStream { char pad[0x1C]; int bytesLoaded; int buffer; int readOffset; int moreToRead;   /* (inferred) */ char pad2C[0x30 - 0x2C];
    CBinaryAsyncStream(int cb, void *pv) __asm__("__18CBinaryAsyncStreamPv"); ~CBinaryAsyncStream(); };
// Points the stream at a movie entry: the entry stores its disc offset and length XOR-masked with two key words
// (offsetKey/lengthKey); the sector offset is biased by 0xCA0. Returns the length (0 for no entry).
inline bool X::FSet(K *k)
{
    if (!k) return 0;
    uint offset = k->offsetScrambled ^ k->offsetKey;
    uint length = k->lengthScrambled ^ k->lengthKey;
    mode = 2;
    this->sector = offset + 0xCA0;
    this->length = length;
    return length;
}
// Whether two movie entries are the same movie (same pointer, or equal XOR-unmasked ids).
static inline int FMatch(K *a, K *b)
{
    if (a == b) return 1;
    if (!a || !b) return 0;
    return ((a->idScrambled ^ a->idKey) ^ (b->idScrambled ^ b->idKey)) == 0;
}
typedef char X_PrefixSize[sizeof(X)==272?1:-1];
typedef char K_PrefixSize[sizeof(K)==64?1:-1];
typedef char STRM_PrefixSize[sizeof(CBinaryAsyncStream)==48?1:-1];
typedef char MPEG_PrefixSize[sizeof(MPEG)==68?1:-1];
// Movie loop: open async stream, Start, per frame MarkClockTick(paused)/UpdateJoy/UpdateCd, decode, Finish.
void Execute__5CMpegPv(CMPEG *player, K *k)
{
    STACKSCOPE ctx;
    player->pb = (char *)AllocClearedFromStack(AREQ(0x41FC00, 0x40));
    CBinaryAsyncStream strm(0x80000, player->pb);
    X x;
    bool ok = x.FSet(k);
    if (ok && OpenBasFromFileKey(&strm, &x)) {
        CMpeg_Start(player, &strm);
        while (!sceMpegIsEnd(&player->mpeg)) {
            int f;
            MarkClockTick(&g_clock, 1);
            UpdateJoy__FP3JOY(&g_joy);
            if (!g_save.currentSlot) UpdateSave__FP4SAVE(&g_save);
            f = 0;
            if (FMatch(k, (K *)D_002A3968) || FMatch(k, (K *)D_002A39A8)) f = D_002F68F8 != 0;
            if (D_002C0268) f = 1;
            if (f) {
                int fQuit = 0;
                if (HandleJoyButtons(&g_joy, 0x91F)) fQuit = 1;
                if (fQuit) {
                    player->fRepeat = 0;
                    break;
                }
            }
            if (x.mode == 2) {
                UpdateCd__Fv();
                if (D_002BD4C0 != 3) break;
            }
            Update__5CMpeg(player);
        }
        if (FMatch(k, (K *)D_002A39A8)) D_002F68F8 = 1;
        Finish__5CMpeg(player);
        Close__18CBinaryAsyncStream(&strm);
    }
    player->pb = 0;
}

// init byte queue, CMpegAudio::Init, sceMpegInit/Create, add callbacks.
void CMpeg_Start(CMPEG *player, CBinaryAsyncStream *a)
{
    player->pstrm = a;
    player->n10C = 0;
    Init__10CByteQueueiPv(&player->qVideo, 0x40000, (void *)(((unsigned)player->pb + 0x245A80) | 0x20000000));
    Init__10CMpegAudio(&player->audio);
    sceMpegInit();
    sceMpegCreate(&player->mpeg, player->pb + 0x98000, 0x1ADA60);
    sceMpegAddStrCallback(&player->mpeg, 0, 0, (void *)AcceptMpegVideoData__FP7sceMpegP16sceMpegCbDataStrP5CMpeg, player);
    sceMpegAddStrCallback(&player->mpeg, 2, 0, (void *)AcceptMpegAudioData__FP7sceMpegP16sceMpegCbDataStrP5CMpeg, player);
    sceMpegAddStrCallback(&player->mpeg, 3, 0, (void *)AcceptMpegAudioData__FP7sceMpegP16sceMpegCbDataStrP5CMpeg, player);
    sceMpegAddStrCallback(&player->mpeg, 2, D_002E1E54 + 1, (void *)AcceptMpegAudioData__FP7sceMpegP16sceMpegCbDataStrP5CMpeg, player);
    sceMpegAddStrCallback(&player->mpeg, 3, D_002E1E54 + 1, (void *)AcceptMpegAudioData__FP7sceMpegP16sceMpegCbDataStrP5CMpeg, player);
    sceMpegAddCallback(&player->mpeg, 1, (void *)FeedMpegVideoDecoder__FP7sceMpegP13sceMpegCbDataP5CMpeg, player);
    sceMpegAddCallback(&player->mpeg, 4, (void *)OnMpegDecoderIdle__FP7sceMpegP13sceMpegCbDataP5CMpeg, player);
    sceMpegAddCallback(&player->mpeg, 0, (void *)OnMpegDecoderError__FP7sceMpegP13sceMpegCbDataP5CMpeg, player);
    while (!DemuxStream__5CMpegi(player, 1)) ;
    D_002DEDBC = 0;
}

// get decoded picture, BuildMpegGifs, subtitles, signal display thread.
void Update__5CMpeg(CMPEG *player)
{
    while (player->stage == 1) DemuxStream__5CMpegi(player, 0);
    if (sceMpegGetPicture(&player->mpeg, player->pb + 0x285A80, 0x5CD) < 0) return;
    if (player->mpeg.frameCount == 0) {
        player->t10C = g_clock.realTime;
        BuildMpegGifs(player);
        DrawMpegSubtitles(player);
        func_00234CA0(0);
    }
    while (player->stage) DemuxStream__5CMpegi(player, 0);
    func_00234A40(player->sema);
    Update__10CMpegAudio(&player->audio);
}

// audio reset/close, sceMpegReset/Delete.
void Finish__5CMpeg(CMPEG *player)
{
    player->pstrm = 0;
    player->n10C = 0;
    Reset__10CMpegAudio(&player->audio);
    Close__10CMpegAudio(&player->audio);
    func_0024B590(&player->mpeg);
    sceMpegDelete(&player->mpeg);
    while (player->stage)
        __asm__ __volatile__("");
    D_002DEDBC = 3;
}

// spool the stream, demux one chunk: 0 = progress, 1 = stalled (drain the queues), 2 = EOF / stalled with room left.
int DemuxStream__5CMpegi(CMPEG *player, int wait)
{
    int n, r;
    n = player->pstrm->bytesLoaded - player->pstrm->readOffset;
    while (n == 0) {
        if (!player->pstrm->moreToRead) return 2;
        Spool__18CBinaryAsyncStream(player->pstrm);
        n = player->pstrm->bytesLoaded - player->pstrm->readOffset;
        if (n == 0 && !wait) return 0;
    }
    func_00234CA0(0);
    r = sceMpegDemuxPss(&player->mpeg, player->pstrm->buffer + player->pstrm->readOffset, n);
    Skip__18CBinaryAsyncStreami(player->pstrm, r);
    if (r) return 0;
    if (player->qVideo.cb >= n && player->audio.qData.cb >= n) return 2;
    return 1;
}

// returns stream(+0x114)!=0. (inferred)
extern "C" int IsMpegActive(CMPEG *player) {
    return player->pstrm != 0;
}

