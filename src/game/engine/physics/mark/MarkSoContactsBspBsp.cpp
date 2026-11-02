// MarkSoContactsBspBsp: edge/edge and vertex/face contacts between two clipped shapes.
// ClsgClipSoAllEdges + ClsgClipSoBspEdges clip each shape's edges against the other, CategorizeAlsg sorts each side's LSGs, func_001A87F0 adds the
// vertex contacts of both sides; then every free LSG of side 0 either follows the CplsgFindEdgePath edge path of side 1 (closest points of the
// two edges via func_0011EA60, kind 3) or, when no side-1 LSG shares a surface with its edge, contacts the nearer endpoint against the
// farther plane (kind 1). With no new contact and one side empty, a kind-13 contact at the shape position is added.
// LSG / EDGE / SURF layouts and names as in Lilly's matched ClsgClipEdgeToBsp port (src/game/core/math/bsp/func_00118668.cpp).
// How it matched (w4lane4 -> w5lane2):
//  - Separate Sly-1-style local arrays (one frame base each, shared by gcse with the call arguments); the side-1 loop indexes &aalsg[1][i].
//  - Lilly's QwMix(VuFloat(1 - t), a, VuFloat(t), b) lerps reproduce the dead VU_FLOAT stores at 0x6900.
//  - The four arrays aaipos[0/1] and aaplsg[0/1] are gcse PRE reaching registers that get spill slots in pseudo order = gcse hash-bucket
//    order, so the slot order depends on the expression table size ((insns / 2) | 1, needs 219). The by-value VV / VU_FLOAT helper
//    parameters (struct copies that survive cse1 and vanish at register allocation) give exactly that count; GFromVf taking a VU_FLOAT
//    in Length and PlaneDist was the last step (215 -> 219).
typedef int qword __attribute__((mode(TI)));
union U { qword q; float f[4]; };
struct VV { qword q; VV() {} VV(const qword &v) { q = v; } operator qword() const { return q; } };
struct SCALAR;
extern "C" SCALAR *func_002564B8(SCALAR *);
struct SCALAR { qword q; SCALAR() { (void)func_002564B8(this); } };
struct SCENE;
struct CONTACT;
struct SPACE { char pad0[0x70]; U position; };
struct EDGE { unsigned short aipos[2], aisurf[2]; EDGE *pedgeNext; EDGE *pedgeOtherNext; };
struct SURF { U plane; EDGE *pedge; EDGE *pedgeOther; unsigned int grf; int ipos; };
struct SHAPE { SPACE *space; void *owner; char pad8[0x8C]; U *apos; int pad98; SURF *asurf; int cedge; EDGE *aedge; };
struct NODE { unsigned int left, right; SURF *psurf; };
struct LSG { U apos[2], anormal[2]; float au[2]; int lsgk; SURF *apsurf[2]; EDGE *pedge; int iiposSwap; LSG *plsgNext; };
struct XP { char pad0[0x9C]; int kind; void *first, *second; int padA8; XP *next; };
extern "C" {
extern U D_002A2DC0;
int ClsgClipSoAllEdges(SCENE *, SHAPE *, SHAPE *, NODE *, int, LSG *) __asm__("ClsgClipSoAllEdges__FP2SOT0P3BSPiP3LSG");
int ClsgClipSoBspEdges(SCENE *, SHAPE *, SHAPE *, int, NODE *, int, LSG *);
void CategorizeAlsg(int, LSG *, int *, int *, LSG **, LSG **) __asm__("CategorizeAlsg__FiP3LSGPiT2PP3LSGT4");
void func_001A87F0(SCENE *, SHAPE *, SHAPE *, int, int *, LSG **, CONTACT **);
int CplsgFindEdgePath__FP4SURFiiP3LSGPP3LSG(SURF *, int, int, LSG *, LSG **);
int func_0011EA60(qword, qword, qword, qword, float *, U *);
void PxpAdd(SCENE *, SHAPE *, SHAPE *, VV, float, VV, int, void *, void *, CONTACT **);
}

static inline VV Sub(VV a, VV b) { qword r; __asm__("vsub.xyzw %0,%1,%2" : "=j"(r) : "j"(a), "j"(b)); return r; }

struct VU_FLOAT { qword qw; };
static inline VU_FLOAT VuFloat(float g)
{
    VU_FLOAT v;
    __asm__("qmtc2.ni %1,%0" : "=j"(v.qw) : "r"(g));
    return v;
}
static inline VV QwMix(VU_FLOAT g1, VV qw1, VU_FLOAT g2, VV qw2)
{
    qword qw;
    __asm__("vmulax.xyzw $ACC,%1,%3x\n\tvmaddx.xyzw %0,%2,%4x" : "=j"(qw) : "j"(qw1), "j"(qw2), "j"(g1.qw), "j"(g2.qw));
    return qw;
}
/* gcc supplies the qmfc2 for the "r" operand; the repeated "j" operand is allocation-only (keeps the value VU class). */
static inline float GFromVf(VU_FLOAT v) { float g; __asm__("mtc1 %1,%0" : "=f"(g) : "r"(v.qw), "j"(v.qw)); return g; }
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
static inline void Closest(VV a, VV b, VV c, VV d, float *au, U *normal)
{
    (void)func_0011EA60(a, b, c, d, au, normal);
}

/** @sly2-annot addr=0x1A8B50
 * Purpose: Edge/edge and vertex/face contacts between two BSP shapes: clip edges both ways, CategorizeAlsg,
 *   embedded vertex XPs, edge paths and closest points (kind 3)
 * Info: proposed name MarkSoContactsBspBsp (T1); TU mark
 * Confidence: high [T1 0.85] - evidence: static analysis; sources: r4b_rest+verdicts_A2+verdicts_followups
 * See: docs/engine/physics-collision.md#7-key-functions,
 *   docs/reference/functions/physics-collision.md#mark-0x1a7c18-0x1aa978
 * @end-sly2-annot */
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
    U normal;
    U apos[2];
    CONTACT *old = *out;
    aclsg[0] = ClsgClipSoAllEdges(scene, b, a, last, 128, aalsg[0]);
    int clsg1 = ClsgClipSoBspEdges(scene, a, b, nodeCount, nodes, 128, aalsg[1]);
    aclsg[1] = clsg1;
    if (!aclsg[0] && !clsg1)
        return;
    {
        int i;
        for (i = 0; i < 2; i++)
            CategorizeAlsg(aclsg[i], aalsg[i], &acpos[i], aaipos[i], aaplsg[i], &aplsgBoth[i]);
    }
    func_001A87F0(scene, a, b, acpos[0], aaipos[0], aaplsg[0], out);
    func_001A87F0(scene, b, a, acpos[1], aaipos[1], aaplsg[1], out);
    for (LSG *plsg = aplsgBoth[0]; plsg; plsg = plsg->plsgNext) {
        if (!plsg->apsurf[0] || !plsg->apsurf[1])
            continue;
        int clsgPath = CplsgFindEdgePath__FP4SURFiiP3LSGPP3LSG(b->asurf, plsg->apsurf[0] - b->asurf, plsg->apsurf[1] - b->asurf, aplsgBoth[1], aplsgPath);
        if (clsgPath > 0) {
            int j;
            for (j = 0; j < clsgPath; j++) {
                LSG *plsgOther = aplsgPath[j];
                qword left0 = a->apos[plsg->pedge->aipos[0]].q, left1 = a->apos[plsg->pedge->aipos[1]].q;
                qword right0 = b->apos[plsgOther->pedge->aipos[0]].q, right1 = b->apos[plsgOther->pedge->aipos[1]].q;
                Closest(left0, left1, right0, right1, au, &normal);
                apos[0].q = QwMix(VuFloat(1.0f - au[0]), left0, VuFloat(au[0]), left1);
                apos[1].q = QwMix(VuFloat(1.0f - au[1]), right0, VuFloat(au[1]), right1);
                float g = -Length(Sub(apos[0].q, apos[1].q));
                PxpAdd(scene, a, b, apos[0].q, g, normal.q, 3, plsg->pedge, plsgOther->pedge, out);
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
            U *ppos = a->apos + plsg->pedge->aipos[ipos];
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
            PxpAdd(scene, a, b, pos, g, plane, 1, ppos, psurf, out);
        }
    next:;
    }
    if (old == *out && (!aclsg[0] || !aclsg[1]))
        PxpAdd(scene, a, b, a->space->position.q, 0.0f, D_002A2DC0.q, 13, 0, 0, out);
}
