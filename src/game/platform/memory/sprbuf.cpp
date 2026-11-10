// sprbuf.cpp -- double-buffered scratchpad transfers (LoadSprBuf/SaveSprBuf) and the two SPRBUF descriptors.
// The EE scratchpad (16 KB at 0x70000000) is split into two 8 KB halves: while code works on the current half, the
// next one is filled by toSPR DMA (LoadSprBuf) and a finished half is written back by fromSPR DMA (SaveSprBuf).
// SwapSprBuffers waits for the fill and makes the next half current. Entry points: LoadSprBuf, SaveSprBuf,
// SwapSprBuffers. TU 0x201620..0x201D78 (5 functions, address order).
// SLY2_ALIAS: __static_initialization_and_destruction_0=StaticInit_sprbuf
// SLY2_ALIAS: _GLOBAL_$I$*=func_00201D58
#include "sly2/slotheap.h"
#include "sly2/hw.h"

typedef char *va_list;
#define va_start(ap, last) (ap = ((char *)__builtin_next_arg(last) - (__builtin_args_info(2) >= 8 ? 0 : (8 - __builtin_args_info(2)) * 8)))
#define va_arg(ap, T) (*(T *)((ap += 8) - 8))
#define va_end(ap)
typedef unsigned long ulong;

/* One transfer of a SPRBUF: its size and where its data starts (a DMA tag qword precedes it). */
struct SBE { int size; ulong *scratchpad; };   /* one loaded piece: its size and scratchpad address */
typedef char assert_SBE_size[sizeof(SBE) == 8 ? 1 : -1];

extern "C" void *func_002333B4(void *pv, int b, unsigned int cb) __asm__("memset");   /* memset */

/* A scratchpad buffer: its DMA tag list, its scratchpad address, up to eight transfers and a state
   (1 loading, 2 current, 3 saving). */
struct SPRBUF
{
    ulong *dmaTags;     /* +0x00 DMA tag list (uncached address) */
    char *scratchpad;   /* +0x04 scratchpad address of this half */
    SBE pieces[8];      /* +0x08 where each loaded piece landed */
    int pieceCount;     /* +0x48 transfers */
    int state;          /* +0x4C 1 loading, 2 current, 3 saving */
    // Zero the scratchpad buffer record.
    SPRBUF() { func_002333B4(this, 0, sizeof(*this)); }
};
typedef char assert_SPRBUF_size[sizeof(SPRBUF) == 0x50 ? 1 : -1];

struct DMAC { int chcr; int pad[31]; int sadr; };
extern "C" {
int D_002C3C5C = 0;          /* index of the current buffer */
extern DMAC *g_pdcFromSpr;        /* fromSPR channel */
extern DMAC *g_pdcToSpr;        /* toSPR channel */
void sceDmaSync(DMAC *pch, int a, int b);
void sceDmaSend(DMAC *pch, int a);
void sceDmaRecv(DMAC *pch);
}
extern ulong D_003DD380[16], D_003DD400[16];   /* the two tag lists, defined after D_003DD2C0 (address order) */

// The first scratchpad double buffer (the scratchpad's first 8 KB, DMA tags D_003DD380 in uncached space).
static inline SPRBUF MakeFirst() { SPRBUF b; b.scratchpad = (char *)SLY2_HW_SCRATCHPAD; b.dmaTags = (ulong *)SLY2_UNCACHED((sptr)D_003DD380); return b; }
// The second scratchpad double buffer (the second 8 KB, DMA tags D_003DD400 in uncached space).
static inline SPRBUF MakeSecond() { SPRBUF b; b.dmaTags = (ulong *)SLY2_UNCACHED((sptr)D_003DD400); b.scratchpad = (char *)(SLY2_HW_SCRATCHPAD + 0x2000); return b; }

// 0x2019F0 StaticInit_sprbuf (218 instructions) and its _GLOBAL_$I$ stub func_00201D58: generated
// by g++ for the SPRBUF pair below, then the six engine slot pools.
SPRBUF D_003DD2C0[2] = { MakeFirst(), MakeSecond() };
/* the two tag lists (0x80 bytes each, up to stepguard's .bss), read uncached: 64-aligned (a cache line), which is
 * what puts the first at 0x3DD380 after D_003DD2C0 ends at 0x3DD360 */
ulong D_003DD380[16] __attribute__((aligned(64))), D_003DD400[16] __attribute__((aligned(64)));

SLY2_INSTANTIATE_SLOT_POOLS()

// Queues the next scratchpad half to be filled: the varargs are (size, source) pairs ending with size 0 (a null
// source reserves space without loading it). Lays the pieces out 16-aligned with a DMA tag before each, builds the
// toSPR tag list and starts the transfer.
void LoadSprBuf(int size, ...)
{
    SPRBUF *buffer = &D_003DD2C0[1 - D_002C3C5C];
    int count = 0;
    char *dest;
    char *source;
    va_list args;
    va_start(args, size);
    dest = buffer->scratchpad;
    if (buffer->state == 3)
        sceDmaSync(g_pdcFromSpr, 0, 0);
    buffer->state = 1;
    source = va_arg(args, char *);
    while (size) {
        int nextSize;
        char *nextSource;
        size = (size + 15) & ~15;
        nextSize = va_arg(args, int);
        dest += 16;
        buffer->pieces[count].scratchpad = (ulong *)dest;
        buffer->pieces[count].size = size;
        dest += size;
        nextSource = va_arg(args, char *);
        if (source) {
            buffer->dmaTags[count * 2] = (ulong)(size >> 4) | ((ulong)(nextSource ? 3 : 0) << 28) | ((ulong)(unsigned int)source << 32);
        }
        source = nextSource;
        size = nextSize;
        count++;
    }
    buffer->pieceCount = count;
    g_pdcToSpr->chcr |= 0x40;
    g_pdcToSpr->sadr = ((int)buffer->scratchpad & 0x0FFFFFFF) | 0x80000000;
    sceDmaSend(g_pdcToSpr, (int)buffer->dmaTags & 0x0FFFFFFF);
    va_end(args);
}

// Writes the current scratchpad half back: the varargs are destination addresses, null-terminated, one for each of
// the last loaded pieces. Patches their DMA tags and starts the fromSPR transfer.
void SaveSprBuf(void *dest, ...)
{
    SPRBUF *buffer = &D_003DD2C0[D_002C3C5C];
    void *dests[8];
    int count = 0;
    int destIndex;
    DMAC *channel;
    va_list args;
    buffer->state = 3;
    sceDmaSync(g_pdcFromSpr, 0, 0);
    va_start(args, dest);
    while (dest) {
        dests[count++] = dest;
        dest = va_arg(args, void *);
    }
    destIndex = 0;
    for (count = buffer->pieceCount - count; count < buffer->pieceCount - 1; count++, destIndex++)
        buffer->pieces[count].scratchpad[-2] = (ulong)(buffer->pieces[count].size >> 4) | ((ulong)1 << 28) | ((ulong)(int)dests[destIndex] << 32);
    channel = g_pdcFromSpr;
    buffer->pieces[count].scratchpad[-2] = (ulong)(buffer->pieces[count].size >> 4) | ((ulong)7 << 28) | ((ulong)(int)dests[destIndex] << 32);
    {
        channel->chcr &= ~0x40;
        channel->sadr = ((int)buffer->scratchpad & 0x0FFFFFFF) | 0x80000000;
        sceDmaRecv(channel);
    }
    buffer = &D_003DD2C0[1 - D_002C3C5C];
    if (buffer->state == 3)
        buffer->state = 0;
    va_end(args);
}

// Waits for the pending fill, then makes the other half current (state 2).
extern "C" void SwapSprBuffers(void)
{
    sceDmaSync(g_pdcToSpr, 0, 0);
    D_002C3C5C = 1 - D_002C3C5C;
    D_003DD2C0[D_002C3C5C].state = 2;
}
