typedef int qword __attribute__((mode(TI)));
struct VU_FLOAT { qword q; VU_FLOAT(float f) { __asm__("qmtc2.ni %1, %0" : "=j"(q) : "r"(f)); } };
struct M3 { qword r[3]; };
struct S { char pad[0x218]; qword *origin; int pad21C; float radius; };
extern "C" {
M3 *func_001A6318(S *p);
qword func_001A62E8(S *p);
extern int D_002C3CC8[2][3];
}
static inline qword MulX(qword v, VU_FLOAT g) { qword r; __asm__("vmulx.xyzw %0, %1, %2x" : "=j"(r) : "j"(v), "j"(g.q)); return r; }
static inline qword Add(qword a, qword b) { qword r; __asm__("vadd.xyzw %0, %1, %2" : "=j"(r) : "j"(a), "j"(b)); return r; }
static inline qword LoadVf(qword *p) { qword r; __asm__("lqc2 %0, %1" : "=j"(r) : "m"(*p)); return r; }
/** @sly2-annot addr=0x2057E8
 * Purpose: Build 2 triangles (diamond of radius +0x220 around the base point +0x218, oriented by the world
 *   matrix) from table D_002C3CC8
 * Info: proposed name BuildStepFootprint (T2); TU step
 * Confidence: low (draft) [T2 0.30] - evidence: static analysis; sources: r7b_guard
 * See: docs/reference/functions/guards-ai.md#step-0x203948-0x205e70
 * @end-sly2-annot */
extern "C" void func_002057E8(S *p, qword out[2][3])
{
    qword point[5];
    qword origin = *p->origin;
    qword right = func_001A6318(p)->r[0];
    qword forward = LoadVf(&func_001A6318(p)->r[1]);
    point[0] = func_001A62E8(p);
    float radius = p->radius;
    point[1] = Add(origin, MulX(right, VU_FLOAT(radius)));
    point[2] = Add(origin, MulX(forward, VU_FLOAT(radius)));
    point[3] = Add(origin, MulX(right, VU_FLOAT(-radius)));
    point[4] = Add(origin, MulX(forward, VU_FLOAT(-radius)));
    for (unsigned i = 0; i < 2; i++) {
        int *indices = D_002C3CC8[i];
        qword *dest = out[i];
        for (unsigned j = 0; j < 3; j++)
            dest[j] = *(qword *)((indices[j] << 4) + (unsigned int)point);
    }
}
