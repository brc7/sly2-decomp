typedef int qword __attribute__((mode(TI)));
typedef struct { float m[4][4]; } M4F;
typedef struct { qword r[4]; } M4;
void *func_002333B4(void *p, int c, int cb);
void InvertMatrix4Rigid(M4F *pmOut, M4F *pm);
void __ml__FRC7MATRIX4T0(M4 *pmOut, M4F *pa, M4F *pb);
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
        InvertMatrix4Rigid(&t, &m);
        __ml__FRC7MATRIX4T0(pr, pproj, &t);
    }
    *pmOut = r;
}
