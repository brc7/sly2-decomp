int CploFindSwObjectsByClass(void *a, int b, int c, int d, int f, int *pn);
int PloFindSwObjectByClass__FP2SWi3CIDP2LO(void *a, int b, int c, int d)
{
    int n;
    if (!CploFindSwObjectsByClass(a, b | 0x200, c, d, 1, &n))
        return 0;
    return n;
}
