typedef int qword __attribute__((mode(TI)));
qword func_001A62E8(void *p);
void SetStepRadTargetVec(void *p, qword v);
void FaceStepToward(void *p, void *q)
{
    qword a, b, d;
    a = func_001A62E8(q);
    b = func_001A62E8(p);
    __asm__("vsub.xyzw %0, %1, %2" : "=j"(d) : "j"(a), "j"(b));
    SetStepRadTargetVec(p, d);
}
