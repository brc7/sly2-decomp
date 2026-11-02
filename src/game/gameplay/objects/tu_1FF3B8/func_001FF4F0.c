typedef int qword __attribute__((mode(TI)));
struct P { char pad[0x240]; qword pos; };
qword func_001A62E8(void *p);
/** @sly2-annot addr=0x1FF4F0
 * Purpose: *pv = PosLoWorld(a) - p->pos when pv non-NULL
 * Info: proposed name GetCid27VecToLo (T2); class CID27; TU tu_1FF3B8
 * Confidence: low (draft) [T2 0.25] - evidence: static analysis; sources: r7_1F8DD8
 * See: docs/reference/functions/gameplay-objects.md#tu_1ff3b8-0x1ff3b8-0x1ff690
 * @end-sly2-annot */
void func_001FF4F0(void *a, struct P *p, qword *pv)
{
    qword a0, r;
    if (pv) {
        a0 = func_001A62E8(a);
        __asm__("vsub.xyzw %0, %1, %2" : "=j"(r) : "j"(a0), "j"(p->pos));
        *pv = r;
    }
}
