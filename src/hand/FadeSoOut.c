void SetSoFadeRate(void *, float);
void SetSoAlpha(void *, float);
void FadeSoOut(void *p, float f)
{
    if (f <= 0.0f) {
        SetSoFadeRate(p, 0.0f);
        SetSoAlpha(p, 0.0f);
    } else {
        SetSoFadeRate(p, -1.0f / f);
    }
}
