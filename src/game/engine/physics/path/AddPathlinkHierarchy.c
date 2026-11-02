struct O { char pad[0x28]; int n; };
struct G { char pad[0x3D74]; int c; char pad2[0x854]; struct O *po; };
extern struct G D_003DD490;
void AddLoHierarchy(void *p);
/** @sly2-annot addr=0x1C2078
 * Purpose: CID 124 vt11: AddLoHierarchy then bump world nav version
 * Info: proposed name AddPathlinkHierarchy (T1); class PATHLINK slot11; TU path
 * Confidence: mid (draft) [T1 0.50] - evidence: static analysis; sources: r5b_rest
 * See: docs/reference/functions/physics-collision.md#path-0x1bf110-0x1c2920
 * @end-sly2-annot */
void AddPathlinkHierarchy(void *p)
{
    struct G *pg;
    int c;
    AddLoHierarchy(p);
    pg = &D_003DD490;
    c = pg->c + 1;
    pg->c = c;
    pg->po->n = c;
}
