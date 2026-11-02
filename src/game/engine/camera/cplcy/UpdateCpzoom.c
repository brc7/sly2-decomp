typedef int qword __attribute__((mode(TI)));
typedef struct { qword r[3]; } M3;
typedef union { struct { qword r[4]; } q; M3 m3; float af[4][4]; } M4;
struct CLOCK { float t; float dt; };
extern struct CLOCK D_002DDED0;
struct CM;
extern struct CM D_002DDF40;
extern float D_003E1BE0;
extern char D_002BD9B8[], D_002BD9C8[], D_002BD9D8[], D_002BD9E8[], D_002BD9F8[];
extern char D_002BBB60[], D_002BBB68[];
struct SND { char pad[0x4E]; short c4E; };
struct X {
    char pad0[0x18]; float g18; char pad1[0x20 - 0x1C];
    qword q20; qword q30; void *p40; char pad2[0x50 - 0x44];
    qword q50; qword q60; float g70; char pad3[0x80 - 0x74];
    qword q80; qword q90; float gA0; float gA4; float gA8; float gAC; float gB0; float gB4; float gB8; float gBC;
    struct SND *pC0;
};
void UpdateCplcy(struct X *p);
float GSmooth__FfffP3SMPPf(void *p, float *pdg, float g, float gTarget, float dt);
float func_0011D948(void *p, float g);
void SetSndPitch(struct SND *p, float g);
void func_001FB1F8(struct SND *p, float g);
void ResumeSnd(struct SND *p);
void PauseSnd(struct SND *p);
qword func_0011F088(qword a, qword b, float g, void *p, int n);
void LoadRotateMatrixEuler(M3 *pm, qword v);
void func_0011C950(M3 *a, M3 *b, void *p, M3 *out, int n, float g);
qword func_0011BFF0(M3 *pm);
float func_0011D2D0(void *p, int n, float a, float b, float c);
qword *VECTOR_opAssign(qword *p, qword v);
void SetCmMat4(struct CM *p, M4 *m);
void SetCmFov__FP2CMf(struct CM *p, float g);
void func_0011BC88(M4 *pm, float *pa, float *pb);
qword PosCpzoomEye(struct X *p);
qword PosCpzoomCenter(struct X *p);
void func_0014E1A8(struct CM *p, qword a, qword b, float c, float d);

static __inline__ float GClamp(float g, float gMin, float gMax)
{
    if (g < gMin) return gMin;
    if (gMax < g) return gMax;
    return g;
}

static __inline__ float GFrac(float g, float a, float b)
{
    if (a == b) return 0.0f;
    return (g - a) / (b - a);
}

/** @sly2-annot addr=0x150A28
 * Purpose: Smooth uZoom toward uZoomTarget with sounds; if posed (fPose +0x40) blend pos/eul/fov to target
 *   pose, else smooth center/eye and SetCmLookAt
 * Info: proposed name UpdateCpzoom (T1); TU cplcy
 * Notes: runtime6 C7: CPZOOM = recon camera (S57)
 * Confidence: high [T1 0.85] - evidence: runtime (save-state RAM); semantic review; sources:
 *   r2_131F70+runtime
 * See: docs/reference/functions/camera.md#cplcy-0x14fca8-0x1520c8
 * @end-sly2-annot */
void UpdateCpzoom(struct X *p)
{
    UpdateCplcy(p);
    if (D_002DDED0.dt == 0.0f)
        return;
    if (0.0f <= p->g18) {
        float g = GClamp(GSmooth__FfffP3SMPPf(D_002BD9B8, 0, p->gAC, p->g18, D_002DDED0.dt), 0.0f, 1.0f);
        p->gB0 = (g - p->gAC) / D_002DDED0.dt;
        p->gAC = g;
    }
    if (p->gB0 != 0.0f) {
        struct SND *ps = p->pC0;
        SetSndPitch(ps, func_0011D948(D_002BBB68, __builtin_fabsf(p->gB0)));
        ps = p->pC0;
        func_001FB1F8(ps, func_0011D948(D_002BBB60, __builtin_fabsf(p->gB0) * 0.5f + 0.5f));
        if (p->pC0->c4E > 0)
            ResumeSnd(p->pC0);
    } else {
        if (p->pC0->c4E <= 0)
            PauseSnd(p->pC0);
    }
    if (p->p40) {
        M3 m0, m30, m60, m90;
        M4 m4;
        float u;
        p->q50 = func_0011F088(p->q50, p->q80, D_002DDED0.dt, D_002BD9C8, 0);
        LoadRotateMatrixEuler(&m90, p->q60);
        m0 = m90;
        LoadRotateMatrixEuler(&m90, p->q90);
        m30 = m90;
        func_0011C950(&m0, &m30, D_002BD9D8, &m60, 0, D_002DDED0.dt);
        p->q60 = func_0011BFF0(&m60);
        p->g70 = func_0011D2D0(D_002BD9E8, 0, p->g70, p->gA0, D_002DDED0.dt);
        {
        M4 *pm4 = &m4;
        VECTOR_opAssign(&pm4->q.r[3], p->q50);
        pm4->q.r[0] = m60.r[0];
        pm4->q.r[1] = m60.r[1];
        pm4->q.r[2] = m60.r[2];
        m4.af[2][3] = 0.0f;
        m4.af[1][3] = 0.0f;
        m4.af[0][3] = 0.0f;
        pm4->af[3][3] = 1.0f;
        SetCmMat4(&D_002DDF40, pm4);
        SetCmFov__FP2CMf(&D_002DDF40, p->g70);
        func_0011BC88(pm4, &p->gA4, &p->gA8);
        }
        p->gAC = func_0011D948(D_002BBB60, GFrac(p->g70, p->gB8, p->gBC));
    } else {
        float u = 1.0f - p->gAC;
        SetCmFov__FP2CMf(&D_002DDF40, (1.0f - u) * p->gB8 + u * p->gBC);
        {
            qword a = p->q30;
            p->q30 = func_0011F088(a, PosCpzoomEye(p), D_002DDED0.dt, D_002BD9F8, 0);
        }
        {
            qword a = p->q20;
            p->q20 = func_0011F088(a, PosCpzoomCenter(p), D_002DDED0.dt, D_002BD9F8, 0);
        }
        func_0014E1A8(&D_002DDF40, p->q30, p->q20, 0.0f, 0.0f);
    }
    D_003E1BE0 = D_002DDED0.t;
}
