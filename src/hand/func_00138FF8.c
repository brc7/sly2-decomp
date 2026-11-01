struct N { char pad[0x1088]; void *f1088; void *f108c; };
struct P { char pad[0x160]; int f160; char pad2[0x448 - 0x164]; int f448; };
struct G { char pad[0x4088]; int pool4088; char pad2[0x40a0 - 0x408c]; int pool40a0; };
struct N *func_0011CCC0(void *);
void func_0011CD98(void *, struct N *);
void func_0011A220(void *, struct N *);
/** @sly2-annot addr=0x138FF8
 * Purpose: Allocate a BLIP (+spline block) and append to the blipg list
 * Info: proposed name PblipNew (T1); TU blip
 * Confidence: high [T1 0.85] - evidence: static analysis; sources: r2b_rest+verdicts_A1
 * See: docs/engine/particles-effects.md#6-key-functions,
 *   docs/reference/functions/particles-effects.md#blip-0x138f08-0x13a9e8
 * @end-sly2-annot */
struct N *func_00138FF8(struct G *pg, struct P *p) {
    void *pool = &pg->pool4088;
    struct N *pn = func_0011CCC0(pool);
    if (!pn) return 0;
    if (p->f160 == 2) {
        pn->f1088 = func_0011CCC0(&pg->pool40a0);
        if (!pn->f1088) { func_0011CD98(pool, pn); return 0; }
    }
    pn->f108c = p;
    func_0011A220(&p->f448, pn);
    return pn;
}
