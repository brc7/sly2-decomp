typedef int qword __attribute__((mode(TI)));
struct O { int n; float g; int pad[2]; qword q10; qword q20; };
struct P { char pad[0x13D0]; int n13D0; char pad1[0x1830 - 0x13D4]; qword q1830; };
extern qword D_002A2DD0;
void GetStepplayerStunHold(struct P *p, struct O *po);
void func_001B9B70(struct P *p, struct O *po)
{
    GetStepplayerStunHold(p, po);
    switch (p->n13D0) {
    case 11:
        po->n = 1;
        po->q10 = p->q1830;
        po->q20 = D_002A2DD0;
        break;
    case 0:
    case 1:
    case 9:
    case 10:
        po->n = 1;
        break;
    }
    po->g = 200.0f;
}
