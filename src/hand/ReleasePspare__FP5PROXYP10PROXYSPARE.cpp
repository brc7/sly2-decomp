// ReleasePspare: ported from Lilly's personal Sly 2 decomp (src/P2/proxy.c), reduced to the declarations it needs.
// Every global is named by its address; the function keeps its readable name and gets the team symbol through __asm__.
// Globals: D_002A2DD0 (g_qwZero)
// 0x001CAA60 ReleasePspare__FP5PROXYP10PROXYSPARE  (MATCHES Sly 2, T0861)
// Takes the copy away from the proxy that holds it and puts it back in the pool: out of the proxy's list, the
// PROXYINIT splice frames undone (CFrame::RemoveParent), then for every object VUnk26(proxy), splice event 0xF
// to both, Remove, moved back by the inverse of the matrix it was placed with, into the world without a parent,
// and an SO is put back where the source had it (with zero velocities) when it has the XFs for that.
typedef unsigned  size_t;
typedef unsigned char    uint8_t;
typedef unsigned int     uint32_t;
typedef unsigned long    uint64_t;
typedef uint8_t     uchar;
typedef uint32_t    uint;
typedef uint64_t    ulong;
typedef uchar   undefined1;
typedef uint    undefined4;
typedef unsigned long long qword __attribute__((mode(TI)));
enum CID
{
};
enum MSGID
{
};
struct GLOB;
struct SO;
struct VECTOR
{
    float x, y, z;
};
extern "C" {
void* memset(void* ptr, int value, size_t num);
}
struct MATRIX4
{
	float mat[4][4];
} __attribute__((aligned(16)));
struct MATRIX3
{
	float mat[3][4];
} __attribute__((aligned(16)));
inline void SetMatrix4Identity(MATRIX4 *pmat)
{
    memset(pmat, 0, sizeof(MATRIX4));
    pmat->mat[3][3] = 1.0f;
    pmat->mat[2][2] = 1.0f;
    pmat->mat[1][1] = 1.0f;
    pmat->mat[0][0] = 1.0f;
}
MATRIX4 MatInvertMat4(MATRIX4 *pmat);
enum OID
{
};
struct SL
{
    void *head;
    void *tail;
    int ibSle;
};
struct SLE
{
    void *next;
};
struct DL
{
    void *head;
    void *tail;
    int ibDle;
};
struct DLE
{
    void *next;
    void *prev;
};
struct XF;
struct LO;
extern qword D_002A2DD0;
template <class T>
class CRefPtr
{
public:
    T *m_p;
    CRefPtr() {}
    CRefPtr(T *p) { Attach(p); }
    CRefPtr(const CRefPtr &ref)
    {
        m_p = ref.m_p;
        if (m_p)
        {
            m_p->m_cref++;
        }
    }
    ~CRefPtr()
    {
        if (m_p)
        {
            m_p->Release();
        }
    }
    void operator=(T *p);
    void operator=(const CRefPtr &ref) { *this = ref.m_p; }
    operator T *() { return m_p; }
    T *operator->() { return m_p; }
};
class CSidebag;
class CFrame;
struct CLASSINFO
{
      CLASSINFO *pclassinfoSuper;
      CID cid;
      CID cidMax;
      int cb;
};
struct BASIC
{
      CRefPtr<CFrame> pframe;
      CRefPtr<CSidebag> psidebag;
      struct REFNODE *prefnodeFirst;
      virtual void VUnk00();
      virtual void VUnk01();
      virtual void HandleSpliceEvent(uint symidEvent, int fPost, int cargs, void **ppvargs, int nUnk);
      virtual void SendMessage(MSGID msgid, void *pv);
      virtual void HandleMessage(MSGID msgid, void *pv);
      virtual CLASSINFO *Pclassinfo();
};
int FIsBasicDerivedFrom(BASIC *pbasic, CID cid);
template <class T>
class CGcObj
{
public:
      int m_cref;
      uint m_grfGc;
    ~CGcObj() { Unlink(); }
    void Release()
    {
        if (--m_cref == 0)
        {
            delete (T *)this;
        }
    }
};
class CVarb;
class CFrame : public CGcObj<CFrame>
{
public:
      int m_cpframeParent;
      CRefPtr<CFrame> m_apframeParent[4];
      CRefPtr<CVarb> m_pvarb;
      int m_fVisited;
    void RemoveParent(CFrame *pframeParent);
};
class CBinaryInputStream;
struct LOROOT
{
      LO *plo;
      DLE dle;
      DLE dleBusy;
};
struct LORUN
{
      void *pv;
      LORUN *plorunNext;
};
struct PXR
{
      int idProxy;
      PXR *ppxrParent;
      int oidProxy;
      SLE slePxrOid;
      LO *plo;
      SLE sleSibling;
      SL slChild;
      undefined4 unk_0x24;
};
struct LO : public BASIC
{
      SO *psoParent;
      LOROOT *ploroot;
      OID oid;
      SLE sleOid;
      SLE sleCid;
      DLE dleChild;
      char cpaloFindSwObjects;
      uchar grfvis;
      long nUnk48 : 4;
               long busyk : 4;
               ulong fRemoved : 1;
               ulong fUnk57 : 1;
               ulong fUnk58 : 1;
               ulong fUnk59 : 1;
               ulong fInWorld : 1;
      float gUnk30;
      LORUN *plorunFirst;
      float sBusy;
      PXR *ppxr;
      int grfUnk40;
      undefined4 unk_0x44[3];
      virtual void Init();
      virtual void Add();
      virtual void Remove();
      virtual void SetParent(SO *psoParent, XF *pxfParent, int grf);
      virtual void AddHierarchy();
      virtual void RemoveHierarchy();
      virtual void UpdateLoroot(LO *plo, SO *psoOld);
      virtual void LoadFromBrx(CBinaryInputStream *pbis);
      virtual void VUnk14();
      virtual void VUnk15();
      virtual void VUnk16();
      virtual void UpdateXfWorldHierarchy();
      virtual void RenderSelf();
      virtual void VUnk19();
      virtual void VUnk20(int fOn);
      virtual void Freeze();
      virtual void Unfreeze();
      virtual void VUnk23();
      virtual void SetIdProxy(int idProxy, int fChildren);
      virtual void VUnk25(LO *plo);
      virtual void VUnk26(LO *plo);
      virtual int CploChildren(LO ***paplo);
      virtual void TranslateToPos(qword qwPos);
      virtual void RotateToMat(MATRIX4 *pmatWorld);
      virtual MATRIX4 *PmatWorld();
      virtual XF *PxfLocal(MATRIX4 *pmatLocal);
      virtual qword GetVelocity();
      virtual qword GetAngularVelocity();
      virtual void SetVelocity(qword qwVel);
      virtual void SetAngularVelocity(qword qwW);
      virtual void ApplyProxy(MATRIX4 *pmatProxy);
};
void SetLoParent(LO *plo, SO *psoParent, int grf);
struct SOPART;
struct WRBG;
struct SAA;
struct SUBGLOB
{
      char bUnk00;
      char cbnd;
      int bUnk02 : 8;
      char type;
      float gUnk04;
      void *pvLit;
      int iqwBone;
      int cqw;
};
struct SUBGLOBX
{
      VECTOR posCenter;
      float sRadius;
      float gUnk10;
      uint grfUnk14;
};
struct POSE
{
      VECTOR pos;
      float gUnk0c;
      float gUnk10;
      long nKind : 4;
    ulong ucUnk : 8;
    ulong ibUnk : 6;
    ulong fHide : 1;
    long iglob : 13;
      void *pvUnk18;
      undefined4 unk_0x1c;
};
struct POSESET
{
    POSE apose[4];
};
struct GLOBSET
{
      SO *pso;
      float tUnk04;
      undefined4 unk_0x08[2];
      VECTOR posCenter;
      undefined4 unk_0x1c;
      float sRadius;
      int cpsaa;
      SAA **apsaa;
      int cnUnk2c;
      int *anUnk30;
      int nUnk34;
      int cglob;
      SUBGLOB *asubglob;
      GLOB *aglob;
      SUBGLOBX *asubglobx;
      POSESET *aposeset;
      int nUnk4c;
      union
    {
        float gUnk50;
        struct
        {
            ulong unk_0x50 : 32;
            ulong fBoundsUnk32 : 1;
            ulong fBoundsUnk33 : 1;
            ulong fUnk34 : 1;
            ulong unk_0x54 : 29;
        };
    };
      WRBG *pwrbgFirst;
      void *pbatchrecsFirst;
};
struct XFM : public LO
{
      XF *pxfParent;
      int nStampWorld;
      undefined4 unk_0x58[2];
      MATRIX4 mat;
      MATRIX4 matWorld;
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
struct PROXY : public XFM
{
      PROXYSRC *psrc;
      int nUnkE4;
      PROXYSPARE *pspareFirst;
      int cinit;
      PROXYINIT *ainit;
      undefined4 unk_0xf4[3];
};
int IcopyFromPspare(PROXY *pproxy, PROXYSPARE *pspare);
struct CONSTR
{
      qword vecNormal;
      int nKind;
};
struct XA;
struct SOTREE;
struct SOPHYS
{
      MATRIX3 momintLocal;
      MATRIX3 momintInvLocal;
      qword qwVelocity;
      qword qwAngVelocity;
      VECTOR dvGravity;
      undefined4 unk_0x8c;
      CONSTR constrTorque;
      CONSTR constrForce;
      float m;
      undefined4 unk_0xd4[2];
      struct REGENT *pregent;
};
struct SO : public LO
{
      int cxf;
      XF *axf;
      XF *pxfUnk58;
      XF *pxfUnk5c;
      XF *pxfRoot;
      DL dlChild;
      LO *ploRemovedFirst;
      undefined4 unk_0x74[3];
      GLOBSET globset;
      int cpart;
      SOPART *apart;
      SOPHYS *psophys;
      SOTREE *ptree;
      qword qwDvContact;
      qword qwDwContact;
      uchar agrfZpk[26];
      undefined1 unk_0x12a[2];
      float uAlpha;
      float duAlpha;
      struct SOBOUNDS *psobounds;
      XA *pxa;
      float gUnk13c;
      int nStampUnk140;
      undefined4 unk_0x144;
      char bUnk148;
      char bUnk149;
      ulong unk_0x14a : 4;
                long nUnk14a : 4;
                ulong nZapMode : 2;
                ulong fIgnoreFixed : 1;
                ulong unk_0x14c : 2;
                ulong fNoJtPlace : 1;
                ulong unk_0x14d : 3;
                ulong fHurtRespawns : 1;
                ulong fTreeRoot : 1;
                ulong fUnk35 : 1;
                ulong fHasIkLimbs : 1;
};
void ReleasePspare(PROXY *pproxy, PROXYSPARE *pspare) __asm__("ReleasePspare__FP5PROXYP10PROXYSPARE");
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
