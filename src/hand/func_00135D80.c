struct S { char pad[0x268]; int f268; char pad1[0x2c0 - 0x26c]; int f2c0; char pad2[0x4c0 - 0x2c4]; int f4c0; char pad3[0xdf0 - 0x4c4]; int fdf0; char pad4[0x13d0 - 0xdf4]; int st; };
int func_001944C8(void *, int *);
int func_0019C870(void *, int *);
int func_001A23F8(void *, int *);
void SetStepPhys(struct S *, int, int);
void func_00135D80(struct S *p) {
    int n = 0;
    int r;
    switch (p->st) {
    case 3: r = func_001944C8(&p->f4c0, &n); break;
    case 4: r = func_001A23F8(&p->fdf0, &n); break;
    case 2: r = func_0019C870(&p->f2c0, &n); break;
    default: r = 0; break;
    }
    if (!r) r = p->f268;
    SetStepPhys(p, r, n);
}
