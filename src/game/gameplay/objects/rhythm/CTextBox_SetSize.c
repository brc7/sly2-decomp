typedef struct { float a, b, x; float y; } P;
/** @sly2-annot addr=0x258448
 * Purpose: p->dx=dx; p->dy=dy [game linkonce (inline/template)]
 * Info: proposed name CTextBox_SetSize (T1); TU game_linkonce
 * Confidence: mid (draft) [T1 0.55] - evidence: static analysis; sources: sdk_names
 * See: docs/reference/functions/core-library.md#game_linkonce-0x254f00-0x259de4
 * @end-sly2-annot */
void CTextBox_SetSize(P *p, float x, float y) { p->x = x; p->y = y; }
