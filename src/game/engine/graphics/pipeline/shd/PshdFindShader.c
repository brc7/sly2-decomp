
struct E { int oid; char pad[0x28]; };
extern int D_003DCFE8;
extern struct E *D_003DCFEC;
/** @sly2-annot addr=0x1EC820
 * Purpose: Find shader by oid in D_003DCFEC[D_003DCFE8] (stride 0x2C); -1 -> NULL
 * Info: proposed name PshdFindShader (T1); TU shd
 * Notes: textures format doc cites this function by this name (CONFIRMED(code) in the doc)
 * Confidence: mid (draft) [T1 0.75] - evidence: static analysis; sources: r6b_rest+format-doc:textures
 * See: docs/engine/rendering.md#13-key-functions, docs/reference/functions/rendering.md#shd-0x1ea568-0x1ed7a8
 * @end-sly2-annot */
struct E *PshdFindShader(int oid)
{
    int i;
    if (oid == -1) return 0;
    for (i = 0; i < D_003DCFE8; i++) {
        if (D_003DCFEC[i].oid == oid) return &D_003DCFEC[i];
    }
    return 0;
}
