struct S { char pad[0xe8]; int fe8; };
extern char D_003DD490[];
void func_001F36A8(struct S *);
void RecalcSwOxfFilterForObject(void *, struct S *);
void func_00132100(struct S *p) { func_001F36A8(p); if (p->fe8) RecalcSwOxfFilterForObject(D_003DD490, p); }
