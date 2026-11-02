struct B { int *p; int pad[3]; int f; };
extern int D_002E0CB4;
int FFindSlEntry(void *a, struct B *b);
void func_0011A070(void *a, struct B *b);
/** @sly2-annot addr=0x14BEA8
 * Purpose: If not already in the list (0x11A1B8 ? FFindDlEntry), append it (AppendDlEntry 0x11A070); ijbcCur
 *   = (current buttons D_002E0CB4 == ajbc[0])
 * Info: proposed name AddCode (T1); TU cheat
 * Notes: names_v2 C proposed PvAllocUnsafe__10CSplotheap (0.438)
 * Confidence: high [T1 0.85] - evidence: semantic review; static naming pass; sources: semantic_tu_docs/cheat
 * See: docs/engine/core-library.md#6-source-files, docs/engine/main-loop-startup.md#7-source-files,
 *   docs/engine/main-loop-startup.md#8-key-functions
 * @end-sly2-annot */
int func_0014BEA8(void *a, struct B *b)
{
    int r = FFindSlEntry(a, b);
    if (r) return r;
    func_0011A070(a, b);
    if (*b->p == D_002E0CB4) b->f = 1;
    else b->f = 0;
}
