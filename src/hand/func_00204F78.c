struct G;
extern struct G D_003DD490;
struct O { char pad[0x10]; struct O *pParent; char p2[0xEC - 0x14]; void *pvEC; };
void *PsoFromLo(void *pv);
void UpdateSwPox(struct G *pg, void *a, void *b, int n);
void func_001F4BF8(struct O *p);
void func_00204F78(struct O *p, void *pv, int f)
{
    struct O *o = PsoFromLo(pv);
    while (o && !o->pvEC) o = o->pParent;
    if (p->pvEC && o && o->pvEC) UpdateSwPox(&D_003DD490, p->pvEC, o->pvEC, f ? 1 : 2);
    func_001F4BF8(p);
}
