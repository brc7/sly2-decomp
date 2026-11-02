struct CMD { int kind; int b; int n8; int nC; int n10; int n14; int n18; int n1C; };
struct CMDF { int kind; int b; int n8; int nC; int n10; float g14; int n18; int n1C; };
struct CMD *PrelaycmdAddRelay(void *pq, int kind, int b);
/** @sly2-annot addr=0x1E08C0
 * Purpose: Splice builtin: append a RELAYCK 0x1A (PipeTo) command to the relay
 *   (PrelaycmdAddRelay(prelay,0x1A,fAsync)) and fill its args
 * Info: proposed name AddRelayPipeTo (T1); TU relay
 * Confidence: mid (draft) [T1 0.60] - evidence: static analysis; sources: r6b_rest
 * See: docs/reference/functions/cutscenes.md#relay-0x1dfad8-0x1e5d80
 * @end-sly2-annot */
void AddRelayPipeTo(void *pq, int a, int b, int c, float g)
{
    struct CMDF *p = (struct CMDF *)PrelaycmdAddRelay(pq, 0x1A, b);
    p->n10 = a; p->g14 = g; p->n18 = c;
}
