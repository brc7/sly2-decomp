void InitBhv(void *p);
void func_001FEE90(void *p);
void AddStepguardSgf(void *p, int n);
extern char D_003DD040[];
void func_0014A1B0(char *p)
{
    InitBhv(p);
    func_001FEE90(D_003DD040);
    AddStepguardSgf(*(void **)(p + 0x10), 0x24);
    *(int *)(p + 0x30) = -1;
    *(int *)(p + 0x40) = -1;
    *(int *)(p + 0x3C) = -1;
}
