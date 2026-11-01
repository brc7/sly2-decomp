typedef struct CRef { int m_tagk; float m_g; } CRef;
extern char D_003D4A60[];
struct X { char pad[0xA8]; float *pg; };
extern struct X *D_003E0F9C;
struct S { int pad[2]; void *p8; void *pC; void *p10; };
void func_002581C0(CRef *pref, void *p, void *q);
void func_00129EC8(CRef *pref);
static inline float GGet(void *p) { CRef ref; float g; func_002581C0(&ref, p, D_003D4A60); g = ref.m_g; func_00129EC8(&ref); return g; }
float GGetDifficulty(struct S *p)
{ float g;
  if (D_003E0F9C) g = *D_003E0F9C->pg;
  else if (p->p8 && p->pC && p->p10) { float g1 = GGet(p->p8); float g2 = GGet(p->pC); float g3 = GGet(p->p10); g = g1 + g2 + g3; g /= 3.0f; }
  else g = 0.0f; return g; }
