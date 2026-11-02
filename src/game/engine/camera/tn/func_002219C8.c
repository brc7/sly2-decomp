struct V2 { float x, y; };
extern float D_002A96F8, D_002A96FC;
extern struct V2 D_002C41A8;
struct S { char pad[0x10]; int a, b; };
void InitCplcy(struct S *p);
/** @sly2-annot addr=0x2219C8
 * Purpose: base 0x14FCC0, sets globals D_002C41A8, clears fields
 * Info: proposed name InitCptn (T2); TU tn
 * Notes: names_v2 C proposed SetAloSndRepeat__FP3ALOP2LM (0.200) | agent qualifier: CPTN vt slot1
 * Confidence: low (draft) [T2 0.45] - evidence: static naming pass; sources: r8_21BB88
 * See: docs/reference/functions/camera.md#tn-0x220d40-0x223f70
 * @end-sly2-annot */
void func_002219C8(struct S *p)
{
    InitCplcy(p);
    D_002C41A8.x = D_002A96F8;
    D_002C41A8.y = D_002A96FC;
    p->a = 0;
    p->b = 0;
}
