struct E { int x0; void *p4; void *p8; };
struct O { char pad[0x15C]; int x15C; char pad1[0x16C - 0x160]; void *p16C; };
struct G { int x0; int x4; int x8; int xC; };
struct P { char pad[0x4C0]; char a4C0[0x9BC - 0x4C0]; int x9BC; char pad1[0x11B0 - 0x9C0]; char a11B0[4]; int x11B4; int pad2[2];
    struct O *po11C0; char pad3[0x130C - 0x11C4]; int x130C; char pad4[0x16B4 - 0x1310]; struct E *pe16B4;
    char pad5[0x1874 - 0x16B8]; int x1874; };
extern struct G D_002DEB30;
void BroadcastGameEvent(struct G *pg, int n, struct P *p);
struct E *func_00136A28(struct P *p, int k);
void func_002206C8(void *pv, int n);
void func_001928F8(void *pv, int n, void *pv2, int m);
struct O *PloAcquireProxy(void *pv, int n);
int func_00224610(struct O *po);
void func_0018D698(struct P *p, struct E *pe, struct O *po);
void func_00141EA8(struct P *p, int k);
void func_00135248(struct P *p, int n);
void func_00141CD0(struct P *p, int k);
void func_00141D38(struct P *p, int k);
void func_00136A90(struct P *p, int k)
{
    D_002DEB30.xC = k;
    BroadcastGameEvent(&D_002DEB30, 0x3C, p);
    switch (k) {
    case 4: case 5: case 7: case 9: case 11: case 13: {
        struct E *pe = func_00136A28(p, k);
        if ((unsigned int)(p->x11B4 - 4) < 3) {
            func_002206C8(p->a11B0, 7);
            return;
        }
        if (pe->p8) {
            func_001928F8(p->a4C0, 0xB, pe->p8, 0);
            return;
        }
        p->pe16B4 = pe;
        if (k == 7 || k == 11) {
            struct O *po = PloAcquireProxy(pe->p4, 0);
            if (!po)
                return;
            po->x15C = pe->x0;
            if (func_00224610(po)) {
                po->p16C = p;
                func_0018D698(p, pe, po);
            }
            p->po11C0 = po;
            func_00141EA8(p, k);
            func_00135248(p, 0xC);
            func_002206C8(p->a11B0, 4);
            return;
        }
        func_00141CD0(p, k);
        func_00135248(p, 6);
        return;
    }
    case 10:
        func_00141CD0(p, k);
        func_00135248(p, 0xE);
        return;
    case 8:
        p->x9BC = k;
        p->x130C = p->x1874;
        func_001928F8(p->a4C0, 0xB, 0, 0);
        return;
    default:
        func_00141D38(p, k);
        return;
    }
}
