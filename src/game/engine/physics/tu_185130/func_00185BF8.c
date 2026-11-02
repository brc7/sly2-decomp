struct S { char pad[0x108]; float g; };
extern float D_002A5690;
void func_00185130(struct S *p);
/** @sly2-annot addr=0x185BF8
 * Purpose: CID 9 vt7: InitCid8 then default radius D_002A5690
 * Info: proposed name InitCid9 (T2); class CID9 slot7; TU tu_185130
 * Notes: names_v2 B proposed InitSpire__FP5SPIRE (0.858)
 * Confidence: low (draft) [T2 0.50] - evidence: static naming pass; sources: r4b_rest
 * See: docs/reference/functions/player.md#tu_185130-0x185130-0x186610
 * @end-sly2-annot */
void func_00185BF8(struct S *p)
{
    func_00185130(p);
    p->g = D_002A5690;
}
