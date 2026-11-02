struct S { int a0; int a4; int pad8; int c; char pad[0x80]; int x90; int pad94[2]; float x9c; };
/** @sly2-annot addr=0x1A0B18
 * Purpose: Init the STEPPLAYER+0xDF0 hurt/zap context: owner, state -1, x90=-1, x9C=2.0, count 0x28
 * Info: proposed name InitJtHurt (T1); TU hurt
 * Notes: runtime3: +0x9C=2.0, hpMax 40/60
 * Confidence: mid (draft) [T1 0.60] - evidence: runtime (save-state RAM); sources: r4b_rest+runtime
 * See: docs/engine/animation.md#6-source-files,
 *   docs/engine/player.md#4-stepplayer-tu-0x13ff70-0x144638-file-name-unknown-lilly-stepplayer_rawc,
 *   docs/engine/player.md#10-source-files
 * @end-sly2-annot */
void InitJtHurt(struct S *p, int a)
{
    p->a0 = a; //P
    p->a4 = -1; //P
    p->x90 = -1; //P
    p->x9c = 2.0f; //P
    p->c = 0x28; //P
}
