struct L { char pad[0x84]; char *pb; };
struct S { int x0; int k; struct L *pl; int xc; int i; };
struct T { char pad[0x58]; int x58; char pad5c[5]; unsigned char b61; };
void func_002185F8(void *p, int a, int b);
void PrependSlEntry(void *p, struct T *t);
void func_00215960(struct S *p, struct T *t, int b)
{
    switch (p->k) {
    case 4:
        func_002185F8(p->pl->pb + p->i * 0x30, t->x58, t->b61);
        break;
    case 1:
    case 3:
    case 6:
    case 7:
        t->b61 = b;
        PrependSlEntry((char *)p->pl + 0xFC, t);
        break;
    case 2:
    case 5:
        break;
    }
}
