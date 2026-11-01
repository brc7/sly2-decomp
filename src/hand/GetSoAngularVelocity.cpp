// GetSoAngularVelocity: ported from Lilly's personal Sly 2 decomp (src/P2/so_motion.c), reduced to the declarations it needs.
// Every global is named by its address; the function keeps its readable name and gets the team symbol through __asm__.
// Globals: D_002A2DD0 (g_qwZero)
// 0x001F4558 GetAngularVelocity__2SO  (MATCHES Sly 2, T0906)
// Virtual 33: the angular velocity of the rotation XF (xfk 6) in world space, zero without one. The chain's rate matrix (MatDmatOfXfChain,
// MATCH) is turned by N * inverse(matWorld) (N = matWorld with its rotation orthonormalised: the scale is taken out) and the skew-symmetric
// part (m12, m20, m01) of the product is the angular velocity, packed into a quadword with the MMI instructions pextlw / pcpyld.
// HOW IT IS WRITTEN: (1) the result of MatDmatOfXfChain is used through `&MatDmatOfXfChain(pxf, NULL)` (taking the address of the temporary:
// the compiler warns and keeps the pointer in a register across the call, s0; that is what makes the copy of the temporary read
// `lq 0x60($sp)` / `lq 0x10($s0)` / `lq 0x20($s0)`, rows 1 and 2 through the pointer); (2) matNorm and matRate are declared before the call
// (frame: matNorm 0x00, matRate 0x30, the temporary 0x60) and the product is an in-place assignment; pmatWorld is a pointer variable (s1);
// (3) THE PACKING IS ONE asm STATEMENT that contains the three mfc1 as well (the floats come in as "f" operands, two scratch GPRs):
// with the mfc1 outside (the compiler's own, two asm statements) the scheduler sees gZ's path to the end one instruction shorter than
// gX's and gY's, gives the row-0 chain of the product a lower priority and computes the rows in the order 1, 2, 0 (51.6%); with ONE
// statement the three chains tie and the rows come out 0, 1, 2 as in the game. (Same recipe as the fused qmfc2 + mtc1 of vu0.h.)
#include "sly2/basic.h"
#include "sly2/lo.h"
#include "sly2/so.h"
MATRIX3 InvertMatrix3(MATRIX3 *pmat);
struct VU_VECTOR4
{
    qword qw;
};
inline MATRIX3 operator*(const MATRIX3 &matL, const MATRIX3 &matR) return mat
{
    qword qwL0 = *(qword *)matL.mat[0];
    qword qwL1 = *(qword *)matL.mat[1];
    qword qwL2 = *(qword *)matL.mat[2];
    VU_VECTOR4 v0;
    VU_VECTOR4 v1;
    VU_VECTOR4 v2;
    __asm__("vmulax.xyz $ACC, %1, %4x\n\tvmadday.xyz $ACC, %2, %4y\n\tvmaddz.xyz %0, %3, %4z"
            : "=j"(v0.qw) : "j"(qwL0), "j"(qwL1), "j"(qwL2), "j"(((VU_VECTOR4 *)matR.mat[0])->qw));
    *(VU_VECTOR4 *)mat.mat[0] = v0;
    __asm__("vmulax.xyz $ACC, %1, %4x\n\tvmadday.xyz $ACC, %2, %4y\n\tvmaddz.xyz %0, %3, %4z"
            : "=j"(v1.qw) : "j"(qwL0), "j"(qwL1), "j"(qwL2), "j"(((VU_VECTOR4 *)matR.mat[1])->qw));
    *(VU_VECTOR4 *)mat.mat[1] = v1;
    __asm__("vmulax.xyz $ACC, %1, %4x\n\tvmadday.xyz $ACC, %2, %4y\n\tvmaddz.xyz %0, %3, %4z"
            : "=j"(v2.qw) : "j"(qwL0), "j"(qwL1), "j"(qwL2), "j"(((VU_VECTOR4 *)matR.mat[2])->qw));
    *(VU_VECTOR4 *)mat.mat[2] = v2;
}
struct ACT
{
      int ack;
      SO *pso;
      XF *pxf;
      int nPriority;
      ACT *pactNext;
      int nRole;
    ACT() {}
    ACT(int ack) : ack(ack) {}
};
struct SUBGLOBX
{
      VECTOR posCenter;
      float sRadius;
      float gUnk10;
      uint grfUnk14;
};
extern qword D_002A2DD0;
void NormalizeRotateMatrix3(MATRIX3 *pmat);
MATRIX4 MatDmatOfXfChain(XF *pxfFrom, XF *pxfTo);
qword SO::GetAngularVelocity()
{
    XF *pxf = pxfUnk5c;
    if (!pxf || pxf->xfk != 6)
    {
        return D_002A2DD0;
    }
    MATRIX3 matNorm;
    MATRIX3 matRate;
    MATRIX4 *pmatDmat = &MatDmatOfXfChain(pxf, 0);
    matRate = *(MATRIX3 *)pmatDmat;
    MATRIX3 *pmatWorld = (MATRIX3 *)&pxf->matWorld;
    matNorm = *pmatWorld;
    NormalizeRotateMatrix3(&matNorm);
    matRate = matNorm * InvertMatrix3(pmatWorld) * matRate;
    float gX = matRate.mat[1][2];
    float gY = matRate.mat[2][0];
    float gZ = matRate.mat[0][1];
    qword qwXYZ;
    int iTmp;
    __asm__("mfc1 %0, %2\n\tmfc1 %1, %3\n\tpextlw %0, %1, %0\n\tmfc1 %1, %4\n\tpcpyld %0, %1, %0"
            : "=&r"(qwXYZ), "=&r"(iTmp) : "f"(gX), "f"(gY), "f"(gZ));
    VECTOR4 vecW;
    *(qword *)&vecW = qwXYZ;
    return *(qword *)&vecW;
}
