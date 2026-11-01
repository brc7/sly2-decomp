extern float D_002A8868;
struct DL { void *a, *b; int c; };
extern struct DL D_003E1444;
struct S { char pad[0xFF8]; float t; };
void func_001F3EA8(struct S *p);
void AppendDlEntry__FP2DLPv(struct DL *pdl, void *pv);
void func_002079E0(struct S *p) { func_001F3EA8(p); AppendDlEntry__FP2DLPv(&D_003E1444, p); p->t = D_002A8868; }
