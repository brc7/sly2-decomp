/* include/sly2/gc.h -- the Splice garbage collector: CGc (g_gc, splice/gc.cpp) and CGcObj<T>, the base of the six
 * collected node kinds, with the per-kind pool bookkeeping and the per-kind GC operations.
 *
 * Every collected node starts with {reference count, link word}; the link word holds the node's prev/next neighbours
 * on its kind's list as (index, block) handles into the kind's block table, a root flag and the mark bit. The six
 * kinds and their slot pools (sly2/slotheap.h; the pool order is the static initializer's):
 *
 *   kind       T          pool                         list words (s_ptFirst..)  block table
 *   pair       CPair      CSlotPool<0x14, 0x2000>      0x2DD280                  0x452098
 *   binding    CVarb      CSlotPool<0x18, 0x800>       0x2DD290                  0x4520C0
 *   frame      CFrame     CSlotPool<0x28, 0x200>       0x2DD2A0                  0x4520E8
 *   proc       CProc      CSlotPool<0x1C, 0x800>       0x2DD2B0                  0x452110
 *   sidebag    CSidebag   CSlotPool<0xC, 0xC8>         0x2DD2C0                  0x452138
 *   method     CMethod    CSlotPool<0x14, 0x100>       0x2DD2D0                  0x452160
 *
 * Linkonce copies (docs/guides/tu-merging.md, "Linkonce copies by family"): the members below that are declared in
 * the class and defined after it without `inline` are what g++ 2.95 emitted once per kind in .gnu.linkonce.t.<symbol>
 * (0x254F00..0x259DE4): the constructor (0x2550A0 for pairs), PushFront (0x254F00), Unlink (0x2550F8), PtNew
 * (0x256B48), the empty pre-collection hook ClearMarks (0x2574E0..), the sweep FreeGarbage (0x257510..), Shutdown
 * (0x257E88..) and MakeRoot (0x259A88, 0x258178). Being template members they are emitted per instance and are never
 * inlined into callers, which is what the original shows. splice/gc.cpp instantiates them all over its views of the six
 * node types (PtNew runs T's own inline constructor, so T's view decides what it stores); calls from other files use
 * the mangled names as C symbols, as before (an identifier starting with __as__ cannot be declared: g++ reads it as
 * operator=, so those callers use an asm label). See docs/guides/tu-merging.md, "The Splice GC pool family".
 * CONFIRMED by match: the code of every instance. HYPOTHESIS: the names (CGcObj is the name of Lilly's port; the
 * member names are ours), that PushFront/PtNew/... are members rather than free templates (the mangled names would
 * differ, the code would not), and the static data members: each kind's four list words and its block table are
 * separate small objects in .sbss/.bss (commons in the original, which is what template static data members are);
 * here they are declared only; src/game/app/commons.cpp defines them (explicit specializations, in the original's
 * allocation order, with the other commons: docs/guides/tu-merging.md, "Phase 9: the commons file").
 *
 * g++ 2.95 looks names in template bodies up when it instantiates them, so g_sw (sly2/sw.h) and the node types need
 * to be complete only in the files that instantiate the members. */
#ifndef SLY2_GC_H
#define SLY2_GC_H
#include "sly2/slotheap.h"

/* placement new (PtNew constructs the node in the slot it allocates) */
inline void *operator new(unsigned int, void *pv) { return pv; }

struct CPair;
struct CVarb;
class CFrame;
struct CProc;
struct CSidebag;
struct CMethod;

/* The collector (splice/gc.cpp, g_gc at 0x2DD598). The mark stacks are allocated for each collection. */
struct CGc;
extern "C" CGc *CGc_ctor(CGc *pgc);
struct CGc
{
      /* 0x00 */ unsigned int fMark;  /* current mark, flipped by every collection */
      /* 0x04 */ int cFrame;          /* mark stacks: frames to visit (count, entries) */
      /* 0x08 */ CFrame **apFrame;
      /* 0x0C */ int cNode;           /* nodes (pairs) to visit */
      /* 0x10 */ CPair **apNode;
      /* 0x14 */ int cClo;            /* closures (procedures) to visit */
      /* 0x18 */ CProc **apClo;

      CGc() { CGc_ctor(this); }
      void MarkLiveObjects();
      void Collect();
};
extern CGc g_gc;

/* Each kind's slot pool (sly2/slotheap.h). */
template <class T> struct CGcPool;
template <> struct CGcPool<CPair> { typedef CSlotPool<0x14, 0x2000> POOL; };
template <> struct CGcPool<CVarb> { typedef CSlotPool<0x18, 0x800> POOL; };
template <> struct CGcPool<CFrame> { typedef CSlotPool<0x28, 0x200> POOL; };
template <> struct CGcPool<CProc> { typedef CSlotPool<0x1C, 0x800> POOL; };
template <> struct CGcPool<CSidebag> { typedef CSlotPool<0xC, 0xC8> POOL; };
template <> struct CGcPool<CMethod> { typedef CSlotPool<0x14, 0x100> POOL; };

/* CGcObj<T>: the header of every collected Splice node of kind T (reference count and GC link word) and the
   kind's pool bookkeeping and GC operations. */
template <class T>
class CGcObj
{
public:
      int m_cref;                   /* 0x00 reference count */
      unsigned m_iPrev : 13;        /* 0x04 link word: previous node (index, block) */
      unsigned m_ipagePrev : 2;     /*      block of the previous node */
      unsigned m_fRoot : 1;         /*      on the root list (MakeRoot) */
      unsigned m_iNext : 13;        /*      next node (index, block) */
      unsigned m_ipageNext : 2;     /*      block of the next node */
      unsigned m_fMark : 1;         /*      live when equal to g_gc.fMark */

      CGcObj();
      ~CGcObj() { Unlink(); }
      void Release()
      {
            if (--m_cref == 0)
            {
                  delete (T *)this;
            }
      }
      /* a node's slot goes back to its kind's pool (its destructor has unlinked it) */
      void operator delete(void *pv) { FreeSlotheapSlot(Pslotheap(), pv); }
      void PushFront(T **pptHead);
      void Unlink();
      void MakeRoot();
      static T *PtNew();
      static void ClearMarks();
      static void FreeGarbage();
      static void Shutdown();

      /* the kind's bookkeeping (see above): all nodes newest first, blocks in use, the sweep's recycle cursor, the
         root list, the block table */
      static T *s_ptFirst;
      static unsigned int s_cpage;
      static T *s_ptRecycle;
      static T *s_ptRoot;
      static T *s_aptPage[4];

      typedef typename CGcPool<T>::POOL POOL;
      static SLOTHEAP *Pslotheap() { return &POOL::s_pool.slotheap; }
      /* the (index, block) handle of a node; (0, 0) for none or a node outside the pool */
      static void GetHandle(T *pt, int *pi, int *pipage)
      {
            unsigned int ipage;
            for (ipage = 0; ipage < s_cpage; ipage++)
            {
                  if (pt >= s_aptPage[ipage] && pt < s_aptPage[ipage] + POOL::c)
                  {
                        *pi = pt - s_aptPage[ipage];
                        *pipage = ipage;
                        return;
                  }
            }
            *pi = 0;
            *pipage = 0;
      }
      static T *PtFromHandle(unsigned int i, unsigned int ipage)
      {
            if (i == 0 && ipage == 0)
                  return 0;
            return s_aptPage[ipage] + i;
      }
};

/* 0x2550A0 (pairs): a new node has no references, goes to the front of its kind's list and takes the current mark. */
template <class T> CGcObj<T>::CGcObj()
{
      m_cref = 0;
      PushFront(&s_ptFirst);
      m_fMark = g_gc.fMark;
}

/* 0x254F00 (pairs): push this node on a list of its kind (the node list or the root list). */
template <class T> void CGcObj<T>::PushFront(T **pptHead)
{
      int i, ipage, iHead, ipageHead;
      T *ptHead;
      GetHandle((T *)this, &i, &ipage);
      ptHead = *pptHead;
      if (ptHead)
      {
            ((CGcObj<T> *)ptHead)->m_iPrev = i;
            ((CGcObj<T> *)ptHead)->m_ipagePrev = ipage;
      }
      GetHandle(*pptHead, &iHead, &ipageHead);
      m_iNext = iHead;
      m_ipageNext = ipageHead;
      *pptHead = (T *)this;
}

/* 0x2550F8 (pairs): take this node off whichever list it is on (moving the recycle cursor past it). */
template <class T> void CGcObj<T>::Unlink()
{
      CGcObj<T> *pgcPrev = PtFromHandle(m_iPrev, m_ipagePrev);
      CGcObj<T> *pgcNext = PtFromHandle(m_iNext, m_ipageNext);
      if (this == s_ptRecycle)
            s_ptRecycle = (T *)pgcNext;
      if (pgcPrev)
      {
            pgcPrev->m_iNext = m_iNext;
            pgcPrev->m_ipageNext = m_ipageNext;
      }
      else if (this == s_ptFirst)
      {
            s_ptFirst = (T *)pgcNext;
      }
      else if (this == s_ptRoot)
      {
            s_ptRoot = (T *)pgcNext;
      }
      if (pgcNext)
      {
            pgcNext->m_iPrev = m_iPrev;
            pgcNext->m_ipagePrev = m_ipagePrev;
      }
}

/* 0x259A88 (frames), 0x258178 (sidebags): move this node to the root list, which the collector marks from. */
template <class T> void CGcObj<T>::MakeRoot()
{
      Unlink();
      PushFront(&s_ptRoot);
      m_fRoot = 1;
}

/* 0x256B48 (pairs): a new node. When the pool has no free slot, collect first (unless the world forbids it and the
   pool may still grow), and if that freed nothing, add a block (at most four: s_aptPage). */
template <class T> T *CGcObj<T>::PtNew()
{
      if (Pslotheap()->pslotFree == 0)
      {
            if (!g_sw.fUnk3a14 || s_cpage >= 4)
                  g_gc.Collect();
            if (Pslotheap()->pslotFree == 0)
            {
                  void *pv = PvAllocSlotheapClear(Pslotheap());
                  if (s_cpage)
                        FreeSlotheapSlot(Pslotheap(), pv);
                  s_aptPage[s_cpage++] = (T *)pv;
            }
      }
      return new (PvAllocSlotheapClear(Pslotheap())) T;
}

/* 0x2574E0..0x257508: called for each kind before marking; nothing to do (flipping g_gc.fMark unmarks every node). */
template <class T> void CGcObj<T>::ClearMarks()
{
}

/* 0x257510..0x257C18: the sweep. Nodes before the recycle cursor survive (their mark is refreshed); from the cursor
   on, unmarked nodes are unlinked and their slots freed (no destructor: their references may be garbage too). */
template <class T> void CGcObj<T>::FreeGarbage()
{
      CGcObj<T> *pgc = s_ptFirst;
      while (pgc != s_ptRecycle)
      {
            CGcObj<T> *pgcNext;
            if (pgc->m_fMark != g_gc.fMark)
                  pgc->m_fMark = g_gc.fMark;
            pgcNext = PtFromHandle(pgc->m_iNext, pgc->m_ipageNext);
            pgc = pgcNext;
      }
      while (pgc)
      {
            CGcObj<T> *pgcCur = pgc;
            CGcObj<T> *pgcNext = PtFromHandle(pgcCur->m_iNext, pgcCur->m_ipageNext);
            pgc = pgcNext;
            if (pgcCur->m_fMark != g_gc.fMark)
            {
                  pgcCur->Unlink();
                  FreeSlotheapSlot(Pslotheap(), pgcCur);
            }
      }
}

/* 0x257E88..0x257FC8: forget every node of the kind and empty its pool (ShutdownSplice). */
template <class T> void CGcObj<T>::Shutdown()
{
      s_ptFirst = 0;
      s_ptRecycle = 0;
      s_ptRoot = 0;
      s_cpage = 0;
      ResetSlotheap(Pslotheap());
}

#endif /* SLY2_GC_H */
