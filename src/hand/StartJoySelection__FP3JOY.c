struct S { char pad[0x1024]; int a; int b; };
/** @sly2-annot addr=0x187E78
 * Purpose: Reset the menu selection repeat state (two fields = 0)
 * Info: proposed name StartJoySelection (T1); TU joy
 * Confidence: mid (draft) [T1 0.55] - evidence: static naming pass; sources:
 *   r4b_rest+verdicts_A1+names_v2-signature(verified)
 * See: docs/engine/main-loop-startup.md#8-key-functions,
 *   docs/reference/functions/main-loop-startup.md#joy-0x187030-0x188488
 * @end-sly2-annot */
void StartJoySelection__FP3JOY(struct S *p)
{
    p->a = 0;
    p->b = 0;
}
