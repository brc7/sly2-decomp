struct E { int x0; int x4; int x8; };
struct S { int x0; int pad[4]; int x14; struct E *x18; };
struct D { int a; int b; };
extern char D_002BFA30[];
struct E *func_001AE238(struct D *d, void *p);
int PxfFindSo(int a, int b, int c);
void func_001A1098(struct S *s)
{
    struct D d;
    int i;
    s->x14 = 7;
    d.a = 0x54;
    d.b = 0x10;
    s->x18 = func_001AE238(&d, D_002BFA30);
    for (i = 0; i < s->x14; i++) {
        struct E *e = &s->x18[i];
        e->x8 = PxfFindSo(s->x0, e->x0, 6);
    }
}
