typedef struct S { char pad[0x10]; int a; float f; float g; int c; int d; int e; } S;
S *PrelaycmdAddRelay(void *p, int cb);
/** @sly2-annot addr=0x1E0B90
 * Purpose: Splice builtin: append a RELAYCK 0x24 (GoTo) command to the relay
 *   (PrelaycmdAddRelay(prelay,0x24,fAsync)) and fill its args
 * Info: proposed name AddRelayGoTo (T1); TU relay
 * Notes: agent qualifier: fUnk9=1 variant
 * Confidence: mid (draft) [T1 0.60] - evidence: static analysis; sources: r6b_rest
 * See: docs/reference/functions/cutscenes.md#relay-0x1dfad8-0x1e5d80
 * @end-sly2-annot */
void AddRelayGoTo(void *p, int a, int b, int c, int d, float f, float g)
{
    S *s = PrelaycmdAddRelay(p, 0x24);
    s->a = a;
    s->f = f;
    s->g = g;
    s->c = c;
    s->d = d;
    s->e = 1;
}
