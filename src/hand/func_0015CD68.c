struct S { char pad[0x110]; int a; int b; int c; int d; int e; };
extern struct S *D_003E1C04;
void func_001A5270(struct S *p);
/** @sly2-annot addr=0x15CD68
 * Purpose: LO::Init, four ids = -1, registers itself as the singleton D_003E1C04
 * Info: proposed name InitWidget131 (T2); class WIDGET131 slot7; TU tu_15CD68
 * Notes: names_v2 v1 proposed InitExpls__FP5EXPLS (0.638) | Cid83->Widget131 (CID 0x83=131, vtable)
 * Confidence: low (draft) [T2 0.75] - evidence: static naming pass; sources: r3_1539A8
 * See: docs/engine/hud-ui.md#4-source-files, docs/engine/particles-effects.md#5-source-files,
 *   docs/reference/functions/hud-ui.md#tu_15cd68-0x15cd68-0x15dc80
 * @end-sly2-annot */
void func_0015CD68(struct S *p)
{
    func_001A5270(p);
    p->a = -1;
    p->b = -1;
    p->d = -1;
    p->e = -1;
    D_003E1C04 = p;
}
