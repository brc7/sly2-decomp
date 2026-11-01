struct E { int k; void *p; int *pn; };
struct N { int a0[4]; int a10[4]; char pad[0x34 - 0x20]; int dl34[3]; char pad2[0x60 - 0x40]; void *f60; };
struct P { char pad[0x14]; void *f14; };
struct G { char pad[0x2c]; int dl2c[3]; char pad2[0x3cd0 - 0x38]; int ce; struct E *ape[3]; int pool3ce0;
    char pad3[0x3d10 - 0x3ce4]; int dl3d10[3]; int dl3d1c[3]; };
struct N *func_0011CCC0(void *);
void func_0011A1F8(void *, int);
void func_0011A220(void *, struct N *);
int func_0011A380(void *, void *);
/** @sly2-annot addr=0x1336F0
 * Purpose: Allocate the object's OXA node (pool g_sw+0x3CE0), append to SW lists, and add min/max endpoints
 *   on each of 3 axes to the sweep arrays (count += 2)
 * Info: proposed name AddSwAaobrObject (T1); TU bbmark
 * Notes: names_v2 C proposed InitDzg__FP3DZGi (0.200)
 * Confidence: mid (draft) [T1 0.75] - evidence: static naming pass; sources: r2b_rest
 * See: docs/engine/object-model.md#8-source-files, docs/engine/physics-collision.md#2-contacts-and-solving,
 *   docs/engine/physics-collision.md#6-source-files
 * @end-sly2-annot */
struct N *func_001336F0(struct G *pg, struct P *p) {
    int i;
    struct N *pn = func_0011CCC0(&pg->pool3ce0);
    func_0011A1F8(pn->dl34, 0x60);
    pn->f60 = p;
    func_0011A220(pg->dl3d10, pn);
    if (func_0011A380(pg->dl2c, p->f14)) func_0011A220(pg->dl3d1c, pn);
    for (i = 0; i < 3; i++) {
        struct E *pe = &pg->ape[i][pg->ce];
        pe->k = 0;
        pe->p = pn;
        pe->pn = &pn->a0[i];
        pe++;
        pe->k = 1;
        pe->p = pn;
        pe->pn = &pn->a10[i];
    }
    pg->ce += 2;
    return pn;
}
