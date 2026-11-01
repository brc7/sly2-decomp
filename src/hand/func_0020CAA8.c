struct M { int a, b, n; };
struct S { char pad[0x34]; int f34; int f38; };
void func_00133590(struct S *p, int msg, struct M *pm);
/** @sly2-annot addr=0x20CAA8
 * Purpose: CID 0x92 slot5: labels 0x17D/0x17E fForward, 0x17F/0x180 fBackward
 * Info: proposed name HandleDirAnimMessage (T2); class CID146 slot5; TU tu_20B840
 * Notes: T2 forced: verifier: coined class word without descriptor
 * Confidence: low (draft) [T2 0.85] - evidence: static analysis; sources: r7c_hud+verdicts_followups
 * See: docs/engine/animation.md#7-key-functions,
 *   docs/reference/functions/animation.md#tu_20b840-0x20b840-0x20ce28
 * @end-sly2-annot */
void func_0020CAA8(struct S *p, int msg, struct M *pm)
{
    if (msg == 9) {
        switch (pm->n) {
        case 0x17D: p->f34 = 1; break;
        case 0x17E: p->f34 = 0; break;
        case 0x17F: p->f38 = 1; break;
        case 0x180: p->f38 = 0; break;
        }
    }
    func_00133590(p, msg, pm);
}
