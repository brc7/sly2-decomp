struct VTE { short d; short pad; void (*pfn)(); };
struct O { int a[3]; struct VTE *vt; };
/** @sly2-annot addr=0x15C628
 * Purpose: EXPLS explode: forwards to its own vtable slot 52 (fire)
 * Info: proposed name ExplodeExplsExplso (T1); class EXPLS slot51; TU emitter
 * Confidence: mid (draft) [T1 0.55] - evidence: static analysis; sources: r3_1539A8+verdicts_B
 * See: docs/engine/particles-effects.md#6-key-functions,
 *   docs/reference/functions/particles-effects.md#emitter-0x159118-0x15cd68
 * @end-sly2-annot */
void ExplodeExplsExplso(struct O *o, int x)
{
    struct VTE *e = &o->vt[52];
    e->pfn((char *)o + e->d, x);
}
