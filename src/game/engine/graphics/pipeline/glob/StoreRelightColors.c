/* RGBA (one byte per qword lane) -> 1555 conversion. The C16 bitfield struct copy gives the target's dead
   register copies after each sh. */
typedef struct { unsigned char r, pad0[3], g, pad1[3], b, pad2[3], a, pad3[3]; } CQW;
struct X { int pad[2]; int iA; int iB; int iEnd; char *ib14; char *ib18; int pad2; char *ib20; char *ib24; };
struct S { char pad[0x3010]; int pbBase; char pad2[0x3070 - 0x3014]; struct X x; };
typedef struct { unsigned short rgb : 15, a : 1; } C16;
/** @sly2-annot addr=0x167E58
 * Purpose: Relight readback: converts the two RGBA results per vertex in VU1 data memory to RGBA5551 into the
 *   glob's EE colour buffers (+alpha bytes)
 * Info: proposed name StoreRelightColors (T1); TU glob
 * Notes: no usable identifier; purpose only
 * Confidence: mid (draft) [T1 0.65] - evidence: static analysis; sources: r3_1539A8+rendering
 * See: docs/engine/rendering.md#6-lighting-relight-and-outlines-the-cel-look,
 *   docs/engine/rendering.md#13-key-functions, docs/guides/port-plan.md#62-which-vu-programs-become-what
 * @end-sly2-annot */
void StoreRelightColors(struct S *p)
{
    struct X *px = &p->x;
    int pbBase = p->pbBase;
    CQW *pqwA = (CQW *)p + px->iA;
    CQW *pqwB = (CQW *)p + px->iB;
    CQW *pqwEnd = (CQW *)p + px->iEnd;
    C16 *ps1 = (C16 *)(px->ib14 + pbBase);
    C16 *ps2 = (C16 *)(px->ib18 + pbBase);
    unsigned char *pb2 = (unsigned char *)(px->ib20 + pbBase);
    unsigned char *pbAlpha = (unsigned char *)(px->ib24 + pbBase);
    for (; pqwB < pqwEnd; pqwB++, pqwA++) {
        C16 c1, c2;
        C16 *pd1, *pd2;
        pd1 = ps1++;
        pd2 = ps2++;
        {
            int g = (pqwA->g >> 3) << 5;
            int r = pqwA->r >> 3;
            c1.rgb = g | r | ((pqwA->b >> 3) << 10);
        }
        c1.a = pqwA->a >> 7;
        *pd1 = c1;
        {
            int r = pqwB->r >> 3;
            int g = (pqwB->g >> 3) << 5;
            c2.rgb = g | r | ((pqwB->b >> 3) << 10);
        }
        c2.a = pqwA->a >> 7;
        *pd2 = c2;
        *pbAlpha++ = pqwB->a;
        if (px->ib20)
            *pb2++ = pqwA->a;
    }
}
