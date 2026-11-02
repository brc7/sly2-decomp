struct AR { int cb; int cbAlign; };
struct X { int id; unsigned char b4; signed char cch; char pad[0x10 - 6]; unsigned int cb; void *pv; unsigned int cqw; };
struct Y { char pad[0x14]; void *pv; };
void func_00117968(void *pbis, int cb, void *pv);
void func_001179B8(void *pbis, int cb);
int FMatchLoadFilter__FiPs(int cch, void *pw);
struct Y *func_001ED4E0(int id);
void *func_001AE1D0(struct AR *par);
/** @sly2-annot addr=0x1EAF50
 * Purpose: Read one bitmap header (0x18) and pixels (filtered by name; shared via PbmpFindHash)
 * Info: proposed name LoadBmpFromBrx (T1); TU shd
 * Notes: level_parser CONFIRMED-parse: payload layout followed by the parser, 100%% of bytes in 4 levels |
 *   textures format doc cites this function by this name (CONFIRMED(code) in the doc)
 * Confidence: high [T1 0.80] - evidence: static analysis; sources:
 *   r6b_rest+parse-verified+format-doc:textures
 * See: docs/engine/rendering.md#13-key-functions,
 *   docs/formats/textures.md#1-layout-inside-a-world-entry-cstatic--cparse,
 *   docs/reference/functions/rendering.md#shd-0x1ea568-0x1ed7a8
 * @end-sly2-annot */
void LoadBmpFromBrx(void *pbis, struct X *px)
{
    union { short aw[0x80]; struct AR ar; } u;
    func_00117968(pbis, 0x18, px);
    if (px->cch > 0) {
        func_00117968(pbis, px->cch * 2, u.aw);
        if (!FMatchLoadFilter__FiPs(px->cch, u.aw)) {
            if (px->b4 != 1) {
                func_001179B8(pbis, px->cb);
                px->pv = 0;
            }
            px->b4 = 0;
            return;
        }
    }
    if (px->b4 != 1) {
        struct Y *py = func_001ED4E0(px->id);
        if (py) {
            px->pv = py->pv;
            func_001179B8(pbis, px->cb);
        } else {
            u.ar.cb = px->cb;
            u.ar.cbAlign = 0x10;
            px->pv = func_001AE1D0(&u.ar);
            func_00117968(pbis, px->cb, px->pv);
        }
    }
    px->cqw = (px->cb + 15) >> 4;
}
