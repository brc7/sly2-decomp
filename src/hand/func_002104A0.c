void *PloFindSwObject__FP2SWi3OIDP2LO(void *pv, int k, int id, void *ctx);
int FIsBasicDerivedFrom__FP5BASIC3CID(void *p, int cid);
void func_001F6AC8(void *p, void *q);
void func_001F6B48(void *p, void *q);
void func_002104A0(void *pv, int f, int id, void *q, void *pdef, void *ctx)
{
    void *p;
    if (id == -1)
        p = pdef;
    else
        p = PloFindSwObject__FP2SWi3OIDP2LO(pv, 4, id, ctx);
    if (p && FIsBasicDerivedFrom__FP5BASIC3CID(p, 0x30)) {
        if (f)
            func_001F6AC8(p, q);
        else
            func_001F6B48(p, q);
    }
}
