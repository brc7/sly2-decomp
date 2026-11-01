void LoadVolFromBrx(void *p);
void func_00148738(void *p, void *q);
void func_00149358(char *p)
{
    LoadVolFromBrx(p);
    func_00148738(p + 0x130, p);
    if (*(int *)(p + 0x2C8)) *(int *)(p + 0x1D4) = 1;
}
