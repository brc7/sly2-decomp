struct S { char pad[0x120]; int a; char pad2[0x2c]; int b; };
void InitLo(struct S *p);
void InitPathlink(struct S *p) { InitLo(p); p->a = -1; p->b = -1; }
