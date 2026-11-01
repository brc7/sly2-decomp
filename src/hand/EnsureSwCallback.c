struct K { short x; short y; union { short s; int i; } u; };
struct N { int a; struct K k; int d; struct N *next; };
struct P { char pad[0x3CA8]; struct N *head; };
void PostSwCallback(struct P *p, int a, struct K k, int d);
/** @sly2-annot addr=0x149AB8
 * Purpose: PostSwCallback unless an identical {obj, member pointer, arg} entry is already queued
 * Info: proposed name EnsureSwCallback (T1); TU callback
 * Confidence: high [T1 0.85] - evidence: static analysis; sources: r2b_rest
 * See: docs/engine/object-model.md#9-key-functions,
 *   docs/engine/world-sw.md#6-deferred-callbacks-and-the-splice-schedule,
 *   docs/engine/world-sw.md#10-key-functions
 * @end-sly2-annot */
void EnsureSwCallback(struct P *p, int a, struct K k, int d)
{
    struct N *n;
    for (n = p->head; n; n = n->next) {
        if (n->a == a && n->k.y == k.y &&
            ((n->k.y != -1 && n->k.u.s == k.u.s) || n->k.u.i == k.u.i) &&
            n->d == d)
            return;
    }
    PostSwCallback(p, a, k, d);
}
