struct AR { int cb; int cbAlign; };
void *func_001AE318(struct AR *par);
void *func_002333B4(void *p, int b, int cb);
extern void *D_003DD010;
extern void *D_003DD018;
extern int D_003DD00C;
extern int D_003DD014;
/** @sly2-annot addr=0x1ED3A8
 * Purpose: Allocate two 0x3FFD-entry hash tables (bitmaps D_003DD010, cluts D_003DD018)
 * Info: proposed name AllocTexHashTables (T2); TU shd
 * Notes: names_v2 C proposed ReferenceUVAnimation__FP2QWiP3SAI (0.638)
 * Confidence: low (draft) [T2 0.45] - evidence: static naming pass; sources: r6b_rest
 * See: docs/reference/functions/rendering.md#shd-0x1ea568-0x1ed7a8
 * @end-sly2-annot */
void func_001ED3A8(void)
{
    struct AR ar;
    void **pp1 = &D_003DD010;
    void **pp2 = &D_003DD018;
    ar.cb = 0xFFF4;
    ar.cbAlign = 0x10;
    *pp1 = func_001AE318(&ar);
    func_002333B4(*pp1, 0, 0xFFF4);
    ar.cb = 0xFFF4;
    ar.cbAlign = 0x10;
    *pp2 = func_001AE318(&ar);
    func_002333B4(*pp2, 0, 0xFFF4);
    D_003DD00C = D_003DD014 = 0;
}
