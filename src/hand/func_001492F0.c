void func_00231100(void *p);
void func_00148718(void *p);
void SetLoBusyk(void *p, int n);
void func_001492F0(char *p)
{
    unsigned int v;
    func_00231100(p);
    func_00148718(p + 0x130);
    *(int *)(p + 0x1CC) = 1;
    SetLoBusyk(p, 0);
    v = *(unsigned int *)(p + 0x40);
    if ((v ^ 1) & 1) *(int *)v |= 2;
    else *(unsigned int *)(p + 0x40) = v | 2;
}
