typedef struct { float x, y, z; } V3;
struct S { int x0; int x4; char pad8[0xC]; float x14; float x18; float x1C; float x20; float x24; float x28; int pad2C;
  V3 v30; char pad3C[0x84]; V3 vC0; int padCC; V3 vD0; };
static inline void SetV(V3 *pv, float x, float y, float z) { pv->x = x; pv->y = y; pv->z = z; }
/** @sly2-annot addr=0x21F2E0
 * Purpose: JTTHROW defaults: owner=a, state -1, 500/1500 ranges, 1.5, 1000, launch vectors
 *   (0,0,25),(1000,0,0),(42,-73,-50)
 * Info: proposed name InitThrow (T1); TU throw
 * Notes: verifier PLAUSIBLE: InitJtthrow -> InitThrow
 * Confidence: mid (draft) [T1 0.60] - evidence: static analysis; sources: r8_21BB88+verdicts_tu_r7r8
 * See: docs/engine/core-library.md#6-source-files,
 *   docs/engine/player.md#4-stepplayer-tu-0x13ff70-0x144638-file-name-unknown-lilly-stepplayer_rawc,
 *   docs/engine/player.md#10-source-files
 * @end-sly2-annot */
void InitThrow(struct S *p, int a)
{
    p->x0 = a;
    p->x4 = -1;
    p->x1C = 500.0f;
    p->x20 = 1500.0f;
    p->x24 = 1.5f;
    p->x14 = 1.0f;
    p->x18 = 1.0f;
    p->x28 = 1000.0f;
    SetV(&p->v30, 0.0f, 0.0f, 25.0f);
    SetV(&p->vC0, 1000.0f, 0.0f, 0.0f);
    SetV(&p->vD0, 42.0f, -73.0f, -50.0f);
}
