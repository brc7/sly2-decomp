struct O { char pad[0x44]; int n44; char pad2[0x54 - 0x48]; int k54; struct O *next; };
struct VT;
struct S { char pad[0xC]; struct VT *vt; char pad2[0xF04 - 0x10]; float tF04; char pad3[0xF64 - 0xF08]; void *pF64; };
struct VE { short d; short pad; int (*pf)(void *, struct O *); };
struct VT { struct VE e[90]; };
struct X { int a; int b; };
extern struct { char pad[0x148C]; struct O *first; } D_003E0000;
extern float D_002DDED0;
extern float D_002C3DB0;
int func_00208D70(struct S *p);
int func_0020A168(struct S *p, struct O *a, struct O *b);
struct X *func_00154180(void *p, int n);
void func_00154080(void *p, struct X *x, int n);

/** @sly2-annot addr=0x20A408
 * Purpose: Best perceivable stimulus (vt89) from list D_003E0000 by IcmpStepguardStimulus; a stimulus of kind
 *   4 already tracked just re-marks the detector entry lost and returns the default
 * Info: proposed name PstimChooseStepguard (T2); TU stepguard
 * Confidence: low (draft) [T2 0.45] - evidence: static analysis; sources: r7b_guard
 * See: docs/reference/functions/guards-ai.md#stepguard-0x205e70-0x20b840
 * @end-sly2-annot */
struct O *func_0020A408(struct S *p, struct O *def)
{
    struct O *o, *best;
    int unused[4];
    if (!func_00208D70(p))
        return def;
    best = def;
    for (o = D_003E0000.first; o; o = o->next) {
        if (p->vt->e[89].pf((char *)p + p->vt->e[89].d, o)) {
            if (!best || func_0020A168(p, o, best) < 0)
                best = o;
        }
    }
    if (best && best->k54 == 4) {
        struct X *x = func_00154180(p->pF64, best->n44);
        if (x) {
            if (!x->b) {
                func_00154080(p->pF64, x, 2);
                p->tF04 = D_002DDED0 + D_002C3DB0;
            }
            return def;
        }
    }
    return best;
}
