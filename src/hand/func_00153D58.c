void InitXfm(void *p);
void SetLoBusyk(void *p, int n);
void func_00153D58(char *p)
{
    unsigned int v;
    InitXfm(p);
    SetLoBusyk(p, 1);
    v = *(unsigned int *)(p + 0x40);
    if ((v ^ 1) & 1) *(int *)v |= 2;
    else *(unsigned int *)(p + 0x40) = v | 2;
}
