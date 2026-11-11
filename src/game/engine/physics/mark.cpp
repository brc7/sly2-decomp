// mark.cpp -- narrow-phase collision ("marking" contacts) between the parts (SOPART) of two touching solid objects.
// Role: AddSoXps pairs the parts of two object trees and dispatches by shape: sphere/sphere, sphere/BSP, BSP/BSP
// and swept vertices; the BSP markers clip edges against the other part's BSP (ClipSoBspEdges, CategorizeSegmentList,
// FindEdgePath) and append contacts (XPs) through AddXp. Also the XP list helpers and the per-frame refresh of
// contact positions and relative velocities (RecalcSwXpPos / RecalcSwXpMovement). Called from the broad phase
// (bbmark.cpp RecalcSwXpAll); the contacts feed the solver (solve.cpp, dzg.cpp).
// TU 0x1A7C18..0x1AA978 (21 functions, address order) plus the inline function behind the linkonce copy 0x2564C0
// (defined at the end).
//
// Types: the contact record XP/XPS and the BSP geometry (EDGE, SURF, BSP, LSG, from Lilly's CsbiIntersectSphereBsp
// port) are TU-private; parts, trees, objects and transforms are so.h's SOPART, SOTREE, SO and XF, the world sw.h's SW.
// Six hand-tuned functions (AddEmbeddedVertexXps, MarkSoContactsBspBsp, FindContactInsideBsp, IntersectSphereBsp,
// MarkSoContactsSphereBsp, MarkSoContactsSweptVertices) keep the per-function views and helpers they were matched
// with, each in a namespace named after it (as vec.cpp does): their matches depend on the exact helper shapes.
// SLY2_ALIAS: __static_initialization_and_destruction_0=StaticInit_mark
// SLY2_ALIAS: _GLOBAL_$I$*=GlobalConstructors_mark
#include "sly2/basic.h"
#include "sly2/xfm.h"
#include "sly2/so.h"
#include "sly2/sw.h"
#include "sly2/slotheap.h"
#include "sly2/stackscope.h"
#include "sly2/util.h"        /* NMin */
#include "sly2/vu0.h"

// 0x1AA820 StaticInit_mark and GlobalConstructors_mark: g++-generated for the six engine slot pools.
SLY2_INSTANTIATE_SLOT_POOLS()

/* A contact (XP, 0xC0; SW::slotheapXp holds them) between two parts: per side the part, its transform, the root
   object of its tree, its object (told about the contact through SO slots 54..56), the contact point in world space,
   the same point in the transform's frame (TransformXfPos) and its velocity (GetXfVelocity); then the normal (side 0
   to side 1), the separation along it, friction and elasticity (defaults D_002BFD30/D_002BFD34), the kind (4..6
   sphere/BSP face, edge, vertex; 7 sphere/sphere; 8/9 swept vertex; 12 inside; 13 BSP/BSP fallback), the two
   features that made it (an EDGE, SURF or vertex per kind), the index in its group, the list link and the relative
   velocity. Field names follow dzg.cpp's and solve.cpp's views of the same record. */
struct XPS
{
    SOPART *ppart;          /* +0x00 */
    XF *pxf;                /* +0x04 */
    SO *psoRoot;            /* +0x08 SO::physicsTree->root */
    SO *pso;                /* +0x0C */
    qword pos;              /* +0x10 */
    qword posLocal;         /* +0x20 */
    qword v;                /* +0x30 */
};
struct XP
{
    XPS axps[2];
    qword normal;           /* +0x80 */
    float g;                /* +0x90 */
    float gFriction;        /* +0x94 */
    float gElasticity;      /* +0x98 */
    int kind;               /* +0x9C */
    void *pvFeature0;       /* +0xA0 */
    void *pvFeature1;       /* +0xA4 */
    int ixp;                /* +0xA8 */
    XP *pxpNext;            /* +0xAC */
    qword vRel;             /* +0xB0 */
};
typedef char assert_XP_size[sizeof(XP) == 0xC0 ? 1 : -1];   /* CloneXpList copies 0xC0 */

/* The world position of a transform (XF::worldMatrix's translation row) as a quadword. */
#define QW_XF_POS(pxf) (*(qword *)(pxf)->worldMatrix.mat[3])


extern "C" {
float D_002BFD30 = 0.5f;    /* default contact friction (mark's .data) */
float D_002BFD34 = 0.5f;    /* default contact elasticity */
qword TransformXfPos(XF *pxfTo, XF *pxfFrom, qword pos);   /* converts a point between frames (0: world) */
qword GetXfVelocity(XF *pxf, int n, qword pos);
void RecalcSwXpMovement(SW *psw, int cxp, XP **apxp);
void AppendXpList(SW *psw, XP *pxp, XP **ppxpFirst);
int FindZapResponder(SO *pso, int grfzpk);   /* HYPOTHESIS: whether pso reacts to the zap kinds grfzpk */
extern qword g_normalZ;  /* fallback contact normal */
void AddXp(SW *psw, SOPART *ppart0, SOPART *ppart1, qword pos, qword normal, int kind, void *pvFeature0,
            void *pvFeature1, XP **ppxpFirst, float g);
}
#define FIELD_OFFSET(t, m) ((unsigned int)&(((t *)0x10000)->m) - 0x10000)
typedef char assert_SW_slotheapXp[FIELD_OFFSET(SW, slotheapXp) == 0x3D40 ? 1 : -1];
#undef FIELD_OFFSET

/* The collision geometry of a part (SOPART::meshWorld, xfm.h GEOM) and its BSP (SOPART::bspc), as in Lilly's
   IntersectSphereBsp port: an edge joins two vertices (aipos) between two surfaces (aisurf); a surface is a plane
   (normal, gDot) with its edge lists and flags; a BSP node splits by its surface. */
/* A clipped piece of an edge (ClsgClipEdgeToBsp): its two ends with the normals and the edge parameters there, the
   surfaces it enters/leaves through (null where an end lies inside), the source edge, whether the ends were swapped,
   and a list link. */
struct LSG
{
      qword apos[2];
      qword anormal[2];
      float au[2];
      LSGK lsgk;
      SURF *apsurf[2];
      EDGE *pedge;
      int iiposSwap;
      LSG *plsgNext;
};
typedef char assert_LSG_size[sizeof(LSG) == 0x60 ? 1 : -1];

/* core/math/bsp.cpp (C symbols): clip the segment pos1..pos2 to the tree (pieces inside), prune the tree to a sphere,
   find the leaf containing a point. A pbspLimit of 0x7FFFFFFF means no limit. */
extern "C" {
int ClsgClipEdgeToBsp(BSP *pbspRoot, BSP *pbspLimit, qword pos1, qword pos2, int clsgMax, LSG *alsg) __asm__("ClipSegmentBsp");
PRUNE PruneBsp(BSP *pbsp, BSP *pbspLimit, float sRadius, qword pos, int *pcbspLeft, BSP **ppbspNext) __asm__("PruneBspBySphere");
BSP *PbspPointInBsp(BSP *pbsp, BSP *pbspLimit, qword pos) __asm__("FindBspLeafContainingPoint");
}
/* A contact of a sphere with a part's hull (IntersectSphereBsp's output). */
struct CMCONTACT
{
      qword pos;
      qword plane;
      int kind;
      void *pv;
};
extern "C" void *memmove(void *dst, const void *src, int cb);

/* CategorizeClippedSegments swaps the ends of an LSG through a 16-byte aligned block on the stack (the original's VECTOR
   temporary): a qword temporary stays in a register (154 of 170 off). This form exists to reproduce the original
   codegen. */
struct VECBLK { float x, y, z; } __attribute__((aligned(16)));
#define VB(qw) (*(VECBLK *)&(qw))

// Allocates a contact (XP) between two objects at a world position and normal with separation g and kind k:
// stores both sides' local-frame positions, computes the relative movement and links the XP into *ppxp. Returns it.
// Sly 1 name: PxpAdd.
// Also known as: PxpAddSw (upstream readable / Sly 1 name; PxpAddSw__FP3SOET0P6VECTORG9VU_VECTORT3iiiPvf).
extern "C" void AddXp(SW *psw, SOPART *ppart0, SOPART *ppart1, qword pos, qword normal, int kind, void *pvFeature0,
                       void *pvFeature1, XP **ppxpFirst, float g)
{
    union { qword q; float f[4]; } u;
    XP *pxp;
    int i;
    XPS *pxps;
    XP *pxpNew;
    pxp = pxpNew = (XP *)AllocZeroedFromSlotheap(&psw->slotheapXp);
    pxpNew->axps[0].pos = pos;
    {
        qword s, r;
        __asm__("qmtc2.ni %1, %0" : "=j"(s) : "r"(-g));
        u.q = s;
        __asm__("vadda.xyzw $ACC, %1, $vf0\n\tvmaddx.xyzw %0, %2, %3x" : "=j"(r) : "j"(pos), "j"(normal), "j"(s));
        pxpNew->axps[1].pos = r;
    }
    pxpNew->normal = normal;
    for (i = 0; i < 2; i++) {
        pxps = &pxp->axps[i];
        if (i == 0)
            pxps->ppart = ppart0;
        else
            pxps->ppart = ppart1;
        if (pxps->ppart) {
            pxps->pxf = pxps->ppart->transform;
            pxps->pso = pxps->ppart->owner;
            pxps->psoRoot = pxps->pso->physicsTree->root;
        }
        pxps->posLocal = TransformXfPos(0, pxps->pxf, pxps->pos);
    }
    pxp->g = g;
    pxp->kind = kind;
    pxp->gFriction = D_002BFD30;
    pxp->gElasticity = D_002BFD34;
    pxp->pvFeature0 = pvFeature0;
    pxp->pvFeature1 = pvFeature1;
    for (i = 0; i < 2; i++) {
        SO *pso;
        pxps = &pxp->axps[i];
        pso = pxps->pso;
        if (pso)
            pso->AdjustNewXp(pxp, i);
    }
    RecalcSwXpMovement(psw, 1, &pxp);
    {
        XP *pxpLast = pxp;
        while (pxpLast->pxpNext)
            pxpLast = pxpLast->pxpNext;
        pxpLast->pxpNext = *ppxpFirst;
        *ppxpFirst = pxp;
    }
}


// Returns a whole XP list to the world's contact pool. Sly 1 name.
void FreeSwXpList(SW *world, XP *list)
{
    while (list) {
        XP *pxpNext = list->pxpNext;
        FreeSlotheapSlot(&world->slotheapXp, list);
        list = pxpNext;
    }
}

/* p + v * f.x, with f the float's bits (int) and v read through a pointer ("memory": the original reloads around it).
   The broadcast is stored into its union slot as in the original (sqc2). */
static inline qword QwAddScaledBits(qword a, qword *pb, int f)
{
    union { qword q; float f; } t;
    qword tq;
    __asm__("qmtc2.ni %1, %0" : "=j"(tq) : "r"(f));
    t.q = tq;
    __asm__("vadda.xyzw $ACC, %1, $vf0\n\tvmaddx.xyzw %0, %2, %3x" : "=j"(a) : "0"(a), "j"(*pb), "j"(t.q):"memory");
    return a;
}
/* Sphere sides (kinds 4..6 side 0, 7 and 9 side 1) put the point on the sphere: centre + normal * -/+radius; the other
   sides convert the local point back to world space. Then the separation is (pos0 - pos1) . normal. */
// For each XP in the array recomputes the world positions of its two sides from their local positions.
// Also known as: RecalcSwXpPos (upstream readable / Sly 1 name; RecalcSwXpPos__FP2SWiPP2XP).
extern "C" void RecalcSwXpPos(SW *world, int count, XP **contacts)
{
    int i;
    for (i = 0; i < count; i++) {
        XP *pxp = contacts[i];
        XPS *pxps;
        int j;
        pxps = pxp->axps;
        for (j = 0; j < 2; j++, pxps++) {
            XF *pxf = pxps->pxf;
            if (!pxf)
                continue;
            {
            int f; qword position;
            union { float f; int i; } u;
            if (j == 0) {
                switch (pxp->kind) {
                case 4: case 5: case 6:
                    u.f = -pxps->ppart->radius; f = u.i; position = QW_XF_POS(pxf);
                    break;
                default:
                    goto def;
                }
            } else {
                switch (pxp->kind) {
                case 7: case 9:
                    position = QW_XF_POS(pxf); f = *(int *)&pxps->ppart->radius;
                    break;
                default:
                    goto def;
                }
            }
            pxps->pos = QwAddScaledBits(position, &pxp->normal, f);
            continue;
            }
        def:
            pxps->pos = TransformXfPos(pxf, 0, pxps->posLocal);
        }
        {
            qword d, r, one;
            __asm__("vsub.xyzw %0, %1, %2" : "=j"(d) : "j"(pxp->axps[0].pos), "j"(pxp->axps[1].pos));
            __asm__("vmul.xyz %0, %2, %3\n\tvaddw.x %1, $vf0, $vf0w\n\tvadday.x $ACC, %0, %0y\n\tvmaddz.x %0, %1, %0z" : "=j"(r), "=&j"(one) : "j"(pxp->normal), "j"(d));
            { register float g __asm__("$4"); __asm__("qmfc2.ni %0, %1" : "=r"(g) : "j"(r)); __asm__("sw %1, %0" : "=m"(pxp->g) : "r"(g)); }
        }
    }
}


/* Each side's point velocity (GetXfVelocity), then SO slots 55 and 56 of both objects, then the relative velocity. */
// For each XP recomputes the relative velocity at the contact from both sides' point velocities.
// Also known as: RecalcSwXpMovement (upstream readable / Sly 1 name; RecalcSwXpMovement__FP2SWiPP2XP).
extern "C" void RecalcSwXpMovement(SW *world, int count, XP **contacts)
{
    int i;
    for (i = 0; i < count; i++) {
        XP *pxp = contacts[i];
        XPS *pxps;
        int j;
        pxps = pxp->axps;
        for (j = 0; j < 2; j++, pxps++) {
            if (pxps->pxf) {
                SO *pso;
                pxps->v = GetXfVelocity(pxps->pxf, 0, pxps->pos);
                pso = pxps->pso;
                pso->UnknownSlot56(pxp, j);
            }
        }
        for (j = 0; j < 2; j++) {
            SO *pso = pxp->axps[j].pso;
            if (pso)
                pso->AdjustXpVelocity(pxp, j);
        }
        qword d;
        __asm__("vsub.xyzw %0, %1, %2" : "=j"(d) : "j"(pxp->axps[0].v), "j"(pxp->axps[1].v));
        pxp->vRel = d;
    }
}


/* Clips every edge of ppart's world geometry against the BSP absp (ClsgClipEdgeToBsp) and records the source edge
   in each piece. The Sly 1 symbol says (SO *, SO *, BSP *, int, LSG *); Sly 2's takes the world and the BSP's part
   first (both unused), so it keeps the symbol as a C-linkage label. (Sly 1-derived name: ClsgClipSoAllEdges.) */
// Clips every edge of one object's part against the other part's BSP into LSG spans, each tagged with its source
// edge. Returns the span count (Sly 1 ClsgClipSoAllEdges).
int ClsgClipSoAllEdges(SW *psw, SOPART *ppartBsp, SOPART *ppart, BSP *absp, int clsgMax, LSG *alsg)
    __asm__("ClipSoAllEdges");
// Definition of ClipSoAllEdges (the C-linkage label declared above): clip each edge of ppart's world geometry against
// the BSP absp, tagging each span with its edge; returns the span count.
// Also known as: ClsgClipSoAllEdges (upstream readable / Sly 1 name; ClsgClipSoAllEdges__FP2SOT0P3BSPiP3LSG).
int ClsgClipSoAllEdges(SW *psw, SOPART *ppartBsp, SOPART *ppart, BSP *absp, int clsgMax, LSG *alsg)
{
    int ilsg = 0;
    int clsg = 0;
    int iedge = ppart->meshWorld.numEdges;
    EDGE *aedge = ppart->meshWorld.edges;
    qword *apos = (qword *)ppart->meshWorld.positions;
    for (iedge--; iedge >= 0; iedge--) {
        EDGE *pedge = &aedge[iedge];
        clsg += ClsgClipEdgeToBsp(absp, (BSP *)0x7FFFFFFF, apos[pedge->aipos[0]], apos[pedge->aipos[1]], clsgMax - clsg, &alsg[clsg]);
        for (; ilsg < clsg; ilsg++)
            alsg[ilsg].pedge = pedge;
    }
    return clsg;
}

/* Marks the surfaces of the cbsp nodes absp (grf 1), then clips the edges of each marked surface (once per surface:
   grf 2) whose other surface is marked too against ppartBsp's BSP, and clears both marks again. */
// Like ClipSoAllEdges but only for the edges of surfaces that survive BSP pruning against the other part.
// Also known as: ClsgClipSoBspEdges (upstream readable / Sly 1 name; ClsgClipSoBspEdges__FP2SOT0iP3BSPiP3LSG).
extern "C" int ClipSoBspEdges(SW *world, SOPART *bspPart, SOPART *part, int numNodes, BSP *nodes, int maxSegments, LSG *segments)
{
    int ilsg = 0;
    int clsg = 0;
    qword *apos = (qword *)part->meshWorld.positions;
    SURF *asurf = part->meshWorld.surfaces;
    int i;
    for (i = numNodes - 1; i >= 0; i--)
        nodes[i].psurf->grf |= 1;
    for (i = numNodes - 1; i >= 0; i--) {
        register SURF *psurf __asm__("$5") = nodes[i].psurf;
        EDGE *pedge;
        if ((psurf->grf & 1) == 0)
            continue;
        if (psurf->grf & 2)
            continue;
        psurf->grf |= 2;
        for (pedge = psurf->pedge; pedge; pedge = pedge->pedgeNext) {
            {
                SURF *psurfOther = asurf + pedge->aisurf[1];
                if ((psurfOther->grf ^ 1) & 1)
                    continue;
            }
            clsg += ClsgClipEdgeToBsp(bspPart->bspc.nodes, (BSP *)0x7FFFFFFF, apos[pedge->aipos[0]], apos[pedge->aipos[1]], maxSegments - clsg, &segments[clsg]);
            for (; ilsg < clsg; ilsg++)
                segments[ilsg].pedge = pedge;
        }
    }
    for (i = numNodes; --i >= 0;)
        {register SURF *psurf __asm__("$5")=nodes[i].psurf;psurf->grf &= ~3;}
    return clsg;
}

// Sorts clipped spans into categories (inside, touching, crossing) for the BSP/BSP marker. Sly 1 name:
// CategorizeAlsg. (Sly 1-derived name: CategorizeAlsg.)
// Also known as: CategorizeAlsg (upstream readable / Sly 1 name).
void CategorizeClippedSegments(int count, LSG *segments, int *numPointsOut, int *pointIndices, LSG **categorized, LSG **bothOut)
{
    LSG *plsgNone = 0;
    int cpos = 0;
    int ilsg;
    int ipos;
    LSG *plsg;

    *bothOut = 0;
    for (ilsg = 0; ilsg < count; ilsg++) {
        plsg = &segments[ilsg];
        if (plsg->apsurf[0] && plsg->apsurf[1]) {
            plsg->plsgNext = *bothOut;
            *bothOut = plsg;
        } else if (!plsg->apsurf[0] && !plsg->apsurf[1]) {
            plsg->plsgNext = plsgNone;
            plsgNone = plsg;
        } else {
            if (plsg->apsurf[0]) {
                VECBLK vecT;
                vecT = VB(plsg->apos[0]); VB(plsg->apos[0]) = VB(plsg->apos[1]); VB(plsg->apos[1]) = vecT;
                vecT = VB(plsg->anormal[0]); VB(plsg->anormal[0]) = VB(plsg->anormal[1]); VB(plsg->anormal[1]) = vecT;
                {
                    float gT = plsg->au[0]; plsg->au[0] = plsg->au[1]; plsg->au[1] = gT;
                }
                {
                    SURF *psurfT = plsg->apsurf[0]; plsg->apsurf[0] = plsg->apsurf[1]; plsg->apsurf[1] = psurfT;
                }
                plsg->iiposSwap = 1;
            }
            for (ipos = 0; ipos < cpos; ipos++) {
                if (plsg->pedge->aipos[plsg->iiposSwap] == pointIndices[ipos])
                    break;
            }
            if (ipos >= cpos) {
                pointIndices[ipos] = plsg->pedge->aipos[plsg->iiposSwap];
                cpos++;
            } else {
                plsg->plsgNext = categorized[ipos];
            }
            categorized[ipos] = plsg;
        }
    }

    for (ipos = 0; ipos < cpos; ipos++) {
        LSG **pplsg = &categorized[ipos];
        int *pipos;
        if ((*pplsg)->plsgNext)
            continue;
        for (plsg = plsgNone; plsg; plsg = plsg->plsgNext) {
            if (plsg->pedge->aipos[0] == pointIndices[ipos] || plsg->pedge->aipos[1] == pointIndices[ipos])
                goto next;
        }
        (*pplsg)->plsgNext = *bothOut;
        *bothOut = *pplsg;
        memmove(pplsg, pplsg + 1, (cpos - ipos) * 4 - 4);
        pipos = &pointIndices[ipos];
        memmove(pipos, pipos + 1, (cpos - ipos) * 4 - 4);
        ipos--;
        cpos--;
    next:;
    }
    *numPointsOut = cpos;
}

/* Breadth-first search from surface isurfStart to isurfEnd over the edges of the pieces plsgFirst..(plsgNext): fills
   aplsg with the path's pieces (end first) and returns their count, 0 if there is none. Visited surfaces get grf 4
   (cleared again). Kept as the C-linkage definition of its literal symbol. */
struct EDGEPATHNODE { EDGEPATHNODE *pnodeParent; LSG *plsg; int isurf; };
// Breadth-first search (64 nodes) for a chain of spans connecting two positions along shared edges; used to find
// edge/edge contacts. Sly 1 name: CplsgFindEdgePath.
// Also known as: CPathAlsg (upstream readable / Sly 1 name; CPathAlsg__FP4GEOMiiP3LSGPP3LSG).
extern "C" int FindEdgePath__FP4SURFiiP3LSGPP3LSG(SURF *surfaces, int startSurface, int endSurface, LSG *first, LSG **pathOut)
{
    EDGEPATHNODE anode[64];
    int inode;
    int cnode;
    int c;
    inode = 0;
    anode[0].pnodeParent = 0;
    anode[0].isurf = startSurface;
    anode[0].plsg = 0;
    cnode = 1;
    for (; inode < cnode; inode++) {
        EDGEPATHNODE *pnode = &anode[inode];
        LSG *plsg;
        for (plsg = first; plsg; plsg = plsg->plsgNext) {
            EDGE *pedge = plsg->pedge;
            int isurf;
            if (pedge->aisurf[0] == pnode->isurf)
                isurf = pedge->aisurf[1];
            else if (pedge->aisurf[1] == pnode->isurf)
                isurf = pedge->aisurf[0];
            else
                continue;
            if (isurf == endSurface) {
                c = 0;
                pathOut[c++] = plsg;
                while (pnode->plsg) {
                    pathOut[c++] = pnode->plsg;
                    pnode = pnode->pnodeParent;
                }
                goto done;
            }
            {
            SURF *psurf = surfaces + isurf;
            if (psurf->grf & 4)
                continue;
            psurf->grf |= 4;
            }
            {
                EDGEPATHNODE *pnodeNew = &anode[cnode++];
                pnodeNew->isurf = isurf;
                pnodeNew->pnodeParent = pnode;
                pnodeNew->plsg = plsg;
            }
        }
    }
    c = 0;
done:
    for (inode = 0; inode < cnode; inode++)
        surfaces[anode[inode].isurf].grf &= ~4;
    return c;
}

// .rodata order (docs/guides/tu-merging.md "Phase 5: data order"): a named constant is emitted where it is
// defined, a function's literals and jump tables with the function, so each constant is defined where the ELF
// has it among the functions, under an asm label (no function sees a definition under the name it reads).
namespace RO_ORDER { extern const float r_D_002A6AD0 __asm__("D_002A6AD0") = -3.40282347e+38f; }
// Hand-tuned (gcse table size, register pins, struct copies): keeps the per-function views and helpers it was
// matched with, in a namespace of its own (see the file header).
namespace M_func_001A87F0 {
// AddEmbeddedVertexXps: for each of `count` vertices of mesh A's point table, pick the best-ranked contact plane from the vertex's
// surface list (heads[i], NODEs chained through ->next), then emit contacts through AddXp:
//   - kind 1: the vertex itself (position = points.data[indices[i]]) with the best plane's normal and the vertex's own plane distance;
//   - kind 2: every other list vertex of the same chain that lies closer to the best plane than the vertex does.
// A surface is ranked by the smallest signed distance (dot(normal.xyz, p.xyz) - normal.w) from its plane to the vertex (orig) and to
// the far end of every edge in the chain (min); the surface with the largest `min` above D_002A6AD0 wins, and bestOrig is its `orig`.
// SURFACE.flags bit 8 marks surfaces already ranked (set while scanning, cleared again before the contacts are emitted).
//
// How the match was found (w2lane8 v4 + wave-3 lane 4; all natural C++, VU asm only):
//  - SCALAR/VECTOR have default constructors that call InitVuTemp / DefaultConstructVector (the engine's 16-byte vector initialisers); PlaneDist's two SCALAR
//    locals therefore become the ctor calls + the dead dot/one stores at sp+0x10.. of every inlined expansion.
//  - `float d` and `NODE *m` live at function scope and are shared by the edge loop and the emit loop: cse1 canonicalises the `if (n)` loop guard to the
//    lowest-numbered pseudo of its equivalence class, so loops over `n` (declared after `head`) test `head` and loops over `m` test `m`.
//  - `pt` is read before `nn` is constructed, so it is live across the ctor call (callee-saved GPR) which also decides the s0/s1 split between n and m.
//  - `plane` is read before the `flags & 8` test, which lets reorg fill the bnez delay slot from before.
//  - AddXp takes its float in the middle of the argument list (EABI: float args go to $f12 either way); the position only matters for the order gcc
//    evaluates the arguments in.
//  - PlaneDist takes the plane as a VU_VECTOR4 by value: the by-value class parameter adds a few TImode copies that survive cse1 (and vanish at
//    register allocation), which makes gcse's expression table 77 buckets instead of 73. PRE numbers its reaching registers in hash-bucket order, so
//    &normal (sp+0x50) now gets its pseudo before i+1, and reload gives them the spill slots 0xD0 / 0xD4 as in the original.
typedef int qword __attribute__((mode(TI)));
struct SCALAR;
struct VECTOR;
extern SCALAR *InitVuTemp(SCALAR *) __asm__("InitVuTemp");
extern "C" inline VECTOR *DefaultConstructVector(VECTOR *);   /* defined after its callers (end of file) */
// VU temporary initialised through InitVuTemp.
struct SCALAR { qword q; SCALAR() { (void)InitVuTemp(this); } };
// Vector default-constructed through DefaultConstructVector.
struct VECTOR { qword q; VECTOR() { (void)DefaultConstructVector(this); } };
// Wrap a qword as a VU_VECTOR4.
struct VU_VECTOR4 { qword q; VU_VECTOR4(qword a) { q = a; } };
struct W;
struct B;
struct XP;
struct POINTS { int count; qword *data; };
struct A { char pad[0x90]; POINTS points; };
struct SURFACE { qword plane; char pad10[8]; int flags; };
struct EDGE { unsigned short ends[2]; };
struct NODE { char pad[0x10]; qword point; char pad20[0x50 - 0x20]; SURFACE *surface; EDGE *edge; int reverse; NODE *next; };

extern const float D_002A6AD0 __asm__("D_002A6AD0");   /* defined below where the ELF has it (.rodata order) */
void AddXp(W *, A *, B *, qword, float, qword, int, int, int, XP **) __asm__("AddXp");


/* signed distance of pos from plane: dot(plane.xyz, pos.xyz) - plane.w, via VU0 macro mode */
static inline float PlaneDist(VU_VECTOR4 plane, qword pos)
{
    SCALAR dot, one;
    qword r, unit;
    float f;
    __asm__("vmul.xyz %0,%2,%3\n\tvaddw.x %1,$vf0,$vf0w\n\tvadday.x $ACC,%0,%0y\n\tvmaddaz.x $ACC,%1,%0z\n\tvmsubw.x %0,%1,%2w"
            : "=j"(r), "=&j"(unit) : "j"(plane.q), "j"(pos));
    dot.q = r;
    one.q = unit;
    /* gcc supplies the qmfc2 for the "r" qword operand; the extra unused "j"(r) is allocation-only: it keeps r's preferred class VU (sqc2 store, not sq). */
    __asm__("mtc1 %1,%0" : "=f"(f) : "r"(dot.q), "j"(r));
    return f;
}

// Ranks the surfaces near embedded vertices (largest minimum depth above a threshold) and emits vertex/face
// contacts for them through AddXp. (inferred)
// Also known as: MarkEmbeddedVertices (upstream readable / Sly 1 name; MarkEmbeddedVertices__FP3SOET0PiiPP3LSGPP2XPT5).
extern "C" void AddEmbeddedVertexXps(W *w, A *a, B *b, int count, int *indices, NODE **heads, XP **out)
{
    float d;
    NODE *m;
    for (int i = 0; i < count; i++) {
        POINTS *positions = &a->points;
        qword *data = positions->data;
        int index = indices[i];
        NODE *head = heads[i];
        SURFACE *best = 0;
        float bestMin = D_002A6AD0;
        float bestOrig;
        qword bestNormal;
        qword position = data[index];
        NODE *n;
        for (n = head; n; n = n->next) {
            SURFACE *s = n->surface;
            qword plane = s->plane;
            if (!(s->flags & 8)) {
                s->flags |= 8;
                float orig = PlaneDist(plane, position);
                float min = orig;
                for (m = head; m; m = m->next) {
                    d = PlaneDist(plane, data[m->edge->ends[1 - m->reverse]]);
                    if (d < min) min = d;
                }
                if (bestMin < min) { bestNormal = plane; best = s; bestMin = min; bestOrig = orig; }
            }
        }
        for (n = head; n; n = n->next) n->surface->flags &= ~8;
        VECTOR normal;
        normal.q = bestNormal;
        AddXp(w, a, b, position, bestOrig, normal.q, 1, (int)&a->points.data[index], (int)best, out);
        for (m = head; m; m = m->next) {
            d = PlaneDist(bestNormal, m->point);
            if (d < bestOrig) {
                qword pt = m->point;
                VECTOR nn;
                nn.q = bestNormal;
                AddXp(w, a, b, pt, d, nn.q, 2, (int)m->edge, (int)best, out);
            }
        }
    }
}
} // namespace M_func_001A87F0

// Hand-tuned (gcse table size, register pins, struct copies): keeps the per-function views and helpers it was
// matched with, in a namespace of its own (see the file header).
namespace M_MarkSoContactsBspBsp {
// MarkSoContactsBspBsp: edge/edge and vertex/face contacts between two clipped shapes.
// ClsgClipSoAllEdges + ClipSoBspEdges clip each shape's edges against the other, CategorizeClippedSegments sorts each side's LSGs, AddEmbeddedVertexXps adds the
// vertex contacts of both sides; then every free LSG of side 0 either follows the FindEdgePath edge path of side 1 (closest points of the
// two edges via ClosestPointsOfLines, kind 3) or, when no side-1 LSG shares a surface with its edge, contacts the nearer endpoint against the
// farther plane (kind 1). With no new contact and one side empty, a kind-13 contact at the shape position is added.
// LSG / EDGE / SURF layouts and names as in Lilly's matched ClsgClipEdgeToBsp port (src/game/core/math/bsp/ClipSegmentBsp.cpp).
// How it matched (w4lane4 -> w5lane2):
//  - Separate Sly-1-style local arrays (one frame base each, shared by gcse with the call arguments); the side-1 loop indexes &aalsg[1][i].
//  - Lilly's QwMix(VuFloat(1 - t), a, VuFloat(t), b) lerps reproduce the dead VU_FLOAT stores at 0x6900.
//  - The four arrays aaipos[0/1] and aaplsg[0/1] are gcse PRE reaching registers that get spill slots in pseudo order = gcse hash-bucket
//    order, so the slot order depends on the expression table size ((insns / 2) | 1, needs 219). The by-value VV / VU_FLOAT helper
//    parameters (struct copies that survive cse1 and vanish at register allocation) give exactly that count; GFromVf taking a VU_FLOAT
//    in Length and PlaneDist was the last step (215 -> 219).
typedef int qword __attribute__((mode(TI)));
// Qword wrapper convertible to and from qword.
struct VV { qword q; VV() {} VV(const qword &v) { q = v; } operator qword() const { return q; } };
struct SCALAR;
extern SCALAR *InitVuTemp(SCALAR *) __asm__("InitVuTemp");
// VU temporary initialised through InitVuTemp.
struct SCALAR { qword q; SCALAR() { (void)InitVuTemp(this); } };
struct SCENE;
struct CONTACT;
struct SPACE { char pad0[0x70]; QWROW position; };
struct EDGE { unsigned short aipos[2], aisurf[2]; EDGE *pedgeNext; EDGE *pedgeOtherNext; };
struct SURF { QWROW plane; EDGE *pedge; EDGE *pedgeOther; unsigned int grf; int ipos; };
struct SHAPE { SPACE *space; void *owner; char pad8[0x8C]; QWROW *apos; int pad98; SURF *asurf; int cedge; EDGE *aedge; };
struct NODE { unsigned int left, right; SURF *psurf; };
struct LSG { QWROW apos[2], anormal[2]; float au[2]; int lsgk; SURF *apsurf[2]; EDGE *pedge; int iiposSwap; LSG *plsgNext; };
struct XP { char pad0[0x9C]; int kind; void *first, *second; int padA8; XP *next; };

extern QWROW g_normalZ __asm__("g_normalZ");
int ClsgClipSoAllEdges(SCENE *, SHAPE *, SHAPE *, NODE *, int, LSG *) __asm__("ClipSoAllEdges");
int ClipSoBspEdges(SCENE *, SHAPE *, SHAPE *, int, NODE *, int, LSG *) __asm__("ClipSoBspEdges");
void CategorizeClippedSegments(int, LSG *, int *, int *, LSG **, LSG **) __asm__("CategorizeClippedSegments__FiP3LSGPiT2PP3LSGT4");
void AddEmbeddedVertexXps(SCENE *, SHAPE *, SHAPE *, int, int *, LSG **, CONTACT **) __asm__("AddEmbeddedVertexXps");
int FindEdgePath__FP4SURFiiP3LSGPP3LSG(SURF *, int, int, LSG *, LSG **) __asm__("FindEdgePath__FP4SURFiiP3LSGPP3LSG");
int ClosestPointsOfLines(qword, qword, qword, qword, float *, QWROW *) __asm__("ClosestPointsOfLines");
void AddXp(SCENE *, SHAPE *, SHAPE *, VV, float, VV, int, void *, void *, CONTACT **) __asm__("AddXp");


// Same idiom as sly2/vu0.h VuSub; kept local because the shared template changes MarkSoContactsBspBsp (10/365 instructions off).
static inline VV VuSub(VV a, VV b) { qword r; __asm__("vsub.xyzw %0,%1,%2" : "=j"(r) : "j"(a), "j"(b)); return r; }

struct VU_FLOAT { qword qw; };
// Broadcast a float into a VU0 register.
static inline VU_FLOAT VuFloat(float g)
{
    VU_FLOAT v;
    __asm__("qmtc2.ni %1,%0" : "=j"(v.qw) : "r"(g));
    return v;
}
// Return qw1 * g1 + qw2 * g2.
static inline VV QwMix(VU_FLOAT g1, VV qw1, VU_FLOAT g2, VV qw2)
{
    qword qw;
    __asm__("vmulax.xyzw $ACC,%1,%3x\n\tvmaddx.xyzw %0,%2,%4x" : "=j"(qw) : "j"(qw1), "j"(qw2), "j"(g1.qw), "j"(g2.qw));
    return qw;
}
/* gcc supplies the qmfc2 for the "r" operand; the repeated "j" operand is allocation-only (keeps the value VU class). */
static inline float GFromVf(VU_FLOAT v) { float g; __asm__("mtc1 %1,%0" : "=f"(g) : "r"(v.qw), "j"(v.qw)); return g; }
// Length of the xyz part of v as a float.
static inline float Length(VV v)
{
    VV d, one;
    VU_FLOAT len;
    __asm__("vmul.xyz %0,%2,%2\n\tvaddw.x %1,$vf0,$vf0w\n\tvadday.x $ACC,%0,%0y\n\tvmaddz.x %0,%1,%0z" : "=j"(d), "=j"(one) : "j"(v));
    __asm__("vsqrt $Q,%1x\n\tvwaitq\n\tvaddq.x %0,$vf0,$Q" : "=j"(d) : "j"(d));
    len.qw = d;
    return GFromVf(len);
}
/* signed distance of point from plane: dot(plane.xyz, point.xyz) - plane.w */
static inline float PlaneDist(VV point, VV plane)
{
    SCALAR dot, one;
    qword r, unit;
    __asm__("vmul.xyz %0,%2,%3\n\tvaddw.x %1,$vf0,$vf0w\n\tvadday.x $ACC,%0,%0y\n\tvmaddaz.x $ACC,%1,%0z\n\tvmsubw.x %0,%1,%2w"
            : "=j"(r), "=&j"(unit) : "j"(plane.q), "j"(point));
    dot.q = r;
    one.q = unit;
    VU_FLOAT g;
    g.qw = dot.q;
    return GFromVf(g);
}
// Closest points of the lines (a, b) and (c, d): parameter into *au, normal into *normal.
static inline void Closest(VV a, VV b, VV c, VV d, float *au, QWROW *normal)
{
    (void)ClosestPointsOfLines(a, b, c, d, au, normal);
}

// Contacts between two BSP parts: clips edges both ways, categorises the spans, then adds embedded-vertex
// contacts, edge paths and closest-point contacts.
// Also known as: MarkBspBsp (upstream readable / Sly 1 name; MarkBspBsp__FP3SOET0P5SHAPEiP6BSPSELPP2XPPP7CONTACT).
extern "C" void MarkSoContactsBspBsp(SCENE *scene, SHAPE *a, SHAPE *b, int nodeCount, NODE *nodes, NODE *last, CONTACT **out)
{
    int aclsg[2];
    LSG aalsg[2][128];
    int acpos[2];
    int aaipos[2][128];
    LSG *aaplsg[2][128];
    LSG *aplsgBoth[2];
    LSG *aplsgPath[32];
    float au[2];
    QWROW normal;
    QWROW apos[2];
    CONTACT *old = *out;
    aclsg[0] = ClsgClipSoAllEdges(scene, b, a, last, 128, aalsg[0]);
    int clsg1 = ClipSoBspEdges(scene, a, b, nodeCount, nodes, 128, aalsg[1]);
    aclsg[1] = clsg1;
    if (!aclsg[0] && !clsg1)
        return;
    {
        int i;
        for (i = 0; i < 2; i++)
            CategorizeClippedSegments(aclsg[i], aalsg[i], &acpos[i], aaipos[i], aaplsg[i], &aplsgBoth[i]);
    }
    AddEmbeddedVertexXps(scene, a, b, acpos[0], aaipos[0], aaplsg[0], out);
    AddEmbeddedVertexXps(scene, b, a, acpos[1], aaipos[1], aaplsg[1], out);
    for (LSG *plsg = aplsgBoth[0]; plsg; plsg = plsg->plsgNext) {
        if (!plsg->apsurf[0] || !plsg->apsurf[1])
            continue;
        int clsgPath = FindEdgePath__FP4SURFiiP3LSGPP3LSG(b->asurf, plsg->apsurf[0] - b->asurf, plsg->apsurf[1] - b->asurf, aplsgBoth[1], aplsgPath);
        if (clsgPath > 0) {
            int j;
            for (j = 0; j < clsgPath; j++) {
                LSG *plsgOther = aplsgPath[j];
                qword left0 = a->apos[plsg->pedge->aipos[0]].q, left1 = a->apos[plsg->pedge->aipos[1]].q;
                qword right0 = b->apos[plsgOther->pedge->aipos[0]].q, right1 = b->apos[plsgOther->pedge->aipos[1]].q;
                Closest(left0, left1, right0, right1, au, &normal);
                apos[0].q = QwMix(VuFloat(1.0f - au[0]), left0, VuFloat(au[0]), left1);
                apos[1].q = QwMix(VuFloat(1.0f - au[1]), right0, VuFloat(au[1]), right1);
                float g = -Length(VuSub(apos[0].q, apos[1].q));
                AddXp(scene, a, b, apos[0].q, g, normal.q, 3, plsg->pedge, plsgOther->pedge, out);
            }
            continue;
        }
        int i;
        for (i = 0; i < aclsg[1]; i++) {
            LSG *plsgOther = &aalsg[1][i];
            if (plsgOther->apsurf[0] == a->asurf + plsg->pedge->aisurf[0] || plsgOther->apsurf[0] == a->asurf + plsg->pedge->aisurf[1] ||
                plsgOther->apsurf[1] == a->asurf + plsg->pedge->aisurf[0] || plsgOther->apsurf[1] == a->asurf + plsg->pedge->aisurf[1])
                goto next;
        }
        {
            float g0 = PlaneDist(a->space->position.q, plsg->anormal[0].q);
            float g1 = PlaneDist(a->space->position.q, plsg->anormal[1].q);
            SURF *psurf;
            int ipos;
            if (g1 < g0) {
                ipos = 1 - plsg->iiposSwap;
                psurf = plsg->apsurf[0];
            } else {
                ipos = plsg->iiposSwap;
                psurf = plsg->apsurf[1];
            }
            QWROW *ppos = a->apos + plsg->pedge->aipos[ipos];
            XP *pxp = (XP *)*out;
            while (pxp) {
                if (pxp->kind == 1 && pxp->first == ppos && pxp->second == psurf)
                    break;
                pxp = pxp->next;
            }
            if (pxp)
                continue;
            qword pos = ppos->q, plane = psurf->plane.q;
            float g = PlaneDist(pos, plane);
            AddXp(scene, a, b, pos, g, plane, 1, ppos, psurf, out);
        }
    next:;
    }
    if (old == *out && (!aclsg[0] || !aclsg[1]))
        AddXp(scene, a, b, a->space->position.q, 0.0f, g_normalZ.q, 13, 0, 0, out);
}
} // namespace M_MarkSoContactsBspBsp

// Hand-tuned (gcse table size, register pins, struct copies): keeps the per-function views and helpers it was
// matched with, in a namespace of its own (see the file header).
namespace M_func_001A9108 {

typedef int qword __attribute__((mode(TI)));
struct FRAME{};
struct REQ{unsigned int bytes,alignment;};
struct POLY{qword plane;char opaque10[8];int flags;};
struct ENTRY{ENTRY*child[2];POLY*poly;};
struct HIT{qword p;char pad10[0x30];float time;char pad44[0x1C];};
struct OUT{qword p,plane;int kind;};
union SCR{REQ req;QWROW u;};
struct WORK{FRAME frame;SCR scratch;HIT hits[16];int count;ENTRY*end;QWROW position,endpoint;};
extern FRAME*STACKSCOPE_ctor(FRAME*) __asm__("STACKSCOPE_ctor");
extern void STACKSCOPE_ctor_1AE4A0(FRAME*,int) __asm__("_$_10STACKSCOPE");
extern void*AllocFromStack(REQ*) __asm__("AllocFromStack");
extern int PruneBspBySphere(ENTRY*,int,qword,float,int*,ENTRY**) __asm__("PruneBspBySphere");
extern int ClipSegmentBsp(ENTRY*,int,qword,qword,int,HIT*) __asm__("ClipSegmentBsp");
extern void InitPlaneFromNormalPos(qword*,qword,qword) __asm__("InitPlaneFromNormalPos");
// Return a - b.
static inline qword Sub(qword a,qword b){__asm__("vsub.xyzw %0,%0,%1":"+j"(a):"j"(b));return a;}
// Unit vector along v (pinned to vf2/vf3).
static inline qword Normal(qword v){register qword d __asm__("vf2"),one __asm__("vf3"),r __asm__("vf3");__asm__("vmul.xyz %0,%2,%2\n\tvaddw.x %1,$vf0,$vf0w\n\tvadday.x ACC,%0,%0y\n\tvmaddz.x %0,%1,%0z":"=j"(d),"=&j"(one):"j"(v));__asm__("vsqrt Q,%1x\n\tvwaitq\n\tvaddq.x %0,$vf0,Q":"=j"(d):"0"(d));__asm__("vdiv Q,$vf0w,%1x\n\tvwaitq\n\tvmulq.xyzw %0,%2,Q":"=j"(r):"j"(d),"j"(v));return r;}
// Finds a contact for a point embedded in a BSP: grows a probe radius from max(r, 100) until a face plane is found,
// prunes the BSP to that sphere and clips its edges; writes a contact of kind 0xC.
// Also known as: SphereInsideContacts (upstream readable / Sly 1 name;
//   SphereInsideContacts__FP3SOEiPvG9VU_VECTORP3OUTf).
extern "C" void FindContactInsideBsp(ENTRY*tree,int mask,void*geometry,qword pos,OUT*out,float radius0){
 WORK work;
 work.position.q=pos;
 float radius=FpuMax(radius0,100.0f);
 STACKSCOPE_ctor(&work.frame);
 ENTRY*first;
 {REQ*r=&work.scratch.req;r->bytes=0xFFFC;r->alignment=16;first=(ENTRY*)AllocFromStack(r);}
 int limit=0x1555;
 if(radius<52000.0f)do {
  work.end=first;work.count=limit;
  int status=PruneBspBySphere(tree,mask,work.position.q,radius,&work.count,&work.end);
  if(status<0)goto more;
  if(status<2)goto more;
  if(status!=2)goto more;
  {work.count=work.end-first;
  float best=0.0f;
  int i;
  for(i=0;i<work.count;i++){
   register POLY*poly __asm__("$6")=first[i].poly;
   if(poly->flags&0x1000)continue;
   poly->flags|=0x1000;
   register qword factor __asm__("vf2"),positionVector __asm__("vf3");
   register qword argument __asm__("$7");register ENTRY*last __asm__("$4");
   register qword to __asm__("vf1");
   __asm__("qmtc2.ni %1,%0":"=j"(factor):"r"(radius+1.0f));
   __asm__("lq %0,%5\n\tsqc2 %8,%4\n\tlw %1,%6\n\tqmtc2.ni %0,%2\n\tlqc2 %3,%7\n\taddiu %1,%1,-12\n\tvadda.xyzw ACC,%2,$vf0\n\tvmaddx.xyzw %3,%3,%8x":"=&r"(argument),"=&r"(last),"=j"(positionVector),"=&j"(to),"=m"(work.scratch.u.q):"m"(work.position.q),"m"(work.end),"m"(poly->plane),"j"(factor));
   work.endpoint.q=to;
   int found=ClipSegmentBsp(last,0x7FFFFFFF,to,argument,16,work.hits);
   register qword snapshot __asm__("vf1")=work.endpoint.q;
   if(found>0){
    if(work.hits[0].time>best){
     best=work.hits[0].time;
     qword normal=Normal(Sub(snapshot,work.position.q));
     out->p=work.hits[0].p;
     InitPlaneFromNormalPos(&out->plane,normal,work.hits[0].p);
     out->kind=12;
    }
   }
  }
  for(int j=0;j<work.count;j++)first[j].poly->flags&=~0x1000;
  if(best>0.0f){STACKSCOPE_ctor_1AE4A0(&work.frame,2);return;}
 }
 more:radius+=radius;
 }while(radius<52000.0f);
 STACKSCOPE_ctor_1AE4A0(&work.frame,2);
}

#define FIELD_OFFSET(t,m) ((unsigned int)&(((t*)0x10000)->m)-0x10000)
typedef char REQ_bytes_Offset[FIELD_OFFSET(REQ,bytes)==0?1:-1];
typedef char REQ_alignment_Offset[FIELD_OFFSET(REQ,alignment)==4?1:-1];
typedef char POLY_plane_Offset[FIELD_OFFSET(POLY,plane)==0?1:-1];
typedef char POLY_flags_Offset[FIELD_OFFSET(POLY,flags)==24?1:-1];
typedef char ENTRY_child_Offset[FIELD_OFFSET(ENTRY,child)==0?1:-1];
typedef char ENTRY_poly_Offset[FIELD_OFFSET(ENTRY,poly)==8?1:-1];
typedef char HIT_p_Offset[FIELD_OFFSET(HIT,p)==0?1:-1];
typedef char HIT_time_Offset[FIELD_OFFSET(HIT,time)==64?1:-1];
typedef char OUT_p_Offset[FIELD_OFFSET(OUT,p)==0?1:-1];
typedef char OUT_plane_Offset[FIELD_OFFSET(OUT,plane)==16?1:-1];
typedef char OUT_kind_Offset[FIELD_OFFSET(OUT,kind)==32?1:-1];
typedef char U_q_Offset[FIELD_OFFSET(QWROW,q)==0?1:-1];
typedef char U_f_Offset[FIELD_OFFSET(QWROW,f)==0?1:-1];
typedef char SCR_req_Offset[FIELD_OFFSET(SCR,req)==0?1:-1];
typedef char SCR_u_Offset[FIELD_OFFSET(SCR,u)==0?1:-1];
typedef char WORK_count_Offset[FIELD_OFFSET(WORK,count)==1568?1:-1];
typedef char WORK_end_Offset[FIELD_OFFSET(WORK,end)==1572?1:-1];
typedef char WORK_position_Offset[FIELD_OFFSET(WORK,position)==1584?1:-1];
typedef char WORK_endpoint_Offset[FIELD_OFFSET(WORK,endpoint)==1600?1:-1];
typedef char FRAME_PrefixSize[sizeof(FRAME)==1?1:-1];
typedef char FRAME_Alignment[__alignof__(FRAME)==1?1:-1];
typedef char REQ_PrefixSize[sizeof(REQ)==8?1:-1];
typedef char REQ_Alignment[__alignof__(REQ)==4?1:-1];
typedef char POLY_PrefixSize[sizeof(POLY)==32?1:-1];
typedef char POLY_Alignment[__alignof__(POLY)==16?1:-1];
typedef char ENTRY_PrefixSize[sizeof(ENTRY)==12?1:-1];
typedef char ENTRY_Alignment[__alignof__(ENTRY)==4?1:-1];
typedef char HIT_PrefixSize[sizeof(HIT)==96?1:-1];
typedef char HIT_Alignment[__alignof__(HIT)==16?1:-1];
typedef char OUT_PrefixSize[sizeof(OUT)==48?1:-1];
typedef char OUT_Alignment[__alignof__(OUT)==16?1:-1];
typedef char U_PrefixSize[sizeof(QWROW)==16?1:-1];
typedef char U_Alignment[__alignof__(QWROW)==16?1:-1];
typedef char SCR_PrefixSize[sizeof(SCR)==16?1:-1];
typedef char SCR_Alignment[__alignof__(SCR)==16?1:-1];
typedef char WORK_PrefixSize[sizeof(WORK)==1616?1:-1];
typedef char WORK_Alignment[__alignof__(WORK)==16?1:-1];
typedef char qword_Width[sizeof(qword)==16?1:-1];
typedef char int_Width[sizeof(int)==4?1:-1];
typedef char float_Width[sizeof(float)==4?1:-1];
typedef char pointer_Width[sizeof(void*)==4?1:-1];
typedef char qword_Alignment[__alignof__(qword)==16?1:-1];

typedef char WORK_frame_Offset[FIELD_OFFSET(WORK,frame)==0?1:-1];
typedef char WORK_scratch_Offset[FIELD_OFFSET(WORK,scratch)==16?1:-1];
typedef char WORK_hits_Offset[FIELD_OFFSET(WORK,hits)==32?1:-1];
} // namespace M_func_001A9108

// Hand-tuned (gcse table size, register pins, struct copies): keeps the per-function views and helpers it was
// matched with, in a namespace of its own (see the file header).
namespace M_CsbiIntersectSphereBsp {
// IntersectSphereBsp: ported from Lilly's personal Sly 2 decomp (src/P2/cplane.c), reduced to the declarations it needs.
// Every global is named by its address; the function keeps its readable name and gets the team symbol through __asm__.
// ---- CplaneFromSphere 0x1A93C8 (MATCH, 472 instructions; T1304 draft 48%, T1341 99.4%, T1396 the last mtc1) ----
// The contacts of a sphere (centre pos, radius sRadius) with the camera hull of a collision part: the first pbspLimit nodes of ppart->bspc.absp. At most
// ccontactMax CMCONTACTs (bsp.h) go to acontact; returns how many. Callers: SquishCmEye 0x14EDA0 (eye sphere, cbsp 0 / absp NULL / up to 64) and
// TryCollectDprize 0x1C5B20 (prize sphere, pbspLimit 0x7FFFFFFF, one contact is enough). Steps (all proven by the draft's instruction stream):
//  1. absp == NULL: allocate min(numNodesAllocated, 0x1555) nodes on the stack (STACKSCOPE) and PruneBsp 0x118A88 the tree to the sphere. PRUNE_Pos (wholly outside) -> 0;
//     PRUNE_Neg (wholly inside) -> step 6; else cbsp = nodes written, the last one is the root.
//  2. Every SURF of the pruned tree gets grf 0x10.
//  3. Faces: for each surface (0x10, not 0x40; sets 0x40) the segment pos + r*n .. pos - r*n is clipped to the pruned tree (ClsgClipEdgeToBsp, 16 LSGs);
//     if a piece starts/ends on this surface (apsurf[0] when au0 + au1 > 1, else apsurf[1]) the face is touched: contact kind 6
//     {pos - r*n, the face plane, 6, psurf} and grf |= 0x80.
//  4. Edges: for each surface with 0x10 and neither 0x20 nor 0x80 (sets 0x20), each edge whose other face has 0x10 but not 0x80: the point of the edge closest
//     to pos (u < 0.0001 -> vertex aipos[0], u > 0.9999 -> vertex aipos[1], else the interior point). Within the radius: unit = normalize(pos - closest),
//     plane = (unit, unit . closest) (InitPlaneFromNormalPos), point = pos - r*unit. Interior -> contact kind 5 {point, plane, 5, pedge}; a vertex -> a candidate
//     (at most 0x100 on the stack, no duplicates by vertex index).
//  5. Vertices: each candidate whose vertex is not an end of one of the edge contacts -> contact kind 4 {point, plane, 4, &meshWorld.apos[ivert]}.
//     (The game compares against the edge contacts made in step 4 only.) Reaching ccontactMax in any step skips to the clean-up.
//  6. All 0xF0 surface flags are cleared again. With no contact and the centre inside the hull (PbspPointInBsp 0x119090) the one contact comes from
//     ContactInsideBsp 0x1A9108 (our name, MATCH T1305 above: doubles a probe radius from max(r, 100) until a face plane is found, kind 0xC).
// Settled spellings, T1304 (each one moved the score): `ccontact = cvert = 0;` after the STACKSCOPE (gives `sw zero` in the ctor's delay slot); `*(qword *)&pos`
// in the PruneBsp call (address-taken param -> new pseudo, slot 0x3670 above the spills); `for (i = cbsp; --i >= 0; )` loops (no gcse of cbsp - 1); a
// function-level `BSP *pbspNode` shared by the face and edge loops (a3); a function-level `SURF *psurf` (s0 in the clean-up loop); VECTOR4 locals vecA/vecB
// before LSG alsg[16] and vecEdge/vecDelta/vecUnit/vecClosest/plane (the game's 0x3010.. slots); VERTCAND avert[0x100] before the STACKSCOPE (sp+0); QwAddScaled
// with the scale as the FIRST asm input; `-sRadius` written twice (one neg.s in f20); pposClosest starting as &apos[i0]; the dot / normalize reading
// *(qword *)&vecDelta (the vmove copy); `if (ivert >= 0) {vertex} else {edge}`; the vertex-loop bound snapshot ccontactEdge; (VECTOR4 *) apos (sll 4);
// uint cvert (sltiu); (int)grf & ~0xF0 (addiu -0xF1).
// Settled by T1341 (48% -> 99.4%; scratch tools in decomp/build/scratch/t1341: fd.py = difflib alignment, lc.sh = face-loop insn counts from -dL):
//  - The face contact's position is stored through a VU_VECTOR4 struct return (`*(VU_VECTOR4 *)&pcontact->pos = VecAddScaled(..)`): the struct copies
//    survive until the SECOND loop.c pass (97 insns there, the game needs >= 96), so 0xC / 0x60 / 1.0f stay in the face loop and &absp[i] is not reduced.
//    (Single-use copies of loop invariants die in pass 1: loop.c scan_loop substitutes them; plain inline/qword copies die in cse1.)
//  - Its stores in the order kind, plane, pv, pos; VuFloat written inside the vecA/vecB calls (each home store must precede its normal's load).
//  - ONE function-level `CMCONTACT *pcontact` for all three appends AND the vertex loop's scan of the edge contacts (`pcontact = &acontact[j]`): set twice in
//    the vertex loop, it is no induction variable there, so the game's `lw acontact; addu a2, v1, t2` (only ccontact * 0x30 reduced) appears; in the edge
//    loop it is set once and reduced (s3) as in the game.
//  - A function-level `VU_VECTOR4 posContact` for the edge/vertex point: the vertex candidate reads posContact.qw (the qmfc2 copy in v1, `sq v1`), the edge
//    contact stores the struct (`sqc2 vf2`).
//  - The interior closest point adds onto `qword posBase = *(qword *)pposClosest` (base loaded before the scale: vf1 base, vf2 scale as in the game).
//  - The squared distance reuses `u` (a multi-block pseudo -> global alloc: r*r gets f0, the distance f1 as in the game).
// Settled by T1396 (the last slot, 99.4% -> MATCH): the edge length's mtc1 (0x1A977C) sits BETWEEN the pos reload and the edge-start load in the game. The
// compiler's own mtc1 (GFromVf's float "=r" output) costs 2 cycles on the memory unit and has priority 14, so sched2 issued the edge-start load (priority 16)
// first. An asm mtc1 (no unit, 1 cycle) co-issues with the pos reload and lands in the game's slot, but its int temp is block-local: local-alloc gives it v0 and
// global-alloc then puts ppos1 in v1 (game: ppos1 v0, temp v1). The temp is therefore pinned with a register variable (`register int iLenSq asm("$3")`),
// which is what the game's allocation looks like; the other two temps (u's dot, the distance) are the compiler's mtc1 with reload registers (v1 / v0) as before.
// Natural spellings that did NOT give v1: float / int scratch, a function-level scratch, ivert / psurfOther / the index variables as the scratch (T0950 trick:
// s0 / wrong schedule), fused qmfc2+mtc1 asm.
// Gotos: the early PRUNE exits and the "contact array full" exits jump past whole phases, and the two duplicate scans branch straight to the next
// iteration of the outer loop (loop/flag/inline-function spellings all gave a peeled loop or a leftover flag test, see re/notes/camera-updatecm.md).
#include "sly2/basic.h"
#include "sly2/xfm.h"
#include "sly2/so.h"
struct VU_VECTOR4
{
    qword qw;
};
void *PvAllocStackImpl(SWALLOC *pswalloc) __asm__("AllocFromStack");
struct VU_FLOAT
{
    qword qw;
};
// Broadcast a float into a VU0 register.
static inline VU_FLOAT VuFloat(float g)
{
    VU_FLOAT v;
    __asm__("qmtc2 %1, %0" : "=j"(v.qw) : "r"(g));
    return v;
}
// Unit vector along v.
static inline qword QwNormalize(qword v)
{
    qword qw = VuLength(v);
    qword qwNormal;
    __asm__("vdiv $Q, $vf0w, %1x\n\tvwaitq\n\tvmulq.xyzw %0, %2, $Q" : "=j"(qwNormal) : "j"(qw), "j"(v));
    return qwNormal;
}
extern void InitPlaneFromNormalPos(SURF *psurf, qword qwNormal, qword qwPos) __asm__("InitPlaneFromNormalPos");
void ContactInsideBsp(float sRadius, BSP *absp, BSP *pbspLimit, GEOM *pgeom, qword pos, CMCONTACT *pcontact) __asm__("FindContactInsideBsp");   /* float first: this caller's order */
struct VERTCAND
{
    int ivert;
    qword pos;
    qword plane;
};
// Allocate cb bytes, 16-byte aligned, from the frame stack.
static inline void *PvAllocStack(int cb)
{
    SWALLOC swalloc;
    swalloc.cb = cb;
    swalloc.cbAlign = 0x10;
    return PvAllocStackImpl(&swalloc);
}
// Return qwBase + qwVec * gScale.
static inline qword QwAddScaled(qword qwBase, VU_FLOAT gScale, qword qwVec)
{
    qword qw;
    __asm__("vadda.xyzw $ACC, %1, $vf0\n\tvmaddx.xyzw %0, %3, %2x" : "=j"(qw) : "j"(qwBase), "j"(gScale.qw), "j"(qwVec));
    return qw;
}
// qwBase + qwVec * gScale as a VU_VECTOR4.
static inline VU_VECTOR4 VecAddScaled(qword qwBase, VU_FLOAT gScale, qword qwVec)
{
    VU_VECTOR4 v;
    v.qw = QwAddScaled(qwBase, gScale, qwVec);
    return v;
}
int CplaneFromSphere(qword pos, float sRadius, SOPART *ppart, BSP *pbspLimit, int cbsp, BSP *absp, int ccontactMax, CMCONTACT *acontact) __asm__("IntersectSphereBsp");
// Intersects a sphere with the BSP hull of a part and writes up to ccontactMax contact planes (Sly 1
// CsbiIntersectSphereBsp).
// Also known as: SphereBspContacts (upstream readable / Sly 1 name;
//   SphereBspContacts__FG9VU_VECTORfP6SOPARTP3BSPiT3iP9CMCONTACT).
int CplaneFromSphere(qword pos, float sRadius, SOPART *ppart, BSP *pbspLimit, int cbsp, BSP *absp, int ccontactMax, CMCONTACT *acontact)
{
    int ccontact;
    uint cvert;
    BSP *pbsp;
    int cbspLeft;
    SURF *psurf;
    BSP *pbspNode;
    VU_VECTOR4 posContact;
    CMCONTACT *pcontact;
    VERTCAND avert[0x100];
    STACKSCOPE scope;
    ccontact = cvert = 0;
    int ccontactFace;
    int ccontactEdge;
    if (ccontactMax > 0)
    {
        if (absp == 0)
        {
            int cbspAlloc = 0x1555;
            if (ppart->bspc.numNodesAllocated < 0x1555)
                cbspAlloc = ppart->bspc.numNodesAllocated;
            cbspLeft = cbspAlloc;
            absp = (BSP *)PvAllocStack(cbspAlloc * sizeof(BSP));
            pbsp = absp;
            PRUNE prune = PruneBsp(ppart->bspc.nodes, pbspLimit, sRadius, *(qword *)&pos, &cbspLeft, &pbsp);
            if (prune == PRUNE_Neg)
                goto inside;
            if (prune == PRUNE_Pos)
                goto done;
            cbsp = pbsp - absp;
        }
        int i;
        pbsp = absp + (cbsp - 1);
        for (i = cbsp; --i >= 0; )
            absp[i].psurf->grf |= 0x10;
        for (i = cbsp; --i >= 0; )
        {
            pbspNode = &absp[i];
            psurf = pbspNode->psurf;
            if (psurf->grf & 0x10)
            {
                if (!(psurf->grf & 0x40))
                {
                    psurf->grf |= 0x40;
                    VECTOR4 vecA;
                    VECTOR4 vecB;
                    LSG alsg[16];
                    *(qword *)&vecA = QwAddScaled(pos, VuFloat(sRadius), *(qword *)pbspNode->psurf);
                    *(qword *)&vecB = QwAddScaled(pos, VuFloat(-sRadius), *(qword *)pbspNode->psurf);
                    int clsg = ClsgClipEdgeToBsp(pbsp, (BSP *)0x7fffffff, *(qword *)&vecA, *(qword *)&vecB, 16, alsg);
                    int ilsg;
                    for (ilsg = 0; ilsg < clsg; ilsg++)
                    {
                        SURF *psurfHit;
                        if (1.0f < alsg[ilsg].au[0] + alsg[ilsg].au[1])
                            psurfHit = alsg[ilsg].apsurf[0];
                        else
                            psurfHit = alsg[ilsg].apsurf[1];
                        if (psurfHit == psurf)
                            break;
                    }
                    if (ilsg < clsg)
                    {
                        if (ccontact < ccontactMax)
                        {
                            pcontact = &acontact[ccontact++];
                            qword plane = *(qword *)psurf;
                            pcontact->kind = 6;
                            pcontact->plane = plane;
                            pcontact->pv = psurf;
                            *(VU_VECTOR4 *)&pcontact->pos = VecAddScaled(pos, VuFloat(-sRadius), plane);
                            if (ccontact >= ccontactMax)
                                goto clear;
                        }
                        psurf->grf |= 0x80;
                    }
                }
            }
        }
        ccontactFace = ccontact;
        for (i = cbsp; --i >= 0; )
        {
            VECTOR4 *apos = (VECTOR4 *)ppart->meshWorld.positions;
            SURF *asurf = ppart->meshWorld.surfaces;
            pbspNode = &absp[i];
            psurf = pbspNode->psurf;
            if (psurf->grf & 0x10)
            {
                if (!(psurf->grf & 0xA0))
                {
                    psurf->grf |= 0x20;
                    for (EDGE *pedge = psurf->pedge; pedge != 0; pedge = pedge->pedgeNext)
                    {
                        SURF *psurfOther = &asurf[pedge->aisurf[1]];
                        if (!(psurfOther->grf & 0x10))
                            continue;
                        if (psurfOther->grf & 0x80)
                            continue;
                        VECTOR4 vecEdge;
                        VECTOR4 vecDelta;
                        VECTOR4 vecUnit;
                        VECTOR4 vecClosest;
                        VECTOR4 plane;
                        VECTOR4 *pposClosest = &apos[pedge->aipos[0]];
                        VECTOR4 *ppos1 = &apos[pedge->aipos[1]];
                        int ivert;
                        *(qword *)&vecEdge = VuSub(*(qword *)ppos1, *(qword *)pposClosest);
                        register int iLenSq __asm__("$3");
                        float gLenSq;
                        __asm__("qmfc2 %0, %1" : "=r"(iLenSq) : "j"(VuDot(*(qword *)&vecEdge, *(qword *)&vecEdge)));
                        __asm__("mtc1 %1, %0" : "=f"(gLenSq) : "r"(iLenSq));
                        *(qword *)&vecDelta = VuSub(pos, *(qword *)pposClosest);
                        float u = VuToFloat(VuDot(*(qword *)&vecDelta, *(qword *)&vecEdge)) / gLenSq;
                        if (u < 0.0001f)
                        {
                            ivert = pedge->aipos[0];
                        }
                        else if (0.9999f < u)
                        {
                            pposClosest = ppos1;
                            ivert = pedge->aipos[1];
                            *(qword *)&vecDelta = VuSub(pos, *(qword *)pposClosest);
                        }
                        else
                        {
                            ivert = -1;
                            qword posBase = *(qword *)pposClosest;
                            *(qword *)&vecClosest = QwAddScaled(posBase, VuFloat(u), *(qword *)&vecEdge);
                            pposClosest = &vecClosest;
                            *(qword *)&vecDelta = VuSub(pos, *(qword *)&vecClosest);
                        }
                        u = VuToFloat(VuDot(*(qword *)&vecDelta, *(qword *)&vecDelta));
                        if (u > sRadius * sRadius)
                            continue;
                        {
                            if (ivert >= 0)
                            {
                                for (int j = cvert - 1; j >= 0; j--)
                                    if (avert[j].ivert == ivert)
                                        goto nextedge;
                            }
                            *(qword *)&vecUnit = QwNormalize(*(qword *)&vecDelta);
                            InitPlaneFromNormalPos((SURF *)&plane, *(qword *)&vecUnit, *(qword *)pposClosest);
                            posContact = VecAddScaled(pos, VuFloat(-sRadius), *(qword *)&vecUnit);
                            if (ivert >= 0)
                            {
                                if (cvert < 0x100)
                                {
                                    VERTCAND *pvert = &avert[cvert++];
                                    pvert->ivert = ivert;
                                    pvert->pos = posContact.qw;
                                    pvert->plane = *(qword *)&plane;
                                }
                            }
                            else
                            {
                                if (ccontact >= ccontactMax)
                                    continue;
                                pcontact = &acontact[ccontact++];
                                *(VU_VECTOR4 *)&pcontact->pos = posContact;
                                pcontact->plane = *(qword *)&plane;
                                pcontact->kind = 5;
                                pcontact->pv = pedge;
                                if (ccontact >= ccontactMax)
                                    goto clear;
                            }
                        }
nextedge:
                        ;
                    }
                }
            }
        }
        ccontactEdge = ccontact;
        for (int iv = cvert - 1; iv >= 0; iv--)
        {
            int ivert = avert[iv].ivert;
            for (int j = ccontactFace; j < ccontactEdge; j++)
            {
                pcontact = &acontact[j];
                EDGE *pedge = (EDGE *)pcontact->pv;
                if (pedge->aipos[0] == ivert || pedge->aipos[1] == ivert)
                    goto nextvert;
            }
            if (ccontact < ccontactMax)
            {
                pcontact = &acontact[ccontact++];
                pcontact->pos = avert[iv].pos;
                pcontact->plane = avert[iv].plane;
                pcontact->kind = 4;
                pcontact->pv = &((VECTOR4 *)ppart->meshWorld.positions)[ivert];
                if (ccontact >= ccontactMax)
                    break;
            }
nextvert:
            ;
        }
clear:
        for (i = cbsp; --i >= 0; )
        {
            psurf = absp[i].psurf;
            psurf->grf = (int)psurf->grf & ~0xF0;
        }
inside:
        if (ccontact == 0 && PbspPointInBsp(ppart->bspc.nodes, pbspLimit, pos) != 0)
        {
            ccontact = 1;
            ContactInsideBsp(sRadius, ppart->bspc.nodes, pbspLimit, &ppart->meshWorld, pos, acontact);
        }
    }
done:
    return ccontact;
}
} // namespace M_CsbiIntersectSphereBsp

// Hand-tuned (gcse table size, register pins, struct copies): keeps the per-function views and helpers it was
// matched with, in a namespace of its own (see the file header).
namespace M_MarkSoContactsSphereBsp {
typedef int qword __attribute__((mode(TI)));
union U { qword q; float f[4]; U(); };
struct S0 { char pad[0x70]; qword position; };
struct LO;struct CONTACT;struct NODE;
struct S { struct S0 *transform; struct LO *so; float radius; };
union DETAIL {void*pointer;int word;};
struct E {qword point,plane;int kind;DETAIL detail;};
extern int IntersectSphereBsp(qword pos, struct S*b,int c,int d,struct NODE*e,int n, struct E *buf, float f) __asm__("IntersectSphereBsp");
extern U*InitVuTemp(U*p) __asm__("InitVuTemp");
extern void AddXp(void *a, struct S*s,struct S*b,qword p, qword q, int kind, void*z, void*feature,struct CONTACT**g,float f) __asm__("AddXp");
// Construct a VU temporary via InitVuTemp.
inline U::U() { (void)InitVuTemp(this); }
#define FIELD_OFFSET(t,m) ((unsigned int)&(((t*)0x10000)->m)-0x10000)
typedef char Offset_0[FIELD_OFFSET(U,q)==0?1:-1];
typedef char Offset_1[FIELD_OFFSET(U,f)==0?1:-1];
typedef char Offset_2[FIELD_OFFSET(DETAIL,pointer)==0?1:-1];
typedef char Offset_3[FIELD_OFFSET(DETAIL,word)==0?1:-1];
typedef char Offset_4[FIELD_OFFSET(S0,position)==112?1:-1];
typedef char Offset_5[FIELD_OFFSET(S,transform)==0?1:-1];
typedef char Offset_6[FIELD_OFFSET(S,so)==4?1:-1];
typedef char Offset_7[FIELD_OFFSET(S,radius)==8?1:-1];
typedef char Offset_8[FIELD_OFFSET(E,point)==0?1:-1];
typedef char Offset_9[FIELD_OFFSET(E,plane)==16?1:-1];
typedef char Offset_10[FIELD_OFFSET(E,kind)==32?1:-1];
typedef char Offset_11[FIELD_OFFSET(E,detail)==36?1:-1];
typedef char Size_0[sizeof(U)==16?1:-1];
typedef char Size_1[sizeof(DETAIL)==4?1:-1];
typedef char Size_2[sizeof(S0)==128?1:-1];
typedef char Size_3[sizeof(S)==12?1:-1];
typedef char Size_4[sizeof(E)==48?1:-1];
typedef char Size_5[sizeof(qword)==16?1:-1];
typedef char Size_6[sizeof(void*)==4?1:-1];
typedef char Size_7[sizeof(int)==4?1:-1];
typedef char Size_8[sizeof(float)==4?1:-1];
// Sphere part against BSP part: IntersectSphereBsp, then one AddXp per hit.
// Also known as: MarkSphereBsp (upstream readable / Sly 1 name; MarkSphereBsp__FP3SOET0PviPP2XPPP7CONTACT).
extern "C" void MarkSoContactsSphereBsp(void *a, struct S*s,struct S*b,int d,struct NODE*e,struct CONTACT**g)
{
    register S*bodyOwner __asm__("$19")=b;
    register void*worldOwner __asm__("$21")=a;
    struct E buf[64];

    int i;
    struct E *pe;
    i = IntersectSphereBsp(s->transform->position, bodyOwner, 0x7FFFFFFF, d, e, 0x40, buf, s->radius) - 1;
    if (i >= 0) {
        pe = &buf[i];
        do {
            qword A, B;
            i--;
            {register qword word __asm__("$2")=pe->point;A=word;}
            {register qword word __asm__("$2")=pe->plane;B=word;}
            U r,one;
            register qword scalar __asm__("vf1"),product __asm__("vf2");
            register void*worldArgument __asm__("$4");register S*shapeArgument __asm__("$5");
            __asm__("lqc2 $vf3,%5\n\t"
                    "daddu %2,%6,$zero\n\t"
                    "lqc2 $vf4,%4\n\t"
                    "daddu %3,%7,$zero\n\t"
                    "vmul.xyz %1,$vf3,$vf4\n\t"
                    "vaddw.x %0,$vf0,$vf0w\n\t"
                    "vadday.x $ACC,%1,%1y\n\t"
                    "vmaddaz.x $ACC,%0,%1z\n\t"
                    "vmsubw.x %1,%0,$vf3w"
                    : "=&j"(scalar),"=&j"(product),"=&r"(worldArgument),"=r"(shapeArgument)
                    : "m"(A),"m"(B),"r"(worldOwner),"r"(s) : "vf3","vf4");
            one.q=scalar;
            register S*bodyArgument __asm__("$6");register void*emptyFeature __asm__("$10");
            register qword pointArgument __asm__("$7"),planeArgument __asm__("$8");register float separation __asm__("$f12");
            __asm__("daddu %0,%6,$zero\n\t"
                    "sqc2 %7,%5\n\t"
                    "daddu %1,$zero,$zero\n\t"
                    "lq %2,%8\n\t"
                    "qmfc2.ni $2,%7\n\t"
                    "lq %3,%9\n\t"
                    "mtc1 $2,%4"
                    : "=&r"(bodyArgument),"=&r"(emptyFeature),"=&r"(pointArgument),"=r"(planeArgument),"=f"(separation),"=m"(r.q)
                    : "r"(bodyOwner),"j"(product),"m"(A),"m"(B) : "$2");
            AddXp(worldArgument, shapeArgument, bodyArgument, pointArgument, planeArgument, pe->kind, emptyFeature, pe->detail.pointer, g, separation);
            pe=(E*)((unsigned)pe-sizeof(E));
        } while (i >= 0);
    }
}

typedef char layout_qword_Alignment[(__alignof__(qword)==16)?1:-1];
typedef char layout_U_Alignment[(__alignof__(U)==16)?1:-1];
typedef char layout_S0_Alignment[(__alignof__(S0)==16)?1:-1];
typedef char layout_S_Alignment[(__alignof__(S)==4)?1:-1];
typedef char layout_DETAIL_Alignment[(__alignof__(DETAIL)==4)?1:-1];
typedef char layout_E_Alignment[(__alignof__(E)==16)?1:-1];
typedef char layout_CandidateBuffer_Span[(sizeof(E[64])==0xC00)?1:-1];
typedef char layout_CandidatePoint_Span[(sizeof(((E*)0)->point)==16)?1:-1];
typedef char layout_CandidatePlane_Span[(sizeof(((E*)0)->plane)==16)?1:-1];
typedef char layout_CandidateKind_Width[(sizeof(((E*)0)->kind)==4)?1:-1];
typedef char layout_CandidateFeature_Width[(sizeof(((E*)0)->detail.pointer)==4)?1:-1];
typedef char layout_OwnerPoint_Span[(sizeof(((S0*)0)->position)==16)?1:-1];
typedef char layout_ShapeRadius_Width[(sizeof(((S*)0)->radius)==4)?1:-1];
typedef char layout_U_FloatSpan[(sizeof(((U*)0)->f)==16)?1:-1];
} // namespace M_MarkSoContactsSphereBsp

/* Two sphere parts (centre: the transform's position, radius `radius`): one kind-7 contact on ppart0's surface along
   the centre line (g_normalZ when the centres coincide), separation |d| - (r0 + r1). */
// Sphere part against sphere part: if they overlap adds one contact along the line between the centres.
// Also known as: MarkSphereSphere (upstream readable / Sly 1 name; MarkSphereSphere__FP3SOET0PP2XPPP7CONTACT).
extern "C" void MarkSoContactsSphereSphere(SW *world, SOPART *part0, SOPART *part1, XP **listOut)
{
    qword pos1, d, l, one, normal, pos, pos0;
    float rsum, len2, g, r2;
    union { qword q; float f[4]; } u;
    register XF *pxf1 __asm__("$2") = part1->transform;
    register XF *pxf0 __asm__("$3") = part0->transform;
    pos0 = QW_XF_POS(pxf0);
    pos1 = QW_XF_POS(pxf1);
    rsum = part0->radius + part1->radius;
    r2 = rsum * rsum;
    __asm__("vsub.xyzw %0, %1, %2" : "=j"(d) : "j"(pos0), "j"(pos1));
    __asm__("vmul.xyz %0, %2, %2\n\tvaddw.x %1, $vf0, $vf0w\n\tvadday.x $ACC, %0, %0y\n\tvmaddz.x %0, %1, %0z" : "=&j"(l), "=&j"(one) : "j"(d));
    __asm__ __volatile__("qmfc2.ni %0, %1" : "=r"(len2) : "j"(l));
    if (r2 < len2)
        return;
    if (len2 < 1e-4f) {
        g = 0.0f;
        normal = g_normalZ;
    } else {
        qword s;
        float t;
        __asm__ __volatile__("sqrt.s %0, %1" : "=&f"(t) : "f"(len2));
        g = t;
        __asm__("qmtc2.ni %1, %0" : "=j"(s) : "r"(g));
        u.q = s;
        __asm__("vdiv $Q, $vf0w, %1x\n\tvwaitq\n\tvmulq.xyzw %0, %2, $Q" : "=j"(normal) : "j"(u.q), "j"(d));
    }
    {
        qword s;
        __asm__("qmtc2.ni %1, %0" : "=j"(s) : "r"(-part0->radius));
        u.q = s;
        __asm__("vadda.xyzw $ACC, %1, $vf0\n\tvmaddx.xyzw %0, %2, %3x" : "=j"(pos) : "j"(pos0), "j"(normal), "j"(u.q));
    }
    AddXp(world, part0, part1, pos, normal, 7, 0, 0, listOut, g - rsum);
}

// Hand-tuned (gcse table size, register pins, struct copies): keeps the per-function views and helpers it was
// matched with, in a namespace of its own (see the file header).
namespace M_func_001A9D38 {
typedef int qword __attribute__((mode(TI)));
struct M4{qword r[4];};
struct HIT{qword p;char pad10[0x30];float time;int unknown44,unknown48,tag;char pad50[0x10];};
struct W;struct C;
extern M4 InvertMatrix4Rigid(const M4*) __asm__("InvertMatrix4Rigid");
extern int ClipEdgeToObject(SOPART*,qword,qword,int,HIT*) __asm__("ClipEdgeToObject");
extern void AddXp(W*,SOPART*,SOPART*,qword,qword,int,int,int,C**,float) __asm__("AddXp");
// Transform point p by the 4x4 matrix m into a new register.
static inline qword TransformNew(const M4*m,qword p){qword r;__asm__("vmulax.xyz ACC,%1,%3x\n\tvmadday.xyz ACC,%2,%3y\n\tvmaddaz.xyz ACC,%4,%3z\n\tvmaddw.xyz %0,%5,$vf0w":"=&j"(r):"j"(m->r[0]),"j"(m->r[1]),"j"(p),"j"(m->r[2]),"j"(m->r[3]));return r;}
// Transform point p by the 4x4 matrix m in place.
static inline qword Transform(const M4*m,qword p){__asm__("vmulax.xyz ACC,%1,%3x\n\tvmadday.xyz ACC,%2,%3y\n\tvmaddaz.xyz ACC,%4,%3z\n\tvmaddw.xyz %0,%5,$vf0w":"=j"(p):"j"(m->r[0]),"j"(m->r[1]),"0"(p),"j"(m->r[2]),"j"(m->r[3]));return p;}
// Return a - b, in vf1.
static inline qword SubN(qword a,qword b){register qword r __asm__("vf1");__asm__("vsub.xyzw %0,%1,%2":"=j"(r):"j"(a),"j"(b));return r;}
// Dot product of the xyz parts of a and b as a float.
static inline float Dot(qword a,qword b){qword d,one;float f;__asm__("vmul.xyz %0,%2,%3\n\tvaddw.x %1,$vf0,$vf0w\n\tvadday.x ACC,%0,%0y\n\tvmaddz.x %0,%1,%0z":"=j"(d),"=&j"(one):"j"(a),"j"(b));__asm__("qmfc2.ni %0,%1":"=r"(f):"j"(d));return f;}
// Unit vector along v (pinned to vf2/vf3/vf5).
static inline qword Normal(qword v){register qword d __asm__("vf2"),one __asm__("vf3"),r __asm__("vf5");__asm__("vmul.xyz %0,%2,%2\n\tvaddw.x %1,$vf0,$vf0w\n\tvadday.x ACC,%0,%0y\n\tvmaddz.x %0,%1,%0z":"=j"(d),"=&j"(one):"j"(v));__asm__("vsqrt Q,%1x\n\tvwaitq\n\tvaddq.x %0,$vf0,Q":"=j"(d):"0"(d));__asm__("vdiv Q,$vf0w,%1x\n\tvwaitq\n\tvmulq.xyzw %0,%2,Q":"=j"(r):"j"(d),"j"(v));return r;}
// Copy a VU0 register (vmove).
static inline qword Copy(qword q){qword r;__asm__("vmove.xyzw %0,%1":"=j"(r):"j"(q));return r;}
// Smallest signed distance along dir of the count0 points (*points0) from pos; 0 if all are in front.
static inline float MinDistance(qword pos,qword dir,int count0,const qword*const*points0){
 register int count __asm__("$3")=count0;
 register float best __asm__("$f12")=0;
 if(count>0){register const qword*points __asm__("$4")=*points0; register qword p __asm__("vf4")=Copy(pos);
 register qword n __asm__("vf3")=Copy(dir);
do {float d=Dot(VuSub(*points,p),n);if(d<best)best=d;points++;}while(--count);}
 return best;
}
// Sweep vertices and create contacts from the first accepted segment hit.
// Sweeps the vertices of one part from the previous frame's pose to the current one with line tests against the
// other part, adding a contact at each first hit; catches fast objects that would tunnel through.
// Also known as: MarkSweptVertices (upstream readable / Sly 1 name; MarkSweptVertices__FP3SOET0PP2XPPv).
extern "C" void MarkSoContactsSweptVertices(W*w,SOPART*a,SOPART*b,C**out){
 int i;
 for(i=a->unknown1C;i<a->meshLocal.numPositions;i++){
  HIT hits[2];
  qword pos=TransformNew((M4 *)&a->matWorld,*(qword *)&a->meshLocal.positions[i]);
  if(b->meshStamp!=b->worldStamp)pos=Transform((M4 *)&b->transform->worldMatrix,Transform(&InvertMatrix4Rigid((M4 *)&b->matWorld),pos));
  qword dest=*(qword *)&a->meshWorld.positions[i];
  int count=ClipEdgeToObject(b,pos,dest,2,hits);
  if(count){
   int skip=hits[0].time==0.0f;
   if(skip<count){
    qword dir=Normal(SubN(pos,dest));
    qword hit=hits[0].p;
    float g=0.0f;
    g=MinDistance(hit,dir,a->meshWorld.numPositions,(const qword *const *)&a->meshWorld.positions);
    if(a->unknownFlag10B){float d=Dot(VuSub(*(qword *)&a->transform->worldMatq.r[3],hit),dir)-a->radius;if(d<g)g=d;}
    int kind=b->unknownFlag10B?9:8;
    int tag=b->unknownFlag10B?0:hits[0].tag;
    AddXp(w,a,b,hit,dir,kind,tag,0,out,g);
   }
  }
 }
}
} // namespace M_func_001A9D38

/* Prepends the list pxp..(pxpNext) to *ppxpFirst. */
// Splices XP list b in front of *ppxp (walks b to its tail and links).
// Also known as: PrependXpList (upstream readable / Sly 1 name; PrependXpList__FP2XPPP2XPPv).
extern "C" void AppendXpList(SW *world, XP *list, XP **head)
{
    if (list) {
        XP *pxpLast = list;
        while (pxpLast->pxpNext) pxpLast = pxpLast->pxpNext;
        pxpLast->pxpNext = *head;
        *head = list;
    }
}

/* Copies the list pxp..(pxpNext) into new XPs; returns the copy. */
// Duplicates an XP list into newly allocated contact nodes.
// Also known as: PxpCloneList (upstream readable / Sly 1 name; PxpCloneList__FP2XPPv).
extern "C" XP *CloneXpList(SW *world, XP *list)
{
    XP *pxpFirst;
    XP **ppxp;
    if (!list)
        return 0;
    pxpFirst = 0;
    ppxp = &pxpFirst;
    do {
        XP *pxpNew = (XP *)AllocZeroedFromSlotheap(&world->slotheapXp);
        *pxpNew = *list;
        *ppxp = pxpNew;
        list = list->pxpNext;
        ppxp = &pxpNew->pxpNext;
    } while (list);
    return pxpFirst;
}

/* 1 when side 0 of pxp is ppart (so the other side is 1), else 0. */
static inline int IxpsOther(XP *pxp, SOPART *ppart) { int one = 1; int fNe = pxp->axps[0].ppart != ppart; return one - fNe; }
/* Asks ptree's root object for its own contacts with ppart (SO slot 60, AddCustomXps). Parts with zap kinds (zapKindFlags) collect them
   apart: each one whose other side's object passes FindZapResponder for those kinds is copied onto ptreeOther's zap list,
   then the list goes to *ppxpFirst. */
// Asks the zap responder of the other object (FindZapResponder, slot 0x3C) for its contacts and appends them.
// (inferred)
// Also known as: MarkOwnContacts (upstream readable / Sly 1 name; MarkOwnContacts__FP3OXAT0P3SOEPP2XPiPvN25).
extern "C" void AddZapResponderXps(SW *psw, SOTREE *ptree, SOTREE *ptreeOther, SOPART *ppart, int cbsp, BSP *absp, BSP *pbsp,
                              XP **ppxpFirst)
{
    XP *pxpFirst = 0;
    XP *pxp;
    ptree->root->AddCustomXps(ppart, cbsp, absp, pbsp, ppart->zapKindFlags ? &pxpFirst : ppxpFirst);
    if (pxpFirst) { pxp = pxpFirst; do {
        if (FindZapResponder(pxp->axps[IxpsOther(pxp, ppart)].pso, ppart->zapKindFlags)) {
            XP *pxpNew = (XP *)AllocZeroedFromSlotheap(&psw->slotheapXp);
            *pxpNew = *pxp;
            pxpNew->pxpNext = ptreeOther->firstZapContact;
            ptreeOther->firstZapContact = pxpNew;
        }
    } while ((pxp = pxp->pxpNext) != 0);
    AppendXpList(psw, pxpFirst, ppxpFirst); }
}

/* AddSoXps: the stack scope sits at sp+2 (ach is never touched; the target builds `addiu v0,sp,2`), as in
   grab's ClearJtDetectionOutOfSight. This form exists to reproduce the original codegen. */
struct SCOPE2 { char ach[2]; STACKSCOPE scope; };
struct CBSP { int c; };
// Allocation request: byte count and alignment.
struct ALLOCREQ { int cb, cbAlign; ALLOCREQ(int c, int a) { cb = c; cbAlign = a; } };
extern BSP D_002BBB20;      /* the "outside" leaf (bsp.cpp) */
/* A tree's parts (SOTREE::parts through SOPART::dle) and its flag word (+0x70: so.h's isInFirstQueue..isDone bit-fields and
   more; AddSoXps tests bit 5 as a mask). */
#define PPART_FIRST(ptree) ((SOPART *)(ptree)->parts.head)
#define PPART_NEXT(ppart) ((SOPART *)(ppart)->dle.next)
#define GRF_SOTREE(ptree) (*(ulong *)((char *)(ptree) + 0x70))
extern "C" {
void *AllocFromStack(const ALLOCREQ &req);
int PruneBspP(BSP *pbsp, BSP *pbspLimit, qword pos, float sRadius, int *pcbspLeft, BSP **ppbspNext) __asm__("PruneBspBySphere");   /* AddSoXps's view: position first */
extern "C++" { void MarkSoContactsSweptVertices(SW *psw, SOPART *ppart, SOPART *ppartOther, XP **ppxp) __asm__("MarkSoContactsSweptVertices"); }
extern "C++" { void MarkSoContactsSphereBsp(SW *psw, SOPART *ppart, SOPART *ppartOther, int cbsp, BSP *absp, XP **ppxp) __asm__("MarkSoContactsSphereBsp"); }
extern "C++" { void MarkSoContactsBspBsp(SW *psw, SOPART *ppart, SOPART *ppartOther, int cbsp, BSP *absp, BSP *pbspLast, XP **ppxp) __asm__("MarkSoContactsBspBsp"); }
}
// Squared length of the xyz part of v as a float.
static inline float LengthSq(qword v)
{
    qword d, one;
    float g;
    __asm__("vmul.xyz %0,%2,%2\n\tvaddw.x %1,$vf0,$vf0w\n\tvadday.x $ACC,%0,%0y\n\tvmaddz.x %0,%1,%0z" : "=j"(d), "=&j"(one) : "j"(v));
    __asm__("qmfc2.ni %0,%1" : "=r"(g) : "j"(d));
    return g;
}
/* Pairs every part of the larger tree with every part of the smaller one and marks their contacts: swept vertices
   when either part moved, else sphere/sphere, sphere/BSP or BSP/BSP. Parts with zap kinds (unknownFlag10A / grfzpk) collect
   their contacts apart and hand them to the trees' zap lists (copies where both sides keep them). */
// Builds all contacts between two objects: orders them by radius, then for each pair of parts dispatches to the
// sphere/sphere, sphere/BSP, BSP/BSP or swept-vertex marker, and adds the zap responders' contacts.
// Also known as: PxpMarkOxaContacts (upstream readable / Sly 1 name; PxpMarkOxaContacts__FP3OXAT0P2SO).
extern "C" XP *AddSoXps(SW *world, SOTREE *smallTree, SOTREE *largeTree)
{
    XP *pxpFirst = 0;
    SOTREE *ptreeT = smallTree;
    if (smallTree->boundsRadius > largeTree->boundsRadius) {
        smallTree = largeTree;
        largeTree = ptreeT;
    }
    int cnodeMax = 0;
    SOPART *ppart;
    SOPART *ppartOther;
    for (ppart = PPART_FIRST(largeTree); ppart; ppart = PPART_NEXT(ppart))
        if (cnodeMax < ppart->bspc.numNodesAllocated)
            cnodeMax = ppart->bspc.numNodesAllocated;
    SCOPE2 stackscope;
    cnodeMax = MinInt(cnodeMax, 0x1555);
    BSP *absp = (BSP *)AllocFromStack(ALLOCREQ(cnodeMax * sizeof(BSP), 0x10));
    for (ppart = PPART_FIRST(largeTree); ppart; ppart = PPART_NEXT(ppart)) {
        int fFilter = ppart->zapKindFlags != 0;
        if (ppart->unknownFlag10A && !fFilter)
            continue;
        BSP *pbspFirst;
        int cnode;
        BSP *pbspLast;
        int status = 2;
        if (ppart->unknownFlag10B || (GRF_SOTREE(smallTree) & 0x20)) {
            pbspLast = 0;
            pbspFirst = 0;
            cnode = 0;
        } else {
            CBSP query;
            query.c = cnodeMax;
            pbspLast = absp;
            pbspFirst = absp;
            status = PruneBspP(ppart->bspc.nodes, (BSP *)0x7FFFFFFF, smallTree->boundsCenter, smallTree->boundsRadius, &query.c, &pbspLast);
            if (status == 1)
                continue;
            if (status == 0) {
                cnode = 0;
                pbspLast = &D_002BBB20;
                pbspFirst = &D_002BBB20;
            } else {
                cnode = pbspLast - pbspFirst;
                pbspLast--;
            }
        }
        for (ppartOther = PPART_FIRST(smallTree); ppartOther; ppartOther = PPART_NEXT(ppartOther)) {
            int fFilterOther = ppartOther->zapKindFlags != 0;
            if (ppartOther->unknownFlag10A && !fFilterOther)
                continue;
            if (ppart->unknownFlag10A && ppartOther->unknownFlag10A)
                continue;
            XP *pxpTemp = 0;
            XP **ppxp = &pxpFirst;
            if (fFilter) {
                int fMatch = FindZapResponder(ppartOther->owner, ppart->zapKindFlags) != 0;
                if (ppart->unknownFlag10A) {
                    if (!fMatch)
                        continue;
                    ppxp = &pxpTemp;
                } else if (fMatch) {
                    if (!ppartOther->unknownFlag10A)
                        ppxp = &pxpTemp;
                }
            }
            if (fFilterOther) {
                int fMatch = FindZapResponder(ppart->owner, ppartOther->zapKindFlags) != 0;
                if (ppartOther->unknownFlag10A) {
                    if (!fMatch)
                        continue;
                    ppxp = &pxpTemp;
                } else if (fMatch && !ppart->unknownFlag10A) {
                    ppxp = &pxpTemp;
                }
            }
            XP *pxpPrev = *ppxp;
            if (ppartOther->worldStamp != ppartOther->meshStamp || ppart->worldStamp != ppart->meshStamp) {
                if (ppartOther->unknown1C < ppartOther->meshLocal.numPositions)
                    MarkSoContactsSweptVertices(world, ppartOther, ppart, ppxp);
                if (ppart->unknown1C < ppart->meshLocal.numPositions)
                    MarkSoContactsSweptVertices(world, ppart, ppartOther, ppxp);
            }
            if (*ppxp == pxpPrev) {
                if (ppart->unknownFlag10B) {
                    if (ppartOther->unknownFlag10B)
                        MarkSoContactsSphereSphere(world, ppartOther, ppart, ppxp);
                    else
                        MarkSoContactsSphereBsp(world, ppart, ppartOther, 0, 0, ppxp);
                } else if (ppartOther->unknownFlag10B) {
                    int fFar = 0;
                    if (!status || 1.0f < LengthSq(VuSub(QW_XF_POS(ppart->transform), smallTree->boundsCenter)) ||
                        1.0f < __builtin_fabsf(ppart->radius - smallTree->boundsRadius))
                        fFar = 1;
                    MarkSoContactsSphereBsp(world, ppartOther, ppart, fFar ? 0 : cnode, fFar ? 0 : pbspFirst, ppxp);
                } else {
                    MarkSoContactsBspBsp(world, ppartOther, ppart, cnode, pbspFirst, pbspLast, ppxp);
                }
            }
            if (pxpTemp) {
                int fCopied = 0;
                if (fFilter) {
                    if (ppart->unknownFlag10A)
                        AppendXpList(world, pxpTemp, &largeTree->firstZapContact);
                    else if (!ppartOther->unknownFlag10A) {
                        fCopied = 1;
                        AppendXpList(world, CloneXpList(world, pxpTemp), &largeTree->firstZapContact);
                    }
                }
                if (fFilterOther) {
                    if (ppartOther->unknownFlag10A)
                        AppendXpList(world, pxpTemp, &smallTree->firstZapContact);
                    else if (!ppart->unknownFlag10A) {
                        fCopied = 1;
                        AppendXpList(world, CloneXpList(world, pxpTemp), &smallTree->firstZapContact);
                    }
                }
                if (fCopied)
                    AppendXpList(world, pxpTemp, &pxpFirst);
            }
        }
        if (!ppart->unknownFlag10A)
            AddZapResponderXps(world, smallTree, largeTree, ppart, ppart->bspc.numNodesAllocated, ppart->bspc.nodes, ppart->bspc.nodes, &pxpFirst);
    }
    for (ppartOther = PPART_FIRST(smallTree); ppartOther; ppartOther = PPART_NEXT(ppartOther))
        if (!ppartOther->unknownFlag10A)
            AddZapResponderXps(world, largeTree, smallTree, ppartOther, ppartOther->bspc.numNodesAllocated, ppartOther->bspc.nodes, ppartOther->bspc.nodes, &pxpFirst);
    return pxpFirst;
}



// 0x2564C0: the empty VECTOR constructor's body (AddEmbeddedVertexXps's VECTOR temporaries call it): inline, defined after
// its callers, so g++ calls it and emits the out-of-line copy (linkonce), as the original did. It keeps its C symbol
// (bsp, robottank and waypoint call it by that name).
namespace M_func_001A87F0 {
// Identity "constructor" for VECTOR: returns pvec unchanged (keeps the original's call shape).
extern "C" { inline VECTOR *DefaultConstructVector(VECTOR *pvec)
{
    return pvec;
} }
} // namespace M_func_001A87F0
