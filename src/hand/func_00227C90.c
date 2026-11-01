typedef int qword __attribute__((mode(TI)));
struct S { char pad[0xEC]; int k; char pad2[0x20]; qword pos; float r; };
qword func_001A6F00(int a, struct S *p, qword q);
int func_00118FB0(int a, int b, qword q);
/** @sly2-annot addr=0x227C90
 * Purpose: reject by bounding sphere then BSP point test (0x118FB0) in local space
 * Info: proposed name FCheckVolPoint (T1); TU vol
 * Confidence: mid (draft) [T1 0.60] - evidence: static analysis; sources: r8_21BB88
 * See: docs/reference/functions/guards-ai.md#detector-0x153d58-0x154a60,
 *   docs/reference/functions/guards-ai.md#tu_1553f0-0x1553f0-0x1564e8,
 *   docs/reference/functions/physics-collision.md#vol-0x227bb8-0x227fe8
 * @end-sly2-annot */
int func_00227C90(struct S *p, qword q)
{
    qword a = func_001A6F00(0, p, q), d, r, t;
    float f;
    int fFar;
    __asm__("vsub.xyzw %0, %1, %2" : "=j"(d) : "j"(a), "j"(p->pos));
    __asm__("vmul.xyz %0, %0, %0\n\tvaddw.x %1, $vf0, $vf0w\n\tvadday.x $ACC, %0, %0y\n\tvmaddz.x %0, %1, %0z" : "=j"(r), "=&j"(t) : "0"(d));
    __asm__("qmfc2.ni %0, %1" : "=r"(f) : "j"(r));
    fFar = p->r < f;
    if (fFar) return 0;
    return func_00118FB0(p->k, 0x7FFFFFFF, a) != 0;
}
