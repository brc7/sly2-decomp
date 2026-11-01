typedef int qword __attribute__((mode(TI)));
struct CLOCK { float t; float dt; };
extern struct CLOCK D_002DDED0;
extern char D_002DDF40[];
struct V { char pad[0x20]; qword q20; };
struct T { char pad[0x35C]; struct V *p35C; };
struct S { char pad[0x10]; struct T *pt; char pad2[0x9C]; float gB0; float gB4; };
void func_00174B68(struct S *p);
qword func_001A62E8(void *p);
void func_0014F750(void *p, int n);
void func_001F7A68(struct T *t, int n);
/** @sly2-annot addr=0x1795E8
 * Purpose: BHVHIT Update: BHVANIM update, move held noise with guard, fire delayed camera jolt and zap-mode
 *   change
 * Info: proposed name UpdateBhvhit (T1); class BHVHIT slot10; TU guardbhv
 * Notes: names_v2 B RenderFastShadow__FP3ALOP2CMP2RO graded WRONG | no usable identifier; purpose only
 * Confidence: mid (draft) [T1 0.60] - evidence: static naming pass; sources:
 *   verify:verdicts_B+verdicts_B+r9_bhv_screen
 * See: docs/reference/functions/guards-ai.md#guardbhv-0x174898-0x17f090
 * @end-sly2-annot */
void func_001795E8(struct S *p) {
    float t;
    func_00174B68(p);
    { struct T *pt = p->pt; struct V *v = pt->p35C; v->q20 = func_001A62E8(pt); }
    t = D_002DDED0.t + 0.0001f;
    if (p->gB0 < t) { p->gB0 = 3.40282346638528859812e+38f; func_0014F750(D_002DDF40, 0); }
    if (p->gB4 < t) { p->gB4 = 3.40282346638528859812e+38f; func_001F7A68(p->pt, 2); }
}
