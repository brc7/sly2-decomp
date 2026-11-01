struct S { char pad[0x18]; void *p18; char pad2[0x18]; int n34; char pad3[8]; int n40; };
void UpdateBhv(struct S *p);
int FBhvBusyAbove(struct S *p);
int func_00174FB8(void *p);
void func_00175488(struct S *p, int n);
void func_00175600(struct S *p) {
    int s;
    UpdateBhv(p);
    do {
        if (FBhvBusyAbove(p)) return;
        s = p->n34;
        switch (s) {
        case 0:
            if (p->n40) func_00175488(p, 1);
            else if (func_00174FB8(p->p18)) func_00175488(p, -1);
            break;
        case 1:
            if (func_00174FB8(p->p18)) func_00175488(p, -1);
            break;
        }
    } while (s != p->n34);
}
