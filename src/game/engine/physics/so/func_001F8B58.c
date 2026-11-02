struct R { char pad[0x14]; int n14; int pad2; float g1C; float g20; };
struct P { char pad[0x20]; void *p20; void *p24; };
struct R *func_001F8910(struct P *, int, int, int);
void func_0012DCA0(void *, void *, int, int, float, float);
/** @sly2-annot addr=0x1F8B58
 * Purpose: Update/create kind-4 actuator with two floats
 * Info: proposed name SetXfhActuator4 (T2); TU so
 * Notes: names_v2 v1 proposed RebuildSoPhysHook__FP2SO (0.081)
 * Confidence: low (draft) [T2 0.25] - evidence: static naming pass; sources: r6_1D5150
 * See: docs/reference/functions/physics-collision.md#so-0x1f2108-0x1f8dd8
 * @end-sly2-annot */
void func_001F8B58(struct P *p, int n, int m, float f, float g)
{
    struct R *r = func_001F8910(p, 4, n, m);
    if (r) {
        r->n14 = n;
        r->g1C = f;
        r->g20 = g;
    } else if (n != -1) {
        func_0012DCA0(p->p20, p->p24, m, n, f, g);
    }
}
