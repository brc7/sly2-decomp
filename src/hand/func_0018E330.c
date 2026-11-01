extern char D_003DD490[];
int CploFindSwObjectsByClass(void *g, int a, int b, int c, int cMax, void **ap);
void func_001F1900(void *p);
void func_0018E330(void)
{
    void *ap[32];
    int c, i;
    c = CploFindSwObjectsByClass(D_003DD490, 0x505, 0x38, 0, 0x20, ap);
    for (i = 0; i < c; i++)
        func_001F1900(ap[i]);
}
