struct CLOCK { float t; float dt; };
extern struct CLOCK D_002DDED0;
extern char D_003DD490[];
struct G { char pad[0x4A4]; int n4A4; };
extern struct G D_002DDF40;
int func_00210AE0(void);
void func_00161868(void *p);
int CploFindSwObjectsByClass(void *p, int a, int b, int c, int d, int e);
void func_001618B0(void *p);
void *func_001FBB90(int n);
void func_001FB2F8(void *p, int n);
void func_00140338(void)
{
    if (func_00210AE0()) {
        func_00161868(D_003DD490);
        if (CploFindSwObjectsByClass(D_003DD490, 0x605, 0x5F, 0, 0, 0)) func_001618B0(D_003DD490);
        func_001FB2F8(func_001FBB90(0x32), 0);
        *(float *)(D_003DD490 + 0x4754) = D_002DDED0.t;
        D_002DDF40.n4A4++;
    }
}
