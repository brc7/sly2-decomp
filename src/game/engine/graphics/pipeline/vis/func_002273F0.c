typedef int qword __attribute__((mode(TI)));
struct S { char pad[0xF4]; int x; };
qword ConvertAloPos(int a, struct S *p, qword q);
int PbspPointInBspQuick(int a, int b, qword q);
void func_00227440(struct S *p, int f);
/** @sly2-annot addr=0x2273F0
 * Purpose: BSP point test of a world position (converted to local) -> SetCid5ZoneInside
 * Info: proposed name UpdateCid5ZoneInside (T2); class CID5; TU vis
 * Confidence: low (draft) [T2 0.40] - evidence: static analysis; sources: r8_21BB88
 * See: docs/engine/camera.md#5-key-functions, docs/reference/functions/camera.md#cm-0x14d3d8-0x14fac8,
 *   docs/reference/functions/rendering.md#vis-0x227350-0x2275b8
 * @end-sly2-annot */
void func_002273F0(struct S *p, qword q)
{
    func_00227440(p, PbspPointInBspQuick(p->x, 0x7FFFFFFF, ConvertAloPos(0, p, q)) != 0);
}
