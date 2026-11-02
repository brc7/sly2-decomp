struct A { char pad[0x2D]; unsigned char b; };
struct S { char pad[0x28]; unsigned long fl; };
void func_001A6D28(struct S *p, int n);
/** @sly2-annot addr=0x1CB4E0
 * Purpose: LO slot 26 base: copy the proxy grfvis (+0x2D) onto the hierarchy unless flag bit58
 * Info: proposed name ResetLoForProxy (T2); class LO slot26; TU proxy
 * Notes: v3: superseded LO_VUnk26Proxy | top50_round (0.50): [extra: knock-on fix] Called through vt26 by
 *   PspareAcquire with the PROXY; see ResetSoForProxy 0x1CB518 | v2 draft was LO_VUnk26Proxy
 * Confidence: low (draft) [T2 0.50] - evidence: semantic review; sources: r5_1AE130+top50_round
 * See: docs/reference/functions/object-model.md#proxy-0x1c9940-0x1cb908
 * @end-sly2-annot */
void func_001CB4E0(struct S *p, struct A *pa)
{
    if (pa && !(p->fl & (1UL << 58))) func_001A6D28(p, pa->b);
}
