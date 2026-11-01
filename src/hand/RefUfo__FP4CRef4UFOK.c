typedef struct CRef { int m_tagk; int m_tag; } CRef;
typedef float (*PFNG)(float);
extern PFNG D_002BBB70[];
void SetF32__4CReff(CRef *pref, float g);
CRef *func_001291A8(CRef *pref, CRef *prefSrc);
void func_00129EC8(CRef *pref);
CRef *RefUfo__FP4CRef4UFOK(CRef *ret, CRef *parg, int i)
{
    CRef ref;
    float g;
    ref.m_tagk = -1;
    if (parg->m_tagk == 0x16) {
        g = *(float *)&parg->m_tag;
    } else {
        g = (float)parg->m_tag;
    }
    SetF32__4CReff(&ref, D_002BBB70[i](g));
    func_001291A8(ret, &ref);
    func_00129EC8(&ref);
    return ret;
}
