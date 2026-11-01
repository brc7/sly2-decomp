typedef int qword __attribute__((mode(TI)));
struct C { char pad[0x60]; void *p60; };
struct B { char pad[0xEC]; struct C *pc; };
struct A { int x0; struct B *pb; };
int FIsBasicDerivedFrom__FP5BASIC3CID(void *p, int n);
int func_001CEB08(void *p, struct A *pa, qword q0, qword q1);
void func_001CE980(void *p);
int func_001CFBE0(void *p, struct A *pa, qword q0, qword q1)
{
    void *plo = pa->pb->pc->p60;
    if (FIsBasicDerivedFrom__FP5BASIC3CID(plo, 0x32)) return 0;
    if (FIsBasicDerivedFrom__FP5BASIC3CID(plo, 0x59)) {
        func_001CE980(p);
        return 1;
    }
    return func_001CEB08(p, pa, q0, q1);
}
