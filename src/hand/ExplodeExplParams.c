typedef int qword __attribute__((mode(TI)));
struct VTE { short d; short pad; void (*pfn)(); };
struct O { int a[3]; struct VTE *vt; };
struct P { int a; int b; float f; float g; qword q; qword r; qword s; };
/** @sly2-annot addr=0x15C198
 * Purpose: Explodes an EXPL with explicit flags/reference/vector/origin/scale/radius (packs an EXPLSO, calls
 *   slot 51)
 * Info: proposed name ExplodeExplParams (T1); TU emitter
 * Confidence: mid (draft) [T1 0.75] - evidence: static analysis; sources: r3_1539A8
 * See: docs/engine/particles-effects.md#6-key-functions,
 *   docs/reference/functions/particles-effects.md#emitter-0x159118-0x15cd68
 * @end-sly2-annot */
void ExplodeExplParams(struct O *o, int a, int b, float f, float g, qword q, qword r)
{
    struct P p;
    struct VTE *e;
    p.a = a;
    p.b = b;
    p.f = f;
    p.g = g;
    p.q = q;
    p.r = r;
    e = &o->vt[51];
    e->pfn((char *)o + e->d, &p);
}
