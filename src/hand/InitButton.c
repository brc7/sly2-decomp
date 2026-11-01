void InitSo(void *p);
void InitBtn(void *p);
void InitButton(char *p)
{
    unsigned int v;
    InitSo(p);
    InitBtn(p + 0x150);
    v = *(unsigned int *)(p + 0x40);
    if ((v ^ 1) & 1) *(int *)v |= 2;
    else *(unsigned int *)(p + 0x40) = v | 2;
}
