struct O { char pad[0x130]; int f; };
extern char D_003DD490[];
int CploFindSwObjectsByClass(void *g, int a, int b, int c, int n, struct O **ap);
struct O *func_002280D0(void)
{
    struct O *ap[32];
    struct O *r = 0;
    int n = CploFindSwObjectsByClass(D_003DD490, 5, 0x1A, 0, 0x20, ap);
    int i;
    for (i = 0; i < n; i++) if (ap[i]->f) r = ap[i];
    return r;
}
