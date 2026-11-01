struct VE { short d; short pad; void (*pf)(void *); };
struct O { char pad[0xC]; struct VE *vt; };
struct S {
    char pad0[0x39D0]; int n39D0; int pad39D4; int n39D8; int pad39DC; struct O **ppoFirst; struct O **ppoLast;
    char pad1[0x403C - 0x39E8]; char a403C[0x18]; char a4054[0x1C]; char a4070[0x18]; char a4088[4];
    char pad2[0x45CC - 0x408C]; void *p45CC;
};
extern char D_002DDF40[], D_002DEB30[], D_003D9E70[];
void func_0011CDD8(void *p);
void func_0020E338(struct S *p);
void func_0020E2B0(struct S *p);
void func_001FF828(struct S *p);
void InitSwProxySpares(struct S *p);
void func_0020F710(struct S *p);
void func_001569B8(struct S *p);
void func_001A3260(struct S *p);
void func_001C10C8(struct S *p);
void func_00210940(struct S *p);
void func_001957C0(struct S *p);
void func_001D9520(void *p);
void ProcessSwCallbacks(struct S *p);
void func_001C1260(void *p);
void func_0014D470(void *p);
void BroadcastGameEvent(void *p, int a, int b);

void PostSwLoad(struct S *p)
{
    struct O **ppo;
    func_0011CDD8(p->a403C);
    func_0011CDD8(p->a4054);
    func_0011CDD8(p->a4070);
    func_0011CDD8(p->a4088);
    func_0020E338(p);
    func_0020E2B0(p);
    func_001FF828(p);
    InitSwProxySpares(p);
    func_0020F710(p);
    func_001569B8(p);
    func_001A3260(p);
    func_001C10C8(p);
    func_00210940(p);
    func_001957C0(p);
    func_001D9520(D_003D9E70);
    while (ProcessSwCallbacks(p), p->ppoLast != p->ppoFirst) {
        for (ppo = p->ppoFirst; ppo < p->ppoLast; ppo++)
            (*ppo)->vt[15].pf((char *)*ppo + (*ppo)->vt[15].d);
        p->ppoLast = p->ppoFirst;
    }
    p->ppoLast = 0;
    p->ppoFirst = 0;
    p->n39D8 = 0;
    p->n39D0 = 0;
    func_001C1260(p->p45CC);
    func_0014D470(D_002DDF40);
    BroadcastGameEvent(D_002DEB30, 4, 0);
}
