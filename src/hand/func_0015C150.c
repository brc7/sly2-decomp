typedef int qword __attribute__((mode(TI)));
struct VTE { short d; short pad; void (*pfn)(); };
struct O { int a[3]; struct VTE *vt; };
struct P { int a; int b; float f; float g; qword q; qword r; qword s; };
void func_002333B4(void *pv, int b, int cb);
/** @sly2-annot addr=0x15C150
 * Purpose: Explodes an EXPL with default (zero) parameters via its explode virtual (slot 51)
 * Info: proposed name ExplodeExpl (T1); TU emitter
 * Confidence: mid (draft) [T1 0.75] - evidence: static analysis; sources: r3_1539A8
 * See: docs/engine/particles-effects.md#6-key-functions,
 *   docs/reference/functions/particles-effects.md#emitter-0x159118-0x15cd68
 * @end-sly2-annot */
void func_0015C150(struct O *o)
{
    struct P p = {0};
    struct VTE *vt;

    vt = o->vt;
    vt[51].pfn((char *)o + vt[51].d, &p);
}
