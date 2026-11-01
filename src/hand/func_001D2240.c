struct O { char pad[0x80]; int n80; };
struct S { char pad[0x4F8]; int c; struct O *ap[1]; };
int func_001D4E08(struct O *po);
/** @sly2-annot addr=0x1D2240
 * Purpose: First weapon with enough ammo whose state is not 4
 * Info: proposed name PweaponReadyRobottank (T2); TU tu_1D1370
 * Notes: names_v2 C proposed SetAloBlotContext__FP3ALOP4BLOT (0.200) | class identity from whole_game WE
 *   names/object strings: PweaponReadyRcCar -> PweaponReadyRobottank
 * Confidence: low (draft) [T2 0.40] - evidence: static naming pass; sources: r5b_rest+whole_game(class
 *   identity)
 * See: docs/reference/functions/gameplay-objects.md#tu_1d1370-0x1d1370-0x1d5150
 * @end-sly2-annot */
struct O *func_001D2240(struct S *p) {
    int i;
    for (i = 0; i < p->c; i++) {
        struct O *po = p->ap[i];
        if (func_001D4E08(po) && po->n80 != 4) return po;
    }
    return 0;
}
