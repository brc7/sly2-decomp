typedef struct CRef { int m_tagk; union { int n; float g; } u; } CRef;
CRef *RefCmp__FP4CRef4CMPK(CRef *ret, CRef *aref, int op)
{
    float ag[2];
    int i;
    int f;
    for (i = 0; i < 2; i++) {
        if (aref[i].m_tagk == 0x16)
            ag[i] = aref[i].u.g;
        else
            ag[i] = (float)aref[i].u.n;
    }
    switch (op) {
    case 0:
        f = ag[0] < ag[1];
        break;
    case 1:
        f = ag[0] <= ag[1];
        break;
    case 2:
        f = ag[0] > ag[1];
        break;
    case 3:
        f = ag[0] >= ag[1];
        break;
    }
    ret->u.n = f;
    ret->m_tagk = 0x17;
    return ret;
}
