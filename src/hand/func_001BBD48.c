struct MSG { int cb; int nKind; };
struct I { int a, b; };
struct S;
struct I *func_001BBDB0(struct S *p, int x);
void *func_001AE318(struct MSG *pmsg);
static __inline__ void *PvAllocMsg(struct MSG *pmsg, int cb, int nKind) { pmsg->cb = cb; pmsg->nKind = nKind; return func_001AE318(pmsg); }
/** @sly2-annot addr=0x1BBD48
 * Purpose: Record action 12 with a two-int payload
 * Info: proposed name AddCid98Action12 (T2); class CID98; TU tu_1BAB50
 * Confidence: low (draft) [T2 0.30] - evidence: static analysis; sources: r5b_rest
 * See: docs/reference/functions/gameplay-objects.md#tu_1bab50-0x1bab50-0x1bc050
 * @end-sly2-annot */
void func_001BBD48(struct S *p, int a, int b)
{
    struct MSG msg;
    struct I *pi = func_001BBDB0(p, 0xC);
    pi->b = (int)PvAllocMsg(&msg, 8, 0x10);
    ((int *)pi->b)[0] = a;
    ((int *)pi->b)[1] = b;
}
