// memory.cpp -- the memory manager: the level ("SW") heap that grows up from the end of the program image and the
// stack heap that grows down from 32 MB (allocate, allocate cleared, reallocate-copy, free all), the stack scopes
// that mark and release the stack heap (STACKSCOPE, under a critical section), the empty malloc/free/operator
// delete, and the word/quadword copy routines.
// There is no general-purpose free: level data is bump-allocated from the SW heap and dropped wholesale by FreeSw at
// level unload; per-frame scratch comes from the stack heap and is released by STACKSCOPE marks. The two heaps grow
// toward each other and GetFreeMemorySize is the gap. Allocation requests (AREQ) carry a size and an alignment.
// Entry points: StartupMemMgr, AllocFromWorldHeap and its copy/clear variants, FreeSw, AllocFromStack and
// AllocClearedFromStack, STACKSCOPE, operator new, CopyAb, CopyAqw. TU 0x1AE150..0x1AE7D0 (21 functions).
// SLY2_ALIAS: __static_initialization_and_destruction_0=StaticInit_memory
// SLY2_ALIAS: _GLOBAL_$I$*=GlobalConstructors_memory
#include "sly2/slotheap.h"
#include "sly2/stackscope.h"
#include "sly2/port.h"

// 0x1AE678 StaticInit_memory and GlobalConstructors_memory: g++-generated for the six engine slot pools.
SLY2_INSTANTIATE_SLOT_POOLS()

typedef int qword __attribute__((mode(TI)));
typedef unsigned int uint;
typedef unsigned char uchar;
/* An allocation request: size and alignment. */
struct AREQ { int size; int alignment; };
struct CRITSECT { int a[3]; };

extern char D_0047220F;           /* the last byte of the program image */
/* memory's .bss (0x2F6380..0x2F6468), defined in address order; ui/memcard also declares a few. */
uptr s_pvWorldMin;                  /* SW heap base */
uptr s_pvWorldMac;                  /* SW heap top (grows up) */
uptr s_pvStackMin;                  /* stack heap top (grows down) */
uptr s_pvStackMac;                  /* stack heap base (32 MB) */
CRITSECT s_critsectStack;               /* the heap lock */
int s_ipvStackCur;                   /* stack scope depth */
uptr s_apvStackMin[50];              /* stack heap tops of the open scopes (to ui/menu's .bss at 0x2F6468) */
extern "C" {
void func_002333B4(void *p, int c, int n) __asm__("memset");
void *AllocFromWorldHeap(AREQ *preq);
void *AllocClearedFromWorldHeap(AREQ *preq);
void *AllocFromStack(AREQ *preq);
}
void InitCritSect(CRITSECT *pcritsect);
void EnterCritSect(CRITSECT *pcritsect);
void LeaveCritSect(CRITSECT *pcritsect);
void CheckForOutOfMemory();
/* CheckForOutOfMemory as the allocators call it, with the size (an empty function in the release build). */
void CheckForOutOfMemoryCb(int cb) __asm__("CheckForOutOfMemory__Fv");
void InitStackImpl();
void FreeStackImpl();
void CopyAqw(void *pvDst, void *pvSrc, int cqw);
void CopyAb(void *pvDst, void *pvSrc, uint cb);

// Returns the bytes still free between the top of the SW heap and the bottom of the stack heap.
extern "C" int GetFreeMemorySize()
{
    return s_pvStackMin - s_pvWorldMac;
}

/* The arena the two heaps share. On the EE: from the end of the program image (the SW heap grows up from it) to the
   top of the 32 MB of RAM (the stack heap grows down from it). A port defines both first, e.g. as the bounds of a
   static buffer: SLY2_HEAP_BEGIN the lowest address (any alignment; StartupMemMgr rounds it down to 16 bytes, so
   leave 15 bytes of slack), SLY2_HEAP_END one past the highest. */
#ifndef SLY2_HEAP_BEGIN
#define SLY2_HEAP_BEGIN ((sptr)&D_0047220F)
#endif
#ifndef SLY2_HEAP_END
#define SLY2_HEAP_END 0x02000000
#endif

// Boot step: the SW heap starts at the 16-aligned end of the program image, the stack heap at 32 MB (the top of
// EE RAM) with no marks pushed; initialises the critical section the stack scopes use.
void StartupMemMgr()
{
    uptr end = SLY2_HEAP_BEGIN & ~0x0f;
    s_pvWorldMin = s_pvWorldMac = end;
    s_pvStackMin = s_pvStackMac = SLY2_HEAP_END;
    s_ipvStackCur = -1;
    InitCritSect(&s_critsectStack);
}

// Out-of-memory hook; empty in the retail build.
void CheckForOutOfMemory()
{
}

// Bump-allocates request->size bytes (rounded up to 16) from the SW (level) heap at request->alignment alignment.
// Also known as: PvAllocSwImpl (upstream readable / Sly 1 name; PvAllocSwImpl__Fi).
extern "C" void *AllocFromWorldHeap(AREQ *request)
{
    uptr pv;
    if (!request->size)
        return 0;
    CheckForOutOfMemoryCb(request->size);
    pv = (s_pvWorldMac + (request->alignment - 1)) & -request->alignment;
    s_pvWorldMac = pv + ((request->size + 15) & -16);
    return (void*)pv;
}

// AllocFromWorldHeap, then copies request->size bytes from source into the new block.
// Also known as: PvAllocSwCopyImpl (upstream readable / Sly 1 name; PvAllocSwCopyImpl__FiPv).
extern "C" void *AllocCopyFromWorldHeap(AREQ *request, void *source)
{
    AREQ m = *request;
    void *p = AllocFromWorldHeap(&m);
    if (p) CopyAb(p, source, request->size);
    return p;
}

// AllocFromWorldHeap, then zero-fills the block.
// Also known as: PvAllocSwClearImpl (upstream readable / Sly 1 name; PvAllocSwClearImpl__Fi).
extern "C" void *AllocClearedFromWorldHeap(AREQ *request)
{
    AREQ m = *request;
    void *p = AllocFromWorldHeap(&m);
    if (p) func_002333B4(p, 0, request->size);
    return p;
}

// Releases the whole SW heap (level unload): its top returns to its base.
void FreeSw()
{
    s_pvWorldMac = s_pvWorldMin;
}

// Allocates request->size bytes (rounded up to 16) from the stack heap, which grows down, at request->alignment
// alignment (PvAllocStackImpl in Sly 1 terms; this symbol was misnamed PvAllocSlot__FP8ALLOCREQ before).
// Also known as: PvAllocStackImpl (upstream readable / Sly 1 name; PvAllocStackImpl__Fi).
extern "C" void *AllocFromStack(AREQ *request)
{
    int cb = request->size;
    uptr pv = 0;
    if (cb != 0) {
        CheckForOutOfMemoryCb(cb);
        pv = (s_pvStackMin - ((request->size + 0xF) & ~0xF)) & -request->alignment;
        s_pvStackMin = pv;
    }
    return (void *)pv;
}

// Stack-heap allocation (AllocFromStack), then zero-fills the block.
// Also known as: PvAllocStackClearImpl (upstream readable / Sly 1 name; PvAllocStackClearImpl__Fi).
extern "C" void *AllocClearedFromStack(AREQ *request)
{
    AREQ m = *request;
    void *p = AllocFromStack(&m);
    if (p) func_002333B4(p, 0, request->size);
    return p;
}

// Opens a stack-heap scope: enters the memory critical section and pushes the current stack-heap position.
void InitStackImpl()
{
    EnterCritSect(&s_critsectStack);
    s_apvStackMin[++s_ipvStackCur] = s_pvStackMin;
}

// Closes a stack-heap scope: pops the saved position (freeing everything allocated since) and leaves the
// critical section.
void FreeStackImpl()
{
    s_pvStackMin = s_apvStackMin[s_ipvStackCur--];
    LeaveCritSect(&s_critsectStack);
}

/* STACKSCOPE's constructor and destructor (sly2/stackscope.h; g++ adds the destructor's in-charge test and delete). */
// STACKSCOPE constructor: opens a stack-heap scope (InitStackImpl) for the object's lifetime.
STACKSCOPE::STACKSCOPE()
{
    InitStackImpl();
}

// STACKSCOPE destructor: closes the scope (FreeStackImpl); deletes the object when asked to (flag bit 0).
// Also known as: STACKSCOPE constructor (upstream readable / Sly 1 name).
STACKSCOPE::~STACKSCOPE()
{
    FreeStackImpl();
}

// libc malloc, disabled: always returns null (the game allocates through its own heaps).
extern "C" void *malloc(unsigned int cb)
{
    return 0;
}

// libc free, disabled: does nothing.
extern "C" void free(void *pv)
{
}

// Global operator new: a zero-filled, 16-aligned block from the SW heap (freed only with the level).
/* ::operator new (g++ 2.95's symbol for it is __builtin_new). */
void *operator new(unsigned int cb) { AREQ m; m.size = cb; m.alignment = 0x10; return AllocClearedFromWorldHeap(&m); }

/* ::operator delete (g++ 2.95's symbol for it is __builtin_delete): nothing to free. */
// Global operator delete: does nothing (SW-heap blocks are released with the level).
void operator delete(void *pv)
{
}

// Copies `count` quadwords (16 bytes each; the remainder first, then four at a time).
void CopyAqw(void *dst, void *src, int count)
{
    qword *dstQwords = (qword *)dst;
    qword *srcQwords = (qword *)src;
    int remainder = count & 0x03;
    int bulk = count - remainder;
    int i = 0;
    for (; i < remainder; i++)
        *dstQwords++ = *srcQwords++;
    i = 0;
    for (; i < bulk; i += 4) {
        qword qw0 = srcQwords[0];
        qword qw1 = srcQwords[1];
        qword qw2 = srcQwords[2];
        qword qw3 = srcQwords[3];
        srcQwords += 4;
        dstQwords[0] = qw0;
        dstQwords[1] = qw1;
        dstQwords[2] = qw2;
        dstQwords[3] = qw3;
        dstQwords += 4;
    }
}

// memcpy: byte loop for unaligned arguments, word loop for 4-aligned ones, CopyAqw for 16-aligned ones.
void CopyAb(void *dst, void *src, uint size)
{
    if (((uptr)dst | (uptr)src | size) & 0x03)
    {
        uchar *dstBytes = (uchar *)dst;
        uchar *srcBytes = (uchar *)src;
        for (uint i = 0; i < size; i++)
        {
            *dstBytes++ = *srcBytes++;
        }
        return;
    }
    if (((uptr)dst | (uptr)src | size) & 0x0f)
    {
        uint *dstWords = (uint *)dst;
        uint *srcWords = (uint *)src;
        int remainder = (size / sizeof(uint)) & 0x03;
        int bulk = (size / sizeof(uint)) - remainder;
        int i = 0;
        for (; i < remainder; i++)
        {
            *dstWords++ = *srcWords++;
        }
        i = 0;
        for (; i < bulk; i += 4)
        {
            uint w0 = srcWords[0];
            uint w1 = srcWords[1];
            uint w2 = srcWords[2];
            uint w3 = srcWords[3];
            srcWords += 4;
            dstWords[0] = w0;
            dstWords[1] = w1;
            dstWords[2] = w2;
            dstWords[3] = w3;
            dstWords += 4;
        }
        return;
    }
    CopyAqw(dst, src, size >> 4);
}
