struct K { int a; int b; };
struct E { int a; int b; struct K k1; struct K k2; struct K k3; struct K k4; };
struct S { char pad[0x270]; struct E ae[5]; };
/** @sly2-annot addr=0x181378
 * Purpose: Twin setter
 * Info: proposed name SetHackenemyEntryVectors2 (T2); class HACKENEMY; TU tu_17F090
 * Notes: Cid49->Cid73 (CID 0x49=73, agent-default(hex)) | class identity from whole_game WE names/object
 *   strings: SetCid73EntryVectors2 -> SetHackenemyEntryVectors2
 * Confidence: low (draft) [T2 0.30] - evidence: static analysis; sources: r4b_rest+whole_game(class identity)
 * See: docs/reference/functions/gameplay-objects.md#tu_17f090-0x17f090-0x182c70
 * @end-sly2-annot */
void func_00181378(struct S *p, unsigned int i, struct K *pk1, struct K *pk2)
{
    if (i < 5) {
        struct E *pe = &p->ae[i];
        pe->b = 1;
        pe->k3 = *pk1;
        pe->k4 = *pk2;
    }
}
