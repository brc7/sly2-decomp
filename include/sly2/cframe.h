/* include/sly2/cframe.h -- CFrame, the Splice environment frame (moved out of sly2/basic.h, types/views-3). Members in
 * splice/frame.cpp.
 *
 * A frame is a GC node (sly2/gc.h's CGcObj<CFrame>: count, link word) with up to four parent frames, the bindings of
 * its scope (sly2/varb.h) and two flags (0x28 bytes, the third engine slot pool). Its links call the counted-pointer
 * members out of line: the parents' assignment is CRefPtr<CFrame>'s instance (0x256EE0), the bindings' link
 * sly2/varb.h's CVarbPtr. The destructor is declared: the original's was the implicit one, emitted once (0x257078,
 * splice/gc.cpp, whose view of the node keeps it implicit because it instantiates the real templates); every other TU
 * calls it, which is what declaring it gives (an implicit destructor would be synthesized, and inlined or emitted, in
 * each TU that deletes a frame). Owners that release a frame inline use sly2/sp.h's CRefOwner<CFrame>. */
#ifndef SLY2_CFRAME_H
#define SLY2_CFRAME_H
#include "sly2/basic.h"
#include "sly2/gc.h"
#include "sly2/varb.h"   /* before CFrame: CRefPtr<CVarb> must be varb.h's specialization */

/* CFrame: a Splice environment frame: the variable bindings of one scope and up to four parent frames searched after
   it. Layout from src/game/object/proxy/ReleaseProxySpare__FP5PROXYP10PROXYSPARE.cpp (2 copies in Lilly ports) */
class CFrame : public CGcObj<CFrame>
{
public:
      int m_cpframeParent;   /* 0x08 number of parent frames */
      CRefPtr<CFrame> m_apframeParent[4];   /* 0x0C the parent frames */
      CVarbPtr m_pvarb;   /* 0x1C first variable binding of this scope */
      int m_fVisited;   /* 0x20 set while a lookup is inside this frame (FindBindingPtr's cycle guard) */
      int m_fObjectFrame;   /* 0x24 an object's own frame (BuiltinFindBindingOrNoop climbs to it; inferred) */

      ~CFrame();   /* 0x257078 (splice/gc.cpp), see above */
      CRef AssignBinding(unsigned int sym, CRef *pref);
      int FindBinding(unsigned int sym, int fRecurse, CRef *pref);
      CRef *FindBindingPtr(unsigned int sym, int fRecurse) __asm__("FindBindingPtr__6CFrameUii");
      void SetSingleParent(CFrame *pframeParent) __asm__("CFrame_SetSingleParent");
      void AddParent(CFrame *pframeParent) __asm__("CFrame_AddParent");
      void RemoveParent(CFrame *pframeParent) __asm__("CFrame_RemoveParent");
      void SetPbinding(CVarb *pvarb) { m_pvarb = pvarb; }
};
typedef char assert_CFrame_size[sizeof(CFrame) == 0x28 ? 1 : -1];   /* the frame pool's slot size */

/* emitted by splice/gc.cpp; declared so that includers call them and emit no copy */
template <> void CGcObj<CFrame>::Unlink();
template <> CFrame *CGcObj<CFrame>::PtNew();
template <> void CGcObj<CFrame>::MakeRoot();

#endif /* SLY2_CFRAME_H */
