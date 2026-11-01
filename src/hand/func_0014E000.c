struct T { char pad[0xE4]; int nE4; char pad2[0x110 - 0xE8]; int n110; float g114; char pad3[0x120 - 0x118]; };
/** @sly2-annot addr=0x14E000
 * Purpose: qsort comparator for vis-zone candidates: by +0x110 desc, +0xE4 desc, then +0x114 (distance) asc
 * Info: proposed name ICompareCmVisZone (T2); TU cm
 * Notes: names_v2 B proposed DrawCm__FP2CM (0.856)
 * Confidence: low (draft) [T2 0.45] - evidence: semantic review; static naming pass; sources: r2_131F70
 * See: docs/reference/functions/camera.md#cm-0x14d3d8-0x14fac8
 * @end-sly2-annot */
int func_0014E000(struct T **ppa, struct T **ppb)
{
    struct T *a = *ppa;
    struct T *b = *ppb;
    int d = b->n110 - a->n110;
    if (d) return d;
    d = b->nE4 - a->nE4;
    if (d) return d;
    if (a->g114 < b->g114) return -1;
    if (b->g114 < a->g114) return 1;
    return a - b;
}
