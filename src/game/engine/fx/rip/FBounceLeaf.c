typedef int qword __attribute__((mode(TI)));
struct A { void *x0; void *x4; };
struct S { char pad[0xF0]; void *xF0; };
extern int D_002C1F70[1];
int FIsBasicDerivedFrom__FP5BASIC3CID(void *p, int n);
void CreateStuck(struct S *p, void *a, void *b, qword q0, qword q1);
void func_001CE980(struct S *p);
/** @sly2-annot addr=0x1CF968
 * Purpose: FLYING FBounce: on contact CreateStuck then RemoveRip
 * Info: proposed name FBounceLeaf (T1); TU rip
 * Notes: names_v2 A proposed FBounceLeaf__FP4LEAFP2SOP6VECTORT2 (0.927) [verified CORRECT] | CONFLICT:
 *   verified tier-A name differs from TU name; orchestrator: verified name FBounceLeaf__FP4LEAFP2SOP6VECTORT2
 *   restored over FBounceFlying (verified > unverified) | arbitration adopt verified: Verifier matched Sly1
 *   FBounceLeaf; referenced only from data word 0x2D777C (callback). Mangling not checked here (dispute note
 *   says vectors by value), leave bla
 * Confidence: mid (draft) [T1 0.65] - evidence: Sly 1 / Lilly reference; arbitration; static naming pass;
 *   sources: r5_1AE130+verdicts_A2;verified_A+arbitration
 * See: docs/engine/particles-effects.md#6-key-functions,
 *   docs/open-questions.md#names-and-classes-cross-cutting,
 *   docs/reference/functions/particles-effects.md#rip-0x1cdcd0-0x1d1370
 * @end-sly2-annot */
int FBounceLeaf(struct S *p, struct A *pa, qword q0, qword q1)
{
    unsigned int i;
    for (i = 0; i < sizeof(D_002C1F70) / sizeof(D_002C1F70[0]); i++) {
        if (FIsBasicDerivedFrom__FP5BASIC3CID(pa->x4, D_002C1F70[i])) return 0;
    }
    if (!FIsBasicDerivedFrom__FP5BASIC3CID(pa->x4, 0x59)) CreateStuck(p, p->xF0, pa->x0, q0, q1);
    func_001CE980(p);
    return 1;
}
