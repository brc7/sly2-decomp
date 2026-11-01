struct S { char pad[0x7C]; int x7c; char pad2[0x90]; int x110; };
extern struct G D_003DD490;
void func_002128E8(struct S *p, int k);
void IncrementSwHandsOff(struct G *g, int a);
void func_00213B00(struct S *p, int a);
void func_00211CA8(struct S *p, int k)
{
    if (p->x7c == -1) {
        func_002128E8(p, k);
        IncrementSwHandsOff(&D_003DD490, 0);
        p->x110 = 1;
        func_00213B00(p, 0x33A);
    }
}
