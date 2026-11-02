struct MSG { int cb; int nKind; };
typedef struct { int c; short *a; } SA;
void *func_001AE318(struct MSG *pmsg);
void CopyAb__FPvT0Ui(void *, void *, int);
/** @sly2-annot addr=0x1D8CC8
 * Purpose: Allocate a new key = concatenation of two keys' id arrays
 * Info: proposed name PkeyConcatSaveKeys (T1); TU save
 * Confidence: mid (draft) [T1 0.65] - evidence: static analysis; sources: r6_1D5150
 * See: docs/reference/functions/save-memcard.md#save-0x1d7b88-0x1d93d8
 * @end-sly2-annot */
SA *PkeyConcatSaveKeys(SA *p1, SA *p2)
{
    struct MSG msg;
    SA *r;
    int c;
    msg.cb = 8;
    msg.nKind = 0x10;
    r = (SA *)func_001AE318(&msg);
    c = p1->c + p2->c;
    r->c = c;
    msg.cb = c * 2;
    msg.nKind = 0x10;
    r->a = (short *)func_001AE318(&msg);
    CopyAb__FPvT0Ui(r->a, p1->a, p1->c * 2);
    CopyAb__FPvT0Ui(r->a + p1->c, p2->a, p2->c * 2);
    return r;
}
