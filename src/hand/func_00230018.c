typedef struct VT { short d; short pad; void *pfn; } VT;
typedef struct N { char pad[0xC]; int k; struct N *next; int x14; VT *vt; } N;
typedef struct S { char pad[0x90]; N *first; } S;
void func_0022C840(void *, S *);

#define VCALL(o, k) ((void (*)(void *))(o)->vt[k].pfn)((char *)(o) + (o)->vt[k].d)

/** @sly2-annot addr=0x230018
 * Purpose: Insert an actuator into an XF slot list (+0x90) sorted by priority (+0xC); if it becomes the head,
 *   wake the slot and notify old head (vt2) and new head (vt1); else notify head (vt3)
 * Info: proposed name AddXfAct (T1); TU xf
 * Notes: top50_round (0.60): Body. Pairs RemoveXfAct 0x230100, ResortXfActList 0x2301D0. Callers
 *   AttachActToAlo, PactNew*, ApplyAseg, LoadSoXfsFromBrx. Sly 1 ACT has nPriority and ALO dlAct sorted list
 * Confidence: mid (draft) [T1 0.60] - evidence: Sly 1 / Lilly reference; semantic review; sources:
 *   r8_21BB88+top50_round
 * See: docs/reference/functions/animation.md#act-0x12b3b0-0x12e600,
 *   docs/reference/functions/animation.md#tail-0x21c820-0x21d278,
 *   docs/reference/functions/animation.md#xf-0x22c818-0x231100
 * @end-sly2-annot */
void func_00230018(void *ctx, S *s, N *n)
{
    N **pp = &s->first;
    N *head = s->first;
    N *e;
    for (e = head; e && e->k > n->k; e = e->next)
        pp = &e->next;
    n->next = *pp;
    *pp = n;
    if (s->first != head) {
        func_0022C840(ctx, s);
        if (head)
            VCALL(head, 2);
        VCALL(s->first, 1);
    } else if (head) {
        VCALL(head, 3);
    }
}
