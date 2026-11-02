struct E { int n; int a; int b; };
struct O { char pad[0x574]; struct E ae[1]; };
struct S { char p0[0x10]; struct O *po; char p1[0x6C-0x14]; int i; int n; };
/** @sly2-annot addr=0x1D4E08
 * Purpose: ammo of the weapon's slot >= its cost
 * Info: proposed name FRobottankWeaponHasAmmo (T2); TU tu_1D1370
 * Notes: names_v2 C proposed GetAloLookAtTiltLimits__FP3ALOP2LM (0.200) | class identity from whole_game WE
 *   names/object strings: FRcCarWeaponHasAmmo -> FRobottankWeaponHasAmmo
 * Confidence: low (draft) [T2 0.45] - evidence: static naming pass; sources: r5b_rest+whole_game(class
 *   identity)
 * See: docs/reference/functions/gameplay-objects.md#tu_1d1370-0x1d1370-0x1d5150
 * @end-sly2-annot */
int func_001D4E08(struct S *p) {
    return p->po->ae[p->i].n >= p->n;
}
