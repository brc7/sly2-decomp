struct T { char pad[0xEC]; int n; };
struct S { char pad[0x1C8]; struct T *pt; };
int func_001F4FC8(struct S *p, struct T *q);
/** @sly2-annot addr=0x1803A8
 * Purpose: vt61 override
 * Info: proposed name HACKBULLET_FIgnoreIntersection (T2); class HACKBULLET slot61; TU tu_17F090
 * Notes: CID4A->CID74 (CID 0x4A=74, hex-literal) | class identity from whole_game WE names/object strings:
 *   CID74_FIgnoreIntersection -> HACKBULLET_FIgnoreIntersection
 * Confidence: low (draft) [T2 0.30] - evidence: static analysis; sources: r4b_rest+whole_game(class identity)
 * See: docs/reference/functions/gameplay-objects.md#tu_17f090-0x17f090-0x182c70
 * @end-sly2-annot */
int func_001803A8(struct S *p, struct T *q)
{
    if (p->pt && q->n == p->pt->n)
        return 3;
    return func_001F4FC8(p, q);
}
