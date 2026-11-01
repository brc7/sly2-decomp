typedef int qword __attribute__((mode(TI)));
struct A { void *x0; void *x4; };
struct S { char pad[0xF0]; void *xF0; };
extern int D_002C1F70[1];
int FIsBasicDerivedFrom__FP5BASIC3CID(void *p, int n);
void func_001CF6B0(struct S *p, void *a, void *b, qword q0, qword q1);
void func_001CE980(struct S *p);
int func_001CF968(struct S *p, struct A *pa, qword q0, qword q1)
{
    unsigned int i;
    for (i = 0; i < sizeof(D_002C1F70) / sizeof(D_002C1F70[0]); i++) {
        if (FIsBasicDerivedFrom__FP5BASIC3CID(pa->x4, D_002C1F70[i])) return 0;
    }
    if (!FIsBasicDerivedFrom__FP5BASIC3CID(pa->x4, 0x59)) func_001CF6B0(p, p->xF0, pa->x0, q0, q1);
    func_001CE980(p);
    return 1;
}
