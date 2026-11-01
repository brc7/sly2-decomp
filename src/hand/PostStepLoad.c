struct S { char pad[0x58]; int n58; int n5C; char pad2[0x294 - 0x60]; int x294; };
struct O { char pad[0x264]; int oid; void *p268; char x26C[0x28]; int x294; };
void PostSoLoad(struct O *p);
int func_001E94B0(int a, int b);
void func_002303A0(struct O *p, int n, int f);
void *PsopartFindSo(struct O *p, int oid);
void func_00203AF0(struct O *p, void *pv);
void HideSoPart(struct O *p, void *pv);
void PostBhvstkLoad(void *pv);
/** @sly2-annot addr=0x203A38
 * Purpose: STEP vt15: PostSoLoad, alloc 0x294, attach two child objects (oids +0x58/+0x5C), find phys SOPART
 *   by oid +0x264 -> CloneStepPhys + hide it, allocate the 8 BHV slots
 * Info: proposed name PostStepLoad (T1); class STEP slot15; TU step
 * Confidence: high [T1 0.80] - evidence: static analysis; sources: r7b_guard
 * See: docs/engine/gameplay-objects.md#key-functions,
 *   docs/reference/functions/gameplay-objects.md#rhythm-0x1520c8-0x1539a8,
 *   docs/reference/functions/guards-ai.md#step-0x203948-0x205e70
 * @end-sly2-annot */
void PostStepLoad(struct O *p)
{
    PostSoLoad(p);
    p->x294 = func_001E94B0(0x2C9, 0x10);
    func_002303A0(p, ((struct S *)p)->n58, 1);
    func_002303A0(p, ((struct S *)p)->n5C, 1);
    if (p->oid != -1) {
        p->p268 = PsopartFindSo(p, p->oid);
        if (p->p268) {
            func_00203AF0(p, p->p268);
            HideSoPart(p, p->p268);
        }
    }
    PostBhvstkLoad(p->x26C);
}
