typedef struct VECTOR { float x, y, z, w; } __attribute__((aligned(16))) VECTOR;
struct E { float x; int pad[3]; VECTOR v; int n20; int pad2[3]; };
struct S { char pad[0x60]; int c; int pad64[3]; struct E a[6]; int n190; int n194; int pad198[2]; VECTOR v1A0; };
void func_001AE5A0(void *dst, void *src, int cb);
void SetRhythmMgrState(struct S *p, void *q);
static inline void SetVector(VECTOR *pv, float x, float y, float z)
{
    pv->x = x;
    pv->y = y;
    pv->z = z;
}
/** @sly2-annot addr=0x1531A8
 * Purpose: Copy c prompt records (0x30 each, max 6) and lay them out on a 440-px timeline from x=100 at
 *   y=468.8; then SetRhythmMgrState(q)
 * Info: proposed name SetRhythmMgrPrompts (T2); TU rhythm
 * Confidence: low (draft) [T2 0.40] - evidence: semantic review; sources: r2_131F70
 * See: docs/engine/minigames.md#7-dance-dancer-0x44--rhythm-manager-cid121-0x79-rhythm-tu-0x1520c8-0x1539a8-ccode,
 *   docs/reference/functions/gameplay-objects.md#rhythm-0x1520c8-0x1539a8
 * @end-sly2-annot */
void func_001531A8(struct S *p, void *q, int c, void *pv)
{
    p->c = c;
    if (c) {
        float x0, r;
        int i;
        func_001AE5A0(p->a, pv, c * sizeof(struct E));
        x0 = p->a[0].x;
        r = 440.0f / (p->a[p->c - 1].x - x0);
        SetVector(&p->v1A0, 100.0f, 468.80002f, 0.0f);
        for (i = 0; i < p->c; i++) {
            struct E *pe = &p->a[i];
            pe->v = p->v1A0;
            pe->n20 = 0;
            pe->v.x += r * (pe->x - x0);
        }
        SetVector(&p->v1A0, 10.0f, 468.80002f, 0.0f);
        p->n190 = 0;
        p->n194 = 0;
    }
    SetRhythmMgrState(p, q);
}
