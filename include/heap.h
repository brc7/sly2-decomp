/* Memory heaps. HEAP is an abstract allocator interface (vptr at 0);
 * CLinearHeap (vtable @0x2CA550) bump-allocates 16-byte aligned blocks from a fixed buffer. */
#ifndef HEAP_H
#define HEAP_H
#include "common.h"

class HEAP
{
public:
    virtual void *PvAlloc(int cb, int n);                 /* slot 1 */
    virtual void *PvRealloc(void *pv, int cb, int n);     /* slot 2 */
    virtual void Free(void *pv);                          /* slot 3 */
};

/* The level heap (heap.cpp, vtable 0x2CA578): PvAlloc takes cb bytes from the SW allocator, PvRealloc forwards to
   PvAlloc with a fixed alignment (cbAlign, -1 = use the caller's). The virtuals keep their C names as asm labels; heap.cpp
   defines PvAlloc, the key function. Defined before CLinearHeap: heap.cpp emits CLinearHeap's vtable first. */
class CSwHeap : public HEAP
{
public:
    /* 0x04 */ int cbAlign;

    virtual void *PvAlloc(int cb, int n) __asm__("CSwHeap_PvAlloc");
    virtual void *PvRealloc(void *pv, int cb, int n) __asm__("CSwHeap_Realloc");
    virtual void Free(void *pv) __asm__("CSwHeap_Free");
};

class CLinearHeap : public HEAP
{
public:
    /* 0x04 */ int m_cbMax;
    /* 0x08 */ byte *m_pbBase;
    /* 0x0c */ int m_ib;

    CLinearHeap(int cbMax, byte *pbBase);
    virtual void *PvAlloc(int cb, int n);
    virtual void *PvRealloc(void *pv, int cb, int n);
    virtual void Free(void *pv);                          /* no-op: a bump heap never frees (0x116B28) */
};

/* Growable array of m_cb-byte elements allocated from m_pheap */
class CDynArray
{
public:
    /* 0x00 */ void *m_pv;
    /* 0x04 */ int m_c;
    /* 0x08 */ int m_cMax;
    /* 0x0c */ int m_cb;
    /* 0x10 */ int m_unk10;
    /* 0x14 */ HEAP *m_pheap;

    void Free();
    void Reserve(int c);
};

extern "C" void CopyAb(void *pvDst, void *pvSrc, uint cb) __asm__("CopyAb__FPvT0Ui");   /* memory.cpp's C++ CopyAb */
extern "C" void *memset(void *pv, int b, uint cb);
extern "C" int strlen(const char *sz);
void *PvAllocCopy(HEAP *pheap, int cb, void *pv, int n);
#endif
