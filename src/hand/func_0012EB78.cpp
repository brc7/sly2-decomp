struct AREQ { int cb; int cbAlign; };
struct OBJ { int a[4]; OBJ(); ~OBJ(); };
struct T { char pad[0xE4]; int c; void *p; int pad2; int c2; void *q; };
extern "C" {
void *func_001AE378(AREQ *preq);
void *func_001AE238(AREQ *preq, void *pv);
void func_00231170(T *t, void *p);
void func_0012EB78(T *t, void *p);
}
/** @sly2-annot addr=0x12EB78
 * Purpose: slot 14: LoadXfmFromBrx (0x231170) then reads/allocates its sound list
 * Info: proposed name LoadCid43FromBrx (T2); class CID43 slot14; TU amb
 * Notes: level_parser CONFIRMED-parse: payload layout followed by the parser, 100%% of bytes in 4 levels
 * Confidence: low (draft) [T2 0.75] - evidence: static analysis; sources: r1_116918+parse-verified
 * See: docs/engine/animation.md#7-key-functions, docs/reference/functions/animation.md#amb-0x12eb48-0x12f0f0
 * @end-sly2-annot */
void func_0012EB78(T *t, void *p)
{
    OBJ obj;
    AREQ req;
    AREQ *preq = &req;
    req.cb = 0xA0;
    preq->cbAlign = 0x10;
    t->p = func_001AE378(preq);
    req.cb = 0x80;
    preq->cbAlign = 0x10;
    t->q = func_001AE378(preq);
    func_00231170(t, p);
    req.cb = t->c * 0x14;
    preq->cbAlign = 0x10;
    t->p = func_001AE238(preq, t->p);
    req.cb = t->c2 * 0x10;
    preq->cbAlign = 0x10;
    t->q = func_001AE238(preq, t->q);
}
