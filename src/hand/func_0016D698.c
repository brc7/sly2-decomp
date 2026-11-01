struct D { int a; char *b; int c; char *d; };
struct H { short a; short b; };
/** @sly2-annot addr=0x16D698
 * Purpose: Records a (framebuffer word index, qword offset+0x23) patch for the VU1 microprogram, which fills
 *   FRAME/TEX0 of the current buffer from VU qw 0xB/0xC
 * Info: proposed name AddFramePatch (T1); TU glob
 * Notes: no usable identifier; purpose only
 * Confidence: mid (draft) [T1 0.50] - evidence: static analysis; sources: r3_1539A8+rendering
 * See: docs/reference/functions/rendering.md#glob-0x167310-0x16f148
 * @end-sly2-annot */
void func_0016D698(int n, struct D *pd, struct H **pph)
{
    struct H *ph = (*pph)++;
    ph->a = n;
    ph->b = ((pd->d - pd->b) >> 4) + 0x23;
}
