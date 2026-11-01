struct S { char pad[0x1000]; int a; int b; char p1[0xE0]; int n; float g; };
void SetJoyJoys__FP3JOY4JOYS4JOYK(struct S *p, int a, int b);
void func_00243088(int a, int b, struct S *p);
void InitJoy__FP3JOYii(struct S *p, int a, int b)
{
    SetJoyJoys__FP3JOY4JOYS4JOYK(p, 1, 0);
    p->a = a;
    p->b = b;
    func_00243088(a, b, p);
    p->n = 1;
    p->g = 1.0f;
}
