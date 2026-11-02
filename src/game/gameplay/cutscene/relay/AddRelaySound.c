union U { int n; float g; void *pv; };
struct CMD { int kind; int b; int n8; int nC; union U a[7]; };
struct CMD *PrelaycmdAddRelay(void *pq, int kind, int b);
/** @sly2-annot addr=0x1E1178
 * Purpose: Splice builtin: append a RELAYCK 0x36 (Sound) command to the relay
 *   (PrelaycmdAddRelay(prelay,0x36,fAsync)) and fill its args
 * Info: proposed name AddRelaySound (T1); TU relay
 * Confidence: mid (draft) [T1 0.60] - evidence: static analysis; sources: r6b_rest
 * See: docs/reference/functions/cutscenes.md#relay-0x1dfad8-0x1e5d80
 * @end-sly2-annot */
void AddRelaySound(void *pq, int a, int b, int c, int d, float f, float g, float h)
{
    struct CMD *p = PrelaycmdAddRelay(pq, 0x36, 1);
    p->a[0].n = a; p->a[1].n = b; p->a[2].n = c; p->a[3].n = d;
    p->a[4].g = f; p->a[5].g = g; p->a[6].g = h;
}
