typedef struct S { char pad[0x10]; int a; float f; int b; float g; int c; int d; } S;
S *func_001E01F8(void *p, int k, int n);
/** @sly2-annot addr=0x1E0D38
 * Purpose: Splice builtin: append a RELAYCK 0x28 (WalkTo) command to the relay
 *   (PrelaycmdAddRelay(prelay,0x28,fAsync)) and fill its args
 * Info: proposed name AddRelayWalkTo (T1); TU relay
 * Confidence: mid (draft) [T1 0.60] - evidence: static analysis; sources: r6b_rest
 * See: docs/reference/functions/cutscenes.md#relay-0x1dfad8-0x1e5d80
 * @end-sly2-annot */
void func_001E0D38(void *p, int a, float f, int b, float g, int c, int d)
{
    S *s = func_001E01F8(p, 0x28, 0);
    s->a = a;
    s->f = f;
    s->b = b;
    s->g = g;
    s->c = c;
    s->d = d;
}
