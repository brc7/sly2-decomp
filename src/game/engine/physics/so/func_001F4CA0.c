struct T { char pad[0x80]; int a80; int a84; void *p88; void *p8C; char pad2[0x100 - 0x90]; };
struct S { char pad[0x10]; struct S *next; char pad2[0x50 - 0x14]; int c; struct T *base; };
/** @sly2-annot addr=0x1F4CA0
 * Purpose: SO vt52: re-point an XF* from base SO's XF block to the clone's (also via parent)
 * Info: proposed name CloneXfRef (T1); class SO slot52; TU so
 * Confidence: high [T1 0.85] - evidence: static analysis; sources: r6_1D5150+verdicts_tu_r4r6
 * See: docs/engine/physics-collision.md#7-key-functions,
 *   docs/reference/functions/physics-collision.md#so-0x1f2108-0x1f8dd8
 * @end-sly2-annot */
void func_001F4CA0(struct S *pnew, struct S *pold, struct T **pp)
{
    struct T *t = *pp;
    struct T *n;
    int i;
    if (!t) return;
    n = 0;
    i = t - pold->base;
    if (i >= 0 && i < pnew->c) {
        n = pnew->base + i;
    } else {
        pold = pold->next;
        i = t - pold->base;
        if (i >= 0 && i < pold->c) n = pnew->next->base + i;
    }
    if (n->p88 == &t->a80) n->p88 = &n->a80;
    if (n->p8C == &t->a84) n->p88 = &n->a84;
    *pp = n;
}
