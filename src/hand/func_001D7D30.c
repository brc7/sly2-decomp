struct B { int c; unsigned char *pb; };
struct Q { char pad[0x5400]; int n5400; int n5404; };
static inline int Sum(unsigned char *pb, unsigned int n) { int sum = 0; while (n) { sum += *pb++; n--; } return sum; }
/** @sly2-annot addr=0x1D7D30
 * Purpose: True if GS stored version/checksum (+0x5400/+0x5404) equal schema's
 * Info: proposed name FGsMatchesSchema (T1); TU save
 * Confidence: mid (draft) [T1 0.75] - evidence: static analysis; sources: r6_1D5150
 * See: docs/engine/save-memcard.md#5-key-functions,
 *   docs/reference/functions/save-memcard.md#save-0x1d7b88-0x1d93d8
 * @end-sly2-annot */
int func_001D7D30(struct B *p, struct Q *q) {
    int sum = Sum(p->pb, p->c * 10);
    return *(unsigned short *)(p->pb + p->c * 10 - 6) == q->n5400 && sum == q->n5404;
}
