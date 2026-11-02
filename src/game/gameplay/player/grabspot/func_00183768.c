struct S { char pad[0x38]; float g; char p2[0xA4]; void *pe0; };
extern float D_002BE8C8;
void PostLoLoad(struct S *p);
void SetTnTnsOverride(void *p, int n);
/** @sly2-annot addr=0x183768
 * Purpose: vt15: PostLoLoad, TN override off, default radius D_002BE8C8
 * Info: proposed name PostCid38Load (T2); class CID38 slot15; TU grabspot
 * Confidence: low (draft) [T2 0.55] - evidence: static analysis; sources: r4b_rest
 * See: docs/reference/functions/player.md#grabspot-0x182c70-0x1848a8
 * @end-sly2-annot */
void func_00183768(struct S *p)
{
    PostLoLoad(p);
    if (p->pe0) SetTnTnsOverride(p->pe0, 0);
    if (p->g == 1e10f) p->g = D_002BE8C8;
}
