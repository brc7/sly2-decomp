struct L { char pad[0x24]; struct L *next; char pad2[0x64 - 0x28]; struct L *first; char pad3[0x70 - 0x68]; struct L *end; char pad4[0xA4 - 0x74]; int c; void **ap; };
int FIsBasicDerivedFrom__FP5BASIC3CID(void *, int);
void func_001EE3B0(void *, float);
void func_001F80D0(struct L *p, float f)
{
    int i;
    struct L *q;
    for (i = 0; i < p->c; i++) {
        void *pv = p->ap[i];
        if (FIsBasicDerivedFrom__FP5BASIC3CID(pv, 0x8E)) func_001EE3B0(pv, f);
    }
    for (q = p->first; q != p->end; q = q->next)
        if (FIsBasicDerivedFrom__FP5BASIC3CID(q, 0x30)) func_001F80D0(q, f);
}
