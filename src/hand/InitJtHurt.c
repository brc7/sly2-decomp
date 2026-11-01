struct S { int a0; int a4; int pad8; int c; char pad[0x80]; int x90; int pad94[2]; float x9c; };
void InitJtHurt(struct S *p, int a)
{
    p->a0 = a; //P
    p->a4 = -1; //P
    p->x90 = -1; //P
    p->x9c = 2.0f; //P
    p->c = 0x28; //P
}
