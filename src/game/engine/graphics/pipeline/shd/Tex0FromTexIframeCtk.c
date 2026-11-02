typedef unsigned long ulong;
struct E { char pad[0x18]; ulong a; ulong b; };
struct S { int x0, x4; void *p8; struct E **apeC; struct E **ape10; };
int func_001EA688(void *p);
/** @sly2-annot addr=0x1EA6C8
 * Purpose: TEX0 register value for frame i / pass j of a texture: base bitmap TEX0 | clut TEX0
 * Info: proposed name Tex0FromTexIframeCtk (T1); TU shd
 * Notes: textures format doc cites this function by this name (CONFIRMED(code) in the doc)
 * Confidence: mid (draft) [T1 0.65] - evidence: static analysis; sources: r6b_rest+format-doc:textures
 * See: docs/formats/textures.md#4-shd-record-shader-and-tex-cstatic,
 *   docs/reference/functions/rendering.md#shd-0x1ea568-0x1ed7a8
 * @end-sly2-annot */
ulong Tex0FromTexIframeCtk(struct S *p, int i, int j)
{
    ulong fl = p->apeC[i]->b;
    struct E *pe = p->ape10[i * func_001EA688(p->p8) + j];
    if (pe)
        fl |= pe->a;
    return fl;
}
