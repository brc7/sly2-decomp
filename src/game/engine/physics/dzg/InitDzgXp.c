typedef struct MSG { int cb; int nKind; } MSG;
void *func_001AE378(MSG *pmsg);
static inline void *PvAllocMsg(MSG *pmsg, int cb, int nKind) { pmsg->cb = cb; pmsg->nKind = nKind; return func_001AE378(pmsg); }
struct S { int cdzMax; char pad[0x4C]; int a; int b; int c; int d; void *pv; };
struct S *InitDzg(struct S *p, int c);
/** @sly2-annot addr=0x1585D8
 * Purpose: Initialises a DZG constraint group: base init (0x11A550) with 2*cdz rows, stores four params,
 *   allocates 0x60 bytes per row
 * Info: proposed name InitDzgXp (T1); TU dzg
 * Notes: COLLISION: InitDzg already used (same name proposed for 0x11A550); renamed to InitDzg_1585D8 |
 *   superseded: InitDzg_1585D8 (arbitration) | arbitration other name: CONFIRMED static: InitDzg(pdzg,2*cpxp)
 *   then stores cpxp,+0x54,+0x58,+0x5C (= Sly1 SolveDzg args apxp/acpso/aapso moved into the DZG) and
 *   allocates a 0x60-byte r
 * Confidence: mid (draft) [T1 0.50] - evidence: Sly 1 / Lilly reference; arbitration; sources:
 *   r3b_rest+arbitration
 * See: docs/engine/core-library.md#6-source-files, docs/engine/core-library.md#7-key-functions,
 *   docs/engine/physics-collision.md#2-contacts-and-solving
 * @end-sly2-annot */
struct S *InitDzgXp(struct S *p, int a, int b, int c, int d)
{
    MSG msg;
    InitDzg(p, a * 2);
    p->a = a;
    p->b = b;
    p->c = c;
    p->d = d;
    msg.nKind = 0x10;
    msg.cb = 0x60 * p->cdzMax;
    p->pv = func_001AE378(&msg);
    return p;
}
