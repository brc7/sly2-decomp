struct B { int a; void *p; };
int FIsBasicDerivedFrom__FP5BASIC3CID(void *p, int k);
int FFilterSoPair(void *a, struct B *b);
int func_0021FB18(void *a, struct B *b)
{
    if (FIsBasicDerivedFrom__FP5BASIC3CID(b->p, 0x59))
        return 1;
    return FFilterSoPair(a, b);
}
