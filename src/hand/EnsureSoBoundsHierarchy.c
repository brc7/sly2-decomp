struct L { char pad[0x24]; struct L *next; char pad2[0x64 - 0x28]; struct L *first; char pad3[0x70 - 0x68]; struct L *end; };
void EnsureSoBounds(struct L *);
int FIsBasicDerivedFrom__FP5BASIC3CID(struct L *, int);
void EnsureSoBoundsHierarchy(struct L *p)
{
    struct L *q;
    EnsureSoBounds(p);
    for (q = p->first; q != p->end; q = q->next) {
        if (FIsBasicDerivedFrom__FP5BASIC3CID(q, 0x30)) EnsureSoBoundsHierarchy(q);
    }
}
