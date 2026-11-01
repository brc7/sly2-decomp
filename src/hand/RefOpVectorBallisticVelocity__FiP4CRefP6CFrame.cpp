typedef int qword __attribute__((mode(TI)));
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
union VQ { struct { float x, y, z, w; } v; qword q; };
struct XFM { char pad[0x80]; qword pos; };
struct O { char pad[0xE8]; XFM *pxfm; };
struct W { char pad[0xC]; O *po; };
extern "C" {
CRef CRef_RefCoerceF32(CRef *pref);
qword func_001D6848(qword a, qword b, qword c, float g);
CRef RefOpVectorBallisticVelocity__FiP4CRefP6CFrame(void *pbasic, CRef *aref);
}
/** @sly2-annot addr=0x122BE0
 * Purpose: Splice builtin "vector-ballistic-velocity": calls FCalculateMuzzleVelocity-like 0x1D6848 with
 *   gravity (0,0,-1750) or object xfm
 * Info: proposed name RefOpVectorBallisticVelocity (T1); TU bif
 * Confidence: high [T1 0.93] - evidence: static analysis; sources: r1_116918
 * See: docs/reference/functions/splice-scripting.md#bif-0x11f440-0x1258f0
 * @end-sly2-annot */
CRef RefOpVectorBallisticVelocity__FiP4CRefP6CFrame(void *pbasic, CRef *aref)
{
    VQ v;
    v.v.x = 0.0f;
    v.v.y = 0.0f;
    v.v.z = -1750.0f;
    if (aref[3].m_tagk) {
        O *po = ((W *)aref[3].u.pv)->po;
        if (po && po->pxfm)
            v.q = po->pxfm->pos;
    }
    CRef ref(0x1A);
    qword *pq = (qword *)ref.u.pv;
    *pq = func_001D6848(*(qword *)aref[0].u.pv, *(qword *)aref[1].u.pv, v.q, CRef_RefCoerceF32(&aref[2]).u.g);
    return ref;
}
