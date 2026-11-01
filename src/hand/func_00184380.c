struct T { char pad[0xF4]; int n; };
struct S { char pad[0x180]; int x; char pad2[0x18]; int n; };
int func_001A6AF8(struct T *q, int x);
/** @sly2-annot addr=0x184380
 * Purpose: 1 if ids differ else FFindLoParent
 * Info: proposed name FCid35NotTarget (T2); class CID35; TU grabspot
 * Notes: names_v2 C proposed OnHbskAdd__FP4HBSK (0.638) | verifier PLAUSIBLE: Returns 1 if q+0xF4 != p+0x198,
 *   else FFindLoParent(q, p+0x180); used by ChooseMurrayThrowTarget. Body matches purpose, but 'NotTarget'
 *   does not describe the parent test; class placeholder. | T2 forced: verifier: role word not supported by
 *   the code
 * Confidence: low (draft) [T2 0.25] - evidence: static naming pass; sources: r4b_rest+verdicts_followups
 * See: docs/reference/functions/player.md#grabspot-0x182c70-0x1848a8
 * @end-sly2-annot */
int func_00184380(struct S *p, struct T *q)
{
    if (q->n != p->n) return 1;
    return func_001A6AF8(q, p->x);
}
