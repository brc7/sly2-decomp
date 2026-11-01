struct E { int a; int b; int c; int d; };
void func_002090D0(struct E *pe, void *p);
/** @sly2-annot addr=0x207EB8
 * Purpose: For each of 26 zap-kind bits in mask: release entry, set oid/arg
 * Info: proposed name SetZapReactionBindings (T2); TU stepguard
 * Confidence: low (draft) [T2 0.40] - evidence: static analysis; sources: r7b_guard
 * See: docs/reference/functions/guards-ai.md#stepguard-0x205e70-0x20b840
 * @end-sly2-annot */
void func_00207EB8(void *p, int mask, int b, int d, struct E *pe)
{
    int i;
    int bit = 1;
    for (i = 25; i >= 0; i--) {
        if (mask & bit) {
            func_002090D0(pe, p);
            pe->b = b;
            pe->d = d;
        }
        bit <<= 1;
        pe++;
    }
}
