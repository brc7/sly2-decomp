struct E { int n; int nMax; int b; };
struct Q { char pad[0x574]; struct E ae[1]; };
struct S { char p0[0x150]; int n150; int a; int i; int d; };
void SetRcPickupState(struct S *p, int n);
static inline int Min(int a, int b) { return a < b ? a : b; }
/** @sly2-annot addr=0x1D3FF0
 * Purpose: Pickup callback: if pickup not yet used, add its ammo to the car's weapon slot (clamped to max)
 *   and set pickup state 2
 * Info: proposed name CollectRobottankPickup (T2); TU tu_1D1370
 * Notes: names_v2 v1 proposed InitGlint__FP5GLINTP6VECTORfP2SO (0.002) | class identity from whole_game WE
 *   names/object strings: CollectRcCarPickup -> CollectRobottankPickup
 * Confidence: low (draft) [T2 0.45] - evidence: static naming pass; sources: r5b_rest+whole_game(class
 *   identity)
 * See: docs/reference/functions/gameplay-objects.md#tu_1d1370-0x1d1370-0x1d5150
 * @end-sly2-annot */
void func_001D3FF0(struct Q *q, struct S *p) {
    if (p->n150 != 2) {
        struct E *pe = &q->ae[p->i];
        int n = pe->n + p->d;
        pe->n = Min(pe->nMax, n);
        SetRcPickupState(p, 2);
    }
}
