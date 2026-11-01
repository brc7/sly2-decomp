struct X;
struct E { int f0; struct X *px; float *pg; };
struct P { char pad[0x3CCC]; int fDirty; int c; struct E *aae[3]; };
void UpdateSwPox(struct P *, struct X *, struct X *, int);
void *func_002332B8(void *, void *, unsigned int);
/** @sly2-annot addr=0x133E80
 * Purpose: If dirty: insertion-sort the 3 endpoint arrays; when a min/max pair crosses call UpdateSwPox(..,0)
 *   (sweep and prune)
 * Info: proposed name UpdateSwAaox (T1); TU bbmark
 * Confidence: mid (draft) [T1 0.75] - evidence: static analysis; sources: r2b_rest
 * See: docs/reference/functions/physics-collision.md#bbmark-0x1336f0-0x1344f0
 * @end-sly2-annot */
void UpdateSwAaox(struct P *p)
{
    int i;
    if (!p->fDirty)
        return;
    p->fDirty = 0;
    if (!p->c)
        return;
    for (i = 0; i < 3; i++) {
        struct E *pe = p->aae[i];
        struct E *ae = pe;
        float gMax = *pe->pg;
        int j;
        for (j = p->c, pe++; --j > 0; pe++) {
            float g = *pe->pg;
            if (gMax <= g) {
                gMax = g;
            } else {
                struct E *pe2;
                struct E e;
                for (pe2 = pe - 1; pe2 >= ae; pe2--) {
                    if (*pe2->pg <= g)
                        break;
                    if (pe->f0 != pe2->f0)
                        UpdateSwPox(p, pe->px, pe2->px, 0);
                }
                pe2++;
                e = *pe;
                func_002332B8(pe2 + 1, pe2, (pe - pe2) * sizeof(struct E));
                *pe2 = e;
            }
        }
    }
}
