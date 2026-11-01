struct ALLOCREQ { int cb; int nKind; };
class FRAME {
public:
    int a[4];
    FRAME();
    ~FRAME();
};
class CBinaryInputStream {
public:
    int U8Read();
    int S32Read();
};
struct S { char pad[0x90]; int *a90; int c94; void *a98; int *a9C; int cA0; void *aA4; int cA8; void *aAC; int cB0; void *aB4; };
extern "C" void *func_001AE318(ALLOCREQ *preq);
extern "C" void *func_001AE238(ALLOCREQ *preq, void *pv);
extern "C" void *func_001AE2A0(ALLOCREQ *preq);
extern "C" void *func_001AE1D0(ALLOCREQ *preq);
extern "C" void func_001A5C28(S *p, CBinaryInputStream *pbis);
extern "C" void func_00118420(CBinaryInputStream *pbis);
static inline void *PvAllocReq(ALLOCREQ *preq, int cb, int nKind)
{
    preq->cb = cb;
    preq->nKind = nKind;
    return func_001AE318(preq);
}
static inline void *PvShrinkReq(ALLOCREQ *preq, int cb, int nKind, void *pv)
{
    preq->cb = cb;
    preq->nKind = nKind;
    return func_001AE238(preq, pv);
}
static inline void *PvAllocA(ALLOCREQ *preq, int cb, int nKind)
{
    preq->cb = cb;
    preq->nKind = nKind;
    return func_001AE2A0(preq);
}
static inline void *PvAllocB(ALLOCREQ *preq, int cb, int nKind)
{
    preq->cb = cb;
    preq->nKind = nKind;
    return func_001AE1D0(preq);
}
/** @sly2-annot addr=0x1635F8
 * Purpose: TASK LoadFromBrx: visibility/RSMG arrays, then u8-counted oid lists of prerequisites (+0x90) and
 *   successors (+0x9C)
 * Info: proposed name LoadTaskFromBrx (T1); class TASK slot14; TU game
 * Notes: superseded r3_1539A8 proposal: LoadCid85FromBrx (0.7) | level_parser CONFIRMED-parse: payload layout
 *   followed by the parser, 100%% of bytes in 4 levels
 * Confidence: mid (draft) [T1 0.75] - evidence: static analysis; sources: r3b_rest+parse-verified
 * See: docs/engine/game-progression.md#8-key-functions,
 *   docs/formats/level-objects.md#5-per-class-payloads-vtable-slot-14,
 *   docs/reference/functions/game-progression.md#game-0x1613a8-0x166270
 * @end-sly2-annot */
extern "C" void func_001635F8(S *p, CBinaryInputStream *pbis)
{
    int i = 0;
    FRAME fr;
    ALLOCREQ req;
    p->aAC = PvAllocReq(&req, 0x200, 0x10);
    p->aB4 = PvAllocReq(&req, 0x300, 0x10);
    func_001A5C28(p, pbis);
    p->aAC = PvShrinkReq(&req, p->cA8 * 16, 0x10, p->aAC);
    p->aB4 = PvShrinkReq(&req, p->cB0 * 0x18, 0x10, p->aB4);
    p->c94 = pbis->U8Read();
    p->a98 = PvAllocA(&req, p->c94 * 4, 0x10);
    p->a90 = (int *)PvAllocB(&req, p->c94 * 4, 0x10);
    for (; i < p->c94; i++)
        p->a90[i] = pbis->S32Read();
    p->cA0 = pbis->U8Read();
    p->aA4 = PvAllocA(&req, p->cA0 * 4, 0x10);
    p->a9C = (int *)PvAllocB(&req, p->cA0 * 4, 0x10);
    for (int j = 0; j < p->cA0; j++)
        p->a9C[j] = pbis->S32Read();
    func_00118420(pbis);
}
