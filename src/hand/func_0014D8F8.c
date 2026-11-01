typedef int qword __attribute__((mode(TI)));
typedef struct { float m[4][4]; } M4F;
typedef struct { qword r[4]; } M4;
void *func_002333B4(void *p, int c, int cb);
void func_0011C318(M4F *pmOut, M4F *pm);
void func_0011C168(M4 *pmOut, M4F *pa, M4F *pb);
/** @sly2-annot addr=0x14D8F8
 * Purpose: matViewProj = proj * inverse(eye matrix with game->camera axis swap: x=-y, y=-z, z=x)
 * Info: proposed name CombineEyeLookAtProj (T1); TU cm
 * Confidence: high [T1 0.85] - evidence: semantic review; sources: r2_131F70
 * See: docs/engine/camera.md#5-key-functions, docs/reference/functions/camera.md#cm-0x14d3d8-0x14fac8
 * @end-sly2-annot */
void func_0014D8F8(M4F *pm, M4F *pproj, M4 *pmOut)
{
    M4F m;
    M4 r;
    int i;
    func_002333B4(&m, 0, sizeof(m));
    m.m[3][3] = 1.0f;
    m.m[2][2] = 1.0f;
    m.m[1][1] = 1.0f;
    m.m[0][0] = 1.0f;
    for (i = 0; i < 3; i++) {
        m.m[0][i] = -pm->m[1][i];
        m.m[1][i] = -pm->m[2][i];
        m.m[2][i] = pm->m[0][i];
        m.m[3][i] = pm->m[3][i];
    }
    {
        M4F t;
        M4 *pr = &r;
        func_0011C318(&t, &m);
        func_0011C168(pr, pproj, &t);
    }
    *pmOut = r;
}
