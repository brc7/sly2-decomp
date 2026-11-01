typedef struct S { char pad[0x18]; int x18; } S;
extern struct G D_003DD490;
void *PloFindSwObject__FP2SWi3OIDP2LO(struct G *g, int k, int id, int x);
int FIsBasicDerivedFrom__FP5BASIC3CID(void *p, int k);
void *func_001CB2E0(void *p);
void *func_001E1DB8(S *p, int id)
{
    void *q;
    if (id == -1) {
        q = 0;
    } else {
        q = PloFindSwObject__FP2SWi3OIDP2LO(&D_003DD490, 4, id, p->x18);
        if (q && FIsBasicDerivedFrom__FP5BASIC3CID(q, 6)) q = func_001CB2E0(q);
    }
    return q;
}
