// func_00151700 (Claude agent): camera path keyframe lerp. VU_FLOAT is a qmtc2 asm fed by a gcc mfc1, and Dot ends
// with separate qmfc2/mtc1 asms, so the float t lives only in an FPR and sub.s schedules like the original.
extern "C" {
typedef int qword __attribute__((mode(TI)));
typedef union { qword q; float f[4]; } U;
typedef struct { qword r[3]; } M3;
typedef union { struct { qword r[4]; } q; M3 m3; float af[4][4]; } M4;
struct CLOCK { float t; float dt; };
extern struct CLOCK D_002DDED0;
struct CM { char pad[0x390]; float g390; };
extern struct CM D_002DDF40;
extern qword D_002BDA70[];
struct K { qword pos; qword rot; float fov; };
struct PATH { struct K *pk0; struct K *pk1; float dt; int pad; int ik; };
struct X { char pad[0x10]; struct PATH *ppath; };
void func_0014FD40(struct X *p);
void LoadRotateMatrixEuler(M3 *pm, qword v);
qword *func_00256D18(qword *p, qword v);
void func_0014E168(struct CM *p, M4 *m);
void func_0014F5A0(struct CM *p, float g);

static __inline__ float GClamp(float g, float gMin, float gMax)
{
    if (g < gMin) return gMin;
    if (gMax < g) return gMax;
    return g;
}

static __inline__ float Dot(qword a, qword b)
{
    qword d, one;
    float g;
    __asm__("vmul.xyz %0, %2, %3\n\tvaddw.x %1, $vf0, $vf0w\n\tvadday.x $ACC, %0, %0y\n\tvmaddz.x %0, %1, %0z"
            : "=j"(d), "=&j"(one) : "j"(a), "j"(b));
    int r;
    __asm__("qmfc2.ni %0, %1" : "=r"(r) : "j"(d));
    __asm__("mtc1 %1, %0" : "=f"(g) : "r"(r));
    return g;
}

struct VU_FLOAT { qword q; VU_FLOAT(float f) { __asm__("qmtc2.ni %1, %0" : "=j"(q) : "r"(f)); } };
struct VU_VECTOR { qword q; VU_VECTOR(const qword &v) { q = v; } };
static inline qword VecCombo(VU_FLOAT f1, VU_VECTOR v1, VU_FLOAT f2, VU_VECTOR v2)
{
    qword r;
    __asm__("vmulax.xyzw $ACC, %2, %1x\n\tvmaddx.xyzw %0, %4, %3x" : "=j"(r) : "j"(f1.q), "j"(v1.q), "j"(f2.q), "j"(v2.q));
    return r;
}

}
/** @sly2-annot addr=0x151700
 * Purpose: t=(clock.t - tModeStart)/dt; curve weight dot((t^3,t^2,t), D_002BDA70[iCurve]); lerp pos/eul/fov
 *   between the two CAMPOSEs; SetCmMat4 + SetCmFov
 * Info: proposed name UpdateCpcamblend (T1); TU cplcy
 * Notes: runtime6 C7: CPCAMBLEND is the cutscene blend camera (S59)
 * Confidence: high [T1 0.85] - evidence: runtime (save-state RAM); semantic review; sources:
 *   r2_131F70+runtime
 * See: docs/engine/camera.md#open-questions, docs/reference/functions/camera.md#cplcy-0x14fca8-0x1520c8
 * @end-sly2-annot */
extern "C" void func_00151700(struct X *p)
{
    float u, t, t1, fov;
    U pos, rot;
    M4 m4;
    U c;
    struct PATH *ppath;
    func_0014FD40(p);
    u = D_002DDED0.t - D_002DDF40.g390;
    t = GClamp(u / p->ppath->dt, 0.0f, 1.0f);
    u = t;
    c.f[1] = u * u;
    c.f[0] = u * u * u;
    c.f[2] = u;
    ppath = p->ppath;
    t = Dot(c.q, D_002BDA70[ppath->ik]);
    t1 = 1.0f - t;
    pos.q = VecCombo(VU_FLOAT(t1), VU_VECTOR(ppath->pk0->pos), VU_FLOAT(t), VU_VECTOR(ppath->pk1->pos));
    rot.q = VecCombo(VU_FLOAT(t1), VU_VECTOR(ppath->pk0->rot), VU_FLOAT(t), VU_VECTOR(ppath->pk1->rot));
    fov = t1 * ppath->pk0->fov + t * ppath->pk1->fov;
    {
        M3 m3;
        M4 *pm4;
        LoadRotateMatrixEuler(&m3, rot.q);
        pm4 = &m4;
        func_00256D18(&pm4->q.r[3], pos.q);
        pm4->q.r[0] = m3.r[0];
        pm4->q.r[1] = m3.r[1];
        pm4->q.r[2] = m3.r[2];
        m4.af[2][3] = 0.0f;
        m4.af[1][3] = 0.0f;
        m4.af[0][3] = 0.0f;
        pm4->af[3][3] = 1.0f;
        func_0014E168(&D_002DDF40, pm4);
    }
    func_0014F5A0(&D_002DDF40, fov);
}
