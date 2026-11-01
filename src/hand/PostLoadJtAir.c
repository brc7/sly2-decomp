struct S { int a; char pad[0x17C-4]; char b[0x1B8-0x17C]; char c[4]; };
void EnsureAsegBlendDynamic__FP3ALOiiPviPiT5PP6ASEGBL(int, int, int, void *, int, int, int, void *);
void PostLoadJtAir(struct S *s) { EnsureAsegBlendDynamic__FP3ALOiiPviPiT5PP6ASEGBL(s->a, 0xC, 5, s->b, 0, 0, 0, s->c); }
