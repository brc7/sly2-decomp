// brx.cpp -- loading objects from a level file (.brx): the object list reader (LoadSwObjectsFromBrx), object creation
// and cloning (CreateLo, CloneLoWithParent), the option readers that apply a level file's option values through g_aeopid
// (LoadOptionsFromBrx, LoadOptionFromBrx, RefFromBrx), the post-load object-reference fixups, and the two load
// collectors (an array of the loaded objects; a proxy collector that parents and tags them).
// TU 0x13E368..0x13FF70 (16 functions, address order).
// Entry points: LoadSwObjectsFromBrx (from the level loader), CreateLo / CloneLoWithParent (object creation),
// LoadOptionsFromBrx (from every class's LoadFromBrx).
// SLY2_ALIAS: __static_initialization_and_destruction_0=StaticInit_brx
// SLY2_ALIAS: _GLOBAL_$I$*=GlobalConstructors_brx
#include "sly2/basic.h"
#include "sly2/cref.h"
#include "sly2/eopid.h"     /* EOPID, SETTER, PMF */
#include "sly2/lo.h"
#include "sly2/so.h"
#include "sly2/sw.h"
#include "sly2/slotheap.h"
#include "sly2/stackscope.h"
#include "sly2/util.h"
#include "sly2/vtables.h"
#include "bis.h"

// 0x13FE18 StaticInit_brx and GlobalConstructors_brx: g++-generated for the six engine slot pools.
SLY2_INSTANTIATE_SLOT_POOLS()

/* Declarations from LoadOptionFromBrx's per-function file (Lilly port): a private CBinaryInputStream view (S8Read and
   Skip added for LoadSwObjectsFromBrx). EOPID is sly2/eopid.h's, CRef sly2/cref.h's. Callees keep the names that file
   gave them. */
MATRIX3 LoadRotateMatrix(qword vec) SLY2_SRET_LABEL("BuildRotationMatrixFromVector", "BuildRotationMatrixFromVectorValue");
extern "C" LORUN *AddLoDeferredCallback(LO *plo);
void *PvAllocSwClearImpl(SWALLOC *pswalloc) __asm__("AllocClearedFromWorldHeap");
void *PvAllocStackClearImpl(SWALLOC *pswalloc) __asm__("AllocClearedFromStack");
CRef BuildSpliceListFromValues(int cargs, OTYP *aotyp, void **apv) __asm__("BuildSpliceListFromValues");
void PostLoadOidField(LO *plo, void *pv);
void PostLoadOidSetter(LO *plo, void *pv);
CRef RefFromBrx(OTYP otyp, CBinaryInputStream *stream) SLY2_SRET_LABEL("ReadOptionArgFromBrx", "ReadOptionArgFromBrxValue");   /* defined below (C symbol) */
void *AllocSwPostLoadRecord(SW *world) __asm__("AllocSwPostLoadRecord");
extern "C" void ColorCorrectRgba(RGBA *prgba);
// Allocate cb zeroed bytes with alignment cbAlign from the world heap.
inline void *PvAllocSwClear(int cb, int cbAlign)
{
    SWALLOC swalloc;
    swalloc.cb = cb;
    SWALLOC *pswalloc = &swalloc;
    pswalloc->cbAlign = cbAlign;
    return PvAllocSwClearImpl(pswalloc);
}
// Allocate cb zeroed bytes with alignment cbAlign from the frame stack.
inline void *PvAllocStackClear(int cb, int cbAlign)
{
    SWALLOC swalloc;
    swalloc.cb = cb;
    SWALLOC *pswalloc = &swalloc;
    pswalloc->cbAlign = cbAlign;
    return PvAllocStackClearImpl(pswalloc);
}

/* g++ 2.95 vtable entry, for the hand-made calls through a load collector's vtable. */

/* Load collectors: LoadSwObjectsFromBrx hands every object it creates to one through vtable slot 1, Add(c, aplo). The
   array collector (vtable 0x2CBE50) keeps them; the proxy collector (vtable D_002CBE38) reparents and tags them. */
struct CLoadCollector
{
    virtual void Add(int cAdd, LO **aploAdd);   /* slot 1 (no vtable of its own in the ELF: nothing defines it) */
};
struct CLoadCollectorArray : CLoadCollector
{
    int cMax;
    int c;
    LO **aplo;
    virtual void Add(int cAdd, LO **aploAdd);   /* key function: CLoadCollectorArray's vtable comes out here */
};
typedef char assert_CLoadCollectorArray_size[sizeof(CLoadCollectorArray) == 0x10 ? 1 : -1];
struct CLoadCollectorProxy : CLoadCollector
{
    virtual void Add(int cAdd, LO **aploAdd) __asm__("CLoadCollectorProxy_Add");   /* key function */
    int nUnk48;    /* stored into each object's LO::proxyLoadKind (and its children's) */
    int idProxy;   /* -1, or set on each object with LO::SetIdProxy(proxyId, 1) */
};


/* SW::alangstr entry (the same view as proxy/ and brx_sw). */

extern CLASSDESC D_002BAAE8[];   /* the class descriptor table (sly2/basic.h) */
extern EOPID D_002AC880[];   /* g_aeopid */
extern const char D_002A4BE0[] = "";   /* the empty string (a zero-filled .rodata row) */
extern "C" {
void FramePtr_Assign(void *pspframe, void *frame) __asm__("__as__t7CRefPtr1Z6CFrameP6CFrame");
void *GetSpliceFrame(SW *world, int isplice, LO *plo);
DL *GetSwOidBucket(SW *world, OID oid);
DL *GetSwClassList(SW *world, CID cid);
void AppendSlEntry(DL *pdl, LO *plo);
int IsLoInWorld(LO *plo);
void CRef_ctor_CLQ(CRef *pref, float *ag);
void CRef_ctor_LM(CRef *pref, float *ag);
void CRef_ctor_VECTOR(CRef *pref, float *ag);
void *AllocFromStack(SWALLOC *pswalloc);
void SetLoParent(LO *plo, SO *psoParent, int grf);
void SetLoTagHierarchy(LO *plo, int n);
int IsCidDerivedFrom(CID cid, CID cidBase);
LO *FindSwObjectInProxyScope(SW *world, int grf, OID oid, LO *ploCtx, void *pchList);
PXR *FindProxyRefPath(PXR *ppxr, int n, int *pid);
}
LO *CreateLo(CID cid, SO *psoParent, OID oid, int isplice);
LO *FindSwObject(SW *world, int grf, OID oid, LO *ploCtx);
/* The same function called with a fifth argument (the target loads a4 before the call; 0x15E938 passes 0 on to
   FindSwObjectInProxyScope): an asm-labelled second form, as frm's PropagateSaisF. */
LO *FindSwObjectExtraArg(SW *world, int grf, OID oid, LO *ploCtx, void *unusedA4) __asm__("FindSwObject__FP2SWi3OIDP2LO");
void CopyAb(void *pvDst, void *pvSrc, uint cb);
int IsBasicDerivedFrom(BASIC *pbasic, CID cid);
int GetCurrentLevelIndex();
int MakeProxyId(int isrc, int n);
int IsCurrentLevelInList(int c, short *aid);
void LoadProxyFromBrx(SW *world, SO *psoParent, CID cid, OID oid, int isplice, CBinaryInputStream *stream,
                      CLoadCollector *plc);

/* PostLoadOidSetter calls the setter on an incomplete class, as the per-function file did (with LO the call
   schedules the PMF's index arithmetic differently: 2 of 43 off). */
class LOSETTEE;
struct OIDPMF
{
    OID oid;
    void (LOSETTEE::*pmf)(LO *);
};


// Returns the localized string with the given id from the world's string table, or "" when id is -1 or not found.
char *FindSwLocalizedString(SW *world, int id)
{
    int i;
    if (id == -1) return (char *)D_002A4BE0;
    for (i = 0; i < world->clangstr; i++)
    {
        if (world->alangstr[i].id == id) return world->alangstr[i].pchz;
    }
    return (char *)D_002A4BE0;
}

// Creates an object of class cid: allocates it through the class descriptor, sets parent, oid and Splice frame, links
// it into the world's oid and class lists, runs Init (slot 7) and the in-world callbacks, and records it in the
// current load collector. The general object factory.
// Also known as: PloNew (upstream readable / Sly 1 name; PloNew__F3CIDP2SW3OIDi).
LO *CreateLo(CID cid, SO *psoParent, OID oid, int isplice)
{
    SW *world;
    LO *plo = (LO *)(D_002BAAE8 + cid)->pfnNew();
    plo->parent = psoParent;
    plo->oid = oid;
    world = &g_sw;
    FramePtr_Assign(&plo->scriptFrame, GetSpliceFrame(world, isplice, plo));
    AppendSlEntry(GetSwOidBucket(world, oid), plo);
    AppendSlEntry(GetSwClassList(world, cid), plo);
    plo->Init();
    if ((uchar)plo->isInWorld)  /* (uchar): extract the bit as the target does ((int) adds a sign extension) */
        plo->AddHierarchy();
    if (IsLoInWorld(plo))
        plo->OnAdd();
    if (world->pploNew)
        *world->pploNew++ = plo;
    return plo;
}

// Clones an object under a new parent: a new object of the same class and oid, then CloneHierarchy (slot 40), which for
// an SO also clones the children. Layout parse-verified: a parser following it reads every byte of 4 levels.
// Also known as: PloCloneLo (upstream readable / Sly 1 name; PloCloneLo__FP2LOi).
LO *CloneLoWithParent(LO *plo, SO *psoParent)
{
    CLASSINFO *pclassinfo;
    LO *ploNew;
    if (!plo) return 0;
    pclassinfo = plo->GetClassInfo();
    ploNew = CreateLo(pclassinfo->cid, psoParent, plo->oid, -1);
    ploNew->CloneHierarchy(plo);   /* vtable slot 40 */
    return ploNew;
}

// Deferred post-load callback: resolves a stored oid to an object (FindSwObject relative to the object) and writes the
// pointer into the object's field.
void PostLoadOidField(LO *plo, void *pv)
{
    OIDFIELD *poidfield = (OIDFIELD *)pv;
    *(LO **)((char *)plo + poidfield->ib) = FindSwObject(&g_sw, 4, poidfield->oid, plo);
}

// Deferred post-load callback: resolves a stored oid to an object and calls the stored member-function setter with it.
void PostLoadOidSetter(LO *plo, void *pv)
{
    LO *ploFound = FindSwObject(&g_sw, 4, ((OIDPMF *)pv)->oid, plo);
    (((LOSETTEE *)plo)->*((OIDPMF *)pv)->pmf)(ploFound);
}

/* RefFromBrx: one typed value of a method option's argument list as a CRef (returned through the hidden pointer). */
// Reads one typed option argument from the level stream into a CRef by object type (bool, float, ints, vectors, 2D
// vectors, object ids, ...; unknown types give the null tag). (Sly 1: RefFromBrx.)
// Layout parse-verified: a parser following it reads every byte of 4 levels.
// Also known as: RefReadOption (upstream readable / Sly 1 name; RefReadOption__FiP18CBinaryInputStream).
extern "C" CRef *ReadOptionArgFromBrx(CRef *pref, int otyp, CBinaryInputStream *stream)
{
    float ag[3];
    switch (otyp)
    {
    case 0:
        {
            int f = stream->U8Read() != 0;
            pref->m_tagk = 0x17;
            pref->u.n = f;
        }
        break;
    case 5: case 6:
        pref->u.n = stream->S32Read();
        pref->m_tagk = 0x15;
        break;
    case 1:
        pref->u.g = stream->F32Read();
        pref->m_tagk = 0x16;
        break;
    case 7: case 14: case 15: case 16: case 17: case 19: case 20: case 21: case 23:
        pref->u.n = stream->S16Read();
        pref->m_tagk = 0x15;
        break;
    case 9:
        {
            float *pg = ag;
            int j;
            for (j = 0; j < 3; j++)
                *pg++ = stream->F32Read();
        }
        CRef_ctor_CLQ(pref, ag);
        break;
    case 10:
        ag[0] = stream->F32Read();
        ag[1] = stream->F32Read();
        CRef_ctor_LM(pref, ag);
        break;
    case 2:
        {
            float *pg = ag;
            int j;
            for (j = 0; j < 3; j++)
                *pg++ = stream->F32Read();
        }
        CRef_ctor_VECTOR(pref, ag);
        break;
    default:
        if (((otyp & ~0x8000) >> 12) == 1)
        {
            pref->u.n = stream->U32Read();
            pref->m_tagk = 0x15;
        }
        else
        {
            pref->SetTag((TAGK)0x24);
        }
        break;
    }
    return pref;
}

/* LoadOptionFromBrx (comment from its per-function file): */
// LoadOptionFromBrx: ported from Lilly's personal Sly 2 decomp (src/P2/brx.c), reduced to the declarations it needs.
// Every global is named by its address; the function keeps its readable name and gets the team symbol through __asm__.
// Globals: g_sw
// 0x0013E818 LoadOptionFromBrx__FPvP5EOPIDiP18CBinaryInputStream  (MATCHES Sly 2, T1071; 99.8% in T0906, 98.2% in T1003)
// T1071: the MMI packs are written as in quirks section 41 (the mfc1 INSIDE the asm, "f" float operands): case 3 and case 2 are each ONE fused asm
// (the game's registers and the order `lhu v0,2(sp)`, mfc1, mfc1, pextlw come out exactly). The seven spill slots at sp+0x268..0x280 are gcse's
// reaching pseudos of hoisted frame addresses; their order = the gcse hash-table size S = max_cuid/2|1 (section 37). The fused spelling has fewer insns
// after cse1 (S = 557) than the game (S 559..563 gives the game's slot order; the game's case 2 had 4+ more insns at that point). The 4-statement spelling
// has S = 559 but mis-orders lhu (99.8%). STAND-IN: `if (fNever) pbis->F32Read();` is dead code (cprop folds it, jump deletes it; the final code is
// unchanged, 1022 instructions) that only supplies those insns. What the game really had there is unknown.
// Reads the value of ONE option from a level file (or from a PROXYINIT record's bytes) and applies it to the object pvStruct through the
// option's record peopid of g_aeopid (brx.h EOPID, 0x28 bytes). grfeopid is the record's flags MASKED by the caller: LoadOptionsFromBrx
// (level files, 0x13F810) passes grf & 0x2580, AcquireProxySpare (0x1CAEC8, options of a copy's objects) grf & 0x2031. Four shapes:
//  1. grf & 0x401 - a METHOD option (g_aeopid[26] = Remove, otyp 0x12, grf 0x4401): record +0x18 arguments are read (each: a u16 type, bit 0x8000 = a u8
//     count of elements of type & 0x7fff -> a splice list by BuildSpliceListFromValues; else one RefFromBrx value) into 32 CRefs, then the record's
//     splice thunk is called: +8 when grf has 0x400 (FILE options: SplLoRemoveDeferred 0x100B10 = DeferLoRemove), else +0x10 (the thunk scripts call:
//     SplLoRemove 0x100B88 = LO::Remove NOW). So a level file's option 26 is a deferred Remove; AcquireProxySpare (mask 0x2031 has no 0x400) removes at once.
//  2. grf & 0x90 - the value is stored straight into a C member at pv + record +0x18 (pv = pmfObj(1) when grf & 0x2000), by type (the same type codes
//     as RefFromPv / SetOptionRecordValue). An OBJECT REFERENCE (otyp & 0x7fff >> 12 == 2, e.g. 0x2006) holds an oid (s32): with grf 0x80 (file) it is queued
//     on the object's run list as {oid, member offset} -> PostLoadOidField at PostLoad; else (0x10) stored at once with the offset at +0x10.
//  3. otherwise a typed SETTER (pointer to member function) is called with the value: the one at +0x18 when grf & 0x100, else the one at +0x10, on pv
//     (or pmfObj(1)). A 0x2xxx object reference is queued on the run list as {oid, the +0x18 setter} -> PostLoadOidSetter (grf & 0x100) or the +0x10
//     setter is called at once with the object found by PostLoadOidSetter; either way the object is FindSwObject(&g_sw, 4, oid, the loading object):
//     the nearest object with that oid (mode 4 of FindSwObjectsInProxyScope, relative to the loading object), found AFTER the whole level is loaded for files.
// The two MMI arms (types 2 and 3) pack a vector4 with mfc1 / pextlw / pcpyld and pass it by value in $a1. Found on the way: the incoming pvStruct is
// `void *const` (assign_parms marks a const parameter's pseudo RTX_UNCHANGING_P, so reload_cse and the schedulers move its reloads across the record stores:
// 91.1% -> 97.7%); the setters of non-int types are CASTS of the int setter (`(void (BASIC::*)(float))pmfSet`: gcc's cast of a pointer to member function is
// the idx > 0 ? ... : ... conditional that gives the bgtz in the game); the record arms are switches (jump tables 0x2A4C50 / 0x2A4CB0 / 0x2A4BF0).
// See re/notes/decomp-compiler-quirks.md sections 31, 37 and 41 for everything tried.
void LoadOptionFromBrx(void *const pvStruct, EOPID *peopid, GRFEOPID grfeopid, CBinaryInputStream *stream) __asm__("LoadOptionFromBrx__FPvP5EOPIDiP18CBinaryInputStream");
// Reads one option value from the level stream and applies it through its g_aeopid record (method call, direct member
// or setter); object-id references are deferred to post-load callbacks.
// Layout parse-verified: a parser following it reads every byte of 4 levels.
void LoadOptionFromBrx(void *const pvStruct, EOPID *peopid, GRFEOPID grfeopid, CBinaryInputStream *stream)
{
    int fNever = 0;
    if (grfeopid & 0x401)
    {
        CRef aref[32];
        int carg = peopid->crefReq;
        for (int iarg = 0; iarg < carg; iarg++)
        {
            int otyp = stream->U16Read();
            if (otyp & 0x8000)
            {
                CRef arefElt[16];
                OTYP aotyp[16];
                void *apv[16];
                int cElt = stream->U8Read();
                otyp &= ~0x8000;
                for (int iElt = 0; iElt < cElt; iElt++)
                {
                    arefElt[iElt] = RefFromBrx((OTYP)otyp, stream);
                    apv[iElt] = &arefElt[iElt].u;
                    aotyp[iElt] = (OTYP)otyp;
                }
                aref[iarg] = BuildSpliceListFromValues(cElt, aotyp, apv);
            }
            else
            {
                aref[iarg] = RefFromBrx((OTYP)otyp, stream);
            }
        }
        if (grfeopid & 0x400)
        {
            CRef ref = (*peopid->pfnthunkDeferred)((BASIC *)pvStruct, carg, aref);
        }
        else
        {
            CRef ref = (*peopid->pfnthunk)((BASIC *)pvStruct, carg, aref);
        }
        return;
    }
    if (grfeopid & 0x90)
    {
        char *pf;
        if (grfeopid & 0x2000)
        {
            void *(BASIC::*pmfObj)(int) = peopid->pmfObj;
            pf = (char *)(((BASIC *)pvStruct)->*pmfObj)(1) + peopid->ibFile;
        }
        else
        {
            pf = (char *)pvStruct + peopid->ibFile;
        }
        switch (peopid->otyp)
        {
        case 0:
            *(uchar *)pf = stream->U8Read();
            return;
        case 5:
        case 6:
            *(int *)pf = stream->S32Read();
            return;
        case 1:
            *(float *)pf = stream->F32Read();
            return;
        case 10:
            ((float *)pf)[0] = stream->F32Read();
            ((float *)pf)[1] = stream->F32Read();
            return;
        case 2:
        case 9:
        case 11:
            ((float *)pf)[0] = stream->F32Read();
            ((float *)pf)[1] = stream->F32Read();
            ((float *)pf)[2] = stream->F32Read();
            return;
        case 3:
        case 12:
            ((float *)pf)[0] = stream->F32Read();
            ((float *)pf)[1] = stream->F32Read();
            ((float *)pf)[2] = stream->F32Read();
            ((float *)pf)[3] = stream->F32Read();
            return;
        case 13:
        {
            RGBA rgba;
            *(uint *)&rgba = stream->U32Read();
            ColorCorrectRgba(&rgba);
            *(uint *)pf = *(uint *)&rgba;
            return;
        }
        case 4:
        {
            VECTOR4 vec;
            vec.x = stream->F32Read();
            vec.y = stream->F32Read();
            vec.z = stream->F32Read();
            *(MATRIX3 *)pf = LoadRotateMatrix(*(qword *)&vec);
            return;
        }
        case 22:
        {
            int c = stream->U16Read();
            ((int *)pf)[0] = c;
            void *pv = PvAllocSwClear(c * 2, 0x10);
            ((void **)pf)[1] = pv;
            stream->Read(((int *)pf)[0] * 2, pv);
            return;
        }
        case 7:
        case 14:
        case 15:
        case 16:
        case 17:
        case 19:
        case 20:
        case 21:
        case 23:
            *(int *)pf = stream->S16Read();
            return;
        default:
            switch ((peopid->otyp & ~0x8000) >> 12)
            {
            case 1:
                *(uint *)pf = stream->U32Read();
                return;
            case 2:
                if (grfeopid & 0x80)
                {
                    int oid = stream->S32Read();
                    OIDFIELD *poidfield = (OIDFIELD *)AllocSwPostLoadRecord(&g_sw);
                    poidfield->oid = (OID)oid;
                    poidfield->ib = pf - (char *)pvStruct;
                    LORUN *plorun = AddLoDeferredCallback((LO *)pvStruct);
                    plorun->argument = poidfield;
                    plorun->pfn = PostLoadOidField;
                    return;
                }
                OIDFIELD oidfield;
                oidfield.oid = (OID)stream->S32Read();
                oidfield.ib = peopid->ibSet;
                PostLoadOidField((LO *)pvStruct, &oidfield);
                return;
            }
            return;
        }
    }
    BASIC *pbasic;
    if (grfeopid & 0x2000)
    {
        void *(BASIC::*pmfObj)(int) = peopid->pmfObj;
        pbasic = (BASIC *)(((BASIC *)pvStruct)->*pmfObj)(1);
    }
    else
    {
        pbasic = (BASIC *)pvStruct;
    }
    void (BASIC::*pmfSet)(int);
    if (grfeopid & 0x100)
    {
        pmfSet = peopid->setFile.pmfSetN;
    }
    else
    {
        pmfSet = peopid->pmfSetN;
    }
    switch (peopid->otyp)
    {
    case 0:
        (pbasic->*pmfSet)(stream->U8Read());
        break;
    case 5:
    case 6:
        (pbasic->*pmfSet)(stream->S32Read());
        break;
    case 1:
        (pbasic->*(void (BASIC::*)(float))pmfSet)(stream->F32Read());
        break;
    case 10:
    {
        LM lm;
        lm.minValue = stream->F32Read();
        lm.maxValue = stream->F32Read();
        (pbasic->*(void (BASIC::*)(LM *))pmfSet)(&lm);
        break;
    }
    case 9:
    {
        CLQ clq;
        float g0 = stream->F32Read();
        float g1 = stream->F32Read();
        float g2 = stream->F32Read();
        clq.g0 = g0;
        clq.g1 = g1;
        clq.g2 = g2;
        (pbasic->*(void (BASIC::*)(CLQ *))pmfSet)(&clq);
        break;
    }
    case 11:
    {
        SMP smp;
        float g0 = stream->F32Read();
        float g1 = stream->F32Read();
        float g2 = stream->F32Read();
        smp.fastSpeed = g0;
        smp.slowSpeed = g1;
        smp.fastDuration = g2;
        (pbasic->*(void (BASIC::*)(SMP *))pmfSet)(&smp);
        break;
    }
    case 2:
    {
        float gX = stream->F32Read();
        float gY = stream->F32Read();
        float gZ = stream->F32Read();
        if (fNever)
        {
            stream->F32Read();
        }
        qword q;
        int iTmp;
        __asm__("mfc1 %0, %2\n\tmfc1 %1, %3\n\tpextlw %0, %1, %0\n\tmfc1 %1, %4\n\tpcpyld %0, %1, %0"
                : "=&r"(q), "=&r"(iTmp) : "f"(gX), "f"(gY), "f"(gZ));
        VECTOR4 vec;
        *(qword *)&vec = q;
        (pbasic->*(void (BASIC::*)(qword))pmfSet)(q);
        break;
    }
    case 12:
    {
        SMPA smpa;
        float g0 = stream->F32Read();
        float g1 = stream->F32Read();
        float g2 = stream->F32Read();
        float g3 = stream->F32Read();
        smpa.fastSpeed = g0;
        smpa.slowSpeed = g1;
        smpa.fastDuration = g2;
        smpa.maxSpeedChange = g3;
        (pbasic->*(void (BASIC::*)(SMPA *))pmfSet)(&smpa);
        break;
    }
    case 3:
    {
        float gX = stream->F32Read();
        float gY = stream->F32Read();
        float gZ = stream->F32Read();
        float gW = stream->F32Read();
        qword q; qword qwZW; qword qwTmp;
        __asm__("mfc1 %0, %3\n\tmfc1 %1, %4\n\tpextlw %0, %1, %0\n\tmfc1 %1, %5\n\tmfc1 %2, %6\n\tpextlw %1, %2, %1\n\tpcpyld %0, %1, %0"
                : "=&r"(q), "=&r"(qwZW), "=&r"(qwTmp) : "f"(gX), "f"(gY), "f"(gZ), "f"(gW));
        VECTOR4 vec;
        *(qword *)&vec = q;
        (pbasic->*(void (BASIC::*)(qword))pmfSet)(q);
        break;
    }
    case 13:
    {
        RGBA rgba;
        *(uint *)&rgba = stream->U32Read();
        ColorCorrectRgba(&rgba);
        RGBA rgbaSet = rgba;
        (pbasic->*(void (BASIC::*)(RGBA *))pmfSet)(&rgbaSet);
        break;
    }
    case 4:
    {
        VECTOR4 vec;
        vec.x = stream->F32Read();
        vec.y = stream->F32Read();
        vec.z = stream->F32Read();
        MATRIX3 mat;
        mat = LoadRotateMatrix(*(qword *)&vec);
        (pbasic->*(void (BASIC::*)(MATRIX3 *))pmfSet)(&mat);
        break;
    }
    case 22:
    {
        STACKSCOPE stackscope;
        int c = stream->U16Read();
        struct { int c; void *pv; } arr;
        arr.c = c;
        arr.pv = PvAllocStackClear(c * 2, 0x10);
        stream->Read(arr.c * 2, arr.pv);
        (pbasic->*(void (BASIC::*)(void *))pmfSet)(&arr);
        break;
    }
    case 7:
    case 14:
    case 15:
    case 16:
    case 17:
    case 19:
    case 20:
    case 21:
    case 23:
        (pbasic->*pmfSet)(stream->S16Read());
        break;
    default:
        switch ((peopid->otyp & ~0x8000) >> 12)
        {
        case 1:
            (pbasic->*pmfSet)(stream->U32Read());
            break;
        case 2:
            if (grfeopid & 0x100)
            {
                int oid = stream->S32Read();
                OIDSETTER *poidsetter = (OIDSETTER *)AllocSwPostLoadRecord(&g_sw);
                poidsetter->oid = (OID)oid;
                poidsetter->setter = peopid->setFile;
                LORUN *plorun = AddLoDeferredCallback((LO *)pvStruct);
                plorun->argument = poidsetter;
                plorun->pfn = PostLoadOidSetter;
                return;
            }
            OIDSETTER oidsetter;
            oidsetter.oid = (OID)stream->S32Read();
            oidsetter.setter = *(SETTER *)&peopid->pmfSet;
            PostLoadOidSetter((LO *)pvStruct, &oidsetter);
            break;
        }
        break;
    }
}

// Reads an object's option list: option ids (S16) until a negative one, each applied with LoadOptionFromBrx.
// Layout parse-verified: a parser following it reads every byte of 4 levels.
void LoadOptionsFromBrx(void *pv, CBinaryInputStream *stream)
{
    int eopid;
    while ((eopid = stream->S16Read()) >= 0)
    {
        EOPID *peopid = &D_002AC880[eopid];
        LoadOptionFromBrx(pv, peopid, peopid->grfeopid & 0x2580, stream);
    }
}

// CLoadCollectorArray constructor: an empty growable array of loaded objects.
extern "C" CLoadCollectorArray *CLoadCollectorArray_ctor(CLoadCollectorArray *plca)
{
    plca->cMax = 0;
    SLY2_SET_VPTR_AT(plca, 0, CLoadCollectorArray);
    plca->c = 0;
    plca->aplo = 0;
    return plca;
}

// Appends objects to the collector's array, doubling its capacity (at least 32) when full.
void CLoadCollectorArray::Add(int cAdd, LO **aploAdd)
{
    if (cMax < c + cAdd)
    {
        int cMaxNew = cMax;
        SWALLOC swalloc;
        LO **aploNew;
        do
        {
            cMaxNew = StdMaxInt(cMaxNew * 2, 32);
        } while (cMaxNew < c + cAdd);
        swalloc.cb = cMaxNew * 4;
        swalloc.cbAlign = 0x10;
        aploNew = (LO **)AllocFromStack(&swalloc);
        CopyAb(aploNew, aplo, c * 4);
        cMax = cMaxNew;
        aplo = aploNew;
    }
    CopyAb(aplo + c, aploAdd, cAdd * 4);
    c += cAdd;
}

// Sets the 4-bit field at bits 48..51 of the object's flag word, and recursively on the children of an SO. Used by the
// proxy collector. (inferred: the field's meaning is not known.)
extern "C" void SetLoTagHierarchy(LO *plo, int n)
{
    LO *ploChild;
    plo->proxyLoadKind = n;
    if (IsBasicDerivedFrom(plo, (CID)0x30))
    {
        for (ploChild = (LO *)((SO *)plo)->children.head; ploChild; ploChild = (LO *)ploChild->siblingLink.next)
            SetLoTagHierarchy(ploChild, n);
    }
}

// The proxy collector's Add: detaches each loaded object from any parent, sets its 4-bit tag field and, when the
// collector has a proxy id, registers that id on the object (slot 25).
void CLoadCollectorProxy::Add(int c, LO **aplo)   /* key function: CLoadCollectorProxy's vtable comes out here */
{
    CLoadCollectorProxy *plcp = this;
    int i;
    for (i = 0; i < c; i++)
    {
        LO *plo = aplo[i];
        SetLoParent(plo, 0, 0);
        SetLoTagHierarchy(plo, plcp->nUnk48);
        if (plcp->idProxy != -1)
            plo->SetIdProxy(plcp->idProxy, 1);
    }
}

// True when no load filter is active (GetCurrentLevelIndex() = g_sw.loadFilter, D_003E1110, is -1) or it is in the list.
// Also known as: FFilterMatches (upstream readable / Sly 1 name).
int IsCurrentLevelInList(int c, short *aid)
{
    int ilevel = GetCurrentLevelIndex();
    int i;
    if (ilevel == -1) return 1;
    for (i = 0; ; i++)
    {
        if (i >= c) return 0;
        if (aid[i] == ilevel) break;
    }
    return 1;
}

/* Collector call through vtable slot 1 (Add), spelled as the per-function file did. */
#define LC_ADD(plc, c, pplo) ((plc)->Add(c, pplo))

/* LoadSwObjectsFromBrx (Claude agent, laneC11 2026-10-23): matched from wip v3. One commented register pin (pbis in
   s5). The plo1->unk50 = 1 store is written in both arms of `if (cid == 0x84)`: gcc cross-jumps them and leaves the
   dead `li v1,0x84` the target has. */
// Reads an object list from the level stream: per object a kind, an optional filter list, cid, oid, Splice index and
// payload size; creates the object and calls its LoadFromBrx (slot 14). Proxies go through LoadProxyFromBrx and
// filtered-out objects are skipped by size. Layout parse-verified: a parser following it reads every byte of 4 levels.
// Also known as: LoadSwObjsFromBrx (upstream readable / Sly 1 name;
//   LoadSwObjsFromBrx__FP2SWP3ALOP18CBinaryInputStream).
void LoadSwObjectsFromBrx(SO *psoParent, CBinaryInputStream *pbisArg, CLoadCollector *plcParent)
{
    register CBinaryInputStream *stream asm("$21") = pbisArg; /* register pin (Claude agent): target keeps pbis in s5 */
    CLoadCollectorProxy lcp;
    short as[0x80];
    int cid, oid, cb, n;
    int t;
    LO *plo1;
    LO *plo2;
    int c;
    int i;
    lcp.nUnk48 = -1;
    lcp.idProxy = -1;
    c = stream->S16Read();
    for (i = 0; i < c; i++)
    {
        int k = stream->S8Read();
        CLoadCollector *plc;
        if (k > 0)
            goto read;
        if (k < -2)
        {
        read:
            stream->Read(k * 2, as);
        }
        plc = plcParent;
        cid = stream->S16Read();
        oid = stream->S32Read();
        n = stream->S16Read();
        cb = stream->S32Read();
        if (k < -2)
            goto filter;
        if (k >= 0)
        {
            if (k != 0)
                goto filter;
            goto create;
        }
        {
            SW *world = &g_sw;
            PXR *r;
            t = MakeProxyId(world->isrcLoading, -1);
            r = FindProxyRefPath(world->ppxrRoot, 1, &t);
            if (k == -2)
            {
                if (FindSwObjectExtraArg(world, 0x405, (OID)oid, 0, r))
                {
                    goto skip;
                }
                lcp.nUnk48 = 1;
            }
            else
            {
                if (FindSwObjectInProxyScope(world, 0x805, (OID)oid, 0, r))
                {
                    goto skip;
                }
                lcp.nUnk48 = 0;
            }
            lcp.idProxy = t;
            plc = &lcp;
            goto create;
        }
        {
        filter:
            if (IsCurrentLevelInList(k, as) == 0)
            {
                if (cid == 0x84 || cid == 0x85)
                {
                    plo1 = CreateLo((CID)cid, psoParent, (OID)oid, -1);
                    if (cid == 0x84)
                        *(int *)((char *)plo1 + 0x50) = 1;
                    else
                        *(int *)((char *)plo1 + 0x50) = 1;
                    plo1->LoadFromBrx(stream);
                    if (plcParent)
                        LC_ADD(plcParent, 1, &plo1);
                    continue;
                }
                goto skip;
            }
        }
    create:
        if (IsCidDerivedFrom((CID)cid, (CID)6))
        {
            LoadProxyFromBrx(&g_sw, psoParent, (CID)cid, (OID)oid, n, stream, plc);
        }
        else
        {
            plo2 = CreateLo((CID)cid, psoParent, (OID)oid, n);
            plo2->LoadFromBrx(stream);
            if (plc)
                LC_ADD(plc, 1, &plo2);
        }
        continue;
    skip:
        stream->Skip(cb);
    }
}

#include "sly2/port.h"
#if !defined(SLY2_EE_LAYOUT)
/* Native adapters for SLY2_SRET_LABEL (sly2/port.h; docs/guides/portability.md "Struct results and arm64"). The EE
   passes a struct result's address as a hidden first argument, so the declarations elsewhere that spell this file's
   functions with the other form (a by-value result for an explicit out-pointer, or the reverse) call the same code
   there; natively they bind to these entries, which have their spelling. Plain records with the definitions' sizes. */
namespace SRET_brx {
extern "C" CRef ReadOptionArgFromBrxValue(int otyp, CBinaryInputStream *stream)
{ CRef r; ReadOptionArgFromBrx(&r, otyp, stream); return r; }   /* r starts nil, so writing over it releases nothing */
} // namespace SRET_brx
#endif
