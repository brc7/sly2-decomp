struct MSG { int cb; int nKind; };
struct I { int a, b; };
struct E { int id; int n; int c; struct I *a; };
struct S { char pad[0x15C]; int i; int c; struct E *ae; };
void *func_001AE318(struct MSG *pmsg);
static __inline__ void *PvAllocMsg(struct MSG *pmsg, int cb, int nKind) { pmsg->cb = cb; pmsg->nKind = nKind; return func_001AE318(pmsg); }
/** @sly2-annot addr=0x1BB950
 * Purpose: Append stage id with action buffer (max 32)
 * Info: proposed name AddCid98Stage (T2); class CID98; TU tu_1BAB50
 * Confidence: low (draft) [T2 0.35] - evidence: static analysis; sources: r5b_rest
 * See: docs/reference/functions/gameplay-objects.md#tu_1bab50-0x1bab50-0x1bc050
 * @end-sly2-annot */
void func_001BB950(struct S *p, int id)
{
    struct MSG msg;
    if (p->c < 0x20) {
        struct E *pe;
        p->i = p->c++;
        pe = &p->ae[p->i];
        pe->id = id;
        pe->n = 1;
        pe->a = PvAllocMsg(&msg, 0x200, 0x10);
    }
}
