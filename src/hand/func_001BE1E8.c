struct DL { void *pvFirst; void *pvLast; int ibDle; };
extern struct DL D_003E1A4C;
void RemoveDlEntry__FP2DLPv(struct DL *pdl, void *pv);
void func_001BD040(void *pv);
int FIsDlEmpty__FP2DL(struct DL *pdl);
void func_001BE1E8(void)
{
    while (!FIsDlEmpty__FP2DL(&D_003E1A4C)) {
        void *pv = D_003E1A4C.pvFirst;
        RemoveDlEntry__FP2DLPv(&D_003E1A4C, pv);
        func_001BD040(pv);
    }
}
