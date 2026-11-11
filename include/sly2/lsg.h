/* include/sly2/lsg.h -- LSG, a span of a segment clipped against solid geometry (core/math/bsp.cpp ClipSegmentBsp and
 * the edge clippers, engine/physics/clip.cpp ClipEdgeToSphere / MergeSegmentLists): entry and exit points, the
 * surface normals there, the parameters along the segment, the kind, the surfaces entered and left, the edge, and the
 * list link. One definition for both TUs, so a native build lays the pointer members out the same way in both. */
#ifndef SLY2_LSG_H
#define SLY2_LSG_H
#include "sly2/basic.h"

struct LSG
{
      qword apos[2];        /* +0x00 entry / exit point */
      qword anormal[2];     /* +0x20 the surface normals there */
      float au[2];          /* +0x40 parameters along the segment */
      LSGK lsgk;            /* +0x44 kind (clip.cpp ClipEdgeToSphere: 1) */
      SURF *apsurf[2];      /* +0x48 surface entered / left (MergeSegmentLists copies each with its end) */
      EDGE *pedge;
      int iiposSwap;
      LSG *plsgNext;
};
typedef char assert_LSG_size[sizeof(LSG) == 0x60 ? 1 : -1];

#endif /* SLY2_LSG_H */
