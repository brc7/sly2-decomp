void StartBhv(void *p);
void func_0014AE38(void *p, int n);
extern float D_002DDED0;
void func_0014A200(char *p)
{
    StartBhv(p);
    func_0014AE38(p, 0);
    *(float *)(p + 0x48) = D_002DDED0;
    *(int *)(*(char **)(p + 0x10) + 0x4A4) = 0;
}
