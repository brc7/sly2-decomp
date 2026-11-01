struct S { char pad[0x200]; float g200; char p1[0x10]; float g214; char p2[8]; float g220; char p3[0x3C]; float g260; int n264; char p4[4]; char x26C[4]; };
void func_001C3D70(struct S *p);
void func_00204D48(struct S *p, float g);
void func_00204660(struct S *p, float g);
void func_001EEC60(void *pv, struct S *p);
/** @sly2-annot addr=0x203948
 * Purpose: STEP vt7 Init: InitPo, rGravity(0x260)=1, SetStepSpinRatio(1), SetStepRadTarget(0), base radius
 *   0x220=30, 0x214=-50, InitBhvstk(+0x26C,this)
 * Info: proposed name InitStep (T1); class STEP slot7; TU step
 * Confidence: high [T1 0.85] - evidence: static analysis; sources: r7b_guard
 * See: docs/engine/guards-ai.md#8-source-files, docs/engine/player.md#10-source-files,
 *   docs/engine/player.md#11-key-functions
 * @end-sly2-annot */
void func_00203948(struct S *p)
{
    func_001C3D70(p);
    p->n264 = -1;
    p->g260 = 1.0f;
    p->g200 = 1.0f;
    func_00204D48(p, 1.0f);
    func_00204660(p, 0.0f);
    p->g220 = 30.0f;
    p->g214 = -50.0f;
    func_001EEC60(p->x26C, p);
}
