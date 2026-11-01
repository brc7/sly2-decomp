typedef int qword __attribute__((mode(TI)));
typedef struct { short d; short pad; void *pfn; } VTE;
typedef union { qword q; float f[4]; } U;
struct B { char pad[0xB0]; qword v; };
struct S { char pad[0xC]; VTE *vt; char pad2[0x5C - 0x10]; struct B *pb; char pad3[0x1A8 - 0x60]; float g; char pad4[0x1F4 - 0x1AC]; int x; };
float func_002458F0(float y, float x);
void func_0011D2D0(float a, float b, float c, int *p, float *pf);
/** @sly2-annot addr=0x204CA8
 * Purpose: RadSmooth(heading, radTarget, 0, &smpSpin, &w.z) then SetAngularVelocity (vt36)
 * Info: proposed name FixStepAngularVelocity (T1); TU step
 * Confidence: high [T1 0.80] - evidence: static analysis; sources: r7b_guard
 * See: docs/reference/functions/guards-ai.md#step-0x203948-0x205e70
 * @end-sly2-annot */
void FixStepAngularVelocity(struct S *p)
{
    U v;
    float *a, *b;
    v.q = p->pb->v;
    a = ((float *(*)(void *))p->vt[31].pfn)((char *)p + p->vt[31].d);
    b = ((float *(*)(void *))p->vt[31].pfn)((char *)p + p->vt[31].d);
    func_0011D2D0(func_002458F0(a[1], b[0]), p->g, 0.0f, &p->x, &v.f[2]);
    ((void (*)(void *, qword))p->vt[36].pfn)((char *)p + p->vt[36].d, v.q);
}
