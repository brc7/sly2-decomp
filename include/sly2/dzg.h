/* include/sly2/dzg.h -- DZ and DZG, the inequality-constraint solver (Sly 1's dzg: core/math/dzgcore.cpp), and
 * DZGXP : DZG, the solver set up over a group of contacts (engine/physics/dzg.cpp; built by engine/physics/solve.cpp).
 *
 * Representation (docs/guides/tu-merging.md "Representation decisions"): DZG is the 0x50-byte solver that dzgcore
 * works on and that SolveInequalities keeps on its stack (a bigger DZG changes that frame: 28 of 86 off, wave 6).
 * The contact solver's extra fields (+0x50..+0x64) are a derived struct, DZGXP, initialised by InitDzgXp after
 * InitDzg (the base initialiser) and kept on SolveSwGroup's stack; dzgcore's functions take either as a DZG. */
#ifndef SLY2_DZG_H
#define SLY2_DZG_H
#include "sly2/basic.h"     /* DL, DLE, qword */

/* A constraint (0x28): kind (1: always active), bounds, current value and residual, its row of the matrix. */
struct DZ
{
      int nKind;            /* +0x00 */
      float gMin;           /* +0x04 */
      float gMax;           /* +0x08 */
      float gC;             /* +0x0C solution */
      float g10;            /* +0x10 residual */
      float g14;            /* +0x14 residual change per unit step */
      float *ag;            /* +0x18 row (0 until the row is filled) */
      DZ *pdzOther;         /* +0x1C the friction row paired with a contact row, and back (SolveDzgFric) */
      DLE dle;              /* +0x20 */
};
typedef char assert_DZ_size[sizeof(DZ) == 0x28 ? 1 : -1];

/* The solver: constraints, the four lists, the active count and the Crout work arrays. */
struct DZG
{
      int cdzMax;           /* +0x00 */
      int cdz;              /* +0x04 */
      DZ *adz;              /* +0x08 */
      DL dlA;               /* +0x0C active */
      DL dlB;               /* +0x18 at the lower bound */
      DL dlC;               /* +0x24 at the upper bound */
      DL dlD;               /* +0x30 free */
      int cdzA;             /* +0x3C size of dlA */
      float *agMat;         /* +0x40 */
      float *agLu;          /* +0x44 */
      float *agB;           /* +0x48 */
      float *agX;           /* +0x4C */
};
typedef char assert_DZG_size[sizeof(DZG) == 0x50 ? 1 : -1];   /* SolveInequalities' stack frame (match) */

/* One side of a contact row: the change of the side object's linear and angular velocity per unit impulse along the
 * row. Filled by the object's SO slot 67 (VUnk67(DZ *, DZX *, int, int)), read back through slot 68 (VUnk68(FX *,
 * qword, VECTOR4 *)), applied by ApplyDzg. HYPOTHESIS: Sly 1's name for it is FX (so.h's slot 68 takes an FX *). */
struct FX
{
      qword qwDv;
      qword qwDw;
};

/* The contact data of one constraint row (0x60; DZGXP::adzx, parallel to DZG::adz). */
struct DZX
{
      FX afx[2];            /* +0x00 per side of the contact */
      qword qwNormal;       /* +0x40 row direction */
      XP *pxp;              /* +0x50 the contact the row was made from */
      qword *aqwRel;        /* +0x54 per contact of the group: this row's relative response (FillDzgDz) */
      int unk_0x58[2];
};
typedef char assert_DZX_size[sizeof(DZX) == 0x60 ? 1 : -1];   /* InitDzgXp allocates 0x60 per row */

/* The solver over a group of contacts (Sly 1 passed these to SolveDzg as arguments). */
struct DZGXP : public DZG
{
      int cpxp;             /* +0x50 */
      XP **apxp;            /* +0x54 */
      int cpso;             /* +0x58 */
      SO **apso;            /* +0x5C */
      DZX *adzx;            /* +0x60 */
};
typedef char assert_DZGXP_size[sizeof(DZGXP) == 0x64 ? 1 : -1];

#endif
