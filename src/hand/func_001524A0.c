void ApplyAsegCur__FP4ASEGP3ALOffiPP5ASEGA(void *paseg, void *palo, float a, float b, int n, void **pp);
/** @sly2-annot addr=0x1524A0
 * Purpose: ApplyAsegCur(paseg, this, 0, 1, grf|1, &this->pasega408)
 * Info: proposed name ApplyDancerAseg (T1); TU rhythm
 * Confidence: mid (draft) [T1 0.60] - evidence: semantic review; sources: r2_131F70
 * See: docs/reference/functions/gameplay-objects.md#rhythm-0x1520c8-0x1539a8
 * @end-sly2-annot */
void func_001524A0(char *p, void *paseg, int n)
{
    ApplyAsegCur__FP4ASEGP3ALOffiPP5ASEGA(paseg, p, 0.0f, 1.0f, n | 1, (void **)(p + 0x408));
}
