struct BUF { int pad0[2]; char *p; int c; int pad1[2]; char *p2; int c2; int pad2[12]; };
extern struct BUF D_002DEA50[2];
extern int D_003D3980;
extern char D_003CF980[];
extern char D_002F7980[];
extern char D_00303980[];
void func_00234CA0(int n);
void SetPhase__F5PHASE(int mode)
{
    struct BUF *pb;
    int i;
    if (mode == D_003D3980) return;
    func_00234CA0(0);
    D_003D3980 = mode;
    switch (mode) {
    case 3:
        for (i = 0; i < 2; i++) {
            pb = &D_002DEA50[i];
            pb->p = D_002F7980 + i * 0x6000;
            pb->p2 = D_00303980 + i * 0x58000;
            pb->c = 0x600;
            pb->c2 = 0x5800;
        }
        break;
    case 2:
        for (i = 0; i < 2; i++) {
            pb = &D_002DEA50[i];
            pb->p = D_003CF980 + i * 0x1800;
            pb->p2 = D_003CF980 + 0x3000 + i * 0x800;
            pb->c = 0x180;
            pb->c2 = 0x80;
        }
        break;
    default:
        for (i = 0; i < 2; i++) {
            pb = &D_002DEA50[i];
            pb->c = 0;
            pb->p = 0;
            pb->c2 = 0;
            pb->p2 = 0;
        }
        break;
    }
}
