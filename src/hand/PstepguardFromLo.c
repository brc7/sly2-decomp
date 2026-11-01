struct O;
struct L { struct O *po; };
struct O { char pad[0x14]; struct L *p14; char pad2[0x1E8]; struct O *p200; };
int FIsBasicDerivedFrom__FP5BASIC3CID(struct O *p, int n);
struct O *PstepguardFromLo(struct O *p) {
    if (!p) return 0;
    if (FIsBasicDerivedFrom__FP5BASIC3CID(p->p14->po, 0x38)) return p->p14->po;
    if (FIsBasicDerivedFrom__FP5BASIC3CID(p, 0x4F) && FIsBasicDerivedFrom__FP5BASIC3CID(p->p200->p14->po, 0x38)) return p->p200->p14->po;
    return 0;
}
