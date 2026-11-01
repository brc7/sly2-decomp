typedef int qword __attribute__((mode(TI)));
struct S { char pad[0x1880]; int n; char p1[0xC]; qword a; qword b; qword c; char p2[0x18]; int x18d8; char p3[0xC8]; void *x19a4; };
qword func_001A62E8(void *p);
void func_001F3980(struct S *p);
/** @sly2-annot addr=0x18A278
 * Purpose: JT vt17: cache PosLoWorld of x19A4 child, call SO vt17 0x1F3980, record self/child positions when
 *   x18D8==0
 * Info: proposed name JT_vt17 (T2); class JT slot17; TU jt
 * Confidence: low (draft) [T2 0.30] - evidence: static analysis; sources: r4_17F090
 * See: docs/reference/functions/player.md#jt-0x188488-0x18e540
 * @end-sly2-annot */
void func_0018A278(struct S *p)
{
    p->b = func_001A62E8(p->x19a4);
    p->n = 0;
    func_001F3980(p);
    if (!p->x18d8) {
        p->a = func_001A62E8(p);
        p->c = func_001A62E8(p->x19a4);
        p->n = 3;
    } else {
        p->n = 0;
    }
}
