struct G;
extern struct G D_003DD490;
struct N { int k; int b; char pad[0x10]; struct N *next; };
struct S { char pad[0x50]; struct N *pn; };
struct S *PloFindSwObject__FP2SWi3OIDP2LO(struct G *pg, int k, int b, struct S *p);
int func_001709A8(struct S *p);
void func_001E9C30(struct S *p);
void func_00170A58(struct S *p) {
    struct N *pn;
    for (pn = p->pn; pn; pn = pn->next) {
        switch (pn->k) {
        case 6: {
            struct S *o = PloFindSwObject__FP2SWi3OIDP2LO(&D_003DD490, 4, pn->b, p);
            if (o && func_001709A8(o) == -1) func_001E9C30(o);
            break;
        }
        case 0:
            p = PloFindSwObject__FP2SWi3OIDP2LO(&D_003DD490, p ? 4 : 5, pn->b, p);
            break;
        }
    }
}
