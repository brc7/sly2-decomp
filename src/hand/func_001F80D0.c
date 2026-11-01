struct L { char pad[0x24]; struct L *next; char pad2[0x64 - 0x28]; struct L *first; char pad3[0x70 - 0x68]; struct L *end; char pad4[0xA4 - 0x74]; int c; void **ap; };
int func_00132B10(void *, int);
void func_001EE3B0(void *, float);
/** @sly2-annot addr=0x1F80D0
 * Purpose: SetEyesClosed(f) on EYES (CID 0x8E) SAAs, recursively through SO children
 * Info: proposed name SetSoEyesClosed (T2); TU so
 * Notes: names_v2 v1 proposed PsoFirstSoTouching__FP2SO (0.087)
 * Confidence: low (draft) [T2 0.45] - evidence: static naming pass; sources: r6_1D5150
 * See: docs/reference/functions/physics-collision.md#so-0x1f2108-0x1f8dd8,
 *   docs/reference/functions/rendering.md#shdanim-0x1ed7a8-0x1ee988,
 *   docs/reference/functions/sound.md#sound-0x1faf70-0x1ff3b8
 * @end-sly2-annot */
void func_001F80D0(struct L *p, float f)
{
    int i;
    struct L *q;
    for (i = 0; i < p->c; i++) {
        void *pv = p->ap[i];
        if (func_00132B10(pv, 0x8E)) func_001EE3B0(pv, f);
    }
    for (q = p->first; q != p->end; q = q->next)
        if (func_00132B10(q, 0x30)) func_001F80D0(q, f);
}
