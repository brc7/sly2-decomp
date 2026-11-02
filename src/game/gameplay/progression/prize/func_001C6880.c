struct VTE { short d; short pad; void (*pfn)(void *, int); };
struct O { int a[3]; struct VTE *vt; char pad[0x140]; int n150; char pad2[0xE4]; float g238; };
struct CLOCK { float t; float dt; };
extern struct CLOCK D_002DDED0;
void UpdateDprize(struct O *p);
/** @sly2-annot addr=0x1C6880
 * Purpose: CID 93 Update
 * Info: proposed name UpdateMedicine (T2); class MEDICINE slot16; TU prize
 * Notes: class identity from whole_game WE names/object strings: UpdateDprize93 -> UpdateMedicine
 * Confidence: low (draft) [T2 0.35] - evidence: static analysis; sources: r5_1AE130+whole_game(class
 *   identity)
 * See: docs/reference/functions/game-progression.md#prize-0x1c4e50-0x1c8cc0
 * @end-sly2-annot */
void func_001C6880(struct O *p)
{
    UpdateDprize(p);
    if (!p->n150) {
        p->g238 -= D_002DDED0.dt;
        if (p->g238 <= 0.0f) { struct VTE *e = &p->vt[76]; e->pfn((char *)p + e->d, 5); }
    }
}
