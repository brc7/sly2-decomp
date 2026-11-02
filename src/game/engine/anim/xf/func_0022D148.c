typedef int qword __attribute__((mode(TI)));
typedef struct { qword r[4]; } M4;
struct O { char pad[0x40]; M4 mat; char pad1[0x98 - 0x80]; short k; char pad2[0xA0 - 0x9A]; struct O *pParent; };
void InvertMatrix4Rigid(M4 *pmDst, M4 *pmSrc);
void __ml__FRC7MATRIX4T0(M4 *pmDst, M4 *pmA, M4 *pmB);
void *func_0023320C(void *d, void *s, int n);
void *func_002333B4(void *p, int c, int n);
M4 *func_0022CDC0(M4 *pr, struct O *po);
static __inline__ void AddM4(M4 *pmDst, M4 *pmA, M4 *pmB)
{
    int i;
    for (i = 0; i < 4; i++) {
        qword r;
        __asm__("vadd.xyzw %0, %1, %2" : "=j"(r) : "j"(pmA->r[i]), "j"(pmB->r[i]));
        pmDst->r[i] = r;
    }
}
/** @sly2-annot addr=0x22D148
 * Purpose: angular velocity of an XF (finite difference via 0x22CDC0)
 * Info: proposed name GetXfAngularVelocity (T2); TU xf
 * Confidence: low (draft) [T2 0.45] - evidence: static analysis; sources: r8_21BB88
 * See: docs/reference/functions/animation.md#xf-0x22c818-0x231100,
 *   docs/reference/functions/physics-collision.md#so-0x1f2108-0x1f8dd8
 * @end-sly2-annot */
M4 *func_0022D148(M4 *pm, struct O *po, struct O *poStop)
{
    M4 mB, mA, mE, mC, mD;
    struct O *p;
    func_002333B4(pm, 0, 0x40);
    for (p = po; p != poStop; p = p->pParent) {
        switch (p->k) {
        case 0: case 3: case 10: case 12: case 15: case 19:
            break;
        default: {
            
            func_0022CDC0(&mA, p);
            { qword d = mA.r[3], a = mA.r[0], b = mA.r[1], c = mA.r[2]; mB.r[0] = a; mB.r[1] = b; mB.r[2] = c; mB.r[3] = d; }
            
            if (p->pParent) {
                M4 *pmE = &mE;
                
                
                __ml__FRC7MATRIX4T0(&mC, &p->pParent->mat, &mB);
                
                InvertMatrix4Rigid(&mD, &p->mat);
                __ml__FRC7MATRIX4T0(pmE, &mC, &mD);
                AddM4(&mA, pm, pmE);
                func_0023320C(pm, &mA, 0x40);
            } else {
                M4 *pmE = &mE;
                InvertMatrix4Rigid(&mC, &p->mat);
                __ml__FRC7MATRIX4T0(pmE, &mB, &mC);
                AddM4(&mA, pm, pmE);
                func_0023320C(pm, &mA, 0x40);
            }
            break;
        }
        }
    }
    if (poStop) {
        M4 *pmE = &mE;
        M4 *pmA = &mA;
        InvertMatrix4Rigid(pmE, &poStop->mat);
        __ml__FRC7MATRIX4T0(pmA, pmE, pm);
        func_0023320C(pm, pmA, 0x40);
    }
    return pm;
}
