typedef unsigned long ulong;
struct DL { void *a, *b; int c; };
extern struct DL D_003DD4BC;
int FFindDlEntry__FP2DLPv(struct DL *pdl, void *pv);
struct S { char pad[0x14]; void *x14; char pad2[0x10]; ulong fl; };
int func_001A61A0(struct S *p)
{
    int r = 0;
    if ((int)(p->fl >> 60) & 1) r = FFindDlEntry__FP2DLPv(&D_003DD4BC, p->x14) != 0;
    return r;
}
