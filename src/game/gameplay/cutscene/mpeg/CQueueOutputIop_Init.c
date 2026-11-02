struct A { int pad[2]; int a, b, c, d; };
struct S { int n0; int a, b, c, d; int n14; int n18; };
int func_00232EE8(int a, int b, int c, int d, int e, int f);
void func_001B2A80(struct S *p);
/** @sly2-annot addr=0x1B29D0
 * Purpose: Init IOP output queue from audio params (0x10 = streaming mode), allocate IOP buffer (0x232EE8),
 *   Reset
 * Info: proposed name CQueueOutputIop_Init (T1); class CQueueOutputIop; TU mpeg
 * Confidence: mid (draft) [T1 0.75] - evidence: static analysis; sources: r5_1AE130
 * See: docs/engine/cutscenes.md#5-key-functions, docs/reference/functions/cutscenes.md#mpeg-0x1b2820-0x1b4438
 * @end-sly2-annot */
int CQueueOutputIop_Init(struct S *p, struct A *pa)
{
    p->a = pa->a;
    p->b = pa->b;
    p->c = pa->c;
    p->d = pa->d;
    if (p->a == 0x10) {
        p->n18 = p->c << 12;
        p->n14 = func_00232EE8(0x400, 0x1000, 0x400, 0, 2, 3);
    } else {
        p->n18 = 0x6000;
        p->n14 = func_00232EE8(0x6000, 0, 0x400, 0, 2, 2);
    }
    if (p->n14 < 0)
        return 0;
    func_001B2A80(p);
    return 1;
}
