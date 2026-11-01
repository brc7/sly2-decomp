struct N { char pad[0x10]; struct N *pnext; };
extern int D_002BC9D0[4];
int FIsBasicDerivedFrom__FP5BASIC3CID(struct N *, int);
int func_0013B1C8(void *unused, struct N *pn) {
    unsigned int i;
    for (; pn; pn = pn->pnext) {
        for (i = 0; i < 4; i++) {
            if (FIsBasicDerivedFrom__FP5BASIC3CID(pn, D_002BC9D0[i])) return 0;
        }
    }
    return 1;
}
