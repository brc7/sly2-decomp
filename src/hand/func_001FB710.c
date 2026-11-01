struct VT { int pad[4]; short d; short p; void (*pfn)(void *); };
struct X { char pad[0xC]; struct VT *pvt; };
struct DL;
extern struct DL D_003DD08C;
int FFindDlEntry__FP2DLPv(struct DL *, void *);
void RemoveDlEntry__FP2DLPv(struct DL *, void *);
void func_001FB710(struct X *px)
{
    if (FFindDlEntry__FP2DLPv(&D_003DD08C, px)) {
        RemoveDlEntry__FP2DLPv(&D_003DD08C, px);
        px->pvt->pfn((char *)px + px->pvt->d);
    }
}
