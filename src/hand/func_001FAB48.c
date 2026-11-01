typedef int qword __attribute__((mode(TI)));
struct B { char pad[0x70]; qword pos; };
struct A { char pad[0x5C]; struct B *pb; };
/** @sly2-annot addr=0x1FAB48
 * Purpose: v + w x (pos - center): velocity of a point of the SO
 * Info: proposed name VelocityAtSoPos (T2); TU solve
 * Confidence: low (draft) [T2 0.40] - evidence: static analysis; sources: r7_1F8DD8
 * See: docs/reference/functions/physics-collision.md#solve-0x1f8dd8-0x1faf70
 * @end-sly2-annot */
qword func_001FAB48(struct A *p, qword *pv, qword q)
{
    qword r, c, s;
    __asm__("vsub.xyzw %0, %1, %2" : "=j"(r) : "j"(q), "j"(p->pb->pos));
    c=pv[1];
    __asm__("vopmula.xyz $ACC, %0, %1\n\tvopmsub.xyz %0, %1, %0" : "+j"(c) : "j"(r));
    __asm__("vadd.xyzw %0, %1, %2" : "=j"(s) : "j"(pv[0]), "j"(c));
    return s;
}

#define OFF(T,F) ((unsigned long)&((T*)0)->F)
typedef char velocity_point_layout[(sizeof(qword)==16 && __alignof__(qword)==16 && sizeof(void*)==4 && sizeof(struct B)==0x80 && OFF(struct B,pos)==0x70 && OFF(struct A,pb)==0x5C)?1:-1];
