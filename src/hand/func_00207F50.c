struct E { int a; int b; int c; int d; };
void ReleaseAnimBinding(struct E *pe, void *p);
void ResolveAnimBinding(struct E *pe, void *p);
/** @sly2-annot addr=0x207F50
 * Purpose: Same and resolve each
 * Info: proposed name SetZapReactionBindingsResolve (T2); TU stepguard
 * Confidence: low (draft) [T2 0.40] - evidence: static analysis; sources: r7b_guard
 * See: docs/reference/functions/guards-ai.md#stepguard-0x205e70-0x20b840
 * @end-sly2-annot */
void func_00207F50(void *p, int mask, int b, int d, struct E *pe)
{
    int i;
    int bit = 1;
    for (i = 25; i >= 0; i--) {
        if (mask & bit) {
            ReleaseAnimBinding(pe, p);
            pe->b = b;
            pe->d = d;
            ResolveAnimBinding(pe, p);
        }
        bit <<= 1;
        pe++;
    }
}
