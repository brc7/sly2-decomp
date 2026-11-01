typedef struct VECTOR { float x, y, z; } VECTOR;
typedef struct RGBA { unsigned char r, g, b, a; } RGBA;

void ConvertVectorToRgba(VECTOR *pvec, int a, RGBA *prgba)
{
    prgba->r = (int)(pvec->x * 255.0f + 0.5f);
    prgba->g = (int)(pvec->y * 255.0f + 0.5f);
    prgba->b = (int)(pvec->z * 255.0f + 0.5f);
    prgba->a = a;
}
