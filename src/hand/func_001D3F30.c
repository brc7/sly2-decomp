struct E { int a[2]; char b[0x58]; };
struct S { char pad[0x190]; int c; int pad2[3]; struct E ae[8]; };
void func_001F5988(struct S *p, void *q);
void func_001F4DA8(struct S *p, void *q, void *pe);
/** @sly2-annot addr=0x1D3F30
 * Purpose: CID 77 vt54(pso): SO base no-op then RemoveSoContactsWith for every wheel
 * Info: proposed name Robottank_vt54 (T2); class ROBOTTANK slot54; TU tu_1D1370
 * Notes: agent qualifier: (remove wheel contacts) | class identity from whole_game WE names/object strings:
 *   RcCar_vt54 -> Robottank_vt54
 * Confidence: low (draft) [T2 0.35] - evidence: static analysis; sources:
 *   r5b_rest+verdicts_B+whole_game(class identity)
 * See: docs/engine/gameplay-objects.md#key-functions,
 *   docs/reference/functions/gameplay-objects.md#tu_1d1370-0x1d1370-0x1d5150
 * @end-sly2-annot */
void func_001D3F30(struct S *p, void *q) {
    int i;
    func_001F5988(p, q);
    for (i = 0; i < p->c; i++) func_001F4DA8(p, q, p->ae[i].b);
}
