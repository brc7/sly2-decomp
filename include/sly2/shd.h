/* include/sly2/shd.h -- the level's colour tables (CLUT) and bitmaps (BMP) and the GS TEX0 register they upload with
 * (engine/graphics/pipeline/shd.cpp loads, rebases and uploads them; font.cpp uploads its built-in ones). Kind 1 is
 * shared (rebased after load). Self-contained (no sly2/basic.h): its includers keep their own basic types. */
#ifndef SLY2_SHD_H
#define SLY2_SHD_H

/* GS TEX0 (or TEX2) register: as one 64-bit word or its fields */
union TEX0
{
    unsigned long word;
    struct
    {
        unsigned long TBP0 : 14; unsigned long TBW : 6; unsigned long PSM : 6; unsigned long TW : 4;
        unsigned long TH : 4; unsigned long TCC : 1; unsigned long TFX : 2; unsigned long CBP : 14;
        unsigned long CPSM : 4; unsigned long CSM : 1; unsigned long CSA : 5; unsigned long CLD : 3;
    };
};

struct CLUT; struct BMP;
extern "C" {
CLUT *CopyClutHeader(CLUT *pclut, void *pvHeader);  /* shd.cpp: copies the 0x10-byte header, colour-corrects, sets cqwPal */
BMP *CopyBitmapHeader(BMP *pbmp, void *pvHeader);   /* shd.cpp: copies the 0x18-byte header, sets cqw */
}

/* a colour table entry */
struct SHDRGBA { unsigned char r, g, b, a; };

/* CLUT: a colour table (0x20) */
struct CLUT
{
    int n0;
    unsigned char kind;     /* +0x04 1: shared */
    char pad5[3];
    unsigned short ccolor;  /* +0x08 entry count */
    unsigned short gsBlocks; /* +0x0A GS blocks it needs (IgsAllocGsb) */
    SHDRGBA *argba;         /* +0x0C the entries */
    int cqwPal;             /* +0x10 their size in quadwords */
    int unknown14;
    TEX0 tex2;              /* +0x18 its TEX2 word (BuildClutTex2) */

    /* built from a header at startup (tu_1147D8/tu_114AE0's built-in tables); no default constructor: every other
       CLUT is loaded in place (LoadClutFromBrx) */
    CLUT(void *pvHeader) { CopyClutHeader(this, pvHeader); }
};
typedef char assert_CLUT_size[sizeof(CLUT) == 0x20 ? 1 : -1];

/* BMP: a bitmap (0x28) */
struct BMP
{
    int n0;
    unsigned char kind;     /* +0x04 1: shared */
    char pad5[3];
    unsigned short cx;      /* +0x08 size */
    unsigned short cy;
    unsigned char psm;      /* +0x0C GS pixel format */
    unsigned char cbw;      /* +0x0D buffer width (64-pixel units) */
    unsigned short gsBlocks; /* +0x0E GS blocks it needs */
    unsigned int cb;        /* +0x10 pixel bytes */
    void *pv;               /* +0x14 pixels */
    int cqw;                /* +0x18 their size in quadwords */
    int unknown1C;
    TEX0 tex0;              /* +0x20 its TEX0 word (BuildBmpTex0) */

    /* built from a header at startup, as CLUT */
    BMP(void *pvHeader) { CopyBitmapHeader(this, pvHeader); }
};
typedef char assert_BMP_size[sizeof(BMP) == 0x28 ? 1 : -1];

#endif
