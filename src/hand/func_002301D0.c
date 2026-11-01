/* (Claude agent, lane E13) Matched. c is reused as the insertion index (for (c--; ...)), and the sorted insert is
   an if + do-while instead of a for loop. */
typedef struct VT { short d; short pad; void *pfn; } VT;
typedef struct N { char pad[0xC]; int k; struct N *next; int x14; VT *vt; } N;
typedef struct S { char pad[0x90]; N *first; } S;
void func_0022C840(void *, S *);

#define VCALL(o, k) ((void (*)(void *))(o)->vt[k].pfn)((char *)(o) + (o)->vt[k].d)

/** @sly2-annot addr=0x2301D0
 * Purpose: re-sorts up to 16 actuators by priority
 * Info: proposed name ResortXfActList (T2); TU xf
 * Notes: names_v2 C proposed ResortAloActList__FP3ALO (0.638)
 * Confidence: low (draft) [T2 0.45] - evidence: static naming pass; sources: r8_21BB88
 * See: docs/reference/functions/animation.md#asega-0x1308a8-0x131f70,
 *   docs/reference/functions/animation.md#xf-0x22c818-0x231100
 * @end-sly2-annot */
void func_002301D0(void *ctx, S *s)
{
    N *a[16];
    int c = 0;
    N *e;
    N *head;
    {
        N *ee;
        for (ee = s->first; ee; ee = ee->next)
            a[c++] = ee;
    }
    head = s->first;
    s->first = 0;
    for (c--; c >= 0; c--) {
        N *n = a[c];
        N **pp = &s->first;
        e = s->first;
        if (e && e->k > n->k) {
            do {
                pp = &e->next;
                e = e->next;
            } while (e && e->k > n->k);
        }
        n->next = *pp;
        *pp = n;
    }
    if (s->first != head) {
        func_0022C840(ctx, s);
        if (head)
            VCALL(head, 2);
        if (s->first)
            VCALL(s->first, 1);
    } else if (head) {
        VCALL(head, 3);
    }
}
