typedef unsigned long ulong;
struct L { char pad[0x24]; struct L *next; char pad2[0x64 - 0x28]; struct L *first; char pad3[0x148 - 0x68]; ulong lo : 35; ulong b35 : 1; ulong hi : 28; };
struct G { char pad[0x3E5C]; int f; };
extern struct G D_003DD490;
void func_001D6378(struct L *);
void func_001D6310(struct L *);
int FIsBasicDerivedFrom__FP5BASIC3CID(void *, int);
/** @sly2-annot addr=0x1F8668
 * Purpose: Set +0x148 bit35 recursively; when g_sw+0x3E5C and enabling register (0x1D6378)
 * Info: proposed name SetSoFlag35Hierarchy (T2); TU so
 * Notes: names_v2 v1 proposed PxpFindSoGround__FP2SOT0Pi (0.047)
 * Confidence: low (draft) [T2 0.30] - evidence: semantic review; static naming pass; sources: r6_1D5150
 * See: docs/reference/functions/physics-collision.md#so-0x1f2108-0x1f8dd8
 * @end-sly2-annot */
void func_001F8668(struct L *p, long f)
{
    struct L *q;
    if (f != p->b35) {
        p->b35 = f;
        if (D_003DD490.f) {
            if (f) func_001D6378(p);
            else func_001D6310(p);
        }
        for (q = p->first; q; q = q->next)
            if (FIsBasicDerivedFrom__FP5BASIC3CID(q, 0x30)) func_001F8668(q, f);
    }
}
