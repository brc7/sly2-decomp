typedef int qword __attribute__((mode(TI)));
typedef struct { qword r[4]; } M4;
struct CM { char pad[0x40]; float sx, sy; char pad2[0x18]; M4 mat; };
static __inline__ float GetX(qword q) { float x; __asm__("mtc1 %1,%0" : "=f"(x) : "r"(q)); return x; }
static __inline__ float GetY(qword q) { float y; __asm__("prot3w %0,%1" : "=r"(y) : "r"(q)); return y; }
static __inline__ float GetZ(qword q) { float z; __asm__("pextuw %0,$0,%1" : "=r"(z) : "r"(q)); return z; }
static __inline__ qword Mul(M4 *pm, qword v)
{
    qword o;
    __asm__("vmulax.xyz $ACC, %2, %1x\n\tvmadday.xyz $ACC, %3, %1y\n\tvmaddaz.xyz $ACC, %4, %1z\n\tvmaddw.xyz %0, %5, $vf0w"
            : "=j"(o) : "j"(v), "j"(pm->r[0]), "j"(pm->r[1]), "j"(pm->r[2]), "j"(pm->r[3]));
    return o;
}
/** @sly2-annot addr=0x14F268
 * Purpose: Screen (x,y,depth) -> world: axis swap, scale by tanHalfX/Y (+0x40/+0x44), multiply by CM::mat
 * Info: proposed name ConvertCmScreenToWorld (T1); TU cm
 * Confidence: high [T1 0.85] - evidence: semantic review; sources: r2_131F70
 * See: docs/engine/camera.md#5-key-functions, docs/reference/functions/camera.md#cm-0x14d3d8-0x14fac8
 * @end-sly2-annot */
qword ConvertCmScreenToWorld(struct CM *p, qword v)
{
    union { qword q; float f[4]; } r;
    float x = GetX(v);
    float z = GetZ(v);
    float y = GetY(v);
    r.f[2] = y;
    r.f[0] = z;
    r.f[1] = -x;
    if (z > 1e-4f) {
        r.f[1] = -x * z;
        r.f[2] = y * z;
    }
    r.f[1] *= p->sx;
    r.f[2] *= p->sy;
    return Mul(&p->mat, r.q);
}
