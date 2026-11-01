typedef unsigned long ulong;
struct S { char pad[0x28]; ulong x28; };
/** @sly2-annot addr=0x259AE8
 * Purpose: return (x28>>60)&1 [game linkonce (inline/template)]
 * Info: proposed name FGetBit60At28 (T2); TU game_linkonce
 * Confidence: low (draft) [T2 0.20] - evidence: static analysis; sources: sdk_names
 * See: docs/reference/functions/core-library.md#game_linkonce-0x254f00-0x259de4
 * @end-sly2-annot */
unsigned long func_00259AE8(struct S *p)
{
    return (p->x28 >> 60) & 1;
}
