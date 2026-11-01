struct S { char pad[0x18]; void *p18; char pad2[0x14]; int n30; };
int FBhvpathFinished(void *p);
int FBhvgotoPathFinished(struct S *p) { if (p->n30 >= 3) return 0; if (p->n30 <= 0) return 0; return FBhvpathFinished(p->p18); }
