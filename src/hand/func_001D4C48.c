struct K { int a, b; };
struct E { int n; int a, b; struct K k; };
struct S { char pad[0x4A4]; unsigned int c; struct E ae[4]; };
/** @sly2-annot addr=0x1D4C48
 * Purpose: Append (id, data) record if fewer than 4
 * Info: proposed name AddRobottankAttachment (T2); TU tu_1D1370
 * Notes: names_v2 v1 proposed RenderMatch__FP5MATCHP2CM (0.039) | class identity from whole_game WE
 *   names/object strings: AddRcCarAttachment -> AddRobottankAttachment
 * Confidence: low (draft) [T2 0.30] - evidence: static naming pass; sources: r5b_rest+whole_game(class
 *   identity)
 * See: docs/reference/functions/gameplay-objects.md#tu_1d1370-0x1d1370-0x1d5150
 * @end-sly2-annot */
void func_001D4C48(struct S *p, int n, struct K *pk) {
    if (p->c < 4) {
        struct E *pe = &p->ae[p->c++];
        pe->n = n;
        pe->k = *pk;
    }
}
