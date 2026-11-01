void LoadVolFromBrx(void *p);
void LoadBtn(void *p, void *q);
void LoadVolbtnFromBrx(char *p)
{
    LoadVolFromBrx(p);
    LoadBtn(p + 0x130, p);
    if (*(int *)(p + 0x2C8)) *(int *)(p + 0x1D4) = 1;
}
