struct O { char pad[0x28]; unsigned long f28; };
struct T { char pad[0x180]; struct O *po; };
struct P { char pad[0x1030]; int x1030; char pad1[0x13F0 - 0x1034]; struct T *x13F0; char pad2[0x1448 - 0x13F4]; int x1448; int x144C; int x1450; };
void SetMurrayState(struct P *p, int n);
int func_00131968(int n);
void UpdateMurrayLift(struct P *p)
{
    int s;
    for (;;) {
        s = p->x1450;
        switch (s) {
        case 0: break;
        case 1:
            if (p->x1448) {
                if (!p->x13F0) SetMurrayState(p, 0);
                else if (!((int)(p->x13F0->po->f28 >> 60) & 1)) {
                    SetMurrayState(p, 0);
                    p->x13F0 = 0;
                }
            }
            break;
        case 2:
            if (func_00131968(p->x1030)) SetMurrayState(p, 5);
            break;
        }
        if (p->x1450 == s || p->x1450 == -1) break;
    }
}
