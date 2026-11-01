struct S { char pad[0x40]; int f40; char pad2[0x150 - 0x44]; int f150; float f154; int f158; float f15c; float f160;
    char pad3[0x1e0 - 0x164]; int f1e0; int f1e4; int f1e8; int f1ec; int f1f0; char pad4[0x200 - 0x1f4]; struct S *f200; };
extern float D_002BC9C8;
extern float D_002A4B08;
void InitSo(struct S *);
void func_0013A9E8(struct S *p) {
    int x;
    InitSo(p);
    p->f154 = 3.40282346638528859812e+38f;
    p->f15c = D_002BC9C8;
    p->f160 = D_002BC9C8;
    p->f1e0 = 1;
    p->f1ec = 0x100;
    p->f1f0 = -10;
    x = p->f40;
    p->f150 = 0;
    if ((x ^ 1) & 1) *(int *)x |= 2;
    else p->f40 = x | 2;
    p->f200 = p;
}
