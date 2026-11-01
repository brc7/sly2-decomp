typedef int qword __attribute__((mode(TI)));
struct B { qword min; qword max; qword c; float r; };
struct P { char pad[0xEC]; struct B *pb; };
/** @sly2-annot addr=0x1F5480
 * Purpose: Grow tree AABB/radius to include sphere (c,r)
 * Info: proposed name ExtendSoTreeBounds (T1); TU so
 * Confidence: mid (draft) [T1 0.55] - evidence: static analysis; sources: r6_1D5150
 * See: docs/reference/functions/physics-collision.md#so-0x1f2108-0x1f8dd8,
 *   docs/reference/functions/player.md#jt-0x188488-0x18e540
 * @end-sly2-annot */
void func_001F5480(struct P *p, qword c, float r)
{
    qword rr, lo, hi, d, t, one;
    float dist;
    __asm__("pextlw %0, %1, %1\n\tpextlw %0, %0, %0" : "=r"(rr) : "r"(r));
    __asm__("vsub.xyzw %0, %1, %2" : "=j"(lo) : "j"(c), "j"(rr));
    __asm__("vmini.xyz %0, %1, %2" : "=j"(p->pb->min) : "j"(p->pb->min), "j"(lo));
    __asm__("vadd.xyzw %0, %1, %2" : "=j"(hi) : "j"(c), "j"(rr));
    __asm__("vmax.xyz %0, %1, %2" : "=j"(p->pb->max) : "j"(p->pb->max), "j"(hi));
    __asm__("vsub.xyzw %0, %1, %2" : "=j"(c) : "0"(c), "j"(p->pb->c));
    __asm__("vmul.xyz %0, %2, %2\n\tvaddw.x %1, $vf0, $vf0w\n\tvadday.x $ACC, %0, %0y\n\tvmaddz.x %0, %1, %0z\n\tvsqrt $Q, %0x\n\tvwaitq\n\tvaddq.x %0, $vf0, $Q" : "=j"(d), "=&j"(one) : "0"(c));
    __asm__("qmfc2.ni %0, %1" : "=r"(dist) : "j"(d));
    if (p->pb->r < dist) p->pb->r = dist;
}
