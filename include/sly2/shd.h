/* include/sly2/shd.h -- the level's colour tables (CLUT), bitmaps (BMP), textures (TEX) and shaders (SHD), and the GS
 * TEX0 register they upload with (engine/graphics/pipeline/shd.cpp loads, rebases and uploads them; font.cpp uploads
 * its built-in ones). Kind 1 is shared (rebased after load). Self-contained (no sly2/basic.h): its includers keep their own basic types. */
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
    signed char clevel;     /* +0x05 > 0: loaded only in the levels of the id list that follows it in the file */
    char pad6[2];
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
    signed char clevel;     /* +0x05 as CLUT's */
    char pad6[2];
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

struct SHD; struct SAA; union QW;

/* TEX: one texture of a shader (0x14; LoadTexFromBrx reads its first 8 bytes from the level): its frames' bitmaps
   and colour tables (Tex0FromTexIframeCtk combines frame i's BMP TEX0 with pass j's CLUT TEX2) */
struct TEX
{
    unsigned short oid;     /* +0x00 */
    short unknown02;
    unsigned short grftex;  /* +0x04 bit 0 / 1: clamp in S / T (PackTexGifs) */
    unsigned char cbmp;     /* +0x06 bitmaps, one per frame (the frame count) */
    unsigned char cclut;    /* +0x07 colour tables: frames * passes (index frame * passes + pass) */
    SHD *pshd;              /* +0x08 owning shader */
    BMP **apbmp;            /* +0x0C */
    CLUT **apclut;          /* +0x10 0 = none */
};
typedef char assert_TEX_size[sizeof(TEX) == 0x14 ? 1 : -1];

/* SHDP: one upload record of a shader (cqw quadwords at pv; the count of records depends on the shader kind) */
struct SHDP { int cqw; QW *aqw; };

/* SHD: a shader (0x2C), the level's D_003DCFEC array (LoadShdFromBrx) */
struct SHD
{
    int oid;                /* +0x00 */
    unsigned char shdk;     /* +0x04 kind: 1 two passes, 3 shadow, 7 three passes (two with grfshd 0x20) */
    unsigned char grfshd;   /* +0x05 0x02 alpha, 0x20, 0x40 blips use it (BuildShaderTex0List) */
    unsigned char unknown06;
    unsigned char unknown07;
    SHDRGBA rgba;           /* +0x08 colour-corrected at load */
    SHDRGBA rgbaVolume;     /* +0x0C */
    SHDRGBA rgba2;          /* +0x10 */
    unsigned short oidAltSat; /* +0x14 */
    unsigned char rp;       /* +0x16 render pass */
    unsigned char ctex;     /* +0x17 texture count */
    TEX *atex;              /* +0x18 */
    int cshdp;              /* +0x1C */
    SHDP *ashdp;            /* +0x20 */
    int cframe;             /* +0x24 the first texture's frame count */
    SAA *psaa;              /* +0x28 shader animation */
};
typedef char assert_SHD_size[sizeof(SHD) == 0x2C ? 1 : -1];

/* SHDBLIP: the compact shader record blips use (0x1C; BuildShaderTex0List builds it for shaders flagged grfshd 0x40,
   linked on SW::slShd; FindShdOrDefault finds it). Name ours. */
struct SHDBLIP
{
    int oid;                /* +0x00 */
    int unknown4;           /* +0x04 SHD::unknown06 */
    int grfshd;             /* +0x08 bit 0: the second blend packet */
    SHDRGBA rgba;           /* +0x0C */
    int cframe;             /* +0x10 */
    unsigned long *atex0;   /* +0x14 one TEX0 word per frame */
    SHDBLIP *pshdblipNext;  /* +0x18 SW::slShd link */
};
typedef char assert_SHDBLIP_size[sizeof(SHDBLIP) == 0x1C ? 1 : -1];

#endif
