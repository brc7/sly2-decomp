struct S {
    char pad[0x40]; int x40;
    char pad2[0xE0 - 0x44]; int xE0; int xE4;
    char pad3[0x118 - 0xE8]; int x118;
    char pad4[0x154 - 0x11C]; float x154; int x158; int x15c, x160; float x164; float x168;
};
void func_00231100(struct S *s);
void SetLoBusyk(struct S *s, int n);
void func_001A3360(struct S *s)
{
    int v;
    func_00231100(s);
    s->xE0 = 0;
    s->xE4 = -1; s->x118 = -1;
    s->x164 = 200.0f;
    s->x168 = 2000.0f;
    s->x154 = 240.0f;
    s->x158 = 1;
    SetLoBusyk(s, 1);
    v = s->x40;
    if ((v ^ 1) & 1) *(int *)v &= ~0x20;
    else s->x40 = v & ~0x20;
}
