// vis.cpp -- VIS (class VISZONE), a camera visibility zone: an XFM carrying a BSP volume. Each frame the camera tests its
// position against every zone (cm.cpp calls UpdateVisInside on each, sorts them, and makes the first one it is inside
// the camera's vis zone, SetCmVisZone), which lets the renderer cull by zone. Entry points: VISZONE::Init,
// LoadVisFromBrx, UpdateVisInside. TU 0x227350..0x2275B8 (6 functions, address order).
// SLY2_ALIAS: __static_initialization_and_destruction_0=StaticInit_vis
// SLY2_ALIAS: _GLOBAL_$I$*=GlobalConstructors_vis
#include "sly2/xfm.h"
#include "sly2/slotheap.h"
#include "sly2/classes.h"
#include "sly2/clock.h"

// 0x227460 StaticInit_vis and GlobalConstructors_vis: g++-generated for the six engine slot pools.
SLY2_INSTANTIATE_SLOT_POOLS()

/* The zone: classes.h VISZONE (its fields are declared there; vis.cpp is the only TU that reads them). */
typedef VISZONE VIS;

extern "C" {
void InitXfm(XFM *pxfm);
void LoadXfmXfFromBrx(XFM *pxfm, void *pbis);
int U32Read__18CBinaryInputStream(void *pbis);
void ReadGeom__18CBinaryInputStreamP4GEOM(void *pbis, void *pgeom);
void CBinaryInputStream_ReadBspc(void *pbis, void *pgeom, void *pbspc);
void LoadOptionsFromBrx__FPvP18CBinaryInputStream(void *pv, void *pbis);
qword ConvertPosBetweenLos(int a, VIS *pvis, qword pos);
int FindBspLeafContainingPointFast(BSP *pbsp, uptr n, qword pos);
void SetVisInside(VIS *vis, int isInside);
}

// Initializes the zone: XFM init, then marks the inside state unknown (-1) so the first test always records a change.
void VISZONE::Init()
{
    VIS *vis = (VIS *)this;
    InitXfm(vis);
    vis->isInside = -1;
}

// Reads a zone from the level file (vtable slot 14): transform, a 32-bit value, the volume geometry, its BSP and the
// object options. The payload layout is parse-verified; the meaning of the 32-bit value is not known.
// Also known as: LoadVolumeFromBrx (upstream readable / Sly 1 name; LoadVolumeFromBrx__FP2LOP18CBinaryInputStream).
extern "C" void LoadVisFromBrx(VIS *vis, void *stream)
{
    LoadXfmXfFromBrx(vis, stream);
    vis->grfvis = U32Read__18CBinaryInputStream(stream);
    ReadGeom__18CBinaryInputStreamP4GEOM(stream, &vis->geom);
    CBinaryInputStream_ReadBspc(stream, &vis->geom, &vis->bspc);
    LoadOptionsFromBrx__FPvP18CBinaryInputStream(vis, stream);
}

// Tests a world position (the camera's) against the zone's BSP volume, in the zone's local space, and records the
// result with SetVisInside. Called by the camera for every zone each frame.
extern "C" void UpdateVisInside(VIS *vis, qword worldPos)
{
    SetVisInside(vis, FindBspLeafContainingPointFast(vis->bspc.nodes, SLY2_BSP_NO_LIMIT, ConvertPosBetweenLos(0, vis, worldPos)) != 0);
}

// Stores the zone's inside state; on a change also stamps the time (g_clock.time) it changed.
extern "C" void SetVisInside(VIS *vis, int isInside)
{
    if (isInside != vis->isInside) {
        vis->isInside = isInside;
        vis->changeTime = g_clock.time;
    }
}
