struct VTE { short d; short pad; void (*pf)(); };
struct O { int a[3]; struct VTE *vt; };
struct Q { char pad[0x80]; float a, b; void *pa, *pb; char pad2[8]; short k; short l; };
extern float D_002A98D0;
extern char D_002C44E0[];
/** @sly2-annot addr=0x22C840
 * Purpose: for active roles marks the owning SO awake
 * Info: proposed name WakeXfSlot (T2); TU xf
 * Confidence: low (draft) [T2 0.30] - evidence: static analysis; sources: r8_21BB88
 * See: docs/reference/functions/animation.md#act-0x12b3b0-0x12e600,
 *   docs/reference/functions/animation.md#tail-0x21c820-0x21d278,
 *   docs/reference/functions/animation.md#xf-0x22c818-0x231100
 * @end-sly2-annot */
void func_0022C840(struct O *p, struct Q *q)
{
    switch (q->k) {
    case 0: case 1: case 2: case 3: case 13: case 14: case 15: case 16: case 20: case 21:
        break;
    default:
        q->b = D_002A98D0;
        q->a = D_002A98D0;
        q->pb = D_002C44E0;
        q->pa = D_002C44E0;
        break;
    }
    q->l = 0;
    p->vt[47].pf((char *)p + p->vt[47].d, 4);
}
