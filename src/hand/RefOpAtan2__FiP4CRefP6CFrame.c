typedef struct CRef { int m_tagk; union { int n; float g; } u; } CRef;
float func_002458F0(float y, float x);
CRef *RefOpAtan2__FiP4CRefP6CFrame(CRef *ret, void *pbasic, CRef *aref)
{
    float ag[2];
    int i;
    float g;
    for (i = 0; i < 2; i++) {
        CRef *p = &aref[i];
        if (p->m_tagk == 0x16) ag[i] = p->u.g; else ag[i] = p->u.n;
    }
    g = func_002458F0(ag[0], ag[1]);
    ret->u.g = g;
    ret->m_tagk = 0x16;
    return ret;
}
