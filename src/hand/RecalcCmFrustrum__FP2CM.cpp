#include "sly2/cm.h"
// sqrt.s as volatile inline asm (as in other matched cm/util code): it orders the stores like the original.
extern "C" float tanf(float);
static inline float sqrtf(float x){float r; __asm__ __volatile__("sqrt.s %0,%1":"=&f"(r):"f"(x)); return r;}
void BuildProj(void *p, float a, float b, float c, float d, float e, float f);
void SetCmLookAt(CM *p, float g, void *pv);
void RecalcCm(CM *p);
static inline float GMax(float a, float b) { return a > b ? a : b; }
void RecalcCmFrustrum(CM *pcm)
{
    float t = tanf(pcm->fov * 0.5f);
    float r = 1.0f / pcm->fov;
    float c = pcm->rAspect;
    pcm->gTanHalfY = t;
    pcm->gTanHalfX = t * c;
    pcm->rCullScale = GMax(pcm->rFovCullRef * r, 1.0f);
    pcm->sNearCorner = t * sqrtf(c * c + 1.0f) * pcm->sNearClip + 1.0f;
    BuildProj(&pcm->matProj, 512.0f / (pcm->gTanHalfX * 4096.0f), 224.0f / (pcm->gTanHalfY * 4096.0f), 0.0f, 0.0f, pcm->sNearClip, pcm->sFarClip);
    SetCmLookAt(pcm, 1.0f, pcm->agRecipFit);
    RecalcCm(pcm);
}
