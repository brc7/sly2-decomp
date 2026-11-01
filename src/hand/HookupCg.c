typedef int qword __attribute__((mode(TI)));
struct F { qword *pq; int d; };
struct E { qword pos; int c; struct F *af; };
struct A { int c; struct E *ae; };
static inline int Dist(qword a, qword b)
{
    qword d;
    register int f __asm__("$2");
    __asm__("vsub.xyzw %0, %1, %2" : "=j"(d) : "j"(a), "j"(b));
    __asm__("vmul.xy %0, %1, %1" : "=j"(d) : "j"(d));
    __asm__("vaddy.x %0, %1, %1y" : "=j"(d) : "j"(d));
    __asm__("vsqrt $Q, %1x\n\tvwaitq\n\tvaddq.x %0, $vf0, $Q" : "=j"(d) : "j"(d));
    __asm__("qmfc2.ni %0, %1" : "=r"(f) : "j"(d));
    return f;
}
void HookupCg(struct A *pa)
{
    int i, j;
    for (i = 0; i < pa->c; i++) {
        struct E *pe = &pa->ae[i];
        for (j = 0; j < pe->c; j++) {
            register struct F *field __asm__("$3") = pe->af;
            field += j;
            field->d = Dist(pe->pos, *field->pq);
        }
    }
}
