typedef int qword __attribute__((mode(TI)));
struct X; struct SPACE;
struct S { struct X *a0; struct SPACE *a4; int pad[2]; qword v; };
void func_001E67B8(struct X *, float, qword *, qword *);
qword func_001FA970(struct SPACE *, qword);
static inline qword Sub(qword a,qword b) { qword r; __asm__("vsub.xyzw %0, %1, %2" : "=j"(r) : "j"(a), "j"(b)); return r; }
static inline qword SquareBits(qword a) {
    register qword d __asm__("vf1"),one __asm__("vf3");
    __asm__("vmul.xyz %0, %2, %2\n\tvaddw.x %1, $vf0, $vf0w\n\tvadday.x $ACC, %0, %0y\n\tvmaddz.x %0, %1, %0z" : "=j"(d), "=&j"(one) : "j"(a));
    return d;
}
static inline qword ConvertedSquareBits(qword a) {
    register qword value __asm__("vf4");
    register qword square __asm__("vf1"),one __asm__("vf2");
    __asm__("qmtc2.ni %1,%0":"=j"(value):"r"(a));
    __asm__("vmul.xyz %0,%2,%2\n\tvaddw.x %1,$vf0,$vf0w\n\tvadday.x $ACC,%0,%0y\n\tvmaddz.x %0,%1,%0z":"=j"(square),"=&j"(one):"j"(value));
    return square;
}
/** @sly2-annot addr=0x1E69A0
 * Purpose: Minimizer callback: squared distance from a query point (in a space) to EvaluateShapeU(u)
 * Info: proposed name GDistShapeU (T2); TU shape
 * Notes: names_v2 C proposed PemitbCopyOnWrite__FP5EMITB (0.638)
 * Confidence: low (draft) [T2 0.40] - evidence: static naming pass; sources: r6b_rest
 * See: docs/reference/functions/physics-collision.md#shape-0x1e5d80-0x1ea568
 * @end-sly2-annot */
float func_001E69A0(void *context,float g){
    struct S *p=(struct S *)context;
    qword v;register qword bits __asm__("$2");float result;
    func_001E67B8(p->a0,g,&v,0);
    if(!p->a4){
        register qword point __asm__("vf1")=p->v;
        register qword path __asm__("vf2")=v;
        __asm__("vsub.xyzw %0,%0,%1":"+j"(point):"j"(path));
        bits=SquareBits(point);
    }
    else {
        register qword point __asm__("vf2")=p->v;
        register qword path __asm__("vf1")=v;
        register qword delta __asm__("vf3");
        __asm__("vsub.xyzw %0,%1,%2":"=j"(delta):"j"(path),"j"(point));
        bits=ConvertedSquareBits(func_001FA970(p->a4,delta));
    }
    __asm__ __volatile__("mtc1 %1,%0":"=f"(result):"r"(bits));
    return result;
}
