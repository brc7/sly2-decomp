struct S { int a; char pad[0x8C]; int b; float g; float h; };
/** @sly2-annot addr=0x20B840
 * Purpose: GAIT init: stepplayer, sfxidStep, gVolWalk 0.6, gVolRun 1.0
 * Info: proposed name InitGait (T1); TU tu_20B840
 * Confidence: mid (draft) [T1 0.70] - evidence: static analysis; sources: r7c_hud
 * See: docs/engine/animation.md#6-source-files, docs/engine/guards-ai.md#8-source-files,
 *   docs/engine/player.md#4-stepplayer-tu-0x13ff70-0x144638-file-name-unknown-lilly-stepplayer_rawc
 * @end-sly2-annot */
void func_0020B840(struct S *p, int a, int b) {
  p->a = a;
  p->b = b;
  p->h = 1.0f;
  p->g = 0.6f;
}
