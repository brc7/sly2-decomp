typedef int qword __attribute__((mode(TI)));
typedef union { qword q; float f; } U;
struct T { char pad[0x70]; qword x70; };
union WORD { int bits; float value; };
struct S { char pad[0x134]; union WORD x134; };
struct P { char pad[0x18C4]; struct S *x18c4; char pad2[0x19CC-0x18C8]; struct T *x19cc; };
extern qword D_002A2DC0;
qword func_001A62E8(struct S *p);
#define FIELD_OFFSET(t,m) ((unsigned int)&(((t*)0x10000)->m)-0x10000)
typedef char T_x70_Offset[FIELD_OFFSET(struct T,x70)==112?1:-1];
typedef char S_x134_Offset[FIELD_OFFSET(struct S,x134)==308?1:-1];
typedef char P_x18c4_Offset[FIELD_OFFSET(struct P,x18c4)==6340?1:-1];
typedef char P_x19cc_Offset[FIELD_OFFSET(struct P,x19cc)==6604?1:-1];
typedef char Size_0[sizeof(qword)==16?1:-1];
typedef char Size_1[sizeof(U)==16?1:-1];
typedef char Size_2[sizeof(union WORD)==4?1:-1];
typedef char Size_3[sizeof(struct T)==128?1:-1];
typedef char Size_4[sizeof(struct S)==312?1:-1];
typedef char Size_5[sizeof(struct P)==6608?1:-1];
typedef char LengthBits_Size[sizeof(int)==4?1:-1];
typedef char DataPointer_Size[sizeof(struct S*)==4?1:-1];
/** @sly2-annot addr=0x190C58
 * Purpose: Return hook pos + normalize(hook pos - hand XF x19CC pos) * hook radius (x134)
 * Info: proposed name PosJtHangHandle (T2); TU hang
 * Confidence: low (draft) [T2 0.45] - evidence: static analysis; sources: r4b_rest
 * See: docs/reference/functions/player.md#hang-0x18f908-0x191090
 * @end-sly2-annot */
qword func_00190C58(struct P *p)
{
    qword q, d, dir, r;
    register qword fallback __asm__("$2");
    register qword l __asm__("vf1");
    register qword t __asm__("vf2");
    register int lengthBits __asm__("$3");
    U s;
    float g;
    q = func_001A62E8(p->x18c4);
    __asm__("vsub.xyzw %0, %1, %2" : "=j"(d) : "j"(q), "j"(p->x19cc->x70));
    {
        register const qword *address __asm__("$2") = &D_002A2DC0;
        __asm__("lq %0,%1" : "=r"(fallback) : "m"(*address));
    }
    __asm__("vmul.xyz %0, %2, %2\n\tvaddw.x %1, $vf0, $vf0w\n\tvadday.x $ACC, %0, %0y\n\tvmaddz.x %0, %1, %0z\n\tvsqrt $Q, %0x\n\tvwaitq\n\tvaddq.x %0, $vf0, $Q"
            : "=&j"(l), "=&j"(t) : "j"(d));
    __asm__("qmfc2.ni %0,%2\n\tmtc1 %0,%1" : "=&r"(lengthBits), "=f"(g) : "j"(l));
    if (g < 0.0001f)
        dir = fallback;
    else
        __asm__ __volatile__("vdiv $Q, $vf0w, %1x\n\tvwaitq\n\tvmulq.xyzw %0, %2, $Q" : "=j"(dir) : "j"(l), "j"(d));
    {
        register struct S *object __asm__("$2") = p->x18c4;
        register int bits __asm__("$2") = object->x134.bits;
        __asm__("qmtc2.ni %1,%0" : "=j"(s.q) : "r"(bits));
    }
    __asm__("vadda.xyzw $ACC, %1, $vf0\n\tvmaddx.xyzw %0, %2, %3x" : "=j"(r) : "j"(q), "j"(dir), "j"(s.q));
    return r;
}
