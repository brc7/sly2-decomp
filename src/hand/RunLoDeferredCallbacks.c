typedef struct E { void (*pfn)(void *, void *); void *arg; struct E *next; } E;
struct S { char pad[0x34]; E *first; };
/** @sly2-annot addr=0x1A5D98
 * Purpose: Call every node of plo->x34 list in insertion order (reverse of push)
 * Info: proposed name RunLoDeferredCallbacks (T1); TU lo
 * Confidence: mid (draft) [T1 0.50] - evidence: static analysis; sources: r4_17F090
 * See: docs/reference/functions/object-model.md#lo-0x1a5270-0x1a7518
 * @end-sly2-annot */
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
