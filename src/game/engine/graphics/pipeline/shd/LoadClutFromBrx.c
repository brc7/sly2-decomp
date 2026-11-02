struct AR { int cb; int cbAlign; };
struct X { int id; unsigned char b4; signed char cch; char pad[2]; unsigned short c; char pad2[2]; int *pv; unsigned int cqw; };
struct Y { char pad[0xC]; int *pv; };
void Read__18CBinaryInputStreamiPv(void *pbis, int cb, void *pv);
void Skip__18CBinaryInputStreami(void *pbis, int cb);
int FMatchLoadFilter__FiPs(int cch, void *pw);
struct Y *func_001ED5D0(int id);
void *func_001AE1D0(struct AR *par);
void ColorCorrectRgba(int *p);
/** @sly2-annot addr=0x1EA8A8
 * Purpose: Read one color table header (0x10) and colors; skip if its name fails the load filter; shared
 *   entries resolved via PclutFindHash; color-correct entries
 * Info: proposed name LoadClutFromBrx (T1); TU shd
 * Notes: level_parser CONFIRMED-parse: payload layout followed by the parser, 100%% of bytes in 4 levels |
 *   textures format doc cites this function by this name (CONFIRMED(code) in the doc)
 * Confidence: high [T1 0.80] - evidence: static analysis; sources:
 *   r6b_rest+parse-verified+format-doc:textures
 * See: docs/engine/rendering.md#13-key-functions,
 *   docs/formats/textures.md#1-layout-inside-a-world-entry-cstatic--cparse,
 *   docs/reference/functions/rendering.md#shd-0x1ea568-0x1ed7a8
 * @end-sly2-annot */
void LoadClutFromBrx(void *pbis, struct X *px)
{
    union { short aw[0x80]; struct AR ar; } u;
    unsigned int cb;
    Read__18CBinaryInputStreamiPv(pbis, 0x10, px);
    cb = px->c * sizeof(int);
    if (px->cch > 0) {
        Read__18CBinaryInputStreamiPv(pbis, px->cch * 2, u.aw);
        if (!FMatchLoadFilter__FiPs(px->cch, u.aw)) {
            if (px->b4 != 1)
                Skip__18CBinaryInputStreami(pbis, cb);
            px->b4 = 0;
            return;
        }
    }
    if (px->b4 != 1) {
        struct Y *py = func_001ED5D0(px->id);
        if (py) {
            px->pv = py->pv;
            Skip__18CBinaryInputStreami(pbis, cb);
        } else {
            int *pn;
            u.ar.cb = cb;
            u.ar.cbAlign = 0x10;
            px->pv = func_001AE1D0(&u.ar);
            Read__18CBinaryInputStreamiPv(pbis, cb, px->pv);
            for (pn = px->pv; pn < px->pv + px->c; pn++)
                ColorCorrectRgba(pn);
        }
    }
    px->cqw = (cb + 15) >> 4;
}
