struct G { char pad[0xC]; int xC; };
extern struct G D_002DEB30;
struct P { char pad[0x9BC]; int x9BC; char pad1[0x10DC - 0x9C0]; int x10DC; char pad2[0x130C - 0x10E0]; int x130C; char pad3[0x13E8 - 0x1310]; int x13E8; char pad4[0x143C - 0x13EC]; int x143C; char pad5[0x1AC4 - 0x1440]; int x1AC4; };
void BroadcastGameEvent(void *p, int a, struct P *b);
void ChargeGadgetMp(struct P *p, int k);
void StartStepplayerGadget(struct P *p, int k);
void SetMurrayState(struct P *p, int n);
void func_001B8D30(struct P *p, int n);
void SetJtGrabState(void *p, int a, int b, int c);
void UseMurrayGadget(struct P *p, int k)
{
    D_002DEB30.xC = k;
    BroadcastGameEvent(&D_002DEB30, 0x3C, p);
    switch (k) {
    case 20:
        ChargeGadgetMp(p, k);
        SetMurrayState(p, 0xC);
        func_001B8D30(p, 0);
        func_001B8D30(p, 1);
        break;
    case 15:
        p->x9BC = k;
        p->x130C = p->x1AC4;
        SetJtGrabState((char *)p + 0x4C0, 0xB, 0, 0);
        break;
    case 19:
        ChargeGadgetMp(p, k);
        p->x143C = 1;
        SetMurrayState(p, 8);
        break;
    case 22:
        ChargeGadgetMp(p, k);
        p->x13E8 = 1;
        SetMurrayState(p, 3);
        break;
    case 21:
        p->x9BC = 0x15;
        SetJtGrabState((char *)p + 0x4C0, 0xB, p->x10DC, 0);
        break;
    case 16:
        SetMurrayState(p, 0x15);
        break;
    default:
        StartStepplayerGadget(p, k);
        break;
    }
}
