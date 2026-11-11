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

struct CLUT; struct BMP; struct CLUTHDR; struct BMPHDR;
extern "C" {
CLUT *CopyClutHeader(CLUT *pclut, CLUTHDR *pcluthdr); /* shd.cpp: copies the header, colour-corrects, sets cqwPal */
BMP *CopyBitmapHeader(BMP *pbmp, BMPHDR *pbmphdr);    /* shd.cpp: copies the header, sets cqw */
}

/* a colour table entry */
struct SHDRGBA { unsigned char r, g, b, a; };

/* CLUTHDR: the part of a colour table stored in the file or a built-in header (0x10) */
struct CLUTHDR
{
    int n0;
    unsigned char kind;     /* +0x04 1: shared */
    signed char clevel;     /* +0x05 > 0: loaded only in the levels of the id list that follows it in the file */
    char pad6[2];
    unsigned short ccolor;  /* +0x08 entry count */
    unsigned short gsBlocks; /* +0x0A GS blocks it needs (IgsAllocGsb) */
    SHDRGBA *argba;         /* +0x0C the entries */
};
typedef char assert_CLUTHDR_size[sizeof(CLUTHDR) == 0x10 ? 1 : -1];
/* CLUTFILE: CLUTHDR as the level file stores it (0x10 on every target). argba is a 32-bit offset: into the shared data
   block for a shared table (RelocateSharedColorTables rebases it), meaningless otherwise (LoadClutFromBrx sets it).
   With the EE layout (SLY2_EE_LAYOUT) the file record is read straight into CLUTHDR; elsewhere LoadClutFromBrx reads
   a CLUTFILE and widens it. */
struct CLUTFILE
{
    unsigned int n0;
    unsigned char kind;
    signed char clevel;
    unsigned char pad6[2];
    unsigned short ccolor;
    unsigned short gsBlocks;
    unsigned int ibArgba;   /* +0x0C */
};
typedef char assert_CLUTFILE_size[sizeof(CLUTFILE) == 0x10 ? 1 : -1];   /* every target */

/* CLUT: a colour table (0x20): its header and what is derived from it */
struct CLUT : CLUTHDR
{
    int cqwPal;             /* +0x10 their size in quadwords */
    int unknown14;
    TEX0 tex2;              /* +0x18 its TEX2 word (BuildClutTex2) */

    /* built from a header at startup (palette16/palette256's built-in tables); no default constructor: every other
       CLUT is loaded in place (LoadClutFromBrx) */
    CLUT(void *pvHeader) { CopyClutHeader(this, (CLUTHDR *)pvHeader); }
};
typedef char assert_CLUT_size[sizeof(CLUT) == 0x20 ? 1 : -1];

/* BMPHDR: the part of a bitmap stored in the file (LoadBmpFromBrx reads it) or a built-in header (0x18) */
struct BMPHDR
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
};
typedef char assert_BMPHDR_size[sizeof(BMPHDR) == 0x18 ? 1 : -1];
/* BMPFILE: BMPHDR as the level file stores it (0x18 on every target); pv is a 32-bit offset as CLUTFILE's argba. */
struct BMPFILE
{
    unsigned int n0;
    unsigned char kind;
    signed char clevel;
    unsigned char pad6[2];
    unsigned short cx;
    unsigned short cy;
    unsigned char psm;
    unsigned char cbw;
    unsigned short gsBlocks;
    unsigned int cb;
    unsigned int ibPv;      /* +0x14 */
};
typedef char assert_BMPFILE_size[sizeof(BMPFILE) == 0x18 ? 1 : -1];   /* every target */

/* BMP: a bitmap (0x28): its header and what is derived from it */
struct BMP : BMPHDR
{
    int cqw;                /* +0x18 their size in quadwords */
    int unknown1C;
    TEX0 tex0;              /* +0x20 its TEX0 word (BuildBmpTex0) */

    /* built from a header at startup, as CLUT */
    BMP(void *pvHeader) { CopyBitmapHeader(this, (BMPHDR *)pvHeader); }
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

/* COPYREC: a shader-animation copy record (0x14), one of the 0x200 in shd.cpp's table D_003DA798 (D_003DA790 in use):
   ApplyCopyRecord copies cb bytes from pvSrc (in the animation) to pvDst (in an upload chain; RelocateShaderUploadRecords
   rebases it when the chain moves); the records of an animation are linked through pcopyrecNext. Name ours. */
struct COPYREC
{
    unsigned int cb;        /* +0x00 */
    void *pvSrc;            /* +0x04 */
    void *pvDst;            /* +0x08 */
    SAA *psaa;              /* +0x0C owning animation */
    COPYREC *pcopyrecNext;  /* +0x10 its list link */
};
typedef char assert_COPYREC_size[sizeof(COPYREC) == 0x14 ? 1 : -1];

/* SHDBLIP: the compact shader record blips use (0x1C; BuildShaderTex0List builds it for shaders flagged grfshd 0x40,
   linked on SW::slShd; FindShdOrDefault finds it). Name ours. */
struct SHDBLIP
{
    int oid;                /* +0x00 */
    int unknown4;           /* +0x04 SHD::unknown06 */
    int grfshd;             /* +0x08 bit 0: the second blend packet */
    union                   /* +0x0C the colour; PropagateBlipgShader indexes its channels (ab[j]: an array member
                               keeps the address of the colour in a register, as the original code does) */
    {
        SHDRGBA rgba;
        unsigned char ab[4];
    };
    int cframe;             /* +0x10 */
    unsigned long *atex0;   /* +0x14 one TEX0 word per frame */
    SHDBLIP *pshdblipNext;  /* +0x18 SW::slShd link */
};
typedef char assert_SHDBLIP_size[sizeof(SHDBLIP) == 0x1C ? 1 : -1];

#endif
