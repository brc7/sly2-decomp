struct G;
extern struct G D_003DD490;
struct S { char pad[0xC0]; int oid; };
const char *PchzFindSwLangstr__FP2SWi(struct G *p, int oid);
const char *func_002026F8(struct S *p) { if (p->oid == -1) return 0; return PchzFindSwLangstr__FP2SWi(&D_003DD490, p->oid); }
