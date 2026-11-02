struct VTE { short d; short pad; void *pfn; };
struct S {
    void *x0; int x4, x8; struct VTE *vt; char pad[0x18];
    long b0 : 52; long st : 4; long b1 : 8;
    float x30; char pad2[0xC]; int x40;
};
extern float D_002A6A40;
void RunLoDeferredCallbacks(struct S *s);
int PrefFindBinding__6CFrameUii(void *p, int a, int b);
void SetLoBusyk(struct S *s, int n);
/** @sly2-annot addr=0x1A5C48
 * Purpose: LO vt15: run deferred callbacks, vt3 message 0; if Splice frame has binding 1 -> busyk 2->0, grf
 *   x40|=2 (wants Splice update) else tSplice(x30)=D_002A6A40
 * Info: proposed name PostLoLoad (T1); class LO slot15; TU lo
 * Confidence: high [T1 0.85] - evidence: static analysis; sources: r4_17F090+verdicts_tu_r4r6
 * See: docs/engine/object-model.md#2-the-lo-virtual-interface, docs/engine/object-model.md#9-key-functions,
 *   docs/reference/functions/animation.md#aseg-0x12f248-0x1308a8
 * @end-sly2-annot */
void PostLoLoad(struct S *s)
{
    struct VTE *e;
    RunLoDeferredCallbacks(s);
    e = &s->vt[3];
    ((void (*)(void *, int, int, int, int, int))e->pfn)((char *)s + e->d, 0, 1, 0, 0, 0);
    if (s->x0 && PrefFindBinding__6CFrameUii(s->x0, 1, 1)) {
        int v;
        if (s->st == 2)
            SetLoBusyk(s, 0);
        v = s->x40;
        if ((v ^ 1) & 1) *(int *)v |= 2;
        else s->x40 = v | 2;
    } else {
        s->x30 = D_002A6A40;
    }
}
