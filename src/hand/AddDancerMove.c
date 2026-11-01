struct E { int a; int b; int pad8; int c; int d; int pad14; int z18; int e; int pad20; int z24; };
struct S { char pad[0x2C4]; unsigned int c; struct E a[8]; };
void AddDancerMove(struct S *p, int a, int b, int c, int d, int e)
{
    if (p->c < 8) {
        struct E *pe = &p->a[p->c++];
        pe->a = a;
        pe->b = b;
        pe->c = c;
        pe->d = d;
        pe->e = e;
        pe->z18 = 0;
        pe->z24 = 0;
    }
}
