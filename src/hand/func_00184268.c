struct S { char pad[0xF0]; int f0; char p1[8]; int fc; char p2[0x84]; int x184; int p3; int x18c; int p4; int x194; int p5; int x19c; };
void func_00183F08(struct S *p);
/** @sly2-annot addr=0x184268
 * Purpose: CID 0x24 (child of CID35) vt7: InitCid35 and four oids -1
 * Info: proposed name InitCid36 (T2); class CID36 slot7; TU grabspot
 * Confidence: low (draft) [T2 0.45] - evidence: static analysis; sources: r4b_rest
 * See: docs/reference/functions/player.md#grabspot-0x182c70-0x1848a8
 * @end-sly2-annot */
void func_00184268(struct S *p)
{
    func_00183F08(p);
p->x184 = -1;
p->x18c = -1;
p->x194 = -1;
p->x19c = -1;
p->f0 = -1;
p->fc = 4;
}
