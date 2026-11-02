typedef int qword __attribute__((mode(TI)));
struct O { char pad[0xC0]; void *pc; };
struct S {
    char pad0[0x28]; unsigned long fl28;
    char pad1[0x208 - 0x30]; int n208; char pad1b[0x218 - 0x20C]; qword *pq218;
    char pad2[0x26C - 0x21C]; char x26C[0x34C - 0x26C]; int h34C;
    char pad3[0x48C - 0x350]; int n48C; char pad4[0x498 - 0x490]; void *p498; void *p49C;
    char pad5[0x4A4 - 0x4A0]; int n4A4; char pad6[0x4B0 - 0x4A8]; qword q4B0;
    char pad7[0xF00 - 0x4C0]; struct O *po; char pad8[0xF0C - 0xF04]; int hF0C;
    char pad9[0xFBC - 0xF10]; int kFBC; char padA[0xFD0 - 0xFC0]; char xFD0[0x10];
};
extern qword D_002A2DD0;
void UpdateStep(struct S *p);
void UpdateStepguardSpecial(struct S *p);
void UpdateStepguardDetectTargets(struct S *p);
int FAnimBindingDone(void *pc);
void func_0020AB68(struct S *p, struct O *po, int n, qword v);
void func_0020ACC0(struct S *p);
void StageStepguardLoot(struct S *p);
void func_0020CE70(void *p);
void func_0020D1F8(void *p);
void UpdateBhvstk(void *p);
void func_002097E0(void *p);
int FAsegaRetracted(int h);
void func_001306A8(int *ph);

/** @sly2-annot addr=0x207350
 * Purpose: STEPGUARD vt16: UpdateStep; if in world and not time-frozen: special timeouts, detect targets,
 *   finish attack anim, aim, pickpocket, update components, BHV stack (0x1EED10), mover, release finished
 *   ASEGA, track base position
 * Info: proposed name UpdateStepguard (T1); class STEPGUARD slot16; TU stepguard
 * Confidence: high [T1 0.85] - evidence: static analysis; sources: r7b_guard
 * See: docs/reference/functions/guards-ai.md#tu_14fac8-0x14fac8-0x14fca8,
 *   docs/reference/functions/guards-ai.md#stepguard-0x205e70-0x20b840
 * @end-sly2-annot */
void UpdateStepguard(struct S *p)
{
    UpdateStep(p);
    if (!((p->fl28 >> 60) & 1))
        return;
    if (p->kFBC == 0xE)
        return;
    UpdateStepguardSpecial(p);
    UpdateStepguardDetectTargets(p);
    if (p->po && FAnimBindingDone(p->po->pc))
        func_0020AB68(p, 0, 0, D_002A2DD0);
    if (p->hF0C)
        func_0020ACC0(p);
    if (p->n48C)
        StageStepguardLoot(p);
    func_0020CE70(p->p498);
    func_0020D1F8(p->p49C);
    UpdateBhvstk(p->x26C);
    func_002097E0(p->xFD0);
    if (p->h34C && FAsegaRetracted(p->h34C))
        func_001306A8(&p->h34C);
    if (p->n4A4 && p->n208)
        p->q4B0 = *p->pq218;
}
