typedef struct { short d; short pad; void *pfn; } VTE;
struct Q { char pad[0xC]; VTE *vt; char pad2[0xEC - 0x10]; void *pEC; };
struct T { char pad[0xEC]; void *pEC; };
struct O { char pad[0x50]; int k; char pad2[8]; struct T *pt; };
struct S { char pad[0x26C]; char x26C[4]; };
int func_001F4FC8(struct S *p, struct Q *q);
int func_00209A68(struct S *p, int n);
struct O *func_001EEEE0(void *p, int n);
int func_00134310(int a, int b);
/** @sly2-annot addr=0x206380
 * Purpose: STEPGUARD vt61: base test, plus ignore non-fixed objects while flag 7 (dead), or BHV177
 *   (stunned/carried, cid 0xB1) states 0/4/5 vs its holder group
 * Info: proposed name FIgnoreStepguardIntersection (T1); class STEPGUARD slot61; TU stepguard
 * Confidence: mid (draft) [T1 0.60] - evidence: static analysis; sources: r7b_guard
 * See: docs/reference/functions/guards-ai.md#stepguard-0x205e70-0x20b840
 * @end-sly2-annot */
int func_00206380(struct S *p, struct Q *q)
{
    int r = func_001F4FC8(p, q);
    if (func_00209A68(p, 7)) {
        if (!((int (*)(void *))q->vt[62].pfn)((char *)q + q->vt[62].d))
            r = func_00134310(r, 1);
    } else if (func_00209A68(p, 0x17)) {
        struct O *o = func_001EEEE0(p->x26C, 0xB1);
        if (q->pEC == o->pt->pEC) {
            switch (o->k) {
            case 0:
            case 4:
            case 5:
                r = func_00134310(r, 1);
                break;
            }
        }
    }
    return r;
}
