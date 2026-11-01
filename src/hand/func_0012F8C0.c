typedef struct P { float x; float y; int n; } P;
void func_0012F9B8(void *a, float x, float y, int b, int n, int c);
/** @sly2-annot addr=0x12F8C0
 * Purpose: Message callback: applies the ASEG (ApplyAsegOvr 0x12F9B8) to the object recorded at load
 * Info: proposed name PostAsegLoadCallback (T1); TU aseg
 * Confidence: mid (draft) [T1 0.50] - evidence: static analysis; sources: r1_116918
 * See: docs/reference/functions/animation.md#aseg-0x12f248-0x1308a8
 * @end-sly2-annot */
void func_0012F8C0(void *a, P *p)
{
    func_0012F9B8(a, p->x, p->y, 0, p->n, 0);
}
