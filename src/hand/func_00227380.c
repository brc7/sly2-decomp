struct S { char pad[0xE0]; int a; int b; char c[0x10]; char d[4]; };
void func_00231768(struct S *p, void *q);
int func_00117C80(void *q);
void func_00117FD0(void *q, void *a);
void func_00118220(void *q, void *a, void *b);
void func_0013F810(struct S *p, void *q);
/** @sly2-annot addr=0x227380
 * Purpose: XFM XF read 0x231768, U32, ReadGeom, ReadBspc, LoadOptionsFromBrx
 * Info: proposed name LoadCid5ZoneFromBrx (T2); class CID5 slot14; TU vis
 * Notes: level_parser CONFIRMED-parse: payload layout followed by the parser, 100%% of bytes in 4 levels
 * Confidence: low (draft) [T2 0.75] - evidence: static analysis; sources: r8_21BB88+parse-verified
 * See: docs/engine/rendering.md#13-key-functions,
 *   docs/formats/level-objects.md#5-per-class-payloads-vtable-slot-14,
 *   docs/reference/functions/rendering.md#vis-0x227350-0x2275b8
 * @end-sly2-annot */
void func_00227380(struct S *p, void *q)
{
    func_00231768(p, q);
    p->a = func_00117C80(q);
    func_00117FD0(q, p->d);
    func_00118220(q, p->d, p->c);
    func_0013F810(p, q);
}
