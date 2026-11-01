struct S { char pad[0x18]; void *p18; char pad2[0x18]; int n34; char pad3[8]; int n40; };
void UpdateBhv(struct S *p);
int FBhvBusyAbove(struct S *p);
int FBhvanimDone(void *p);
void SetBhvattackState(struct S *p, int n);
void UpdateBhvattack(struct S *p) {
    int s;
    UpdateBhv(p);
    do {
        if (FBhvBusyAbove(p)) return;
        s = p->n34;
        switch (s) {
        case 0:
            if (p->n40) SetBhvattackState(p, 1);
            else if (FBhvanimDone(p->p18)) SetBhvattackState(p, -1);
            break;
        case 1:
            if (FBhvanimDone(p->p18)) SetBhvattackState(p, -1);
            break;
        }
    } while (s != p->n34);
}
