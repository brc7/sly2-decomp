typedef int qword __attribute__((mode(TI)));
struct MATRIX4 { qword r[4]; };
struct VU_FLOAT { qword q; };
static inline qword Mulx(qword a, qword f) { qword r; __asm__("vmulx.xyzw %0, %2, %1x" : "=j"(r) : "j"(f), "j"(a)); return r; }
MATRIX4 operator*(const MATRIX4 &m, VU_FLOAT s) return mat
{
    mat.r[0] = Mulx(m.r[0], s.q);
    mat.r[1] = Mulx(m.r[1], s.q);
    mat.r[2] = Mulx(m.r[2], s.q);
    mat.r[3] = Mulx(m.r[3], s.q);
}
