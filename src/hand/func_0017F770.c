typedef int qword __attribute__((mode(TI)));
extern qword D_002A2DD0;
struct G;
extern struct G D_003DD490;
struct E { int a; int b; int c; int d; int e; int f; };
struct S { char pad[0x230]; int c; struct E ae[1]; };
int func_001F4C58(struct S *p, int a, int b);
int func_0015E938(struct G *pg, int k, int b, struct S *p);
int func_0012DC18(struct S *p, int c, int a, int b, int d, qword q);
int func_001F0380(int e, struct S *p, int a, int b);
/** @sly2-annot addr=0x17F770
 * Purpose: Look up child XFs / objects for entries and create vector actuators
 * Info: proposed name BindHackshipParts (T2); class HACKSHIP; TU tu_17F090
 * Notes: names_v2 C proposed FTakeStepguardDamage__FP9STEPGUARDP3ZPR (0.638) | Cid48->Cid72 (CID 0x48=72,
 *   agent-default(hex)) | class identity from whole_game WE names/object strings: BindCid72Parts ->
 *   BindHackshipParts
 * Confidence: low (draft) [T2 0.30] - evidence: static naming pass; sources: r4b_rest+whole_game(class
 *   identity)
 * See: docs/reference/functions/gameplay-objects.md#tu_17f090-0x17f090-0x182c70
 * @end-sly2-annot */
void func_0017F770(struct S *p) {
    int i;
    for (i = 0; i < p->c; i++) {
        struct E *pe = &p->ae[i];
        pe->c = func_001F4C58(p, pe->a, 6);
        pe->e = func_0015E938(&D_003DD490, 4, pe->b, p);
        if (pe->c) pe->d = func_0012DC18(p, pe->c, 0, 7, 2, D_002A2DD0);
        if (pe->e) pe->f = func_001F0380(pe->e, p, -1, 0);
    }
}
