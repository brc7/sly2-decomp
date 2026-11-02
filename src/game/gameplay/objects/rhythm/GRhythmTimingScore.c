extern char D_003DD040[];
extern float D_002BBB60[2];
struct Q { int pad[3]; float gC; };
struct S { char pad[0x58]; float g58; float g5C; };
float func_001FE3F8(void *p);
static inline float GLerp(float a, float b, float t) { return (1.0f - t) * a + t * b; }
/** @sly2-annot addr=0x1535A0
 * Purpose: Score = lerp(D_002BBB60 range, 1 - |t - tCue| / window), window = early(+0x58) or late(+0x5C)
 * Info: proposed name GRhythmTimingScore (T1); TU rhythm
 * Confidence: high [T1 0.85] - evidence: semantic review; sources: r2_131F70+verdicts_tu_r1r2
 * See: docs/engine/gameplay-objects.md#key-functions,
 *   docs/engine/minigames.md#7-dance-dancer-0x44--rhythm-manager-cid121-0x79-rhythm-tu-0x1520c8-0x1539a8-ccode,
 *   docs/reference/functions/gameplay-objects.md#rhythm-0x1520c8-0x1539a8
 * @end-sly2-annot */
float GRhythmTimingScore(struct S *p, struct Q *q)
{ float g = func_001FE3F8(D_003DD040); float d;
  if (g < q->gC) { d = (q->gC - g) / p->g58; } else { d = (g - q->gC) / p->g5C; }
  return GLerp(D_002BBB60[0], D_002BBB60[1], 1.0f - d); }
