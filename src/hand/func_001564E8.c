typedef struct LM { float gMin; float gMax; } LM;
extern float D_002BDCD0;
extern LM D_002BDCD8;
struct S { char pad[0x40]; int fl; char pad2[0x10]; float g54; LM lm; float g60; };
void func_001A5270(struct S *p);
/** @sly2-annot addr=0x1564E8
 * Purpose: LO::Init (0x1A5270) then defaults g54/lm/g60=0.25 and flag 0x40
 * Info: proposed name InitCid117 (T2); class CID117 slot7; TU noise
 * Notes: Cid75->Cid117 (CID 0x75=117, vtable)
 * Confidence: low (draft) [T2 0.75] - evidence: static analysis; sources: r3_1539A8
 * See: docs/engine/guards-ai.md#8-source-files,
 *   docs/reference/functions/guards-ai.md#noise-0x1564e8-0x157200,
 *   docs/reference/tu-map.md#tu-map-address-range---source-file
 * @end-sly2-annot */
void func_001564E8(struct S *p)
{
    func_001A5270(p);
    p->g54 = D_002BDCD0;
    p->lm = D_002BDCD8;
    p->g60 = 0.25f;
    p->fl |= 0x40;
}
