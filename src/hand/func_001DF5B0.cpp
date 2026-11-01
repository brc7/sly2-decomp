// func_001DF5B0 (w2lane4, wave 2): per-frame render of a "connector" (a chain of links drawn between pairs of points).
// After the generic update (func_001F3C70) and a camera visibility test of the connector's bounding sphere
// (func_0014F850), each link is queried for its two end points and its length (func_001DEF98). A link longer than its
// maximum is pulled back toward the first point, then the line object, the stretched middle object and the two end caps
// (func_001A5FB8 / func_001A5EE8) are placed along the segment, in a basis built from the segment direction (BuildOrthonormalMatrixZ).
// Matching notes (all found by reading the sched1/local-alloc/reload dumps):
//  * The radius temporary VU_FLOAT(const float *, int) is a volatile empty asm (word load into a 128-bit GPR value); it acts as the
//    scheduler barrier that keeps the camera/bound loads before the call arguments, which is what puts the camera address
//    straight into $a0 and the bound pointer in $v1.
//  * D_002A2DC0 must NOT be const: a const global would be an unchanging load that sched2 moves above the spill store of `dir`.
//  * `dir` is a plain qword that lives across BuildOrthonormalMatrixZ, so it is spilled (store right after the vsub, reload for the
//    argument and for Length). Length lists its input twice (as in the other matched Length/Dot helpers), which decides the
//    reload register.
//  * BuildOrthonormalMatrixZ / LoadMatrixFromPosRotScale return M3 / M4 by value (hidden result pointer), assigned to locals.
//  * `matrix` is declared at link-loop scope and the facing branch writes rows 1/2 and the corner through a pointer
//    (pm), rows 0 and the w words directly: that is the mixed sp / s2 addressing of the original.
typedef int qword __attribute__((mode(TI)));
union U { qword q; float f[4]; int bits[4]; };
struct M3 { U rows[3]; };
struct M4 { U rows[4]; };
struct ENTRY { char pad0[0x144]; float maximumLength; char pad148[8]; };
struct CONFIG { char pad0[0x1C]; float first; char pad20[0x20]; float second; };
struct VENTRY { short delta, pad; void (*fn)(void *, U *, int, float, float); };
struct HOOK { VENTRY *vt; };
struct ENDPOINT { void *object; int unused4; CONFIG *config; HOOK *scale; };
struct BOUND { char pad0[0x20]; U position; float radius; };
struct P {
    char pad0[0xEC]; BOUND *bound; char padF0[0xA0];
    void *line, *stretch; int count; ENTRY *entries; ENDPOINT endpoints[2];
};
struct CAMERA { char pad0[0x90]; U position; };
extern CAMERA D_002DDF40;
extern qword D_002A2DC0;
struct VU_FLOAT {
    qword q;
    VU_FLOAT(float f) { __asm__("qmtc2.ni %1,%0" : "=j"(q) : "r"(f)); }
    /* barrier: loads the float's word into a 128-bit GPR value without any instruction of its own */
    VU_FLOAT(const float *p, int) { __asm__ __volatile__("" : "=r"(q) : "0"(*(const int *)p)); }
};
extern "C" {
void func_001F3C70(P *);
int func_0014F850(CAMERA *, qword, VU_FLOAT);
void func_001DEF98(P *, ENTRY *, qword *, float *);
void func_001A5FB8(void *, qword, qword, float, float);
M3 BuildOrthonormalMatrixZ(qword, qword);
M4 LoadMatrixFromPosRotScale(qword, const M3 *, const U *);
qword *func_00256D18(qword *, qword);
void func_001A5EE8(void *, const M4 *, float, float);
}
static inline qword Subtract(qword a, qword b)
{
    qword r;
    __asm__("vsub.xyzw %0,%1,%2" : "=j"(r) : "j"(a), "j"(b));
    return r;
}
static inline qword Add(qword a, qword b)
{
    qword r;
    __asm__("vadd.xyzw %0,%1,%2" : "=j"(r) : "j"(a), "j"(b));
    return r;
}
static inline qword Scale(qword a, VU_FLOAT b)
{
    qword r;
    __asm__("vmulx.xyzw %0,%1,%2x" : "=j"(r) : "j"(a), "j"(b.q));
    return r;
}
static inline float Length(qword value)
{
    qword d, one;
    float r;
    __asm__("vmul.xyz %0,%2,%3\n\tvaddw.x %1,$vf0,$vf0w\n\tvadday.x ACC,%0,%0y\n\tvmaddz.x %0,%1,%0z"
            : "=j"(d), "=&j"(one) : "j"(value), "j"(value));
    __asm__("vsqrt Q,%0x\n\tvwaitq\n\tvaddq.x %0,$vf0,Q" : "+j"(d));
    __asm__("mtc1 %1,%0" : "=f"(r) : "r"(d));
    return r;
}
static inline void Query(P *owner, ENTRY *entry, U (&points)[2], float &distance)
{
    func_001DEF98(owner, entry, &points[0].q, &distance);
}

extern "C" void func_001DF5B0(P *p)
{
    func_001F3C70(p);
    BOUND *bound = p->bound;
    if (!bound) return;
    CAMERA *cam = &D_002DDF40;
    qword boundPosition = bound->position.q;
    VU_FLOAT radius(&bound->radius, 0);
    qword relativePosition = Subtract(boundPosition, cam->position.q);
    if (!func_0014F850(cam, relativePosition, radius)) return;
    U points[2];
    M3 basis;
    float dist;
    for (int i = 0; i < p->count;) {
        ENTRY *entry = &p->entries[i];
        Query(p, entry, points, dist);
        if (entry->maximumLength < dist) {
            float ratio = entry->maximumLength / dist;
            points[1].q = Add(Scale(points[0].q, VU_FLOAT(1.0f - ratio)), Scale(points[1].q, VU_FLOAT(ratio)));
        }
        if (p->line) func_001A5FB8(p->line, points[0].q, points[1].q, 1.0f, 1.0f);
        qword dir;
        if (p->stretch || p->endpoints[0].object || p->endpoints[1].object) {
            dir = Subtract(points[1].q, points[0].q);
            basis = BuildOrthonormalMatrixZ(dir, D_002A2DC0);
        }
        if (p->stretch) {
            float length = Length(dir);
            if (0.01f < length) {
                U scale;
                scale.f[0] = length * 0.01f;
                scale.f[1] = 1.0f;
                scale.f[2] = 1.0f;
                M4 render;
                render = LoadMatrixFromPosRotScale(points[0].q, &basis, &scale);
                func_001A5EE8(p->stretch, &render, 1.0f, 1.0f);
            }
        }
        int next = i + 1;
        M4 matrix;
        for (unsigned int j = 0; j < 2; j++) {
            ENDPOINT *end = &p->endpoints[j];
            if (end->object) {
                if (end->scale) {
                    U scale;
                    HOOK *hook = end->scale;
                    VENTRY &callback = hook->vt[3];
                    callback.fn((char *)hook + callback.delta, &scale, 0, end->config->first, end->config->second);
                    matrix = LoadMatrixFromPosRotScale(points[j].q, &basis, &scale);
                } else {
                    M4 *pm = &matrix;
                    func_00256D18(&matrix.rows[3].q, points[j].q);
                    matrix.rows[0] = basis.rows[0]; pm->rows[1] = basis.rows[1]; pm->rows[2] = basis.rows[2];
                    matrix.rows[0].bits[3] = matrix.rows[1].bits[3] = matrix.rows[2].bits[3] = 0;
                    pm->rows[3].f[3] = 1.0f;
                }
                func_001A5EE8(end->object, &matrix, 1.0f, 1.0f);
            }
        }
        i = next;
    }
}
