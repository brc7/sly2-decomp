struct MSG { int cb; int nKind; };
struct S { int a, b; void *ap[8]; };
int CbBhvMax(void);
void *func_001AE1D0(struct MSG *pmsg);
void PostBhvstkLoad(struct S *p)
{
    unsigned int i;
    struct MSG msg;
    for (i = 0; i < 8; i++) {
        msg.cb = CbBhvMax();
        msg.nKind = 0x10;
        p->ap[i] = func_001AE1D0(&msg);
    }
}
