struct T { char pad[0x1C]; int x1C; int x20; int x24; int x28; };
struct S { char pad[0x114]; struct T *x114; char x118[0x58]; int x170; char pad2[0x6C]; int x1E0; };
void func_00132930(struct T *t);
void func_00234CA0(int n);
int sceMpegDemuxPss(void *p, int a, int n);
void func_00132998(struct T *t, int r);
/** @sly2-annot addr=0x1B41D0
 * Purpose: spool async stream and demux into sceMpeg (returns 2 at EOF)
 * Info: proposed name CMpeg_CbDemuxed (T1); class CMpeg; TU mpeg
 * Notes: verifier side finding names it CbDemuxed | superseded: CMpeg_Update (arbitration) | arbitration
 *   adopt verifier: CONFIRMED static: body = Sly1 CMpeg::CbDemuxed(int fBlock): while stream empty Spool,
 *   return 0 if still empty and !fBlock; FlushCache(0); demux (0x24F770 = sceM | arbitration mangling:
 *   compile a declaration in the scratch build and compare before committing
 * Confidence: high [T1 0.85] - evidence: Sly 1 / Lilly reference; arbitration; sources: r5_1AE130+arbitration
 * See: docs/engine/cutscenes.md#5-key-functions, docs/reference/functions/cutscenes.md#mpeg-0x1b2820-0x1b4438
 * @end-sly2-annot */
int CbDemuxed__5CMpegi(struct S *p, int wait)
{
    int n, r;
    n = p->x114->x1C - p->x114->x24;
    while (n == 0) {
        if (!p->x114->x28) return 2;
        func_00132930(p->x114);
        n = p->x114->x1C - p->x114->x24;
        if (n == 0 && !wait) return 0;
    }
    func_00234CA0(0);
    r = sceMpegDemuxPss(p->x118, p->x114->x20 + p->x114->x24, n);
    func_00132998(p->x114, r);
    if (r) return 0;
    if (p->x170 >= n && p->x1E0 >= n) return 2;
    return 1;
}
