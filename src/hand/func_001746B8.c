struct E { int n; int a[11]; };
struct S { char pad[0x5C]; int c; struct E *ae; };
void RemoveLoHierarchy(struct S *p);
void func_00173928(struct S *p, struct E *pe, int a, int b);
/** @sly2-annot addr=0x1746B8
 * Purpose: Base remove then releases all active slots
 * Info: proposed name RemoveCid119Hierarchy (T2); class CID119 slot12; TU tu_173158
 * Notes: Cid77->Cid119 (CID 0x77=119, vtable) | slot 12 is RemoveHierarchy (CONFLICTS#5: LO base
 *   0x1A5800/0x1A5848 AddLoHierarchy/RemoveLoHierarchy; OnAdd/OnRemove are slots 23/22 per arbitration); was
 *   OnCid119Remove
 * Confidence: low (draft) [T2 0.55] - evidence: arbitration; sources: r3b_rest
 * See: docs/reference/functions/guards-ai.md#tu_173158-0x173158-0x174898
 * @end-sly2-annot */
void func_001746B8(struct S *p) {
    int i;
    RemoveLoHierarchy(p);
    if (!p->ae) return;
    for (i = 0; i < p->c; i++) {
        struct E *pe = &p->ae[i]; if (pe->n) func_00173928(p, pe, 0, 0);
    }
}
