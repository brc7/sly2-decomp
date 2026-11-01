typedef struct { float x, y, z, w; } __attribute__((aligned(16))) V;
struct S { char pad[0xE8]; int c; float *ag; char pad2[4]; V *av; char pad3[0x190 - 0xF8]; int x190; };
void PostLoLoad(struct S *p);
int func_001BC2F8(struct S *p);
float UMaxShape(struct S *p);
void MeasureShape(struct S *p);
static __inline__ void SwapV(V *a, V *b) { V t = *a; *a = *b; *b = t; }
static __inline__ void SwapG(float *a, float *b) { float t = *a; *a = *b; *b = t; }
/** @sly2-annot addr=0x1BC570
 * Purpose: CID 14 vt15: PostLoLoad, classify polygon, reverse/measure when needed
 * Info: proposed name PostCid14Load (T2); class CID14 slot15; TU tu_1BC050
 * Notes: names_v2 B proposed AdjustJtNewXp__FP2JTP2XPi (0.952)
 * Confidence: low (draft) [T2 0.55] - evidence: static naming pass; sources: r5b_rest
 * See: docs/reference/functions/physics-collision.md#tu_1bc050-0x1bc050-0x1be398
 * @end-sly2-annot */
void func_001BC570(struct S *p)
{
    int r, h, i, j;
    float t;
    PostLoLoad(p);
    r = func_001BC2F8(p);
    p->x190 = r;
    if (r == 3) return;
    if (r != 4) return;
    h = p->c / 2;
    t = UMaxShape(p);
    for (i = 0; i < h; i++) {
        j = p->c - i - 1;
        SwapV(&p->av[i], &p->av[j]);
        SwapG(&p->ag[i], &p->ag[j]);
        p->ag[i] = t - p->ag[i];
        p->ag[j] = t - p->ag[j];
    }
    MeasureShape(p);
    p->x190 = 3;
}
