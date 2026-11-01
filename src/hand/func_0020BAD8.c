typedef int qword __attribute__((mode(TI)));
typedef union { qword q; float f; } U;
extern qword D_002A2DD0;
struct C { int pad; float g; };
struct B { int pad[2]; struct C *pc; };
struct A { char pad[0xD0]; struct B *pb; };
struct E { float w; int pad[3]; qword v; int pad2; float t; char pad3[0x40 - 0x28]; };
struct S { int pad; struct A *pa; int pad2[2]; struct E ae[2]; };
/** @sly2-annot addr=0x20BAD8
 * Purpose: weighted root velocity of the two clips
 * Info: proposed name VRootGait (T2); TU tu_20B840
 * Notes: names_v2 C proposed ConvertApos__FiP6VECTORP7MATRIX4T2 (0.438)
 * Confidence: low (draft) [T2 0.45] - evidence: static naming pass; sources: r7c_hud
 * See: docs/reference/functions/animation.md#tu_20b840-0x20b840-0x20ce28
 * @end-sly2-annot */
qword func_0020BAD8(struct S *p)
{
    struct B *pb = p->pa->pb;
    float denominator=pb->pc->g;
    qword acc = D_002A2DD0;
    struct E *pe = p->ae;
    unsigned int i;
    for (i = 0; i < 2; i++) {
        U u;
        __asm__("qmtc2.ni %1, %0" : "=j"(u.q) : "r"(pe->w * (pe->t / denominator)));
        __asm__("vadda.xyzw $ACC, %0, $vf0\n\tvmaddx.xyzw %0, %1, %2x" : "+j"(acc) : "j"(pe->v), "j"(u.q));
        pe++;
    }
    return acc;
}
