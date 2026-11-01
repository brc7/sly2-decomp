struct P { char pad[0x10]; void *x10; int pad1; void *x18; char pad2[0x30 - 0x1C]; int x30; int pad3[2]; int x3C; int x40; int x44; };
extern char D_003DD490[];
void UpdateBhv(struct P *p);
void func_0014A4D0(struct P *p);
int FBhvBusyAbove(struct P *p);
int func_00209098(void *p);
int func_0014A478(struct P *p);
int func_0014AA30(struct P *p);
int PpoHero(void *p);
int func_001758A0(void *p, int n, int m);
void func_0014AE38(struct P *p, int k);
void func_0014AD70(struct P *p);
int func_00178D00(void *p);
int func_00174FB8(void *p);
int func_00175748(void *p);
void func_0014A2A0(struct P *p)
{
    int s;
    UpdateBhv(p);
    func_0014A4D0(p);
    for (;;) {
        if (FBhvBusyAbove(p))
            break;
        s = p->x30;
        switch (s) {
        case 0:
            if (func_00209098(p->x10)) {
                func_0014AE38(p, 4);
            } else if (func_0014A478(p)) {
                func_0014AE38(p, 2);
            } else {
                int v = func_0014AA30(p);
                p->x40 = v;
                if (v == p->x3C) {
                    int r = func_001758A0(p->x10, PpoHero(D_003DD490), 0);
                    p->x44 = r;
                    if (r)
                        func_0014AE38(p, 3);
                } else {
                    func_0014AD70(p);
                }
            }
            break;
        case 1:
            if (func_00209098(p->x10))
                func_0014AE38(p, 4);
            else if (func_0014A478(p))
                func_0014AE38(p, 2);
            else if (func_00178D00(p->x18))
                func_0014AE38(p, 0);
            else
                func_001758A0(p->x10, PpoHero(D_003DD490), 0);
            break;
        case 2:
            if (func_00209098(p->x10))
                func_0014AE38(p, 4);
            else if (func_00174FB8(p->x18))
                func_0014AE38(p, 0);
            break;
        case 3:
            if (func_00209098(p->x10))
                func_0014AE38(p, 4);
            else if (func_00175748(p->x18))
                func_0014AE38(p, 0);
            break;
        case 4:
            if (!func_00209098(p->x10))
                func_0014AE38(p, 0);
            break;
        }
        if (s == p->x30)
            break;
    }
}
