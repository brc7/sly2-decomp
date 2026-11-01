float GModPositive__Fff(float gDividend, float gDivisor);

float RadNormalize__Ff(float rad)
{
    if (rad < -3.14159265359f || 3.14159265359f < rad) {
        float modpos = GModPositive__Fff(rad + 3.14159265359f, 2 * 3.14159265359f);
        rad = modpos - 3.14159265359f;
    }
    return rad;
}
