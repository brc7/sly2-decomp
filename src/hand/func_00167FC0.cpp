#include "vifs.h"
struct X { char pad[0x120]; QW amat[3]; int f150[4]; int f160; };
struct G;
extern G D_003DD490;
extern char D_0025E090[];
extern char D_0025E668[];
extern "C" void func_00160918(VIFS *pvifs, void *pvStart, void *pvEnd);
extern "C" int func_0015E9C8(G *pg, int kind, int n, int m, int c, void **apv);
extern "C" void func_001AE5A0(void *pvDst, void *pvSrc, unsigned int cb);
static inline int Min(int a, int b) { return a < b ? a : b; }
/** @sly2-annot addr=0x167FC0
 * Purpose: Pass 4 begin (kind 0): upload relight microprogram D_0025E090..D_0025E668 (compute only), BASE
 *   0x32 OFFSET 0, upload up to 256 lights x 3 qw
 * Info: proposed name BeginRpRelight (T1); TU glob
 * Notes: verifier: purpose partly wrong: No name proposed. Pass-table entry 4 begin: VU1 code + UNPACK of up
 *   to 256 objects' 0x30-byte matrices. Wrong detail: 0x505 is the grffso; class arg is cid 4 = | no usable
 *   identifier; purpose only
 * Confidence: mid (draft) [T1 0.70] - evidence: static analysis; sources:
 *   r3_1539A8+verdicts_tu_r3r5+rendering
 * See: docs/engine/rendering.md#4-the-26-kinds-and-12-passes-cstatic-tables-dumped-from-the-elf-roles-cstatic-unless-marked,
 *   docs/engine/rendering.md#13-key-functions, docs/reference/functions/rendering.md#glob-0x167310-0x16f148
 * @end-sly2-annot */
extern "C" void func_00167FC0(VIFS *pvifs)
{
    X *apx[0x180];
    int i;
    X *px;
    int cActive = 0;
    func_00160918(pvifs, D_0025E090, D_0025E668);
    pvifs->AddDmaCnt();
    pvifs->AddVifFlusha();
    pvifs->AddVifBaseOffset(0x32, 0);
    int c = func_0015E9C8(&D_003DD490, 0x505, 4, 0, 0x180, (void **)apx);
    for (i = 0; i < c; i++) {
        px = apx[i];
        if (!px->f160)
            cActive++;
    }
    if (cActive > 0x100)
        cActive = 0x100;
    uint *pun;
    pvifs->UnpackHelper(UPK_V4_32, 1, 0, 0, &pun);
    pun[0] = 1;
    pun[1] = cActive * 3 + 1;
    uint *pb = 0;
    int n = 0;
    int iEnd = 0;
    for (i = 0; i < c; i++) {
        px = apx[i];
        if (px->f160)
            continue;
        if (n >= iEnd) {
            iEnd = n + Min(cActive - n, 32);
            pvifs->UnpackHelper(UPK_V4_32, (iEnd - n) * 3, n * 3 + 1, 0, &pb);
        }
        func_001AE5A0(pb, px->amat, 0x30);
        n++;
        pb = (uint *)((char *)pb + 0x30);
        if (n >= 0x100)
            break;
    }
    pvifs->EndDmaCnt();
}
