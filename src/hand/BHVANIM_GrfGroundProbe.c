struct U { char pad[0x10]; int n10; };
struct T { int n0; int b; struct U *p8; };
struct S { char pad[0x40]; struct T *p40; };
int func_001EEFA0(struct S *p);
int BHVANIM_GrfGroundProbe(struct S *p) {
    if (p->p40->n0 == 2) {
        switch (p->p40->p8->n10) { case 0: case 3: case 4: return 7; default: return 1; }
    }
    return func_001EEFA0(p);
}
