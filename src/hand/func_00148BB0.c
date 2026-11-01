void InitSo(void *p);
void func_00148718(void *p);
void func_00148BB0(char *p)
{
    unsigned int v;
    InitSo(p);
    func_00148718(p + 0x150);
    v = *(unsigned int *)(p + 0x40);
    if ((v ^ 1) & 1) *(int *)v |= 2;
    else *(unsigned int *)(p + 0x40) = v | 2;
}
