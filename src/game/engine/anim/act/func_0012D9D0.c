typedef int qword __attribute__((mode(TI)));
typedef struct VT { short d; short pad; void (*pfn)(void *); } VT;
typedef struct O { char pad[0x18]; VT *vt; } O;
typedef struct T2 { char pad[0x1C]; O *po; qword v; } T2;
/** @sly2-annot addr=0x12D9D0
 * Purpose: vtable 0x2CA758 slot 5
 * Info: proposed name ActUnk_2CA758_5 (T2); TU act
 * Confidence: low (draft) [T2 0.20] - evidence: static analysis; sources: r1_116918
 * See: docs/reference/functions/animation.md#act-0x12b3b0-0x12e600
 * @end-sly2-annot */
void func_0012D9D0(T2 *p, qword *pv, void *x)
{
    O *po = p->po;
    ((void (*)(void *, qword *, void *))po->vt[5].pfn)((char *)po + po->vt[5].d, pv, x);
    if (pv) {
        register qword b __asm__("vf2") = p->v;
        register qword a __asm__("vf1") = *pv;
        __asm__ ("vadd.xyzw %0, %0, %1" : "+j"(a) : "j"(b));
        *pv = a;
    }
}
