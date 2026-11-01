struct S { char pad[0x190]; char sub[0x2C]; int n; };
void func_001E5D80(struct S *p);
void func_00182C70(void *p);
/** @sly2-annot addr=0x182E80
 * Purpose: CID12 vt7: InitShape, InitGrabspot(+0x190 sub), kind 3
 * Info: proposed name InitCid12 (T2); class CID12 slot7; TU grabspot
 * Confidence: low (draft) [T2 0.60] - evidence: static analysis; sources: r4b_rest
 * See: docs/reference/functions/player.md#grabspot-0x182c70-0x1848a8
 * @end-sly2-annot */
void func_00182E80(struct S *p)
{
    func_001E5D80(p);
    func_00182C70(p->sub);
    p->n = 3;
}
