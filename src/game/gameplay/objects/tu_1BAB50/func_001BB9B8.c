struct MSG { int cb; int nKind; };
struct I { int a, b; };
struct E { int id; int n; int c; struct I *a; };
struct S { char pad[0x15C]; int i; int c; struct E *ae; };
void *func_001AE318(struct MSG *pmsg);
static __inline__ void *PvAllocMsg(struct MSG *pmsg, int cb, int nKind) { pmsg->cb = cb; pmsg->nKind = nKind; return func_001AE318(pmsg); }
/** @sly2-annot addr=0x1BB9B8
 * Purpose: Init stage -1 with action buffer
 * Info: proposed name InitCid98PreStage (T2); class CID98; TU tu_1BAB50
 * Confidence: low (draft) [T2 0.30] - evidence: static analysis; sources: r5b_rest
 * See: docs/reference/functions/gameplay-objects.md#tu_1bab50-0x1bab50-0x1bc050
 * @end-sly2-annot */
void func_001BB9B8(struct S *p)
{
    struct E *pe;
    struct MSG msg;
    p->i = -1;
    pe = &p->ae[-1];
    pe->id = -1;
    pe->n = 1;
    if (!pe->a) pe->a = PvAllocMsg(&msg, 0x200, 0x10);
}
