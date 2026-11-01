typedef struct { float x, y, z, w; } __attribute__((aligned(16))) VECTOR;
typedef int qword __attribute__((mode(TI)));
static __inline__ float GetX(qword q) { float x; __asm__("mtc1 %1,%0" : "=f"(x) : "r"(q)); return x; }
static __inline__ float GetY(qword q) { float y; __asm__("prot3w %0,%1" : "=r"(y) : "r"(q)); return y; }
static __inline__ float GetZ(qword q) { float z; __asm__("pextuw %0,$0,%1" : "=r"(z) : "r"(q)); return z; }
typedef struct { VECTOR r[3]; } MATRIX3;
void func_0011D608(float g, float *ps, float *pc);
/** @sly2-annot addr=0x11B628
 * Purpose: Rotation matrix from Euler angles (sx,cx,sy,cy,sz,cz products)
 * Info: proposed name LoadRotateMatrixEuler (T1); TU mat
 * Notes: names_v2 C proposed LoadRotateMatrixRad__FfP6VECTORP7MATRIX3 (0.638)
 * Confidence: high [T1 0.85] - evidence: static naming pass; sources: r1_116918
 * See: docs/reference/functions/camera.md#cm-0x14d3d8-0x14fac8,
 *   docs/reference/functions/core-library.md#mat-0x11b300-0x11cca8,
 *   docs/reference/functions/object-model.md#lo-0x1a5270-0x1a7518
 * @end-sly2-annot */
MATRIX3 *func_0011B628(MATRIX3 *pmat, qword v)
{
    float sx, cx, sy, cy, sz, cz;
    float sxsy, cxsy;
    func_0011D608(GetX(v), &sx, &cx);
    func_0011D608(GetY(v), &sy, &cy);
    func_0011D608(GetZ(v), &sz, &cz);
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
