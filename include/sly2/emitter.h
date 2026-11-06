/* include/sly2/emitter.h -- the emitter block (EMITB) shared by EMITTER and EXPLO, and the option accessors over it.
 *
 * EMITTER (CID 0x1D : XFM, cb 0x160) and EXPLO (CID 0x20 : EXPL : XFM, cb 0xF0) both keep, right after XFM, a dirty
 * flag (+0xE0) and a pointer to a reference-counted emitter block (+0xE4) that PemitbEnsureEmitter /
 * PemitbEnsureExplo copy before a write (copy on write: share count at +0). Only the fields the accessors below read
 * are named; the rest of both classes is padding here.
 *
 * The accessors exist twice in the binary, once per class (EMITTER 0x258898..0x258EC8, EXPLO 0x259330..0x259960),
 * with the same code; they are written once as function templates over the emitter-bearing class, and
 * src/game/app/eopid_linkonce.cpp instantiates both sets by taking their addresses as the option table g_aeopid does.
 * See docs/guides/tu-merging.md "Inline functions and linkonce copies". Names are placeholders: each template is
 * named after its EMITTER copy's address (the EXPLO copy is the other instantiation of the same template). */
#ifndef SLY2_EMITTER_H
#define SLY2_EMITTER_H
#include "sly2/xfm.h"

/* ensure kind: what an accessor is asked for (the same enum as PemitterEnsureEmitter__FP7EMITTER4ENSK's) */
#ifndef SLY2_ENUM_ENSK
#define SLY2_ENUM_ENSK
enum ENSK { ENSK_Nil = -1, ENSK_Get = 0, ENSK_Set = 1 };
#endif

/* payload shapes of an EMITV slot (copied as structs) */
struct EMITV2 { float a[2]; };
struct EMITV3 { float a[3]; };
struct EMITV4 { float a[4]; };

/* a 0x40-byte variant record: a kind, and its payload at +0x30; the defaults it is reset from are whole records */
struct EMITV
{
    int k;                  /* +0x00 kind of the payload (-1 or 0: unset) */
    char pad04[0x2C];
    union
    {
        float g;
        int n;
        EMITV2 v2;
        EMITV3 v3;
        EMITV4 v4;
    } u;                    /* +0x30 */
} __attribute__((aligned(16)));
typedef char assert_EMITV_size[sizeof(EMITV) == 0x40 ? 1 : -1];

/* EMITB: the emitter block (only the fields the accessors use) */
struct EMITB
{
    int cref;               /* +0x00 share count (copy on write) */
    int kv;                 /* +0x04 which record the slot at +0xC0 holds (0, 1; -1 none) */
    char pad08[8];
    int kEmito;             /* +0x10 EMITO kind (1, 2, 3; 0 unset) */
    char pad14[0x1C];
    int pvEmito;            /* +0x30 EMITO data (allocated per kind; -1 for kind 2) */
    char pad34[0x8C];
    EMITV v;                /* +0xC0 */
};
typedef char assert_EMITB_v[SLY2_OFFSETOF(EMITB, v) == 0xC0 ? 1 : -1];

struct EMITTER : public XFM
{
    int fDirty;             /* +0xE0 set by an ensure for writing */
    EMITB *pemitb;          /* +0xE4 */
    char padE8[0x160 - 0xE8];
    virtual void Init() __asm__("InitEmitter");   /* slot 7 */
    virtual CLASSINFO *Pclassinfo() __asm__("GetEmitterClassDesc");   /* slot 6 */
    virtual void HandleMessage(MSGID msgid, void *pv) __asm__("HandleEmitterMessage__FP7EMITTER5MSGIDPv");   /* slot 5 */
    virtual void LoadFromBrx(CBinaryInputStream *pbis) __asm__("LoadEmitterFromBrx");   /* slot 14 */
    virtual void VUnk14() __asm__("PostEmitterLoad__FP7EMITTER");   /* slot 15 */
    virtual void VUnk15() __asm__("UpdateEmitter__FP7EMITTERf");   /* slot 16 */
    virtual void Clone(LO *ploBase) __asm__("CloneEmitter");   /* slot 41 */
};
typedef char assert_EMITTER_size[sizeof(EMITTER) == 0x160 ? 1 : -1];   /* class descriptor cb */

struct EXPL : public XFM
{
    virtual CLASSINFO *Pclassinfo() __asm__("GetExplClassDesc");   /* slot 6 */
    virtual void VSlot51() __asm__("ExplodeExplExplso__FP4EXPLP6EXPLSO");   /* slot 51 */
};
typedef char assert_EXPL_size[sizeof(EXPL) == 0xE0 ? 1 : -1];   /* class descriptor cb */

struct EXPLO : public EXPL
{
    int fDirty;             /* +0xE0 */
    EMITB *pemitb;          /* +0xE4 */
    char padE8[0xF0 - 0xE8];
    virtual CLASSINFO *Pclassinfo() __asm__("GetExploClassDesc");   /* slot 6 */
    virtual void LoadFromBrx(CBinaryInputStream *pbis) __asm__("LoadExploFromBrx");   /* slot 14 */
    virtual void VUnk14() __asm__("func_00259170");   /* slot 15 */
    virtual void Clone(LO *ploBase) __asm__("CloneExplo");   /* slot 41 */
};
typedef char assert_EXPLO_size[sizeof(EXPLO) == 0xF0 ? 1 : -1];   /* class descriptor cb */

/* the per-class callees, overloaded so that one template body calls the right one */
EMITB *PemitbEnsure(EMITTER *pemitter, ENSK ensk) __asm__("PemitbEnsureEmitter");
EMITB *PemitbEnsure(EXPLO *pexplo, ENSK ensk) __asm__("PemitbEnsureExplo");

/* the shared defaults (read-only records in .data/.bss) */
extern EMITV4 D_002BDDF0;
extern float D_002BDE00;
extern EMITV D_002BDE10;
extern EMITV2 D_002BDE50;
extern int D_002BDE58;
extern EMITV3 D_002BDE60;
extern EMITV D_002BDE70;
extern char D_002DE720[];
extern char D_002DE750[];

/* an allocation request for the level heap (TU-private copies of the same struct exist in several merged files) */
#ifndef SLY2_STRUCT_AREQ
#define SLY2_STRUCT_AREQ
struct AREQ { int cb; int cbAlign; };
#endif
/* allocate from the level heap (func_001AE2A0); a C++ name bound by label, so that the merged files' own extern "C"
   declarations of func_001AE2A0 (on their AREQ copies) do not conflict with it where classes.h is included */
void *PvAllocLevelHeap(AREQ *preq) __asm__("func_001AE2A0");

/* ---- the accessors (g_aeopid records: get +0xC, set +0x14, setUser +0x1C, ensure +0x24) ---- */

/* ensure: the slot at +0xC0 as record 0 (reset from D_002BDE10 on a write if unset); D_002DE720 if it holds the other */
template <class T> EMITV *func_00258898(T *p, ENSK ensk)
{
    EMITB *pemitb = PemitbEnsure(p, ensk);
    if (ensk == ENSK_Set && pemitb->kv == -1)
    {
        pemitb->kv = 0;
        pemitb->v = D_002BDE10;
    }
    if (pemitb->kv != 0)
        return (EMITV *)D_002DE720;
    return &pemitb->v;
}

/* ensure: the slot as record 1 (reset from D_002BDE70) */
template <class T> EMITV *func_00258920(T *p, ENSK ensk)
{
    EMITB *pemitb = PemitbEnsure(p, ensk);
    if (ensk == ENSK_Set && pemitb->kv == -1)
    {
        pemitb->kv = ensk;
        pemitb->v = D_002BDE70;
    }
    if (pemitb->kv != 1)
        return (EMITV *)D_002DE720;
    return &pemitb->v;
}

/* setUser: make the slot hold record kv */
template <class T> void func_002589A8(T *p, int kv)
{
    switch (kv)
    {
    case 0:
        func_00258898(p, ENSK_Set);
        return;
    case 1:
        func_00258920(p, ENSK_Set);
        break;
    }
}

/* get: which record the slot holds */
template <class T> int func_002589F0(T *p) { return p->pemitb->kv; }

/* setUser: set the EMITO kind and allocate its data (only from unset) */
template <class T> void func_00258A00(T *p, int k)
{
    EMITB *pemitb;
    if (p->pemitb->kEmito == k)
        return;
    if (p->pemitb->kEmito != 0)
        return;
    pemitb = PemitbEnsure(p, ENSK_Set);
    pemitb->kEmito = k;
    switch (k)
    {
    case 1: { AREQ req; req.cb = 0x20; req.cbAlign = 0x10; pemitb->pvEmito = (int)PvAllocLevelHeap(&req); } break;
    case 2: pemitb->pvEmito = -1; break;
    case 3: { AREQ req; req.cb = 0x30; req.cbAlign = 0x10; pemitb->pvEmito = (int)PvAllocLevelHeap(&req); } break;
    }
}

/* get: the EMITO kind */
template <class T> int func_00258AB8(T *p) { return p->pemitb->kEmito; }

/* ensure: the EMITO kind (as kind 0 on a write) */
template <class T> int *func_00258AC8(T *p, ENSK ensk)
{
    if (ensk == ENSK_Set)
        func_00258A00(p, 0);
    return &p->pemitb->kEmito;
}

/* ensure: the EMITO data (as kind 1 on a write) */
template <class T> int func_00258B00(T *p, ENSK ensk)
{
    if (ensk == ENSK_Set)
        func_00258A00(p, 1);
    return p->pemitb->pvEmito;
}

/* ensure: the EMITO kind (as kind 2 on a write) */
template <class T> int *func_00258B38(T *p, ENSK ensk)
{
    if (ensk == ENSK_Set)
        func_00258A00(p, 2);
    return &p->pemitb->kEmito;
}

/* setUser: record 1's payload kind (2, 3 or 8; only from unset), payload reset from its default */
template <class T> void func_00258B70(T *p, int k)
{
    EMITV *pv = func_00258920(p, ENSK_Set);
    if (pv->k == k)
        return;
    if (pv->k != -1)
        return;
    pv->k = k;
    switch (k)
    {
    case 2: pv->u.v2 = D_002BDE50; break;
    case 3: pv->u.n = D_002BDE58; break;
    case 8: pv->u.v3 = D_002BDE60; break;
    }
}

/* get: record 1's payload kind */
template <class T> int func_00258C40(T *p) { return func_00258920(p, ENSK_Get)->k; }

/* ensure: record 1's payload as kind 2, 3, 8 (D_002DE750 if it holds another kind) */
template <class T> void *func_00258C60(T *p, ENSK ensk)
{
    EMITV *pv;
    if (ensk == ENSK_Set)
    {
        func_00258B70(p, 2);
        pv = &p->pemitb->v;
    }
    else
        pv = func_00258920(p, ensk);
    if (pv->k != 2)
        return D_002DE750;
    return &pv->u;
}

template <class T> void *func_00258CC8(T *p, ENSK ensk)
{
    EMITV *pv;
    if (ensk == ENSK_Set)
    {
        func_00258B70(p, 3);
        pv = &p->pemitb->v;
    }
    else
        pv = func_00258920(p, ensk);
    if (pv->k != 3)
        return D_002DE750;
    return &pv->u;
}

template <class T> void *func_00258D30(T *p, ENSK ensk)
{
    EMITV *pv;
    if (ensk == ENSK_Set)
    {
        func_00258B70(p, 8);
        pv = &p->pemitb->v;
    }
    else
        pv = func_00258920(p, ensk);
    if (pv->k != 8)
        return D_002DE750;
    return &pv->u;
}

/* setUser: record 0's payload kind (1 or 2; only from unset), payload reset from its default */
template <class T> void func_00258D98(T *p, int k)
{
    EMITV *pv = func_00258898(p, ENSK_Set);
    if (pv->k == k)
        return;
    if (p->pemitb->v.k != 0)
        return;
    pv->k = k;
    switch (k)
    {
    case 2: pv->u.v4 = D_002BDDF0; break;
    case 1: pv->u.g = D_002BDE00; break;
    }
}

/* get: record 0's payload kind */
template <class T> int func_00258E40(T *p) { return func_00258898(p, ENSK_Get)->k; }

/* ensure: record 0's payload as kind 2, 1 */
template <class T> void *func_00258E60(T *p, ENSK ensk)
{
    EMITV *pv;
    if (ensk == ENSK_Set)
    {
        func_00258D98(p, 2);
        pv = &p->pemitb->v;
    }
    else
        pv = func_00258898(p, ensk);
    if (pv->k != 2)
        return D_002DE750;
    return &pv->u;
}

template <class T> void *func_00258EC8(T *p, ENSK ensk)
{
    EMITV *pv;
    if (ensk == ENSK_Set)
    {
        func_00258D98(p, 1);
        pv = &p->pemitb->v;
    }
    else
        pv = func_00258898(p, ensk);
    if (pv->k != 1)
        return D_002DE750;
    return &pv->u;
}

#endif /* SLY2_EMITTER_H */
