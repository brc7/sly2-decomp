struct S { char pad[0x2044]; void *p2044; };
struct CLOCK { float t; float dt; };
extern struct CLOCK D_002DDED0;
extern int D_003E1BEC;
void func_0014D1D0(struct CLOCK *pclock, int n, float g);
void func_001FB098(void *p, float g);
void func_00258040(void **pp, int n);
void StopStepplayerGadget(struct S *p, int n);
void func_0018DD78(struct S *p, int n)
{
    if (n == 0x1D || n == 0x2A) {
        void **pp;
        func_0014D1D0(&D_002DDED0, 1, 1.0f);
        pp = &p->p2044;
        if (*pp) {
            func_001FB098(*pp, 0.0f);
            func_00258040(pp, 0);
        }
        D_003E1BEC = 0;
    } else {
        StopStepplayerGadget(p, n);
    }
}
