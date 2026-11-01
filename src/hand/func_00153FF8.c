typedef struct CLOCK { float t; float dt; } CLOCK;
extern CLOCK D_002DDED0;
struct E { int n; float t; int pad[2]; };
struct S { char pad[0xFC]; int c; int pad100; struct E a[1]; };
/** @sly2-annot addr=0x153FF8
 * Purpose: Clears every target state to 0 and stamps the current clock time
 * Info: proposed name ResetDetectorTargets (T2); TU detector
 * Confidence: low (draft) [T2 0.45] - evidence: static analysis; sources: r3_1539A8
 * See: docs/reference/functions/guards-ai.md#detector-0x153d58-0x154a60
 * @end-sly2-annot */
void func_00153FF8(struct S *p)
{
    int i;
    for (i = 0; i < p->c; i++) {
        p->a[i].n = 0;
        p->a[i].t = D_002DDED0.t;
    }
}
