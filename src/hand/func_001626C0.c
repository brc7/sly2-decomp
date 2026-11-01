struct E { int a, b, c, d; };
struct S { char pad[0x50]; int f50; char pad2[0x8C - 0x54]; int c; struct E *ae; };
int GrfFromState(int i);
void func_001626C0(struct S *p, int a, int i, int b, int d)
{
    if (p->f50 == 0 && p->c < 32) {
        struct E *pe = &p->ae[p->c++];
        pe->a = a;
        pe->b = b;
        pe->c = GrfFromState(i);
        pe->d = d;
    }
}
