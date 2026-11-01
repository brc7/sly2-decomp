typedef struct E { void (*pfn)(void *, void *); void *arg; struct E *next; } E;
struct S { char pad[0x34]; E *first; };
void RunLoDeferredCallbacks(struct S *s)
{
    E *a[32];
    E *e = s->first;
    int n;
    if (!e)
        return;
    n = 0;
    do { a[n] = e; n++; e = e->next; } while (e);
    for (n--; n >= 0; n--) { e = a[n]; e->pfn(s, e->arg); }
}
