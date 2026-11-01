struct K { int a; int b; };
struct S { char pad[0x1f4]; int f1f4; int f1f8; int f1fc; };
void PostSoLoad(struct S *);
int func_001AE1D0(struct K *);
void PostBombLoad(struct S *p) {
    PostSoLoad(p);
    if (p->f1f4) { struct K k; k.a = 0xc0; k.b = 0x10; p->f1fc = func_001AE1D0(&k); }
}
