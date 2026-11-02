// func_00118668: ported from Lilly's personal Sly 2 decomp (src/P2/bsp.c), reduced to the declarations it needs.
// Every global is named by its address; the function keeps its readable name and gets the team symbol through __asm__.
// 0x00118668 ClsgClipEdgeToBsp__FP3BSPT0UxUxiP3LSG  (MATCHES Sly 2; Sly 1's ClsgClipEdgeToBsp with qword positions, no mpibspn and the node limit)
// Clips the edge pos1 - pos2 to the BSP: the pieces of it on the negative side of every plane (= inside the volume) go to alsg (at most clsgMax),
// the return value is how many. A work stack of CEB (0x20 bytes: pbsp = node to look at, pbspNeg = the node whose negative side this piece is on,
// NULL = outside, g = where along the edge the piece starts (0 = pos1, 1 = pos2), psurf + normal = the plane it starts at) walks the tree: the piece
// [ceb.g, pceb->g] (ceb is the current one, pceb the top of the stack = where it ends) is cut by each node's plane (g1 / g2 = distance of pos1 / pos2 to
// it): both ends behind -> the negative child; both in front -> the positive child; otherwise it is split at t = g1 / (g1 - g2) (a new stack entry for the
// far part, pushed when t is inside the piece) and the near part goes on by the side of the middle (gMid, ties decided by the far end gTop).
// A leaf (pbsp == NULL) with pbspNeg set is a piece inside the volume: it is merged into the last LSG when it starts where that one ended
// (ceb.g == plsgLast->au[1]), else a new LSG (0x60 bytes, cleared) is made: apos[2] = pos1 * (1 - g) + pos2 * g, anormal[2], au[2], apsurf[2].
// How it matches (T1120): (1) the LOOP NEST. `while (ceb.pbsp == NULL)` inside `while (1)`, and pbsp is never copied to a local (every use re-reads ceb.pbsp
// from the stack: the game has no register for it). With the oracle's loop.c threshold (1 + n_non_fixed_regs = 95) the 0xAAAAAAAB constant of `plsgOut - alsg`
// (pointer difference of 0x60-byte elements = x * 0xAAAAAAAB >> 5) is hoisted out of the INNER loop only when that has < 93 real insns: then the multiply
// is strength-reduced there with a `mult` initialisation. The game's inner loop is bigger because its VU0 weights go through a helper with by-value VU_FLOAT
// parameters (QwMix: four parameter copies per call that cse2 removes later, 101 insns here): K is then hoisted by the OUTER pass together with the
// matching one of the return, and both counts become registers that step by 0x20 with initial value 0 (s5 / s6; the plsgLast giv s0 = s4 is the inner
// pass's only reduction). (2) `pceb = aceb;` first and every aceb[0] access through pceb (the game's s1 is both). (3) The third plane case is its own
// `else if (g1 > 0 && g2 > 0)`; the split case ends in an if / else whose arms are the same assignments again (jump2 merges the tails). (4) The dot
// products are the non-tied form (`%2w`: the plane is not overwritten); the first reads g1 with a plain qmfc2 (GFromVf: the sqc2 home store lands between
// qmfc2 and mtc1), the second is ONE asm with the qmfc2 + mtc1 inside and its outputs listed vfOne first: vfOne2 is then stored (and dies) before
// the branch and gets vf1, dot2 (stored after it) vf2.
#include "sly2/basic.h"
typedef unsigned  size_t;
extern "C" {
void* memset(void* ptr, int value, size_t num);
}
struct EDGE
{
    ushort aipos[2];
    ushort aisurf[2];
    EDGE *pedgeNext;
    EDGE *pedgeOtherNext;
};
struct SURF
{
    VECTOR normal;
    float gDot;
    EDGE *pedge;
    EDGE *pedgeOther;
    uint grf;
    int ipos;
};
struct BSP
{
    BSP *pbspNeg;
    BSP *pbspPos;
    SURF *psurf;
};
struct VU_QWORD
{
    qword qw;
    VU_QWORD();
};
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
struct CEB
{
      BSP *pbsp;
      BSP *pbspNeg;
      float g;
      SURF *psurf;
      qword normal;
};
struct VU_FLOAT
{
    qword qw;
};
static inline VU_FLOAT VuFloat(float g)
{
    VU_FLOAT v;
    __asm__("qmtc2 %1, %0" : "=j"(v.qw) : "r"(g));
    return v;
}
static inline float GFromVf(qword qw)
{
    float g;
    __asm__("qmfc2 %0, %1" : "=r"(g) : "j"(qw));
    return g;
}
static inline BSP *PbspBelow(BSP *pbsp, BSP *pbspLimit)
{
    if (pbsp >= pbspLimit)
    {
        pbsp = 0;
    }
    return pbsp;
}
static inline qword QwMix(VU_FLOAT g1, qword qw1, VU_FLOAT g2, qword qw2)
{
    qword qw;
    __asm__("vmulax.xyzw $ACC, %1, %3x\n\tvmaddx.xyzw %0, %2, %4x" : "=j"(qw) : "j"(qw1), "j"(qw2), "j"(g1.qw), "j"(g2.qw));
    return qw;
}
int ClsgClipEdgeToBsp(BSP *pbspRoot, BSP *pbspLimit, qword pos1, qword pos2, int clsgMax, LSG *alsg) __asm__("func_00118668");
/** @sly2-annot addr=0x118668
 * Purpose: Clips an edge against a BSP volume; returns inside segments (LSG list)
 * Info: proposed name ClsgClipEdgeToBsp (T1); TU bsp
 * Confidence: high [T1 0.90] - evidence: static analysis; sources: r1_116918+verdicts_A1
 * See: docs/engine/camera.md#5-key-functions, docs/engine/core-library.md#6-source-files,
 *   docs/engine/core-library.md#7-key-functions
 * @end-sly2-annot */
int ClsgClipEdgeToBsp(BSP *pbspRoot, BSP *pbspLimit, qword pos1, qword pos2, int clsgMax, LSG *alsg)
{
    CEB ceb;
    CEB aceb[32];
    CEB *pceb;
    LSG *plsgLast;
    LSG *plsgOut;
    pceb = aceb;
    plsgLast = 0;
    plsgOut = alsg;
    memset(&ceb, 0, sizeof(CEB));
    memset(pceb, 0, sizeof(CEB));
    ceb.pbsp = PbspBelow(pbspRoot, pbspLimit);
    pceb->g = 1.0f;
    while (1)
    {
        while (ceb.pbsp == 0)
        {
            if (ceb.pbspNeg != 0)
            {
                if (plsgLast != 0 && ceb.g == plsgLast->au[1])
                {
                    plsgLast->apos[1] = QwMix(VuFloat(1.0f - pceb->g), pos1, VuFloat(pceb->g), pos2);
                    plsgLast->anormal[1] = pceb->normal;
                    plsgLast->au[1] = pceb->g;
                    plsgLast->apsurf[1] = pceb->psurf;
                }
                else
                {
                    if (plsgOut - alsg >= clsgMax)
                    {
                        return clsgMax;
                    }
                    plsgLast = plsgOut;
                    plsgOut++;
                    memset(plsgLast, 0, sizeof(LSG));
                    plsgLast->apos[0] = QwMix(VuFloat(1.0f - ceb.g), pos1, VuFloat(ceb.g), pos2);
                    plsgLast->apos[1] = QwMix(VuFloat(1.0f - pceb->g), pos1, VuFloat(pceb->g), pos2);
                    plsgLast->anormal[0] = ceb.normal;
                    plsgLast->anormal[1] = pceb->normal;
                    plsgLast->au[0] = ceb.g;
                    plsgLast->au[1] = pceb->g;
                    plsgLast->apsurf[0] = ceb.psurf;
                    plsgLast->apsurf[1] = pceb->psurf;
                }
            }
            if (pceb <= aceb)
            {
                return plsgOut - alsg;
            }
            ceb = *pceb;
            pceb--;
        }
        qword plane = *(qword *)ceb.pbsp->psurf;
        VU_QWORD dot1;
        VU_QWORD vfOne1;
        __asm__("vmul.xyz %0, %2, %3\n\tvaddw.x %1, $vf0, $vf0w\n\tvadday.x $ACC, %0, %0y\n\tvmaddaz.x $ACC, %1, %0z\n\tvmsubw.x %0, %1, %2w"
                : "=j"(dot1), "=j"(vfOne1) : "j"(plane), "j"(pos1));
        float g1 = GFromVf(dot1.qw);
        VU_QWORD dot2;
        VU_QWORD vfOne2;
        float g2;
        int iScratch;
        __asm__("vmul.xyz %1, %4, %5\n\tvaddw.x %0, $vf0, $vf0w\n\tvadday.x $ACC, %1, %1y\n\tvmaddaz.x $ACC, %0, %1z\n\tvmsubw.x %1, %0, %4w\n\tqmfc2 %3, %1\n\tmtc1 %3, %2"
                : "=j"(vfOne2), "=j"(dot2), "=f"(g2), "=&r"(iScratch) : "j"(plane), "j"(pos2));
        if (g1 <= 0.0f && g2 <= 0.0f)
        {
            ceb.pbspNeg = ceb.pbsp;
            ceb.pbsp = PbspBelow(ceb.pbsp->pbspNeg, pbspLimit);
        }
        else if (g1 > 0.0f && g2 > 0.0f)
        {
            ceb.pbspNeg = 0;
            ceb.pbsp = PbspBelow(ceb.pbsp->pbspPos, pbspLimit);
        }
        else
        {
            float t = g1 / (g1 - g2);
            float gMid = g2 * ceb.g + (1.0f - ceb.g) * g1;
            float gTop = g2 * pceb->g + (1.0f - pceb->g) * g1;
            if (ceb.g < t && t < pceb->g)
            {
                pceb++;
                pceb->g = t;
                pceb->normal = plane;
                pceb->psurf = ceb.pbsp->psurf;
                if (gMid <= 0.0f)
                {
                    pceb->pbspNeg = 0;
                    pceb->pbsp = PbspBelow(ceb.pbsp->pbspPos, pbspLimit);
                }
                else
                {
                    pceb->pbspNeg = ceb.pbsp;
                    pceb->pbsp = PbspBelow(ceb.pbsp->pbspNeg, pbspLimit);
                }
            }
            if (gMid < 0.0f || (gMid == 0.0f && gTop <= 0.0f))
            {
                ceb.pbspNeg = ceb.pbsp;
                ceb.pbsp = PbspBelow(ceb.pbsp->pbspNeg, pbspLimit);
            }
            else
            {
                ceb.pbspNeg = 0;
                ceb.pbsp = PbspBelow(ceb.pbsp->pbspPos, pbspLimit);
            }
        }
    }
}
