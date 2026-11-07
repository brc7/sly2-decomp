/* include/sly2/sidebag.h -- CSidebag, a Splice object's side bag: the script variables attached to an object (BASIC's
 * scriptVariables, sly2/basic.h) as a list of bindings (sly2/varb.h). Members in splice/sidebag.cpp.
 *
 * A side bag is a GC node (sly2/gc.h's CGcObj<CSidebag>: count, link word) with the first binding (0xC bytes, the
 * fifth engine slot pool). Its binding link releases inline (CRefOwner<CVarb>, sly2/sp.h): deleting a bag deletes
 * its first binding when that was the last reference (CONFIRMED by match: CSidebag_CloneTo's release). splice/gc.cpp
 * keeps its own view (it instantiates the real CRefPtr template). */
#ifndef SLY2_SIDEBAG_H
#define SLY2_SIDEBAG_H
#include "sly2/varb.h"
#include "sly2/sp.h"

class CSidebag : public CGcObj<CSidebag>
{
public:
      CRefOwner<CVarb> m_pvarb;   /* 0x08 first binding */

      CRef AssignSidebagBinding(int sym, CRef *pref);
      int FindSidebagBinding(int sym, CRef *pref);
      void SetBinding(CVarb *pvarb) { m_pvarb = pvarb; }
};
typedef char assert_CSidebag_size[sizeof(CSidebag) == 0xC ? 1 : -1];   /* the sidebag pool's slot size */
/* emitted by splice/gc.cpp; declared so that includers call them and emit no copy */
template <> void CGcObj<CSidebag>::Unlink();
template <> CSidebag *CGcObj<CSidebag>::PtNew();

extern "C" CRef CSidebag_AddBinding(CSidebag *psidebag, int sym, CRef *pref);

#endif /* SLY2_SIDEBAG_H */
