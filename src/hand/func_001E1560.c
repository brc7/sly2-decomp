struct CMD { int kind; int b; int n8; int nC; int n10; int n14; int n18; int n1C; };
struct CMDF { int kind; int b; int n8; int nC; float g10; float g14; int n18; int n1C; };
struct CMD *func_001E01F8(void *pq, int kind, int b);
/** @sly2-annot addr=0x1E1560
 * Purpose: Splice builtin: append a RELAYCK 0x42 (ButtonPrompt) command to the relay
 *   (PrelaycmdAddRelay(prelay,0x42,fAsync)) and fill its args
 * Info: proposed name AddRelayButtonPrompt (T1); TU relay
 * Confidence: mid (draft) [T1 0.60] - evidence: static analysis; sources: r6b_rest
 * See: docs/reference/functions/cutscenes.md#relay-0x1dfad8-0x1e5d80
 * @end-sly2-annot */
void func_001E1560(void *pq, int a, int b)
{
    struct CMD *p = func_001E01F8(pq, 0x42, 0);
    if (a == -1) a = 0x40;
    p->n10 = a; p->n14 = b;
}
