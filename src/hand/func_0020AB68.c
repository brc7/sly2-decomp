typedef int qword __attribute__((mode(TI)));
extern struct { float t; float dt; } D_002DDED0;
extern float D_002A88C4;
struct C { char pad[0xC]; int fl; };
struct O { char pad[0x60]; float f60; float f64; char pad2[0xC0 - 0x68]; struct C *pc; };
struct S { char pad[0xF00]; struct O *po; float t; };
void func_00209360(struct C *pc);
float func_0011D448(float a, float b);
void func_00209278(struct C *pc, struct S *p, int fl, float g);
void func_0020AC28(struct S *p, struct O *po, int n, qword v);
/** @sly2-annot addr=0x20AB68
 * Purpose: Switch current attack +0xF00: stop old anim, cooldown +0xF04 = now+GRandInRange(lm), start new
 *   anim, then aim (0x20AC28)
 * Info: proposed name SetStepguardAttack (T2); TU stepguard
 * Confidence: low (draft) [T2 0.45] - evidence: static analysis; sources: r7b_guard
 * See: docs/reference/functions/guards-ai.md#stepguard-0x205e70-0x20b840
 * @end-sly2-annot */
void func_0020AB68(struct S *p, struct O *po, int n, qword v)
{
    if (p->po) {
        float t;
        func_00209360(p->po->pc);
        t = D_002DDED0.t;
        t += func_0011D448(p->po->f60, p->po->f64);
        p->po = 0;
        p->t = t;
    }
    p->po = po;
    if (po) {
        func_00209278(po->pc, p, po->pc->fl | 1, 0.0f);
        p->t = D_002A88C4;
    }
    func_0020AC28(p, po, n, v);
}
