typedef int qword __attribute__((mode(TI)));
struct LO { char pad[0x218]; qword *pq; };
int FIsBasicDerivedFrom__FP5BASIC3CID(struct LO *p, int n);
qword func_001C1EC0(void *p, qword q);
int func_001C1FA0(void *p, struct LO *plo, float r)
{
    qword a, b, d;
    float f;
    if (FIsBasicDerivedFrom__FP5BASIC3CID(plo, 0x40)) return 1;
    a = *plo->pq;
    b = func_001C1EC0(p, a);
    __asm__("vsub.xyzw %0, %1, %2\n\tvmul.xy %0, %0, %0\n\tvaddy.x %0, %0, %0y\n\tvsqrt $Q, %0x\n\tvwaitq\n\tvaddq.x %0, $vf0, $Q" : "=j"(d) : "j"(a), "j"(b));
    __asm__("qmfc2.ni %0, %1" : "=r"(f) : "j"(d));
    return f <= r;
}
