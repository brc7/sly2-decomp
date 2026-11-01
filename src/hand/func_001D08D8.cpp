// Initialize orientation from three random direction components.
typedef int qword __attribute__((mode(TI)));
typedef struct { float x, y, z; } __attribute__((aligned(16))) VECTOR;
typedef struct { float mat[3][4]; } __attribute__((aligned(16))) MATRIX3;
typedef struct {
    char pad0[0x40];
    MATRIX3 x40;
} OBJ;
extern "C" void func_001CE9B8(OBJ *);
extern "C" float GRandInRange__Fff(float, float);
extern "C" MATRIX3 *LoadLookAtMatrix(MATRIX3 *,qword);
static inline MATRIX3 MakeBasis(qword vector) return r {LoadLookAtMatrix(&r,vector);}
extern "C" void func_001D08D8(OBJ *p)
{
    VECTOR v;
    float x, y, z;
    register float lo __asm__("$f20")=-1.0f;
    func_001CE9B8(p);
    register float hi __asm__("$f21")=1.0f;
    x = GRandInRange__Fff(lo,hi);
    y = GRandInRange__Fff(lo,hi);
    z = GRandInRange__Fff(0.1f, 0.9f);
    v.x = x;
    v.y = y;
    v.z = z + 0.1f;
    p->x40 = MakeBasis(*(qword *)&v);
}
