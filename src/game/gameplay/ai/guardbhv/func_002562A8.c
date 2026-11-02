typedef struct { int a; int b; } P;
/** @sly2-annot addr=0x2562A8
 * Purpose: Inline default constructor of a two-pointer record: p->a=0; p->b=0
 * Info: proposed name ClearPathlinkEndpoint (T2); TU game_linkonce
 * Notes: v3: superseded ClearTwoWords | top50_round (0.30): Body only. In PlanBhvgoto 0x17DA28 the record is
 *   then filled from p->x60 and passed to PosPathlinkEndpoint 0x1C1EC0 ({pshape,plo,..,pos}); PbhvNew
 *   0x1EF138 constructs four at +0x30..+0x80. Role of the record HYPOTHESIS |
 * Confidence: low (draft) [T2 0.30] - evidence: semantic review; sources: sdk_names+top50_round
 * See: docs/reference/functions/core-library.md#game_linkonce-0x254f00-0x259de4
 * @end-sly2-annot */
void func_002562A8(P *p) { p->a = 0; p->b = 0; }
