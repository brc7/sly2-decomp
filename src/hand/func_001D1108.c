struct O { char p0[8]; int *pn; char p1[0xF0-0xC]; int oid; float gF4; };
extern struct G D_003DD490;
void func_001CE9B8(struct O *p, struct O *q);
int PloFindSwObject__FP2SWi3OIDP2LO(struct G *g, int k, int oid, int n);
void func_001D1108(struct O *p, struct O *q) {
    func_001CE9B8(p, q);
    p->oid = PloFindSwObject__FP2SWi3OIDP2LO(&D_003DD490, 4, q->oid, *q->pn);
    p->gF4 = q->gF4;
}
