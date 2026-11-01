void func_001F2108(void *p);
void SetLoBusyk(void *p, int n);
void func_001471C0(char *p)
{
    unsigned int v;
    func_001F2108(p);
    *(int *)(p + 0x150) = -1;
    SetLoBusyk(p, 1);
    v = *(unsigned int *)(p + 0x40);
    if ((v ^ 1) & 1) *(int *)v |= 0x102;
    else *(unsigned int *)(p + 0x40) = v | 0x102;
}
