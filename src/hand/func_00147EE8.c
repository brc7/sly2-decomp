void func_0011A2E8(void *p, int a);
void func_0011A220(void *p, int a);
struct X { char pad[0x20]; char sub[0x39A4 - 0x20]; int n; };
/** @sly2-annot addr=0x147EE8
 * Purpose: Move entry to the end of a sub-list, recording the first
 * Info: proposed name MoveBusyEntryToEnd (T2); TU busy
 * Confidence: low (draft) [T2 0.35] - evidence: static analysis; sources: r2b_rest
 * See: docs/reference/functions/object-model.md#busy-0x147e20-0x148620
 * @end-sly2-annot */
void func_00147EE8(struct X *p, int a)
{
    func_0011A2E8(p->sub, a);
    if (p->n == 0) p->n = a;
    func_0011A220(p->sub, a);
}
