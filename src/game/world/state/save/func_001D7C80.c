struct B { int c; unsigned char *pb; };
struct Q { char pad[0x5400]; int n5400; int n5404; };
void *func_002333B4(void *p, int n, int cb);
static inline int Sum(unsigned char *pb, unsigned int n) { int sum = 0; while (n) { sum += *pb++; n--; } return sum; }
/** @sly2-annot addr=0x1D7C80
 * Purpose: Byte-sum the schema trie and read U16 version at end; if differs from GS+0x5400/+0x5404 memset GS
 *   0x5400 bytes and store new version/sum
 * Info: proposed name ResetGsIfSchemaChanged (T1); TU save
 * Confidence: mid (draft) [T1 0.70] - evidence: static analysis; sources: r6_1D5150
 * See: docs/engine/save-memcard.md#1-the-save-blob-g_gs-savecpp-0x1d7b88-0x1d93d8,
 *   docs/engine/save-memcard.md#5-key-functions, docs/engine/save-memcard.md#open-questions
 * @end-sly2-annot */
void func_001D7C80(struct B *p, struct Q *q) {
    int sum = Sum(p->pb, p->c * 10);
    int n = *(unsigned short *)(p->pb + p->c * 10 - 6);
    if (n == q->n5400 && sum == q->n5404) return;
    func_002333B4(q, 0, 0x5400);
    q->n5400 = n;
    q->n5404 = sum;
}
