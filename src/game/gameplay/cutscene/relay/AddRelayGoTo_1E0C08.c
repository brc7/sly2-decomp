union U { int n; float g; };
struct CMD { int kind; int b; int n8; int nC; union U a[6]; };
struct CMD *PrelaycmdAddRelay(void *pq, int kind, int b);
/** @sly2-annot addr=0x1E0C08
 * Purpose: Splice builtin: append a RELAYCK 0x24 (GoTo) command to the relay
 *   (PrelaycmdAddRelay(prelay,0x24,fAsync)) and fill its args
 * Info: proposed name AddRelayGoTo_1E0C08 (T1); TU relay
 * Notes: agent qualifier: fUnk9=0 variant | COLLISION: AddRelayGoTo already used (same name proposed for
 *   0x1E0B90); renamed to AddRelayGoTo_1E0C08
 * Confidence: mid (draft) [T1 0.60] - evidence: static analysis; sources: r6b_rest
 * See: docs/reference/functions/cutscenes.md#relay-0x1dfad8-0x1e5d80
 * @end-sly2-annot */
void AddRelayGoTo_1E0C08(void *pq, int a, int x, int c, int d, float g, float h)
{
    struct CMD *p = PrelaycmdAddRelay(pq, 0x24, x);
    p->a[0].n = a; p->a[1].g = g; p->a[2].g = h; p->a[3].n = c; p->a[4].n = d; p->a[5].n = 0;
}
