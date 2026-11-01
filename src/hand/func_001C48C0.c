struct C { int pad[2]; int *pn; };
struct B { char pad[0xE4]; struct C *pc; };
struct S { char pad[0x3EFC]; int n; char pad2[0x4778 - 0x3F00]; int c; struct B *a[1]; };
void func_001A58D0(struct B *pb, int n, int m);
/** @sly2-annot addr=0x1C48C0
 * Purpose: For each LO in SW+0x4778 list: store current PO into its record and re-parent it to the current PO
 *   (0x1A58D0)
 * Info: proposed name UpdateSwPoFollowers (T2); TU po
 * Notes: names_v2 B proposed PostRopLoad__FP3ROP (0.964)
 * Confidence: low (draft) [T2 0.40] - evidence: static naming pass; sources: r5_1AE130+verdicts_tu_r3r5
 * See: docs/reference/functions/player.md#po-0x1c3d70-0x1c4e50
 * @end-sly2-annot */
void func_001C48C0(struct S *p)
{
    int i;
    for (i = 0; i < p->c; i++) {
        struct B *pb = p->a[i];
        *pb->pc->pn = p->n;
        func_001A58D0(pb, p->n, 0);
    }
}
