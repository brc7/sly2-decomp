/* Binary input stream. Sly 2 rewrote this class from Sly 1: it is now an abstract base
 * (vtable _vt$18CBinaryInputStream @0x2CA5C0, vptr at +0x30); subclasses supply Fill(). */
#ifndef BIS_H
#define BIS_H
#include "common.h"

/* The readers take these by pointer only: declared, not defined, so that a TU can bring bis.h together with
   sly2/basic.h (whose VECTOR is not mat.h's 16-byte one). bis.cpp includes mat.h itself. */
struct VECTOR; struct VECTOR4; struct MATRIX3; struct MATRIX4;
struct GEOM;

class CBinaryInputStream
{
public:
    /* 0x00 */ int m_grfbis;       /* bit 2: stream is compressed */
    /* 0x04 */ byte *m_pb;         /* current read position */
    /* 0x08 */ int m_cb;           /* bytes left in the current buffer (-1 = hit end) */
    /* 0x0c */ int m_cbConsumed;   /* running total of bytes made available */
    /* 0x10 */ byte *m_pbRaw;      /* next raw block from Fill() */
    /* 0x14 */ int m_cbRaw;
    /* 0x18 */ int m_grfDecomp;    /* LZSS flag bits still pending (0xFF00 sentinel) */
    /* 0x1c */ int m_cbPending;    /* decompressed bytes past the 8K window, carried to next call */
    /* 0x20 */ int m_unk20;
    /* 0x24 */ byte *m_pbDecomp;   /* 2 x 8K output window */
    /* 0x28 */ int m_unk28;
    /* 0x2c */ int m_unk2C;
    /* 0x30    vptr */

    CBinaryInputStream();
    virtual void Fill() = 0;       /* slot 1 */
    virtual void Close();          /* slot 2 */

    void Refill();
    int CbRead(int cb, void *pv);
    void Read(int cb, void *pv);
    void Align(int n);
    void Skip(int cb);
    void SetDecompress(int n, byte *pb);
    void Decompress();
    uchar U8Read();
    ushort U16Read();
    uint U32Read();
    char S8Read();
    short S16Read();
    int S32Read();
    float F32Read();
    void ReadVector(VECTOR *pvec);
    void ReadMatrix(MATRIX3 *pmat);
    void ReadMatrix4(MATRIX4 *pmat);
    void ReadVector4(VECTOR4 *pvec);
    void ReadCb12(void *pv);
    void ReadCb8(void *pv);
    void ReadGeom(GEOM *pgeom);    /* 0x117FD0 */
};

extern "C" void CopyAb(void *pvDst, void *pvSrc, uint cb);
extern "C" void *memcpy(void *dst, const void *src, uint cb);
#endif
