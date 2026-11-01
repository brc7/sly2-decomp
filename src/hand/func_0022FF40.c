typedef int qword __attribute__((mode(TI)));
typedef struct { qword r[4]; } M4;
struct O { char pad[0x40]; M4 mat; };
void func_0011C318(M4 *pmDst, M4 *pmSrc);
void func_0011C168(M4 *pmDst, M4 *pmA, M4 *pmB);
void *func_0023320C(void *d, void *s, int n);
/** @sly2-annot addr=0x22FF40
 * Purpose: relative M4 between two XFs (inverse)
 * Info: proposed name ConvertXfMatrix4 (T2); TU xf
 * Confidence: low (draft) [T2 0.30] - evidence: static analysis; sources: r8_21BB88
 * See: docs/reference/functions/animation.md#xf-0x22c818-0x231100
 * @end-sly2-annot */
M4 *func_0022FF40(M4 *pr, struct O *pa, struct O *pb, M4 *pm)
{
    M4 m, mT, mInv;
    if (pa) {
        M4 *pmA;
        if (pb) {
            M4 *pmT = &mT;
            func_0011C318(&mInv, &pb->mat);
            func_0011C168(pmT, &mInv, &pa->mat);
            pmA = pmT;
        } else {
            pmA = &pa->mat;
        }
        func_0011C168(&m, pmA, pm);
        func_0023320C(pr, &m, 0x40);
    } else if (pb) {
        func_0011C318(&mT, &pb->mat);
        func_0011C168(&m, &mT, pm);
        func_0023320C(pr, &m, 0x40);
    } else {
        func_0023320C(pr, pm, 0x40);
    }
    return pr;
}
