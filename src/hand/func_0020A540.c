struct O { char pad[0x30]; int n30; };
struct L { int k; int rest[27]; };
struct S { char pad[0x26C]; char x26C[0x2DC - 0x26C]; int a2DC[3]; int n2E8; };
void *func_002333B4(void *p, int c, int n);
struct O *PbhvFindBhvstk(void *p, int n);
void func_00176750(struct O *po, struct L *pl);
void func_0020A540(struct S *p, int n)
{
    if (p->n2E8 != n) {
        struct O *po;
        p->n2E8 = n;
        func_002333B4(p->a2DC, 0, 0xC);
        po = PbhvFindBhvstk(p->x26C, 0x9C);
        if (po) {
            int k = po->n30;
            if (k == 1) {
                struct L l;
                func_002333B4(&l, 0, sizeof(l));
                l.k = -1;
                func_00176750(po, &l);
                func_002333B4(p->a2DC, 0, 0xC);
                l.k = k;
                func_00176750(po, &l);
            }
        }
    }
}
