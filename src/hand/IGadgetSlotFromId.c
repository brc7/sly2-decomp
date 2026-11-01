struct E { int id; int a, b; };
struct X { char pad[0x1180]; struct E ae[3]; };
int IGadgetSlotFromId(struct X *p, int id)
{
    int i;
    for (i = 0; i < 3; i++) {
        if (id == p->ae[i].id) return i;
    }
    return -1;
}
