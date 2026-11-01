struct A { char pad[0x15c]; void *f15c; };
struct G { char pad[0x4638]; void *f4638; };
extern struct G D_003DD490;
struct S { char pad[0x194]; struct A *pa; };
int NBootyContainerKind(struct S *);
struct A *PloAcquireProxy(void *, int);
void SelectBootyEntry(struct A *, int);
void SetBootyState(struct A *, int);
/** @sly2-annot addr=0x13CF38
 * Purpose: If none yet and the manager exists: look up saved kind (0x13D248), clone a booty from the
 *   template, select its entry, link and set state 1
 * Info: proposed name SpawnBootyInContainer (T2); TU booty
 * Confidence: low (draft) [T2 0.40] - evidence: static analysis; sources: r2b_rest
 * See: docs/reference/functions/game-progression.md#booty-0x13bdf0-0x13d558
 * @end-sly2-annot */
void func_0013CF38(struct S *p) {
    struct G *pg;
    int i;
    struct A *pa;
    if (p->pa) return;
    pg = &D_003DD490;
    if (!pg->f4638) return;
    i = NBootyContainerKind(p);
    if (i == -1) return;
    pa = PloAcquireProxy(pg->f4638, 0);
    p->pa = pa;
    if (pa) {
        SelectBootyEntry(pa, i);
        p->pa->f15c = p;
        SetBootyState(p->pa, 1);
    }
}
