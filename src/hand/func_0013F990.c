struct S { char pad[0x24]; struct S *pnext; unsigned long f28lo : 48; unsigned long f28b : 4; unsigned long f28hi : 12; char pad2[0x64 - 0x30]; struct S *pchild; };
int func_00132B10(struct S *, int);
/** @sly2-annot addr=0x13F990
 * Purpose: Sets 4-bit LO field at +0x28 bits 48..51 (Lilly nUnk48) on plo and, if plo is an SO (cid 0x30
 *   range), recursively on its children (+0x64 list, +0x24 next)
 * Info: proposed name SetLoNUnk48Hierarchy (T2); TU brx
 * Notes: v2: demoted to T2 (placeholder token in name: Unk)
 * Confidence: low (draft) [T2 0.55] - evidence: semantic review; sources: r2_131F70
 * See: docs/reference/functions/world-sw.md#brx-0x13e368-0x13ff70
 * @end-sly2-annot */
void func_0013F990(struct S *p, int n) {
    struct S *pc;
    p->f28b = n;
    if (func_00132B10(p, 0x30)) {
        for (pc = p->pchild; pc; pc = pc->pnext) func_0013F990(pc, n);
    }
}
