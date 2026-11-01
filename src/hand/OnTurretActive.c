extern char D_002DDF40[];
void OnPoActive(void *p, int b);
void func_0014E328(void *g, int a, void *h, void *p, int z);
void func_0014E4C8(void *g, int a, int b, void *h, void *p, int z);
void OnTurretActive(void *p, int b)
{
    OnPoActive(p, b);
    if (b) func_0014E328(D_002DDF40, 9, D_002DDF40 + 0x5F0, p, 0);
    else func_0014E4C8(D_002DDF40, 0xF, 9, D_002DDF40 + 0x5F0, p, 0);
}
