void *PloFindSwObject__FP2SWi3OIDP2LO(void *pv, int k, int id);
void *func_001EC918(int id);
int FIsBasicDerivedFrom__FP5BASIC3CID(void *p, int cid);
void func_001F80D0(void *p, float f);
void func_001EE3B0(void *p, float f);
void func_00210678(void *pv, int id1, int id2, float f)
{
    void *p1 = 0;
    void *p2 = 0;
    if (id1 != -1) {
        void *q = PloFindSwObject__FP2SWi3OIDP2LO(pv, 4, id1);
        if (q && FIsBasicDerivedFrom__FP5BASIC3CID(q, 0x30))
            p1 = q;
    }
    if (id2 != -1) {
        p2 = func_001EC918(id2);
        if (p2 && !FIsBasicDerivedFrom__FP5BASIC3CID(p2, 0x8E))
            p2 = 0;
    }
    if (p1)
        func_001F80D0(p1, f);
    if (p2)
        func_001EE3B0(p2, f);
}
