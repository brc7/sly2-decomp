struct CRef {
    int m_tagk; union { int n; float g; void *pv; } u;
    CRef() { m_tagk = -1; }
    CRef(int tagk) { InitTag(tagk); }
    CRef(const CRef &r);
    void Decref();
    void InitTag(int tagk);
    ~CRef() { Decref(); }
    CRef &operator=(const CRef &r);
    CRef &operator=(float g);
};
struct VECTOR { float x, y, z; };
struct NODE { int cref; int x4; CRef ref; NODE *pnext; };
extern "C" {
CRef CRef_RefCoerceF32(CRef *pref);
CRef RefOpVector__FiP4CRefP6CFrame(int c, CRef *pref);
}
/** @sly2-annot addr=0x121DD8
 * Purpose: Splice builtin "vector": 0/1/3 args -> VECTOR (Sly2 made it varargs)
 * Info: proposed name RefOpVector (T1); TU bif
 * Notes: names_v2 B proposed RefOpSmp__FiP4CRefP6CFrame (0.967)
 * Confidence: high [T1 0.85] - evidence: static naming pass; sources: r1_116918
 * See: docs/reference/functions/splice-scripting.md#bif-0x11f440-0x1258f0
 * @end-sly2-annot */
CRef RefOpVector__FiP4CRefP6CFrame(int c, CRef *pref)
{
    CRef ref(0x1A);
    VECTOR *pv = (VECTOR *)ref.u.pv;
    NODE *pn;
    if (c > 0)
        pn = (NODE *)pref->u.pv;
    switch (c) {
    case 0:
        pv->z = 0.0f;
        pv->y = 0.0f;
        pv->x = 0.0f;
        break;
    case 1: {
        CRef r = CRef_RefCoerceF32(&pn->ref);
        float g = r.u.g;
        pv->z = g;
        pv->y = g;
        pv->x = g;
        break;
    }
    case 3: {
        float *pg = &pv->x;
        int i;
        for (i = 0; i < 3; i++) {
            CRef r = CRef_RefCoerceF32(&pn->ref);
            *pg = r.u.g;
            pg++;
            pn = pn->pnext;
        }
        break;
    }
    }
    return ref;
}
