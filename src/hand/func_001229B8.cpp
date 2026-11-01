typedef int qword __attribute__((mode(TI)));
struct CRef {
    int m_tagk; union { int n; float g; void *pv; qword *pq; } u;
    CRef() { m_tagk = -1; }
    CRef(const qword *pq);
    CRef(const CRef &r);
    void Decref();
    void InitTag(int tagk);
    ~CRef() { Decref(); }
    CRef &operator=(const CRef &r);
};
struct W { char pad[0xC]; qword *pv; };
extern qword D_002A2DD0;
extern "C" CRef func_001229B8(void *pbasic, CRef *aref);
static __inline__ qword VLen(qword c)
{
    qword l, one;
    __asm__("vmul.xyz %0, %2, %2\n\tvaddw.x %1, $vf0, $vf0w\n\tvadday.x ACC, %0, %0y\n\tvmaddz.x %0, %1, %0z" : "=&j"(l), "=&j"(one) : "j"(c));
    __asm__("vsqrt Q, %1x\n\tvwaitq\n\tvaddq.x %0, $vf0, Q" : "=j"(l) : "j"(l));
    return l;
}
static __inline__ void Normalize(qword *pv, qword *pdef)
{
    __asm__("" : "+r"(pdef));
    qword c = *pv;
    qword l = VLen(c);
    float gl;
    __asm__("qmfc2.ni %0, %1" : "=r"(gl) : "j"(l));
    if (gl < 1e-4f) {
        *pv = *pdef;
    } else {
        qword rn;
        __asm__("vdiv Q, $vf0w, %2x\n\tvwaitq\n\tvmulq.xyzw %0, %1, Q" : "=j"(rn) : "j"(c), "j"(l));
        *pv = rn;
    }
}
static __inline__ void Normalize1(qword *pv, qword *pdef)
{
    qword c;
    __asm__("qmtc2.ni %1, %0" : "=j"(c) : "r"(*pv));
    qword l = VLen(c);
    float gl;
    __asm__("qmfc2.ni %0, %1" : "=r"(gl) : "j"(l));
    if (gl < 1e-4f) {
        *pv = *pdef;
    } else {
        qword rn;
        __asm__("vdiv Q, $vf0w, %2x\n\tvwaitq\n\tvmulq.xyzw %0, %1, Q" : "=j"(rn) : "j"(c), "j"(l));
        *pv = rn;
    }
}
/** @sly2-annot addr=0x1229B8
 * Purpose: Splice builtin "vector-normalize": normalize with 1e-4 guard/default
 * Info: proposed name RefOpVectorNormalize (T1); TU bif
 * Confidence: high [T1 0.93] - evidence: static analysis; sources: r1_116918
 * See: docs/reference/functions/splice-scripting.md#bif-0x11f440-0x1258f0
 * @end-sly2-annot */
CRef func_001229B8(void *pbasic, CRef *aref) { CRef ref((qword *)aref[0].u.pv); if (aref[1].m_tagk == 0) Normalize1(ref.u.pq, &D_002A2DD0); else Normalize(ref.u.pq, ((W *)aref[1].u.pv)->pv); return ref; }
