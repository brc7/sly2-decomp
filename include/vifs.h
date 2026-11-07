#ifndef VIFS_H
#define VIFS_H
#include "dmas.h"

enum UPK
{
    UPK_Nil = -1, UPK_S32 = 0, UPK_S16 = 1, UPK_S8 = 2,
    UPK_V2_32 = 4, UPK_V2_16 = 5, UPK_V2_8 = 6,
    UPK_V3_32 = 8, UPK_V3_16 = 9, UPK_V3_8 = 10,
    UPK_V4_32 = 12, UPK_V4_16 = 13, UPK_V4_8 = 14, UPK_V4_5 = 15, UPK_Max = 16
};

class GIFS;
class CBinaryInputStream;

/* VIF packet builder on top of DMAS */
class VIFS : public DMAS
{
public:
    /* 0x1c */ uint m_unMask;
    /* 0x20 */ int m_fCheckCnt;

    VIFS();
    void Align(int iun);
    void AddVifBaseOffset(int iqwBase, int diqwOffset);
    void AddVifMscal(void *pv);
    void AddVifFlush();
    void AddVifFlusha();
    void AddVifStcycl(int wl, int cl);
    void AddVifStrow(QW *pqw);
    void AddVifStmask(uint unMask);
    int WriteUnpackSetup(UPK upk, int c, int iqw);
    void UnpackHelper(UPK upk, int c, int iqw, int *pcb, uint **ppun);
    void AddVifUnpack(UPK upk, int c, void *pvSrc, int iqw);
    void AddVifUnpack(UPK upk, int c, CBinaryInputStream *pbis, int iqw);
    void AddVifUnpackRefs(UPK upk, int c, void *pvSrc, int iqw, void ***pppv);
    void AddVifDirect(int cqw, QW *aqw, int fInt);
    void AddVifDirectRefs(int cqw, QW *aqw, int fInt);
    void AddVifGifs(GIFS *pgifs);
};

static inline void AddDmaInt(DMAS *pdmas, int n)
{
    uchar *pb = pdmas->m_pb;
    pdmas->m_pb = pb + 4;
    *(int *)pb = n;
}
#endif
