typedef int qword __attribute__((mode(TI)));
struct S { void *p0; void *p4; int pad[2]; qword q10; };
extern char D_003DD020[];
void func_001E6F60(void *p, qword a, void *b, qword *pq, int c, int d, int e);
qword func_001A62E8(void *p);
/** @sly2-annot addr=0x1C1EC0
 * Purpose: Position of a path endpoint record: closest point on its shape (+0), else its object position
 *   (+4), else stored vector (+0x10)
 * Info: proposed name PosPathlinkEndpoint (T2); TU path
 * Notes: top50_round (0.50): Body; callers BHVGOTO/BHVPATHLINK/BHV189/191 planning, FNearPathlinkEndpoint
 *   0x1C1FA0
 * Confidence: low (draft) [T2 0.50] - evidence: semantic review; sources: r5b_rest+top50_round
 * See: docs/reference/functions/physics-collision.md#path-0x1bf110-0x1c2920
 * @end-sly2-annot */
qword func_001C1EC0(struct S *p, qword a)
{
    qword q;
    if (p->p0) {
        func_001E6F60(p->p0, a, D_003DD020, &q, 0, 0, 0);
    } else if (p->p4) {
        q = func_001A62E8(p->p4);
    } else {
        q = p->q10;
    }
    return q;
}
