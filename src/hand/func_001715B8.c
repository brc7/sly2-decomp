struct S { char pad[0x34]; int n34; };
struct G { int a; };
extern struct G D_003DD490;
void *PloFindSwObject__FP2SWi3OIDP2LO(struct G *pg, int kind, int oid, int owner);
void *func_001715B8(struct S *p, int oid) { int o = p->n34; int k = o ? 4 : 5; return PloFindSwObject__FP2SWi3OIDP2LO(&D_003DD490, k, oid, o); }
