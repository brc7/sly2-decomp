struct S { char pad[0x4C]; int n4C; char pad2[0x90]; float gE0; };
int FBhvwalkpathStuck(struct S *p) { int f = 0; if (p->n4C < 2) { if (p->n4C >= 0) f = p->gE0 < 0.0f; } return f; }
