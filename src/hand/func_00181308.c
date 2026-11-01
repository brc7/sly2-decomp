struct E { int a; int b; int c; int d; };
struct S { char pad[0x220]; struct E ae[5]; };
/** @sly2-annot addr=0x181308
 * Purpose: Set entry i (<5)
 * Info: proposed name SetHackenemyEntry (T2); class HACKENEMY; TU tu_17F090
 * Notes: Cid49->Cid73 (CID 0x49=73, agent-default(hex)) | class identity from whole_game WE names/object
 *   strings: SetCid73Entry -> SetHackenemyEntry
 * Confidence: low (draft) [T2 0.30] - evidence: static analysis; sources: r4b_rest+whole_game(class identity)
 * See: docs/reference/functions/gameplay-objects.md#tu_17f090-0x17f090-0x182c70
 * @end-sly2-annot */
void func_00181308(struct S *p, unsigned int i, int a, int d)
{
    if (i < 5) {
        p->ae[i].a = a;
        p->ae[i].d = d;
    }
}
