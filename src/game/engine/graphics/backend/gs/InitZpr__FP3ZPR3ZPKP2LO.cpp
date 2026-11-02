enum ZPK { ZPK_Nil = -1 };
struct LO;
struct ZPR { ZPK zpk; ZPK zpk2; LO *ploSource; char pad[0x630 - 12]; };
extern "C" void func_002333B4(void *p, int c, int n);
/** @sly2-annot addr=0x1703F8
 * Purpose: not written up yet; only the symbol name (pre-existing / Sly 1-derived) describes it.
 * Info: proposed name InitGsb__FP3GSBii (EXISTING); TU gs
 * Notes: RENAME proposed -> InitGsb__FP3GSBii (existing_name_changes.csv)
 * Confidence: name high [EXISTING 0.85]; purpose not reviewed - evidence: arbitration; pre-existing symbol in
 *   symbol_addrs.txt
 * See: docs/engine/rendering.md#9-textures-cluts-and-gs-memory-shdcpp-0x1ea568-gscpp-0x16f2d0-cstatic,
 *   docs/engine/rendering.md#13-key-functions, docs/reference/functions/rendering.md#gs-0x16f2d0-0x1709a8
 * @end-sly2-annot */
void InitZpr(ZPR *pzpr, ZPK zpk, LO *ploSource) { func_002333B4(pzpr, 0, sizeof(ZPR)); pzpr->zpk = pzpr->zpk2 = zpk; pzpr->ploSource = ploSource; }
