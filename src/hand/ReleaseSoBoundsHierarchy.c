struct R { int pad[2]; int c; };
struct L { char pad[0x24]; struct L *next; char pad2[0x64 - 0x28]; struct L *first; char pad3[0x70 - 0x68]; struct L *end; char pad4[0x134 - 0x74]; struct R *pr; };
struct Pool;
extern struct Pool D_003E12D4;
void func_0011CD98(struct Pool *, struct R *);
int FIsBasicDerivedFrom__FP5BASIC3CID(struct L *, int);
void ReleaseSoBoundsHierarchy(struct L *p)
{
    struct L *q;
    struct R *pr = p->pr;
    if (pr) {
        if (--pr->c == 0) func_0011CD98(&D_003E12D4, pr);
        p->pr = 0;
    }
    for (q = p->first; q != p->end; q = q->next) {
        if (FIsBasicDerivedFrom__FP5BASIC3CID(q, 0x30)) ReleaseSoBoundsHierarchy(q);
    }
}
