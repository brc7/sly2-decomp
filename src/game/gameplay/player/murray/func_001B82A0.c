typedef int qword __attribute__((mode(TI)));
struct M { char pad[0x148]; unsigned long f148; };
struct Q { char pad[0x40]; int x40; char pad2[8]; struct M *pm; char pad3[0x10]; qword q60; char pad4[0x18]; float f88; };
struct S { char pad[0x208]; struct Q *pq; char pad2[0x108C - 0x20C]; int x108C; char pad3[0x13D0 - 0x1090]; int k; };
void func_0019FEB0(void *p, int a, qword q);
void func_001942F0(void *p);
void func_0019EA60(void *p);
/** @sly2-annot addr=0x1B82A0
 * Purpose: Per-frame: on flat ground (normal z>0.7, material flag clear) call 0x19FEB0 on +0x9E0; component
 *   hooks for states 13/20
 * Info: proposed name UpdateMurrayGroundEffects (T2); TU murray
 * Confidence: low (draft) [T2 0.25] - evidence: static analysis; sources: r5b_rest
 * See: docs/reference/functions/player.md#murray-0x1b4728-0x1b9f80
 * @end-sly2-annot */
void func_001B82A0(struct S *p)
{
    struct Q *pq;
    switch (p->k) {
    case 0: case 1: case 4: case 5: case 7: case 8: case 9: case 10: case 18: case 21:
        pq = p->pq;
        if (pq && pq->f88 > 0.7f && !(pq->pm->f148 & 0x20000000) && !p->x108C)
            func_0019FEB0((char *)p + 0x9E0, pq->x40, pq->q60);
        break;
    case 13: func_001942F0((char *)p + 0x4C0); break;
    case 20: func_0019EA60((char *)p + 0xF50); break;
    }
}
