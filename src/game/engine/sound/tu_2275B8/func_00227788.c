struct C { unsigned char a[4]; };
struct S { int k; struct C c; int z; };
void RgbaFromSpeakerCid(struct C *pc, int a);
/** @sly2-annot addr=0x227788
 * Purpose: kind=-1, default colour, flag 0
 * Info: proposed name InitSpeakerLabel (T2); TU tu_2275B8
 * Confidence: low (draft) [T2 0.30] - evidence: static analysis; sources: r8_21BB88
 * See: docs/reference/functions/sound.md#tu_2275b8-0x2275b8-0x227bb8
 * @end-sly2-annot */
void func_00227788(struct S *p)
{
    struct C c;
    p->k = -1;
    RgbaFromSpeakerCid(&c, -1);
    p->c = c;
    p->z = 0;
}
