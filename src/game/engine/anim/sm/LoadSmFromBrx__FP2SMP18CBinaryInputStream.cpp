// LoadSmFromBrx__FP2SMP18CBinaryInputStream: ported from Lilly's personal Sly 2 decomp (src/P2/sm.c), reduced to the declarations it needs.
// Every global is named by its address; the function keeps its readable name and gets the team symbol through __asm__.
// 0x001EFAC0 LoadFromBrx__2SMP18CBinaryInputStream  (MATCHES Sly 2)
// Sly 2's LoadSmFromBrx (SM virtual 13): reads the animation segments the machine plays (a nested object list, held out of the world
// with Remove), the states (oid + follow-up oid, turned into a state index; 0x20 means "any": -2), the transitions (from, to, route flags,
// break flags, probability, the oid of its animation segment; one whose segment is not in the list is dropped), the options, and builds one
// route table (SMRT) per group of routes: the number of steps between every two states, by breadth first search over the transitions of the
// route (a reversible one, bit 1, also counts backwards). Route groups: bits 4..0x1000 of the OR of all grfsmt; the group of a bit is the
// AND of the flags of the transitions that have the bit, minus the flags of the transitions that overlap it without having it.
// What T0921 / T1006 found (the code below has all of it): the block allocations are an inline helper with a local SWALLOC {cb, 16} whose cbAlign goes
// through a SWALLOC * (as in proxy.c); `i` is ONE variable for the object / state / segment-search / table-row loops (that is what gives the
// original's register numbers: this = s3, pbis = s4, i = s1, j = s0), `j` ONE for the transition, route-group and BFS-transition loops;
// the follow-up of a state is a `switch`; the diagonal of the table is a ternary; the group loop needs a goto (or an early-out of its search
// loop) to get the original's single loop copy; operands of the flag tests are `grfBit & grfAll`; the BFS is `for (cstep = 1;; cstep++) {
// ... if (!fChanged) break; }` (a do-while increments `cstep + 1` early: PRE copy).
// `plo` is ONE variable too, declared at the top and first used by the Remove loop: jump.c duplicate_loop_exit_test gives a pseudo that is set and last used inside
// the copied exit code NEW registers only if this very insn is its first use; a `plo` local to the segment search was copied (two pseudos, two `sw`s that
// jump2 cannot merge, v1 / v0), the shared one stays one pseudo in the peeled first iteration and the loop (one `sw a2,0(s2)`).
#include "sly2/basic.h"
#include "sly2/lo.h"
#include "sly2/aseg.h"
void InitDl(DL *pdl, int ibDle);
struct SMRT
{
      int grf;
      signed char *ab;
};
struct SMINIT
{
      OID oidRoot;
      OID oidInitialState;
      GRFAPL grfapl;
};
struct SMT
{
    union
    {
          int fAseg;
          ASEG *paseg;
    };
      int ismsFrom;
      int ismsTo;
      GRFSMT grfsmt;
      GRFSMT grfsmtBreak;
      float gProbability;
};
struct SMS
{
      OID oid;
    union
    {
          OID oidNext;
          int ismsNext;
    };
};
int IsmsFindSmRequired(SM *psm, OID oid);
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
    uchar U8Read();
    int S32Read();
    float F32Read();
};
void *PvAllocSwCopyImpl(struct SWALLOC *pswalloc, void *pvBase);
struct SWALLOC
{
    int cb;
    int cbAlign;
};
void *PvAllocSwImpl(SWALLOC *pswalloc);
void *PvAllocStackClearImpl(SWALLOC *pswalloc);
struct STACKSCOPE
{
    STACKSCOPE();
    ~STACKSCOPE();
};
void LoadOptionsFromBrx(void *pvStruct, CBinaryInputStream *pbis);
class CLoadCollector
{
public:
};
class CLoadCollectorArray : public CLoadCollector
{
public:
    CLoadCollectorArray();
    int m_unk4;
    int m_cplo;
    LO **m_aplo;
};
void LoadSwObjectsFromBrx(SO *psoParent, CBinaryInputStream *pbis, CLoadCollector *pcollector);
inline void *PvAllocSw(int cb, int cbAlign)
{
    SWALLOC swalloc;
    swalloc.cb = cb;
    SWALLOC *pswalloc = &swalloc;
    pswalloc->cbAlign = cbAlign;
    return PvAllocSwImpl(pswalloc);
}
inline void *PvAllocStackClear(int cb, int cbAlign)
{
    SWALLOC swalloc;
    swalloc.cb = cb;
    SWALLOC *pswalloc = &swalloc;
    pswalloc->cbAlign = cbAlign;
    return PvAllocStackClearImpl(pswalloc);
}
inline void *PvAllocSwCopy(int cb, int cbAlign, void *pvBase)
{
    SWALLOC swalloc;
    swalloc.cb = cb;
    SWALLOC *pswalloc = &swalloc;
    pswalloc->cbAlign = cbAlign;
    return PvAllocSwCopyImpl(pswalloc, pvBase);
}
void SM::LoadFromBrx(CBinaryInputStream *pbis)
{
    int i = 0;
    int j;
    int cstep;
    int fChanged;
    LO *plo;
    InitDl(&dlSma, 0x14);
    STACKSCOPE stackscope;
    CLoadCollectorArray collector;
    LoadSwObjectsFromBrx(psoParent, pbis, &collector);
    for (i = 0; i < collector.m_cplo; i++)
    {
        plo = collector.m_aplo[i];
        plo->Remove();
    }
    csms = pbis->U8Read();
    asms = (SMS *)PvAllocSw(csms * sizeof(SMS), 16);
    for (i = 0; i < csms; i++)
    {
        SMS *psms = &asms[i];
        psms->oid = (OID)pbis->S32Read();
        psms->oidNext = (OID)pbis->S32Read();
    }
    for (i = 0; i < csms; i++)
    {
        SMS *psms = &asms[i];
        switch (psms->oidNext)
        {
        case OID_Nil:
            psms->ismsNext = -1;
            break;
        case 0x20:
            psms->ismsNext = -2;
            break;
        default:
            psms->ismsNext = IsmsFindSmRequired(this, psms->oidNext);
            break;
        }
    }
    int grfAll = 0;
    csmt = pbis->U8Read();
    asmt = (SMT *)PvAllocStackClear(csmt * sizeof(SMT), 16);
    for (j = 0; j < csmt; j++)
    {
        SMT *psmt = &asmt[j];
        psmt->ismsFrom = IsmsFindSmRequired(this, (OID)pbis->S32Read());
        psmt->ismsTo = IsmsFindSmRequired(this, (OID)pbis->S32Read());
        psmt->grfsmt = pbis->S32Read();
        psmt->grfsmtBreak = pbis->S32Read();
        psmt->gProbability = pbis->F32Read();
        OID oidAseg = (OID)pbis->S32Read();
        if (oidAseg != OID_Nil)
        {
            for (i = 0; i < collector.m_cplo; i++)
            {
                plo = collector.m_aplo[i];
                if (plo->oid == oidAseg)
                {
                    psmt->paseg = (ASEG *)plo;
                    break;
                }
            }
            if (!psmt->paseg)
            {
                j--;
                csmt--;
                continue;
            }
        }
        grfAll |= psmt->grfsmt & 0xfffc;
    }
    asmt = (SMT *)PvAllocSwCopy(csmt * sizeof(SMT), 16, asmt);
    LoadOptionsFromBrx(this, pbis);
    int agrf[32];
    int cgrf = 0;
    for (int grfBit = 4; grfBit <= 0x1000; grfBit <<= 1)
    {
        if (grfBit & grfAll)
        {
            for (int igrf = 0; igrf < cgrf; igrf++)
            {
                if (grfBit & agrf[igrf])
                    goto next;
            }
            int grf = grfAll;
            for (j = 0; j < csmt; j++)
            {
                if (asmt[j].grfsmt & grfBit)
                {
                    grf &= asmt[j].grfsmt;
                }
            }
            for (j = 0; j < csmt; j++)
            {
                if ((asmt[j].grfsmt & grf) && !(asmt[j].grfsmt & grfBit))
                {
                    grf &= ~asmt[j].grfsmt;
                }
            }
            agrf[cgrf++] = grf;
        }
    next:;
    }
    csmrt = cgrf;
    asmrt = (SMRT *)PvAllocSw(csmrt * sizeof(SMRT), 16);
    for (int ismrt = 0; ismrt < csmrt; ismrt++)
    {
        SMRT *psmrt = &asmrt[ismrt];
        psmrt->grf = agrf[ismrt];
        psmrt->ab = (signed char *)PvAllocSw(csms * csms, 16);
        for (int ismsFrom = 0; ismsFrom < csms; ismsFrom++)
        {
            for (int ismsTo = 0; ismsTo < csms; ismsTo++)
            {
                psmrt->ab[ismsFrom * csms + ismsTo] = (ismsFrom == ismsTo) ? 0 : -1;
            }
        }
        for (cstep = 1;; cstep++)
        {
            fChanged = 0;
            for (j = 0; j < csmt; j++)
            {
                SMT *psmt = &asmt[j];
                if (psmt->grfsmt & psmrt->grf)
                {
                    for (i = 0; i < csms; i++)
                    {
                        if (psmrt->ab[i * csms + psmt->ismsFrom] == cstep - 1 && psmrt->ab[i * csms + psmt->ismsTo] < 0)
                        {
                            psmrt->ab[i * csms + psmt->ismsTo] = cstep;
                            fChanged = 1;
                        }
                        else if ((psmt->grfsmt & 1) && psmrt->ab[i * csms + psmt->ismsTo] == cstep - 1 &&
                                 psmrt->ab[i * csms + psmt->ismsFrom] < 0)
                        {
                            psmrt->ab[i * csms + psmt->ismsFrom] = cstep;
                            fChanged = 1;
                        }
                    }
                }
            }
            if (!fChanged)
                break;
        }
    }
}
