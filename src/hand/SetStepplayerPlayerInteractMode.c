struct X { char pad[0x10DC]; int b; int pad2; int a; int d; int c; };
void func_001431D0(struct X *p, int a, int b, int c, int d);
void SetStepplayerPlayerInteractMode(struct X *p, int a, int b, int c, int d)
{
    if (b != p->b) func_001431D0(p, a, b, c, d);
    p->b = b;
    p->a = a;
    p->d = d;
    p->c = c;
}
