typedef struct { short d; short pad; void *pfn; } VTE;
struct O { char pad[0xC]; VTE *vt; };
struct S { int k; int pad; struct O *po; };
void func_00130B18(struct O *po);
void func_001F0598(struct O *po);
void func_0020C080(struct O *po);
void func_0020CA48(struct O *po);
#define VDEL(o) ((void (*)(void *))(o)->vt[2].pfn)((char *)(o) + (o)->vt[2].d)
/** @sly2-annot addr=0x209360
 * Purpose: Stop/release the running instance per kind (RetractAsega, RetractSma, ...) and Decref
 * Info: proposed name StopAnimBinding (T1); TU stepguard
 * Confidence: mid (draft) [T1 0.50] - evidence: static analysis; sources: r7b_guard
 * See: docs/reference/functions/guards-ai.md#stepguard-0x205e70-0x20b840
 * @end-sly2-annot */
void StopAnimBinding(struct S *p)
{
    switch (p->k) {
    case -1:
        break;
    case 0:
        if (p->po) {
            func_00130B18(p->po);
            VDEL(p->po);
            p->po = 0;
        }
        break;
    case 1:
        if (p->po) {
            func_001F0598(p->po);
            VDEL(p->po);
            p->po = 0;
        }
        break;
    case 2:
        if (p->po) {
            func_0020C080(p->po);
            VDEL(p->po);
            p->po = 0;
        }
        break;
    case 3:
        if (p->po) {
            func_0020CA48(p->po);
            VDEL(p->po);
            p->po = 0;
        }
        break;
    }
}
