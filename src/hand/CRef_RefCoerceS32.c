typedef struct CRef { int m_tagk; union { int n; float g; } u; } CRef;
CRef *__4CRefRC4CRef(CRef *pref, CRef *prefSrc);
CRef *CRef_RefCoerceS32(CRef *pref, CRef *prefSrc)
{
    if (prefSrc->m_tagk == 0x15) {
        __4CRefRC4CRef(pref, prefSrc);
    } else {
        float g = prefSrc->u.g;
        pref->m_tagk = 0x15;
        pref->u.n = (int)g;
    }
    return pref;
}
