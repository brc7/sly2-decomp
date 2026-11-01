struct S {
    char pad[0x16C]; int a, b;
    void Action() __asm__("func_001BB400");
};
struct G;
extern G D_003DD490;
typedef void (S::*PMF)();
extern "C" void EnsureSwCallback(G *, S *, PMF, int);
extern "C" void func_001BAED8(S *p, int a, int b)
{
    p->b = b;
    p->a = a;
    EnsureSwCallback(&D_003DD490, p, &S::Action, 0);
}
