/* DMA chain builder. vtable _vt$4DMAS @0x2CE488, vptr at +0x18. */
#ifndef DMAS_H
#define DMAS_H
#include "qw.h"

struct HEAP;
struct ALLOCREQ { int cb; int cbAlign; };       /* passed by value to the allocator */

class DMAS
{
public:
    /* 0x00 */ uchar *m_pbMax;
    /* 0x04 */ uchar *m_ab;
    /* 0x08 */ QW *m_pqwCnt;
    /* 0x0c */ uchar *m_pb;
    /* 0x10 */ int m_fPad;
    /* 0x14 */ int m_fEndPrim;
    /* 0x18    vptr */

    DMAS();
    void Clear();
    void Reset();
    void AllocSw(int cqw);
    void AllocStatic(int cqw, QW *aqw);
    void Detach(int *pcqw, QW **paqw);
    void DetachCopySw(int *pcqw, QW **paqw, QW **paqwCopy);
    void DetachCopySw(HEAP *pheap, int *pcqw, QW **paqw, QW **paqwCopy);
    void Flush();
    void AddDmaCnt();
    void AddDmaRefs(int cqw, QW *aqw);
    void AddDmaCall(QW *aqw);
    void AddDmaRet();
    void AddDmaBulk(int cqw, QW *aqw);
    void AddDmaEnd();
    void EndDmaCnt();

    virtual void EndPrim(int n);
};

extern HEAP g_heapSw __asm__("D_002DD590");
void *AllocFromStack(ALLOCREQ *preq) __asm__("AllocFromStack"); /* 0x1AE318 */
void *PvAllocCopy(HEAP *pheap, int cb, void *pv, int n); /* 0x116970 */
void CopyAqw(void *pvDst, void *pvSrc, int cqw);        /* 0x1AE528 */
#endif
