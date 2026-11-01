struct K { short x; short y; int i; };
extern struct K D_002A5010;
extern char D_003DD490[];
void func_001A5C48(void *p);
void func_00149A50(void *g, void *p, struct K k, int n);
/** @sly2-annot addr=0x1527C8
 * Purpose: cid 0x79 vt15: base 0x1A5C48 then registers callback 0x152810 (pointer at D_002A5010+4) with
 *   0x149A50
 * Info: proposed name PostLoadRhythmMgr (T2); class CID121 slot15; TU rhythm
 * Confidence: low (draft) [T2 0.40] - evidence: semantic review; sources: r2_131F70
 * See: docs/reference/functions/gameplay-objects.md#rhythm-0x1520c8-0x1539a8
 * @end-sly2-annot */
void func_001527C8(void *p)
{
    func_001A5C48(p);
    func_00149A50(D_003DD490, p, D_002A5010, 0);
}
