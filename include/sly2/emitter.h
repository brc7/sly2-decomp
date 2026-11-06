/* include/sly2/emitter.h -- the emitter block (EMITB) shared by EMITTER and EXPLO, and the option accessors over it.
 *
 * EMITTER (CID 0x1D : XFM, cb 0x160) and EXPLO (CID 0x20 : EXPL : XFM, cb 0xF0) both keep, right after XFM, a dirty
 * flag (+0xE0) and a pointer to a reference-counted emitter block (+0xE4) that PemitbEnsure copies before a write
 * (copy on write: share count at +0). Only the fields the code reads are named (EMITTER's for engine/fx/emitter.cpp,
 * which uses this EMITTER and EMITB); the rest is padding here.
 *
 * Their linkonce members (LoadFromBrx, Clone, the post-load reference binding, PemitbEnsure, the blip colours and a
 * second Pclassinfo) also exist once per class with the same code (EMITTER 0x2584A8..0x258F30, EXPLO
 * 0x258F40..0x2599C8): function templates as well, instantiated by emitter.cpp.
 *
 * The accessors exist twice in the binary, once per class (EMITTER 0x258898..0x258EC8, EXPLO 0x259330..0x259960),
 * with the same code; they are written once as function templates over the emitter-bearing class, and the option
 * table g_aeopid (src/game/app/class_registry_options.inc, EOPID_PMFT) instantiates both sets by taking their addresses.
 * See docs/guides/tu-merging.md "Inline functions and linkonce copies". Names are placeholders: each template is
 * named after its EMITTER copy's name (the EXPLO copy is the other instantiation of the same template). */
#ifndef SLY2_EMITTER_H
#define SLY2_EMITTER_H
#include "sly2/xfm.h"
#include "sly2/util.h"
struct RIPG; struct BLIPG; struct SW;

/* ensure kind: what an accessor is asked for (the same enum as PemitterEnsureEmitter__FP7EMITTER4ENSK's) */
#ifndef SLY2_ENUM_ENSK
#define SLY2_ENUM_ENSK
enum ENSK { ENSK_Nil = -1, ENSK_Get = 0, ENSK_Set = 1 };
#endif

/* payload shapes of an EMITV slot (copied as structs) */
struct EMITV2 { float a[2]; };
struct EMITV3 { float a[3]; };
struct EMITV4 { float a[4]; };

/* a 0x40-byte variant record: a kind, and its payload at +0x30; the defaults it is reset from are whole records.
   +0x08..+0x14 hold OIDs that func_002586D8 resolves (record 1: +0x08, +0x0C, +0x10; record 0: +0x14);
   LoadEmitblipColorsFromBrx stores a byte it reads at +0x0C and its colour list at +0x1C. */
struct EMITBLIPCLRS { int c; RGBA argba[1]; };   /* count, then c colours (0x10-aligned allocation) */
struct EMITV
{
    int k;                  /* +0x00 kind of the payload (-1 or 0: unset) */
    char pad04[4];
    int oid08;              /* +0x08 an OID (record 1) */
    int n0C;                /* +0x0C an OID (record 1); the blip colour byte (LoadEmitblipColorsFromBrx) */
    int oid10;              /* +0x10 an OID (record 1) */
    int oid14;              /* +0x14 an OID (record 0) */
    char pad18[4];
    EMITBLIPCLRS *pblipclrs; /* +0x1C */
    char pad20[0x10];
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
    LO **aploRef;           /* +0x08 the five objects the block's OIDs resolve to (func_002586D8, a 0x14-byte block;
                               [0] is the origin object). emitter.cpp's views: the EMITO pointer, `poidParent` */
    char pad0C[4];
    int kEmito;             /* +0x10 EMITO kind (1, 2, 3; 0 unset) */
    char pad14[0xC];
    int oidEmito;           /* +0x20 the origin object's OID */
    char pad24[0xC];
    int pvEmito;            /* +0x30 EMITO data (allocated per kind; for kind 2: -1, then an OID) */
    char pad34[0x8C];
    EMITV v;                /* +0xC0 */
};
typedef char assert_EMITB_v[SLY2_OFFSETOF(EMITB, v) == 0xC0 ? 1 : -1];

struct EMITTER : public XFM
{
    int fDirty;             /* +0xE0 set by an ensure for writing */
    EMITB *pemitb;          /* +0xE4 */
    char padE8[8];
    int k;                  /* +0xF0 emission kind (3: rate per second) */
    int c;                  /* +0xF4 */
    LM lmRate;              /* +0xF8 */
    char pad100[0x114 - 0x100];
    int fAutoPause;         /* +0x114 */
    char pad118[0x130 - 0x118];
    float tNext;            /* +0x130 */
    char pad134[0x148 - 0x134];
    float tUnpause;         /* +0x148 */
    RIPG *pripg;            /* +0x14C */
    BLIPG *pblipg;          /* +0x150 */
    char pad154[0x160 - 0x154];
    virtual void Init() __asm__("InitEmitter");   /* slot 7 */
    virtual CLASSINFO *Pclassinfo() __asm__("GetEmitterClassDesc");   /* slot 6 */
    virtual void HandleMessage(MSGID msgid, void *pv) __asm__("HandleEmitterMessage__FP7EMITTER5MSGIDPv");   /* slot 5 */
    virtual void LoadFromBrx(CBinaryInputStream *pbis) __asm__("LoadEmitterFromBrx__H1Z7EMITTER_PX01P18CBinaryInputStream_v");   /* slot 14 */
    virtual void VUnk14() __asm__("PostEmitterLoad__FP7EMITTER");   /* slot 15 */
    virtual void VUnk15() __asm__("UpdateEmitter__FP7EMITTERf");   /* slot 16 */
    virtual void Clone(LO *ploBase) __asm__("CloneEmitter__H1Z7EMITTER_PX01P2LO_v");   /* slot 41 */
};
typedef char assert_EMITTER_size[sizeof(EMITTER) == 0x160 ? 1 : -1];   /* class descriptor cb */

struct EXPL : public XFM
{
    virtual void VSlot51() __asm__("ExplodeExplExplso__FP4EXPLP6EXPLSO");   /* slot 51 (ExplodeExplso); key function (emitter.cpp) */
    virtual CLASSINFO *Pclassinfo() __asm__("GetExplClassDesc");   /* slot 6 */
};
typedef char assert_EXPL_size[sizeof(EXPL) == 0xE0 ? 1 : -1];   /* class descriptor cb */

struct EXPLO : public EXPL
{
    int fDirty;             /* +0xE0 */
    EMITB *pemitb;          /* +0xE4 */
    char padE8[0xF0 - 0xE8];
    virtual CLASSINFO *Pclassinfo() __asm__("GetExploClassDesc");   /* slot 6 */
    virtual void LoadFromBrx(CBinaryInputStream *pbis) __asm__("LoadEmitterFromBrx__H1Z5EXPLO_PX01P18CBinaryInputStream_v");   /* slot 14 */
    virtual void VUnk14() __asm__("func_002586D8__H1Z5EXPLO_PX01_v");   /* slot 15 */
    virtual void Clone(LO *ploBase) __asm__("CloneEmitter__H1Z5EXPLO_PX01P2LO_v");   /* slot 41 */
};
typedef char assert_EXPLO_size[sizeof(EXPLO) == 0xF0 ? 1 : -1];   /* class descriptor cb */

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
/* the empty reference block of EMITTER and of EXPLO (five null pointers each, 0x2CE4A0 / 0x2CE4B8 in the linkonce data
   area, emitted by emitter.cpp before its vtables): a static data member of a class template, one instance per class
   (DRAFT names; the original's spelling is not known) */
template <class T> struct EMITBREFS { static void *s_apvNull[5]; };

/* an allocation request for the level heap (TU-private copies of the same struct exist in several merged files) */
#ifndef SLY2_STRUCT_AREQ
#define SLY2_STRUCT_AREQ
struct AREQ { int cb; int cbAlign; };
#endif
/* allocate from the level heap (func_001AE2A0); a C++ name bound by label, so that the merged files' own extern "C"
   declarations of func_001AE2A0 (on their AREQ copies) do not conflict with it where classes.h is included */
void *PvAllocLevelHeap(AREQ *preq) __asm__("func_001AE2A0");
void *PvAllocLevelHeapUncleared(AREQ *preq) __asm__("func_001AE1D0");   /* allocate (uncleared); labelled as above */
void *PvAllocLevelHeapCopy(AREQ *preq, void *pv) __asm__("func_001AE238");   /* allocate a copy of pv */
extern "C" void InitEmitBlock(EMITB *pemitb); /* InitEmitb: zero the block, install the EMITO/EMITV defaults */
extern "C" void ColorCorrectRgba(RGBA *prgba);
/* CBinaryInputStream's readers by their symbols (the class is only forward-declared in the sly2 headers: bis.h's
   definition clashes with basic.h's VECTOR/MATRIX types); C++ names bound by label, as PvAllocLevelHeap */
int BisU8Read(CBinaryInputStream *pbis) __asm__("U8Read__18CBinaryInputStream");
int BisU16Read(CBinaryInputStream *pbis) __asm__("U16Read__18CBinaryInputStream");
int BisU32Read(CBinaryInputStream *pbis) __asm__("U32Read__18CBinaryInputStream");
extern "C" void *memset(void *pv, int b, unsigned int cb);
extern "C" int memcmp(const void *pv1, const void *pv2, unsigned int cb);
struct EMITMESH;
void LoadEmitmeshFromBrx(EMITMESH *pemitmesh, CBinaryInputStream *pbis);
/* PloFindSwObject (sw.cpp) under a label: files that include this header declare its mangled name as a C function */
LO *PloFindSwObjectE(SW *psw, int grffso, OID oid, LO *ploContext) __asm__("PloFindSwObject__FP2SWi3OIDP2LO");
extern SW g_sw __asm__("D_003DD490");
extern "C" char base_D_002BAAE8[] __asm__("D_002BAAE8");   /* the class descriptor table (0x14-byte rows) */

/* ---- the class members shared by EMITTER and EXPLO (linkonce copies at 0x2584A8..0x258F30 and 0x258F40..0x2599C8).
   They are function templates for the same reason as the accessors below: one body per pair, emitted per instance
   in .gnu.linkonce.t.<symbol>, never inlined into callers. emitter.cpp instantiates both sets explicitly. Each is
   named after its EMITTER copy (the EXPLO copy is the other instance); the virtual slots of EMITTER/EXPLO name the
   instances (LoadFromBrx, Clone, VUnk14, and the second vtable's Pclassinfo). ---- */

/* the class descriptor of each (rows 29 and 32), for GetEmitterClassDesc_linkonce */
inline CLASSINFO *PclassinfoEmitb(EMITTER *) { return (CLASSINFO *)(base_D_002BAAE8 + 0x244); }
inline CLASSINFO *PclassinfoEmitb(EXPLO *) { return (CLASSINFO *)(base_D_002BAAE8 + 0x280); }
/* the empty reference block of each, for func_002586D8 */
inline void *PvEmitbRefsNull(EMITTER *) { return EMITBREFS<EMITTER>::s_apvNull; }
inline void *PvEmitbRefsNull(EXPLO *) { return EMITBREFS<EXPLO>::s_apvNull; }

/* copy on write: before a write (ENSK_Set), mark the object dirty and give it its own copy of a shared block */
template <class T> EMITB *PemitbEnsure(T *p, ENSK ensk)
{
    if (ensk == ENSK_Set)
    {
        p->fDirty = ensk;
        if (p->pemitb->cref >= 2)
        {
            AREQ req;
            p->pemitb->cref--;
            req.cb = 0x100;
            req.cbAlign = 0x10;
            p->pemitb = (EMITB *)PvAllocLevelHeapCopy(&req, p->pemitb);
            p->pemitb->cref = ensk;
        }
    }
    return p->pemitb;
}

/* the blip colours: a u16 count, a byte, then count RGBA words (colour corrected; at most 32 kept) */
/** @sly2-annot addr=0x2584A8
 * Purpose: read count + RGBA list into EMITB [game linkonce (inline/template)]
 * Info: proposed name LoadEmitblipColorsFromBrx (T1); TU game_linkonce
 * Notes: level_parser CONFIRMED-parse: payload layout followed by the parser, 100%% of bytes in 4 levels
 * Confidence: mid (draft) [T1 0.75] - evidence: static analysis; sources: sdk_names+parse-verified
 * See: docs/engine/core-library.md#7-key-functions,
 *   docs/reference/functions/core-library.md#game_linkonce-0x254f00-0x259de4
 * @end-sly2-annot */
/** @sly2-annot addr=0x258F40
 * Purpose: EXPLO twin of 0x2584A8 [game linkonce (inline/template)]
 * Info: proposed name LoadExploBlipColorsFromBrx (T1); TU game_linkonce
 * Notes: level_parser CONFIRMED-parse: payload layout followed by the parser, 100%% of bytes in 4 levels
 * Confidence: mid (draft) [T1 0.75] - evidence: static analysis; sources: sdk_names+parse-verified
 * See: docs/engine/core-library.md#7-key-functions,
 *   docs/reference/functions/core-library.md#game_linkonce-0x254f00-0x259de4
 * @end-sly2-annot */
template <class T> void LoadEmitblipColorsFromBrx(T *p, CBinaryInputStream *pbis)
{
    struct RGBAW { unsigned char r, g, b, a; };   /* basic.h's RGBA cannot be a union member (operator=) */
    int n = BisU16Read(pbis);
    int i;
    EMITV *pv;
    EMITBLIPCLRS *pclrs;
    union { AREQ req; RGBAW rgba; } u;   /* one stack slot for the request and the colour */
    if (n == 0)
        return;
    pv = &p->pemitb->v;
    u.req.cb = n * 4 + 4;
    u.req.cbAlign = 0x10;
    pclrs = (EMITBLIPCLRS *)PvAllocLevelHeapUncleared(&u.req);
    pv->pblipclrs = pclrs;
    pv->n0C = BisU8Read(pbis);
    pclrs->c = NMin(n, 32);
    for (i = 0; i < n; i++)
    {
        *(int *)&u.rgba = BisU32Read(pbis);
        ColorCorrectRgba((RGBA *)&u.rgba);
        if (i < pclrs->c)
            *(RGBAW *)&pclrs->argba[i] = u.rgba;
    }
}

/* LoadFromBrx (slot 14): a fresh block, the XFM part, the emit mesh (EMITO kind 3), the blip colours */
template <class T> void LoadEmitterFromBrx(T *p, CBinaryInputStream *pbis)
{
    AREQ req;
    req.cb = 0x100;
    req.cbAlign = 0x10;
    InitEmitBlock(p->pemitb = (EMITB *)PvAllocLevelHeapUncleared(&req));
    p->XFM::LoadFromBrx(pbis);
    if (p->pemitb->kEmito == 3)
        LoadEmitmeshFromBrx((EMITMESH *)p->pemitb->pvEmito, pbis);
    LoadEmitblipColorsFromBrx(p, pbis);
}

/* Clone (slot 41): the XFM part, then one more share of the block */
/** @sly2-annot addr=0x258620
 * Purpose: EMITTER vt41: CloneXfm then share EMITB (cref++) [game linkonce (inline/template)]
 * Info: proposed name CloneEmitter (T1); class EMITTER slot41; TU game_linkonce
 * Confidence: mid (draft) [T1 0.55] - evidence: static analysis; sources: sdk_names
 * See: docs/reference/functions/core-library.md#game_linkonce-0x254f00-0x259de4
 * @end-sly2-annot */
/** @sly2-annot addr=0x2590B8
 * Purpose: EXPLO/EXPLS vt41 [game linkonce (inline/template)]
 * Info: proposed name CloneExplo (T1); class EXPLO slot41; TU game_linkonce
 * Confidence: mid (draft) [T1 0.55] - evidence: static analysis; sources: sdk_names
 * See: docs/reference/functions/core-library.md#game_linkonce-0x254f00-0x259de4
 * @end-sly2-annot */
template <class T> void CloneEmitter(T *p, LO *ploBase)
{
    p->XFM::Clone(ploBase);
    p->pemitb->cref++;
}

/** @sly2-annot addr=0x2586D8
 * Purpose: resolve emitter object references (PloFindSwObject) after load [game linkonce (inline/template)]
 * Info: proposed name BindEmitter (T2); TU game_linkonce
 * Confidence: low (draft) [T2 0.40] - evidence: static analysis; sources: sdk_names
 * See: docs/reference/functions/core-library.md#game_linkonce-0x254f00-0x259de4
 * @end-sly2-annot */
/** @sly2-annot addr=0x259170
 * Purpose: EXPLO vt15 PostLoad: resolve object refs [game linkonce (inline/template)]
 * Info: proposed name BindExplo (T2); class EXPLO slot15; TU game_linkonce
 * Confidence: low (draft) [T2 0.40] - evidence: static analysis; sources: sdk_names
 * See: docs/reference/functions/core-library.md#game_linkonce-0x254f00-0x259de4
 * @end-sly2-annot */
/* PostLoad (slot 15 of EXPLO; EMITTER's PostEmitterLoad calls it): resolve the block's OIDs to objects, and give the
   block its own reference record when they differ from the shared one */
template <class T> void func_002586D8(T *p)
{
    LO *aplo[5];
    AREQ req;
    AREQ *preq;
    EMITB *pemitb;
    int oid1;
    int oid2 = -1;

    p->LO::VUnk14();
    {
        EMITB *pemitb0 = p->pemitb;
        oid1 = pemitb0->oidEmito;
        if (pemitb0->kEmito == 2)
            oid2 = pemitb0->pvEmito;
    }
    memset(aplo, 0, 0x14);
    if (oid1 != -1)
        aplo[0] = PloFindSwObjectE(&g_sw, 4, (OID)oid1, p);
    if (oid2 != -1)
    {
        LO *plo = PloFindSwObjectE(&g_sw, 4, (OID)oid2, p);
        aplo[0] = 0;
        aplo[1] = plo;
    }
    pemitb = p->pemitb;
    switch (pemitb->kv)
    {
    case 1:
    {
        int oid08 = pemitb->v.oid08;
        int oid0C = pemitb->v.n0C;
        int oid10 = pemitb->v.oid10;
        if (oid08 != -1)
            aplo[2] = PloFindSwObjectE(&g_sw, 4, (OID)oid08, p);
        if (oid0C != -1)
            aplo[3] = PloFindSwObjectE(&g_sw, 4, (OID)oid0C, p);
        if (oid10 != -1)
            aplo[4] = PloFindSwObjectE(&g_sw, 4, (OID)oid10, p);
        break;
    }
    case 0:
        if (pemitb->v.oid14 != -1)
            aplo[2] = PloFindSwObjectE(&g_sw, 4, (OID)pemitb->v.oid14, p);
        break;
    }
    if (p->pemitb->aploRef == 0)
        p->pemitb->aploRef = (LO **)PvEmitbRefsNull(p);
    if (memcmp(p->pemitb->aploRef, aplo, 0x14))
    {
        PemitbEnsure(p, ENSK_Set);
        preq = &req;
        req.cb = 0x14;
        preq->cbAlign = 0x10;
        p->pemitb->aploRef = (LO **)PvAllocLevelHeapCopy(preq, aplo);
    }
}

/** @sly2-annot addr=0x258F30
 * Purpose: duplicate EMITTER class-descriptor getter (vtable slot 6 of the second, never-stored EMITTER vtable):
 *   returns the same record as GetEmitterClassDesc (0x113BE0)
 * Info: was proposed as EMITTER_PvOptionDefaults (wrong); _linkonce suffix keeps the symbol distinct from
 *   GetEmitterClassDesc; TU game_linkonce
 * Confidence: high - evidence: static analysis (same data record as GetEmitterClassDesc); sources:
 *   class_reference hidden-types
 * See: docs/reference/functions/core-library.md#game_linkonce-0x254f00-0x259de4
 * @end-sly2-annot */
/** @sly2-annot addr=0x2599C8
 * Purpose: duplicate EXPLO class-descriptor getter (vtable slot 6 of the second, never-stored EXPLO vtable):
 *   returns the same record as GetExploClassDesc (0x113C10)
 * Info: was proposed as EXPLO_PvOptionDefaults (wrong); _linkonce suffix keeps the symbol distinct from
 *   GetExploClassDesc; TU game_linkonce
 * Confidence: high - evidence: static analysis (same data record as GetExploClassDesc); sources:
 *   class_reference hidden-types
 * See: docs/reference/functions/core-library.md#game_linkonce-0x254f00-0x259de4
 * @end-sly2-annot */
/* Pclassinfo of the second (never stored) vtable of each class: the class descriptor */
template <class T> CLASSINFO *GetEmitterClassDesc_linkonce(T *p) { return PclassinfoEmitb(p); }

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
