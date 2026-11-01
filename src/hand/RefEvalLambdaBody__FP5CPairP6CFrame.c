typedef struct CRef { int m_tagk; union { int n; float g; void *pv; } u; } CRef;
typedef struct LO { char pad[0x10]; struct LO *pnext; } LO;
typedef struct BASIC { char pad[0x18]; LO *plo; } BASIC;
void Decref__4CRef(CRef *pref);
CRef *func_001291A8(CRef *pref, CRef *prefSrc);
CRef *func_00129658(CRef *pref, CRef *prefSrc);
CRef *RefEval__FP5CPairP6CFrame(CRef *ret, LO *plo, CRef *aref);
CRef *RefEvalLambdaBody__FP5CPairP6CFrame(CRef *ret, BASIC *pbasic, CRef *aref)
{
    CRef ref;
    CRef refT;
    LO *plo;
    ref.m_tagk = -1;
    plo = pbasic->plo;
    do {
        RefEval__FP5CPairP6CFrame(&refT, plo, aref);
        func_00129658(&ref, &refT);
        Decref__4CRef(&refT);
        plo = plo->pnext;
    } while (plo);
    func_001291A8(ret, &ref);
    Decref__4CRef(&ref);
    return ret;
}
