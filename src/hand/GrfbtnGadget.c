struct E { int id; int a, b; };
struct X { char pad[0x1180]; struct E ae[3]; };
extern int D_002C3258[];
int GrfbtnGadget(struct X *p, int id)
{
    int i;
    int m = 0;
    for (i = 0; i < 3; i++) {
        if (id == p->ae[i].id) m |= D_002C3258[i];
    }
    return m;
}
