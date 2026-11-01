struct S { char pad[0xF4]; int a; int b; int c; char pad2[0x10]; int d; };
void InitXfm(struct S *p);
void InitReconTarget(struct S *p) { InitXfm(p);
p->a = -1; //P
p->d = -1; //P
p->c = -1; //P
p->b = -2; //P
}
