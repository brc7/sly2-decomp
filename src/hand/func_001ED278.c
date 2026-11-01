struct RGBA { unsigned char r, g, b, a; };
struct VECTOR { float x, y, z, w; } __attribute__((aligned(16)));
extern int D_003E12C8;
void func_0011DCD8(struct RGBA *prgba, struct VECTOR *pv);
void func_001ED2D8(struct VECTOR *pv);
void ConvertVectorToRgba(struct VECTOR *pv, int a, struct RGBA *prgba);
void func_001ED278(struct RGBA *prgba)
{
    struct VECTOR v;
    struct RGBA rgba;
    if (D_003E12C8) {
        rgba = *prgba;
        func_0011DCD8(&rgba, &v);
        func_001ED2D8(&v);
        ConvertVectorToRgba(&v, prgba->a, prgba);
    }
}
