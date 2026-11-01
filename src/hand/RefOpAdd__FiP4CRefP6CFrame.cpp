// (Claude agent, lane A9, 2026-10-23) script `+` builtin (probably). The first node is read through an inline accessor in an
// if/else: the inline blocks jump.c's "x = b; if (c) x = a" rewrite, so `pn = 0` is laid out after the branch and
// reorg steals it into the beqz delay slot as in the original.
typedef int qword __attribute__((mode(TI)));
struct VECTOR { float x, y, z, w; };
struct MATRIX4 { float m[4][4]; };
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
    CRef &operator=(const VECTOR &v);
    CRef &operator=(const MATRIX4 &m);
};
struct NODE { int x0, x4; CRef ref; NODE *pnext; };
extern "C" CRef RefOpAdd__FiP4CRefP6CFrame(void *pv, CRef *aref);

static inline NODE *PnFromRef(CRef *p) { return (NODE *)p->u.pv; }
CRef RefOpAdd__FiP4CRefP6CFrame(void *pv, CRef *aref)
{
    CRef ref;
    int tagk = aref[0].m_tagk;
    NODE *pn;
    if (aref[1].m_tagk)
        pn = PnFromRef(&aref[1]);
    else
        pn = 0;
    switch (tagk) {
    case 0x15:
    case 0x16: {
        float g;
        int fInt;
        if (tagk == 0x15) {
            g = (float)aref[0].u.n;
            fInt = 1;
        } else {
            g = aref[0].u.g;
            fInt = 0;
        }
        for (; pn; pn = pn->pnext) {
            if (pn->ref.m_tagk == 0x15) {
                g += (float)pn->ref.u.n;
            } else {
                fInt = 0;
                g += pn->ref.u.g;
            }
        }
        if (fInt)
            ref = (int)g;
        else
            ref = g;
        break;
    }
    case 0x1A:
        ref = *(VECTOR *)aref[0].u.pv;
        for (; pn; pn = pn->pnext) {
            qword *pq = (qword *)ref.u.pv;
            qword a = *pq, b = *(qword *)pn->ref.u.pv, r;
            __asm__("vadd.xyzw %0, %1, %2" : "=j"(r) : "j"(a), "j"(b));
            *pq = r;
        }
        break;
    case 0x1B:
        ref = *(MATRIX4 *)aref[0].u.pv;
        for (; pn; pn = pn->pnext) {
            MATRIX4 *pm = (MATRIX4 *)ref.u.pv;
            MATRIX4 *pmB = (MATRIX4 *)pn->ref.u.pv;
            pm->m[0][0] += pmB->m[0][0]; pm->m[0][1] += pmB->m[0][1]; pm->m[0][2] += pmB->m[0][2]; pm->m[0][3] += pmB->m[0][3];
            pm->m[1][0] += pmB->m[1][0]; pm->m[1][1] += pmB->m[1][1]; pm->m[1][2] += pmB->m[1][2]; pm->m[1][3] += pmB->m[1][3];
            pm->m[2][0] += pmB->m[2][0]; pm->m[2][1] += pmB->m[2][1]; pm->m[2][2] += pmB->m[2][2]; pm->m[2][3] += pmB->m[2][3];
            pm->m[3][0] += pmB->m[3][0]; pm->m[3][1] += pmB->m[3][1]; pm->m[3][2] += pmB->m[3][2]; pm->m[3][3] += pmB->m[3][3];
        }
        break;
    }
    return ref;
}
