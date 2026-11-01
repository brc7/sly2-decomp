struct T { char pad[0x30]; int n30; };
struct S { char pad[0x10]; void *p10; int pad2; struct T *p18; char pad3[0x14]; int n30; char pad4[0xC]; int n40; };
void UpdateBhv(struct S *p);
int func_00209098(void *p);
void func_0017B9C0(struct S *p);
int FBhvBusyAbove(struct S *p);
void func_0017B8B0(struct S *p, int n);
int FBhvcombatDone(struct T *p);
void func_0017BD78(struct S *p) {
    int f, s;
    UpdateBhv(p);
    f = func_00209098(p->p10);
    func_0017B9C0(p);
    while (!FBhvBusyAbove(p)) {
        s = p->n30;
        switch (s) {
        case 0:
            if (f) func_0017B8B0(p, 2);
            else if (p->n40) func_0017B8B0(p, 1);
            break;
        case 1:
            if (f) func_0017B8B0(p, 2);
            else if (!p->n40) func_0017B8B0(p, 0);
            else if (p->n40 != p->p18->n30) { func_0017B8B0(p, -1); func_0017B8B0(p, 1); }
            else if (FBhvcombatDone(p->p18)) func_0017B8B0(p, 0);
            break;
        case 2:
            if (!f) func_0017B8B0(p, 0);
            break;
        }
        if (s != p->n30) continue;
        break;
    }
}
