union U { int n; float g; void *pv; };
struct CMD { int kind; int b; int n8; int nC; union U a[4]; };
struct CMD *func_001E01F8(void *pq, int kind, int b);
/** @sly2-annot addr=0x1E0AE0
 * Purpose: Splice builtin: append a RELAYCK 0x22 (StepCmd22) command to the relay
 *   (PrelaycmdAddRelay(prelay,0x22,fAsync)) and fill its args
 * Info: proposed name AddRelayStepCmd22 (T1); TU relay
 * Confidence: mid (draft) [T1 0.50] - evidence: static analysis; sources: r6b_rest
 * See: docs/reference/functions/cutscenes.md#relay-0x1dfad8-0x1e5d80
 * @end-sly2-annot */
void func_001E0AE0(void *pq, int a, int b, int c, int d)
{
    struct CMD *p = func_001E01F8(pq, 0x22, 0);
    p->a[0].n = a; p->a[1].n = b; p->a[2].n = c; p->a[3].n = d;
}
