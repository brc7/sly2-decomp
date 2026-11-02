typedef int qword __attribute__((mode(TI)));
struct LSG { char pad[0x60]; };
int ClsgClipEdgeToObject(void *pso, qword a, qword b, int c, struct LSG *plsg);
int ClsgMergeAlsg(int c, struct LSG *alsg, int n, struct LSG *plsg, int cMax, struct LSG *alsg2, int f);
/** @sly2-annot addr=0x14CDB0
 * Purpose: Clip a segment against a list of SOs, merging each object's LSG list into the result
 *   (ClsgMergeAlsg k=0)
 * Info: proposed name ClsgClipEdgeToObjects (T1); TU clip
 * Notes: names_v2 v1 proposed FUN_00141b48 (0.000)
 * Confidence: mid (draft) [T1 0.60] - evidence: static naming pass; sources: r2b_rest
 * See: docs/reference/functions/physics-collision.md#clip-0x14c6a0-0x14cfe0
 * @end-sly2-annot */
int ClsgClipEdgeToObjects(qword a, qword b, int cso, void **apso, int cMax, struct LSG *alsg)
{
    int c = 0;
    int i;
    for (i = 0; i < cso; i++) {
        struct LSG *plsg = &alsg[c];
        int n = ClsgClipEdgeToObject(apso[i], a, b, cMax - c, plsg);
        if (n)
            c = ClsgMergeAlsg(c, alsg, n, plsg, cMax, alsg, 0);
    }
    return c;
}
