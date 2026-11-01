typedef int qword __attribute__((mode(TI)));
typedef struct VECTOR { float x, y, z, w; } __attribute__((aligned(16))) VECTOR;
enum { SCALE_WORD_ADDRESS = 0x002C0D04 };
void GetPipeGoalMat(void *p, float g, int n, qword *pv);
void EvaluateShapeS(void *p, float g, VECTOR *pv, VECTOR *tangent);

static inline int ScaleBits(void) {
    register int *page __asm__("$2") = (int *)(SCALE_WORD_ADDRESS & ~0xFFFF);
    register int bits __asm__("$2");
    __asm__("lw %0,%1":"=r"(bits):"m"(page[(SCALE_WORD_ADDRESS & 0xFFFF) / sizeof(int)]));
    return bits;
}

void func_001C3A08(void *p, int unused, qword *pvec, float g)
{
    qword a[3];
    VECTOR b;
    union { qword q; float f; } s;
    register qword r __asm__("vf2");
    register qword scale __asm__("vf3"), axis __asm__("vf1"), position __asm__("vf2");
    GetPipeGoalMat(p, g, 1, a);
    EvaluateShapeS(p, g, &b, 0);
    if (pvec) {
        __asm__("lqc2 %0,%1":"=j"(position):"m"(b));
        __asm__("qmtc2.ni %2,%0\n\tlqc2 %1,%3":"=j"(scale),"=j"(axis):"r"(ScaleBits()),"m"(a[0]));
        s.q = scale;
        __asm__("vadda.xyzw $ACC, %1, $vf0\n\tvmaddx.xyzw %0, %2, %3x" : "=j"(r) : "j"(position), "j"(axis), "j"(s.q));
        *pvec = r;
    }
}
