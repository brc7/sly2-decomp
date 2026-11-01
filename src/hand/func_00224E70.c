typedef int qword __attribute__((mode(TI)));
struct S { char pad[0x174]; int a, b; };
void SetLoParent_1A58D0(struct S *p, int a, int b);
qword func_001A62E8(struct S *p);
void func_00224BF8(struct S *p, int a, int b, qword q, float f);
/** @sly2-annot addr=0x224E70
 * Purpose: SetLoParent(this,0,1) then 0x224BF8 at world pos
 * Info: TU tu_223F70
 * Notes: descriptive only: Cid100 detach+sweep | no usable identifier; purpose only
 * Confidence: low (draft) [T2 0.30] - evidence: static analysis; sources: r8_21BB88
 * @end-sly2-annot */
void func_00224E70(struct S *p, float f)
{
    SetLoParent_1A58D0(p, 0, 1);
    func_00224BF8(p, p->a, p->b, func_001A62E8(p), f);
}
