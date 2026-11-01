struct S { char pad[0x2C0]; long fl; char pad2[0xE64 - 0x2C8]; int nE64; };
long GrfFromSgf(int n);
int FStepguardSensesEnabled(struct S *p)
{
    long m = GrfFromSgf(0xB) | GrfFromSgf(0x1D) | GrfFromSgf(7) | GrfFromSgf(5) | GrfFromSgf(6)
           | GrfFromSgf(0x10) | GrfFromSgf(1) | GrfFromSgf(0x16) | GrfFromSgf(0x2A);
    if (m & p->fl)
        return 0;
    return p->nE64;
}
