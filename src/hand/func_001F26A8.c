struct L { char pad[0x24]; struct L *next; char pad2[0x64 - 0x28]; struct L *first; char pad3[0x70 - 0x68]; struct L *end; };
void func_001F2500(struct L *);
int FIsBasicDerivedFrom__FP5BASIC3CID(struct L *, int);
void func_001F26A8(struct L *p)
{
    struct L *q;
    func_001F2500(p);
    for (q = p->first; q != p->end; q = q->next) {
        if (FIsBasicDerivedFrom__FP5BASIC3CID(q, 0x30)) func_001F26A8(q);
    }
}
