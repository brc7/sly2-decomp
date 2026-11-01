typedef struct CRef { int m_tagk; union { int n; float g; } u; } CRef;
int func_0011D4D0(float a, float b, float c);
CRef *RefOpAreNear__FiP4CRefP6CFrame(CRef *ret, void *pbasic, CRef *aref)
{
    float ag[3];
    int i;
    int f;
    for (i = 0; i < 3; i++) {
        CRef *p = &aref[i];
        if (p->m_tagk == 0x15) ag[i] = p->u.n; else ag[i] = p->u.g;
    }
    f = func_0011D4D0(ag[0], ag[1], ag[2]) != 0;
    ret->m_tagk = 0x17;
    ret->u.n = f;
    return ret;
}
