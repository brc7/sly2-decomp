struct SAI { int a; };
struct SHDANIM { char pad[0x24]; short du; short dv; int pad28; int fDirty; char pad2[0x34 - 0x30]; struct SAI sai; };
int func_0011A1E8(struct SAI *psai);
static inline int IFromG(float g) { return (int)(g * 4096.0f + 0.5f) & 0xFFF; }
/** @sly2-annot addr=0x1EDAD8
 * Purpose: Set the SAI UV scroll (du,dv in 1/4096 fixed point) and mark dirty, unless locked
 * Info: proposed name SetSaiDuDv (T1); TU shdanim
 * Confidence: mid (draft) [T1 0.65] - evidence: static analysis; sources: r6b_rest
 * See: docs/reference/functions/physics-collision.md#so-0x1f2108-0x1f8dd8,
 *   docs/reference/functions/rendering.md#shdanim-0x1ed7a8-0x1ee988
 * @end-sly2-annot */
void func_001EDAD8(struct SHDANIM *p, float du, float dv)
{
    int u, v;
    if (func_0011A1E8(&p->sai)) return;
    u = IFromG(du);
    v = IFromG(dv);
    if (p->du == u && p->dv == v) return;
    p->du = u;
    p->dv = v;
    p->fDirty = 1;
}
