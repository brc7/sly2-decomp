struct H { int a, b, c, d; };
struct AL { int cb; int cbAlign; int pad[2]; };
struct S { char p0[0x23C]; int x23C; void *x240; };
struct H *STACKSCOPE_ctor(struct H *ph);
void *func_001AE318(struct AL *pal);
void *func_001AE238(struct AL *pal, void *pv);
void STACKSCOPE_ctor_1AE4A0(struct H *ph, int n);
void LoadDprizeFromBrx(struct S *p, void *pv);
void func_001C7A08(struct S *p);
/** @sly2-annot addr=0x1C76D0
 * Purpose: CID 94 LoadFromBrx (base + copies table on stack heap)
 * Info: proposed name LoadLootFromBrx (T1); class LOOT slot14; TU prize
 * Notes: level_parser CONFIRMED-parse: payload layout followed by the parser, 100%% of bytes in 4 levels
 * Confidence: mid (draft) [T1 0.75] - evidence: static analysis; sources:
 *   r5_1AE130+verdicts_tu_r3r5+parse-verified
 * See: docs/engine/game-progression.md#8-key-functions,
 *   docs/formats/level-objects.md#5-per-class-payloads-vtable-slot-14,
 *   docs/reference/functions/game-progression.md#prize-0x1c4e50-0x1c8cc0
 * @end-sly2-annot */
void LoadLootFromBrx(struct S *p, void *pv)
{
    struct H h;
    struct AL al;
    struct AL *pal;
    STACKSCOPE_ctor(&h);
    pal = &al;
    pal->cb = 0x580;
    pal->cbAlign = 0x10;
    p->x240 = func_001AE318(pal);
    LoadDprizeFromBrx(p, pv);
    func_001C7A08(p);
    pal->cb = p->x23C * 0x2C;
    pal->cbAlign = 0x10;
    p->x240 = func_001AE238(pal, p->x240);
    STACKSCOPE_ctor_1AE4A0(&h, 2);
}
