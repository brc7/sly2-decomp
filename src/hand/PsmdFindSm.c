struct E { int fl; int b; };
struct S { char pad[0x60]; int c; struct E *ae; };
struct E *PsmdFindSm(struct S *p, int m)
{
    int i;
    for (i = 0; i < p->c; i++) { struct E *pe = &p->ae[i]; if (pe->fl & m) return pe; }
    return 0;
}
