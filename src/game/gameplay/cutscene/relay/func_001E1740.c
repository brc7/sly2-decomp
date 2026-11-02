union U { int n; float g; };
struct CMD { int kind; int b; int n8; int nC; union U a[4]; };
struct CMD *PrelaycmdAddRelay(void *pq, int kind, int b);
/** @sly2-annot addr=0x1E1740
 * Purpose: Splice builtin: append a RELAYCK 0x4A (Unk4A) command to the relay
 *   (PrelaycmdAddRelay(prelay,0x4A,fAsync)) and fill its args
 * Info: proposed name AddRelayUnk4A (T2); TU relay
 * Notes: v2: demoted to T2 (placeholder token in name: Unk)
 * Confidence: low (draft) [T2 0.50] - evidence: static analysis; sources: r6b_rest
 * See: docs/reference/functions/cutscenes.md#relay-0x1dfad8-0x1e5d80
 * @end-sly2-annot */
void func_001E1740(void *pq, int a, int b, int c, float g)
{
    struct CMD *p = PrelaycmdAddRelay(pq, 0x4A, 0);
    p->a[0].g = g; p->a[1].n = a; p->a[2].n = b; p->a[3].n = c;
}
