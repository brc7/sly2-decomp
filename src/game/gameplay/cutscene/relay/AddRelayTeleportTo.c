struct CMD { int kind; int b; int n8; int nC; int n10; int n14; int n18; int n1C; };
struct CMDF { int kind; int b; int n8; int nC; float g10; float g14; int n18; int n1C; };
struct CMD *PrelaycmdAddRelay(void *pq, int kind, int b);
/** @sly2-annot addr=0x1E07D0
 * Purpose: Splice builtin: append a RELAYCK 0x16 (TeleportTo) command to the relay
 *   (PrelaycmdAddRelay(prelay,0x16,fAsync)) and fill its args
 * Info: proposed name AddRelayTeleportTo (T1); TU relay
 * Notes: r0 hint: optid = CUT (Sly 1 AddJsgCut)
 * Confidence: mid (draft) [T1 0.60] - evidence: Sly 1 / Lilly reference; sources: r6b_rest
 * See: docs/reference/functions/cutscenes.md#relay-0x1dfad8-0x1e5d80
 * @end-sly2-annot */
void AddRelayTeleportTo(void *pq, int a, int b, int c)
{
    struct CMD *p = PrelaycmdAddRelay(pq, 0x16, 1);
    p->n10 = a; p->n14 = b; p->n18 = c;
}
