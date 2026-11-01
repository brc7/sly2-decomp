// sqrt.s as volatile inline asm (as in other matched cm/util code): it orders the stores like the original.
struct CM {
    char pad0[0x18]; float x18; float x1C; float x20; char pad1[0x3C - 0x24];
    float x3C; float x40; float x44; float x48; float x4C; float x50;
    char pad2[0x100 - 0x54]; char m100[0x150]; char m250[1];
};
extern "C" float tanf(float);
static inline float sqrtf(float x){float r; __asm__ __volatile__("sqrt.s %0,%1":"=&f"(r):"f"(x)); return r;}
void BuildProj(void *p, float a, float b, float c, float d, float e, float f);
void SetCmLookAt(CM *p, float g, void *pv);
void RecalcCm(CM *p);
static inline float GMax(float a, float b) { return a > b ? a : b; }
/** @sly2-annot addr=0x14DB70
 * Purpose: not written up yet; only the symbol name (pre-existing / Sly 1-derived) describes it.
 * Confidence: mid (draft) - evidence: pre-existing symbol name only; behaviour not reviewed
 * See: docs/engine/camera.md#2-the-policy-stack, docs/reference/functions/camera.md#cm-0x14d3d8-0x14fac8
 * @end-sly2-annot */
void RecalcCmFrustrum(CM *pcm)
{
    float t = tanf(pcm->x20 * 0.5f);
    float r = 1.0f / pcm->x20;
    float c = pcm->x3C;
    pcm->x44 = t;
    pcm->x40 = t * c;
    pcm->x1C = GMax(pcm->x18 * r, 1.0f);
    pcm->x48 = t * sqrtf(c * c + 1.0f) * pcm->x4C + 1.0f;
    BuildProj(pcm->m100, 512.0f / (pcm->x40 * 4096.0f), 224.0f / (pcm->x44 * 4096.0f), 0.0f, 0.0f, pcm->x4C, pcm->x50);
    SetCmLookAt(pcm, 1.0f, pcm->m250);
    RecalcCm(pcm);
}
