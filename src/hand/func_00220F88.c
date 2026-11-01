struct K { int a, b; };
extern struct K D_002A96E8;
extern char D_003DD490[];
void RemoveLoHierarchy(void *p);
void ClearSwCallbacks(void *g, int a, void *p, struct K k, int z);
void func_00220F88(void *p)
{
    RemoveLoHierarchy(p);
    ClearSwCallbacks(D_003DD490, 1, p, D_002A96E8, 0);
}
