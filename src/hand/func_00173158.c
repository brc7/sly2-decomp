struct S { char pad[0x80]; float g80; int n84; int n88; int n8C; char pad2[0x10]; float gA0; float gA4; };
void InitLo(struct S *p);
void func_00173158(struct S *p) { InitLo(p); p->g80 = 5500.0f; p->n84 = p->n8C = p->n88 = 1; p->gA0 = 30.0f; p->gA4 = 50.0f; }
