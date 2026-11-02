struct ALLOCREQ { int cb; int nKind; };
class CBinaryInputStream {
public:
    int U8Read();
    int U16Read();
    int S16Read();
};
struct E6 { short a; short b; char c; char pad; };
struct E8 { int c; E6 *a; };
struct T { int c; E8 *a; int *ai; };
extern "C" void *func_001AE378(ALLOCREQ *preq);
extern "C" void func_001ED1A8(short *ps);
static inline void *PvAllocReq(ALLOCREQ *preq, int cb, int nKind)
{
    preq->cb = cb;
    preq->nKind = nKind;
    return func_001AE378(preq);
}
/** @sly2-annot addr=0x16C8E0
 * Purpose: Reads a u16-counted table of vertex-binding groups (u16/u16/u8 records) and fixes up indices
 *   (0x1ED1A8)
 * Info: proposed name LoadClutsetFromBrx (T2); TU glob
 * Notes: no usable identifier; purpose only | rendering proposes LoadSubglobBindingsFromBrx (0.35) |
 *   MESH_FORMAT + LEVEL_FORMAT both call this block CLUTSET; rendering proposes LoadSubglobBindingsFromBrx
 *   (0.35) | level_parser CONFIRMED-parse: payload layout followed by the parser, 100%% of bytes in 4 levels
 *   | T2 forced: noun disputed (CLUTSET vs subglob bindings); layout parse-verified
 * Confidence: low (draft) [T2 0.75] - evidence: static analysis; sources:
 *   r3_1539A8+rendering+format-docs+parse-verified
 * See: docs/engine/rendering.md#13-key-functions,
 *   docs/formats/meshes-scene.md#type-0-the-visible-textured-mesh-loadsubglobtype0frombrx-0x16a000-cparse,
 *   docs/formats/meshes-scene.md#types-1-3-byte-layouts-cparse-roles-h-except-where-noted
 * @end-sly2-annot */
extern "C" T *func_0016C8E0(void *pv, CBinaryInputStream *pbis)
{
    T *pt = 0;
    int c = pbis->U16Read();
    if (c) {
        ALLOCREQ req;
        pt = (T *)PvAllocReq(&req, sizeof(T), 0x10);
        pt->c = c;
        pt->a = (E8 *)PvAllocReq(&req, c * sizeof(E8), 0x10);
        for (int i = 0; i < pt->c; i++) {
            E8 *pe = &pt->a[i];
            int n = pbis->U16Read();
            pe->a = (E6 *)PvAllocReq(&req, n * sizeof(E6), 0x10);
            for (int j = 0; j < n; j++) {
                E6 *p6 = &pe->a[j];
                short *pb = &p6->b;
                p6->a = pbis->U16Read();
                p6->b = pbis->U16Read();
                p6->c = pbis->U8Read();
                func_001ED1A8(&p6->a);
                func_001ED1A8(pb);
            }
        }
        int m = pbis->U16Read();
        pt->ai = (int *)PvAllocReq(&req, m * 4, 0x10);
        for (int k = 0; k < m; k++)
            pt->ai[k] = pbis->S16Read();
    }
    return pt;
}
