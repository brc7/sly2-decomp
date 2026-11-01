typedef struct {
    char pad[0x150];
    int c1;
    void *a1[7];
    int c2;
    void *a2[8];
} OBJ;
int FMatchesLoName(void *, void *);
int FIsBasicDerivedFrom__FP5BASIC3CID(void *, void *);
int func_00134310(int, int);
int func_001DE2D8(OBJ *p, void *x)
{
    int i, j;
    int r = 1;
    for (i = 0; i < p->c1; i++) {
        if (FMatchesLoName(x, p->a1[i]))
            r = func_00134310(r, 2);
    }
    for (j = 0; j < p->c2; j++) {
        if (FIsBasicDerivedFrom__FP5BASIC3CID(x, p->a2[j]))
            r = func_00134310(r, 2);
    }
    return r;
}
