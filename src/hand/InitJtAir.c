typedef int qword __attribute__((mode(TI)));
typedef struct T {
    int x0, x4; int p8[5]; int x1c; char pad[0xF0-0x20]; qword xf0; char pad2[0x108-0x100];
    float x108, x10c, x110, x114, x118, x11c; char pad3[0x164-0x120]; float x164;
} T;
extern qword D_002BF740;
void InitJtAir(T *t, int a)
{
    t->x0 = a;
    t->x4 = -1;
    t->x1c = -1;
    t->xf0 = D_002BF740;
    t->x108 = -60.0f;
    t->x10c = -50.0f;
    t->x110 = 75.0f;
    t->x114 = 50.0f;
    t->x11c = 75.0f;
    t->x118 = -150.0f;
    t->x164 = 1.0f;
}
