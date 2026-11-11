// vol.cpp -- VOL, a trigger volume: an XFM carrying a convex-cell BSP and its collision geometry.
// Role: answers "is this point / sphere inside the volume?" for gameplay (guard detectors and zones, buttons,
// splice scripts' CHECK_POINT option) and keeps the world's list of active volumes (g_sw.dlCid23).
// Entry points: VOL::LoadFromBrx (level load), IsPointInVol, IsSphereInVol, CID23::OnAdd / FreezeVol
// (enter / leave the active world). VU0 macro-mode asm.
// TU 0x227BB8..0x227FE8 (7 functions, address order).
// SLY2_ALIAS: __static_initialization_and_destruction_0=StaticInit_vol
// SLY2_ALIAS: _GLOBAL_$I$*=GlobalConstructors_vol
#include "sly2/slotheap.h"
#include "sly2/vu0.h"
#include "sly2/util.h"

// 0x227E90 StaticInit_vol and GlobalConstructors_vol: g++-generated for the six engine slot pools.
SLY2_INSTANTIATE_SLOT_POOLS()

#include "sly2/sw.h"
#include "sly2/classes.h"
typedef union { qword q; float f[4]; } VU;

/* View of VOL : XFM (fields from 0xE0; names from sly2/xfm.h's BSPC and GEOM, points as qwords). */
struct VOLV : XFM 
{
    
    struct { int numNodes; int numNodesFull; int unknown08; BSP *nodes; } bspc;   /* +0xE0 (xfm.h BSPC) */
    struct { int numPoints; qword *points; int numSurfaces; void *surfaces; int numEdges; void *edges; } geom;   /* +0xF0 */
    char pad108[8];
    qword center;       /* +0x110 centre of the bounding sphere */
    float radiusSquared; /* +0x120 squared radius of the bounding sphere (-FLT_MAX when the volume has no points) */
};
typedef char assert_VOL_radiusSquared[(unsigned int)&((VOLV *)0)->radiusSquared == 0x120 ? 1 : -1];

typedef struct MARK { int a[4]; } MARK;
struct DL;
extern const float D_002A9880;   /* defined below where the ELF has it (.rodata order) */
extern "C" {
void LoadXfmFromBrx(VOLV *p, void *pbis);
void ReadGeom__18CBinaryInputStreamP4GEOM(void *pbis, void *pgeom);   /* linker name of CBinaryInputStream::ReadGeom */
void CBinaryInputStream_ReadBspc(void *pbis, void *pgeom, void *pbspc);
qword VecMinArray(int c, qword *points);
qword VecMaxArray(int c, qword *points);
qword ConvertPosBetweenLos(int a, VOLV *p, qword q);
int FindBspLeafContainingPointFast(BSP *a, uptr b, qword q);
MARK *STACKSCOPE_ctor(MARK *pm);
void STACKSCOPE_ctor_1AE4A0(MARK *pm, int n) __asm__("_$_10STACKSCOPE");
void *AllocFromStack(SWALLOC *pmsg);
int PruneBspBySphere(void *p, uptr n, qword v, int *pc, void **ppv, float g);
void OnLoAdd(VOLV *p);
void OnLoRemove(VOLV *p);
int IsPointInVol(VOLV *p, qword q);
}
void AppendDlEntry(DL *pdl, void *pv);   /* AppendDlEntry__FP2DLPv */
void RemoveDlEntry(DL *pdl, void *pv);   /* RemoveDlEntry__FP2DLPv */

// .rodata order (docs/guides/tu-merging.md "Phase 5: data order"): a named constant is emitted where it is
// defined, a function's literals and jump tables with the function, so each constant is defined where the ELF
// has it among the functions, under an asm label (no function sees a definition under the name it reads).
namespace RO_ORDER { extern const float r_D_002A9880 __asm__("D_002A9880") = -3.40282347e+38f; }
// Loads a volume from the level stream: the XFM part, then the collision geometry and its BSP, then a bounding
// sphere around the geometry's points (centre = middle of the axis-aligned box, radius = half its diagonal). The
// sphere is the cheap early-out of IsPointInVol.
// Layout parse-verified: a parser following it reads every byte of 4 levels.
// Also known as: LoadShapeFromBrx (upstream readable / Sly 1 name; LoadShapeFromBrx__FP5SHAPEP18CBinaryInputStream).
void VOL::LoadFromBrx(CBinaryInputStream *pbis_m)
{
    VOLV *vol = (VOLV *)this;
    void *stream = (void *)pbis_m;
    LoadXfmFromBrx(vol, stream);
    ReadGeom__18CBinaryInputStreamP4GEOM(stream, &vol->geom);
    CBinaryInputStream_ReadBspc(stream, &vol->geom, &vol->bspc);
    if (vol->geom.numPoints > 0) {
        VU u;
        qword boxMin,boxMax,center; register qword half __asm__("vf1");
        qword radiusSquared;
        boxMin = VecMinArray(vol->geom.numPoints, vol->geom.points);
        __asm__("qmtc2.ni %1,%0" : "=j"(boxMin) : "r"(boxMin));
        boxMax = VecMaxArray(vol->geom.numPoints, vol->geom.points);
        half = VuFromFloat<qword>(0.5f);
        u.q = half;
        __asm__("vmulax.xyzw $ACC, %1, %2x\n\tvmaddx.xyzw %0, %3, %2x" : "=j"(center) : "j"(boxMin), "j"(half), "j"(boxMax));
        boxMin = VuSub(center, boxMin);
        vol->center = center;
        boxMin = VuDot(boxMin, boxMin);
        __asm__("qmfc2.ni %0, %1" : "=r"(radiusSquared) : "j"(boxMin));
        __asm__("sw %1,%0" : "=m"(vol->radiusSquared) : "r"(radiusSquared));
    } else {
        vol->radiusSquared = D_002A9880;
    }
}

// Returns 1 when a world-space point is inside the volume: the point is converted into the volume's local space,
// rejected if outside the bounding sphere, otherwise looked up in the volume's BSP. Used by guard detectors and
// zones, buttons and the splice option CHECK_POINT.
// Also known as: FPointInShape (upstream readable / Sly 1 name; FPointInShape__FP2LOG9VU_VECTOR).
extern "C" int IsPointInVol(VOLV *vol, qword point)
{
    qword localPoint = ConvertPosBetweenLos(0, vol, point), offset, distanceSquared;
    float distanceSquaredValue;
    int isOutsideSphere;
    offset = VuSub(localPoint, vol->center);
    distanceSquared = VuDot(offset, offset);
    distanceSquaredValue = VuToFloat(distanceSquared);
    isOutsideSphere = vol->radiusSquared < distanceSquaredValue;
    if (isOutsideSphere) return 0;
    return FindBspLeafContainingPointFast(vol->bspc.nodes, SLY2_BSP_NO_LIMIT, localPoint) != 0;
}


// Returns 1 when a sphere (world-space centre, radius) touches the volume (inferred from the BSP query's result
// codes 0/2). A radius of 1 or less degrades to IsPointInVol; otherwise the BSP is pruned against the sphere using
// a scratch list of up to 0x1555 leaves allocated in a stack scope. Called by the splice option thunk at 0x1047A0.
extern "C" int IsSphereInVol(VOLV *vol, qword center, float radius)
{
    if (radius <= 1.0f) {
        return IsPointInVol(vol, center);
    } else {
        MARK scope;
        int isInside = 0;
        SWALLOC request;
        int numLeaves;
        void *leaves;
        STACKSCOPE_ctor(&scope);
        {
            int capacity = vol->bspc.numNodesFull;
            SWALLOC *requestPtr = &request;
            capacity = MinInt(capacity, 0x1555);
            numLeaves = capacity;
            requestPtr->cb = capacity * 12;
            requestPtr->cbAlign = 0x10;
            leaves = AllocFromStack(requestPtr);
        }
        {
            int result = PruneBspBySphere((void *)vol->bspc.nodes, SLY2_BSP_NO_LIMIT, ConvertPosBetweenLos(0, vol, center), &numLeaves, &leaves, radius);
            if (result == 0 || result == 2) isInside = 1;
        }
        STACKSCOPE_ctor_1AE4A0(&scope, 2);
        return isInside;
    }
}

// Slot 23 (OnAdd): the volume enters the active world; runs the base LO hook and appends it to the world's
// volume list g_sw.dlCid23.
void CID23::OnAdd()
{
    VOLV *vol = (VOLV *)this;
    OnLoAdd(vol);
    AppendDlEntry(&g_sw.dlCid23, vol);
}

// Slot 22 (OnRemove), the counterpart of CID23::OnAdd: the volume leaves the active world; runs the base LO
// hook and unlinks it from g_sw.dlCid23.
extern "C" void FreezeVol(VOLV *vol)
{
    OnLoRemove(vol);
    RemoveDlEntry(&g_sw.dlCid23, vol);
}
