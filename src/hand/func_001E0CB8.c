typedef struct S { char pad[0x10]; int a; int b; int c; float f; int d; int e; } S;
S *func_001E01F8(void *p, int k, int n);
/** @sly2-annot addr=0x1E0CB8
 * Purpose: Splice builtin: append a RELAYCK 0x27 (FollowPath) command to the relay
 *   (PrelaycmdAddRelay(prelay,0x27,fAsync)) and fill its args
 * Info: proposed name AddRelayFollowPath (T1); TU relay
 * Confidence: mid (draft) [T1 0.60] - evidence: static analysis; sources: r6b_rest
 * See: docs/reference/functions/cutscenes.md#relay-0x1dfad8-0x1e5d80
 * @end-sly2-annot */
void func_001E0CB8(void *p, int a, int b, int c, int d, int e, float f)
{
    S *s = func_001E01F8(p, 0x27, 0);
    s->a = a;
    s->b = b;
    s->c = c;
    s->f = f;
    s->d = d;
    s->e = e;
}
