struct E { char b[0x44]; };
struct X { int a; struct E ae[2]; };
void PostAshLoad(struct E *e, int a);
/** @sly2-annot addr=0x148740
 * Purpose: PostAshLoad for the on and off ASH lists
 * Info: proposed name PostBtnLoad (T1); TU button
 * Notes: names_v2 C proposed FOnlySensorTriggerObject__FP6SENSORP2SO (0.167)
 * Confidence: mid (draft) [T1 0.65] - evidence: static naming pass; sources: r2b_rest
 * See: docs/reference/functions/gameplay-objects.md#button-0x148620-0x149a50
 * @end-sly2-annot */
void PostBtnLoad(struct X *p)
{
    unsigned int i;
    for (i = 0; i < 2; i++) PostAshLoad(&p->ae[i], p->a);
}
