#include "mat.h"
struct CM { char pad[0x90]; struct VECTOR pos; };
void SetCmMat4(struct CM *p, struct MATRIX4 *m);
/** @sly2-annot addr=0x14F428
 * Purpose: Set rotation from MATRIX3, keep pos, SetCmMat4
 * Info: proposed name SetCmMat (T1); TU cm
 * Confidence: mid (draft) [T1 0.75] - evidence: semantic review; sources: r2_131F70
 * See: docs/reference/functions/camera.md#cm-0x14d3d8-0x14fac8,
 *   docs/reference/functions/core-library.md#game_linkonce-0x254f00-0x259de4
 * @end-sly2-annot */
void SetCmMat(struct CM *p, struct MATRIX3 *pmat)
{
    struct MATRIX4 m;
    *(struct MATRIX3 *)&m = *pmat;
    *(struct VECTOR *)m.mat[3] = p->pos;
    m.mat[2][3] = 0.0f;
    m.mat[1][3] = 0.0f;
    m.mat[0][3] = 0.0f;
    SetCmMat4(p, &m);
}
