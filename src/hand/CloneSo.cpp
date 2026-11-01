// CloneSo: ported from Lilly's personal Sly 2 decomp (src/P2/so.c), reduced to the declarations it needs.
// Every global is named by its address; the function keeps its readable name and gets the team symbol through __asm__.
// Globals: D_002C44F0 (g_xfWorld)
// 0x001F6068 Clone__2SOP2LO  (MATCHES Sly 2, T1075)
// Virtual 40. LO's clone (the byte copy) took the base's child list, tree record and removed-children mark along: the clone's own
// are put back. Then everything the SO owns is copied for the clone and every pointer into the base's XFs is re-pointed at the
// clone's (CloneXfRef): the XF block (each XF's parent, the record its slot holds in roles 4 / 13 / 15 / 16, its actuators - cloned
// in their order of priority by PactNewClone + InsertNodeBySlot), the three XFs of the object (position, rotation, root), the
// model (CloneGlobset), the collision parts (geometry, BSP and the per-geometry arrays) and the physics record with its region
// entry. A clone in the world joins the contact list, and it gets a bounds record of its own (EnsureSoBounds).
typedef unsigned char    uint8_t;
typedef unsigned short   uint16_t;
typedef unsigned int     uint32_t;
typedef unsigned long    uint64_t;
typedef uint8_t     uchar;
typedef uint16_t    ushort;
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
struct CBinaryInputStream;
struct LO;
struct SO;
struct SAA;
struct GLOB;
struct SUBGLOB;
struct VECTOR
{
    float x, y, z;
};
struct VECTOR4
{
    float x, y, z, w;
} __attribute__((aligned(16)));
struct MATRIX4
{
	float mat[4][4];
} __attribute__((aligned(16)));
struct MATRIX3
{
	float mat[3][4];
} __attribute__((aligned(16)));
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
struct GEOM
{
      int cpos;
      VECTOR *apos;
      int csurf;
      SURF *asurf;
      int cedge;
      EDGE *aedge;
};
void CloneGeom(GEOM *pgeomSrc, MATRIX4 *pdmat, GEOM *pgeomDst);
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
void ClearDle(DLE *pdle);
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
struct XF;
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
      virtual void VUnk37(LO *plo);
      virtual void VUnk38();
      virtual void CloneHierarchy(LO *ploBase);
      virtual void Clone(LO *ploBase);
      virtual void VUnk41(float sBusy);
      virtual void *VUnk42(int id);
      virtual void VUnk43(int id);
      virtual void VUnk44(int id);
      virtual void SetVisMask(uint grfvis);
      virtual void VUnk46(int grf);
      virtual void VUnk47(int grf);
};
inline int FIsLoInWorld(LO *plo)
{
    return plo->fInWorld != 0;
}
struct BSP
{
    BSP *pbspNeg;
    BSP *pbspPos;
    SURF *psurf;
};
struct BSPC
{
      int cbsp;
      int cbspFull;
      int nUnk08;
      BSP *absp;
};
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
void InsertNodeBySlot(SO *pso, XF *pxf, ACT *pact);
ACT *PactNewClone(ACT *pactBase, SO *pso, XF *pxf);
struct WRBG;
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
struct XF
{
      MATRIX4 mat;
      MATRIX4 matWorld;
      float gUnk80;
      float gUnk84;
      float *pgUnk88;
      float *pgUnk8c;
      ACT *pactFirst;
      int xfk;
      short nRole;
      short nPhase;
      OID oid;
      XF *pxfParent;
      int nStampWorld;
      XF *pxfRef;
      short nUnkAc;
      short nUnkAe;
      VECTOR4 vUnkB0;
      qword qwRest;
      qword qwUnkD0;
      qword qwRestUnkE0;
      qword qwUnkF0;
};
struct CPDEFI
{
      int grfcpd;
      undefined4 field1_0x4;
      undefined4 field2_0x8;
      undefined4 field3_0xc;
      VECTOR4 posBase;
      VECTOR4 vBase;
      float radHome;
      float sAdjust;
      float tMoveLast;
      int nTurnAxis;
      VECTOR4 posAdjust;
};
struct CONSTR
{
      qword vecNormal;
      int nKind;
};
struct XA;
struct XP;
struct SURFX;
struct LVO
{
    VECTOR *mpiposvLocal;
    VECTOR *mpiposdvLocal;
    SURFX *mpisurfsurfx;
};
struct SOPART
{
      XF *pxf;
      SO *pso;
      float gUnk08;
      OID oid;
      LVO *plvo;
      short dhp;
      ulong fUnk48 : 1;
               ulong fUnk49 : 1;
               ulong fHidden : 1;
               ulong fUnk51 : 1;
               ulong unk_0x16 : 12;
      uint grfzpk;
      int nUnk1c;
      MATRIX4 matWorld;
      DLE dle;
      BSPC bspc;
      GEOM geomLocal;
      GEOM geomWorld;
      int nStampGeom;
      int nStampWorld;
};
struct XPLIST
{
      XP *pxpFirst;
      XPLIST *pxplistNext;
      XP **ppxpHead;
      SO *psoOther;
};
struct SOTREE
{
      qword qwMin;
      qword qwMax;
      qword qwCenter;
      float sRadius;
      DL dlpart;
      DLE dleAwake;
      DLE dleAll;
      DLE dleQueue1;
      DLE dleQueue2;
      SO *psoRoot;
      XPLIST *pxplistFirst;
      XP *pxpFirst;
      XP *pxpZap;
      ulong fQueued1 : 1;
               ulong fQueued2 : 1;
               ulong fNoUpdate : 1;
               ulong fTouched : 1;
               ulong fDone : 1;
               ulong unk_0x70 : 59;
};
struct REGENT
{
      SO *pso;
      undefined4 unk_0x04[3];
      struct CID17 *pcid17;
      undefined4 unk_0x14[3];
      undefined4 unk_0x20[20];
};
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
    virtual void Clone(LO *ploBase) __asm__("CloneSo");
      virtual void VUnk48();
      virtual void GetCpdefi(CPDEFI *pcpdefi);
      virtual XF *PxfFromIxf(int ixf);
      virtual void CloneXfRef(LO *ploBase, XF **ppxf);
      virtual SOTREE *VUnk52();
      virtual void VUnk53(SO *pso);
      virtual void AdjustNewXp(XP *pxp, int ixpd);
      virtual void VUnk55();
      virtual void VUnk56(XP *pxp, int ixpd);
      virtual void VUnk57();
      virtual void VUnk58();
      virtual void VUnk59(SOPART *ppart, int n2, int n3, void *pvGeom, XP **ppxpFirst);
      virtual int FIgnoreIntersection(SO *psoOther);
      virtual int VUnk61();
      virtual int CpartExtra();
};
void *PvAllocSwCopyImpl(struct SWALLOC *pswalloc, void *pvBase);
struct SWALLOC
{
    int cb;
    int cbAlign;
};
void *PvAllocStackImpl(SWALLOC *pswalloc);
struct STACKSCOPE
{
    STACKSCOPE();
    ~STACKSCOPE();
};
void EnsureSoBounds(SO *pso);
struct IKLIMB
{
      XF *pxfGoal;
      XF *pxfS;
      XF *pxfE;
      XF *pxfEnd;
      undefined1 unk_0x10[0xA0];
      qword qwEndLocal;
      float gLenUpper;
      float gLenLower;
      undefined4 unk_0xc8[6];
};
struct XFLINK
{
      XF *pxfFrom;
      XF *pxfTo;
      undefined4 unk_0x08[2];
};
struct CHAIN;
struct CHAINLINK
{
      undefined1 unk_0x00[0x60];
      CHAIN *pchain;
      XF *pxf;
      undefined4 unk_0x68[2];
};
struct CHAIN
{
      int clink;
      CHAINLINK *alink;
      undefined4 unk_0x08[18];
};
extern XF D_002C44F0;
void AddSoParts(SO *pso);
void CloneGlobset(GLOBSET *pglobset, SO *pso, LO *ploBase);
void CloneBspc(BSPC *pbspcBase, GEOM *pgeomBase, BSPC *pbspc, GEOM *pgeom);
inline void *PvAllocSwCopy(int cb, void *pvBase)
{
    SWALLOC swalloc;
    swalloc.cb = cb;
    SWALLOC *pswalloc = &swalloc;
    pswalloc->cbAlign = 0x10;
    return PvAllocSwCopyImpl(pswalloc, pvBase);
}
inline void *PvAllocStack(int cb)
{
    SWALLOC swalloc;
    swalloc.cb = cb;
    SWALLOC *pswalloc = &swalloc;
    pswalloc->cbAlign = 0x10;
    return PvAllocStackImpl(pswalloc);
}
void SO::Clone(LO *ploBase)
{
    SO *psoBase = (SO *)ploBase;
    SOTREE *ptreeSave = ptree;
    LO *ploRemovedFirstSave = ploRemovedFirst;
    DL dlChildSave = dlChild;
    int ixf = 0;
    LO::Clone(ploBase);
    ptree = ptreeSave;
    dlChild = dlChildSave;
    ploRemovedFirst = ploRemovedFirstSave;
    psobounds = 0;
    axf = (XF *)PvAllocSwCopy(cxf * sizeof(XF), axf);
    for (ixf = 0; ixf < cxf; ixf++)
    {
        XF *pxf = &axf[ixf];
        CloneXfRef(ploBase, &pxf->pxfParent);
        switch (pxf->nRole)
        {
        case 4:
            CloneXfRef(ploBase, &pxf->pxfRef);
            break;
        case 13:
        {
            IKLIMB *plimb = (IKLIMB *)PvAllocSwCopy(sizeof(IKLIMB), pxf->pxfRef);
            CloneXfRef(ploBase, &plimb->pxfGoal);
            CloneXfRef(ploBase, &plimb->pxfS);
            CloneXfRef(ploBase, &plimb->pxfE);
            CloneXfRef(ploBase, &plimb->pxfEnd);
            plimb->pxfS->pxfRef = (XF *)plimb;
            plimb->pxfE->pxfRef = (XF *)plimb;
            break;
        }
        case 15:
        {
            XFLINK *pxflink = (XFLINK *)PvAllocSwCopy(sizeof(XFLINK), pxf->pxfRef);
            pxf->pxfRef = (XF *)pxflink;
            if (pxflink->pxfFrom != &D_002C44F0)
            {
                CloneXfRef(ploBase, &pxflink->pxfFrom);
            }
            CloneXfRef(ploBase, &pxflink->pxfTo);
            break;
        }
        case 16:
        {
            CHAINLINK *plink = (CHAINLINK *)pxf->pxfRef;
            CHAIN *pchain = plink->pchain;
            if (plink == pchain->alink)
            {
                pchain = (CHAIN *)PvAllocSwCopy(sizeof(CHAIN), pchain);
                pchain->alink = (CHAINLINK *)PvAllocSwCopy(pchain->clink * sizeof(CHAINLINK), pchain->alink);
                for (int ilink = 0; ilink < pchain->clink; ilink++)
                {
                    CHAINLINK *plinkNew = &pchain->alink[ilink];
                    plinkNew->pchain = pchain;
                    CloneXfRef(ploBase, &plinkNew->pxf);
                    plinkNew->pxf->pxfRef = (XF *)plinkNew;
                }
            }
            break;
        }
        }
        if (pxf->pactFirst)
        {
            int cact = 0;
            ACT *pact = pxf->pactFirst;
            do
            {
                pact = pact->pactNext;
                cact++;
            } while (pact);
            STACKSCOPE scope;
            ACT **apact = (ACT **)PvAllocStack(cact * sizeof(ACT *));
            int iact = 0;
            for (pact = pxf->pactFirst; pact; pact = pact->pactNext)
            {
                apact[iact++] = pact;
            }
            pxf->pactFirst = 0;
            for (iact = cact - 1; iact >= 0; iact--)
            {
                ACT *pactNew = PactNewClone(apact[iact], this, pxf);
                InsertNodeBySlot(this, pxf, pactNew);
            }
        }
    }
    CloneXfRef(ploBase, &pxfUnk58);
    CloneXfRef(ploBase, &pxfUnk5c);
    CloneXfRef(ploBase, &pxfRoot);
    CloneGlobset(&globset, this, ploBase);
    int cpartExtra = CpartExtra();
    apart = (SOPART *)PvAllocSwCopy((cpart + cpartExtra) * sizeof(SOPART), apart);
    for (int ipart = 0; ipart < cpart; ipart++)
    {
        SOPART *ppart = &apart[ipart];
        CloneXfRef(ploBase, &ppart->pxf);
        ppart->pso = this;
        ClearDle(&ppart->dle);
        GEOM geom;
        CloneGeom(&ppart->geomWorld, 0, &geom);
        ppart->geomWorld = geom;
        SOPART *ppartBase = &psoBase->apart[ipart];
        CloneBspc(&ppartBase->bspc, &ppartBase->geomWorld, &ppart->bspc, &ppart->geomWorld);
        if (ppart->plvo)
        {
            int cb = sizeof(LVO);
            ppart->plvo = (LVO *)PvAllocSwCopy(cb, ppart->plvo);
            ppart->plvo->mpiposvLocal = (VECTOR *)PvAllocSwCopy(ppart->geomLocal.cpos * 0x10, ppart->plvo->mpiposvLocal);
            ppart->plvo->mpiposdvLocal = (VECTOR *)PvAllocSwCopy(ppart->geomLocal.cpos * 0x10, ppart->plvo->mpiposdvLocal);
            ppart->plvo->mpisurfsurfx = (SURFX *)PvAllocSwCopy(ppart->geomLocal.csurf * 0xC, ppart->plvo->mpisurfsurfx);
        }
    }
    if (psophys)
    {
        psophys = (SOPHYS *)PvAllocSwCopy(sizeof(SOPHYS), psophys);
        if (psophys->pregent)
        {
            psophys->pregent = (REGENT *)PvAllocSwCopy(sizeof(REGENT), psophys->pregent);
            psophys->pregent->pso = this;
        }
    }
    if (FIsLoInWorld(this))
    {
        AddSoParts(this);
    }
    EnsureSoBounds(this);
}
