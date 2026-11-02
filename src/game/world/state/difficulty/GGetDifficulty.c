typedef struct CRef { int m_tagk; float m_g; } CRef;
extern char D_003D4A60[];
struct X { char pad[0xA8]; float *pg; };
extern struct X *D_003E0F9C;
struct S { int pad[2]; void *p8; void *pC; void *p10; };
void RefGetGlobalSaveVar(CRef *pref, void *p, void *q);
void Decref__4CRef(CRef *pref);
static inline float GGet(void *p) { CRef ref; float g; RefGetGlobalSaveVar(&ref, p, D_003D4A60); g = ref.m_g; Decref__4CRef(&ref); return g; }
/** @sly2-annot addr=0x154C90
 * Purpose: Current difficulty: override value if set, else average of the last three samples
 * Info: proposed name GGetDifficulty (T1); TU difficulty
 * Confidence: mid (draft) [T1 0.55] - evidence: static analysis; sources: r3_1539A8
 * See: docs/reference/functions/game-progression.md#difficulty-0x154a60-0x1553f0,
 *   docs/reference/functions/splice-scripting.md#bif-0x11f440-0x1258f0
 * @end-sly2-annot */
float GGetDifficulty(struct S *p)
{ float g;
  if (D_003E0F9C) g = *D_003E0F9C->pg;
  else if (p->p8 && p->pC && p->p10) { float g1 = GGet(p->p8); float g2 = GGet(p->pC); float g3 = GGet(p->p10); g = g1 + g2 + g3; g /= 3.0f; }
  else g = 0.0f; return g; }
