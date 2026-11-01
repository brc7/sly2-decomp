struct O { int x0; void *x4; int x8; void *xC; };
extern char D_003DD490[];
extern struct O D_003E1C20;
extern char D_002C3EB8[], D_003E1C60[];
extern int D_002DD2E0, D_002DD2E4, D_002DD2E8, D_002DD2EC, D_002DD2F0, D_002DD2F4;
extern char D_004520A8[], D_004520D0[], D_004520F8[], D_00452120[], D_00452148[], D_00452170[];
void SW_ctor(void *);
void func_0011CCA8(void *, int, int, int);
void func_00258348(void *, int);
void StaticInit_sw(int fInit, int prio)
{
    if (prio != 0xFFFF)
        return;
    if (fInit) {
        SW_ctor(D_003DD490);
        D_003E1C20.x0 = 0;
        D_003E1C20.x4 = D_002C3EB8;
        D_003E1C20.x8 = 0;
        D_003E1C20.xC = D_003E1C60;
        if (++D_002DD2E0 == 1)
            func_0011CCA8(D_004520A8, 0x14, 0x2000, 0x10);
        if (++D_002DD2E4 == 1)
            func_0011CCA8(D_004520D0, 0x18, 0x800, 0x10);
        if (++D_002DD2E8 == 1)
            func_0011CCA8(D_004520F8, 0x28, 0x200, 0x10);
        if (++D_002DD2EC == 1)
            func_0011CCA8(D_00452120, 0x1C, 0x800, 0x10);
        if (++D_002DD2F0 == 1)
            func_0011CCA8(D_00452148, 0xC, 0xC8, 0x10);
        if (++D_002DD2F4 == 1)
            func_0011CCA8(D_00452170, 0x14, 0x100, 0x10);
    } else {
        func_00258348(D_003DD490, 2);
    }
}
