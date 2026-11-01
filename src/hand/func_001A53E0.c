extern char D_003DD490[];
struct S { char pad[0x14]; void *pv; };
int FFindDlEntry__FP2DLPv(void *, void *);
void RemoveDlEntry__FP2DLPv(void *, void *);
void func_00147E20(void *, void *);
void func_0011CD98(void *, void *);
void func_001A53E0(struct S *p)
{
    char *pdl = D_003DD490 + 0x2C;
    if (FFindDlEntry__FP2DLPv(pdl, p->pv))
        RemoveDlEntry__FP2DLPv(pdl, p->pv);
    func_00147E20(D_003DD490, p->pv);
    func_0011CD98(D_003DD490 + 0x398C, p->pv);
    p->pv = 0;
}
