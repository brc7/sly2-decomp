struct T { char pad[0xD4]; float a, b; };
struct S { char pad[0xE8]; struct T *t; };
void func_001F3590(struct S *p);
void func_001F68A8(struct S *p, int a);
/** @sly2-annot addr=0x228BB0
 * Purpose: 0x1F3590 then 0x1F68A8(p,1); t->b=1.0
 * Info: class WATER slot64; TU water
 * Notes: descriptive only: water slot override (constraints) | no usable identifier; purpose only
 * Confidence: low (draft) [T2 0.20] - evidence: static analysis; sources: r8_21BB88+verdicts_B
 * @end-sly2-annot */
void func_00228BB0(struct S *p)
{
    func_001F3590(p);
    func_001F68A8(p, 1);
    { struct T *t = p->t;
    t->b = 1.0f;
    t->a = 1.0f; }
}
