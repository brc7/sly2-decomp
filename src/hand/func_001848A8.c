struct BIS {
    int grf; char pad[0x2C];
    void *vt;       /* 0x30 */
    int cbSpool;    /* 0x34 */
    char *abSpool;  /* 0x38 */
    int x3c, x40, x44, x48, x4c, x50, x54, x58, x5c, x60;
};
extern char D_002D2918[];
void func_001177A8(struct BIS *p);
/** @sly2-annot addr=0x1848A8
 * Purpose: Constructor of a CBinaryInputStream subclass: base ctor 0x1177A8, vptr D_002D2918, grf,
 *   64-byte-aligned spool buffer from (cb, pv)
 * Info: proposed name CStreamBis_ctor_1848A8 (T2); class CStreamBis; TU tu_1848A8
 * Notes: COLLISION: CStreamBis_ctor already used (same name proposed for 0x184950); renamed to
 *   CStreamBis_ctor_1848A8
 * Confidence: low (draft) [T2 0.40] - evidence: static analysis; sources: r4b_rest
 * See: docs/engine/core-library.md#4-binary-input-streams-and-cd, docs/engine/core-library.md#6-source-files,
 *   docs/engine/player.md#10-source-files
 * @end-sly2-annot */
struct BIS *func_001848A8(struct BIS *p, int cb, char *pv, int grf)
{
    char *ab;
    func_001177A8(p);
    ab = (char *)(((int)pv + 0x3F) & ~0x3F);
    p->vt = D_002D2918;
    p->grf = grf;
    p->cbSpool = ((int)pv + cb - (int)ab) & ~0x3F;
    p->x50 = -1;
    p->x3c = -1;
    p->abSpool = ab;
    p->x40 = p->x44 = p->x48 = p->x4c = p->x54 = p->x58 = p->x5c = p->x60 = 0;
    return p;
}
