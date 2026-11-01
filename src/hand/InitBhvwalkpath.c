struct S { char pad[0xC0]; int nC0; float gC4; char pad2[0x18]; float gE0; };
extern float D_002BE694;
void InitBhvpath(struct S *p);
void InitBhvwalkpath(struct S *p) { InitBhvpath(p); p->gE0 = D_002BE694; p->gC4 = 1.0f; p->nC0 = 9; }
