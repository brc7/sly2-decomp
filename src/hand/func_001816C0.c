struct E { int a; int b; int c; int d; };
struct S { char pad[0x1D0]; struct E ae[3]; };
/** @sly2-annot addr=0x1816C0
 * Purpose: Set entry i (<3)
 * Info: proposed name SetHacklockEntry (T2); class HACKLOCK; TU tu_17F090
 * Notes: names_v2 v1 proposed PsoEnemySgg__FP3SGG (0.438) | Cid4B->Cid75 (CID 0x4B=75, hex-literal) | class
 *   identity from whole_game WE names/object strings: SetCid75Entry -> SetHacklockEntry
 * Confidence: low (draft) [T2 0.30] - evidence: static naming pass; sources: r4b_rest+whole_game(class
 *   identity)
 * See: docs/reference/functions/gameplay-objects.md#tu_17f090-0x17f090-0x182c70
 * @end-sly2-annot */
void func_001816C0(struct S *p, unsigned int i, int a, int d)
{
    if (i < 3) {
        p->ae[i].a = a;
        p->ae[i].d = d;
    }
}
