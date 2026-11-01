struct O { char pad[0x40]; float g; };
struct P { char pad[0x30]; struct O *po; };
void func_00130D30(struct O *, int, float, float);
void func_00130B18(struct O *);
void EndSmaTransition(struct P *);
void AbortSmaTransition(struct P *p)
{
    struct O *po = p->po;
    if (po) {
        p->po = 0;
        if (po->g < 0.0f) func_00130D30(po, 0, 0.0f, po->g);
        else func_00130D30(po, 2, 0.0f, po->g);
        func_00130B18(po);
        EndSmaTransition(p);
    }
}
