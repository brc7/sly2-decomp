typedef int qword __attribute__((mode(TI)));
struct S { char pad[0x4020]; int a; int b; };
void func_00156698(int n, qword q, int c);
void EmitSwNoise(struct S *p, int k, qword q, int c)
{
    int n;
    switch (k) {
    case 0: n = p->a; break;
    case 1: n = p->b; break;
    }
    func_00156698(n, q, c);
}
