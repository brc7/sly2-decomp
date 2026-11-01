struct L { int x; int c; void *a[1]; };
int FIsBasicDerivedFrom__FP5BASIC3CID(void *p, void *key);
void *func_001EEEE0(struct L *pl, void *key)
{
    int i;
    for (i = 0; i < pl->c; i++) {
        void *p = pl->a[i];
        if (FIsBasicDerivedFrom__FP5BASIC3CID(p, key)) return p;
    }
    return 0;
}
