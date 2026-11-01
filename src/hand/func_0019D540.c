struct G;
extern struct G D_003DD490;
struct Q { int x0; };
struct P { struct Q *x0; int x4; void *x8; void *xc; int x10; void *x14; };
int FIsBasicDerivedFrom__FP5BASIC3CID(int a, int b);
void *PloFindSwObject__FP2SWi3OIDP2LO(struct G *g, int k, int oid, int z);
void PostReconSelLoad(void *p);
void func_0019DCA0(struct P *p, void *q);
void func_0019D540(struct P *p)
{
    if (FIsBasicDerivedFrom__FP5BASIC3CID(p->x0->x0, 0x35)) {
        p->x8 = PloFindSwObject__FP2SWi3OIDP2LO(&D_003DD490, 0x405, 0x35F, 0);
        p->xc = PloFindSwObject__FP2SWi3OIDP2LO(&D_003DD490, 0x405, 0x368, 0);
    } else if (FIsBasicDerivedFrom__FP5BASIC3CID(p->x0->x0, 0x37)) {
        p->x8 = PloFindSwObject__FP2SWi3OIDP2LO(&D_003DD490, 0x405, 0x360, 0);
        p->xc = PloFindSwObject__FP2SWi3OIDP2LO(&D_003DD490, 0x405, 0x369, 0);
    } else if (FIsBasicDerivedFrom__FP5BASIC3CID(p->x0->x0, 0x36)) {
        p->x8 = PloFindSwObject__FP2SWi3OIDP2LO(&D_003DD490, 0x405, 0x361, 0);
        p->xc = PloFindSwObject__FP2SWi3OIDP2LO(&D_003DD490, 0x405, 0x36A, 0);
    }
    if (p->x14)
        PostReconSelLoad(p->x14);
    func_0019DCA0(p, p->xc);
}
