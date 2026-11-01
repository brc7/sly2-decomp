struct Q { char pad[0x4C]; int st; };
struct S { char pad[0x17C]; struct Q *pq; };
void func_001E8378(struct Q *q);
void func_001E9090(struct Q *q);
void func_001A5E80(struct S *p);
/** @sly2-annot addr=0x1E6340
 * Purpose: SHAPE vt17: if a chain is attached step its simulation (mode 0/3 -> 0x1E8378, 1/2 -> 0x1E9090)
 *   else LO vt17
 * Info: proposed name UpdateShapeXfWorld (T2); class SHAPE slot17; TU shape
 * Notes: superseded r6_1D5150 proposal: SHAPE::vt17 (0.3) | names_v2 C proposed SMeasureApos__FiP6VECTORPf
 *   (0.638) | agent qualifier: (vt17)
 * Confidence: low (draft) [T2 0.35] - evidence: static naming pass; sources: r6b_rest
 * See: docs/reference/functions/physics-collision.md#shape-0x1e5d80-0x1ea568
 * @end-sly2-annot */
void func_001E6340(struct S *p)
{
    if (p->pq) {
        switch (p->pq->st) {
        case 0:
        case 3:
            func_001E8378(p->pq);
            break;
        case 1:
        case 2:
            func_001E9090(p->pq);
            break;
        }
    } else {
        func_001A5E80(p);
    }
}
