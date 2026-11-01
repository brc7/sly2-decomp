struct E { int n; int a; int b; };
struct O { char pad[0x574]; struct E ae[1]; };
struct S { char p0[0x10]; struct O *po; char p1[0x6C-0x14]; int i; int n; };
/** @sly2-annot addr=0x1D4E30
 * Purpose: Subtract weapon cost from its slot, clamp 0
 * Info: proposed name UseRobottankWeaponAmmo (T2); TU tu_1D1370
 * Notes: names_v2 C proposed SetAloTargetAttacks__FP3ALOi (0.638) | class identity from whole_game WE
 *   names/object strings: UseRcCarWeaponAmmo -> UseRobottankWeaponAmmo
 * Confidence: low (draft) [T2 0.40] - evidence: static naming pass; sources: r5b_rest+whole_game(class
 *   identity)
 * See: docs/reference/functions/gameplay-objects.md#tu_1d1370-0x1d1370-0x1d5150
 * @end-sly2-annot */
void func_001D4E30(struct S *p) {
    struct E *pe = &p->po->ae[p->i];
    int n = pe->n - p->n;
    if (n < 0) n = 0;
    pe->n = n;
}
