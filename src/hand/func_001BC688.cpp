// func_001BC688: clips the segment pos1-pos2 to the convex volume pcnvo (its cplane - 1 planes, built by func_001BD400)
// and writes the parts of it OUTSIDE the volume to alsg (at most clsgMax LSGs); returns how many. Convex-volume twin of
// ClsgClipEdgeToBsp (func_00118668), same LSG record (0x60 bytes: apos[2], anormal[2], au[2], apsurf[2]).
// gIn / gOut = where the segment enters / leaves the volume (0 = pos1, 1 = pos2), normalIn / normalOut = those planes.
// If the segment never enters (gOut <= gIn) it is emitted whole; else [0, gIn] and [gOut, 1] are emitted with the
// negated plane normal at the cut.
// How it matches (T1830, no pins; lilly5/lilly7 were the earlier near-misses, see notes/1B0000.md):
// - the game keeps a register copy of the plane for the else branch (`lq s1,0x70` in the 2nd ctor's delay slot). A plain
//   `qword normal = plane` dies in cse/gcse copy-propagation; constructing a VU_VECTOR from it (inline ctor taking a
//   const qword &) keeps the copy, and the other uses (dot asms, if branch) read the spilled plane at sp+0x70.
// - in the else branch `normalIn = normal.qw` comes AFTER the gIn update: that makes the copy live 2 insns longer, so its
//   global-alloc priority drops below i's (i gets s0, the copy s1, as in the game).
// - GXOfQw: the qmfc2 after the 2nd dot product is a compiler move (unit vu0, so the sqc2 home stores don't pair with it)
//   and the mtc1 is asm. Its extra "j" inputs are allocation-only: the duplicate "j"(vf) keeps dot2 VU-class (else it is
//   stored with sq), the two "j"(vfLive) raise vfOne2's local-alloc priority so it gets vf1 like vfOne1.
typedef int qword __attribute__((mode(TI)));
struct VU_QWORD { qword qw; VU_QWORD(); };
struct VU_FLOAT { qword qw; };
struct VU_VECTOR { qword qw; VU_VECTOR(const qword &q) { qw = q; } };
struct LSG {
    qword apos[2];
    qword anormal[2];
    float au[2];
    int lsgk;
    void *apsurf[2];
    void *pedge;
    int iiposSwap;
    LSG *plsgNext;
};
struct CNVO { char pad[0xE8]; int cplane; char pad2[0x1EC - 0xEC]; qword *aplane; };
extern "C" {
void func_001BD400(CNVO *p);
void *memset(void *, int, unsigned);
extern float D_002C0AB4;
}
static inline VU_FLOAT VuFloat(float g)
{
    VU_FLOAT v;
    __asm__("qmtc2.ni %1, %0" : "=j"(v.qw) : "r"(g));
    return v;
}
static inline float GFromVf(qword qw)
{
    float g;
    __asm__("qmfc2.ni %0, %1" : "=r"(g) : "j"(qw));
    return g;
}
static inline float GXOfQw(const VU_QWORD &vf, const VU_QWORD &vfLive)
{
    float g;
    __asm__("mtc1 %1,%0" : "=f"(g) : "r"(vf.qw), "j"(vf.qw), "j"(vfLive.qw), "j"(vfLive.qw)); /* "j" inputs: allocation only (see header) */
    return g;
}
static inline qword QwMix(VU_FLOAT g1, const qword &qw1, VU_FLOAT g2, const qword &qw2)
{
    qword qw;
    __asm__("vmulax.xyzw $ACC, %1, %3x\n\tvmaddx.xyzw %0, %2, %4x" : "=j"(qw) : "j"(qw1), "j"(qw2), "j"(g1.qw), "j"(g2.qw));
    return qw;
}
static inline qword QwZero() { qword qw; __asm__("vsub.xyzw %0, $vf0, $vf0" : "=j"(qw)); return qw; }
static inline qword QwNeg(qword v) { qword qw; __asm__("vsub.xyzw %0, $vf0, $vf0\n\tvsub.xyzw %0, %0, %1" : "=&j"(qw) : "j"(v)); return qw; }

/** @sly2-annot addr=0x1BC688
 * Purpose: Clip segment to the convex volume and write the parts outside it (LSG 0x60)
 * Info: proposed name ClsgClipEdgeToCnvo (T1); TU tu_1BC050
 * Confidence: high [T1 0.85] - evidence: static analysis; sources: r5b_rest+verdicts_followups
 * See: docs/engine/physics-collision.md#7-key-functions,
 *   docs/reference/functions/physics-collision.md#tu_1bc050-0x1bc050-0x1be398,
 *   docs/reference/functions/physics-collision.md#path-0x1bf110-0x1c2920
 * @end-sly2-annot */
extern "C" int func_001BC688(CNVO *pcnvo, qword pos1, qword pos2, int clsgMax, LSG *alsg)
{
    func_001BD400(pcnvo);
    float gIn = 0.0f;
    qword normalIn = QwZero();
    float gOut = 1.0f;
    qword normalOut = normalIn;
    int i;
    for (i = 0; i < pcnvo->cplane - 1; i++) {
        qword plane = pcnvo->aplane[i];
        VU_VECTOR normal(plane);
        VU_QWORD dot1;
        VU_QWORD vfOne1;
        __asm__("vmul.xyz %0, %2, %3\n\tvaddw.x %1, $vf0, $vf0w\n\tvadday.x $ACC, %0, %0y\n\tvmaddaz.x $ACC, %1, %0z\n\tvmsubw.x %0, %1, %2w"
                : "=j"(dot1), "=j"(vfOne1) : "j"(plane), "j"(pos1));
        float g1 = GFromVf(dot1.qw);
        VU_QWORD dot2;
        VU_QWORD vfOne2;
        __asm__("vmul.xyz %1, %2, %3\n\tvaddw.x %0, $vf0, $vf0w\n\tvadday.x $ACC, %1, %1y\n\tvmaddaz.x $ACC, %0, %1z\n\tvmsubw.x %1, %0, %2w"
                : "=j"(vfOne2), "=j"(dot2) : "j"(plane), "j"(pos2));
        float g2 = GXOfQw(dot2, vfOne2);
        g1 += D_002C0AB4;
        g2 += D_002C0AB4;
        if (g1 < 0.0f && g2 < 0.0f)
            continue;
        if (g1 > 0.0f && g2 > 0.0f) {
            gIn = gOut = 0.0f;
            break;
        }
        float t = g1 / (g1 - g2);
        if (g1 < 0.0f) {
            gOut = (gOut < t) ? gOut : t;
            normalOut = plane;
        } else {
            gIn = (t < gIn) ? gIn : t;
            normalIn = normal.qw;
        }
        if (gOut <= gIn)
            break;
    }
    int clsg = 0;
    LSG *plsg;
    if (gOut <= gIn && clsg < clsgMax) {
        plsg = &alsg[clsg++];
        memset(plsg, 0, sizeof(LSG));
        plsg->apos[0] = pos1;
        plsg->au[1] = 1.0f;
        plsg->apos[1] = pos2;
        plsg->au[0] = 0.0f;
    } else {
        if (gIn > 0.0f && clsg < clsgMax) {
            plsg = &alsg[clsg++];
            memset(plsg, 0, sizeof(LSG));
            plsg->apos[0] = pos1;
            plsg->apos[1] = QwMix(VuFloat(1.0f - gIn), pos1, VuFloat(gIn), pos2);
            plsg->anormal[1] = QwNeg(normalIn);
            plsg->au[0] = 0.0f;
            plsg->au[1] = gIn;
            plsg->apsurf[1] = pcnvo;
        }
        if (gOut < 1.0f && clsg < clsgMax) {
            plsg = &alsg[clsg++];
            memset(plsg, 0, sizeof(LSG));
            plsg->apos[0] = QwMix(VuFloat(1.0f - gOut), pos1, VuFloat(gOut), pos2);
            plsg->anormal[0] = QwNeg(normalOut);
            plsg->apos[1] = pos2;
            plsg->au[0] = gOut;
            plsg->au[1] = 1.0f;
            plsg->apsurf[0] = pcnvo;
        }
    }
    return clsg;
}
