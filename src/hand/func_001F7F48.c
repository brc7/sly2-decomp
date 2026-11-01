typedef unsigned long ulong;
struct A { char pad[0x148]; ulong a:34; ulong f:1; ulong b:1; };
void func_001F55C8(struct A *p);
/** @sly2-annot addr=0x1F7F48
 * Purpose: Set +0x148 bit34 and RebuildSoPhysHook if changed
 * Info: proposed name SetSoPhysHookFlag (T2); TU so
 * Confidence: low (draft) [T2 0.30] - evidence: static analysis; sources: r6_1D5150
 * See: docs/reference/functions/physics-collision.md#so-0x1f2108-0x1f8dd8
 * @end-sly2-annot */
void func_001F7F48(struct A *p, long f)
{
    if (f != p->f) {
        p->f = f;
        func_001F55C8(p);
    }
}
