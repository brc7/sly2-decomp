struct E { struct E *next; int x; int pad[3]; };
extern struct E D_002BAAE8[];
int FIsCidDerivedFrom(int i, int x)
{
    struct E *p;
    for (p = &D_002BAAE8[i]; p; p = p->next)
        if (p->x == x)
            return 1;
    return 0;
}
