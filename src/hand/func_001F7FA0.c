struct P { char pad[0xA4]; int c; void **ap; };
int FIsBasicDerivedFrom__FP5BASIC3CID(void *, int);
void func_001EE678(void *, float, float);
void func_001F7FA0(struct P *p, float f, float g)
{
    int i;
    for (i = 0; i < p->c; i++) {
        void *q = p->ap[i];
        if (FIsBasicDerivedFrom__FP5BASIC3CID(q, 0x8F)) func_001EE678(q, f, g);
    }
}
