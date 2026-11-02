extern int D_002E1E00;
struct G;
extern struct G D_003DD490;
int FSwHandsOff(struct G *p);
/** @sly2-annot addr=0x1C3E90
 * Purpose: PO virtual slot 76: true if invulnerability cheat bit (D_002E1E00&1) or world hands-off
 *   (FSwHandsOff(&g_sw))
 * Info: proposed name FInvulnerablePo (T1); class PO slot76; TU po
 * Confidence: high [T1 0.90] - evidence: static analysis; sources: r5_1AE130
 * See: docs/engine/player.md#2-po-and-poctl-pocpp-0x1c3d70,
 *   docs/reference/functions/player.md#po-0x1c3d70-0x1c4e50
 * @end-sly2-annot */
int FInvulnerablePo(void) { /*B*/if ((D_002E1E00 & 1) || FSwHandsOff(&D_003DD490)) return 1; return 0;/*E*/ }
