struct S { int x0; int x4; char pad[0x2C]; int x34; int x38; int x3C; };
struct K;
extern "C" void func_00258248(K *k);
extern "C" int func_0014B670(K *k);
struct K { int a; int b; int c; char pad[0x104];
  K() { c = 2; func_00258248(this); }
  bool FSet(S *p) { if (!p) return false; b = p->x4 ^ p->x3C; a = (p->x0 ^ p->x34) + 0xCA0; c = 2; return b != 0; }
};
/** @sly2-annot addr=0x1AB750
 * Purpose: Build a disc FLOC (lba=(w0^w13)+0xCA0,size=w1^w15) for a file entry and open it via 0x14B670
 * Info: proposed name FOpenSaveIconFile (EXISTING); TU memcard
 * Notes: agent/verifier proposes FOpenSaveIconFile for this existing name (see existing_name_changes.csv) |
 *   arbitration: draft replacement (not re-derived) | RENAME proposed -> FOpenSaveIconFile
 *   (existing_name_changes.csv)
 * Confidence: high [EXISTING 0.35] - evidence: arbitration; pre-existing symbol in symbol_addrs.txt; sources:
 *   r4_17F090+arbitration
 * See: docs/engine/save-memcard.md#5-key-functions,
 *   docs/reference/functions/save-memcard.md#memcard-0x1aa978-0x1ae150
 * @end-sly2-annot */
extern "C" int FOpenSaveIconFile(S *p)
{ K k; bool f = k.FSet(p); if (f) return func_0014B670(&k); else return 0; }
