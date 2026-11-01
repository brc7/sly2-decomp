typedef int qword __attribute__((mode(TI)));
struct S { char pad[0xEC]; int k; char pad2[0x20]; qword pos; float r; };
qword ConvertAloPos(int a, struct S *p, qword q);
int PbspPointInBspQuick(int a, int b, qword q);
int func_00227C90(struct S *p, qword q)
{
    qword a = ConvertAloPos(0, p, q), d, r, t;
    float f;
    int fFar;
    __asm__("vsub.xyzw %0, %1, %2" : "=j"(d) : "j"(a), "j"(p->pos));
    __asm__("vmul.xyz %0, %0, %0\n\tvaddw.x %1, $vf0, $vf0w\n\tvadday.x $ACC, %0, %0y\n\tvmaddz.x %0, %1, %0z" : "=j"(r), "=&j"(t) : "0"(d));
    __asm__("qmfc2.ni %0, %1" : "=r"(f) : "j"(r));
    fFar = p->r < f;
    if (fFar) return 0;
    return PbspPointInBspQuick(p->k, 0x7FFFFFFF, a) != 0;
}
