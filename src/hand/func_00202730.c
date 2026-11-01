struct G;
extern struct G D_003DD490;
struct S { char pad[0xBC]; int oid; };
const char *func_0013E368(struct G *p, int oid);
/** @sly2-annot addr=0x202730
 * Purpose: langstr of P+0xBC
 * Info: proposed name PchzReconSelected (T1); TU tu_201D78
 * Notes: names_v2 C proposed SetBrkOnBomb__FP3BRKi (0.638)
 * Confidence: mid (draft) [T1 0.50] - evidence: static naming pass; sources: r7c_hud
 * See: docs/reference/functions/hud-ui.md#hud-0x210eb0-0x21bb88,
 *   docs/reference/functions/player.md#tu_201d78-0x201d78-0x203948
 * @end-sly2-annot */
const char *func_00202730(struct S *p) { if (p->oid == -1) return 0; return func_0013E368(&D_003DD490, p->oid); }
