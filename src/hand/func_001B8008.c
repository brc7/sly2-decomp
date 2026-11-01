struct P { char pad[0x268]; int x268; char pad1[0x13D0 - 0x26C]; int k; char pad2[0x13F0 - 0x13D4]; int x13F0; char pad3[0x1458 - 0x13F4]; int x1458; char pad4[0x1A58 - 0x145C]; int a1A58[0x16]; int a1AB0[3]; int x1ABC; };
int func_001944C8(void *p, int *pk);
int func_001A23F8(void *p, int *pk);
int func_0019C870(void *p, int *pk);
void SetStepPhys(struct P *p, int v, int k);
void func_001B8008(struct P *p)
{
    int k = 0;
    int v;
    switch (p->k) {
    case 13: v = func_001944C8((char *)p + 0x4C0, &k); break;
    case 14: v = func_001A23F8((char *)p + 0xDF0, &k); break;
    case 2:
        if (p->x13F0) v = p->x1ABC;
        else v = func_0019C870((char *)p + 0x2C0, &k);
        break;
    case 5: v = p->a1AB0[p->x1458]; break;
    default: v = p->a1A58[p->k]; break;
    }
    if (!v) v = p->x268;
    SetStepPhys(p, v, k);
}
