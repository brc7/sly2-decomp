typedef int ti128 __attribute__((mode(TI)));
struct S { char pad[0x40]; int f40; char pad2[0x170 - 0x44]; ti128 v170; char pad3[4]; int f184; };
extern ti128 D_002A2DE0;
void InitSo(struct S *);
void InitBooty(struct S *p) {
    int x;
    InitSo(p);
    p->v170 = D_002A2DE0; p->f184 = -1; x = p->f40;
    if ((x ^ 1) & 1) *(int *)x |= 2;
    else p->f40 = x | 2;
}
