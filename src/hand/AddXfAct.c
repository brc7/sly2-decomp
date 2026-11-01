typedef struct VT { short d; short pad; void *pfn; } VT;
typedef struct N { char pad[0xC]; int k; struct N *next; int x14; VT *vt; } N;
typedef struct S { char pad[0x90]; N *first; } S;
void func_0022C840(void *, S *);

#define VCALL(o, k) ((void (*)(void *))(o)->vt[k].pfn)((char *)(o) + (o)->vt[k].d)

void AddXfAct(void *ctx, S *s, N *n)
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
