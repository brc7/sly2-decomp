typedef int qword __attribute__((mode(TI)));
struct S { char pad[0xF4]; int x; };
qword func_001A6F00(int a, struct S *p, qword q);
int PbspPointInBspQuick(int a, int b, qword q);
void func_00227440(struct S *p, int f);
void func_002273F0(struct S *p, qword q)
{
    func_00227440(p, PbspPointInBspQuick(p->x, 0x7FFFFFFF, func_001A6F00(0, p, q)) != 0);
}
