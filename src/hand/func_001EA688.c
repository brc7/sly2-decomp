struct S { int n0; unsigned char b4; unsigned char b5; };
/** @sly2-annot addr=0x1EA688
 * Purpose: Texture passes for a shader kind: shdk 1 -> 2, shdk 7 -> 3 (2 if flag 0x20), else 1
 * Info: proposed name CpassShd (T2); TU shd
 * Confidence: low (draft) [T2 0.35] - evidence: static analysis; sources: r6b_rest
 * See: docs/formats/textures.md#4-shd-record-shader-and-tex-cstatic,
 *   docs/reference/functions/rendering.md#shd-0x1ea568-0x1ed7a8
 * @end-sly2-annot */
int func_001EA688(struct S *p)
{
    switch (p->b4) {
    case 1:
        return 2;
    case 7:
        if (!(p->b5 & 0x20)) return 3;
        return 2;
    }
    return 1;
}
