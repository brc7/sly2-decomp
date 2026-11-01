struct CLOCK { float t; float dt; };
extern struct CLOCK D_002DDED0;
struct U { int k; int pad; void *p8; };
struct T { char pad[0xE7C]; float gE7C; float gE80; };
struct S { char pad[0x10]; struct T *pt; char pad2[0x2C]; struct U *p40; char pad3[0xC]; int n50; float g54; float g58; };
void func_001F0660(void *p, int n);
float GRandInRange__Fff(float a, float b);
void func_0017A5B0(struct S *p, int n) {
    if (n == p->n50) return;
    p->n50 = n;
    p->g54 = n;
    switch (n) {
    case 0:
        if (p->p40->k == 1) func_001F0660(p->p40->p8, 0x44);
        { float t = D_002DDED0.t; p->g58 = t + GRandInRange__Fff(p->pt->gE7C, p->pt->gE80); }
        break;
    case 1:
        if (p->p40->k == n) func_001F0660(p->p40->p8, 0x43);
        break;
    }
}
