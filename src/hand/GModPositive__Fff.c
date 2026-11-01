float fmodf(float x, float y);

float GModPositive__Fff(float gDividend, float gDivisor)
{
    float result = fmodf(gDividend, gDivisor);
    if (result < 0.0f) {
        result += gDivisor;
    }
    return result;
}
