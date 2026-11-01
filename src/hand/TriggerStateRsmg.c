struct E { int n0; int oid; int n8; int id; int f10; int f14; };
struct G;
extern struct G D_003DD490;
int func_00210A88(void);
int CploFindSwObjects_15E650(struct G *pg, int kind, int oid, int a, int c, void **apv);
int FIsBasicDerivedFrom__FP5BASIC3CID(void *pv, int n);
void *func_001F67E0(void *pv, int n);
void func_001F0948(void *p, int id);
void func_001F0660(void *p, int id);
void TriggerStateRsmg(void *pvUnused, struct E *pe, int n, int k)
{
    void *pv;
    void *ps;
    int id;
    if (pe->n0 != -1 && pe->n0 != n) return;
    if (pe->f10 && !k) return;
    if (pe->f14 != -1 && pe->f14 != func_00210A88()) return;
    if (CploFindSwObjects_15E650(&D_003DD490, 0x205, pe->oid, 0, 1, &pv) != 1) return;
    if (!FIsBasicDerivedFrom__FP5BASIC3CID(pv, 0x30)) return;
    ps = func_001F67E0(pv, pe->n8);
    if (!ps) return;
    id = pe->id;
    if (pe->n0 == -1) {
        switch (n) {
        case 0: id = 0x612; break;
        case 1: id = 0x613; break;
        case 2: id = 0x614; break;
        case 3: id = 0x615; break;
        }
    }
    if (id != -1) {
        if (k == 1 || k == 4)
            func_001F0948(ps, id);
        else
            func_001F0660(ps, id);
    }
}
