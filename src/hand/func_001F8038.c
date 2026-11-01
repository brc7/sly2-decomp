struct P { char pad[0xA4]; int c; void **ap; };
int FIsBasicDerivedFrom__FP5BASIC3CID(void *, int);
void func_001EDAD8(void *, float, float);
/** @sly2-annot addr=0x1F8038
 * Purpose: 0x1EDAD8(f,g) on every CID 0x8F SAA
 * Info: proposed name SetSoSaaParams (T2); TU so
 * Notes: names_v2 v1 proposed GetSoCpdefi__FP2SOfP6CPDEFI (0.016)
 * Confidence: low (draft) [T2 0.35] - evidence: static naming pass; sources: r6_1D5150
 * See: docs/reference/functions/physics-collision.md#so-0x1f2108-0x1f8dd8
 * @end-sly2-annot */
void func_001F8038(struct P *p, float f, float g)
{
    int i;
    for (i = 0; i < p->c; i++) {
        void *q = p->ap[i];
        if (FIsBasicDerivedFrom__FP5BASIC3CID(q, 0x8F)) func_001EDAD8(q, f, g);
    }
}
