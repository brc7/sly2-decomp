// PostJtLoadGait: ported from Lilly's personal Sly 2 decomp (src/P2/jt.c), reduced to the declarations it needs.
// Every global is named by its address; the function keeps its readable name and gets the team symbol through __asm__.
// Globals: D_002C3DC0 (D_002C3DC0), D_002C3DC8 (D_002C3DC8), D_002C3DD0 (D_002C3DD0), D_002C3DD8 (D_002C3DD8)
// 0x0020B868 PostJtLoadGait__FP4GAIT  (our name)
// After the level load (JT::VUnk14, for the gaits at JT+0x1320 and +0x1C30): the foot markers of both clips (FillBlAmrsg), each clip's root movement per second
// ((position at the end - position at the start) / length), the blend (EnsureAsegBlendDynamic with the two marker arrays). Then every blended channel is told
// which marker array keeps it in step: XFs with oid 0x10 / 0x12 / 0x14 the second foot's (amrsgc[1]), 0x11 / 0x13 / 0x15 the first's (the default), any other none.
#include "sly2/basic.h"
#include "sly2/lo.h"
#include "sly2/so.h"
#include "sly2/aseg.h"
#include "sly2/stepplayer.h"
struct SWALLOC
{
    int cb;
    int cbAlign;
};
void *PvAllocSwClearImpl(SWALLOC *pswalloc);
struct KEY
{
    virtual void Load(CBinaryInputStream *pbis);
    static void *operator new(uint cb)
    {
        SWALLOC swalloc;
        swalloc.cb = cb;
        SWALLOC *pswalloc = &swalloc;
        pswalloc->cbAlign = 0x10;
        return PvAllocSwClearImpl(pswalloc);
    }
};
struct VECKEY : public KEY
{
    virtual void CacheSeg(XF *pxf);
    virtual void GetValue(float t, float svt, qword *ppos, qword *pv);
};
struct SCALARKEY : public KEY
{
};
struct CHN
{
      int oid : 20;
      int ixf : 10;
      uint fStripped : 1;
      uint fScalar : 1;
      KEY *pkey;
};
struct OVR
{
      int id;
      OID oid;
      KEY *pkey;
};
struct EASND
{
      int sfxid;
      OID oid;
      int sNear;
      int sFar;
      float gPitch;
      float gUnk14;
      float gUnk18;
};
struct EA17
{
      int n0;
      int n4;
      int n8;
      float gC;
};
struct EA20
{
      int n0;
      int n4;
      float g8;
};
struct EA24
{
      RAW8 a[3];
};
struct MRSGC
{
      int ib;
      int cmrsg;
      MRSG *amrsg;
};
void FindAsegChannelTarget(ASEG *paseg, ALO *paloRoot, CHN *pchn, SO **ppso, XF **ppxf);
CHN *PchnFindAseg(ASEG *paseg, ALO *palo, int xfk, int fStripped);
void EnsureAsegBlendDynamic(ALO *palo, int cbBl, int cbl, void *abl, int cmrsgc, int *mpimrsgcib, int *mpimrsgccmrsg, ASEGBL **ppasegbl);
void FillBlAmrsg(int cbBl, int cbl, void *abl, int ibMrsg, int coid, OID *aoid);
void ReblendAsegbl(ASEGBL *pasegbl);
struct VKBLEND : public VECKEY
{
      ASEGBL *pasegbl;
      MRSGC *amrsgc;
      KEY **apkey;
      qword qwRest;
    virtual int NKeyType() { return 5; }
};
struct SKBLEND : public SCALARKEY
{
      ASEGBL *pasegbl;
      MRSGC *amrsgc;
      KEY **apkey;
      float gRest;
};
struct VU_FLOAT
{
    qword qw;
};
static inline qword QwSub(qword qw0, qword qw1)
{
    qword qw;
    __asm__("vsub.xyzw %0, %1, %2" : "=j"(qw) : "j"(qw0), "j"(qw1));
    return qw;
}
static inline qword QwDivVu(qword qw, VU_FLOAT g)
{
    qword qwRet;
    __asm__("vdiv $Q, $vf0w, %1x\n\tvwaitq\n\tvmulq.xyzw %0, %2, $Q" : "=j"(qwRet) : "j"(g.qw), "j"(qw));
    return qwRet;
}
extern OID D_002C3DC0[2];
extern OID D_002C3DC8[2];
extern int D_002C3DD0[2];
extern int D_002C3DD8[2];
void PostJtLoadGait(GAIT *pgait) __asm__("PostJtLoadGait");
void PostJtLoadGait(GAIT *pgait)
{
    if (pgait->abl[0].paseg == 0)
        return;
    FillBlAmrsg(0x40, 2, pgait->abl, 0x20, 2, D_002C3DC0);
    FillBlAmrsg(0x40, 2, pgait->abl, 0x30, 2, D_002C3DC8);
    for (uint ibl = 0; ibl < 2; ibl++)
    {
        BLGAIT *pblgait = &pgait->abl[ibl];
        ASEG *paseg = pblgait->paseg;
        if (paseg != 0)
        {
            CHN *pchn = PchnFindAseg(paseg, (ALO *)pgait->pstepplayer, 1, 1);
            VECTOR4 vStart, vEnd;
            ((VECKEY *)pchn->pkey)->GetValue(0.0f, 1.0f, (qword *)&vStart, 0);
            ((VECKEY *)pchn->pkey)->GetValue(paseg->tMax, 1.0f, (qword *)&vEnd, 0);
            qword qwDiff = QwSub(*(qword *)&vEnd, *(qword *)&vStart);
            VU_FLOAT vuT;
            __asm__("qmtc2 %1, %0" : "=j"( vuT.qw ) : "rV"(  *(int *)&paseg->tMax )) ;
            pblgait->vRoot = QwDivVu(qwDiff, vuT);
        }
    }
    EnsureAsegBlendDynamic((ALO *)pgait->pstepplayer, 0x40, 2, pgait->abl, 2, D_002C3DD8, D_002C3DD0, &pgait->pasegbl);
    for (int ichn = 0; ichn < pgait->pasegbl->cchn; ichn++)
    {
        CHN *pchn = &pgait->pasegbl->achn[ichn];
        VKBLEND *pvkblend;
        SKBLEND *pskblend;
        if (pchn->fScalar)
        {
            pvkblend = 0;
            pskblend = (SKBLEND *)pchn->pkey;
        }
        else
        {
            pvkblend = (VKBLEND *)pchn->pkey;
            pskblend = 0;
        }
        SO *pso;
        XF *pxf;
        FindAsegChannelTarget(pgait->pasegbl, (ALO *)pgait->pstepplayer, pchn, &pso, &pxf);
        switch (pxf->oid)
        {
            case 0x10:
            case 0x12:
            case 0x14:
                if (pvkblend != 0)
                    pvkblend->amrsgc = &pgait->pasegbl->amrsgc[1];
                if (pskblend != 0)
                    pskblend->amrsgc = &pgait->pasegbl->amrsgc[1];
                break;
            case 0x11:
            case 0x13:
            case 0x15:
                break;
            default:
                if (pvkblend != 0)
                    pvkblend->amrsgc = 0;
                if (pskblend != 0)
                    pskblend->amrsgc = 0;
                break;
        }
    }
    ReblendAsegbl(pgait->pasegbl);
}
