int FIgnoreSoIntersection(void *p, int a);
int func_00220498(void *p, int a, int b);
void func_001CC9A8(void *p, int a, int b);
void func_00144388(char *p, int a)
{
    int x = FIgnoreSoIntersection(p, a);
    x = func_00220498(p + 0x11B0, a, x);
    func_001CC9A8(p + 0x900, a, x);
}
