struct CMD { int kind; int b; int n8; int nC; int n10; int n14; int n18; int n1C; };
struct CMDF { int kind; int b; int n8; int nC; int n10; int n14; float g18; int n1C; };
struct CMD *func_001E01F8(void *pq, int kind, int b);
/** @sly2-annot addr=0x1E1670
 * Purpose: Splice builtin: append a RELAYCK 0x47 (Unk47) command to the relay
 *   (PrelaycmdAddRelay(prelay,0x47,fAsync)) and fill its args
 * Info: proposed name AddRelayUnk47 (T2); TU relay
 * Notes: v2: demoted to T2 (placeholder token in name: Unk)
 * Confidence: low (draft) [T2 0.50] - evidence: static analysis; sources: r6b_rest
 * See: docs/reference/functions/cutscenes.md#relay-0x1dfad8-0x1e5d80
 * @end-sly2-annot */
void func_001E1670(void *pq, int a, int b, int c)
{
    struct CMD *p = func_001E01F8(pq, 0x47, 1);
    p->n10 = a; p->n14 = b; p->n18 = c;
}
