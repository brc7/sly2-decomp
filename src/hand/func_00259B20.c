struct B { char pad[0x10]; int x10; };
struct A { char pad[0xDC]; struct B *xDC; };
struct O { char pad[0xE8]; struct A *xE8; };
/** @sly2-annot addr=0x259B20
 * Purpose: p->xE8 ? p->xE8->xDC ? ->x10 : 0 [game linkonce (inline/template)]
 * Info: proposed name GetE8DC10 (T2); TU game_linkonce
 * Confidence: low (draft) [T2 0.15] - evidence: static analysis; sources: sdk_names
 * See: docs/reference/functions/core-library.md#game_linkonce-0x254f00-0x259de4
 * @end-sly2-annot */
int func_00259B20(struct O *p)
{
    struct A *a = p->xE8;
    struct B *b = a ? a->xDC : 0;
    return b ? b->x10 : 0;
}
