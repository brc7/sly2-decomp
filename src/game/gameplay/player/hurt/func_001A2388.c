void func_001F80D0(void *p, float g);
void OnStepplayerHeroDied(void *p);
/** @sly2-annot addr=0x1A2388
 * Purpose: JT vt93: SetSoEyesClosed(1.0) then base 0x140EB0
 * Info: proposed name OnJtDeath (T2); class JT slot93; TU hurt
 * Confidence: low (draft) [T2 0.25] - evidence: static analysis; sources: r4b_rest
 * See: docs/reference/functions/player.md#hurt-0x1a0b18-0x1a2bd0
 * @end-sly2-annot */
void func_001A2388(void *p) { func_001F80D0(p, 1.0f); OnStepplayerHeroDied(p); }
