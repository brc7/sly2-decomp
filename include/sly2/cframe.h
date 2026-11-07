/* include/sly2/cframe.h -- CFrame, the Splice environment frame, as object/proxy.cpp uses it (moved out of
 * sly2/basic.h, types/views-3).
 * Views still separate: splice/frame.cpp and splice/eval.cpp define their own CFrame (same layout: count, GC link word,
 * parent count, four parent pointers, binding list, visited flag), with members defined in frame.cpp
 * (FFindBinding__6CFrame..., ~CFrame) and their own inline/out-of-line counted-pointer forms (SPO/SPZ, CVarbPtr), which
 * are part of their codegen. Unifying the three is the next step (see docs/guides/tu-merging.md, "Views unified"). */
#ifndef SLY2_CFRAME_H
#define SLY2_CFRAME_H
#include "sly2/basic.h"
#include "sly2/gc.h"

/* CFrame: a Splice environment frame: the variable bindings of one scope and up to four parent frames searched after
   it. Layout from src/game/object/proxy/ReleaseProxySpare__FP5PROXYP10PROXYSPARE.cpp (2 copies in Lilly ports) */
class CFrame : public CGcObj<CFrame>
{
public:
      int m_cpframeParent;   /* number of parent frames */
      CRefPtr<CFrame> m_apframeParent[4];   /* the parent frames */
      CRefPtr<CVarb> m_pvarb;   /* first variable binding of this scope */
      int m_fVisited;   /* visited mark of a lookup or GC walk (inferred) */
    void RemoveParent(CFrame *pframeParent) __asm__("CFrame_RemoveParent");
    void AddParent(CFrame *pframeParent) __asm__("CFrame_AddParent");   /* from LoadProxyFromBrx__FP2SWP2SO3CID3OIDiP18CBinaryInputStreamP14CLoadCollector.cpp */
};

#endif /* SLY2_CFRAME_H */
