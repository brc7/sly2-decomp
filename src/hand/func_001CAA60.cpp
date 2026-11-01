// func_001CAA60: ported from Lilly's personal Sly 2 decomp (src/P2/proxy.c), reduced to the declarations it needs.
// Every global is named by its address; the function keeps its readable name and gets the team symbol through __asm__.
// Globals: D_002A2DD0 (g_qwZero)
// 0x001CAA60 ReleasePspare__FP5PROXYP10PROXYSPARE  (MATCHES Sly 2, T0861)
// Takes the copy away from the proxy that holds it and puts it back in the pool: out of the proxy's list, the
// PROXYINIT splice frames undone (CFrame::RemoveParent), then for every object VUnk26(proxy), splice event 0xF
// to both, Remove, moved back by the inverse of the matrix it was placed with, into the world without a parent,
// and an SO is put back where the source had it (with zero velocities) when it has the XFs for that.
#include "sly2/basic.h"
#include "sly2/lo.h"
#include "sly2/xfm.h"
#include "sly2/so.h"
typedef unsigned  size_t;
extern "C" {
void* memset(void* ptr, int value, size_t num);
}
inline void SetMatrix4Identity(MATRIX4 *pmat)
{
    memset(pmat, 0, sizeof(MATRIX4));
    pmat->mat[3][3] = 1.0f;
    pmat->mat[2][2] = 1.0f;
    pmat->mat[1][1] = 1.0f;
    pmat->mat[0][0] = 1.0f;
}
MATRIX4 MatInvertMat4(MATRIX4 *pmat);
extern qword D_002A2DD0;
int FIsBasicDerivedFrom(BASIC *pbasic, CID cid);
void SetLoParent(LO *plo, SO *psoParent, int grf);
struct SUBGLOBX
{
      VECTOR posCenter;
      float sRadius;
      float gUnk10;
      uint grfUnk14;
};
struct PROXYSPARE
{
      struct PROXY *pproxy;
      PROXYSPARE *pspareNext;
      int fLocked;
      undefined4 unk_0xc;
      MATRIX4 mat;
};
struct PROXYREST
{
      qword pos;
      MATRIX3 mat;
};
struct PROXYSRC
{
      int cplo;
      LO **aplo;
      int ccopy;
      int icopyNext;
      int id;
      int nUnk14;
      OID *aoid;
      char achzName[0x40];
      int nUnk5c;
      int fUnk60;
      PROXYREST *aposRest;
      PROXYSPARE *aspare;
};
struct PROXYINIT
{
      int kind;
      CID cid;
      CFrame *pframe;
      int cb;
      void *pvOptions;
      LO *plo;
};
int IcopyFromPspare(PROXY *pproxy, PROXYSPARE *pspare);
void ReleasePspare(PROXY *pproxy, PROXYSPARE *pspare) __asm__("func_001CAA60");
void ReleasePspare(PROXY *pproxy, PROXYSPARE *pspare)
{
    int iplo;
    PROXYSPARE *psparecur = pproxy->pspareFirst;
    if (psparecur)
    {
        if (psparecur == pspare)
        {
            pproxy->pspareFirst = pspare->pspareNext;
            pspare->pspareNext = 0;
        }
        else
        {
            while (true)
            {
                PROXYSPARE **ppspare = &psparecur->pspareNext;
                psparecur = *ppspare;
                if (!psparecur)
                {
                    break;
                }
                if (psparecur == pspare)
                {
                    *ppspare = pspare->pspareNext;
                    pspare->pspareNext = 0;
                    break;
                }
            }
        }
    }
    pspare->pproxy = 0;
    LO *ploProxy = pproxy;
    MATRIX4 matInverse;
    matInverse = MatInvertMat4(&pspare->mat);
    SetMatrix4Identity(&pspare->mat);
    int icopy = IcopyFromPspare(pproxy, pspare);
    for (iplo = 0; iplo < pproxy->psrc->cplo; iplo++)
    {
        LO *plo = pproxy->psrc->aplo[icopy * pproxy->psrc->cplo + iplo];
        for (int iinit = 0; iinit < pproxy->cinit; iinit++)
        {
            PROXYINIT *pinit = &pproxy->ainit[iinit];
            if (pinit->kind == 5)
            {
                continue;
            }
            CFrame *pframe = pinit->pframe;
            if (pframe && !pproxy->nUnkE4)
            {
                LO *ploInit = pinit->plo;
                if (pframe->m_cpframeParent)
                {
                    ploInit->pframe = pframe->m_apframeParent[0];
                    pframe->RemoveParent(ploInit->pframe);
                }
                else
                {
                    ploInit->pframe = 0;
                }
            }
            pinit->plo = 0;
        }
        plo->VUnk26(pproxy);
        void *apvArgs[1];
        apvArgs[0] = &plo;
        pproxy->HandleSpliceEvent(0xF, 0, 1, apvArgs, 0);
        apvArgs[0] = &ploProxy;
        plo->HandleSpliceEvent(0xF, 0, 1, apvArgs, 0);
        plo->Remove();
        plo->ApplyProxy(&matInverse);
        SetLoParent(plo, 0, 0);
        if (FIsBasicDerivedFrom(plo, (CID)0x30))
        {
            PROXYREST *pposrest = &pproxy->psrc->aposRest[iplo];
            SO *pso = (SO *)plo;
            if (pso->pxfUnk58)
            {
                pso->TranslateToPos(pposrest->pos);
                pso->SetVelocity(D_002A2DD0);
            }
            if (pso->pxfUnk5c)
            {
                pso->RotateToMat((MATRIX4 *)&pposrest->mat);
                pso->SetAngularVelocity(D_002A2DD0);
            }
        }
    }
}
