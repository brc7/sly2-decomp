// `/` script builtin (Claude agent). The int-matrix path builds its VU_FLOAT temp in the caller, before the inline scale
// helper frame, so the temp reuses the freed CRef slot at sp+0x10 as in the original.
typedef int qword __attribute__((mode(TI)));
struct VECTOR { float x, y, z, w; };
struct MATRIX4 { float af[16]; } __attribute__((aligned(16)));
struct V8 { float x, y; };
struct VU_FLOAT {
    qword q;
    VU_FLOAT(float f) { __asm__("mfc1 %0, %1" : "=r"(q) : "f"(f)); }
    float F() const { float g; __asm__("mtc1 %1, %0" : "=f"(g) : "r"(q)); return g; }
};
struct VU_FLOAT_X { qword q; VU_FLOAT_X(float f); };
struct VU_VECTOR { qword q; VU_VECTOR(const VECTOR &v); };
qword operator*(VU_FLOAT_X f, VU_VECTOR v);
extern "C" qword *func_00256D18(VECTOR *p, qword v);
struct CRef {
    int m_tagk; union { int n; float g; void *pv; } u;
    CRef() { m_tagk = -1; }
    CRef(const CRef &r);
    void Decref();
    void InitTag(int tagk);
    ~CRef() { Decref(); }
    CRef &operator=(const CRef &r);
    CRef &operator=(int n);
    CRef &operator=(float g);
};
struct NODE { int x0, x4; CRef ref; NODE *pnext; };
CRef func_00129BC8(CRef *pref, void *a, void *b);
extern "C" CRef func_0011FEC8(void *pv, CRef *aref);

struct VU_FLOAT_V {
    qword q;
    VU_FLOAT_V(float f) { __asm__("qmtc2.ni %1, %0" : "=j"(q) : "r"(f)); }
};
static inline void DivVector(qword *pq, float d)
{
    VU_FLOAT_V s(1.0f / d);
    qword a = *pq, r;
    __asm__("vmulx.xyzw %0, %2, %1x" : "=j"(r) : "j"(s.q), "j"(a));
    *pq = r;
}

static inline void ScaleM(MATRIX4 *pm, float g)
{
    MATRIX4 mat;
    for (int i = 0; i < 16; i++)
        mat.af[i] = g * pm->af[i];
    *pm = mat;
}
static inline void DivMatrix(MATRIX4 *pm, float d)
{
    MATRIX4 mat;
    float g = VU_FLOAT(1.0f / d).F();
    for (int i = 0; i < 16; i++)
        mat.af[i] = g * pm->af[i];
    *pm = mat;
}

/** @sly2-annot addr=0x11FEC8
 * Purpose: Splice builtin "/": n-ary divide incl. int-matrix path
 * Info: proposed name RefOpDiv (T1); TU bif
 * Confidence: high [T1 0.93] - evidence: static analysis; sources: r1_116918
 * See: docs/reference/functions/splice-scripting.md#bif-0x11f440-0x1258f0
 * @end-sly2-annot */
CRef func_0011FEC8(void *pv, CRef *aref)
{
    CRef ref;
    int fInt = 0;
    switch (aref[0].m_tagk) {
    case 0x15:
        ref = (float)aref[0].u.n;
        fInt = 1;
        break;
    case 0x16:
    case 0x1A:
    case 0x1B:
    case 0x1C:
    case 0x1D:
        ref = func_00129BC8(&aref[0], 0, 0);
        break;
    }
    NODE *pn = 0;
    if (aref[1].m_tagk)
        pn = (NODE *)aref[1].u.pv;
    for (; pn; pn = pn->pnext) {
        CRef *pr = &pn->ref;
        switch (ref.m_tagk) {
        case 0x16: {
            float d;
            switch (pr->m_tagk) {
            case 0x16:
                d = pr->u.g;
                fInt = 0;
                break;
            case 0x15:
                d = (float)pr->u.n;
                break;
            default:
                continue;
            }
            ref.u.g /= d;
            break;
        }
        case 0x1A:
            switch (pr->m_tagk) {
            case 0x15: DivVector((qword *)ref.u.pv, (float)pr->u.n); break;
            case 0x16: DivVector((qword *)ref.u.pv, pr->u.g); break;
            }
            break;
        case 0x1B:
            switch (pr->m_tagk) {
            case 0x15: ScaleM((MATRIX4 *)ref.u.pv, VU_FLOAT(1.0f / (float)pr->u.n).F()); break;
            case 0x16: DivMatrix((MATRIX4 *)ref.u.pv, pr->u.g); break;
            }
            break;
        case 0x1C:
            switch (pr->m_tagk) {
            case 0x15: {
                VECTOR *pvec = (VECTOR *)ref.u.pv;
                func_00256D18(pvec, VU_FLOAT_X(1.0f / (float)pr->u.n) * VU_VECTOR(*pvec));
                break;
            }
            case 0x16: {
                VECTOR *pvec = (VECTOR *)ref.u.pv;
                func_00256D18(pvec, VU_FLOAT_X(1.0f / pr->u.g) * VU_VECTOR(*pvec));
                break;
            }
            }
            break;
        case 0x1D: {
            float d;
            switch (pr->m_tagk) {
            case 0x15: d = (float)pr->u.n; break;
            case 0x16: d = pr->u.g; break;
            default: continue;
            }
            float s = 1.0f / d;
            V8 *p = (V8 *)ref.u.pv;
            p->x *= s;
            p->y *= s;
            break;
        }
        }
    }
    if (fInt && ref.m_tagk == 0x16)
        ref = (int)ref.u.g;
    return ref;
}
