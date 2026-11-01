typedef int qword __attribute__((mode(TI)));
extern qword D_002A2DD0;
struct MK { int a[4]; };
struct AI { int cb; int align; };
struct S { char pad[0x50]; int c50; void *p54; char pad2[0x94 - 0x58]; void *p94; int c98; void *p9C; };
struct MK *func_001AE478(struct MK *pm);
void func_001AE4A0(struct MK *pm, int n);
void *func_001AE378(struct AI *pa);
void *func_001AE318(struct AI *pa);
void *func_001AE238(struct AI *pa, void *pv);
void func_001733C8(struct S *p, qword q);
void LoadLoFromBrx(struct S *p, void *pv);
static inline struct AI *SetAi(struct AI *pa, int cb, int align) { pa->cb = cb; pa->align = align; return pa; }
/** @sly2-annot addr=0x1731B8
 * Purpose: Preallocates spawn-point (0x1C each), template and offset arrays, LO load, shrinks
 * Info: proposed name LoadCid119FromBrx (T2); class CID119 slot14; TU tu_173158
 * Notes: Cid77->Cid119 (CID 0x77=119, vtable) | level_parser CONFIRMED-parse: payload layout followed by the
 *   parser, 100%% of bytes in 4 levels
 * Confidence: low (draft) [T2 0.75] - evidence: static analysis; sources: r3b_rest+parse-verified
 * See: docs/engine/guards-ai.md#9-key-functions,
 *   docs/formats/level-objects.md#5-per-class-payloads-vtable-slot-14,
 *   docs/reference/functions/guards-ai.md#tu_173158-0x173158-0x174898
 * @end-sly2-annot */
void func_001731B8(struct S *p, void *pv) {
    struct MK mk;
    struct AI ai;
    func_001AE478(&mk);
    p->p54 = func_001AE378(SetAi(&ai, 0xE00, 0x10));
    p->p9C = func_001AE318(SetAi(&ai, 0x100, 0x10));
    p->p94 = func_001AE318(SetAi(&ai, 0x20, 0x10));
    func_001733C8(p, D_002A2DD0);
    LoadLoFromBrx(p, pv);
    p->p54 = func_001AE238(SetAi(&ai, p->c50 * 0x1C, 0x10), p->p54);
    { void *pv9C = func_001AE238(SetAi(&ai, p->c98 << 4, 0x10), p->p9C); void *pv94 = p->p94; p->p9C = pv9C; p->p94 = func_001AE238(SetAi(&ai, 0x20, 0x10), pv94); }
    func_001AE4A0(&mk, 2);
}
