typedef int qword __attribute__((mode(TI)));
struct E { int a; int b; int c; char p[0x10]; int x1c; float g; int x24; int p2; float h; };
struct S { char pad[0x1C8]; int c; struct E ae[4]; };
void func_001F50E0(struct S *p);
void func_001E7BB8(int n, qword *pa, qword *pb);
void func_001F5480(struct S *p, qword q, float g);
/** @sly2-annot addr=0x182780
 * Purpose: vt49 override
 * Info: proposed name HACKWALL_vt49 (T2); class HACKWALL slot49; TU tu_17F090
 * Notes: CID4C->CID76 (CID 0x4C=76, hex-literal) | class identity from whole_game WE names/object strings:
 *   CID76_vt49 -> HACKWALL_vt49
 * Confidence: low (draft) [T2 0.25] - evidence: static analysis; sources: r4b_rest+whole_game(class identity)
 * See: docs/reference/functions/gameplay-objects.md#tu_17f090-0x17f090-0x182c70
 * @end-sly2-annot */
void func_00182780(struct S *p)
{
    int i;
    func_001F50E0(p);
    for (i = 0; i < p->c; i++) {
        struct E *pe = &p->ae[i];
        qword a, b;
        func_001E7BB8(pe->x24, &a, &b);
        func_001F5480(p, a, pe->g);
        func_001F5480(p, b, pe->g);
    }
}
