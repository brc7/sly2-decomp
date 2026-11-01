struct E { int id; int cref; struct E *pnext; };
struct S { int pad[2]; struct E *pfirst; };
extern char D_003E1144[];
void func_0011CD98(void *, void *);
/** @sly2-annot addr=0x1334A8
 * Purpose: Free the whole subscriber list
 * Info: proposed name UnsubscribeLoAll (T1); TU basic
 * Confidence: mid (draft) [T1 0.55] - evidence: static analysis; sources: r2b_rest
 * See: docs/reference/functions/object-model.md#basic-0x132b00-0x1336f0
 * @end-sly2-annot */
void UnsubscribeLoAll(struct S *p) {
    struct E *pe = p->pfirst;
    while (pe) { struct E *pNext = pe->pnext; func_0011CD98(D_003E1144, pe); pe = pNext; }
    p->pfirst = 0;
}
