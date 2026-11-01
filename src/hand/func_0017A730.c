typedef int qword __attribute__((mode(TI)));
extern qword D_002A2DD0;
struct T { char pad[0xF64]; void *pF64; };
struct S { char pad[0x10]; struct T *pt; char pad2[0x28]; int n3C; };
void func_001EEF70(struct S *p);
void func_0017A8C8(struct S *p, int n);
void func_0020B210(struct T *t);
void func_0020AB68(struct T *t, int a, int b, qword q);
void func_001F4E28(struct T *t);
void func_001EEFC0(struct S *p);
void func_00153FF8(void *p);
void func_00154038(void *p, int a, int b);
/** @sly2-annot addr=0x17A730
 * Purpose: BHVTHROWN Start: state 0, detach, cancel attack, free contacts, busy, reset detector, mark thrower
 * Info: proposed name StartBhv176 (T2); class BHV176 slot8; TU guardbhv
 * Notes: BHV class word normalised: StartBhvthrown -> StartBhv176
 * Confidence: low (draft) [T2 0.45] - evidence: static analysis; sources: r9_bhv_screen
 * See: docs/reference/functions/guards-ai.md#guardbhv-0x174898-0x17f090
 * @end-sly2-annot */
void func_0017A730(struct S *p) {
    func_001EEF70(p); func_0017A8C8(p, 0); func_0020B210(p->pt); func_0020AB68(p->pt, 0, 0, D_002A2DD0);
    func_001F4E28(p->pt); func_001EEFC0(p); func_00153FF8(p->pt->pF64);
    if (p->n3C) func_00154038(p->pt->pF64, p->n3C, 2);
}
