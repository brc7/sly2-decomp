struct E { int a; int n; float g; };
struct S { char pad[0x574]; struct E ae[1]; };
/** @sly2-annot addr=0x1D4CE0
 * Purpose: Set ammo slot i (count, max)
 * Info: proposed name SetRobottankAmmo (T2); TU tu_1D1370
 * Notes: class identity from whole_game WE names/object strings: SetRcCarAmmo -> SetRobottankAmmo
 * Confidence: low (draft) [T2 0.30] - evidence: static analysis; sources: r5b_rest+whole_game(class identity)
 * See: docs/reference/functions/gameplay-objects.md#tu_1d1370-0x1d1370-0x1d5150
 * @end-sly2-annot */
void func_001D4CE0(struct S *p, int i, float g, int n) {
    struct E *pe = &p->ae[i];
    pe->n = n;
    pe->g = g;
}
