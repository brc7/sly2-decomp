struct K { int a, b; };
struct S { char pad[0xE0]; struct K k; };
void CloneXfm(struct S *);
void CloneJmt(struct S *p) { struct K k = p->k; CloneXfm(p); p->k = k; }
