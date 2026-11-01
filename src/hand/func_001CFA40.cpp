typedef int qword __attribute__((mode(TI)));
struct M { qword a, b, c; };
struct S { char pad[0x40]; M m; qword q70; char pad2[0xF0 - 0x80]; void *p; };
extern "C" {
qword func_0022FC48(void *p, int n, qword q);
M ConvertXfMat(void *p, int n, M *pm);
int func_001CF338(S *p, qword q, M m);
void func_001CFA40(S *p);
}
void func_001CFA40(S *p)
{
    qword q = func_0022FC48(p->p, 0, p->q70);
    M m;
    m = ConvertXfMat(p->p, 0, &p->m);
    func_001CF338(p, q, m);
}
