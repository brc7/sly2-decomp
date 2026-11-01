extern char D_003DD490[];
void *PrecWorldEntry(void *p);
void func_0022B490(void *a, void *b, int c, int d, int e, int f);
void func_0014C190(void *p)
{
    void *v = PrecWorldEntry(p);
    if (!v) v = PrecWorldEntry(0);
    func_0022B490(D_003DD490, v, 0x189, -1, 0, -1);
}
