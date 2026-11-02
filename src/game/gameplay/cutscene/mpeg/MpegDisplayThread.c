struct CLOCK { float t; float dt; char pad[0x34]; float t3c; };
extern struct CLOCK D_002DDED0;
struct LM { float low, high; };
struct H { char pad[8]; struct LM window; };
struct G { char pad[0x104]; struct H *x104; char pad1[4]; float f10C; unsigned packetOffset; char pad2[0x23C - 0x114]; int x23C; volatile int stage; };
struct DMA;
struct QW { int q __attribute__((mode(TI))); };
extern struct G D_002F66B0;
extern struct DMA *D_002DE598;
int func_00234A60(int n);
void SendDmaSyncGsFinish__FP10sceDmaChanP2QW(struct DMA *, struct QW *);
void RestoreGsBuffers__Fv(void);
int FCheckLm__FP2LMf(const struct LM *, float);
void SwapGsBuffers__Fv(void);
static __inline__ void Draw(struct QW *x, struct G *draw)
{
    SendDmaSyncGsFinish__FP10sceDmaChanP2QW(D_002DE598, (struct QW *)(draw->packetOffset + 0x41BC00));
    if (x) SendDmaSyncGsFinish__FP10sceDmaChanP2QW(D_002DE598, x);
    SwapGsBuffers__Fv();
}
/** @sly2-annot addr=0x1B2828
 * Purpose: Movie display thread: wait sema, DMA the decoded frame to GS (SendDmaSyncGsFinish),
 *   RestoreGsBuffers, time window check
 * Info: proposed name MpegDisplayThread (T1); TU mpeg
 * Confidence: mid (draft) [T1 0.50] - evidence: static analysis; sources: r5_1AE130
 * See: docs/reference/functions/cutscenes.md#mpeg-0x1b2820-0x1b4438
 * @end-sly2-annot */
void MpegDisplayThread(void)
{
    struct QW *x;
    for (;;) {
        D_002F66B0.stage = 0;
        func_00234A60(D_002F66B0.x23C);
        D_002F66B0.stage = 1;
        SendDmaSyncGsFinish__FP10sceDmaChanP2QW(D_002DE598, (struct QW *)(D_002F66B0.packetOffset + 0x3F8E80));
        D_002F66B0.stage = 2;
        RestoreGsBuffers__Fv();
        x = 0;
        if (D_002F66B0.x104 && FCheckLm__FP2LMf(&D_002F66B0.x104->window, D_002DDED0.t3c - D_002F66B0.f10C))
            x = (struct QW *)(D_002F66B0.packetOffset + 0x41DC00);
        Draw(x, &D_002F66B0);
        Draw(x, &D_002F66B0);
    }
}
