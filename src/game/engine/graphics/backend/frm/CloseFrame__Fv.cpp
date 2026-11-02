#include "vifs.h"
#include "gifs.h"
struct DL { void *pvFirst; void *pvLast; int ibDle; };
struct FR {
    int cqwGifs;     /* 0x00 */
    QW *aqwGifs;     /* 0x04 */
    int pad8[2];
    int cqwVifs;     /* 0x10 */
    QW *aqwVifs;     /* 0x14 */
    int pad18[2];
    int cqw20;       /* 0x20 */
    QW *aqw24;       /* 0x24 */
    int pad28[3];
    DL dl34;         /* 0x34 */
    DL dl40;         /* 0x40 */
};
extern FR *D_002DEA4C;
extern VIFS D_002DEA28;
extern GIFS D_002DEA00;
extern DL D_002DEAF0;
extern DL D_002DEB00;
extern int D_002DEB18;
void FinalizeFrameVifs(VIFS *pvifs, int *pcqw, QW **paqw);
void FinalizeFrameGifs(GIFS *pgifs, int *pcqw, QW **paqw);
extern "C" void ClearDl__FP2DL(DL *pdl);
extern "C" void func_00234A40(int sid);
/** @sly2-annot addr=0x160A98
 * Purpose: Finalizes the open frame: closes its VIF and GIF chains into g_pfrmOpen, moves the two
 *   deferred-release DLs into the frame, clears g_pfrmOpen and signals the render thread
 * Info: proposed name CloseFrame (T1); TU frm
 * Notes: names_v2 C proposed check_anticrack_antigrab__Fv (0.650)
 * Confidence: high [T1 0.85] - evidence: static naming pass; sources: r3_1539A8+verify_side
 * See: docs/engine/overview.md#2-the-frame-loop-exact-order, docs/engine/rendering.md#1-frame-structure,
 *   docs/engine/rendering.md#13-key-functions
 * @end-sly2-annot */
extern "C" void CloseFrame__Fv()
{
    FinalizeFrameVifs(&D_002DEA28, &D_002DEA4C->cqwVifs, &D_002DEA4C->aqwVifs);
    FinalizeFrameGifs(&D_002DEA00, &D_002DEA4C->cqwGifs, &D_002DEA4C->aqwGifs);
    D_002DEA4C->cqw20 = D_002DEA4C->aqwVifs + D_002DEA4C->cqwVifs - D_002DEA4C->aqw24;
    D_002DEA4C->dl34 = D_002DEAF0;
    ClearDl__FP2DL(&D_002DEAF0);
    D_002DEA4C->dl40 = D_002DEB00;
    ClearDl__FP2DL(&D_002DEB00);
    D_002DEA4C = 0;
    func_00234A40(D_002DEB18);
}
