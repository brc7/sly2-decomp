extern char D_002D9D00[];
struct S { int a; int b; int c; void *vt; float g10; char pad[0x1C]; float g30; float g34; char pad2[0xC]; float g44; };
/** @sly2-annot addr=0x1FAFD0
 * Purpose: Base sound-instance ctor (vt 0x2D9D00, CID 0x97): vol 1, pitch-ish 1, range 300..3000
 * Info: proposed name SND_ctor (T1); class SND; TU sound
 * Confidence: mid (draft) [T1 0.60] - evidence: static analysis; sources: r7_1F8DD8
 * See: docs/reference/functions/sound.md#sound-0x1faf70-0x1ff3b8
 * @end-sly2-annot */
struct S *func_001FAFD0(struct S *p)
{
    p->vt = D_002D9D00;
    p->a = 0; //P
    p->b = 0; //P
    p->g10 = 1.0f; //P
    p->g44 = 1.0f; //P
    p->g30 = 300.0f; //P
    p->g34 = 3000.0f; //P
    return p;
}
