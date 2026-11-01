struct E { int a; int b; int c; int d; };
struct S { char pad[0x1C8]; struct E ae[5]; };
/** @sly2-annot addr=0x1800D8
 * Purpose: Set entry i (<5) fields a,d
 * Info: proposed name SetHackshipEntry (T2); class HACKSHIP; TU tu_17F090
 * Notes: Cid48->Cid72 (CID 0x48=72, agent-default(hex)) | class identity from whole_game WE names/object
 *   strings: SetCid72Entry -> SetHackshipEntry
 * Confidence: low (draft) [T2 0.35] - evidence: static analysis; sources: r4b_rest+whole_game(class identity)
 * See: docs/reference/functions/gameplay-objects.md#tu_17f090-0x17f090-0x182c70
 * @end-sly2-annot */
void func_001800D8(struct S *p, unsigned int i, int a, int d)
{
    if (i < 5) {
        p->ae[i].a = a;
        p->ae[i].d = d;
    }
}
