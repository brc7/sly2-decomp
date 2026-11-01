struct S { int a; void *o; char pad[8]; long fl; };
int FIsBasicDerivedFrom__FP5BASIC3CID(void *o, int n);
int func_0020FA50(void *pv, struct S *p);
int func_0020CFC0(void *pv, struct S *p) { if ((p->fl & 0x1000000000000L) && FIsBasicDerivedFrom__FP5BASIC3CID(p->o, 0x59)) return 1; return func_0020FA50(pv, p); }
