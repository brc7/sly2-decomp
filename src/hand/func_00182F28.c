struct S { char pad[0x38]; float g; char p2[0x154]; void *p190; };
extern float D_002BE8B4;
void func_001A5C48(struct S *p);
void func_00221268(void *p, int n);
float func_001E7D90(struct S *p);
/** @sly2-annot addr=0x182F28
 * Purpose: vt15: PostLoLoad, private TN override off, default busy radius = bounding radius + D_002BE8B4
 * Info: proposed name PostCid12Load (T2); class CID12 slot15; TU grabspot
 * Confidence: low (draft) [T2 0.60] - evidence: static analysis; sources: r4b_rest
 * See: docs/reference/functions/player.md#grabspot-0x182c70-0x1848a8
 * @end-sly2-annot */
void func_00182F28(struct S *p)
{
    func_001A5C48(p);
    if (p->p190) func_00221268(p->p190, 0);
    if (p->g == 1e10f) p->g = func_001E7D90(p) + D_002BE8B4;
}
