typedef int qword __attribute__((mode(TI)));
struct S { char pad[0x1F30]; qword a; qword b; qword c; };
/** @sly2-annot addr=0x185E30
 * Purpose: Store three vectors
 * Info: proposed name SetMeshVectors (T2); TU tu_185130
 * Confidence: low (draft) [T2 0.25] - evidence: static analysis; sources: r4b_rest
 * See: docs/reference/functions/player.md#tu_185130-0x185130-0x186610
 * @end-sly2-annot */
void func_00185E30(struct S *p, qword a, qword b, qword c)
{
    p->a = a;
    p->b = b;
    p->c = c;
}
