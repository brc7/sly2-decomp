union U { int n; float g; };
struct CMD { int kind; int b; int n8; int nC; union U a[6]; };
struct CMD *func_001E01F8(void *pq, int kind, int b);
/** @sly2-annot addr=0x1E1870
 * Purpose: Splice builtin: append a RELAYCK 0x4F (Unk4F) command to the relay
 *   (PrelaycmdAddRelay(prelay,0x4F,fAsync)) and fill its args
 * Info: proposed name AddRelayUnk4F (T2); TU relay
 * Notes: v2: demoted to T2 (placeholder token in name: Unk)
 * Confidence: low (draft) [T2 0.50] - evidence: static analysis; sources: r6b_rest
 * See: docs/reference/functions/cutscenes.md#relay-0x1dfad8-0x1e5d80
 * @end-sly2-annot */
void func_001E1870(void *pq, int a, int b, int c, int d, int e, int x)
{
    struct CMD *p = func_001E01F8(pq, 0x4F, x);
    p->a[1].n = a;
    p->a[3].n = b;
    p->a[2].n = c;
    p->a[4].n = d;
    p->a[0].n = e;
}
