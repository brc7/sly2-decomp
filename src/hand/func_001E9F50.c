typedef int qword __attribute__((mode(TI)));
void *PpathzoneFromPos(qword q, int n);
int func_001C1700(void *p, int n, qword q, qword q2, int c, void **ap);
int FPosInCnvo(void *p, qword q);
void *func_001E9F50(qword q)
{
    void *ap[0x40];
    void *p = PpathzoneFromPos(q, 0);
    int c, i;
    if (!p) return 0;
    c = func_001C1700(p, 0, q, q, 0x40, ap);
    for (i = 0; i < c; i++) {
        void *pT = ap[i];
        if (FPosInCnvo(pT, q)) return pT;
    }
    return 0;
}
