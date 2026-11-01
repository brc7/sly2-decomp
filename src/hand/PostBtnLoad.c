struct E { char b[0x44]; };
struct X { int a; struct E ae[2]; };
void PostAshLoad(struct E *e, int a);
void PostBtnLoad(struct X *p)
{
    unsigned int i;
    for (i = 0; i < 2; i++) PostAshLoad(&p->ae[i], p->a);
}
