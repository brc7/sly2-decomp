struct DL { void *pvFirst; void *pvLast; int ibDle; };
struct O { char pad[0x1C4]; struct DL dl; char pad2[0x1D8 - 0x1D0]; void *powner; };
extern struct DL D_003E1A4C;
void AppendDlEntry__FP2DLPv(struct DL *pdl, void *pv);
void RemoveDlEntry__FP2DLPv(struct DL *pdl, void *pv);
int FFindDlEntry__FP2DLPv(struct DL *pdl, void *pv);
void func_001BD1E0(void *p);
void func_001BDE18(void *p, struct O *po)
{
    struct DL *pdl;
    *(struct O **)((char *)p + 0x1D8) = po;
    AppendDlEntry__FP2DLPv(&po->dl, p);
    func_001BD1E0(p);
    pdl = &D_003E1A4C;
    if (FFindDlEntry__FP2DLPv(pdl, p)) RemoveDlEntry__FP2DLPv(pdl, p);
}
