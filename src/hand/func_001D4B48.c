struct K { int a, b; };
struct E { int n; int a[7]; float g; struct K k; int pad[13]; };
struct S { char pad[0x190]; unsigned int c; int pad2[3]; struct E ae[8]; };
/** @sly2-annot addr=0x1D4B48
 * Purpose: Append wheel record (id, radius, ...) if fewer than 8
 * Info: proposed name AddRobottankWheel (T2); TU tu_1D1370
 * Notes: class identity from whole_game WE names/object strings: AddRcCarWheel -> AddRobottankWheel
 * Confidence: low (draft) [T2 0.35] - evidence: static analysis; sources: r5b_rest+whole_game(class identity)
 * See: docs/reference/functions/gameplay-objects.md#tu_1d1370-0x1d1370-0x1d5150
 * @end-sly2-annot */
void func_001D4B48(struct S *p, int n, struct K *pk, float g) {
    if (p->c < 8) {
        struct E *pe = &p->ae[p->c++];
        pe->n = n;
        pe->g = g;
        pe->k = *pk;
    }
}
