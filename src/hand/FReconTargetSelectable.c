struct O { char pad[0x28]; unsigned long fl; char pad2[0x24]; int n54; char pad3[0x94]; struct O *pEC; char pad4[0x10]; struct O *p100; };
#define BIT60(o) ((int)((o)->fl >> 60) & 1)
int FReconTargetSelectable(struct O *p)
{
    struct O *o;
    if (!BIT60(p)) return 0;
    if (p->p100 && BIT60(p->p100)) return 0;
    o = p->pEC;
    if (o && o->n54 != 1) return 0;
    return 1;
}
