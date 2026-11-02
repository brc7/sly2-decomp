struct E { unsigned int h; char *s; int n; struct E *next; };
struct S { struct E *ap[0x35]; int c; struct E *ae; };
unsigned int UHashPchzCi(char *s);
int func_002342D0(char *a, char *b);
/** @sly2-annot addr=0x1D8720
 * Purpose: Lookup name -> symbol id in 53-bucket hash (strcmp-ish 0x2342D0), -1 if absent
 * Info: proposed name NFindSaveSymbol (T1); TU save
 * Confidence: mid (draft) [T1 0.75] - evidence: static analysis; sources: r6_1D5150
 * See: docs/engine/save-memcard.md#5-key-functions,
 *   docs/reference/functions/save-memcard.md#save-0x1d7b88-0x1d93d8
 * @end-sly2-annot */
int NFindSaveSymbol(struct S *p, char *s)
{
    unsigned int h = UHashPchzCi(s);
    struct E *pe;
    for (pe = p->ap[h % 0x35]; pe; pe = pe->next) {
        if (pe->h == h && func_002342D0(pe->s, s) == 0)
            return pe->n;
    }
    return -1;
}
