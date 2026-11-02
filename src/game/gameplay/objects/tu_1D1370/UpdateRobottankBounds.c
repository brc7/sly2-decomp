typedef int qword __attribute__((mode(TI)));
struct E { int a[8]; float g20; int b[11]; qword q50; };
struct S { char pad[0x190]; int c; int pad2[3]; struct E ae[8]; };
void UpdateSoTreeBounds(struct S *p);
qword ConvertAloPos(struct S *p, int n, qword q);
void ExtendSoTreeBounds(struct S *p, qword q, float g);
/** @sly2-annot addr=0x1D4178
 * Purpose: CID 77 vt49: SO tree bounds then extend by each wheel sphere
 * Info: proposed name UpdateRobottankBounds (T1); class ROBOTTANK slot49; TU tu_1D1370
 * Notes: class identity from whole_game WE names/object strings: UpdateRcCarBounds -> UpdateRobottankBounds
 * Confidence: mid (draft) [T1 0.50] - evidence: static analysis; sources: r5b_rest+whole_game(class identity)
 * See: docs/reference/functions/gameplay-objects.md#tu_1d1370-0x1d1370-0x1d5150
 * @end-sly2-annot */
void UpdateRobottankBounds(struct S *p) {
    int i;
    UpdateSoTreeBounds(p);
    for (i = 0; i < p->c; i++) {
        struct E *pe = &p->ae[i];
        ExtendSoTreeBounds(p, ConvertAloPos(p, 0, pe->q50), pe->g20);
    }
}
