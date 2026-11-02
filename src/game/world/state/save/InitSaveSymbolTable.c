struct MSG { int cb; int nKind; };
struct S { char pad[0xD4]; int c; void *pv; };
void *func_001AE1D0(struct MSG *pmsg);
static inline void *PvAllocMsg(struct MSG *pmsg, int cb, int nKind) { pmsg->cb = cb; pmsg->nKind = nKind; return func_001AE1D0(pmsg); }
/** @sly2-annot addr=0x1D8600
 * Purpose: Allocate c 16-byte hash entries for the symbol table at schema+0xD4
 * Info: proposed name InitSaveSymbolTable (T1); TU save
 * Confidence: mid (draft) [T1 0.60] - evidence: static analysis; sources: r6_1D5150
 * See: docs/reference/functions/save-memcard.md#save-0x1d7b88-0x1d93d8
 * @end-sly2-annot */
void InitSaveSymbolTable(struct S *p, int c) {
    struct MSG msg;
    p->c = c;
    p->pv = PvAllocMsg(&msg, c << 4, 0x10);
}
