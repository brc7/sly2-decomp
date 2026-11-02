typedef int qword __attribute__((mode(TI)));
struct MSG { int cb; int nKind; };
struct P {
    char pad[0xE0];
    int fE0;
    int nE4;
    int c;
    int *aE;
    float *aF0;
    qword *aF4;
    char pad2[0x140 - 0xF8];
    qword *a140;
    qword *a144;
};
void LoadXfmXfFromBrx(struct P *, void *);
int S8Read__18CBinaryInputStream(void *);
int U16Read__18CBinaryInputStream(void *);
void *func_001AE1D0(struct MSG *);
void Read__18CBinaryInputStreamiPv(void *, int, void *);
void ReadVector__18CBinaryInputStreamP6VECTOR(void *, qword *);
void LoadOptionsFromBrx__FPvP18CBinaryInputStream(struct P *, void *);
void MeasureShape(struct P *);
/** @sly2-annot addr=0x1E5DF8
 * Purpose: SHAPE::LoadFromBrx (vt14): XFM xf, mode(+0xE0: 0 linear, 1 curve), fClosed(+0xE4), count, then
 *   times/dists/points (+tangent arrays +0x140/+0x144 for curves), options; then MeasureShape
 * Info: proposed name LoadShapeFromBrx (T1); class SHAPE slot14; TU shape
 * Notes: level_parser CONFIRMED-parse: payload layout followed by the parser, 100%% of bytes in 4 levels
 * Confidence: high [T1 0.80] - evidence: static analysis; sources: r6b_rest+parse-verified
 * See: docs/engine/physics-collision.md#7-key-functions,
 *   docs/formats/level-objects.md#5-per-class-payloads-vtable-slot-14,
 *   docs/reference/functions/physics-collision.md#shape-0x1e5d80-0x1ea568
 * @end-sly2-annot */
void LoadShapeFromBrx(struct P *p, void *pbis)
{
    struct MSG msg;
    int i;
    LoadXfmXfFromBrx(p, pbis);
    p->fE0 = S8Read__18CBinaryInputStream(pbis);
    p->nE4 = S8Read__18CBinaryInputStream(pbis);
    p->c = U16Read__18CBinaryInputStream(pbis);
    msg.cb = p->c * 4;
    msg.nKind = 0x10;
    p->aE = func_001AE1D0(&msg);
    msg.cb = p->c * 4;
    msg.nKind = 0x10;
    p->aF0 = func_001AE1D0(&msg);
    msg.cb = p->c * 16;
    msg.nKind = 0x10;
    p->aF4 = func_001AE1D0(&msg);
    Read__18CBinaryInputStreamiPv(pbis, p->c * 4, p->aE);
    for (i = 0; i < p->c; i++)
        ReadVector__18CBinaryInputStreamP6VECTOR(pbis, &p->aF4[i]);
    if (p->fE0) {
        msg.cb = p->c * 16;
        msg.nKind = 0x10;
        p->a140 = func_001AE1D0(&msg);
        msg.cb = p->c * 16;
        msg.nKind = 0x10;
        p->a144 = func_001AE1D0(&msg);
        for (i = 0; i < p->c; i++)
            ReadVector__18CBinaryInputStreamP6VECTOR(pbis, &p->a140[i]);
        for (i = 0; i < p->c; i++)
            ReadVector__18CBinaryInputStreamP6VECTOR(pbis, &p->a144[i]);
    }
    LoadOptionsFromBrx__FPvP18CBinaryInputStream(p, pbis);
    MeasureShape(p);
}
