struct MSG { int cb; int nKind; };
struct S { char pad[0xE8]; int c; char pad2[0x1EC - 0xEC]; void *p; };
void func_001E5DF8(struct S *p);
void *func_001AE2A0(struct MSG *pmsg);
static __inline__ void *PvAllocMsg(struct MSG *pmsg, int cb, int nKind) { pmsg->cb = cb; pmsg->nKind = nKind; return func_001AE2A0(pmsg); }
/** @sly2-annot addr=0x1BC088
 * Purpose: CID 14 vt14: LoadShapeFromBrx, allocate plane array (count-1)
 * Info: proposed name LoadCid14FromBrx (T2); class CID14 slot14; TU tu_1BC050
 * Notes: level_parser CONFIRMED-parse: payload layout followed by the parser, 100%% of bytes in 4 levels
 * Confidence: low (draft) [T2 0.75] - evidence: static analysis; sources: r5b_rest+parse-verified
 * See: docs/engine/physics-collision.md#7-key-functions,
 *   docs/formats/level-objects.md#5-per-class-payloads-vtable-slot-14,
 *   docs/reference/functions/physics-collision.md#tu_1bc050-0x1bc050-0x1be398
 * @end-sly2-annot */
void func_001BC088(struct S *p)
{
    struct MSG msg;
    func_001E5DF8(p);
    p->p = PvAllocMsg(&msg, (p->c << 4) - 0x10, 0x10);
}
