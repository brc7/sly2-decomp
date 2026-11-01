struct E { unsigned int h; char *s; void *pv; struct E *next; };
struct S { struct E *ap[0x35]; int c; struct E *ae; };
unsigned int func_001D8640(char *s);
/** @sly2-annot addr=0x1D8690
 * Purpose: Insert (name,id) into 53-bucket chained hash table, taking entries from the end of the
 *   preallocated array
 * Info: proposed name AddSaveSymbol (T1); TU save
 * Confidence: mid (draft) [T1 0.70] - evidence: static analysis; sources: r6_1D5150
 * See: docs/engine/save-memcard.md#5-key-functions,
 *   docs/reference/functions/save-memcard.md#save-0x1d7b88-0x1d93d8
 * @end-sly2-annot */
void func_001D8690(struct S *p, char *s, void *pv) {
    unsigned int h = func_001D8640(s);
    unsigned int ih = h % 0x35;
    struct E *pe = &p->ae[p->c - 1];
    p->c--;
    pe->s = s;
    pe->pv = pv;
    pe->h = h;
    pe->next = p->ap[ih];
    p->ap[ih] = pe;
}
