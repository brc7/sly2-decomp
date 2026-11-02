typedef struct VT { short d; short pad; void *pfn; } VT;
typedef struct N { char pad[0x10]; struct N *next; int x14; VT *vt; } N;
typedef struct S { char pad[0x90]; N *first; } S;
void func_0022C840(void *, S *);

#define VCALL(o, k) ((void (*)(void *))(o)->vt[k].pfn)((char *)(o) + (o)->vt[k].d)

/** @sly2-annot addr=0x230100
 * Purpose: unlink actuator from XF list, then wake
 * Info: proposed name RemoveXfAct (T2); TU xf
 * Confidence: low (draft) [T2 0.45] - evidence: static analysis; sources: r8_21BB88
 * See: docs/reference/functions/animation.md#act-0x12b3b0-0x12e600,
 *   docs/reference/functions/animation.md#xf-0x22c818-0x231100
 * @end-sly2-annot */
void func_00230100(void *ctx, S *s, N *n)
{
    N *head = s->first;
    N **pp = &s->first;
    N *e;
    for (e = head; e; e = e->next) {
        if (e == n) {
            *pp = e->next;
            if (s->first != head) {
                func_0022C840(ctx, s);
                VCALL(head, 2);
                if (s->first)
                    VCALL(s->first, 1);
            } else if (head) {
                VCALL(head, 3);
            }
            return;
        }
        pp = &e->next;
    }
}
