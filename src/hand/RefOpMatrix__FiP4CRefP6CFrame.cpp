typedef int qword __attribute__((mode(TI)));
struct MATRIX4 { float m[4][4]; };
struct CRef {
    int m_tagk; union { int n; float g; void *pv; } u;
    CRef() { m_tagk = -1; }
    CRef(int tagk) { InitTag(tagk); }
    CRef(const CRef &r);
    void Decref();
    void InitTag(int tagk);
    ~CRef() { Decref(); }
    CRef &operator=(const CRef &r);
};
struct NODE { int x0, x4; CRef ref; NODE *pnext; };
extern "C" {
void *func_002333B4(void *pv, int b, unsigned int cb);
CRef CRef_RefCoerceF32(CRef *pref);
CRef RefOpMatrix__FiP4CRefP6CFrame(int carg, CRef *pref);
}
CRef RefOpMatrix__FiP4CRefP6CFrame(int carg, CRef *pref)
{
    CRef ref(0x1B);
    MATRIX4 *pm = (MATRIX4 *)ref.u.pv;
    NODE *pn;
    if (carg > 0)
        pn = (NODE *)pref->u.pv;
    func_002333B4(pm, 0, sizeof(*pm));
    pm->m[0][0] = pm->m[1][1] = pm->m[2][2] = pm->m[3][3] = 1.0f;
    switch (carg) {
    case 0:
        break;
    case 1:
        pm->m[0][0] = pm->m[1][1] = pm->m[2][2] = CRef_RefCoerceF32(&pn->ref).u.g;
        break;
    case 3:
    case 4: {
        int i;
        for (i = 0; i < carg; i++) {
            *(qword *)pm->m[i] = *(qword *)pn->ref.u.pv;
            pn = pn->pnext;
        }
        break;
    }
    case 9:
    case 16: {
        int n = (carg == 9) ? 3 : 4;
        int i, j;
        for (i = 0; i < n; i++) {
            for (j = 0; j < n; j++) {
                CRef r = CRef_RefCoerceF32(&pn->ref);
                pm->m[i][j] = r.u.g;
                pn = pn->pnext;
            }
        }
        break;
    }
    }
    return ref;
}
