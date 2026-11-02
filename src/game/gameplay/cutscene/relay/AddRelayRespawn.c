union U { int n; float g; void *pv; };
struct CMD { int kind; int b; int n8; int nC; union U a[4]; };
struct CMD *PrelaycmdAddRelay(void *pq, int kind, int b);
/** @sly2-annot addr=0x1E0E18
 * Purpose: Splice builtin: append a RELAYCK 0x2A (Respawn) command to the relay
 *   (PrelaycmdAddRelay(prelay,0x2A,fAsync)) and fill its args
 * Info: proposed name AddRelayRespawn (T1); TU relay
 * Confidence: mid (draft) [T1 0.60] - evidence: static analysis; sources: r6b_rest
 * See: docs/reference/functions/cutscenes.md#relay-0x1dfad8-0x1e5d80
 * @end-sly2-annot */
void AddRelayRespawn(void *pq, int a, int b, int c, int x, int d)
{
    struct CMD *p = PrelaycmdAddRelay(pq, 0x2A, x);
    p->a[0].n = a; p->a[1].n = b; p->a[2].n = c; p->a[3].n = d;
}
