typedef int qword __attribute__((mode(TI)));
typedef struct { qword r[4]; } M4;
struct O { char pad[0x40]; M4 mat; };
void InvertMatrix4Rigid(M4 *pmDst, M4 *pmSrc);
void __ml__FRC7MATRIX4T0(M4 *pmDst, M4 *pmA, M4 *pmB);
void *func_0023320C(void *d, void *s, int n);
M4 *func_0022FF40(M4 *pr, struct O *pa, struct O *pb, M4 *pm)
{
    M4 m, mT, mInv;
    if (pa) {
        M4 *pmA;
        if (pb) {
            M4 *pmT = &mT;
            InvertMatrix4Rigid(&mInv, &pb->mat);
            __ml__FRC7MATRIX4T0(pmT, &mInv, &pa->mat);
            pmA = pmT;
        } else {
            pmA = &pa->mat;
        }
        __ml__FRC7MATRIX4T0(&m, pmA, pm);
        func_0023320C(pr, &m, 0x40);
    } else if (pb) {
        InvertMatrix4Rigid(&mT, &pb->mat);
        __ml__FRC7MATRIX4T0(&m, &mT, pm);
        func_0023320C(pr, &m, 0x40);
    } else {
        func_0023320C(pr, pm, 0x40);
    }
    return pr;
}
