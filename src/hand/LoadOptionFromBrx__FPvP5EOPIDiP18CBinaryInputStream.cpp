// LoadOptionFromBrx: ported from Lilly's personal Sly 2 decomp (src/P2/brx.c), reduced to the declarations it needs.
// Every global is named by its address; the function keeps its readable name and gets the team symbol through __asm__.
// Globals: D_003DD490 (g_sw)
// 0x0013E818 LoadOptionFromBrx__FPvP5EOPIDiP18CBinaryInputStream  (MATCHES Sly 2, T1071; 99.8% in T0906, 98.2% in T1003)
// T1071: the MMI packs are written as in quirks section 41 (the mfc1 INSIDE the asm, "f" float operands): case 3 and case 2 are each ONE fused asm
// (the game's registers and the order `lhu v0,2(sp)`, mfc1, mfc1, pextlw come out exactly). The seven spill slots at sp+0x268..0x280 are gcse's
// reaching pseudos of hoisted frame addresses; their order = the gcse hash-table size S = max_cuid/2|1 (section 37). The fused spelling has fewer insns
// after cse1 (S = 557) than the game (S 559..563 gives the game's slot order; the game's case 2 had 4+ more insns at that point). The 4-statement spelling
// has S = 559 but mis-orders lhu (99.8%). STAND-IN: `if (fNever) pbis->F32Read();` is dead code (cprop folds it, jump deletes it; the final code is
// unchanged, 1022 instructions) that only supplies those insns. What the game really had there is unknown.
// Reads the value of ONE option from a level file (or from a PROXYINIT record's bytes) and applies it to the object pvStruct through the
// option's record peopid of g_aeopid (brx.h EOPID, 0x28 bytes). grfeopid is the record's flags MASKED by the caller: LoadOptionsFromBrx
// (level files, 0x13F810) passes grf & 0x2580, PspareAcquire (0x1CAEC8, options of a copy's objects) grf & 0x2031. Four shapes:
//  1. grf & 0x401 - a METHOD option (g_aeopid[26] = Remove, otyp 0x12, grf 0x4401): record +0x18 arguments are read (each: a u16 type, bit 0x8000 = a u8
//     count of elements of type & 0x7fff -> a splice list by RefSetArgListFromPvs; else one RefFromBrx value) into 32 CRefs, then the record's
//     splice thunk is called: +8 when grf has 0x400 (FILE options: SplLoRemoveDeferred 0x100B10 = DeferLoRemove), else +0x10 (the thunk scripts call:
//     SplLoRemove 0x100B88 = LO::Remove NOW). So a level file's option 26 is a deferred Remove; PspareAcquire (mask 0x2031 has no 0x400) removes at once.
//  2. grf & 0x90 - the value is stored straight into a C member at pv + record +0x18 (pv = pmfObj(1) when grf & 0x2000), by type (the same type codes
//     as RefFromPv / RefSetPeopid). An OBJECT REFERENCE (otyp & 0x7fff >> 12 == 2, e.g. 0x2006) holds an oid (s32): with grf 0x80 (file) it is queued
//     on the object's run list as {oid, member offset} -> PostLoadOidField at PostLoad; else (0x10) stored at once with the offset at +0x10.
//  3. otherwise a typed SETTER (pointer to member function) is called with the value: the one at +0x18 when grf & 0x100, else the one at +0x10, on pv
//     (or pmfObj(1)). A 0x2xxx object reference is queued on the run list as {oid, the +0x18 setter} -> PostLoadOidSetter (grf & 0x100) or the +0x10
//     setter is called at once with the object found by PostLoadOidSetter; either way the object is PloFindSwObject(&g_sw, 4, oid, the loading object):
//     the nearest object with that oid (mode 4 of CploFindSwObjects, relative to the loading object), found AFTER the whole level is loaded for files.
// The two MMI arms (types 2 and 3) pack a vector4 with mfc1 / pextlw / pcpyld and pass it by value in $a1. Found on the way: the incoming pvStruct is
// `void *const` (assign_parms marks a const parameter's pseudo RTX_UNCHANGING_P, so reload_cse and the schedulers move its reloads across the record stores:
// 91.1% -> 97.7%); the setters of non-int types are CASTS of the int setter (`(void (BASIC::*)(float))pmfSet`: gcc's cast of a pointer to member function is
// the idx > 0 ? ... : ... conditional that gives the bgtz in the game); the record arms are switches (jump tables 0x2A4C50 / 0x2A4CB0 / 0x2A4BF0).
// See re/notes/decomp-compiler-quirks.md sections 31, 37 and 41 for everything tried.
#include "sly2/basic.h"
#include "sly2/lo.h"
MATRIX3 LoadRotateMatrix(qword vec);
LORUN *AddLoRunListEntry(LO *plo);
class CBinaryStreamBase
{
public:
    int m_grfbis;
    byte *m_pb;
    int m_cb;
    int m_ibStream;
    byte *m_pbRaw;
    int m_cbRaw;
    int m_grfDecomp;
    int m_cbSpillOver;
    int m_unk20;
    byte *m_abWindow;
    int m_unk28;
    int m_unk2c;
};
class CBinaryInputStream : public CBinaryStreamBase
{
public:
    int m_cbSpool;
    byte *m_abSpool;
    int m_fd;
    int m_cbDone;
    int m_cbRequest;
    int m_cbRemaining;
    int m_cbRetry;
    int m_isector;
    int m_cbuf;
    int m_cbufFill;
    int m_icbufRead;
    int m_icbufWrite;
    void Read(int cb, void *pv);
    uchar U8Read();
    ushort U16Read();
    uint U32Read();
    short S16Read();
    int S32Read();
    float F32Read();
};
extern SW D_003DD490;
typedef void (BASIC::*PMF)();
union SETTER
{
    int ibSet;
    PMF pmfSet;
    PFNTHUNK pfnthunk;
    void (BASIC::*pmfSetN)(int);
};
struct EOPID
{
      OTYP otyp;
      GRFEOPID grfeopid;
      union
    {
        int ib;
        PFNTHUNK pfnthunkDeferred;
        PMF pmfGet;
    };
      union
    {
        int ibSet;
        PMF pmfSet;
        PFNTHUNK pfnthunk;
        void (BASIC::*pmfSetN)(int);
    };
      union { SETTER setFile; int ibFile; int crefReq; };
      void *(BASIC::*pmfObj)(int);
};
struct SWALLOC
{
    int cb;
    int cbAlign;
};
void *PvAllocSwClearImpl(SWALLOC *pswalloc);
void *PvAllocStackClearImpl(SWALLOC *pswalloc);
struct STACKSCOPE
{
    STACKSCOPE();
    ~STACKSCOPE();
};
union TAG
{
    int m_n;
    float m_g;
    int m_bool;
    uint m_symid;
    BIFK m_bifk;
    const VECTOR4* m_pvec4;
    CPair* m_ppair;
    CProc* m_pproc;
    VECTOR* m_pvector;
    MATRIX4* m_pmatrix;
    CLQ* m_pclq;
    LM* m_plm;
    SMP* m_psmp;
    BASIC* m_pbasic;
    CMethod* m_pmethod;
};
class CRef
{
public:
    TAGK m_tagk;
    TAG m_tag;
    CRef() { m_tagk = TAGK_Nil; }
    CRef(TAGK tagk) { SetTag(tagk); }
    CRef(TAGK tagk, int n) { m_tagk = tagk; m_tag.m_n = n; }
    CRef(TAGK tagk, float g) { m_tagk = tagk; m_tag.m_g = g; }
    ~CRef() { Decref(); }
    CRef &operator=(const CRef &ref);
    bool operator==(const CRef &ref);
    void Decref();
    void SetTag(TAGK tagk);
};
CRef RefSetArgListFromPvs(int cargs, OTYP *aotyp, void **apv);
struct OIDFIELD
{
    OID oid;
    int ib;
};
struct OIDSETTER
{
    OID oid;
    SETTER setter;
};
void PostLoadOidField(LO *plo, void *pv);
void PostLoadOidSetter(LO *plo, void *pv);
CRef RefFromBrx(OTYP otyp, CBinaryInputStream *pbis);
void *func_00210120(SW *psw);
void FixupRgba(RGBA *prgba);
inline void *PvAllocSwClear(int cb, int cbAlign)
{
    SWALLOC swalloc;
    swalloc.cb = cb;
    SWALLOC *pswalloc = &swalloc;
    pswalloc->cbAlign = cbAlign;
    return PvAllocSwClearImpl(pswalloc);
}
inline void *PvAllocStackClear(int cb, int cbAlign)
{
    SWALLOC swalloc;
    swalloc.cb = cb;
    SWALLOC *pswalloc = &swalloc;
    pswalloc->cbAlign = cbAlign;
    return PvAllocStackClearImpl(pswalloc);
}
void LoadOptionFromBrx(void *const pvStruct, EOPID *peopid, GRFEOPID grfeopid, CBinaryInputStream *pbis) __asm__("LoadOptionFromBrx__FPvP5EOPIDiP18CBinaryInputStream");
void LoadOptionFromBrx(void *const pvStruct, EOPID *peopid, GRFEOPID grfeopid, CBinaryInputStream *pbis)
{
    int fNever = 0;
    if (grfeopid & 0x401)
    {
        CRef aref[32];
        int carg = peopid->crefReq;
        for (int iarg = 0; iarg < carg; iarg++)
        {
            int otyp = pbis->U16Read();
            if (otyp & 0x8000)
            {
                CRef arefElt[16];
                OTYP aotyp[16];
                void *apv[16];
                int cElt = pbis->U8Read();
                otyp &= ~0x8000;
                for (int iElt = 0; iElt < cElt; iElt++)
                {
                    arefElt[iElt] = RefFromBrx((OTYP)otyp, pbis);
                    apv[iElt] = &arefElt[iElt].m_tag;
                    aotyp[iElt] = (OTYP)otyp;
                }
                aref[iarg] = RefSetArgListFromPvs(cElt, aotyp, apv);
            }
            else
            {
                aref[iarg] = RefFromBrx((OTYP)otyp, pbis);
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
            *(uchar *)pf = pbis->U8Read();
            return;
        case 5:
        case 6:
            *(int *)pf = pbis->S32Read();
            return;
        case 1:
            *(float *)pf = pbis->F32Read();
            return;
        case 10:
            ((float *)pf)[0] = pbis->F32Read();
            ((float *)pf)[1] = pbis->F32Read();
            return;
        case 2:
        case 9:
        case 11:
            ((float *)pf)[0] = pbis->F32Read();
            ((float *)pf)[1] = pbis->F32Read();
            ((float *)pf)[2] = pbis->F32Read();
            return;
        case 3:
        case 12:
            ((float *)pf)[0] = pbis->F32Read();
            ((float *)pf)[1] = pbis->F32Read();
            ((float *)pf)[2] = pbis->F32Read();
            ((float *)pf)[3] = pbis->F32Read();
            return;
        case 13:
        {
            RGBA rgba;
            *(uint *)&rgba = pbis->U32Read();
            FixupRgba(&rgba);
            *(uint *)pf = *(uint *)&rgba;
            return;
        }
        case 4:
        {
            VECTOR4 vec;
            vec.x = pbis->F32Read();
            vec.y = pbis->F32Read();
            vec.z = pbis->F32Read();
            *(MATRIX3 *)pf = LoadRotateMatrix(*(qword *)&vec);
            return;
        }
        case 22:
        {
            int c = pbis->U16Read();
            ((int *)pf)[0] = c;
            void *pv = PvAllocSwClear(c * 2, 0x10);
            ((void **)pf)[1] = pv;
            pbis->Read(((int *)pf)[0] * 2, pv);
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
            *(int *)pf = pbis->S16Read();
            return;
        default:
            switch ((peopid->otyp & ~0x8000) >> 12)
            {
            case 1:
                *(uint *)pf = pbis->U32Read();
                return;
            case 2:
                if (grfeopid & 0x80)
                {
                    int oid = pbis->S32Read();
                    OIDFIELD *poidfield = (OIDFIELD *)func_00210120(&D_003DD490);
                    poidfield->oid = (OID)oid;
                    poidfield->ib = pf - (char *)pvStruct;
                    LORUN *plorun = AddLoRunListEntry((LO *)pvStruct);
                    plorun->pv = poidfield;
                    plorun->pfn = PostLoadOidField;
                    return;
                }
                OIDFIELD oidfield;
                oidfield.oid = (OID)pbis->S32Read();
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
        (pbasic->*pmfSet)(pbis->U8Read());
        break;
    case 5:
    case 6:
        (pbasic->*pmfSet)(pbis->S32Read());
        break;
    case 1:
        (pbasic->*(void (BASIC::*)(float))pmfSet)(pbis->F32Read());
        break;
    case 10:
    {
        LM lm;
        lm.gMin = pbis->F32Read();
        lm.gMax = pbis->F32Read();
        (pbasic->*(void (BASIC::*)(LM *))pmfSet)(&lm);
        break;
    }
    case 9:
    {
        CLQ clq;
        float g0 = pbis->F32Read();
        float g1 = pbis->F32Read();
        float g2 = pbis->F32Read();
        clq.g0 = g0;
        clq.g1 = g1;
        clq.g2 = g2;
        (pbasic->*(void (BASIC::*)(CLQ *))pmfSet)(&clq);
        break;
    }
    case 11:
    {
        SMP smp;
        float g0 = pbis->F32Read();
        float g1 = pbis->F32Read();
        float g2 = pbis->F32Read();
        smp.svFast = g0;
        smp.svSlow = g1;
        smp.dtFast = g2;
        (pbasic->*(void (BASIC::*)(SMP *))pmfSet)(&smp);
        break;
    }
    case 2:
    {
        float gX = pbis->F32Read();
        float gY = pbis->F32Read();
        float gZ = pbis->F32Read();
        if (fNever)
        {
            pbis->F32Read();
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
        float g0 = pbis->F32Read();
        float g1 = pbis->F32Read();
        float g2 = pbis->F32Read();
        float g3 = pbis->F32Read();
        smpa.svFast = g0;
        smpa.svSlow = g1;
        smpa.dtFast = g2;
        smpa.sdvMax = g3;
        (pbasic->*(void (BASIC::*)(SMPA *))pmfSet)(&smpa);
        break;
    }
    case 3:
    {
        float gX = pbis->F32Read();
        float gY = pbis->F32Read();
        float gZ = pbis->F32Read();
        float gW = pbis->F32Read();
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
        *(uint *)&rgba = pbis->U32Read();
        FixupRgba(&rgba);
        RGBA rgbaSet = rgba;
        (pbasic->*(void (BASIC::*)(RGBA *))pmfSet)(&rgbaSet);
        break;
    }
    case 4:
    {
        VECTOR4 vec;
        vec.x = pbis->F32Read();
        vec.y = pbis->F32Read();
        vec.z = pbis->F32Read();
        MATRIX3 mat;
        mat = LoadRotateMatrix(*(qword *)&vec);
        (pbasic->*(void (BASIC::*)(MATRIX3 *))pmfSet)(&mat);
        break;
    }
    case 22:
    {
        STACKSCOPE stackscope;
        int c = pbis->U16Read();
        struct { int c; void *pv; } arr;
        arr.c = c;
        arr.pv = PvAllocStackClear(c * 2, 0x10);
        pbis->Read(arr.c * 2, arr.pv);
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
        (pbasic->*pmfSet)(pbis->S16Read());
        break;
    default:
        switch ((peopid->otyp & ~0x8000) >> 12)
        {
        case 1:
            (pbasic->*pmfSet)(pbis->U32Read());
            break;
        case 2:
            if (grfeopid & 0x100)
            {
                int oid = pbis->S32Read();
                OIDSETTER *poidsetter = (OIDSETTER *)func_00210120(&D_003DD490);
                poidsetter->oid = (OID)oid;
                poidsetter->setter = peopid->setFile;
                LORUN *plorun = AddLoRunListEntry((LO *)pvStruct);
                plorun->pv = poidsetter;
                plorun->pfn = PostLoadOidSetter;
                return;
            }
            OIDSETTER oidsetter;
            oidsetter.oid = (OID)pbis->S32Read();
            oidsetter.setter = *(SETTER *)&peopid->pmfSet;
            PostLoadOidSetter((LO *)pvStruct, &oidsetter);
            break;
        }
        break;
    }
}
