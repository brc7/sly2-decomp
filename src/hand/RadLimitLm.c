typedef struct LM { float gMin; float gMax; } LM;
float RadNormalize__Ff(float rad);
float GLimitAbs__Fff(float g, float gLimit);

float RadLimitLm(LM *plm, float rad)
{
    float radMid = (plm->gMax + plm->gMin) * 0.5f;
    float radHalf = (plm->gMax - plm->gMin) * 0.5f;
    return RadNormalize__Ff(radMid + GLimitAbs__Fff(RadNormalize__Ff(rad - radMid), radHalf));
}
