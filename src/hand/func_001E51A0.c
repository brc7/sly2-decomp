struct VTE { short d; short pad; void (*pfn)(void *, int, int); };
struct O { int pad[3]; struct VTE *vt; };
struct P { char pad[0xA4]; int c; struct O *apo[4]; int aB8[4]; int aC8[4]; };
int FIsBasicDerivedFrom__FP5BASIC3CID(void *p, int cid);
void func_001E51A0(struct P *p, struct O *po, int a, int b)
{
    int i;
    if (!FIsBasicDerivedFrom__FP5BASIC3CID(po, 0x33)) return;
    for (i = 0; i < p->c; i++)
        if (p->apo[i] == po) return;
    if ((unsigned)p->c >= 4) return;
    p->aB8[p->c] = a;
    p->aC8[p->c] = b;
    p->apo[p->c] = po;
    p->c++;
    po->vt[0x53].pfn((char *)po + po->vt[0x53].d, a, b);
}
