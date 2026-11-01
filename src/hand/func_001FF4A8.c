struct K { int a, b; };
struct S { char pad[0x104]; struct K k; };
void CloneXfm(struct S *);
void func_001FF4A8(struct S *p) { struct K k = p->k; CloneXfm(p); p->k = k; }
