struct S { char pad[0x38]; float g; char p2[0xA4]; void *pe0; char p3[0x28]; float g10c; };
extern float D_002BE8C0;
void PostLoLoad(struct S *p);
void func_00221268(void *p, int n);
/** @sly2-annot addr=0x1833E0
 * Purpose: vt15: PostLoLoad, TN override off, default radius = r(x10C)+D_002BE8C0
 * Info: proposed name PostCid34Load (T2); class CID34 slot15; TU grabspot
 * Confidence: low (draft) [T2 0.55] - evidence: static analysis; sources: r4b_rest
 * See: docs/reference/functions/player.md#grabspot-0x182c70-0x1848a8
 * @end-sly2-annot */
void func_001833E0(struct S *p)
{
    PostLoLoad(p);
    if (p->pe0) func_00221268(p->pe0, 0);
    if (p->g == 1e10f) p->g = p->g10c + D_002BE8C0;
}
