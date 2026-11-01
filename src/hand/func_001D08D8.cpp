// Initialize orientation from three random direction components.
typedef int qword __attribute__((mode(TI)));
typedef struct { float x, y, z; } __attribute__((aligned(16))) VECTOR;
typedef struct { float mat[3][4]; } __attribute__((aligned(16))) MATRIX3;
typedef struct {
    char pad0[0x40];
    MATRIX3 x40;
} OBJ;
extern "C" void func_001CE9B8(OBJ *);
extern "C" float func_0011D448(float, float);
extern "C" MATRIX3 *func_0011B7E0(MATRIX3 *,qword);
static inline MATRIX3 MakeBasis(qword vector) return r {func_0011B7E0(&r,vector);}
/** @sly2-annot addr=0x1D08D8
 * Purpose: LEAF (RIPT 5) PostEmit: random initial orientation
 * Info: proposed name PostLeafEmit (T1); TU rip
 * Confidence: mid (draft) [T1 0.65] - evidence: static analysis; sources: r5_1AE130
 * See: docs/reference/functions/particles-effects.md#rip-0x1cdcd0-0x1d1370
 * @end-sly2-annot */
extern "C" void func_001D08D8(OBJ *p)
{
    VECTOR v;
    float x, y, z;
    register float lo __asm__("$f20")=-1.0f;
    func_001CE9B8(p);
    register float hi __asm__("$f21")=1.0f;
    x = func_0011D448(lo,hi);
    y = func_0011D448(lo,hi);
    z = func_0011D448(0.1f, 0.9f);
    v.x = x;
    v.y = y;
    v.z = z + 0.1f;
    p->x40 = MakeBasis(*(qword *)&v);
}
