struct Q { char pad[0x10]; struct Q *next; };
extern int D_002C42E8[4];
int FIsBasicDerivedFrom__FP5BASIC3CID(struct Q *q, int a);
int func_00224508(void *p, struct Q *q)
{
    unsigned i;
    for (; q; q = q->next) {
        for (i = 0; i < 4; i++)
            if (FIsBasicDerivedFrom__FP5BASIC3CID(q, D_002C42E8[i])) return 0;
    }
    return 1;
}
