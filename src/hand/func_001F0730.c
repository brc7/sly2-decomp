struct P { char pad[0x24]; void *p24; char pad2[0x30 - 0x28]; int n30; int n34; int n38; char pad3[0x48 - 0x3C]; int n48; };
int func_001F0638(struct P *);
int func_001F0400(void *, int);
int func_001F0B80(void *, int, int, int, int);
void func_001F0660(struct P *, int);
/** @sly2-annot addr=0x1F0730
 * Purpose: SetSmaGoal only if the chooser finds a path under the current mask
 * Info: proposed name SetSmaGoalIfReachable (T1); TU sm
 * Notes: names_v2 C proposed SetSmaGoal__FP3SMA3OID (0.200)
 * Confidence: mid (draft) [T1 0.65] - evidence: semantic review; static naming pass; sources:
 *   semantic_tu_docs/sm
 * See: docs/reference/functions/animation.md#sm-0x1efa90-0x1f1768
 * @end-sly2-annot */
void func_001F0730(struct P *p, int n)
{
    if (!func_001F0638(p) && n != -1) {
        int r = func_001F0400(p->p24, n);
        if (r != -1 && func_001F0B80(p->p24, p->n48, !p->n30 ? p->n34 : p->n38, r, 0))
            func_001F0660(p, n);
    }
}
