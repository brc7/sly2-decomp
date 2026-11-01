typedef struct { short d; short pad; void *pfn; } VTE;
struct T { char pad[0x14]; int n14; };
struct A { char pad[0x200]; struct T *pt; };
struct Q { struct A *pa; int pad[2]; int nC; };
struct S { char pad[0xC]; VTE *vt; char pad2[4]; int n14; char pad3[0x26C - 0x18]; char x26C[4]; };
int func_00132B10(struct A *pa, int n);
int func_001F7588(struct S *p, struct Q *q);
void func_001EED78(void *p, int k, struct Q *q);
/** @sly2-annot addr=0x207468
 * Purpose: STEPGUARD vt75: unless the zap is lethal (vt86): ignore if invulnerable (vt76), own-team bomb (cid
 *   0x4F), or base says no; then forward the zap to the BHV stack (message 0xC)
 * Info: proposed name FTakeStepguardDamage (T1); class STEPGUARD slot75; TU stepguard
 * Confidence: mid (draft) [T1 0.50] - evidence: static analysis; sources: r7b_guard
 * See: docs/reference/functions/guards-ai.md#stepguard-0x205e70-0x20b840
 * @end-sly2-annot */
int func_00207468(struct S *p, struct Q *q)
{
    if (!((int (*)(void *))p->vt[86].pfn)((char *)p + p->vt[86].d)) {
        if (((int (*)(void *, int))p->vt[76].pfn)((char *)p + p->vt[76].d, q->nC))
            return 1;
        if (q->pa && func_00132B10(q->pa, 0x4F) && q->pa->pt->n14 == p->n14)
            return 1;
        if (!func_001F7588(p, q))
            return 0;
    }
    func_001EED78(p->x26C, 0xC, q);
    return 1;
}
