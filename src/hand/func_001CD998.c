struct E { int a, b; int c[2]; };
extern struct E *D_003D4A50;
void *func_002333B4(void *p, int c, int n);
/** @sly2-annot addr=0x1CD998
 * Purpose: Bump-allocate a 16-byte RPL from g_prplNext (zeroes pqwA/pqwB)
 * Info: proposed name PrplAllocRpl (T1); TU render
 * Notes: superseded: PvAllocRenderEntry (r5_1AE130)
 * Confidence: mid (draft) [T1 0.60] - evidence: static analysis; sources: r5_1AE130+rendering
 * See: docs/engine/rendering.md#2-render-list-entries-rpl,
 *   docs/reference/functions/rendering.md#render-0x1cd5f0-0x1cdcd0
 * @end-sly2-annot */
struct E *func_001CD998(void)
{
    /*B*/func_002333B4(D_003D4A50->c, 0, 8); return D_003D4A50++;/*E*/
}
