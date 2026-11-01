struct CLOCK { float t; };
extern struct CLOCK D_002DDED0;
struct DL { void *a, *b; int c; };
extern struct DL D_003E1444;
struct S { char pad[0xFF8]; float t; };
void func_001F3E00(struct S *p);
void RemoveDlEntry__FP2DLPv(struct DL *pdl, void *pv);
void func_002079A0(struct S *p) { func_001F3E00(p); RemoveDlEntry__FP2DLPv(&D_003E1444, p); p->t = D_002DDED0.t; }
