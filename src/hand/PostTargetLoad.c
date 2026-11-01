struct S { char pad[0x38]; float x38; char pad2[0xB0]; float xec; };
void PostLoLoad(struct S *p);
extern float D_002C40C0;
/** @sly2-annot addr=0x21D2C8
 * Purpose: PostLoLoad then if sBusy(0x38)==1e10: sBusy = sRadiusTarget + const D_002C40C0
 * Info: proposed name PostTargetLoad (T1); class TARGET slot15; TU target
 * Notes: names_v2 C proposed InitHshape__FP6HSHAPE (0.638)
 * Confidence: mid (draft) [T1 0.60] - evidence: static naming pass; sources: r8_21BB88
 * See: docs/reference/functions/gameplay-objects.md#target-0x21d278-0x21d638
 * @end-sly2-annot */
void PostTargetLoad(struct S *p)
{
    PostLoLoad(p);
    if (p->x38 == 1e10f)
        p->x38 = p->xec + D_002C40C0;
}
