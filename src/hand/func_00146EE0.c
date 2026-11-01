void CloseBhv(void *p);
void SetMurrayState(void *p, int n);
void func_00146EE0(char *p)
{
    CloseBhv(p);
    *(int *)(*(char **)(p + 0x10) + 0x1AC0) = 0;
    SetMurrayState(*(void **)(p + 0x10), 0);
}
