typedef int qword __attribute__((mode(TI)));
struct VTE { short d; short pad; void (*pfn)(void *, qword); };
struct S { char pad[0xC]; struct VTE *vt; char pad1[0x16C - 0x10]; void *p16C; int pad2; float f174; float f178; };
void func_001B17B8(struct S *p, float g);
void func_001B1328(struct S *p, int f);
float UMinShape(void *p);
float DuGetShapeSearchIncrement(void *p);
void EvaluateShapeU(void *p, float g, qword *pv, int n);
/** @sly2-annot addr=0x1B0D90
 * Purpose: Squash ratio 1, if on a path go to state 1 at path start (vt29 TranslateToPos), else idle
 * Info: proposed name ResetCid96 (T2); class CID96; TU tu_1B0B78
 * Notes: names_v2 v1 proposed UpdateStepXfWorld__FP4STEP (0.070)
 * Confidence: low (draft) [T2 0.45] - evidence: static naming pass; sources: r5b_rest
 * See: docs/reference/functions/gameplay-objects.md#tu_1b0b78-0x1b0b78-0x1b1db8
 * @end-sly2-annot */
void func_001B0D90(struct S *p)
{
    qword v;
    func_001B17B8(p, 1.0f);
    if (p->p16C) {
        func_001B1328(p, 1);
        p->f174 = UMinShape(p->p16C);
        p->f178 = DuGetShapeSearchIncrement(p->p16C);
        EvaluateShapeU(p->p16C, p->f174, &v, 0);
        p->vt[29].pfn((char *)p + p->vt[29].d, v);
    } else {
        func_001B1328(p, 0);
    }
}
