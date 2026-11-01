typedef int qword __attribute__((mode(TI)));
typedef struct { short d; short pad; void *pfn; } VTE;
extern float D_002C3DA8;
struct O { char pad[0x50]; int k; };
struct S { char pad[0xC]; VTE *vt; };
int FStepguardHasSgf(struct S *p, int n);
struct O *func_00205458(struct S *p, int n);
void func_0017A8C8(struct O *o, int n);
void SetBhvjuggledState(struct O *o, int n);
qword LimitVectorLength(qword v, float g);
void func_00209EE0(struct S *p)
{
    if (FStepguardHasSgf(p, 0x12) || FStepguardHasSgf(p, 0x13)) {
        func_0017A8C8(func_00205458(p, 0xB0), 2);
    } else {
        struct O *o;
        if (!FStepguardHasSgf(p, 0x17))
            return;
        o = func_00205458(p, 0xB1);
        if (o->k != 3 && o->k != 6)
            SetBhvjuggledState(o, 2);
    }
    {
        qword v = ((qword (*)(void *))p->vt[33].pfn)((char *)p + p->vt[33].d);
        v = LimitVectorLength(v, D_002C3DA8);
        ((void (*)(void *, qword))p->vt[35].pfn)((char *)p + p->vt[35].d, v);
    }
}
