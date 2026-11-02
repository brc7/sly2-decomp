typedef int qword __attribute__((mode(TI)));
typedef unsigned long ulong;
struct VTE { short d; short pad; void (*pfn)(void *, int); };
struct O { char pad[0x28]; ulong fl; };
struct P { char pad[0xC]; struct VTE *pvt; float f10; char pad1[0x1C - 0x14]; struct O *po; qword v20; char x30[0x14]; float f44; };
void StopSnd(struct P *, float);
qword ConvertAloPos(struct O *, int, qword);
void CalculateSndVolPan(struct P *, qword, void *, float *, int *);
void SetSndFlags(struct P *, int, int);
void SetSndPan(struct P *, int);
/** @sly2-annot addr=0x1FB528
 * Purpose: vt16: from source ALO/pos compute distance volume and pan angle (CalculateSndVolPan); vt15 on
 *   audible<->silent transitions
 * Info: proposed name UpdateSndPositional (T1); class SND; TU sound
 * Confidence: mid (draft) [T1 0.55] - evidence: static analysis; sources: r7_1F8DD8
 * See: docs/reference/functions/sound.md#sound-0x1faf70-0x1ff3b8
 * @end-sly2-annot */
void UpdateSndPositional(struct P *p)
{
    float f;
    int n;
    float r;
    if (p->po && !((p->po->fl >> 60) & 1)) {
        StopSnd(p, 0.0f);
        return;
    }
    CalculateSndVolPan(p, ConvertAloPos(p->po, 0, p->v20), p->x30, &f, &n);
    r = p->f10 * f;
    if ((int)(r * 1024.0f) != (int)(p->f44 * 1024.0f)) {
        if (p->f44 == 0.0f) p->pvt[15].pfn((char *)p + p->pvt[15].d, 0);
        else if (r == 0.0f) p->pvt[15].pfn((char *)p + p->pvt[15].d, 1);
        p->f44 = r;
        SetSndFlags(p, 0x10, 0x10);
    }
    SetSndPan(p, n);
}
