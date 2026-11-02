struct CMD { int kind; int b; int n8; int nC; int n10; int n14; int n18; int n1C; };
struct CMDF { int kind; int b; int n8; int nC; float g10; float g14; int n18; int n1C; };
struct CMD *func_001E01F8(void *pq, int kind, int b);
/** @sly2-annot addr=0x1E0B40
 * Purpose: Splice builtin: append a RELAYCK 0x23 (Face) command to the relay
 *   (PrelaycmdAddRelay(prelay,0x23,fAsync)) and fill its args
 * Info: proposed name AddRelayFace (T1); TU relay
 * Notes: CONFLICTS #13 / r0 hint: optid 256 = PAUSE (Sly 1 AddJsgPause); kept Lilly RELAYCK 0x23 "Face" name
 * Confidence: high [T1 0.85] - evidence: Sly 1 / Lilly reference; sources: r6b_rest+verdicts_followups
 * See: docs/engine/cutscenes.md#5-key-functions,
 *   docs/reference/functions/cutscenes.md#relay-0x1dfad8-0x1e5d80
 * @end-sly2-annot */
void func_001E0B40(void *pq, int a, int b, float g)
{
    struct CMDF *p = (struct CMDF *)func_001E01F8(pq, 0x23, 0);
    p->g10 = g; ((struct CMD *)p)->n14 = a; p->n18 = b;
}
