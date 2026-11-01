struct S { char pad[0x154]; float f154; };
extern float D_002DDED0;
void PrimeBomb__FP4BOMBf(struct S *p, float f) { p->f154 = (p->f154 < D_002DDED0 + f) ? p->f154 : D_002DDED0 + f; }
