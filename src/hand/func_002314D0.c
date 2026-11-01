typedef int qword __attribute__((mode(TI)));
typedef struct { float m[16]; } __attribute__((aligned(16))) M4;
typedef struct X { char pad[0x50]; void *x50; } X;
extern qword D_002A2DD0;
M4 func_0022D148(void *, int);

static __inline__ qword Make(float x, float y, float z)
{
    qword q, t;
    __asm__("mfc1 %0,%2\n\tmfc1 %1,%3\n\tpextlw %0,%1,%0\n\tmfc1 %1,%4\n\tpcpyld %0,%1,%0"
            : "=&r"(q), "=&r"(t) : "f"(x), "f"(y), "f"(z));
    return q;
}

/** @sly2-annot addr=0x2314D0
 * Purpose: via GetXfAngularVelocity
 * Info: proposed name GetXfmAngularVelocity (T2); class XFM slot34; TU xform
 * Confidence: low (draft) [T2 0.45] - evidence: static analysis; sources: r8_21BB88
 * See: docs/reference/functions/object-model.md#xform-0x231100-0x2319e8
 * @end-sly2-annot */
qword func_002314D0(X *p)
{
    if (!p->x50) return D_002A2DD0;
    {
        M4 m;
        {
            M4 t = func_0022D148(p->x50, 0);
            m = t;
        }
        {
            union { qword q; float f; } r;
            r.q = Make(m.m[6], m.m[8], m.m[1]);
            return r.q;
        }
    }
}
