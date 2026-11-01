extern int D_002BFB28[];
struct S { int x0; int x4; char pad[0xCC]; void *a[1]; };
/** @sly2-annot addr=0x1A23F8
 * Purpose: Return the state record for the hurt state, falling back through parent table D_002BFB28
 * Info: proposed name PactadjFromJtHurt (T2); TU hurt
 * Confidence: low (draft) [T2 0.35] - evidence: static analysis; sources: r4b_rest
 * See: docs/reference/functions/player.md#hurt-0x1a0b18-0x1a2bd0
 * @end-sly2-annot */
void *func_001A23F8(struct S *p, int *pout)
{
    int i; void *r = 0; for (i = p->x4; i != -1 && (r = p->a[i]) == 0; i = D_002BFB28[i]) ; *pout = 0; return r;
}
