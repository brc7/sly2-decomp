void func_00231100(void *p);
void SetLoBusyk(void *p, int n);
void func_00156CC8(char *p)
{
    unsigned int v;
    func_00231100(p);
    SetLoBusyk(p, 0);
    v = *(unsigned int *)(p + 0x40);
    if ((v ^ 1) & 1) *(int *)v |= 2;
    else *(unsigned int *)(p + 0x40) = v | 2;
}
