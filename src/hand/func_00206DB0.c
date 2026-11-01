extern int D_002C3D10[];
extern char D_003DD490[];
int CploFindSwObjectsByClass(void *p, int a, int b, int c, int d, int e);
void func_00208920(void *p, int n);
void func_00206DB0(void *p)
{
    unsigned int i;
    for (i = 0; i < 5; i++) {
        int n = D_002C3D10[i];
        if (CploFindSwObjectsByClass(D_003DD490, 0x605, n, 0, 0, 0))
            func_00208920(p, n);
    }
}
