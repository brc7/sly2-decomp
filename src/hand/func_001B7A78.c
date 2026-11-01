typedef int qword __attribute__((mode(TI)));
struct P { char pad[0x1420]; qword q1420; };
void SetMurrayState(struct P *p, int n);
qword func_001A62E8(void *p);
void SetStepRadTargetVec(struct P *p, qword q);
void func_001B7A78(struct P *p, void *pv)
{
    qword a, b, d;
    SetMurrayState(p, 7);
    if (pv) {
        a = func_001A62E8(pv);
        p->q1420 = a;
        b = func_001A62E8(p);
        __asm__("vsub.xyzw %0, %1, %2" : "=j"(d) : "j"(a), "j"(b));
        SetStepRadTargetVec(p, d);
    }
}
