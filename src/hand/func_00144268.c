void AddPoHpMp(void *p, int a, int b);
void func_00144188(void *p, int a);
int func_001441A0(void *p);
void func_001441C0(void *p, int a);
void func_00144268(char *p, int a, int b)
{
    AddPoHpMp(p, a, b);
    func_00144188(p, **(int **)(p + 0xE00) + a);
    func_001441C0(p, func_001441A0(p) + b);
}
