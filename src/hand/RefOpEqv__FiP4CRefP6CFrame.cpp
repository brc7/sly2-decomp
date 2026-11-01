// MATCH (Claude agent laneB14, 2026-10-23), from RefOpEqv__FiP4CRefP6CFrame.cpp (6). FEq body written out with one commented register pin (pb in a1)
// plus one empty asm whose only job is extra uses of f (f is live from entry on the default path, so its priority was below the return pointer's).
struct CRef {
    int m_tagk; int m_tag;
    CRef(int n, int tagk) { m_tag = n; m_tagk = tagk; }
};
extern "C" CRef RefOpEqv__FiP4CRefP6CFrame(void *pbasic, CRef *aref);
CRef RefOpEqv__FiP4CRefP6CFrame(void *pbasic, CRef *aref)
{
    CRef *pa = &aref[0];
    register CRef *pb __asm__("$5") = &aref[1];  /* register pin: &aref[1] reuses the dead pbasic register (a1) as in the target */
    int f;
    if (pa->m_tagk != pb->m_tagk) {
        f = 0;
    } else {
        switch (pa->m_tagk) {
        case 0x15: case 0x17: case 0x18: case 0x19:
        case 0x1F: case 0x20: case 0x21: case 0x22:
            f = pa->m_tag == pb->m_tag;
            break;
        case 0: case 1: case 2: case 3: case 4: case 5: case 6: case 7: case 8: case 9:
        case 10: case 11: case 12: case 13: case 14: case 15: case 16: case 17: case 18:
        case 19: case 20: case 0x24:
            f = 1;
            break;
        default:
            break;
        }
    }
    __asm__ __volatile__("" : : "r"(f), "r"(f), "r"(f));  /* allocation-only operands: raise f's global-alloc priority above the return pointer (f a3, ret t0 as in the target) */
    return CRef(f, 0x17);
}
