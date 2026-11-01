typedef struct { short d; short pad; void *pfn; } VTE;
struct S { char pad[0xC]; VTE *vt; };
#define VT(p, n, T) ((T)(p)->vt[n].pfn)
struct REQUEST { int value; int *pointer; };
/** @sly2-annot addr=0x206820
 * Purpose: vt27(0), message 0xF, vt26(0), message 0xE
 * Info: proposed name RestartStepguard (T2); TU stepguard
 * Confidence: low (draft) [T2 0.25] - evidence: static analysis; sources: r7b_guard
 * See: docs/reference/functions/guards-ai.md#stepguard-0x205e70-0x20b840
 * @end-sly2-annot */
void func_00206820(struct S *p)
{
    struct REQUEST request;
    register VTE *initialTable __asm__("$3");
    request.pointer=&request.value;
    initialTable=p->vt;
    request.value=0;
    ((void (*)(void*,int))initialTable[27].pfn)((char*)p+initialTable[27].d,0);
    VT(p, 3, void (*)(void *, int, int, int, int **, int))((char *)p + p->vt[3].d, 0xF, 0, 1, &request.pointer, 0);
    VT(p, 26, void (*)(void *, int))((char *)p + p->vt[26].d, 0);
    VT(p, 3, void (*)(void *, int, int, int, int **, int))((char *)p + p->vt[3].d, 0xE, 0, 1, &request.pointer, 0);
}
