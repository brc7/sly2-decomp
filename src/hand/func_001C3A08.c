typedef int qword __attribute__((mode(TI)));
typedef struct VECTOR { float x, y, z, w; } __attribute__((aligned(16))) VECTOR;
enum { SCALE_WORD_ADDRESS = 0x002C0D04 };
void func_001C3500(void *p, float g, int n, qword *pv);
void func_001E6F10(void *p, float g, VECTOR *pv, VECTOR *tangent);

static inline int ScaleBits(void) {
    register int *page __asm__("$2") = (int *)(SCALE_WORD_ADDRESS & ~0xFFFF);
    register int bits __asm__("$2");
    __asm__("lw %0,%1":"=r"(bits):"m"(page[(SCALE_WORD_ADDRESS & 0xFFFF) / sizeof(int)]));
    return bits;
}

/** @sly2-annot addr=0x1C3A08
 * Purpose: GRABSPOT iface: basis + position at distance
 * Info: proposed name GetPipeGrabPosMat (T2); class PIPE; TU pipe
 * Notes: names_v2 C proposed InitHbsk__FP4HBSK (0.638)
 * Confidence: low (draft) [T2 0.30] - evidence: static naming pass; sources: r5_1AE130
 * See: docs/reference/functions/player.md#pipe-0x1c3390-0x1c3d70
 * @end-sly2-annot */
void func_001C3A08(void *p, int unused, qword *pvec, float g)
{
    qword a[3];
    VECTOR b;
    union { qword q; float f; } s;
    register qword r __asm__("vf2");
    register qword scale __asm__("vf3"), axis __asm__("vf1"), position __asm__("vf2");
    func_001C3500(p, g, 1, a);
    func_001E6F10(p, g, &b, 0);
    if (pvec) {
        __asm__("lqc2 %0,%1":"=j"(position):"m"(b));
        __asm__("qmtc2.ni %2,%0\n\tlqc2 %1,%3":"=j"(scale),"=j"(axis):"r"(ScaleBits()),"m"(a[0]));
        s.q = scale;
        __asm__("vadda.xyzw $ACC, %1, $vf0\n\tvmaddx.xyzw %0, %2, %3x" : "=j"(r) : "j"(position), "j"(axis), "j"(s.q));
        *pvec = r;
    }
}
