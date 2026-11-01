struct DL { void *pvFirst; void *pvLast; int ibDle; };
struct S { char pad[0x1D8]; void *powner; };
extern struct DL D_003E1A4C;
void RemoveLoHierarchy(struct S *p);
void func_001BD218(void *p);
void RemoveDlEntry__FP2DLPv(struct DL *pdl, void *pv);
int FFindDlEntry__FP2DLPv(struct DL *pdl, void *pv);
void func_001BD1E0(void *p);
void func_001BC198(struct S *p)
{
    struct DL *pdl;
    RemoveLoHierarchy(p);
    if (p->powner) func_001BD218(p->powner);
    func_001BD1E0(p);
    pdl = &D_003E1A4C;
    if (FFindDlEntry__FP2DLPv(pdl, p)) RemoveDlEntry__FP2DLPv(pdl, p);
}
