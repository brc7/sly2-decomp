extern char D_002DDF40[];
typedef struct S { char pad[0x130]; float g; } S;
typedef struct T { int pad[4]; S *ps; } T;
void func_0014FCC8(T *t, void *b, S *s);
void func_0014F5A0(void *a, float g);
/** @sly2-annot addr=0x12E798
 * Purpose: CPLCY::Activate (0x14FCC8) then SetCmFov
 * Info: proposed name ActivateCpcamera (T2); class CPCAMERA; TU tu_12E600
 * Notes: verify r1r2: CplcyCid44 is Lilly CPCAMERA (CM+0x4A8)
 * Confidence: low (draft) [T2 0.40] - evidence: Sly 1 / Lilly reference; sources: r1_116918
 * See: docs/reference/functions/camera.md#tu_12e600-0x12e600-0x12eb48
 * @end-sly2-annot */
void func_0012E798(T *t, void *b, S *s)
{
    func_0014FCC8(t, b, s);
    t->ps = s;
    if (s->g != 0.0f)
        func_0014F5A0(D_002DDF40, s->g);
}
