struct S { char pad[0x24]; struct S *pnext; unsigned long f28lo : 48; unsigned long f28b : 4; unsigned long f28hi : 12; char pad2[0x64 - 0x30]; struct S *pchild; };
int FIsBasicDerivedFrom__FP5BASIC3CID(struct S *, int);
void func_0013F990(struct S *p, int n) {
    struct S *pc;
    p->f28b = n;
    if (FIsBasicDerivedFrom__FP5BASIC3CID(p, 0x30)) {
        for (pc = p->pchild; pc; pc = pc->pnext) func_0013F990(pc, n);
    }
}
