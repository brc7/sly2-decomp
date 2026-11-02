typedef unsigned long ulong;
struct TEX1 { ulong LCM : 1; ulong pad1 : 1; ulong MXL : 3; ulong MMAG : 1; ulong MMIN : 3; ulong MTBA : 1; ulong pad2 : 9; ulong L : 2; ulong pad3 : 11; ulong K : 12; ulong pad4 : 20; };
struct CLAMP { ulong WMS : 2; ulong WMT : 2; ulong MINU : 10; ulong MAXU : 10; ulong MINV : 10; ulong MAXV : 10; ulong pad : 20; };
struct S8 { char pad[0x24]; int c; };
struct Y { int a; unsigned short grf; short pad; struct S8 *ps8; };
ulong func_001EA6C8(struct Y *py, int i, int n);
void func_00166E90(void *pq, int reg, ulong val);
void *func_002333B4(void *p, int b, int cb);
/** @sly2-annot addr=0x1EA750
 * Purpose: Pack the TEX0 per bitmap plus TEX1 (LCM/MMAG/MMIN by pass kind) and CLAMP regs into a GIF A+D
 *   stream
 * Info: proposed name PackTexGifs (T1); TU shd
 * Confidence: mid (draft) [T1 0.50] - evidence: static analysis; sources: r6b_rest
 * See: docs/reference/functions/rendering.md#shd-0x1ea568-0x1ed7a8
 * @end-sly2-annot */
void func_001EA750(struct Y *py, int n, void *pq)
{
    struct TEX1 tex1;
    struct CLAMP clamp;
    int i;
    for (i = py->ps8->c - 1; i >= 0; i--)
        func_00166E90(pq, 6, func_001EA6C8(py, i, n));
    func_002333B4(&tex1, 0, 8);
    tex1.LCM = 1;
    if (n == 2) {
        tex1.MMAG = 0;
        tex1.MMIN = 0;
    } else {
        tex1.MMAG = 1;
        tex1.MMIN = 1;
    }
    func_00166E90(pq, 0x14, *(ulong *)&tex1);
    func_002333B4(&clamp, 0, 8);
    if (py->grf & 1) clamp.WMS = 1;
    if (py->grf & 2) clamp.WMT = 1;
    func_00166E90(pq, 8, *(ulong *)&clamp);
}
