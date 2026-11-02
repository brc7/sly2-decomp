struct S { int x0; char pad[0x1C0]; int x1c4; };
extern char D_002A3868[], D_002A38A8[];
void func_001A5020(struct S *p, void *b);
void func_001A50F8(struct S *p);
void func_001A5078(struct S *p, void *b, int n, int m);
void func_001A4E80(struct S *p);
/** @sly2-annot addr=0x1A4E08
 * Purpose: Reset the step queue (count 0, cursor 0), queue a movie (D_002A3868), a memcard step (5), a world
 *   warp to D_002A38A8 oid 0x189, then run the first step
 * Info: proposed name StartBootSequence (T2); TU tu_1A4E08
 * Confidence: low (draft) [T2 0.35] - evidence: static analysis; sources: r4b_rest
 * See: docs/engine/rendering.md#12-source-files,
 *   docs/engine/world-sw.md#4-transitions-warps-exits-wipes-boot-queue,
 *   docs/engine/world-sw.md#9-source-files
 * @end-sly2-annot */
void func_001A4E08(struct S *p)
{
    p->x0 = 0;
    p->x1c4 = 0;
    func_001A5020(p, D_002A3868);
    func_001A50F8(p);
    func_001A5078(p, D_002A38A8, 0x189, 1);
    func_001A4E80(p);
}
