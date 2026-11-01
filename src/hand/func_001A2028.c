struct K { int a, b; };
extern struct K D_002A6790;
void *func_001FBB90(int n);
void func_001FB2F8(void *p, int n);
void SetJtHurtState(void *p, int n);
void func_001A15B0(void *p, struct K k);
void func_001A2028(void *p)
{
    func_001FB2F8(func_001FBB90(0x6A), 0);
    SetJtHurtState(p, 6);
    func_001A15B0(p, D_002A6790);
}
