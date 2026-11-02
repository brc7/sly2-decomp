struct CRef { int tagk; int tag; };
struct CMD { char pad[0x10]; unsigned int c; int a[4]; };
void func_002013D8(struct CRef *pref, void *x, int a, int b, unsigned int *pc, int *pa);
struct CMD *PrelaycmdAddRelay(void *pq, int kind, int b);
void CopyAb__FPvT0Ui(void *pdst, void *psrc, int cb);
void Decref__4CRef(struct CRef *pref);
/** @sly2-annot addr=0x1E0310
 * Purpose: Splice builtin: append a RELAYCK 0x3 (WaitBlocks) command to the relay
 *   (PrelaycmdAddRelay(prelay,0x3,fAsync)) and fill its args
 * Info: proposed name AddRelayWaitBlocks (T1); TU relay
 * Confidence: mid (draft) [T1 0.60] - evidence: static analysis; sources: r6b_rest
 * See: docs/reference/functions/cutscenes.md#relay-0x1dfad8-0x1e5d80
 * @end-sly2-annot */
void AddRelayWaitBlocks(void *pq, void *x)
{
    int a[8];
    struct CRef ref;
    unsigned int c;
    struct CMD *p;
    func_002013D8(&ref, x, 6, 8, &c, a);
    p = PrelaycmdAddRelay(pq, 3, 0);
    if (c > 4) c = 4;
    {
        unsigned int n = c;
        p->c = n;
        CopyAb__FPvT0Ui(&p->a, a, n * 4);
    }
    Decref__4CRef(&ref);
}
