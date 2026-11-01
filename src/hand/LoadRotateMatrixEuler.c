typedef struct { float x, y, z, w; } __attribute__((aligned(16))) VECTOR;
typedef int qword __attribute__((mode(TI)));
static __inline__ float GetX(qword q) { float x; __asm__("mtc1 %1,%0" : "=f"(x) : "r"(q)); return x; }
static __inline__ float GetY(qword q) { float y; __asm__("prot3w %0,%1" : "=r"(y) : "r"(q)); return y; }
static __inline__ float GetZ(qword q) { float z; __asm__("pextuw %0,$0,%1" : "=r"(z) : "r"(q)); return z; }
typedef struct { VECTOR r[3]; } MATRIX3;
void CalculateSinCos__FfPfT1(float g, float *ps, float *pc);
MATRIX3 *LoadRotateMatrixEuler(MATRIX3 *pmat, qword v)
{
    float sx, cx, sy, cy, sz, cz;
    float sxsy, cxsy;
    CalculateSinCos__FfPfT1(GetX(v), &sx, &cx);
    CalculateSinCos__FfPfT1(GetY(v), &sy, &cy);
    CalculateSinCos__FfPfT1(GetZ(v), &sz, &cz);
    sxsy = sx * sy;
    cxsy = cx * sy;
    pmat->r[0].z = -sy;
    pmat->r[0].x = cy * cz;
    pmat->r[0].y = cy * sz;
    pmat->r[1].x = sxsy * cz - cx * sz;
    pmat->r[1].y = sxsy * sz + cx * cz;
    pmat->r[1].z = sx * cy;
    pmat->r[2].x = cxsy * cz + sx * sz;
    pmat->r[2].y = cxsy * sz - sx * cz;
    pmat->r[2].z = cx * cy;
    return pmat;
}
