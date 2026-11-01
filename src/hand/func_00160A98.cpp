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
extern "C" void func_00160A98()
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
