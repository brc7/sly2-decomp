// FSnapPosToSurfaceAbove: ported from Lilly's personal Sly 2 decomp (src/P2/guardbhv_goto.c), reduced to the declarations it needs.
// Every global is named by its address; the function keeps its readable name and gets the team symbol through __asm__.
// Globals: D_002BE750 (D_002BE750), D_002BE754 (D_002BE754), D_002A2DC0 (g_normalZ)
// 0x0017D608 FSnapPosToSurfaceAbove__FPUx  (our name; was FUN_0017D608)
// A line from the position straight up (5000 long) against the world's objects: when it hits one, the position is moved to the end of the
// hit piece and 1 is returned. BHV191::Start (0x17E9B0) tries it when GuardSpawnerGroundSnap finds no floor under the target.
#include "sly2/basic.h"
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
struct STACKSCOPE
{
    STACKSCOPE();
    ~STACKSCOPE();
};
struct VU_FLOAT
{
    qword qw;
};
static inline VU_FLOAT VuFloatFromMemWord(int n)
{
    VU_FLOAT v;
    __asm__("qmtc2 %1, %0" : "=j"(v.qw) : "rV"(n));
    return v;
}
extern float D_002BE750;
extern float D_002BE754;
extern qword D_002A2DC0;
extern "C" {
void func_001B44D8(int *pcpso, SOPART ***papso);
}
extern "C" {
SOPART *PsoHitTestLineObjects(int grfhtl, qword pos0, qword pos1, int cpso, SOPART **apso, LSG *plsgBest);
}
static inline qword QwAddScaled(qword qwBase, qword qwVec, VU_FLOAT gScale)
{
    qword qw;
    __asm__("vadda.xyzw $ACC, %1, $vf0\n\tvmaddx.xyzw %0, %2, %3x" : "=j"(qw) : "j"(qwBase), "j"(qwVec), "j"(gScale.qw));
    return qw;
}
int FSnapPosToSurfaceAbove(qword *ppos) __asm__("FSnapPosToSurfaceAbove");
int FSnapPosToSurfaceAbove(qword *ppos)
{
    STACKSCOPE scope;
    int cpso;
    SOPART **apso;
    func_001B44D8(&cpso, &apso);
    qword posBottom = QwAddScaled(*ppos, D_002A2DC0, VuFloatFromMemWord(*(int *)&D_002BE750));
    qword posTop = QwAddScaled(*ppos, D_002A2DC0, VuFloatFromMemWord(*(int *)&D_002BE754));
    LSG lsg;
    if (PsoHitTestLineObjects(0, posBottom, posTop, cpso, apso, &lsg))
    {
        *ppos = lsg.apos[1];
        return 1;
    }
    return 0;
}
