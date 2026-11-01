struct RGBA {
    unsigned char r, g, b, a;
    RGBA() {}
    RGBA(int r_, int g_, int b_, int a_) { r = r_; g = g_; b = b_; a = a_; }
};
extern "C" RGBA RgbaFromSpeakerCid(int k)
{
    RGBA clr;
    switch (k) {
    case 0x35: clr = RGBA(0x33, 0x66, 0x99, 0x7F); break;
    case 0x37: clr = RGBA(0x33, 0x99, 0x33, 0x7F); break;
    case 0x36: clr = RGBA(0x99, 0x33, 0x66, 0x7F); break;
    case 0x3F: clr = RGBA(0x99, 0x33, 0x33, 0x7F); break;
    case 0x3C: clr = RGBA(0x99, 0x33, 0x99, 0x7F); break;
    default: clr = RGBA(0x66, 0x66, 0x66, 0x7F); break;
    }
    return clr;
}
