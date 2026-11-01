void InitXfm(void *p);
void InitBtn(void *p);
void SetLoBusyk(void *p, int n);
void InitVolbtn(char *p)
{
    unsigned int v;
    InitXfm(p);
    InitBtn(p + 0x130);
    *(int *)(p + 0x1CC) = 1;
    SetLoBusyk(p, 0);
    v = *(unsigned int *)(p + 0x40);
    if ((v ^ 1) & 1) *(int *)v |= 2;
    else *(unsigned int *)(p + 0x40) = v | 2;
}
