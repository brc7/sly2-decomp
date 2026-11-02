struct CMD { int kind; int b; int n8; int nC; int n10; int n14; int n18; int n1C; };
struct CMDF { int kind; int b; int n8; int nC; int n10; int n14; float g18; int n1C; };
struct CMD *func_001E01F8(void *pq, int kind, int b);
/** @sly2-annot addr=0x1E1128
 * Purpose: Splice builtin: append a RELAYCK 0x35 (Eyes) command to the relay
 *   (PrelaycmdAddRelay(prelay,0x35,fAsync)) and fill its args
 * Info: proposed name AddRelayEyes (T1); TU relay
 * Confidence: mid (draft) [T1 0.60] - evidence: static analysis; sources: r6b_rest
 * See: docs/reference/functions/cutscenes.md#relay-0x1dfad8-0x1e5d80
 * @end-sly2-annot */
void func_001E1128(void *pq, int a, int b, float g)
{
    struct CMDF *p = (struct CMDF *)func_001E01F8(pq, 0x35, 1);
    p->n10 = a; p->n14 = b; p->g18 = g;
}
