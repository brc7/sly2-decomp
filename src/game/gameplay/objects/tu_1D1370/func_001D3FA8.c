struct O { char pad[0x14]; int id; };
struct S { char pad[0x4F8]; int c; struct O *ap[1]; };
/** @sly2-annot addr=0x1D3FA8
 * Purpose: Find weapon record by id
 * Info: proposed name PweaponFindRobottank (T2); TU tu_1D1370
 * Notes: names_v2 v1 proposed RenderShrapnel__FP8SHRAPNELP2CM (0.000) | class identity from whole_game WE
 *   names/object strings: PweaponFindRcCar -> PweaponFindRobottank
 * Confidence: low (draft) [T2 0.40] - evidence: static naming pass; sources: r5b_rest+whole_game(class
 *   identity)
 * See: docs/reference/functions/gameplay-objects.md#tu_1d1370-0x1d1370-0x1d5150
 * @end-sly2-annot */
struct O *func_001D3FA8(struct S *p, int id) {
    int i;
    for (i = 0; i < p->c; i++) {
        struct O *po = p->ap[i];
        if (po->id == id) return po;
    }
    return 0;
}
