struct S { char pad[0xe8]; int fe8; };
extern char D_003DD490[];
void PostSoLoad(struct S *);
void RecalcSwOxfFilterForObject(void *, struct S *);
void PostBarrierLoad(struct S *p) { PostSoLoad(p); if (p->fe8) RecalcSwOxfFilterForObject(D_003DD490, p); }
