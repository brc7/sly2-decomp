struct E { int id; int n; float t; int pad; };
struct S { char pad[0xFC]; int c; struct E a[1]; };
/** @sly2-annot addr=0x154180
 * Purpose: Finds the target entry with a given id in the detector list (NULL for id 0)
 * Info: proposed name PdteFindDetectorTarget (T1); TU detector
 * Notes: names_v2 C proposed UseStepguardPhys__FP9STEPGUARD3SGS3OID (0.253)
 * Confidence: mid (draft) [T1 0.55] - evidence: static naming pass; sources: r3_1539A8
 * See: docs/reference/functions/guards-ai.md#detector-0x153d58-0x154a60
 * @end-sly2-annot */
struct E *func_00154180(struct S *p, int id)
{
    int i;
    if (!id)
        return 0;
    for (i = 0; i < p->c; i++) {
        struct E *pe = &p->a[i];
        if (pe->id == id)
            return pe;
    }
    return 0;
}
