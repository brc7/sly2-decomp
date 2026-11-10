/* alo.h -- ALO, the animatable level object: the class between LO and SO.
 *
 * Sly 1's "actor level object". Hierarchy LO -> ALO -> SO, as in Sly 1: every ALO*-typed pointer seen in memory
 * snapshots (SMA::paloRoot, ASEGA::rootObject, ACT::animatedObject) points to an SO subclass (name check over 121
 * RAM images). ALO adds no fields or virtual slots in the reconstruction yet (Sly 2 moved the transform to XFM and the
 * child list to SO, docs/systems/object-model.md); the empty struct keeps the animation code's Sly 1 symbols
 * (ApplyAsegCore__FP4ASEGP3ALO..., ReplaceCurrentAseg__FP4ASEGP3ALOffiPP5ASEGA) and lets an SO be passed where an
 * ALO is expected without a cast. Included by sly2/so.h and sly2/aseg.h. */
#ifndef SLY2_ALO_H
#define SLY2_ALO_H

#include "sly2/lo.h"

struct ALO : public LO
{
};

#endif /* SLY2_ALO_H */
