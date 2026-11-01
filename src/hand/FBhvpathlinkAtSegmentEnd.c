struct E { int a[8]; };
struct S { char pad[0x10]; void *p10; char pad2[0x1C]; char *p30; int n34; };
int func_001C1FA0(void *pe, void *p, float g);
int FBhvpathlinkAtSegmentEnd(struct S *p) { return func_001C1FA0(p->p30 + 0x60 + p->n34 * 32, p->p10, 25.0f); }
