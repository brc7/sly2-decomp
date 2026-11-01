typedef int qword __attribute__((mode(TI)));
static __inline__ float GetX(qword q) { float x; __asm__("mtc1 %1,%0" : "=f"(x) : "r"(q)); return x; }
static __inline__ float GetY(qword q) { float y; __asm__("prot3w %0,%1" : "=r"(y) : "r"(q)); return y; }
static __inline__ float GetZ(qword q) { float z; __asm__("pextuw %0,$0,%1" : "=r"(z) : "r"(q)); return z; }
static __inline__ qword Make(float x, float y, float z)
{
    qword q, t;
    __asm__("mfc1 %0,%2
	mfc1 %1,%3
	pextlw %0,%1,%0
	mfc1 %1,%4
	pcpyld %0,%1,%0"
            : "=&r"(q), "=&r"(t) : "f"(x), "f"(y), "f"(z));
    return q;
}
typedef struct { qword r[3]; } M3;
typedef union { struct { qword r[4]; } q; M3 m3; float af[4][4]; } M4;
struct CM { char pad[0x60]; M4 mat; };
void LoadRotateMatrixEuler(M3 *pm, qword v);
void func_0014E168(struct CM *p, M4 *m);
void func_0014F4E0(struct CM *p, qword v)
{
    M4 mat = p->mat;
    union { qword q; float f; } r;
    M3 m;
    r.q = Make(GetX(v) * 0.017453294f, GetY(v) * 0.017453294f, GetZ(v) * 0.017453294f);
    LoadRotateMatrixEuler(&m, r.q);
    mat.m3 = m;
    mat.af[2][3] = 0.0f;
    mat.af[1][3] = 0.0f;
    mat.af[0][3] = 0.0f;
    func_0014E168(p, &mat);
}
