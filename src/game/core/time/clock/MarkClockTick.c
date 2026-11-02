typedef unsigned long ulong;
struct CLOCK {
    float t; float dt; float rDt; float rDtReal; float dtReal; float tReal; char pad[0x24 - 0x18]; float r24;
    char pad2[0x3C - 0x28]; float t3C; float dt40; ulong tick;
};
extern char D_002DEB30[];
ulong TickNow(struct CLOCK *p);
void UpdateGameTimers(void *p);
static __inline__ float GClamp(float g, float gMin, float gMax)
{
    float r;
    if (g < gMin) r = gMin;
    else if (gMax < g) r = gMax;
    else r = g;
    return r;
}
/** @sly2-annot addr=0x14D0B8
 * Purpose: dt = clamp(?tick?1/294.912MHz, 1/60, 1/30); dtReal = dt?rDtReal, tReal += dtReal; dt40 =
 *   dt?slot[3]; if not paused, dt = dt40?rDt, t += dt, and pumps 0x161478(D_002DEB30); always t3C += dt40
 * Info: proposed name MarkClockTick (T1); TU clock
 * Notes: names_v2 v1 proposed ClsgClipCylinderToSphere__FP6VECTORfT0T0fiP3LSG (0.037)
 * Confidence: high [T1 0.85] - evidence: semantic review; static naming pass; sources: semantic_tu_docs/clock
 * See: docs/engine/main-loop-startup.md#2-clock-clockcpp-0x14cfe0-0x14d3d8,
 *   docs/engine/main-loop-startup.md#8-key-functions, docs/engine/overview.md#2-the-frame-loop-exact-order
 * @end-sly2-annot */
void MarkClockTick(struct CLOCK *p, int fPaused)
{
    ulong tick = TickNow(p);
    float dt = GClamp((float)(tick - p->tick) * 3.3908420e-9f, 1.0f / 60.0f, 1.0f / 30.0f);
    p->dtReal = dt * p->rDtReal;
    p->tReal += p->dtReal;
    {
    float dt2 = dt * p->r24;
    p->dt40 = dt2;
    if (!fPaused) {
        p->dt = dt2 * p->rDt;
        p->t += p->dt;
        UpdateGameTimers(D_002DEB30);
    }
    }
    p->tick = tick;
    p->t3C += p->dt40;
}
