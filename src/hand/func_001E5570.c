struct O { char pad[0x26C]; int a; };
struct P { char pad[0x24]; struct O *po; int *pnMin; int *pnMax; int *pn; };
int FIsBasicDerivedFrom__FP5BASIC3CID(struct O *po, int cid);
void ClearBhvstk__FP6BHVSTK(void *p);
void func_001E5570(struct P *p)
{
    if (p->pnMin < p->pn && p->pn < p->pnMax) {
        switch (*p->pn) {
        case 0x27:
            if (p->po && FIsBasicDerivedFrom__FP5BASIC3CID(p->po, 0x34)) ClearBhvstk__FP6BHVSTK(&p->po->a);
            break;
        case 0x28:
            if (p->po && FIsBasicDerivedFrom__FP5BASIC3CID(p->po, 0x34)) ClearBhvstk__FP6BHVSTK(&p->po->a);
            break;
        }
    }
}
