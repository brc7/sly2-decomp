struct VTE { short d; short pad; void (*pfn)(); };
struct O { char pad[0x18]; struct VTE *vt; };
struct S { char pad[0x80]; float gMin; float gMax; float *pg; int pad1; struct O *po; int pad2; short k; short state; };
void func_0022C818(void *pa, struct S *p);
void func_002303A0(void *pa, struct S *p, int state);
/** @sly2-annot addr=0x22DD00
 * Purpose: limits/advances a job value against gMin/gMax
 * Info: proposed name UpdateXfJobRange (T2); TU xf
 * Confidence: low (draft) [T2 0.25] - evidence: static analysis; sources: r8_21BB88
 * See: docs/reference/functions/animation.md#xf-0x22c818-0x231100
 * @end-sly2-annot */
void func_0022DD00(void *pa, struct S *p)
{
    float g = *p->pg;
    if (g < p->gMin || p->gMax < g || p->state >= 3) {
        if (p->po) {
            p->po->vt[4].pfn((char *)p->po + p->po->vt[4].d);
            switch (p->k) {
            case 10:
                if (p->state == 3 || p->state == 4) func_002303A0(pa, p, 7);
                else if (p->state != 2) func_002303A0(pa, p, 6);
                break;
            case 6:
                if (p->state == 3 || p->state == 4) func_002303A0(pa, p, 7);
                break;
            case 19:
                if (p->state != 2) func_002303A0(pa, p, 0x12);
                break;
            case 12:
                if (p->state != 2) func_002303A0(pa, p, 0xB);
                break;
            }
        } else {
            p->state = 0;
            func_0022C818(pa, p);
            func_002303A0(pa, p, 0);
        }
    }
}
