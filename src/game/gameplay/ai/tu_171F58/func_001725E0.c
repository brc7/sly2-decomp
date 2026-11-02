struct VTE { short d; short pad; void (*pfn)(void *); };
struct O { char pad[0xC]; struct VTE *vt; };
struct S { char pad[0x44]; struct O *p44; int n48; };
/** @sly2-annot addr=0x1725E0
 * Purpose: Swaps a watched refcounted object (+0x44) via its vt1/vt2 (incref/decref) and flags a re-plan
 * Info: proposed name SetBhv171Watched (T2); TU tu_171F58
 * Confidence: low (draft) [T2 0.35] - evidence: static analysis; sources: r3b_rest
 * See: docs/engine/guards-ai.md#5-perception,
 *   docs/reference/functions/guards-ai.md#tu_171f58-0x171f58-0x173158
 * @end-sly2-annot */
void func_001725E0(struct S *p, struct O *o) {
    if (o == p->p44) return;
    if (p->p44) { struct VTE *e = &p->p44->vt[2]; e->pfn((char *)p->p44 + e->d); }
    p->p44 = o;
    if (o) { struct VTE *e = &o->vt[1]; e->pfn((char *)o + e->d); }
    p->n48 = 1;
}
