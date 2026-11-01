typedef int qword __attribute__((mode(TI)));
struct S { qword v; int k; };
static inline qword Proj(qword a, qword n)
{
    qword r, one, d;
    __asm__("vmul.xyz %2, %3, %4\n\tvaddw.x %1, $vf0, $vf0w\n\tvadday.x $ACC, %2, %2y\n\tvmaddz.x %2, %1, %2z\n\tvmulx.xyzw %0, %4, %2x" : "=j"(r), "=&j"(one), "=&j"(d) : "j"(a), "j"(n));
    return r;
}
static inline qword Rej(qword a, qword n)
{
    qword r, one, d;
    __asm__("vmul.xyz %2, %3, %4\n\tvaddw.x %1, $vf0, $vf0w\n\tvadday.x $ACC, %2, %2y\n\tvmaddz.x %2, %1, %2z\n\tvmulx.xyzw %2, %4, %2x\n\tvsub.xyzw %0, %3, %2" : "=j"(r), "=&j"(one), "=&j"(d) : "j"(a), "j"(n));
    return r;
}
qword ApplyConstr_1FA970(struct S *p, qword v)
{
    switch (p->k) {
    case 3:
        __asm__("vsub.xyzw %0, $vf0, $vf0" : "=j"(v));
        break;
    case 2:
        v = Proj(v, p->v);
        break;
    case 1:
        v = Rej(v, p->v);
        break;
    }
    return v;
}
