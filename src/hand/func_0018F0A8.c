struct T { char pad[0x30]; int fl; };
struct S {
    char pad[0x150]; struct T *pt; char p0[0x16C]; char x2c0[0x208]; int x4c8;
    char p1[0x13FC]; int st; int p2; int x18d0; int p3; int x18d8; char p4[8]; int x18e4; int x18e8; int x18ec;
};
int GrfConsumePressed__5POCTLUif(struct T *p, int n, float g);
void func_00195A00(void *p, int a, int b, int c);
void func_0018C250(struct S *p, int a, int b);
void func_0018E540(struct S *p, int n);
int func_0018F050(struct S *p);
int func_0018EE38(struct S *p);
void func_0018F0A8(struct S *p)
{
    int f;
    switch (p->x18d8) {
    case 12 ... 21:
        if (!(p->pt->fl & 0x80)) p->x18ec = 0;
        break;
    }
    f = 0;
    switch (p->st) {
    case 3: case 21: case 22:
        if (GrfConsumePressed__5POCTLUif(p->pt, 0x80, 0.0f))
            func_00195A00(p->x2c0, 0, 0, 0);
        break;
    case 7:
        if (p->x4c8 < 5) f = p->x4c8 >= 3;
        break;
    case 5:
        switch (p->x18d0) {
        case 6: case 7:
            if (!(p->pt->fl & 0x80)) {
                if (p->x18e8) func_0018C250(p, 5, 8);
                else func_0018C250(p, 5, 4);
            }
            break;
        case 8: case 9:
            return;
        default:
            f = 1;
            break;
        }
        break;
    case 0: case 1: case 10: case 12:
        f = 1;
        break;
    }
    if (!f) return;
    switch (p->x18d8) {
    case 12 ... 23:
        if (!p->x18e4) return;
    case -1:
        if (GrfConsumePressed__5POCTLUif(p->pt, 0x10, 0.35f)) {
            func_0018E540(p, -1);
            func_0018E540(p, func_0018F050(p));
        } else if (GrfConsumePressed__5POCTLUif(p->pt, 0x80, 0.35f)) {
            func_0018E540(p, -1);
            func_0018E540(p, func_0018EE38(p));
        }
        break;
    }
}
