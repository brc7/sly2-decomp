void *PloFindSwObject__FP2SWi3OIDP2LO(void *pv, int k, int id, void *ctx);
int FIsBasicDerivedFrom__FP5BASIC3CID(void *p, int cid);
void ShowSoGlobsById(void *p, void *q);
void HideSoGlobsById(void *p, void *q);
void func_00210408(void *pv, int f, int id, void *q, void *pdef, void *ctx)
{
    void *p;
    if (id == -1)
        p = pdef;
    else
        p = PloFindSwObject__FP2SWi3OIDP2LO(pv, 4, id, ctx);
    if (p && FIsBasicDerivedFrom__FP5BASIC3CID(p, 0x30)) {
        if (f)
            ShowSoGlobsById(p, q);
        else
            HideSoGlobsById(p, q);
    }
}
