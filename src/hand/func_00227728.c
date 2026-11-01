struct C { unsigned char a[4]; };
struct S { int k; struct C c; int z; };
void func_00227788(struct S *p);
void func_002275B8(struct C *pc, int a);
/** @sly2-annot addr=0x227728
 * Purpose: reset then kind=k, colour from kind
 * Info: proposed name SetSpeaker (T2); TU tu_2275B8
 * Notes: agent qualifier: kind+colour
 * Confidence: low (draft) [T2 0.30] - evidence: static analysis; sources: r8_21BB88
 * See: docs/reference/functions/sound.md#sound-0x1faf70-0x1ff3b8,
 *   docs/reference/functions/sound.md#tu_2275b8-0x2275b8-0x227bb8
 * @end-sly2-annot */
void func_00227728(struct S *p, int k, int b)
{
    struct C c;
    func_00227788(p);
    p->k = k;
    func_002275B8(&c, b);
    p->c = c;
}
