struct E { int a[10]; };
struct S { char pad[0x128]; struct E *pe; };
extern struct E D_003E1A78[];
extern int *D_002C0BF8[];
/** @sly2-annot addr=0x1BEE10
 * Purpose: Table lookup D_002C0BF8[slot][n] or -1
 * Info: proposed name IRenderSlotTexture (T2); TU tu_1BE398
 * Notes: verifier PLAUSIBLE: Returns D_002C0BF8[(pe - D_003E1A78)][n] (row of 10 ints) or -1; caller
 *   DrawGlobsetSkinned. That the value is a texture index is a guess. | T2 forced: verifier: role word not
 *   supported by the code
 * Confidence: low (draft) [T2 0.30] - evidence: static analysis; sources: r5b_rest+verdicts_followups
 * See: docs/engine/rendering.md#13-key-functions,
 *   docs/reference/functions/rendering.md#tu_1be398-0x1be398-0x1bf110
 * @end-sly2-annot */
int func_001BEE10(struct S *p, int n)
{
    if (!p->pe) return -1;
    return D_002C0BF8[p->pe - D_003E1A78][n];
}
