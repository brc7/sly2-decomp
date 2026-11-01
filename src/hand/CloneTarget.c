struct Q { char b[8]; };
struct S { char pad[0xE0]; struct Q q; };
void CloneXfm(struct S *p);
void CloneTarget(struct S *p)
{
    struct Q t = p->q;
    CloneXfm(p);
    p->q = t;
}
